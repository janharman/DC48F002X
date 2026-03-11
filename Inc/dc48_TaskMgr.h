/*
 * dc48_TaskMgr.h
 *
 *  Created on: 30. 3. 2017
 *      Author: J�n Harman
 */

#ifndef SOURCES_EIL_TASKMGR_H_
#define SOURCES_EIL_TASKMGR_H_

#define NUMBER_OF_TASKS			20
#define TSK_TYPE_EMPTY_TASK		0
#define TSK_TYPE_ONE_SHOT_TASK	1
#define TSK_TYPE_CYCLIC_TASK	2
#define TSK_TYPE_ONCE_AND_KILL	3

typedef void (*TaskFnc)(void);

typedef struct
{
	int TaskType;
	int Interval;
	int Counter;
	TaskFnc TaskFunction;
}
	TTaskStruct;

extern TTaskStruct Task[NUMBER_OF_TASKS];

void Task_EmptyAllTasks(void);
int Task_Create_New(int interval, int task_type, TaskFnc fnc);
void Task_CheckTasksToExe(void);
void Task_DecrementCounters(void);
void Task_SetInterval(int task_id, int interval);

#endif /* SOURCES_EIL_TASKMGR_H_ */
