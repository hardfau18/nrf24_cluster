#pragma once
#include "spi.h"
#include "stdint.h"


#define rf_flush_tx() rf_write
#define rf_flush_rx()
#define rf_read_status()
// reads the data into buffer
// @fd : file descriptor returned by init function
// @return: returns status register content
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
int rf_init(char* device);

