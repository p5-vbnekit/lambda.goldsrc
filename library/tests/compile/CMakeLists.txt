cmake_minimum_required(VERSION 3.5)

include("${PROJECT_SOURCE_DIR}/extra/lib/cmake/auxiliary.cmake")

cmake_language(CALL "_auxiliary::get_target_namespace" "_namespaces.this")
mark_as_advanced("_namespaces.this")

cmake_language(CALL "_auxiliary::get_target_namespace" "_namespaces.related" DIRECTORY "../..")
mark_as_advanced("_namespaces.related")

add_executable("${_namespaces.this}" "src/main.cxx")

function("_inject_sources")
    unset("_include_path")
    get_target_property("_include_path" "${_namespaces.related}" "SOURCE_DIR")
    if("" STREQUAL "${_include_path}")
        message(FATAL_ERROR "_include_path is empty")
    endif("" STREQUAL "${_include_path}")
    set("_include_path" "${_include_path}/include")
    unset("_headers")
    file(
        GLOB_RECURSE "_headers"
        LIST_DIRECTORIES FALSE
        "${_include_path}/*.h*"
    )
    unset("_sources")
    foreach("_path" IN LISTS "_headers")
        if("" STREQUAL "${_path}")
            message(FATAL_ERROR "_header is empty")
        endif("" STREQUAL "${_path}")
        file(RELATIVE_PATH "_path" "${_include_path}" "${_path}")
        if("" STREQUAL "${_path}")
            message(FATAL_ERROR "_path is empty")
        endif("" STREQUAL "${_path}")
        configure_file("src/template.cxx.in" "src/${_path}.cxx" @ONLY)
        list(APPEND "_sources" "${CMAKE_CURRENT_BINARY_DIR}/src/${_path}.cxx")
    endforeach("_path" IN LISTS "_headers")
    target_sources("${_namespaces.this}" PRIVATE "${_sources}")
endfunction("_inject_sources")

cmake_language(CALL "_inject_sources")

target_compile_features("${_namespaces.this}" PRIVATE "cxx_std_20")

set_target_properties("${_namespaces.this}" PROPERTIES
    OUTPUT_NAME "compile"
    CXX_STANDARD "20"
    CXX_STANDARD_REQUIRED TRUE
    POSITION_INDEPENDENT_CODE TRUE
)

target_link_libraries(
    "${_namespaces.this}" PRIVATE
    "${_namespaces.related}"
)

add_test(NAME "${_namespaces.this}" COMMAND "compile")
