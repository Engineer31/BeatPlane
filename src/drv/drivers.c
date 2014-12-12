/***************************************************************************\
	Copyright (c) 2004-2007 threewater@up-tech.com, All rights reserved.
	by threewter	2004.4.26
\***************************************************************************/
	

/***************************************************************************\
    #说明: 驱动程序定义
	----------------------------------  Bug  --------------------------------------

	----------------------------------  TODO list  --------------------------------------

	----------------------------------修正--------------------------------------
	2004-4-26	添加串口驱动程序s3c2410_serialx_driver

\***************************************************************************/

#include "../inc/macro.h"
#include "../inc/drivers.h"
#include "../inc/sys/sysdrv.h"

/***************************************************************************\
	for serial drivers
\***************************************************************************/
extern serial_driver_t s3c2410_serial0_driver, s3c2410_serial1_driver, 
		s3c2410_serial2_driver;
serial_driver_t* serial_drv[]={&s3c2410_serial0_driver, &s3c2410_serial1_driver, 
		&s3c2410_serial2_driver};
int NumberOfUartDrv=NumberOfArray(serial_drv);
struct_Uart_Buffer	Uart_Buffer[NumberOfArray(serial_drv)];


/***************************************************************************\
	for can drivers
\***************************************************************************/
//extern can_driver_t s3c2410_can0_driver, s3c2410_can1_driver;
//can_driver_t* can_driver[]={0};
//int Can_Device_Num=0;//NumberOfArray(can_driver);
//struct_CAN_Buffer CAN_Buffer[NumberOfArray(can_driver)];
//struct_CAN_Buffer *CAN_Buffer;

void GPIO_Init()
{
  set_gpio_ctrl(GPIO_C5|GPIO_MODE_OUT|GPIO_PULLUP_EN);//LED1
  set_gpio_ctrl(GPIO_C6|GPIO_MODE_OUT|GPIO_PULLUP_EN);//LED2
  set_gpio_ctrl(GPIO_C7|GPIO_MODE_OUT|GPIO_PULLUP_EN);//LED3
  
  set_gpio_ctrl(GPIO_F3|GPIO_MODE_IN);//UP
  set_gpio_ctrl(GPIO_F5|GPIO_MODE_IN);//LEFT
  set_gpio_ctrl(GPIO_G1|GPIO_MODE_IN);//RIGHT
  set_gpio_ctrl(GPIO_G3|GPIO_MODE_IN);//DOWN
  set_gpio_ctrl(GPIO_G4|GPIO_MODE_IN);//SEL

}