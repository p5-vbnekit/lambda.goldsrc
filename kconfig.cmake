cmake_minimum_required(VERSION 3.19)

function("_kconfig::enable")
    unset("_defaults.TESTS")
    unset("_defaults.HLDS_LAUNCHER")
    unset("_defaults.METAMOD_PLUGIN")
    if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_FUNCTION_LIST_DIR}")
        set("_defaults.TESTS" "y")
        set("_defaults.METAMOD_PLUGIN" "y")
    else()
        set("_defaults.TESTS" "n")
        set("_defaults.METAMOD_PLUGIN" "n")
    endif("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_FUNCTION_LIST_DIR}")
    set("_defaults.HLDS_LAUNCHER" "n")
    configure_file("KConfig.in" ".kconfig/KConfig" @ONLY)
    include("${CMAKE_CURRENT_FUNCTION_LIST_DIR}/extra/lib/cmake/auxiliary.cmake")
    unset("_json")
    cmake_language(CALL "_auxiliary::kconfig" "_json")
    if("" STREQUAL "${_json}")
        return()
    endif("" STREQUAL "${_json}")
    unset("_length")
    string(JSON "_length" LENGTH "${_json}")
    if("" STREQUAL "${_length}")
        message(FATAL_ERROR "json length is empty")
    endif("" STREQUAL "${_length}")
    if(NOT "0" LESS "${_length}")
        return()
    endif(NOT "0" LESS "${_length}")
    unset("_last")
    math(EXPR "_last" "${_length} - 1")
    unset("_key")
    foreach("_key" RANGE "${_last}")
        unset("_value")
        string(JSON "_value" MEMBER "${_json}" ${_key})
        if("" STREQUAL "${_value}")
            message(FATAL_ERROR "key #${_key} is empty")
        endif("" STREQUAL "${_value}")
        set("_key" "${_value}")
        string(JSON "_value" GET "${_json}" "${_key}")
        set_property(
            DIRECTORY "${CMAKE_CURRENT_FUNCTION_LIST_DIR}"
            PROPERTY "_kconfig::${_key}" "${_value}"
        )
    endforeach("_key" RANGE "${_last}")
endfunction("_kconfig::enable")
