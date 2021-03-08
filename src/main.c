#include "spi.h"
#include "rf24.h"

char rx_buf[100];
char tx_buf[100];
int main(){
    int fd;
    uint8_t mem_data, config;
    config = 0b01101000;
    if ((fd = spi_init("/dev/spidev0.0"))< 0)
        return -1;
    rf_write_mem(fd, 0, &config, sizeof config);
    rf_read_mem(fd, 0, &mem_data, sizeof mem_data);
    printf("memory content is %d, \n", mem_data);
    close(fd);
    return 0;
}
