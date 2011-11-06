/*
 * taskschedule.c
 *
 *  Created on: Aug 29, 2011
 *      Author: PauloDitarso
 */

#include "simulation.h"

void TaskSchedule(event *ptrCurrentEvent, event *ptrEventList, machine *ptrMachineList, task *ptrTaskList, accountInfo *ptrAccountInfoList) {

	if (ptrMachineList && ptrTaskList) {

		task *ptrAuxTask;
		ptrAuxTask = ptrTaskList;

		while(ptrAuxTask) {

			if ( ptrAuxTask->arrivalTime <= ptrCurrentEvent->time && ptrAuxTask->status == QUEUED ) {

				if (ptrAuxTask->taskID > 0) { // 0 means code for an empty task list

					machine *ptrAuxMachine;
					ptrAuxMachine = ptrMachineList;

					while(ptrAuxMachine) {

						if (ptrAuxMachine->status == IDLE) {

							ptrAuxMachine->status = RUNNING;
							ptrAuxTask->status = STARTED;

							InsertAccountList(ptrCurrentEvent, ptrAuxMachine, ptrAuxTask, ptrAccountInfoList);

							// insert a new grid preemption into the event list
							event *ptrNewGridPreemption;

							if ( ptrAuxMachine->source == LOCAL ) {

								if( (ptrNewGridPreemption = malloc(sizeof(event))) ) {
									ptrNewGridPreemption->eventID = GRIDPREEMPTED;
									ptrNewGridPreemption->time = ptrCurrentEvent->time;
									ptrNewGridPreemption->flag = 0;
									ptrNewGridPreemption->nextEvent = NULL;

									InsertEvent(ptrEventList, ptrNewGridPreemption);
								}
								else printf("ERROR (task schedule): merdou o malloc!!!\n");

							}

							event *ptrNewEvent;

							if( (ptrNewEvent = malloc(sizeof(event))) ) {
								ptrNewEvent->eventID = TASKFINNISHED;
								ptrNewEvent->time = (ptrCurrentEvent->time + ptrAuxTask->runtime);
								ptrNewEvent->taskInfo.taskID = ptrAuxTask->taskID;
								ptrNewEvent->taskInfo.arrivalTime = ptrAuxTask->arrivalTime;
								ptrNewEvent->taskInfo.jobID = ptrAuxTask->jobID;
								ptrNewEvent->taskInfo.jobSize = ptrAuxTask->jobSize;
								ptrNewEvent->taskInfo.runtime = ptrAuxTask->runtime;
								ptrNewEvent->taskInfo.status = FINNISHED;
								ptrNewEvent->taskInfo.utilityFunction = 0.0;
								ptrNewEvent->taskInfo.nextTask = NULL;
								ptrNewEvent->nextEvent = NULL;

								InsertEvent(ptrEventList, ptrNewEvent);
							}
							else {
								printf("ERROR (task schedule): merdou o malloc!!!\n");
							}

							printf("eventID %d (Task Scheduled) time %d ", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
							printf("taskID %d jobID %d machineID %d source %d\n", ptrAuxTask->taskID,
									ptrAuxTask->jobID, ptrAuxMachine->machineID, ptrAuxMachine->source);

							break;
						}

						ptrAuxMachine = ptrAuxMachine->nextMachine;
					}

				} // else printf("lista vazia!!!\n");

				break;
			}

			ptrAuxTask = ptrAuxTask->nextTask;
		}

	} else printf("ERROR (arrival): there is no machine or task list!!!\n");

}
