#ifndef _MPU6050_
#define _MPU6050_

#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "../buffer/buffer.h" 

#define SDA_Pin 26
#define SCL_Pin 27

#define Accel_Resolution 0x00000000 //±2g
#define Gyro_Resolution  0x00000000 //±250deg/sec
#define MPU_Time_Stamp 250 // 4 times per second

typedef struct mpu6050_Reg_t{
    const uint8_t address;            //device address
    const uint8_t who_i_am_add;    
    const uint8_t reset_add;          //reset address
    const uint8_t accel_add;          //accelerator data address register
    const uint8_t gyro_add;           //gyroscope data address register
    const uint8_t temp_add;           //temperature data address register
    const uint8_t gyro_res;           // gyroscope resolution config and calibration
    const uint8_t acc_config;         // accelerometer resolution config and calibration
    const uint8_t gyro_config;        // gyroscope resolution config and calibration
    const uint8_t XA_TEST;            //XA_TEST and XG_test register 
    const uint8_t YA_TEST;            //YA_TEST and YG_test register
    const uint8_t ZA_TEST;            //ZA_TEST and ZG_test register
    const uint8_t A_TEST;             //second accelerometer test register XA_TEST[1:0]
    const uint8_t config;             //gyroscope DLPF_CFG set register
    const uint8_t SMPLRT_DIV;         //sample rate divider
    const uint8_t FIFO_EN;            // fifo enable
    const uint8_t FIFO_COUNTER_H ;    // fifo counter high[15 : 8]
    const uint8_t FIFO_COUNTER_L;     // fifo counter high[7 : 0]
    const uint8_t INT_ENABLE;         // interrupt register
    const uint8_t USER_CTRL;          // user control
}mpu6050_Reg_t;    

typedef struct mpu6050_t{
    int16_t X_Accel_Offset; 
    int16_t Y_Accel_Offset; 
    int16_t Z_Accel_Offset; 

    int16_t X_Gyro_Offset;
    int16_t Y_Gyro_Offset;
    int16_t Z_Gyro_Offset;

    int16_t X_Accel_Raw_Data; 
    int16_t Y_Accel_Raw_Data; 
    int16_t Z_Accel_Raw_Data; 

    int16_t X_Gyro_Raw_Data;
    int16_t Y_Gyro_Raw_Data;
    int16_t Z_Gyro_Raw_Data;

    float Pitch;
    float Roll;
    float Yaw;
}mpu6050_t;

/// @brief I2C pi pico build in i2c wrapper 
/// @param i2c_address device address
/// @param reg device register(place to write data)
/// @param data data to write in register  
void i2c_Write_Reg(uint8_t i2c_Address, uint8_t reg, uint8_t data);

/// @brief mpu I2C initialisation
/// @param mpu6050_t mpu6050_t data structure 
void mpu_init(void); 

/// @brief device reset
void mpu_Reset(void); 


/// @brief mpu read raw data
/// @param mpu6050_t  mpu6050_t data structure 
void mpu_Read_Raw(void); 

/// @brief read processed(no offset) data from accelerometer and gyroscope
/// @param mpu6050_t  mpu6050_t output data
bool mpu_Read(struct repeating_timer *timer);

/// @brief get accelerometer and gyroscope offset
/// @param mpu6050_t  mpu6050_t data structure 
void mpu_Get_Offset(void); 

/// @brief return yaw from mpu6050
/// @param  -
/// @return yaw from mpu6050
float mpu_Get_Yaw(void);

#endif