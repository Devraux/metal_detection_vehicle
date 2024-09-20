#include "mpu6050.h"

static mpu6050_Reg_t mpu6050_Reg = {
    .address = 0x68,        //device address
    .who_i_am_add = 0x75,
    .reset_add = 0x6B,      //reset address
    .accel_add = 0x3B,      //accelerator data address register
    .gyro_add = 0x43,       //gyroscope data address register
    .temp_add = 0x41,       //temperature data address register
    .acc_config = 0x1C,     //accelerometer resolution config register and calibration
    .gyro_config = 0x1B,    // gyroscope resolution config register and calibration
    .gyro_res = 0x1B,       // gyroscope resolution config register and calibration
    .XA_TEST = 0x0D,        //XA_TEST and XG_test register
    .YA_TEST = 0x0E,        //YA_TEST and YG_test register
    .ZA_TEST = 0x0F,        //ZA_TEST and ZG_test register
    .A_TEST = 0x10,         //second accelerometer test register XA_TEST[1:0]
    .config = 0x1A,         //gyroscope DLPF_CFG set register
    .SMPLRT_DIV = 0x19,     //sample rate divider
    .FIFO_EN = 0x23,        // fifo enable
    .FIFO_COUNTER_H = 0x72, // fifo counter high[15 : 8]
    .FIFO_COUNTER_L = 0x73, // fifo counter high[7 : 0]
    .INT_ENABLE = 0x38,     //  interrupt register
    .USER_CTRL = 0x6A       // user control
};

static mpu6050_t mpu6050 = {0};
static bool transfer_complete = false;
static struct repeating_timer timer;

static void i2c_Write_Reg(uint8_t i2c_Address, uint8_t reg, uint8_t data)
{
    uint8_t array[] = {reg, data};
    i2c_write_blocking(i2c1, i2c_Address, array, sizeof(array)/sizeof(array[0]), false);
}

void mpu_Init(void)
{
    // I2C INIT //
    i2c_init(i2c1, 400000);
    gpio_set_function(SDA_Pin, GPIO_FUNC_I2C); //27 and 26
    gpio_set_function(SCL_Pin, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_Pin);
    gpio_pull_up(SCL_Pin);

    // MPU6050 SENSOR INIT //
    mpu_Reset();
    i2c_Write_Reg(mpu6050_Reg.address, mpu6050_Reg.acc_config, Accel_Resolution); //±2g
    i2c_Write_Reg(mpu6050_Reg.address, mpu6050_Reg.gyro_res, Gyro_Resolution);    //±250deg/sec
    mpu_Get_Offset();
    mpu6050.Yaw = 90.0f; // initial angle is 90* <-> not 0 !!! <->  cartesian coordinate system

    //MPU6050 INTERRUPT INIT
    add_repeating_timer_ms(-123, mpu_Read, NULL, &timer); // 8 times per second
}

void mpu_Reset(void)
{
    i2c_Write_Reg(mpu6050_Reg.address, mpu6050_Reg.reset_add, 0x80);
    sleep_ms(50);
    i2c_Write_Reg(mpu6050_Reg.address, mpu6050_Reg.reset_add, 0x00);
    sleep_ms(50);
}

void mpu_Read_Raw(void)
{
    uint8_t buffer[6];

    //ACCELERATION -> Acceleration not used in this project
    //i2c_write_blocking(i2c1, mpu6050_Reg.address, &mpu6050_Reg.accel_add, 1, true); 
    //i2c_read_blocking(i2c1, mpu6050_Reg.address, buffer, 6, false);

    //mpu6050.X_Accel_Raw_Data = (buffer[0] << 8) | buffer[1];
    //mpu6050.Y_Accel_Raw_Data = (buffer[2] << 8) | buffer[3];
    //mpu6050.Z_Accel_Raw_Data = (buffer[4] << 8) | buffer[5];

    //GYROSCOPE
    i2c_write_blocking(i2c1,mpu6050_Reg.address, &mpu6050_Reg.gyro_add, 1, true);
    i2c_read_blocking(i2c1, mpu6050_Reg.address, buffer, 6, false);  

    mpu6050.X_Gyro_Raw_Data = (buffer[0] << 8) | buffer[1];
    mpu6050.Y_Gyro_Raw_Data = (buffer[2] << 8) | buffer[3];
    mpu6050.Z_Gyro_Raw_Data = (buffer[4] << 8) | buffer[5];
} 

