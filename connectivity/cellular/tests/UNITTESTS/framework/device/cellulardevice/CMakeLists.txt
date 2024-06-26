# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(GoogleTest)

set(TEST_NAME cellular-framework-device-cellular-device-unittest)

add_executable(${TEST_NAME})

target_compile_definitions(${TEST_NAME}
    PRIVATE
        MDMRTS=PTC0
        MDMCTS=PTC1
        MDMTXD=NC
        MDMRXD=NC
        CELLULAR_DEVICE=myCellularDevice
        DEVICE_SERIAL_FC=1
        DEVICE_SERIAL=1
        DEVICE_INTERRUPTIN=1
        MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE=115200
        MBED_CONF_CELLULAR_AT_HANDLER_BUFFER_SIZE=32
        MBED_CONF_CELLULAR_EVENT_QUEUE_SIZE=10
)

target_sources(${TEST_NAME}
    PRIVATE
        ${mbed-os_SOURCE_DIR}/connectivity/cellular/source/framework/device/CellularDevice.cpp
        cellulardevicetest.cpp
)

target_link_libraries(${TEST_NAME}
    PRIVATE
        mbed-headers-platform
        mbed-headers-events
        mbed-headers-rtos
        mbed-headers-drivers
        mbed-headers-hal
        mbed-headers-netsocket
        mbed-headers-cellular
        mbed-stubs-cellular
        mbed-stubs-platform
        mbed-stubs-events
        gmock_main
)

gtest_discover_tests(${TEST_NAME} PROPERTIES LABELS "cellular")
