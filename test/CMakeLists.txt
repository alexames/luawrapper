option(LUAWRAPPER_BUILD_TESTS "Build tests" ON)
option(LUAWRAPPER_TEST_5_1 "Build tests for lua 5.1" ON)

# The top-level project is declared with LANGUAGES NONE since luawrapper
# itself is header-only; enable C and C++ here for the Lua builds and tests.
enable_language(C)
enable_language(CXX)
option(LUAWRAPPER_TEST_5_2 "Build tests for lua 5.2" ON)
option(LUAWRAPPER_TEST_5_3 "Build tests for lua 5.3" ON)
option(LUAWRAPPER_TEST_5_4 "Build tests for lua 5.4" ON)
option(LUAWRAPPER_TEST_5_5 "Build tests for lua 5.5" ON)

set(lua_versions "")
set(lua_branches "")

if (LUAWRAPPER_TEST_5_1)
  list(APPEND lua_versions "5.1.1")
  list(APPEND lua_branches "extensions/5.1/master")
endif()
if (LUAWRAPPER_TEST_5_2)
  list(APPEND lua_versions "5.2.4")
  list(APPEND lua_branches "extensions/5.2/master")
endif()
if (LUAWRAPPER_TEST_5_3)
  list(APPEND lua_versions "5.3.6")
  list(APPEND lua_branches "extensions/5.3/master")
endif()
if (LUAWRAPPER_TEST_5_4)
  list(APPEND lua_versions "5.4.8")
  list(APPEND lua_branches "extensions/5.4/master")
endif()
if (LUAWRAPPER_TEST_5_5)
  list(APPEND lua_versions "5.5.0")
  list(APPEND lua_branches "extensions/5.5/master")
endif()

include(FetchContent)

list(LENGTH lua_versions version_count)
math(EXPR version_last "${version_count} - 1")

foreach(i RANGE ${version_last})
  list(GET lua_versions ${i} version)
  list(GET lua_branches ${i} branch)

  set(lua_name "lua-${version}")
  set(luawrapper_test_name "luawrapper_test-${version}")

  # Fetch the specified version of Lua's source. We only want the sources;
  # the fetched repo's own CMakeLists.txt caches a project name that collides
  # across versions, so use FetchContent_Populate to skip its add_subdirectory
  # and then build with our universal Lua CMakeLists at ../cmake/lua.
  FetchContent_Declare(
    "${lua_name}"
    GIT_REPOSITORY    "https://github.com/alexames/lua.git"
    GIT_TAG           "${branch}"
  )
  FetchContent_GetProperties("${lua_name}")
  if(NOT ${lua_name}_POPULATED)
    FetchContent_Populate("${lua_name}")
  endif()
  set(LUA_VERSION ${version})
  set(LUA_SOURCE_DIR "${${lua_name}_SOURCE_DIR}")
  add_subdirectory("../cmake/lua" "../cmake/${lua_name}")

  # Build the tests.
  set(TEST_MAIN_SOURCE
    "main.cpp")
  set(TEST_LIBRARY_SOURCES
    "BankAccount.cpp"
    "BankAccount.hpp"
    "Example.cpp"
    "Example.hpp")
  set(TEST_LUA_LIBRARY_SOURCES
    "LuaBankAccount.cpp"
    "LuaBankAccount.hpp"
    "LuaExample.cpp"
    "LuaExample.hpp"
    "../include/luawrapper.hpp"
    "../include/luawrapperutil.hpp")
  source_group("Main" FILES ${TEST_MAIN_SOURCE})
  source_group("Cpp Libraries" FILES ${TEST_LIBRARY_SOURCES})
  source_group("Lua Wrappers" FILES ${TEST_LUA_LIBRARY_SOURCES})
  add_executable(
    "${luawrapper_test_name}"
    ${TEST_MAIN_SOURCE}
    ${TEST_LIBRARY_SOURCES}
    ${TEST_LUA_LIBRARY_SOURCES})
  target_include_directories(
    "${luawrapper_test_name}"
    PRIVATE
      "../include"
      "../test"
  )
  set_target_properties(
    "${luawrapper_test_name}"
    PROPERTIES
      FOLDER LuaWrapper
      VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}")

  target_link_libraries("${luawrapper_test_name}" PRIVATE "${lua_name}")

  add_test(
    NAME "${luawrapper_test_name}"
    COMMAND "${luawrapper_test_name}"
    WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
  )
endforeach()
