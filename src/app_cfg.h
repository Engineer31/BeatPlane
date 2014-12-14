#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__


/*******************设置任务优先级*******************/
#define STARTUP_TASK_PRIO       0	   				//开始启动的任务的优先级
#define STARTGAME_TASK_PRIO     1					//开始游戏任务优先级
#define KEY_TASK_PRIO     2					      //键盘任务优先级

#define MYPLANE_TASK_PRIO 3                                            //主飞机任务优先级
#define LCD_TASK_PRIO     4                                            //LCD显示优先级
#define SHOT_TASK_PRIO    6
#define ENEMY_TASK_PRIO    7
#define STATUS_TASK_PRIO    5

/************设置栈大小（单位为 OS_STK ）************/
#define STARTUP_TASK_STK_SIZE   50   
#define STARTGAME_TASK_STK_SIZE 50
#define KEY_TASK_STK_SIZE 100
#define MYPLANE_TASK_STK_SIZE  100
#define LCD_TASK_STK_SIZE 150
#define SHOT_TASK_STK_SIZE 100
#define ENEMY_TASK_STK_SIZE 100
#define STATUS_TASK_STK_SIZE 100

#endif