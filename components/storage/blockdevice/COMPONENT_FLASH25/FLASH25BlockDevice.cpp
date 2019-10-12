/* mbed Microcontroller Library
 * Copyright (c) 2016 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "FLASH25BlockDevice.h"
#include "mbed_atomic.h"

#include <inttypes.h>

using namespace mbed;

/* constants */
#define FLASH25_READ_SIZE        1
#define FLASH25_PROG_SIZE        1
#define FLASH25_TIMEOUT          10000
//#define FLASH25_ID_MATCH         0x1F20
//#define FLASH25_ID_DENSITY_MASK  0x001F
#define FLASH25_PAGE_SIZE_256    0x0100
//#define FLASH25_PAGE_SIZE_264    0x0108
#define FLASH25_PAGE_SIZE_512    0x0200
//#define FLASH25_PAGE_SIZE_528    0x0210
#define FLASH25_BLOCK_SIZE_2K    0x0800
//#define FLASH25_BLOCK_SIZE_2K1   0x0840
#define FLASH25_BLOCK_SIZE_4K    0x1000
//#define FLASH25_BLOCK_SIZE_4K1   0x1080
//#define FLASH25_PAGE_BIT_264     9
//#define FLASH25_PAGE_BIT_528     10

/* enable debug */

#ifndef FLASH25_DEBUG
#define FLASH25_DEBUG 0
#endif /* Flash25_DEBUG */

#if FLASH25_DEBUG
#define DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINTF(...)
#endif

void _print_status_flash25(uint16_t status);

//enum manufacturer {
//    FLASH25_MID_ADESTO                     = 0x1F,
//    FLASH25_MID_MICRON                     = 0x20,
//};
/* non-exhaustive opcode list */
enum opcode {
    FLASH25_OP_NOP                         = 0x00,
    FLASH25_OP_WRITE_STATE_REG             = 0x01, //AT25(1/2), M25P(1)
    FLASH25_OP_PROGRAM_DIRECT              = 0x02, //AT25(1-256), M25P(1-256)
    FLASH25_OP_READ_LOW_FREQUENCY          = 0x03, //AT25, M25P
    FLASH25_OP_WRITE_DISABLE               = 0x04, //AT25(0), M25P(0)
    FLASH25_OP_READ_STATE_REG1             = 0x05, //AT25(1), M25P(1-)
    FLASH25_OP_WRITE_ENABLE                = 0x06, //AT25(0), M25P(0)
    FLASH25_OP_READ_HIGH_FREQUENCY         = 0x0B, //AT25, M25P
    FLASH25_OP_ERASE_BLOCK                 = 0x20, //AT25
    FLASH25_OP_READ_STATE_REG2             = 0x35, //AT25(1)
    FLASH25_OP_READ_ID                     = 0x9F, //AT25 (3Bytes), M25P (1-20Bytes)
    FLASH25_OP_ERASE_SECTOR                = 0xD8, //AT25, M25P
    FLASH25_OP_ERASE_CHIP                  = 0xC7, //AT25, M25P
    FLASH25_OP_REL_DEEP_POWER_DOWN         = 0xAB, //AT25, M25P
    FLASH25_OP_ES_REL_DEEP_POWER_DOWN      = 0xAB, //AT25, M25P
    FLASH25_OP_DEEP_POWER_DOWN             = 0xB9, //AT25, M25P
};

/* non-exhaustive command list */
//enum command {
//    FLASH25_COMMAND_WRITE_DISABLE          = 0x3D2A7FA9,
//    FLASH25_COMMAND_WRITE_ENABLE           = 0x3D2A7F9A,
//    FLASH25_COMMAND_BINARY_PAGE_SIZE       = 0x3D2A80A6,
//    FLASH25_COMMAND_DATAFLASH_PAGE_SIZE    = 0x3D2A80A7,
//};

/* bit masks for interpreting the status register */
enum status_bit {
    FLASH25_BIT_SRP0                   = (0x01 << 15),
    FLASH25_BIT_SEC                    = (0x01 << 14),
    FLASH25_BIT_TB                     = (0x01 << 13),
    FLASH25_BIT_BP                     = (0x07 << 10),
    FLASH25_BIT_WEL                    = (0x01 <<  9),
    FLASH25_BIT_RDY                    = (0x01 <<  8),

