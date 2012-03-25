/*
 * taskarrival.c
 *
 *  Created on: Aug 26, 2011
 *      Author: PauloDitarso
 */

#include "simulation.h"

void TaskArrival(event *ptrCurrentEvent, event *ptrEventList, task *ptrTaskList) {

	if (ptrCurrentEvent->eventID == TASKARRIVAL) {

		if (ptrTaskList) {

			if (ptrTaskList->taskID == 0) {		// code for an empty task list
				ptrTaskList->taskID = ptrCurrentEvent->taskInfo.taskID;
				ptrTaskList->arrivalTime = ptrCurrentEvent->taskInfo.arrivalTime;
				ptrTaskList->jobID = ptrCurrentEvent->taskInfo.jobID;
				ptrTaskList->jobSize = ptrCurrentEvent->taskInfo.jobSize;
				ptrTaskList->runtime = ptrCurrentEvent->taskInfo.runtime;
				ptrTaskList->status = ptrCurrentEvent->taskInfo.status;
				ptrTaskList->utilityFunction = ptrCurrentEvent->taskInfo.utilityFunction;
				ptrTaskList->nextTask = NULL;
			}
			else {

				task *ptrNewTask;

				if ( !(ptrNewTask = malloc(sizeof(task))) ) {
					printf("ERROR (task arrival): merdou o malloc!!!\n");
				}

				ptrNewTask->taskID = ptrCurrentEvent->taskInfo.taskID;
				ptrNewTask->arrivalTime = ptrCurrentEvent->taskInfo.arrivalTime;
				ptrNewTask->jobID = ptrCurrentEvent->taskInfo.jobID;
				ptrNewTask->jobSize = ptrCurrentEvent->taskInfo.jobSize;
				ptrNewTask->runtime = ptrCurrentEvent->taskInfo.runtime;
				ptrNewTask->status = ptrCurrentEvent->taskInfo.status;
				ptrNewTask->utilityFunction = ptrCurrentEvent->taskInfo.utilityFunction;
				ptrNewTask->nextTask = NULL;

				task *ptrAux;

				ptrAux = ptrTaskList;

				while(ptrAux->nextTask != NULL) {
					ptrAux = ptrAux->nextTask;
				}

				ptrAux->nextTask = ptrNewTask;

			} // end else

			// insert a new event into the event list
			event *ptrNewEvent, *ptrTargetEvent;
			ptrTargetEvent = ptrCurrentEvent;

			if( (ptrNewEvent = malloc(sizeof(event))) ) {
				ptrNewEvent->eventNumber = 0;
				ptrNewEvent->eventID = TASKSCHEDULE;
				ptrNewEvent->time = ptrCurrentEvent->time;
				ptrNewEvent->flag = 0;
				ptrNewEvent->nextEvent = NULL;

				InsertAfterEvent(ptrEventList, ptrNewEvent, ptrTargetEvent);
			}
			else printf("ERROR (task arrival): merdou o malloc!!!\n");

			printf("eventID %d (Task Arrival) time %d ", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
			printf("taskID %d jobID %d AT %d jobSize %d runtime %d status %d utility %f\n",
					ptrCurrentEvent->taskInfo.taskID, ptrCurrentEvent->taskInfo.jobID,
					ptrCurrentEvent->taskInfo.arrivalTime, ptrCurrentEvent->taskInfo.jobSize,
					ptrCurrentEvent->taskInfo.runtime, ptrCurrentEvent->taskInfo.status,
					ptrCurrentEvent->taskInfo.utilityFunction);

		} else printf("ERROR (task arrival): there is no task list!!!\n");

	} else printf("ERROR (task arrival): wrong eventID!!!\n");
}
