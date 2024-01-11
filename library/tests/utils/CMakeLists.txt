cmake_minimum_required(VERSION 3.10)

include("${PROJECT_SOURCE_DIR}/extra/lib/cmake/auxiliary.cmake")

find_package("Boost" REQUIRED COMPONENTS "headers" "unit_test_framework" CONFIG)

cmake_language(CALL "_auxiliary::get_target_namespace" "_namespaces.this")
mark_as_advanced("_namespaces.this")

cmake_language(CALL "_auxiliary::get_target_namespace" "_namespaces.related" DIRECTORY "../..")
mark_as_advanced("_namespaces.related")

add_executable("${_namespaces.this}.log" "src/log.cxx")
add_executable("${_namespaces.this}.async" "src/async.cxx")
add_executable("${_namespaces.this}.exception" "src/exception.cxx")
add_executable("${_namespaces.this}.generator" "src/generator.cxx")
add_executable("${_namespaces.this}.container" "src/container.cxx")

target_compile_features("${_namespaces.this}.log" PRIVATE "cxx_std_20")
target_compile_features("${_namespaces.this}.async" PRIVATE "cxx_std_20")
target_compile_features("${_namespaces.this}.exception" PRIVATE "cxx_std_20")
target_compile_features("${_namespaces.this}.generator" PRIVATE "cxx_std_20")
target_compile_features("${_namespaces.this}.container" PRIVATE "cxx_std_20")

set_target_properties("${_namespaces.this}.log" PROPERTIES
    OUTPUT_NAME "log"
    CXX_STANDARD "20"
    CXX_STANDARD_REQUIRED TRUE
    POSITION_INDEPENDENT_CODE TRUE
)

set_target_properties("${_namespaces.this}.async" PROPERTIES
    OUTPUT_NAME "async"
    CXX_STANDARD "20"
    CXX_STANDARD_REQUIRED TRUE
    POSITION_INDEPENDENT_CODE TRUE
)

set_target_properties("${_namespaces.this}.exception" PROPERTIES
    OUTPUT_NAME "exception"
    CXX_STANDARD "20"
    CXX_STANDARD_REQUIRED TRUE
    POSITION_INDEPENDENT_CODE TRUE
)

set_target_properties("${_namespaces.this}.generator" PROPERTIES
    OUTPUT_NAME "generator"
    CXX_STANDARD "20"
    CXX_STANDARD_REQUIRED TRUE
    POSITION_INDEPENDENT_CODE TRUE
)

set_target_properties("${_namespaces.this}.container" PROPERTIES
    OUTPUT_NAME "container"
    CXX_STANDARD "20"
    CXX_STANDARD_REQUIRED TRUE
    POSITION_INDEPENDENT_CODE TRUE
)

target_link_libraries(
    "${_namespaces.this}.log" PRIVATE
    "Boost::boost" "Boost::unit_test_framework"
    "${_namespaces.related}"
)

target_link_libraries(
    "${_namespaces.this}.async" PRIVATE
    "Boost::boost" "Boost::unit_test_framework"
    "${_namespaces.related}"
)

target_link_libraries(
    "${_namespaces.this}.exception" PRIVATE
    "Boost::boost" "Boost::unit_test_framework"
    "${_namespaces.related}"
)

target_link_libraries(
    "${_namespaces.this}.generator" PRIVATE
    "Boost::boost" "Boost::unit_test_framework"
    "${_namespaces.related}"
)

target_link_libraries(
    "${_namespaces.this}.container" PRIVATE
    "Boost::boost" "Boost::unit_test_framework"
    "${_namespaces.related}"
)

target_compile_definitions(
    "${_namespaces.this}.log" PRIVATE
    "P5_LAMBDA__BASE_PATH=\"${PROJECT_SOURCE_DIR}\""
)

add_test(NAME "${_namespaces.this}.log" COMMAND "log")
add_test(NAME "${_namespaces.this}.async" COMMAND "async")
add_test(NAME "${_namespaces.this}.exception" COMMAND "exception")
add_test(NAME "${_namespaces.this}.generator" COMMAND "generator")
add_test(NAME "${_namespaces.this}.container" COMMAND "container")