    FLASH25_BIT_RES2                   = (0x01 <<  7),
    FLASH25_BIT_CMP                    = (0x01 <<  6),
    FLASH25_BIT_LB                     = (0x07 <<  3),
    FLASH25_BIT_RES1                   = (0x01 <<  2),
    FLASH25_BIT_QE                     = (0x01 <<  1),
    FLASH25_BIT_SRP1                   = (0x01 <<  0),
};

/* bit masks for detecting density from status register */
enum status_density {
	FLASH25_STATUS_DENSITY_2_MBIT    = (0x05 << 10),
	FLASH25_STATUS_DENSITY_4_MBIT    = (0x07 << 10),
	FLASH25_STATUS_DENSITY_8_MBIT    = (0x09 << 10),
	FLASH25_STATUS_DENSITY_16_MBIT   = (0x0B << 10),
	FLASH25_STATUS_DENSITY_32_MBIT   = (0x0D << 10),
	FLASH25_STATUS_DENSITY_64_MBIT   = (0x0F << 10),
};

/* code for calculating density */
enum id_density {
	FLASH25_ID_DENSITY_4_MBIT    = 0x04,
	FLASH25_ID_DENSITY_8_MBIT    = 0x05,
	FLASH25_ID_DENSITY_16_MBIT   = 0x06,
	FLASH25_ID_DENSITY_32_MBIT   = 0x07,
};

/* typical duration in milliseconds for each operation */
enum timing {
    FLASH25_TIMING_ERASE_PROGRAM_PAGE     =    17,
	FLASH25_TIMING_PROGRAM_PAGE           =     3,
	FLASH25_TIMING_ERASE_BLOCK            =    45,
	FLASH25_TIMING_ERASE_SECTOR           =   700,
	FLASH25_TIMING_ERASE_CHIP             = 45000
};

/* number of dummy bytes required in each frequency domain */
enum dummy {
    DATAFLASH_LOW_POWER_BYTES         = 0,
    DATAFLASH_LOW_FREQUENCY_BYTES     = 0,
    DATAFLASH_HIGH_FREQUENCY_BYTES    = 1,
    DATAFLASH_HIGHEST_FREQUENCY_BYTES = 2
};

FLASH25BlockDevice::FLASH25BlockDevice(PinName mosi,
                                           PinName miso,
                                           PinName sclk,
                                           PinName cs,
                                           int freq,
                                           PinName nwp)
    :   _spi(mosi, miso, sclk),
        _cs(cs, 1),
        _nwp(nwp),
        _device_size(0),
        _page_size(0),
        _block_size(0),
        _is_initialized(0),
        _init_ref_count(0)
{
	_spi.frequency(freq);

    /* write protect chip if pin is connected */
    if (nwp != NC) {
        _nwp = 0;
    }
}

int FLASH25BlockDevice::init()
{
    _mutex.lock();
    DEBUG_PRINTF("init\r\n");

    if (!_is_initialized) {
        _init_ref_count = 0;
    }

    uint32_t val = core_util_atomic_incr_u32(&_init_ref_count, 1);

    if (val != 1) {
        _mutex.unlock();
        return BD_ERROR_OK;
    }

    int result = BD_ERROR_DEVICE_ERROR;

    /* read ID register to validate model and set dimensions */
    uint16_t id = _get_register(FLASH25_OP_READ_ID);

    DEBUG_PRINTF("id: %04X\r\n", id & MBED_CONF_FLASH25_ID_MATCH);

    /* get status register to verify the page size mode */
    uint8_t status_1 = _get_register(FLASH25_OP_READ_STATE_REG1);

    /* get status register to verify the page size mode */
    uint8_t status_2 = _get_register(FLASH25_OP_READ_STATE_REG2);

    uint16_t status = status_1 << 8 | status_2;

    /* manufacture ID match */
    if (((id >> 8) & MBED_CONF_FLASH25_ID_MATCH) == MBED_CONF_FLASH25_ID_MATCH) {

    	/* calculate density */
    	_device_size = 0x8000 << (id & MBED_CONF_FLASH25_ID_DENSITY_MASK);

    	bool binary_page_size = true;

    	DEBUG_PRINTF("Page size is binary\r\n");

        /* set page program size and block erase size */
        _page_size = FLASH25_PAGE_SIZE_256;
        _block_size = FLASH25_BLOCK_SIZE_4K;

        DEBUG_PRINTF("density: %" PRIu16 "\r\n", id & MBED_CONF_FLASH25_ID_DENSITY_MASK);
        DEBUG_PRINTF("size: %" PRIu32 "\r\n", _device_size);
        DEBUG_PRINTF("page: %" PRIu16 "\r\n", _page_size);
        DEBUG_PRINTF("block: %" PRIu16 "\r\n", _block_size);

        /* device successfully detected, set OK error code */
        result = BD_ERROR_OK;
    }

    /* write protect device when idle */
    _write_enable(false);

    if (result == BD_ERROR_OK) {
        _is_initialized = true;
    }

    _mutex.unlock();
    return result;
}

