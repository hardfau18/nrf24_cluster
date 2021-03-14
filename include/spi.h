#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/ioctl.h>
#include<sys/wait.h>
#include<linux/types.h>
#include<linux/spi/spidev.h>
#include<stdint.h>

// TODO: later use this method
// #define spi_transfer_single_buf(fd, len, buf) __spi_transfer(fd, len, buf, buf)
// #define spi_transfer_double_buf(fd, len, tx_buf, rx_buf) __spi_transfer(fd, len, tx_buf, rx_buf)
//
// #define __spi_transfer_mult(x,A,B,FUNC, ...)  FUNC
// #define spi_transfer(fd, len, ...)                    __spi_transfer_mult(,##__VA_ARGS__,\
//                                           spi_transfer_double_buf(fd, len, __VA_ARGS__),\
//                                           spi_transfer_single_buf(fd, len, __VA_ARGS__),\
//                                           spi_transfer_single_byte(__VA_ARGS__)\
//                                          )


void config_pins(void);

// transmits the content of buf to reciever and writes the recieved content to same buffer
// returns the return status of ioctl transfer
// @ fd - filedescriptor of spi transfer
// @ len - length of the buffer
// @ buf - transfer buffer
int spi_transfer(int fd, uint32_t len, uint8_t* tx_buf, uint8_t *rx_buf);
// TODO: remove these intermediate function and use macros
int spi_transfer_single_byte(int fd, uint8_t* byte);
int spi_transfer_single_buf(int fd, uint32_t len, uint8_t* buf);
// returns the return status of ioctl transfer
// @ fd - filedescriptor of spi transfer
// @ len - length of the buffer
// @ buf - transfer buffer
int spi_read(int fd, uint32_t len, uint8_t* buf);
// returns the return status of ioctl transfer
// @ fd - filedescriptor of spi transfer
// @ len - length of the buffer
// @ buf - transfer buffer
int spi_write(int fd, uint32_t len, uint8_t* buf);

// initializes spi device
// TODO: currently using default configurations later use dynamic configurations
// @return: file descriptor of spi
int spi_init(char file_name[40]);
