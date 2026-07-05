#include "tcs34725.h"

static I2C_HandleTypeDef *tcs_i2c;

static uint8_t initialised = 0;

static void write8(uint8_t reg, uint8_t value);
static uint8_t read8(uint8_t reg);
static uint16_t read16(uint8_t reg);

static void enable(void);
static void setIntegrationTime(uint8_t time);
static void setGain(uint8_t gain);

/* WRITE 8 BIT */
static void write8(uint8_t reg, uint8_t value)
{
    uint8_t data[2];

    data[0] = TCS34725_COMMAND_BIT | reg;
    data[1] = value;

    HAL_I2C_Master_Transmit(tcs_i2c,
                            TCS34725_ADDRESS,
                            data,
                            2,
                            100);
}

/* READ 8 BIT */
static uint8_t read8(uint8_t reg)
{
    uint8_t data;

    uint8_t cmd = TCS34725_COMMAND_BIT | reg;

    HAL_I2C_Master_Transmit(tcs_i2c,
                            TCS34725_ADDRESS,
                            &cmd,
                            1,
                            100);

    HAL_I2C_Master_Receive(tcs_i2c,
                           TCS34725_ADDRESS,
                           &data,
                           1,
                           100);

    return data;
}

/* READ 16 BIT */
static uint16_t read16(uint8_t reg)
{
    uint8_t rx[2];

    uint8_t cmd = TCS34725_COMMAND_BIT | reg;

    HAL_I2C_Master_Transmit(tcs_i2c,
                            TCS34725_ADDRESS,
                            &cmd,
                            1,
                            100);

    HAL_I2C_Master_Receive(tcs_i2c,
                           TCS34725_ADDRESS,
                           rx,
                           2,
                           100);

    return ((uint16_t)rx[1] << 8) | rx[0];
}

/* ENABLE SENSOR */
static void enable(void)
{
    write8(TCS34725_ENABLE,
           TCS34725_ENABLE_PON);

    HAL_Delay(3);

    write8(TCS34725_ENABLE,
           TCS34725_ENABLE_PON |
           TCS34725_ENABLE_AEN);

    HAL_Delay(50);
}

/* SET INTEGRATION TIME */
static void setIntegrationTime(uint8_t time)
{
    write8(TCS34725_ATIME, time);
}

/* SET GAIN */
static void setGain(uint8_t gain)
{
    write8(TCS34725_CONTROL, gain);
}

/* INIT */
void TCS34725_Init(I2C_HandleTypeDef *hi2c)
{
    tcs_i2c = hi2c;

    uint8_t id = read8(TCS34725_ID);

    if(id != 0x44 && id != 0x4D)
    {
        return;
    }

    initialised = 1;

    setIntegrationTime(TCS34725_INTEGRATIONTIME_50MS);

    setGain(TCS34725_GAIN_4X);

    enable();
}

/* GET RAW DATA */
void TCS34725_GetRawData(uint16_t *r,
                         uint16_t *g,
                         uint16_t *b,
                         uint16_t *c)
{
    if(!initialised)
        return;

    *c = read16(TCS34725_CDATAL);

    *r = read16(TCS34725_RDATAL);

    *g = read16(TCS34725_GDATAL);

    *b = read16(TCS34725_BDATAL);

    HAL_Delay(50);
}

/* GET RGB */
void TCS34725_GetRGB(int *R,
                     int *G,
                     int *B)
{
    uint16_t r, g, b, c;

    TCS34725_GetRawData(&r, &g, &b, &c);

    if(c == 0)
    {
        *R = 0;
        *G = 0;
        *B = 0;
        return;
    }

    *R = r * 255 / c;
    *G = g * 255 / c;
    *B = b * 255 / c;
		
		if(*R < 115) *R = 0;
    if(*G < 115) *G = 0;
    if(*B < 98) *B = 0;
}