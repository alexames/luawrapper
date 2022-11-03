#include <iostream>
extern "C" {
#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
}

#include "LuaBankAccount.hpp"

const char kTestFile[] = "example1.lua";

int main(int argc, const char* argv[]) {
  lua_State* L = luaL_newstate();
  luaL_openlibs(L);
  luaopen_BankAccount(L);
  if (luaL_dofile(L, kTestFile)) std::cout << lua_tostring(L, -1) << std::endl;
  lua_close(L);
  return 0;
}
