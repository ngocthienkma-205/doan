#ifndef __TCS34725_H
#define __TCS34725_H

#include "main.h"

/* ADDRESS */
#define TCS34725_ADDRESS          (0x29 << 1)

/* REGISTERS */
#define TCS34725_COMMAND_BIT      0x80
#define TCS34725_ENABLE           0x00
#define TCS34725_ENABLE_AEN       0x02
#define TCS34725_ENABLE_PON       0x01
#define TCS34725_ATIME            0x01
#define TCS34725_CONTROL          0x0F
#define TCS34725_ID               0x12

#define TCS34725_CDATAL           0x14
#define TCS34725_RDATAL           0x16
#define TCS34725_GDATAL           0x18
#define TCS34725_BDATAL           0x1A

/* INTEGRATION TIME */
#define TCS34725_INTEGRATIONTIME_50MS    0xEB
#define TCS34725_INTEGRATIONTIME_154MS   0xC0

/* GAIN */
#define TCS34725_GAIN_1X          0x00
#define TCS34725_GAIN_4X          0x01
#define TCS34725_GAIN_16X         0x02
#define TCS34725_GAIN_60X         0x03

void TCS34725_Init(I2C_HandleTypeDef *hi2c);

void TCS34725_GetRawData(uint16_t *r,
                         uint16_t *g,
                         uint16_t *b,
                         uint16_t *c);

void TCS34725_GetRGB(int *R,
                     int *G,
                     int *B);

#endif