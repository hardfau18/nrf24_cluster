#include "rf24.h"
#include "stdlib.h"
#include "string.h" 

uint8_t rf_read_mem(int fd, uint8_t address, uint8_t* buf, uint8_t len){
    uint8_t *tmp_buf;
    uint8_t transfer_len;
    transfer_len = len+1; // includes 1 byte instruction and len bytes data
    tmp_buf = malloc(transfer_len);
    *tmp_buf = (uint8_t)RF_R_Register | (address & 0b00011111);            // first byte is opcod(000) + address(5 bits)
    len = spi_transfer_single_buf(fd, transfer_len, tmp_buf);
    tmp_buf++;        // first byte recieved is status of previous command
    memcpy(buf, tmp_buf, len);
    free(tmp_buf);
    return len;
}
uint8_t __rf_write_mem(int fd, uint8_t address, uint8_t* buf, uint8_t len){
    uint8_t *tmp_buf;
    uint8_t transfer_len;
    transfer_len = len+1; // includes 1 byte instruction and len bytes data
    tmp_buf = malloc(transfer_len);
    *tmp_buf = (uint8_t)(RF_W_Register | (address & 0b00011111));            // first byte is opcod(000) + address(5 bits)
    memcpy(tmp_buf + 1, buf,  len);
    len = spi_transfer_single_buf(fd, transfer_len, tmp_buf);
    free(tmp_buf);
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

uint8_t rf_read_status(int fd){
    uint8_t status = RF_Nop;        // we write Nop so that we can transfer status
    spi_transfer_single_byte(fd, &status);
    return status;
}

int rf_read_addr(int fd, enum rf_pipes pipe, uint8_t * buf){
    uint8_t* tmp_buf;
    int_fast8_t transfer_len;
    
    // 5 byte address and 1 byte instruction
    tmp_buf = malloc(address_byte_len + 1);
    if (pipe > RF_pipe_1){
        uint8_t pipe_addr;
        tmp_buf[0] = (uint8_t)RF_R_Register | (uint8_t) RF_pipe_1 ;   // reading first4 bytes of address
        // read pipe1 address and then read repsective pipe address append that to pipe 1 address
        transfer_len = spi_transfer_single_buf(fd, address_byte_len + 1, tmp_buf);
        memcpy((void*) buf, (void *)(tmp_buf + 1), address_byte_len - 1); // only first 4 bytes needs to copied

        tmp_buf[0] = (uint8_t)RF_R_Register | sanitize_address(pipe);   // reading last byte
        transfer_len = spi_transfer_single_buf(fd, 2, tmp_buf);
        buf[address_byte_len - 1] = tmp_buf[1];                         // copy last byte of the address
    } else {
        // read all bytes at once
        tmp_buf[0] = (uint8_t)RF_R_Register | sanitize_address(pipe);
        // TODO: add transfer len check
        transfer_len = spi_transfer_single_buf(fd, address_byte_len + 1, tmp_buf);
        memcpy((void*) buf, (void *)(tmp_buf + 1), address_byte_len); 
        // read respective pipe address
    }
    return 0;
}
int rf_write_addr(int fd, enum rf_pipes pipe, uint8_t * buf){
    uint8_t* tmp_buf;
    int_fast8_t transfer_len;
    
    // 5 byte address and 1 byte instruction
    tmp_buf = malloc(address_byte_len + 1);
    tmp_buf[0] = (uint8_t)RF_W_Register | sanitize_address(pipe);
    if (pipe > RF_pipe_1){
        tmp_buf[1] = buf[address_byte_len - 1];             // only last byte needs to be written in this case
        transfer_len = spi_transfer_single_buf(fd, 2, tmp_buf);
        // TODO: add check return value
    } else {
        memcpy((void *)(tmp_buf + 1), buf, address_byte_len);
        transfer_len  = spi_transfer_single_buf(fd, address_byte_len + 1, tmp_buf);
    }
    return 0;
}