int FLASH25BlockDevice::deinit()
{
    _mutex.lock();
    DEBUG_PRINTF("deinit\r\n");

    if (!_is_initialized) {
        _init_ref_count = 0;
        _mutex.unlock();
        return BD_ERROR_OK;
    }

    uint32_t val = core_util_atomic_decr_u32(&_init_ref_count, 1);

    if (val) {
        _mutex.unlock();
        return BD_ERROR_OK;
    }

    _is_initialized = false;
    _mutex.unlock();
    return BD_ERROR_OK;
}

int FLASH25BlockDevice::read(void *buffer, bd_addr_t addr, bd_size_t size)
{
    _mutex.lock();
    DEBUG_PRINTF("read: %p %" PRIX64 " %" PRIX64 "\r\n", buffer, addr, size);

    if (!_is_initialized) {
        _mutex.unlock();
        return BD_ERROR_DEVICE_ERROR;
    }

    int result = BD_ERROR_DEVICE_ERROR;

    /* check parameters are valid and the read is within bounds */
    if (is_valid_read(addr, size) && buffer) {

        uint8_t *external_buffer = static_cast<uint8_t *>(buffer);

        /* activate device */
        _cs = 0;

        /* send read opcode */
        _spi.write(FLASH25_OP_READ_LOW_FREQUENCY);

        /* translate address */
        uint32_t address = _translate_address(addr);

        DEBUG_PRINTF("address: %" PRIX32 "\r\n", address);

        /* send read address */
        _spi.write((address >> 16) & 0xFF);
        _spi.write((address >>  8) & 0xFF);
        _spi.write(address & 0xFF);

        /* clock out one byte at a time and store in external buffer */
        for (uint32_t index = 0; index < size; index++) {
            external_buffer[index] = _spi.write(FLASH25_OP_NOP);
        }

        /* deactivate device */
        _cs = 1;

        result = BD_ERROR_OK;
    }

    _mutex.unlock();
    return result;
}

int FLASH25BlockDevice::program(const void *buffer, bd_addr_t addr, bd_size_t size)
{
    _mutex.lock();
    DEBUG_PRINTF("program: %p %" PRIX64 " %" PRIX64 "\r\n", buffer, addr, size);

    if (!_is_initialized) {
        _mutex.unlock();
        return BD_ERROR_DEVICE_ERROR;
    }

    int result = BD_ERROR_DEVICE_ERROR;

    /* check parameters are valid and the write is within bounds */
    if (is_valid_program(addr, size) && buffer) {

        const uint8_t *external_buffer = static_cast<const uint8_t *>(buffer);

        /* Each write command can only cover one page at a time.
           Find page and current page offset for handling unaligned writes.
         */
        uint32_t page_number = addr / _page_size;
        uint32_t page_offset = addr % _page_size;

        /* disable write protection */
        _write_enable(true);

        /* continue until all bytes have been written */
        uint32_t bytes_written = 0;
        while (bytes_written < size) {

            /* find remaining bytes to be written */
            uint32_t bytes_remaining = size - bytes_written;

            /* cap the value at the page size and offset */
            if (bytes_remaining > (_page_size - page_offset)) {
                bytes_remaining = _page_size - page_offset;
            }

            /* Write one page, bytes_written keeps track of the progress,
               page_number is the page address, and page_offset is non-zero for
               unaligned writes.
             */
            result = _write_page(&external_buffer[bytes_written],
                                 page_number,
                                 page_offset,
                                 bytes_remaining);

            /* update loop variables upon success otherwise break loop */
            if (result == BD_ERROR_OK) {
                bytes_written += bytes_remaining;
                page_number++;

                /* After the first successful write,
                   all subsequent writes will be aligned.
                 */
                page_offset = 0;
            } else {
                break;
            }
        }

        /* enable write protection */
        _write_enable(false);
    }

    _mutex.unlock();
    return result;
}

