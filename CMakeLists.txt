cmake_minimum_required(VERSION 3.15.3)
project(LuaWrapper VERSION 1.0.0)

option(LUAWRAPPER_BUILD_TESTS "Build tests" ON)
option(LUAWRAPPER_TEST_5_0 "Build tests for lua 5.0" ON)
option(LUAWRAPPER_TEST_5_1 "Build tests for lua 5.1" ON)
option(LUAWRAPPER_TEST_5_2 "Build tests for lua 5.2" ON)
option(LUAWRAPPER_TEST_5_3 "Build tests for lua 5.3" ON)
option(LUAWRAPPER_TEST_5_4 "Build tests for lua 5.4" ON)

add_library(luawrapper INTERFACE)
target_include_directories(luawrapper INTERFACE "${CMAKE_CURRENT_LIST_DIR}/include")

if (LUAWRAPPER_BUILD_TESTS)
  if (LUAWRAPPER_TEST_5_0)
    list(APPEND versions "5.0")
  endif()
  if (LUAWRAPPER_TEST_5_1)
    list(APPEND versions "5.1.1")
  endif()
  if (LUAWRAPPER_TEST_5_2)
    list(APPEND versions "5.2.3")
  endif()
  if (LUAWRAPPER_TEST_5_3)
    list(APPEND versions "5.3.6")
  endif()
  if (LUAWRAPPER_TEST_5_4)
    list(APPEND versions "5.4.4")
  endif()

  foreach(version ${versions})
    include(FetchContent)
    set(lua_name "lua-${version}")
    set(luawrapper_test_name "luawrapper_test-${version}")

    # Fetch and build the specified version of Lua.
    FetchContent_Declare(
      "${lua_name}"
      GIT_REPOSITORY    "git@github.com:lua/lua.git"
      GIT_TAG           "v${version}"
    )
    FetchContent_MakeAvailable("${lua_name}")
    set(LUA_VERSION ${version})
    set(LUA_SOURCE_DIR "${${lua_name}_SOURCE_DIR}")
    add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/cmake/lua" "${CMAKE_CURRENT_BINARY_DIR}/cmake/${lua_name}")

    # Build the tests.
    set(TEST_MAIN_SOURCE
      "${CMAKE_CURRENT_LIST_DIR}/tests/main.cpp")
    set(TEST_LIBRARY_SOURCES
      "${CMAKE_CURRENT_LIST_DIR}/tests/BankAccount.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/tests/BankAccount.hpp")
    set(TEST_LUA_LIBRARY_SOURCES
      "${CMAKE_CURRENT_LIST_DIR}/tests/LuaBankAccount.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/tests/LuaBankAccount.hpp")
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
        "${CMAKE_CURRENT_LIST_DIR}/include"
        "${CMAKE_CURRENT_LIST_DIR}/tests"
    )
    set_target_properties(
      "${luawrapper_test_name}"
      PROPERTIES
        FOLDER LuaWrapper
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/tests")

    target_link_libraries("${luawrapper_test_name}" PRIVATE "${lua_name}")
  endforeach()
endif()