bool mpu_Read(struct repeating_timer *timer)
{
    mpu_Read_Raw();

    // Only YAW  is useful for my application so rest of data is skipped due to reduce MCU computations //
    //int16_t X_Accel_No_Offset = mpu6050.X_Accel_Raw_Data - mpu6050.X_Accel_Offset;
    //int16_t Y_Accel_No_Offset = mpu6050.Y_Accel_Raw_Data - mpu6050.Y_Accel_Offset; 
    //int16_t Z_Accel_No_Offset = mpu6050.Z_Accel_Raw_Data - mpu6050.Z_Accel_Offset;

    //int16_t X_Gyro_No_Offset = mpu6050.X_Gyro_Raw_Data - mpu6050.X_Gyro_Offset;
    //int16_t Y_Gyro_No_Offset = mpu6050.Y_Gyro_Raw_Data - mpu6050.Y_Gyro_Offset; 
    int16_t Z_Gyro_No_Offset = mpu6050.Z_Gyro_Raw_Data - mpu6050.Z_Gyro_Offset;

    //float X_Accel = (X_Accel_No_Offset / 16384) * 9.81;
    //float X_Accel = (Y_Accel_No_Offset / 16384) * 9.81;
    //float X_Accel = (Z_Accel_No_Offset / 16384) * 9.81;

    //float X_Gyro = (X_Gyro_No_Offset / 131);
    //float Y_Gyro = (Y_Gyro_No_Offset / 131);
    float Z_Gyro = 0;

    if(Z_Gyro_No_Offset >= 60.0f || Z_Gyro_No_Offset <= -60.0f)
        Z_Gyro = (Z_Gyro_No_Offset / 131.0f);
    else 
        Z_Gyro_No_Offset = 0;

    mpu6050.Yaw += Z_Gyro * 0.123f;

    if(mpu6050.Yaw >= 360.0f)
        mpu6050.Yaw = fmod(mpu6050.Yaw, 360.0f);

    else if(mpu6050.Yaw <= 0.0f)
    mpu6050.Yaw = fmod(mpu6050.Yaw + 360.0f, 360.0f);

    return true;
}

void mpu_Get_Offset(void)
{
    // Only YAW  is useful for my application so rest of data is skipped due to reduce MCU computations //
    //int32_t accel_X_offset = 0, accel_Y_offset = 0, accel_Z_offset = 0;
    int32_t gyro_X_offset = 0, gyro_Y_offset = 0, gyro_Z_offset = 0;
    
    for(uint8_t i = 0; i < 200; i++) // make 200 measurements and get average
    {
        mpu_Read_Raw();
        //accel_X_offset += mpu6050.X_Accel_Raw_Data;
        //accel_Y_offset += mpu6050.Y_Accel_Raw_Data;
        //accel_Z_offset += mpu6050.Z_Accel_Raw_Data;

        //gyro_X_offset += mpu6050.X_Gyro_Raw_Data;
        //gyro_Y_offset += mpu6050.Y_Gyro_Raw_Data;
        gyro_Z_offset += mpu6050.Z_Gyro_Raw_Data;

        sleep_ms(10); // wait for next measurement from mpu
    }

    //accel_X_offset /= 200; accel_Y_offset /= 200; accel_Z_offset /= 200;
    /*gyro_X_offset  /= 200; gyro_Y_offset  /= 200;*/ gyro_Z_offset  /= 200;

    //mpu6050.X_Accel_Offset = accel_X_offset; 
    //mpu6050.Y_Accel_Offset = accel_Y_offset;
    //mpu6050.Z_Accel_Offset = accel_Z_offset;

    //mpu6050.X_Gyro_Offset  = gyro_X_offset;
    //mpu6050.Y_Gyro_Offset  = gyro_Y_offset;
    mpu6050.Z_Gyro_Offset  = gyro_Z_offset;
}

float mpu_Get_Yaw(void)
{
    return mpu6050.Yaw;
}

void DMA_I2C_init(void) 
{
    dma_channel_config channel_config = dma_channel_get_default_config(0);
    channel_config_set_read_increment(&channel_config, true);
    channel_config_set_write_increment(&channel_config, false);
    dma_channel_set_config(0, &channel_config, false);
    irq_set_exclusive_handler(DMA_IRQ_0, DMA_Callback);
    irq_set_enabled(DMA_IRQ_0, true);
}

void DMA_Callback(void) 
{
    transfer_complete = true;
}

void DMA_I2C_Read(void) 
{

}