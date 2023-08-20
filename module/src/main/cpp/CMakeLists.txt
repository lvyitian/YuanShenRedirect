cmake_minimum_required(VERSION 3.18.1)

enable_language(ASM)

if (NOT DEFINED MODULE_NAME)
    message(FATAL_ERROR "MODULE_NAME is not set")
else ()
    project(${MODULE_NAME})
endif ()

message("Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_STANDARD 20)

set(LINKER_FLAGS "-ffixed-x18 -Wl,--hash-style=both")
set(C_FLAGS "-Werror=format -fdata-sections -ffunction-sections")
set(CXX_FLAGS "${CXX_FLAGS} -fno-exceptions -fno-rtti")

if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(C_FLAGS "${C_FLAGS} -O2 -fvisibility=hidden -fvisibility-inlines-hidden")
    set(LINKER_FLAGS "${LINKER_FLAGS} -Wl,-exclude-libs,ALL -Wl,--gc-sections -Wl,--strip-all")
else ()
    set(C_FLAGS "${C_FLAGS} -O0")
endif ()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${C_FLAGS} ${CXX_FLAGS}")

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${LINKER_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${LINKER_FLAGS}")

include_directories(
        xdl/include
        bhook
        bhook/include
        bhook/third_party/bsd
        bhook/third_party/lss
)

aux_source_directory(xdl xdl-src)
aux_source_directory(bhook bhook-src)

add_library(${MODULE_NAME} SHARED
        main.cpp
        hack.cpp
        il2cpp_dump.cpp
        open_hook.c
        ${xdl-src}
        ${bhook-src})
target_link_libraries(${MODULE_NAME} log)

if(USEASAN)
    target_compile_options(${MODULE_NAME} PUBLIC -fsanitize=address -fno-omit-frame-pointer)
    target_link_options(${MODULE_NAME} PUBLIC -fsanitize=address)
else()
    target_compile_options(${MODULE_NAME} PUBLIC -Oz -flto -faddrsig -ffunction-sections -fdata-sections)
    target_link_options(${MODULE_NAME} PUBLIC -Oz -flto -Wl,--icf=all -Wl,--exclude-libs,ALL -Wl,--gc-sections -Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/bhook/bytehook.map.txt)
endif()


if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_custom_command(TARGET ${MODULE_NAME} POST_BUILD
            COMMAND ${CMAKE_STRIP} --strip-all --remove-section=.comment "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/lib${MODULE_NAME}.so")
endif ()
