cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED)
project(my_zephyr_app)

set(SUBSYSTEM_SOURCES "")
if(CONFIG_LED_SUBSYSTEM)
list(APPEND SUBSYSTEM_SOURCES
    subsys/LED/led_subsystem.c
    subsys/Heartbeat/heartbeat.c
)
endif()

target_sources(app PRIVATE 
    src/main.c
    ${SUBSYSTEM_SOURCES}
)
target_include_directories(app PRIVATE 
    include/
    subsys/
)
