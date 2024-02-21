#ifndef __LCDDRIVER_H__
#define __LCDDRIVER_H__

#define LCD_SYSTEM_CLOCK_SPEED                 48000000
// SPI clock speed (in Hz)
#define LCD_SPI_CLOCK_SPEED                    16000000


#define LCD_SCK_PORT          GPIO_PORT_P1
#define LCD_SCK_PIN_FUNCTION  GPIO_PRIMARY_MODULE_FUNCTION
#define LCD_MOSI_PORT         GPIO_PORT_P1
#define LCD_MOSI_PIN_FUNCTION GPIO_PRIMARY_MODULE_FUNCTION
#define LCD_RST_PORT          GPIO_PORT_P5
#define LCD_CS_PORT           GPIO_PORT_P5
#define LCD_DC_PORT           GPIO_PORT_P3

// Pins from MSP432 connected to LCD
#define LCD_SCK_PIN           GPIO_PIN5
#define LCD_MOSI_PIN          GPIO_PIN6
#define LCD_RST_PIN           GPIO_PIN7
#define LCD_CS_PIN            GPIO_PIN0
#define LCD_DC_PIN            GPIO_PIN7

#define LCD_EUSCI_BASE        EUSCI_B0_BASE

#define LCD_VERTICAL_MAX                   128
#define LCD_HORIZONTAL_MAX                 128

#define LCD_ORIENTATION_UP    0
#define LCD_ORIENTATION_LEFT  1
#define LCD_ORIENTATION_DOWN  2
#define LCD_ORIENTATION_RIGHT 3

// ST7735 LCD controller Command Set
#define CM_NOP             0x00
#define CM_SWRESET         0x01
#define CM_RDDID           0x04
#define CM_RDDST           0x09
#define CM_SLPIN           0x10
#define CM_SLPOUT          0x11
#define CM_PTLON           0x12
#define CM_NORON           0x13
#define CM_INVOFF          0x20
#define CM_INVON           0x21
#define CM_GAMSET          0x26
#define CM_DISPOFF         0x28
#define CM_DISPON          0x29
#define CM_CASET           0x2A
#define CM_RASET           0x2B
#define CM_RAMWR           0x2C
#define CM_RGBSET          0x2d
#define CM_RAMRD           0x2E
#define CM_PTLAR           0x30
#define CM_MADCTL          0x36
#define CM_COLMOD          0x3A
#define CM_SETPWCTR        0xB1
#define CM_SETDISPL        0xB2
#define CM_FRMCTR3         0xB3
#define CM_SETCYC          0xB4
#define CM_SETBGP          0xb5
#define CM_SETVCOM         0xB6
#define CM_SETSTBA         0xC0
#define CM_SETID           0xC3
#define CM_GETHID          0xd0
#define CM_SETGAMMA        0xE0
#define CM_MADCTL_MY       0x80
#define CM_MADCTL_MX       0x40
#define CM_MADCTL_MV       0x20
#define CM_MADCTL_ML       0x10
#define CM_MADCTL_BGR      0x08
#define CM_MADCTL_MH       0x04

// Custom __delay_cycles() for non CCS Compiler
#if !defined( __TI_ARM__ )
#undef __delay_cycles
#define __delay_cycles(x)     SysCtlDelay(x)
void SysCtlDelay(uint32_t);
#endif

#define HAL_LCD_delay(x)      __delay_cycles(x * 48)


uint8_t Lcd_Orientation;
uint16_t Lcd_ScreenWidth, Lcd_ScreenHeigth;
uint8_t Lcd_PenSolid, Lcd_FontSolid, Lcd_FlagRead;
uint16_t Lcd_TouchTrim;

extern Graphics_Display g_sCrystalfontz128x128;

extern const Graphics_Display_Functions g_sCrystalfontz128x128_funcs;

extern void Crystalfontz128x128_Init(void);

extern void Crystalfontz128x128_SetDrawFrame(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

extern void Crystalfontz128x128_SetOrientation(uint8_t orientation);

#endif

