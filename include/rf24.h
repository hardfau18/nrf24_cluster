#pragma once
#include "spi.h"
#include "stdint.h"


// ======================== macro sections ============================
#define rf_flush_tx() rf_write
#define rf_flush_rx()
#define rf_read_status()

// ================================ instructions =======================
enum instruct {
    RF_R_Register   = 0b00000000,
    RF_W_Register   = 0b00100000,
    RF_R_Rx_Payload = 0b01100001,
    RF_W_Tx_Payload = 0b10100000,
    RF_Flush_Tx     = 0b11100001,
    RF_Flush_Rx     = 0b11100010,
    RF_ReUse_Tx_Pl  = 0b11100011,
    RF_Nop          = 0b11111111,
};
// reads the data into buffer
// @fd : file descriptor returned by init function
// @return: returns len of data read
// @address: 5 bits address of the memory
uint8_t rf_read_mem(int fd, uint8_t address, uint8_t* buf, uint8_t len);
// writes data present in the buffer to memory
// @return: returns status register content
// @address: 5 bits address of the memory
uint8_t rf_write_mem(int fd, uint8_t address, uint8_t* buf, uint8_t len);

uint8_t rf_tx_data(int fd, uint8_t *buf, uint8_t len);
uint8_t rf_rx_data(int fd, uint8_t *buf, uint8_t len);

// initialize radio 
// @device: device name sysfilesystem tree
// @return: returns a filedescriptors
// TODO: later may be replace it by macro
int rf_init(char* device);

