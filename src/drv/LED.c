#include"LED.h"
#include "../inc/sys/lib.h"

void Led_Init()
{
  rGPCCON=0x5400;         // GPC5~7 OUT 
}

void Led_off()
{
	rGPCDAT|=0xe0;// Led off and pouse high
        //Uart_Printf(0,"Led light off...");
        //Uart_Printf(0,"\n");
	
}
void Led1_on()
{
	rGPCDAT&=~0x20;// Led on and pouse low
        Uart_Printf(0,"Led1 light on...");
        Uart_Printf(0,"\n");
	
}
void Led2_on()
{
	rGPCDAT&=~0x40;// Led on and pouse low
        Uart_Printf(0,"Led2 light on...");
        Uart_Printf(0,"\n");
	
}
void Led3_on()
{
	rGPCDAT&=~0x80;// Led on and pouse low
        Uart_Printf(0,"Led3 light on...");
        Uart_Printf(0,"\n");
	
}