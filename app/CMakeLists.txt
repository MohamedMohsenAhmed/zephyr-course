# SPDX-License-Identifier: Apache-2.0
################################################################################
cmake_minimum_required(VERSION 3.20.0)
################################################################################
list(APPEND BOARD_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)
list(APPEND ZEPHYR_EXTRA_MODULES ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/OurDriver)
################################################################################
# BEFORE find_package
message(STATUS "=== BEFORE find_package ===")
message(STATUS "DTS_ROOT before: ${DTS_ROOT}")	
################################################################################
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
################################################################################
# AFTER find_package
message(STATUS "=== AFTER find_package ===")
message(STATUS "APPLICATION_SOURCE_DIR : ${APPLICATION_SOURCE_DIR}")
message(STATUS "ZEPHYR_BASE            : ${ZEPHYR_BASE}")
message(STATUS "DTS_ROOT               : ${DTS_ROOT}")
message(STATUS "ZEPHYR_MODULES         : ${ZEPHYR_MODULES}")
message(STATUS "ZEPHYR_EXTRA_MODULES   : ${ZEPHYR_EXTRA_MODULES}")
message(STATUS "=== END ===")
project(led_strip)
################################################################################
FILE(GLOB app_sources src/*.c)
target_sources(app PRIVATE ${app_sources})
################################################################################