int FLASH25BlockDevice::erase(bd_addr_t addr, bd_size_t size)
{
    _mutex.lock();
    DEBUG_PRINTF("erase: %" PRIX64 " %" PRIX64 "\r\n", addr, size);

    if (!_is_initialized) {
        _mutex.unlock();
        return BD_ERROR_DEVICE_ERROR;
    }

    int result = BD_ERROR_DEVICE_ERROR;

    /* check parameters are valid and the erase is within bounds */
    if (is_valid_erase(addr, size)) {

        /* disable write protection */
        _write_enable(true);

        if (addr == 0 && size == _device_size) {

            /* set page erase opcode */
            uint32_t command = FLASH25_OP_ERASE_CHIP;

            /* send command to device */
            _write_command(command, NULL, 0);
        }
        else {
            /* erase one page at a time until the full size has been erased */
            uint32_t erased = 0;

            while (erased < size) {

                /* set page erase opcode */
                uint32_t command = FLASH25_OP_ERASE_BLOCK;

                /* translate address */
                uint32_t address = _translate_address(addr);

                /* set page address */
                command = (command << 8) | ((address >> 16) & 0xFF);
                command = (command << 8) | ((address >>  8) & 0xFF);
                command = (command << 8) | (address & 0xFF);

                /* send command to device */
                _write_command(command, NULL, 0);

                /* wait until device is ready and update return value */
                result = _sync();

                /* if erase failed, break loop */
                if (result != BD_ERROR_OK) {
                    break;
                }

                /* update loop variables */
                addr += _block_size;
                erased += _block_size;
            }
        }
        /* enable write protection */
        _write_enable(false);
    }

    _mutex.unlock();
    return result;
}

bd_size_t FLASH25BlockDevice::get_read_size() const
{
    DEBUG_PRINTF("read size: %d\r\n", MBED_CONF_FLASH25_READ_SIZE);

    return FLASH25_READ_SIZE;
}

bd_size_t FLASH25BlockDevice::get_program_size() const
{
    DEBUG_PRINTF("program size: %d\r\n", MBED_CONF_FLASH25_PROG_SIZE);

    return FLASH25_PROG_SIZE;
}

bd_size_t FLASH25BlockDevice::get_erase_size() const
{
    _mutex.lock();
    DEBUG_PRINTF("erase size: %" PRIX16 "\r\n", _page_size);
    bd_size_t block_size = _block_size;
    _mutex.unlock();
    return block_size;
}

bd_size_t FLASH25BlockDevice::get_erase_size(bd_addr_t addr) const
{
    _mutex.lock();
    DEBUG_PRINTF("erase size: %" PRIX16 "\r\n", _page_size);
    bd_size_t block_size = _block_size;
    _mutex.unlock();
    return block_size;
}

bd_size_t FLASH25BlockDevice::size() const
{
    _mutex.lock();
    DEBUG_PRINTF("device size: %" PRIX32 "\r\n", _device_size);
    bd_size_t device_size = _device_size;
    _mutex.unlock();
    return device_size;
}

const char *FLASH25BlockDevice::get_type() const
{
    return "FLASH25";
}

/**
 * @brief Function for reading a specific register.
 * @details Used for reading either the Status Register or Manufacture and ID Register.
 *
 * @param opcode Register to be read.
 * @return value.
 */
uint16_t FLASH25BlockDevice::_get_register(uint8_t opcode)
{
    _mutex.lock();
    DEBUG_PRINTF("_get_register: %" PRIX8 "\r\n", opcode);

    /* activate device */
    _cs = 0;

    /* write opcode */
    _spi.write(opcode);

    /* read and store result */
    uint16_t status = (_spi.write(FLASH25_OP_NOP));
    status = (status << 8) | (_spi.write(FLASH25_OP_NOP));

    /* deactivate device */
    _cs = 1;

    _mutex.unlock();
    return status;
}

