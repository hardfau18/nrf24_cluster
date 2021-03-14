#include "spi.h"
#include "rf24.h"
#include<stdint.h>
#include<string.h>

char rx_buf[100];
char tx_buf[100];
uint8_t addr[5] = {0xff, 0xfe, 0xfd, 0xfc, 0xf4};
int main(){
    int fd;
    uint8_t mem_data, config;
    config = 0b01101000;
    if ((fd = rf_init("/dev/spidev0.0"))< 0)
        return -1;
    /* rf_write_mem(fd, 0, &config, sizeof config); */
    /* rf_read_mem(fd, 0, &mem_data, sizeof mem_data); */
    rf_flush_tx(fd);
    rf_write_addr(fd, RF_pipe_1, addr);
    rf_write_addr(fd, RF_pipe_2, addr);
    memset((void*) addr, 0x01, 5);
    rf_read_addr(fd, RF_pipe_2, addr);
    printf("memory content is %x-%x-%x-%x, \n", (uint8_t)addr[0], (uint8_t) addr[1], (uint8_t) addr[2], (uint8_t) addr[3], (uint8_t) addr[4]);
    close(fd);
    return 0;
}
