#include "rf24.h"
#include "stdlib.h"
#include "string.h" 

uint8_t rf_read_mem(int fd, uint8_t address, uint8_t* buf, uint8_t len){
    uint8_t *tmp_buf;
    uint8_t transfer_len;
    transfer_len = len+1; // includes 1 byte instruction and len bytes data
    tmp_buf = malloc(transfer_len);
    *tmp_buf = (uint8_t)0 | (address & 0b00011111);            // first byte is opcod(000) + address(5 bits)
    len = spi_transfer(fd, transfer_len, tmp_buf);
    tmp_buf++;        // first byte recieved is status of previous command
    memcpy(buf, tmp_buf, len);
    return len;
}
uint8_t rf_write_mem(int fd, uint8_t address, uint8_t* buf, uint8_t len){
    uint8_t *tmp_buf;
    uint8_t transfer_len;
    transfer_len = len+1; // includes 1 byte instruction and len bytes data
    tmp_buf = malloc(transfer_len);
    *tmp_buf = (uint8_t)(0b1 << 5) | (address & 0b00011111);            // first byte is opcod(000) + address(5 bits)
    memcpy(tmp_buf + 1, buf,  len);
    printf("write command: %d, write data %d\n", *tmp_buf, *(tmp_buf+1));
    len = spi_transfer(fd, transfer_len, tmp_buf);
    return len;
}

uint8_t rf_tx_data(int fd, uint8_t *buf, uint8_t len){
}
uint8_t rf_rx_data(int fd, uint8_t *buf, uint8_t len){
}
int rf_init(char* device){
    int fd;
    fd = spi_init(device);
    return fd;
}

