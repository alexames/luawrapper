#include <iostream>
extern "C" {
#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
}

#include "LuaBankAccount.hpp"
#include "luawrapper.hpp"
#include "luawrapperutil.hpp"

const char kTestFile[] = "example1.lua";

// Regression test: luaU_push must accept `long long` / `unsigned long long` /
// `lua_Integer` unambiguously. On LP64 targets where int64_t is `long` (not
// `long long`), a per-fixed-width-type overload set produces an ambiguous call;
// the constrained-template overload in luawrapperutil.hpp must resolve it.
static int testPushLuaInteger(lua_State* L) {
  const long long kSigned = (1LL << 50) + 42;
  const unsigned long long kUnsigned = (1ULL << 50) + 7;
  const lua_Integer kLuaInt = static_cast<lua_Integer>((1LL << 48) + 3);

  luaU_push(L, kSigned);
  luaU_push(L, kUnsigned);
  luaU_push(L, kLuaInt);

  int failures = 0;
  if (lua_tointeger(L, -3) != static_cast<lua_Integer>(kSigned)) {
    std::cout << "FAIL: long long round-trip\n";
    ++failures;
  }
  if (lua_tointeger(L, -2) != static_cast<lua_Integer>(kUnsigned)) {
    std::cout << "FAIL: unsigned long long round-trip\n";
    ++failures;
  }
  if (lua_tointeger(L, -1) != kLuaInt) {
    std::cout << "FAIL: lua_Integer round-trip\n";
    ++failures;
  }
  lua_pop(L, 3);
  if (failures == 0) std::cout << "PASS: luaU_push lua_Integer/long long\n";
  return failures;
}

// Counts destructions of the type below, so a collection that did not happen
// is visible from C++.
static int s_collectedCount = 0;

class Collectable {
 public:
  ~Collectable() { ++s_collectedCount; }
};

// Regression test: luaW_setfuncs must give each class's cache table the weak
// metatable stored under LUAW_CACHE_METATABLE_KEY. That key holds one shared
// table rather than a per-class one, so reaching it through luaW_wrapperfield
// -- which indexes LuaWrapper[field][classname] -- yields nil, and setting nil
// as a metatable leaves the cache with strong values. The cache then keeps
// every wrapped userdata alive: an object Lua owns is never collected and its
// deallocator never runs.
static int testHeldObjectIsCollected(lua_State* L) {
  luaW_setfuncs<Collectable>(L, "Collectable", NULL, NULL);
  lua_pop(L, 1);

  int failures = 0;

  // Pushed AND held: the push is what creates the userdata and caches it, and
  // the hold is what makes its collection free the object.
  Collectable* obj = new Collectable();
  luaW_push<Collectable>(L, obj);
  luaW_hold<Collectable>(L, obj);

  // Weak values must not cost the cache its job. A second push of the same
  // live pointer has to find the first userdata rather than make a second one
  // for the same object.
  luaW_push<Collectable>(L, obj);
  if (!lua_rawequal(L, -1, -2)) {
    std::cout << "FAIL: pushing one object twice made two userdata\n";
    ++failures;
  }
  lua_pop(L, 2);  // The cache is now the only thing referring to it.

  lua_gc(L, LUA_GCCOLLECT, 0);
  if (s_collectedCount != 1) {
    std::cout << "FAIL: an object Lua holds was not collected\n";
    ++failures;
  }

  if (failures == 0) std::cout << "PASS: an object Lua holds is cached and collected\n";
  return failures;
}

class Wrapped {};

// Pushes a full userdata that LuaWrapper did not make: its metatable is a plain
// table whose extends field, if set, is `extends` rather than a table.
static void pushForeignUserdata(lua_State* L, bool setExtends) {
  lua_newuserdata(L, 1);  // ... ud
  lua_newtable(L);        // ... ud mt
  if (setExtends) {
    lua_pushinteger(L, 1);
    lua_setfield(L, -2, LUAW_EXTENDS_KEY);
  }
  lua_setmetatable(L, -2);  // ... ud
}

// Regression test: luaW_is must answer false for a full userdata whose
// metatable is not one of LuaWrapper's, such as an io file or another
// binding's type. Such a metatable has no extends table, and walking the
// missing table with lua_next is undefined behaviour -- in practice a crash.
static int testForeignUserdataIsNotAType(lua_State* L) {
  luaW_setfuncs<Wrapped>(L, "Wrapped", NULL, NULL);
  lua_pop(L, 1);

  int failures = 0;
  for (bool setExtends : {false, true}) {
    const char* which = setExtends ? "a non-table extends" : "no extends";
    pushForeignUserdata(L, setExtends);
    const int top = lua_gettop(L);
    if (luaW_is<Wrapped>(L, -1)) {
      std::cout << "FAIL: luaW_is accepted a foreign userdata with " << which << "\n";
      ++failures;
    }
    if (luaW_to<Wrapped>(L, -1) != NULL) {
      std::cout << "FAIL: luaW_to converted a foreign userdata with " << which << "\n";
      ++failures;
    }
    if (lua_gettop(L) != top) {
      std::cout << "FAIL: luaW_is left the stack unbalanced for " << which << "\n";
      ++failures;
    }
    lua_pop(L, 1);
  }

  if (failures == 0) std::cout << "PASS: a foreign userdata is not a LuaWrapper type\n";
  return failures;
}

int main(int argc, const char* argv[]) {
  lua_State* L = luaL_newstate();
  luaL_openlibs(L);
  luaopen_BankAccount(L);
  int failures = testPushLuaInteger(L);
  failures += testHeldObjectIsCollected(L);
  failures += testForeignUserdataIsNotAType(L);
  if (luaL_dofile(L, kTestFile)) std::cout << lua_tostring(L, -1) << std::endl;
  lua_close(L);
  return failures == 0 ? 0 : 1;
}
