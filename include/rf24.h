#pragma once
#include "spi.h"
#include "stdint.h"
#include <unistd.h>


// ================================ instructions =======================
enum instruct {
    RF_R_Register   = 0b00000000,           // last 5 bits are address bits
    RF_W_Register   = 0b00100000,           // last 5 bits are address bits
    RF_R_Rx_Payload = 0b01100001,
    RF_W_Tx_Payload = 0b10100000,
    RF_Flush_Tx     = 0b11100001,
    RF_Flush_Rx     = 0b11100010,
    RF_ReUse_Tx_Pl  = 0b11100011,
    RF_Nop          = 0b11111111,
};

enum rf_pipes {
    RF_pipe_0 = 0xA,
    RF_pipe_1 = 0xB,
    RF_pipe_2 = 0xC,
    RF_pipe_3 = 0xD,
    RF_pipe_4 = 0xE,
    RF_pipe_5 = 0xF,
};

// ======================== macro sections ============================
#define rf_flush_tx(fd)     spi_transfer_single_byte(fd, & (uint8_t) {RF_Flush_Tx})
#define rf_flush_rx(fd)     spi_transfer_single_byte(fd, & (uint8_t) {RF_Flush_Rx})
#define rf_read_config(fd, address, buf, len)   __rf_read_mem(fd, address, buf, len)
#define rf_write_config(fd, address, buf, len ) __rf_write_mem(fd, address, buf, len)
#define rf_read_data(fd, buf, len )             
#define rf_write_data(fd, buf, len )

#define address_byte_len                    (5)
#define sanitize_address(address)           ((uint8_t)(address & 0b00011111))

// reads the data into buffer
// @fd : file descriptor returned by init function
// @return: returns len of data read
// @address: 5 bits address of the memory
uint8_t __rf_read_mem(int fd, uint8_t address, uint8_t* buf, uint8_t len);
// writes data present in the buffer to memory
// @return: returns status register content
// @address: 5 bits address of the memory
uint8_t __rf_write_mem(int fd, uint8_t address, uint8_t* buf, uint8_t len);

uint8_t rf_read_status(int fd);


// initialize radio 
// @device: device name sysfilesystem tree
// @return: returns a filedescriptors
// TODO: later may be replace it by macro
int rf_init(char* device);

// reads address of the pipe from the register and writes it to buf
// @pipe: Pipe number(pipe address)
// @buf : 5 bytes long buffer which will be use to write address
int rf_read_addr(int fd, enum rf_pipes pipe, uint8_t * buf);
//
// writes address of the pipe into respective register 
// @pipe: Pipe number(pipe address)
// @buf : 5 bytes long buffer which will be use to write address
int rf_write_addr(int fd, enum rf_pipes pipe, uint8_t * buf);
