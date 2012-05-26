/*
 * taskschedule.c
 *
 *  Created on: Aug 29, 2011
 *      Author: PauloDitarso
 */

#include "simulation.h"

void TaskSchedule(event *ptrCurrentEvent, event *ptrEventList, machine *ptrMachineList, task *ptrTaskList, taskAccountInfo *ptrTaskAccountInfoList) {

	if (ptrMachineList && ptrTaskList) {

		task *ptrAuxTask;
		ptrAuxTask = ptrTaskList;
		unsigned short int found = 0;

		while(ptrAuxTask) {

			if ( ptrAuxTask->arrivalTime <= ptrCurrentEvent->time && ptrAuxTask->status == QUEUED ) {

				if (ptrAuxTask->taskID > 0) { // 0 means code for an empty task list

					machine *ptrAuxMachine;
					ptrAuxMachine = ptrMachineList;

					while(ptrAuxMachine) {

						if (ptrAuxMachine->status == IDLE || ptrAuxMachine->status == DONATING) {

							if (ptrAuxMachine->status == DONATING && ptrAuxMachine->source == LOCAL ) {

								// insert a new grid preemption into the event list
								event *ptrNewGridPreemption, *ptrTargetEvent;
								ptrTargetEvent = ptrCurrentEvent;

								if( (ptrNewGridPreemption = malloc(sizeof(event))) ) {
									ptrNewGridPreemption->eventNumber = 0;
									ptrNewGridPreemption->eventID = GRIDPREEMPTED;
									ptrNewGridPreemption->time = ptrCurrentEvent->time;
									ptrNewGridPreemption->machineInfo.machineID = ptrAuxMachine->machineID;
									ptrNewGridPreemption->machineInfo.source = ptrAuxMachine->source;
									ptrNewGridPreemption->machineInfo.status = RUNNING;
									ptrNewGridPreemption->machineInfo.arrivalTime = ptrAuxMachine->arrivalTime;
									ptrNewGridPreemption->machineInfo.departureTime = ptrAuxMachine->departureTime;
									ptrNewGridPreemption->machineInfo.reservationPrice = ptrAuxMachine->reservationPrice;
									ptrNewGridPreemption->machineInfo.usagePrice = ptrAuxMachine->usagePrice;
									ptrNewGridPreemption->machineInfo.nextMachine = ptrAuxMachine->nextMachine;
									ptrNewGridPreemption->nextEvent = NULL;

									InsertAfterEvent(ptrEventList, ptrNewGridPreemption, ptrTargetEvent);
								}
								else printf("ERROR (task schedule): merdou o malloc!!!\n");

							}

							found = 1;
							ptrAuxMachine->status = RUNNING;
							ptrAuxTask->status = STARTED;

							InsertTaskAccountList(ptrCurrentEvent, ptrAuxMachine, ptrAuxTask, ptrTaskAccountInfoList);

							event *ptrNewEvent;

							if( (ptrNewEvent = malloc(sizeof(event))) ) {
								ptrNewEvent->eventNumber = 0;
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

							if (ptrAuxTask->taskID == 1) {

								event *ptrNewEvent, *ptrTargetEvent;
								ptrTargetEvent = ptrCurrentEvent;

								if( (ptrNewEvent = malloc(sizeof(event))) ) {
									ptrNewEvent->eventNumber = 0;
									ptrNewEvent->eventID = JOBSTARTED;
									ptrNewEvent->time = ptrCurrentEvent->time;
									ptrNewEvent->jobInfo.jobID = ptrAuxTask->jobID;
									ptrNewEvent->nextEvent = NULL;

									InsertAfterEvent(ptrEventList, ptrNewEvent, ptrTargetEvent);
								}
								else {
									printf("ERROR (task schedule): merdou o malloc!!!\n");
								}

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

		if (found == 0) {
			printf("eventID %d (Task Scheduled) time %d (NOTHING_TO_DO!!!)\n", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
		}

	} else printf("ERROR (arrival): there is no machine or task list!!!\n");

}
