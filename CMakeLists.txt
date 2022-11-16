cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    luawrapper
    VERSION 0.0.1
    DESCRIPTION "A library to make writing C++ to Lua bindings easier."
    HOMEPAGE_URL "https://github.com/alexames/luawrapper"
    LANGUAGES NONE
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

add_library(luawrapper_luawrapper INTERFACE)
add_library(luawrapper::luawrapper ALIAS luawrapper_luawrapper)

set_property(
    TARGET luawrapper_luawrapper PROPERTY
    EXPORT_NAME luawrapper
)

target_include_directories(
    luawrapper_luawrapper ${warning_guard}
    INTERFACE
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

if(NOT luawrapper_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of luawrapper"
  )
endif()

include(cmake/dev-mode.cmake)
