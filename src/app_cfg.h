#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__


/*******************�����������ȼ�*******************/
#define STARTUP_TASK_PRIO       0	   				//��ʼ��������������ȼ�
#define STARTGAME_TASK_PRIO     1					//��ʼ��Ϸ�������ȼ�
#define KEY_TASK_PRIO     2					      //�����������ȼ�
#define MYPLANE_TASK_PRIO 3                                            //���ɻ�����

/************����ջ��С����λΪ OS_STK ��************/
#define STARTUP_TASK_STK_SIZE   100   
#define STARTGAME_TASK_STK_SIZE 100
#define KEY_TASK_STK_SIZE 100
#define MYPLANE_TASK_STK_SIZE  200

#endif