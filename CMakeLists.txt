cmake_minimum_required(VERSION 3.2)
project(fswrapperwut)
include("${WUT_ROOT}/share/wut.cmake" REQUIRED)

file(GLOB_RECURSE SOURCE_FILES *.c *.cpp)
file(GLOB_RECURSE HEADER_FILES *.h)

add_library(fswrapperwut STATIC ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(fswrapperwut
   utilswut)

target_include_directories(fswrapperwut PUBLIC "include")
target_include_directories(fswrapperwut PRIVATE "source")

include_directories("${WUT_ROOT}/include/libutilswut" REQUIRED)

wut_enable_stdcpp(fswrapperwut)
wut_default_malloc(fswrapperwut)

target_include_directories(fswrapperwut PUBLIC "include")
target_compile_options(fswrapperwut PUBLIC "-D__LOGGING__")

install(TARGETS fswrapperwut
        ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib")
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
        DESTINATION "${CMAKE_INSTALL_PREFIX}/include"
        FILES_MATCHING PATTERN "*.h*")