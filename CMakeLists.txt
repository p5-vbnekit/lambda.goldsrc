cmake_minimum_required(VERSION 3.5)

project("p5-lambda" LANGUAGES "CXX")

include("kconfig.cmake")
cmake_language(CALL "_kconfig::enable")
mark_as_advanced("_kconfig")

unset("_kconfig")
get_directory_property("_kconfig" "_kconfig::TESTS")
if("y" STREQUAL "${_kconfig}")
    include("CTest")
endif("y" STREQUAL "${_kconfig}")

add_subdirectory("library")

unset("_kconfig")
get_directory_property("_kconfig" "_kconfig::HLDS_LAUNCHER")
if("y" STREQUAL "${_kconfig}")
    add_subdirectory("extra/utils/hlds_launcher")
endif("y" STREQUAL "${_kconfig}")

unset("_kconfig")
get_directory_property("_kconfig" "_kconfig::METAMOD_PLUGIN")
if("" STREQUAL "${_kconfig}")
    if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
        set("_kconfig" "y")
    endif("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
endif("" STREQUAL "${_kconfig}")
if("y" STREQUAL "${_kconfig}")
    add_subdirectory("metamod_plugin")
endif("y" STREQUAL "${_kconfig}")
