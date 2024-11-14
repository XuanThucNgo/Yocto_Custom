#ifndef DC_DRIVER_H
#define DC_DRIVER_H

//Define cmd for in out control
#define SWERVE_MODULE_MAGIC_WORD 's'
#define SWERVE_IOCTL_SEND_TARGET_SPEED _IOW(SWERVE_MODULE_MAGIC_WORD,DEV_REG_TARGET_SPEED,uint8_t)
#define SWERVE_IOCTL_SEND_TARGET_ANGLE _IOW(SWERVE_MODULE_MAGIC_WORD,DEV_REG_TARGET_ANGLE,uint8_t)
#define SWERVE_IOCTL_READ_CURRENT_ANGLE _IOR(SWERVE_MODULE_MAGIC_WORD,17,int)

//Define device path in /dev direction
#define DEVICE_PATH "/dev/dc_driver"

//Define an enum for module's register
typedef enum SwerveParamRegister{
    DEV_REG_SPEED_KP = 1,
    DEV_REG_SPEED_KI,
    DEV_REG_SPEED_KD,

    DEV_REG_ANGLE_KP,
    DEV_REG_ANGLE_KI,
    DEV_REG_ANGLE_KD,

    DEV_REG_TARGET_SPEED,
    DEV_REG_TARGET_ANGLE,
    DEV_REG_MODE,
}SwerveParamRegister;

int initDriver();
int exitDriver();
int setTargetSpeed(unsigned int speed);
int setTargetAngle(unsigned int angle);

#endif // DC_DRIVER_H
