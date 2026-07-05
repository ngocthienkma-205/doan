#include "i2c_lcd.h"

extern I2C_HandleTypeDef hi2c2;

#define LCD_ADDRESS (0x27 << 1)

#define LCD_BACKLIGHT 0x08
#define LCD_RS        0x01
#define LCD_EN        0x04

void lcd_send_cmd(uint8_t cmd)
{
    uint8_t data_h, data_l;
    uint8_t data_t[4];

    data_h = cmd & 0xF0;
    data_l = (cmd << 4) & 0xF0;

    data_t[0] = data_h | LCD_EN | LCD_BACKLIGHT;
    data_t[1] = data_h | LCD_BACKLIGHT;
    data_t[2] = data_l | LCD_EN | LCD_BACKLIGHT;
    data_t[3] = data_l | LCD_BACKLIGHT;

    HAL_I2C_Master_Transmit(&hi2c2, LCD_ADDRESS, data_t, 4, 100);

    HAL_Delay(1);
}

void lcd_send_data(uint8_t data)
{
    uint8_t data_h, data_l;
    uint8_t data_t[4];

    data_h = data & 0xF0;
    data_l = (data << 4) & 0xF0;

    data_t[0] = data_h | LCD_RS | LCD_EN | LCD_BACKLIGHT;
    data_t[1] = data_h | LCD_RS | LCD_BACKLIGHT;
    data_t[2] = data_l | LCD_RS | LCD_EN | LCD_BACKLIGHT;
    data_t[3] = data_l | LCD_RS | LCD_BACKLIGHT;

    HAL_I2C_Master_Transmit(&hi2c2, LCD_ADDRESS, data_t, 4, 100);

    HAL_Delay(1);
}

void lcd_init(void)
{
    HAL_Delay(50);

    lcd_send_cmd(0x30);
    HAL_Delay(5);

    lcd_send_cmd(0x20);
    HAL_Delay(5);

    lcd_send_cmd(0x28);

    lcd_send_cmd(0x08);

    lcd_send_cmd(0x01);
    HAL_Delay(5);

    lcd_send_cmd(0x06);

    lcd_send_cmd(0x0C);
}

void lcd_send_string(char *str)
{
    while(*str)
    {
        lcd_send_data(*str++);
    }
}

void lcd_clear(void)
{
    lcd_send_cmd(0x01);
    HAL_Delay(2);
}

void lcd_gotoxy(uint8_t row, uint8_t col)
{
    uint8_t address;

    switch(row)
    {
        case 0:
            address = 0x80 + col;
            break;

        case 1:
            address = 0xC0 + col;
            break;

        default:
            return;
    }

    lcd_send_cmd(address);
}