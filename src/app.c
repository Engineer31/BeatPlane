#include "includes.h"

/*--------设置任务堆栈--------*/
static OS_STK Count_task_stk[COUNT_TASK_STK_SIZE];		  		//定义Count任务的栈
static OS_STK Touch_task_stk[TOUCH_TASK_STK_SIZE];		  		//定义Touch任务的栈

//启动任务
void Task_Start(void *p_arg)
{
          
    OSRunning=TRUE;	//begin OS
    uHALr_InstallSystemTimer();//初始化系统时钟
    
    (void)p_arg;// 'p_arg' 并没有用到，防止编译器提示警告
    
    Led_off();
    /*
    Touch_Q=OSQCreate(&MsgGrp[0],N_MESSAGES); 					//创建消息队列

    OSTaskCreate(Task_Touch,(void *)0,
	   &Touch_task_stk[TOUCH_TASK_STK_SIZE-1],TOUCH_TASK_PRIO);	//创建触摸任务

    OSTaskCreate(Task_Count,(void *)0,							//创建计算任务
	   &Count_task_stk[COUNT_TASK_STK_SIZE-1],COUNT_TASK_PRIO);
   */
    
    while (1)
    {
        Led1_on();
        OSTimeDlyHMSM(0, 0,0,500);
        Led_off();
        OSTimeDlyHMSM(0, 0,0,500);
    }
}