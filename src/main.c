#include "spi.h"
#include "rf24.h"

char rx_buf[100];
char tx_buf[100];
int main(){
    int fd, status ;

    memset(tx_buf, 0x41, 10);
    memset(tx_buf+10, 0x42, 10);
    tx_buf[19]=0;

    // open spi device
    if ((fd = spi_init("/dev/spidev0.0"))< 0)
        return -1;

    if (spi_read(fd, 20, (uint8_t*)tx_buf) < 0)
        fprintf(stderr, "failed to transfer\n");
    tx_buf[20]=0;
    puts(tx_buf);
    close(fd);
    return 0;
}
