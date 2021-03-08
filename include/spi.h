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




void config_pins(void);

// transmits the content of buf to reciever and writes the recieved content to same buffer
// returns the return status of ioctl transfer
// @ fd - filedescriptor of spi transfer
// @ len - length of the buffer
// @ buf - transfer buffer
int spi_transfer(int fd, uint32_t len, uint8_t* buf);
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
// @file_name: name of the spi interface ex:spidev0.0
int spi_init(char file_name[40]);
