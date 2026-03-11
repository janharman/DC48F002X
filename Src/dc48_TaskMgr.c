/*
 * dc48_TaskMgr.c
 *
 *  Created on: 30. 3. 2017
 *      Author: J�n Harman
 */

#include <dc48_TaskMgr.h>

/* GLOBAL VARIABLES */
TTaskStruct Task[NUMBER_OF_TASKS];

/* LOCAL VARIABLES */
int TaskNr = 0;

#define NOT_ACTIVE_TASK_COUNTER_VAL			-1
#define TASK_ID_BASE_NR						1000

void Task_EmptyAllTasks(void)
{
	int i;
	for (i=0; i<NUMBER_OF_TASKS; i++)
	{
		Task[i].Counter = NOT_ACTIVE_TASK_COUNTER_VAL;
		Task[i].TaskType = TSK_TYPE_EMPTY_TASK;
	}
}

int Task_Create_New(int interval, int task_type, TaskFnc fnc)
{
	int i;
	for (i=0; i<NUMBER_OF_TASKS; i++)
		if (Task[i].TaskType == TSK_TYPE_EMPTY_TASK)
		{
			Task[i].TaskType = task_type;
			Task[i].TaskFunction = fnc;
			Task[i].Interval = interval;
			Task[i].Counter = interval;
			return TASK_ID_BASE_NR+i;
		}
	return 0;
}

void Task_DecrementCounters(void)
{
	int i;
	for (i=0; i<NUMBER_OF_TASKS; i++)
		if (Task[i].Counter > 0) Task[i].Counter--;
}

void Task_CheckTasksToExe(void)
{
	if (!Task[TaskNr].Counter)
	{
		Task[TaskNr].TaskFunction();	// Execute Assigned Task Function
		switch (Task[TaskNr].TaskType)
		{
		case TSK_TYPE_ONE_SHOT_TASK:
				Task[TaskNr].Counter = NOT_ACTIVE_TASK_COUNTER_VAL;
				break;
		case TSK_TYPE_CYCLIC_TASK:
				Task[TaskNr].Counter = Task[TaskNr].Interval;
				break;
		case TSK_TYPE_ONCE_AND_KILL:
				Task[TaskNr].Counter = NOT_ACTIVE_TASK_COUNTER_VAL;
				Task[TaskNr].TaskType = TSK_TYPE_EMPTY_TASK;
				break;
		}
	}
	TaskNr++;
	if (TaskNr >= NUMBER_OF_TASKS) TaskNr = 0;
}

void Task_SetInterval(int task_id, int interval)
{
	Task[task_id-TASK_ID_BASE_NR].Interval = interval;
	Task[task_id-TASK_ID_BASE_NR].Counter = interval;
}

