/*
 * taskarrival.c
 *
 *  Created on: Aug 26, 2011
 *      Author: PauloDitarso
 */

#include "simulation.h"

void TaskArrival(event *ptrCurrentEvent, event *ptrEventList, task *ptrTaskList, machine *ptrMachineList) {

	if (ptrCurrentEvent->eventID == TASKARRIVAL) {

		if (ptrTaskList) {

			if (ptrTaskList->taskID == 0) {		// code for an empty task list
				ptrTaskList->taskID = ptrCurrentEvent->taskInfo.taskID;
				ptrTaskList->arrivalTime = ptrCurrentEvent->taskInfo.arrivalTime;
				ptrTaskList->jobID = ptrCurrentEvent->taskInfo.jobID;
				ptrTaskList->jobSize = ptrCurrentEvent->taskInfo.jobSize;
				ptrTaskList->runtime = ptrCurrentEvent->taskInfo.runtime;
				ptrTaskList->status = ptrCurrentEvent->taskInfo.status;
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
				ptrNewTask->nextTask = NULL;

				task *ptrAux;

				ptrAux = ptrTaskList;

				while(ptrAux->nextTask != NULL) {
					ptrAux = ptrAux->nextTask;
				}

				ptrAux->nextTask = ptrNewTask;

			} // end else


			printf("eventID %d (Task Arrival) time %d ", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
			printf("taskID %d jobID %d jobSize %d AT %d RT %d status %d\n",
					ptrCurrentEvent->taskInfo.taskID, ptrCurrentEvent->taskInfo.jobID,
					ptrCurrentEvent->taskInfo.jobSize, ptrCurrentEvent->taskInfo.arrivalTime,
					ptrCurrentEvent->taskInfo.runtime, ptrCurrentEvent->taskInfo.status);

		} else printf("ERROR (task arrival): there is no task list!!!\n");

	} else printf("ERROR (task arrival): wrong eventID!!!\n");
}
