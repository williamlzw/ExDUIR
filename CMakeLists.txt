cmake_minimum_required(VERSION 3.17)
project(ExduiR)

add_definitions(-DUNICODE)
add_definitions(-D_WINDOWS)

set(CMAKE_CXX_STANDARD 14)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/source-charset:UTF-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:UTF-8>")

include_directories(include)

file(GLOB LIBSRCS "src/*.cpp" "src/*.h")

add_library(exdui
        ${LIBSRCS}
        )

ADD_LIBRARY (libexdui SHARED
        msvc/dll/exports.def
        msvc/dll/Resource.rc
        )
target_link_libraries(libexdui PRIVATE exdui)
add_executable(test WIN32
        msvc/test/ExduiR.cpp
        msvc/test/test_obj.cpp
        msvc/test/test_obj.h
        msvc/test/Resource.h
        msvc/test/icon.ico
        )
target_link_libraries(test PRIVATE exdui)