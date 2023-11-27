/* Raspberripi 4 register view for GPIO*/
/* BCM2711 ARM Peripherals */

#define GPIO_BASE 0xFE200000

#define GPFSEL0	0x00 //Offset of the GPIO Function select register 0
#define GPFSEL1	0x04 //Offset of the GPIO Function select register 1
#define GPFSEL2	0x08 //Offset of the GPIO Function select register 2
#define GPFSEL3	0x0C //Offset of the GPIO Function select register 3
#define GPFSEL4	0x10 //Offset of the GPIO Function select register 4
#define GPFSEL5	0x14 //Offset of the GPIO Function select register 5

#define GPSET0	0x1C //Offset of the GPIO set register 0
#define GPSET1	0x20 //Offset of the GPIO set register 1

#define GPCRL0	0x28 //Offset of the GPIO Clear register 0
#define GPCRL1	0x2C //Offset of the GPIO Clear register 1

#define GPLEV0	0x34 //Offset of the GPIO Level register 0
#define GPLEV1	0x38 //Offset of the GPIO Level register 1




