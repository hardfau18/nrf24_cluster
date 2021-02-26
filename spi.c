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


static char rx_buf[100];
static char tx_buf[100];
static uint32_t mode, bits, speed;

struct spi_ioc_transfer xfer;

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

int spi_init(char file_name[40]){
    config_pins();
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

    xfer.bits_per_word = bits;
    xfer.cs_change = 0;
    xfer.speed_hz = speed;
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

    if (spi_read(fd, 20, (uint8_t*)tx_buf) < 0)
        fprintf(stderr, "failed to transfer\n");
    tx_buf[20]=0;
    puts(tx_buf);
    close(fd);
    return 0;
}

// configures mux state of pins through bash commands
void config_pins(){
    int fd;
    // set P9.17
    if ((fd = open("/sys/devices/platform/ocp/ocp:P9_17_pinmux/state", O_RDWR)) < 0){
        puts("failed to open P9_17_pinmux");
        exit(-1);
    } else{
        if (write(fd, "spi_cs", 6) < 0){
            puts("write to p9_17 failed");
            exit(1);
        }
        close(fd);
    }

    // set P9.18
    if ((fd = open("/sys/devices/platform/ocp/ocp:P9_18_pinmux/state", O_RDWR)) < 0){
        puts("failed to open P9_18_pinmux");
        exit(-1);
    } else{
        if (write(fd, "spi", 3) < 0){
            puts("write to p9_18 failed");
            exit(1);
        }
        close(fd);
    }

    // set P9.21
    if ((fd = open("/sys/devices/platform/ocp/ocp:P9_21_pinmux/state", O_RDWR)) < 0){
        puts("failed to open P9_21_pinmux");
        exit(-1);
    } else{
        if (write(fd, "spi", 3) < 0){
            puts("write to p9_21 failed");
            exit(1);
        }
        close(fd);
    }

    // set P9.22
    if ((fd = open("/sys/devices/platform/ocp/ocp:P9_22_pinmux/state", O_RDWR)) < 0){
        puts("failed to open P9_22_pinmux");
        exit(-1);
    } else{
        if (write(fd, "spi_sclk", 8) < 0){
            puts("write to p9_22 failed");
            exit(1);
        }
        close(fd);
    }
}

int spi_transfer(int fd, uint32_t len, uint8_t* buf){
    int status;
    // same buffer can be used to read and write, checked
    xfer.tx_buf = (unsigned long)buf;
    xfer.rx_buf = (unsigned long)buf;
    xfer.len = len;
    status = ioctl(fd, SPI_IOC_MESSAGE(1), &xfer);
    return status;
}

// TODO change this to macro so that transfer func only used for comm
int spi_read(int fd, uint32_t len, uint8_t* buf){
    int status;
    xfer.tx_buf = (unsigned long)NULL;                      // if reading then keep tx buffer empty
    xfer.rx_buf = (unsigned long)buf;
    xfer.len = len;
    status = ioctl(fd, SPI_IOC_MESSAGE(1), &xfer);
    return status;
}
// TODO change this to macro so that transfer func only used for comm
int spi_write(int fd, uint32_t len, uint8_t* buf){
    int status;
    xfer.tx_buf = (unsigned long)buf;
    xfer.rx_buf = (unsigned long)NULL;                      // if writing then keep tx buffer empty
    xfer.len = len;
    status = ioctl(fd, SPI_IOC_MESSAGE(1), &xfer);
    return status;
}
