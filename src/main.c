#include "includes.h"

OS_STK startup_task_stk[STARTUP_TASK_STK_SIZE];		  //∂®“Â’ª

/////////////////////////////////////////////////////
//                  Main function.                //
////////////////////////////////////////////////////
int main(void)
{
    ARMTargetInit();        // do target (uHAL based ARM system) initialisation //
    LCD_Init();
    GPIO_Init(); 
    OSInit();               // needed by uC/OS-II //
    
    OSTaskCreate(Task_Start,(void *)0,
	   &startup_task_stk[STARTUP_TASK_STK_SIZE-1], STARTUP_TASK_PRIO);

    OSStart();
    return 0;
}//main