/**
 * @brief Function for sending command and data to device.
 * @details The command can be an opcode with address and data or
 *          a 4 byte command without data.
 *
 *          The supported frequencies and the opcode used do not
 *          require dummy bytes to be sent after command.
 *
 * @param command Opcode with address or 4 byte command.
 * @param buffer Data to be sent after command.
 * @param size Size of buffer.
 */
void FLASH25BlockDevice::_write_command(uint32_t command, const uint8_t *buffer, uint32_t size)
{
    DEBUG_PRINTF("_write_command: %" PRIX32 " %p %" PRIX32 "\r\n", command, buffer, size);

    /* activate device */
    _cs = 0;

    /* send command (opcode with data or 4 byte command) */
    _spi.write((command >> 24) & 0xFF);
    _spi.write((command >> 16) & 0xFF);
    _spi.write((command >>  8) & 0xFF);
    _spi.write(command & 0xFF);

    /* send optional data */
    if (buffer && size) {
        for (uint32_t index = 0; index < size; index++) {
            _spi.write(buffer[index]);
        }
    }

    /* deactivate device */
    _cs = 1;
}

/**
 * @brief Enable and disable write protection.
 *
 * @param enable Boolean for enabling or disabling write protection.
 */
void FLASH25BlockDevice::_write_enable(bool enable)
{
    DEBUG_PRINTF("_write_enable: %d\r\n", enable);
    uint32_t command = FLASH25_OP_NOP;
    /* enable writing, disable write protection */
    if (enable) {
        /* if not-write-protected pin is connected, select it */
        if (_nwp.is_connected()) {
            _nwp = 1;
        }

        /* send 4 byte command enabling writes */
        command = FLASH25_OP_WRITE_ENABLE;
    } else {

        /* if not-write-protected pin is connected, deselect it */
        if (_nwp.is_connected()) {
            _nwp = 0;
        }

        /* send 4 byte command disabling writes */
        command = FLASH25_OP_WRITE_DISABLE;
    }

    /* activate device */
    _cs = 0;

    _spi.write(command);

    /* deactivate device */
    _cs = 1;
}

/**
 * @brief Sleep and poll status register until device is ready for next command.
 *
 * @return BlockDevice compatible error code.
 */
int FLASH25BlockDevice::_sync(void)
{
    DEBUG_PRINTF("_sync\r\n");

    /* default return value if operation times out */
    int result = BD_ERROR_DEVICE_ERROR;

    /* Poll device until a hard coded timeout is reached.
       The polling interval is based on the typical page program time.
     */
    for (uint32_t timeout = 0;
            timeout < FLASH25_TIMEOUT;
            timeout += FLASH25_TIMING_ERASE_PROGRAM_PAGE) {

        /* get status register to verify the page size mode */
        uint16_t status_1 = _get_register(FLASH25_OP_READ_STATE_REG1);

        /* get status register to verify the page size mode */
        uint16_t status_2 = _get_register(FLASH25_OP_READ_STATE_REG2);

        uint16_t status = status_1 | status_2 >> 8;

        /* erase/program bit set, exit with error code set */
        if (!(status & FLASH25_BIT_RDY)) {
            DEBUG_PRINTF("DATAFLASH_BIT_READY\r\n");
            result = BD_ERROR_OK;
            break;
            /* wait the typical write period before trying again */
        } else {
            DEBUG_PRINTF("sleep_for: %d\r\n", MBED_CONF_FLASH25_TIMING_ERASE_PROGRAM_PAGE);
            rtos::ThisThread::sleep_for(FLASH25_TIMING_ERASE_PROGRAM_PAGE);
        }
    }

    return result;
}

/**
 * @brief Write single page.
 * @details Address can be unaligned.
 *
 * @param buffer Data to write.
 * @param addr Address to write from.
 * @param size Bytes to write. Can at most be the full page.
 * @return BlockDevice error code.
 */
