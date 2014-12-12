#ifndef	_APP_H_
#define	_APP_H_

/************ 用户任务声明 ******************************/
void Task_Start(void *p_arg);
void Task_StartGame(void *p_arg);
void Task_MyPlane(void *p_arg);
void Task_Key(void *Id);
void Task_LCD(void *p_arg);

/************ 用户事件声明 ******************************/
extern OS_EVENT *Key_Mbox;

#endif	//_APP_H_