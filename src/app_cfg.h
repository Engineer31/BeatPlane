#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__


/*******************�����������ȼ�*******************/
#define STARTUP_TASK_PRIO       0	   				//��ʼ��������������ȼ�
#define STARTGAME_TASK_PRIO     1					//��ʼ��Ϸ�������ȼ�
#define KEY_TASK_PRIO     2					      //�����������ȼ�

#define MYPLANE_TASK_PRIO 3                                            //���ɻ��������ȼ�
#define LCD_TASK_PRIO     4                                            //LCD��ʾ���ȼ�
#define SHOT_TASK_PRIO    6
#define ENEMY_TASK_PRIO    7
#define STATUS_TASK_PRIO    5

/************����ջ��С����λΪ OS_STK ��************/
#define STARTUP_TASK_STK_SIZE   50   
#define STARTGAME_TASK_STK_SIZE 50
#define KEY_TASK_STK_SIZE 100
#define MYPLANE_TASK_STK_SIZE  100
#define LCD_TASK_STK_SIZE 150
#define SHOT_TASK_STK_SIZE 100
#define ENEMY_TASK_STK_SIZE 100
#define STATUS_TASK_STK_SIZE 100

#endif