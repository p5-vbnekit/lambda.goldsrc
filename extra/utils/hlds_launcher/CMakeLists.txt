cmake_minimum_required(VERSION 3.5)

include("${PROJECT_SOURCE_DIR}/extra/lib/cmake/auxiliary.cmake")

find_package("Boost" REQUIRED COMPONENTS "headers" CONFIG)

cmake_language(CALL "_auxiliary::get_target_namespace" "_target")
mark_as_advanced("_target")

add_executable("${_target}" "src/hlds.cxx")

target_compile_features("${_target}" PRIVATE "cxx_std_20")

set_target_properties("${_target}" PROPERTIES
    OUTPUT_NAME "hlds"
    CXX_STANDARD "20"
    CXX_STANDARD_REQUIRED TRUE
    POSITION_INDEPENDENT_CODE TRUE
)

target_link_libraries("${_target}" PRIVATE "Boost::boost")
