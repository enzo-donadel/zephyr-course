cmake_minimum_required(VERSION 3.20.0)

list(APPEND BOARD_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
list(APPEND DTS_ROOT   ${CMAKE_CURRENT_SOURCE_DIR})
find_package(Zephyr REQUIRED)
project(my_zephyr_app)

set(SUBSYSTEM_SOURCES "")
if(CONFIG_LED_SUBSYSTEM)
list(APPEND SUBSYSTEM_SOURCES
    subsys/LED/led_subsystem.c
    subsys/Heartbeat/heartbeat.c
)
endif()

if(CONFIG_LED_SENSOR)
list(APPEND SUBSYSTEM_SOURCES
    drivers/sensor/led_sensor/led_sensor.c
    src/shell_commands.c
)
endif()

target_sources(app PRIVATE
    src/main.c
    ${SUBSYSTEM_SOURCES}
)

target_include_directories(app PRIVATE
    include/
    subsys/
    drivers/sensor/led_sensor/
)