int FLASH25BlockDevice::_write_page(const uint8_t *buffer,
                                      uint32_t page,
                                      uint32_t offset,
                                      uint32_t size)
{
    DEBUG_PRINTF("_write_page: %p %" PRIX32 " %" PRIX32 "\r\n", buffer, page, size);

    uint32_t command = FLASH25_OP_NOP;

    /* opcode for writing directly to device, in a single command,
       assuming the page has been erased before hand.
     */
    command = FLASH25_OP_PROGRAM_DIRECT;

    uint32_t address = 0;

    /* convert page number and offset into device address based on address format */
    address = (page * _page_size) | offset;

    /* set write address */
    command = (command << 8) | ((address >> 16) & 0xFF);
    command = (command << 8) | ((address >>  8) & 0xFF);
    command = (command << 8) | (address & 0xFF);

    /* send write command with data */
    _write_command(command, buffer, size);

    /* wait until device is ready before continuing */
    int result = _sync();

    return result;
}

/**
 * @brief Translate address.
 * @details If the device is configured for non-binary page sizes,
 *          the address is translated from binary to non-binary form.
 *
 * @param addr Binary address.
 * @return Address in format expected by device.
 */
uint32_t FLASH25BlockDevice::_translate_address(bd_addr_t addr)
{
    uint32_t address = addr;

    /* translate address if page size is non-binary */
    return address;
}

/**
 * @brief Internal function for printing out each bit set in status register.
 *
 * @param status Status register.
 */
void _print_status_flash25(uint16_t status)
{
#if FLASH25_DEBUG
    DEBUG_PRINTF("%04X\r\n", status);

//    FLASH25_BIT_SRP0                   = (0x01 << 15),
//    FLASH25_BIT_SEC                    = (0x01 << 14),
//    FLASH25_BIT_TB                     = (0x01 << 13),
//    FLASH25_BIT_BP                     = (0x07 << 10),
//    FLASH25_BIT_WEL                    = (0x01 <<  9),
//    FLASH25_BIT_RDY                    = (0x01 <<  8),
//
//    FLASH25_BIT_RES2                   = (0x01 <<  7),
//    FLASH25_BIT_CMP                    = (0x01 <<  6),
//    FLASH25_BIT_LB                     = (0x07 <<  3),
//    FLASH25_BIT_RES1                   = (0x01 <<  2),
//    FLASH25_BIT_QE                     = (0x01 <<  1),
//    FLASH25_BIT_SRP1                   = (0x01 <<  0),


    /* sector lockdown still possible */
    if (status & FLASH25_BIT_SRP1) {
        DEBUG_PRINTF("FLASH25_BIT_STATUS_REGISTER_PROTECTION_1\r\n");
    }

    /* program operation suspended while using buffer 2 */
    if (status & FLASH25_BIT_QE) {
        DEBUG_PRINTF("FLASH25_BIT_QUAD_ENABLE\r\n");
    }

    /* program operation suspended while using buffer 1 */
    if (status & FLASH25_BIT_LB) {
        DEBUG_PRINTF("FLASH25_BIT_LOCK_SECURITY_REGISTER\r\n");
    }

    /* erase has been suspended */
    if (status & FLASH25_BIT_CMP) {
        DEBUG_PRINTF("FLASH25_BIT_COMPLEMENT_BLOCK_PROTECTION\r\n");
    }
    /* device is ready (after write/erase) */
    if (status & FLASH25_BIT_RDY) {
        DEBUG_PRINTF("FLASH25_BIT_READY\r\n");
    }

    /* Buffer comparison failed */
    if (status & FLASH25_BIT_WEL) {
        DEBUG_PRINTF("FLASH25_BIT_WRITE_ENABLE_LATCH_STATUS\r\n");
    }

    /* device size is 2 MB */
    if (status & FLASH25_BIT_BP) {
        DEBUG_PRINTF("FLASH25_BIT_STATUS_BLOCK_PROTECTION\r\n");
    }

    /* sector protectino enabled */
    if (status & FLASH25_BIT_TB) {
        DEBUG_PRINTF("FLASH25_BIT_TOP_BUTTOM_PROTECTION\r\n");
    }

    /* page size is a power of 2 */
    if (status & FLASH25_BIT_SEC) {
        DEBUG_PRINTF("FLASH25_BIT_BLOCK_PROTECTION\r\n");
    }

    /* erase/program error */
    if (status & FLASH25_BIT_SRP0) {
        DEBUG_PRINTF("FLASH25_BIT_STATUS_REGISTER_PROTECTION_0\r\n");
    }
#endif
}
