#include <iostream>
extern "C" {
#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
}

#include "LuaBankAccount.hpp"
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

int main(int argc, const char* argv[]) {
  lua_State* L = luaL_newstate();
  luaL_openlibs(L);
  luaopen_BankAccount(L);
  int failures = testPushLuaInteger(L);
  if (luaL_dofile(L, kTestFile)) std::cout << lua_tostring(L, -1) << std::endl;
  lua_close(L);
  return failures == 0 ? 0 : 1;
}
