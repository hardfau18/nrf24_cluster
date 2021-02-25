#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/ioctl.h>
#include<linux/types.h>
#include<linux/spi/spidev.h>
#include<stdint.h>

static char rx_buf[100];
static char tx_buf[100];
static uint32_t mode, bits, speed;

struct spi_ioc_transfer xfer;

int spi_init(char file_name[40]){
    int fd, ret; 
    mode = SPI_MODE_0;
    bits = 8;
    speed = 500000;
    fd = open(file_name, O_RDWR);
    if (fd <0){
        printf("couldn't able find the device %s\n", file_name);
        return -1;
    }
    
    // set and check for mode
    if ((ret = ioctl(fd, SPI_IOC_WR_MODE, &mode))< 0){
        printf("failed to set mode, status %d\n", ret);
        return -1;
    }
    if ((ret = ioctl(fd, SPI_IOC_RD_MODE, &mode)) < 0){
        printf("failed to read mode, status %d\n", ret);
        return -1;
    }
    // set and check for bits per word
    if ((ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed)) < 0){
        printf("failed to read speed, status %d\n", ret);
        return -1;
    }
    if ((ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed)) < 0){
        printf("failed to read speed, status %d\n", ret);
        return -1;
    }
    // set and check for speed
    if ((ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits)) < 0){
        printf("failed to read bits_per_word, status %d\n", ret);
        return -1;
    }
    if ((ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits)) < 0){
        printf("failed to read mode, status %d\n", ret);
        return -1;
    }

    // print current settings
    printf("spi mode        = 0x%x\n",mode);
    printf("speed           = %u \n", speed);
    printf("bits_per_word   = %u \n", bits);

    xfer.len = 20;                                            // len of command write
    xfer.bits_per_word = bits;
    xfer.cs_change = 0;
    xfer.speed_hz = speed;
    xfer.tx_buf = (unsigned long)tx_buf;
    xfer.rx_buf = (unsigned long)rx_buf;
    return fd;
}

int main(){
    int fd, status ;

    memset(tx_buf, 0x41, 10);
    memset(tx_buf+10, 0x42, 10);
    tx_buf[19]=0;

    // open spi device
    if ((fd = spi_init("/dev/spidev0.0"))< 0)
        return -1;

    // transfer 1 set of data
    if ((status = ioctl(fd, SPI_IOC_MESSAGE(1), &xfer)) <0){
        printf("failed to transfer with status %d\n", status);
        close(fd);
        return -1;
    }

    rx_buf[20]=0;
    puts(rx_buf);
    close(fd);
    return 0;
}
