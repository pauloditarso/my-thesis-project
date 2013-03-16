/*
 * taskarrival.c
 *
 *  Created on: Aug 26, 2011
 *      Author: PauloDitarso
 */

#include "simulation.h"

void TaskArrival(event *ptrCurrentEvent, event *ptrEventList, task *ptrTaskList, machine *ptrMachineList, task **ptrPtrOrderedTaskList) {

	if (ptrCurrentEvent->eventID == TASKARRIVAL) {

		if (ptrTaskList) {

			if (ptrTaskList->taskID == 0) {		// code for an empty task list

				// filling the normal task list
				ptrTaskList->taskID = ptrCurrentEvent->taskInfo.taskID;
				ptrTaskList->arrivalTime = ptrCurrentEvent->taskInfo.arrivalTime;
				ptrTaskList->jobID = ptrCurrentEvent->taskInfo.jobID;
				ptrTaskList->jobSize = ptrCurrentEvent->taskInfo.jobSize;
				ptrTaskList->runtime = ptrCurrentEvent->taskInfo.runtime;
				ptrTaskList->status = ptrCurrentEvent->taskInfo.status;
				ptrTaskList->numberOfSubmissions = ptrCurrentEvent->taskInfo.numberOfSubmissions;
				ptrTaskList->nextTask = NULL;

				// filling the ordered task list
				(*ptrPtrOrderedTaskList)->taskID = ptrCurrentEvent->taskInfo.taskID;
				(*ptrPtrOrderedTaskList)->arrivalTime = ptrCurrentEvent->taskInfo.arrivalTime;
				(*ptrPtrOrderedTaskList)->jobID = ptrCurrentEvent->taskInfo.jobID;
				(*ptrPtrOrderedTaskList)->jobSize = ptrCurrentEvent->taskInfo.jobSize;
				(*ptrPtrOrderedTaskList)->runtime = ptrCurrentEvent->taskInfo.runtime;
				(*ptrPtrOrderedTaskList)->status = ptrCurrentEvent->taskInfo.status;
				(*ptrPtrOrderedTaskList)->numberOfSubmissions = ptrCurrentEvent->taskInfo.numberOfSubmissions;
				(*ptrPtrOrderedTaskList)->nextTask = NULL;

			}
			else {

				// filling the normal task list
				task *ptrNewTask, *ptrAux;
				ptrAux = ptrTaskList;

				if ( !(ptrNewTask = malloc(sizeof(task))) ) {
					printf("ERROR (task arrival): merdou o malloc!!!\n");
				}

				ptrNewTask->taskID = ptrCurrentEvent->taskInfo.taskID;
				ptrNewTask->arrivalTime = ptrCurrentEvent->taskInfo.arrivalTime;
				ptrNewTask->jobID = ptrCurrentEvent->taskInfo.jobID;
				ptrNewTask->jobSize = ptrCurrentEvent->taskInfo.jobSize;
				ptrNewTask->runtime = ptrCurrentEvent->taskInfo.runtime;
				ptrNewTask->status = ptrCurrentEvent->taskInfo.status;
				ptrNewTask->numberOfSubmissions = ptrCurrentEvent->taskInfo.numberOfSubmissions;
				ptrNewTask->nextTask = NULL;

				while(ptrAux->nextTask != NULL) {
					ptrAux = ptrAux->nextTask;
				}

				ptrAux->nextTask = ptrNewTask;

				// filling the orderede task list
				task *ptrNewOrderedTask, *ptrAuxActual, *ptrAuxLast;
				ptrAuxActual = ptrAuxLast = (*ptrPtrOrderedTaskList);

				if ( !(ptrNewOrderedTask = malloc(sizeof(task))) ) {
					printf("ERROR (task arrival): merdou o malloc!!!\n");
				}

				ptrNewOrderedTask->taskID = ptrCurrentEvent->taskInfo.taskID;
				ptrNewOrderedTask->arrivalTime = ptrCurrentEvent->taskInfo.arrivalTime;
				ptrNewOrderedTask->jobID = ptrCurrentEvent->taskInfo.jobID;
				ptrNewOrderedTask->jobSize = ptrCurrentEvent->taskInfo.jobSize;
				ptrNewOrderedTask->runtime = ptrCurrentEvent->taskInfo.runtime;
				ptrNewOrderedTask->status = ptrCurrentEvent->taskInfo.status;
				ptrNewOrderedTask->numberOfSubmissions = ptrCurrentEvent->taskInfo.numberOfSubmissions;
				ptrNewOrderedTask->nextTask = NULL;

				while(ptrAuxActual) {

					if ( ptrAuxActual->jobID == ptrCurrentEvent->taskInfo.jobID && ptrNewOrderedTask->runtime > ptrAuxActual->runtime ) break;

					ptrAuxLast = ptrAuxActual;
					ptrAuxActual = ptrAuxActual->nextTask;

				}

				if (ptrAuxActual) {

					if (ptrAuxActual == ptrAuxLast) {
						ptrNewOrderedTask->nextTask = ptrAuxActual;
						*ptrPtrOrderedTaskList = ptrNewOrderedTask; // alterando o ponteiro para o inicio da lista
					}
					else {
						ptrNewOrderedTask->nextTask = ptrAuxActual;
						ptrAuxLast->nextTask = ptrNewOrderedTask;
					}

				}
				else {
					ptrAuxLast->nextTask = ptrNewOrderedTask;
				}

			} // end else


			printf("eventID %d (Task Arrival) time %ld ", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
			printf("taskID %d jobID %d jobSize %d AT %ld RT %d status %d submissions %d\n",
					ptrCurrentEvent->taskInfo.taskID, ptrCurrentEvent->taskInfo.jobID,
					ptrCurrentEvent->taskInfo.jobSize, ptrCurrentEvent->taskInfo.arrivalTime,
					ptrCurrentEvent->taskInfo.runtime, ptrCurrentEvent->taskInfo.status, ptrCurrentEvent->taskInfo.numberOfSubmissions);

		} else printf("ERROR (task arrival): there is no task list!!!\n");

	} else printf("ERROR (task arrival): wrong eventID!!!\n");
}
