cmake_minimum_required(VERSION 3.13)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(tm1637_project)  

pico_sdk_init()

add_executable(main
    main.c     
    tm1637.c    
)


target_link_libraries(main
    pico_stdlib
    hardware_gpio   
)

pico_enable_stdio_usb(main 1)
pico_enable_stdio_uart(main 0)

pico_add_extra_outputs(main)
