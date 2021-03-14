#include "spi.h"

uint32_t mode, bits, speed;
struct spi_ioc_transfer xfer;

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

int spi_transfer(int fd, uint32_t len, uint8_t* tx_buf, uint8_t* rx_buf){
    int status;
    // same buffer can be used to read and write, checked
    xfer.tx_buf = (unsigned long)tx_buf;
    xfer.rx_buf = (unsigned long)rx_buf;
    xfer.len = len;
    status = ioctl(fd, SPI_IOC_MESSAGE(1), &xfer);
    return status;
}

int spi_transfer_single_byte(int fd, uint8_t *byte){
    xfer.tx_buf = (unsigned long)byte;
    xfer.rx_buf = (unsigned long)NULL;
    xfer.len = 1;
    return ioctl(fd, SPI_IOC_MESSAGE(1), &xfer);
}
int spi_transfer_single_buf(int fd, uint32_t len, uint8_t* buf){
    xfer.tx_buf = (unsigned long)buf;
    xfer.rx_buf = (unsigned long)buf;
    xfer.len = len;
    return ioctl(fd, SPI_IOC_MESSAGE(1), &xfer);
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
