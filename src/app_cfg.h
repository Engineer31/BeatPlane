#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__


/*******************设置任务优先级*******************/
#define STARTUP_TASK_PRIO       0	   				//开始启动的任务的优先级
#define TOUCH_TASK_PRIO       	1					//触摸优先级
#define COUNT_TASK_PRIO       	6					//计算优先级

/************设置栈大小（单位为 OS_STK ）************/
#define STARTUP_TASK_STK_SIZE   100   
#define COUNT_TASK_STK_SIZE   	100   
#define TOUCH_TASK_STK_SIZE   	100 

#endif