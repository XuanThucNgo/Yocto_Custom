#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h> // Include errno header
#include "dc_driver.h"

int fd;

int initDriver() {
    fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("Failed to open the device");
        return errno;
    }
    return 0;
}

int exitDriver() {
    close(fd);
    return 0;
}

int setTargetSpeed(unsigned int speed) {
    if (ioctl(fd, SWERVE_IOCTL_SEND_TARGET_SPEED, &speed) < 0) {
        perror("Failed to send speed");
        close(fd);
        return errno;
    }
    return 0;
}

int setTargetAngle(unsigned int angle) {
    if (ioctl(fd, SWERVE_IOCTL_SEND_TARGET_ANGLE, &angle) < 0) {
        perror("Failed to send angle");
        close(fd);
        return errno;
    }
    return 0;
}
