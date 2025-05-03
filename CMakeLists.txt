cmake_minimum_required(VERSION 3.13)

set(PICOTOOL_FETCH_FROM_GIT_PATH "/home/dev/picotool/bin")
#set(PICO_NO_PICOTOOL 1)

set(PICO_SDK_PATH "/home/dev/rp2040/pico-sdk")
set(PICO_BOARD pico2_w)
set(PICO_CYW43_ARCH_THREADSAFE_BACKGROUND 1)


include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(main C CXX ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)



pico_sdk_init()


file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/*.c
)
file(GLOB_RECURSE EXTERNAL_INCLUDE_DIRS
    "external/*"
)



add_executable(main
        ${SOURCES}
)

target_include_directories(main PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/source
)

foreach(dir ${EXTERNAL_INCLUDE_DIRS})
    if(IS_DIRECTORY ${dir})
        target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${dir})
    endif()
endforeach()

pico_enable_stdio_usb(main OFF)
pico_enable_stdio_uart(main ON)


# pull in common dependencies
target_link_libraries(main

        pico_stdlib
        pico_cyw43_arch_lwip_threadsafe_background
        pico_lwip_sntp
        pico_lwip_mqtt
        pico_mbedtls
        pico_lwip_mbedtls
)

pico_add_extra_outputs(main)