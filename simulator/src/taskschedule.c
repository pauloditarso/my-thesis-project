/*
 * taskschedule.c
 *
 *  Created on: Aug 29, 2011
 *      Author: PauloDitarso
 *
 *  What is done:
 *  	1. it tests if it is the correct event;
 *  	2. if the machine is local and it is donating, it creates a grid preemption event;
 *  	3. it changes the machine status to RUNNING and the task status to STARTED;
 *  	4. it inserts the entry in the TaskAccountList;
 *  	5. it creates a task finish event;
 *  	6. it creates a job start event if it is the first task to be executed from that job;
 */

#include "simulation.h"

void TaskSchedule(event *ptrCurrentEvent, event *ptrEventList, machine *ptrMachineList, task *ptrTaskList, taskAccountInfo *ptrTaskAccountInfoList) {

	if (ptrCurrentEvent->eventID == TASKSCHEDULE) {

		if (ptrMachineList && ptrTaskList) {

			task *ptrAuxTask;
			ptrAuxTask = ptrTaskList;
//			unsigned short int found = 0;

			while(ptrAuxTask) {

				if ( ptrAuxTask->taskID == ptrCurrentEvent->scheduleInfo.taskID && ptrAuxTask->jobID == ptrCurrentEvent->scheduleInfo.jobID ) {

					machine *ptrAuxMachine;
					ptrAuxMachine = ptrMachineList;

					while(ptrAuxMachine) {

						if (ptrAuxMachine->machineID == ptrCurrentEvent->scheduleInfo.machineID && ptrAuxMachine->source == ptrCurrentEvent->scheduleInfo.source) {

							// insert a new grid preemption into the event list
//							if (ptrAuxMachine->source == LOCAL && ptrAuxMachine->status == DONATING) {
//
//								event *ptrNewGridPreemption, *ptrTargetEvent;
//								ptrTargetEvent = ptrCurrentEvent;
//
//								if( (ptrNewGridPreemption = malloc(sizeof(event))) ) {
//									ptrNewGridPreemption->eventNumber = 0;
//									ptrNewGridPreemption->eventID = GRIDPREEMPTED;
//									ptrNewGridPreemption->time = ptrCurrentEvent->time;
//									ptrNewGridPreemption->machineInfo.machineID = ptrAuxMachine->machineID;
//									ptrNewGridPreemption->machineInfo.source = ptrAuxMachine->source;
//									ptrNewGridPreemption->machineInfo.status = RUNNING;
//									ptrNewGridPreemption->machineInfo.arrivalTime = ptrAuxMachine->arrivalTime;
//									ptrNewGridPreemption->machineInfo.departureTime = ptrAuxMachine->departureTime;
//									ptrNewGridPreemption->machineInfo.reservationPrice = ptrAuxMachine->reservationPrice;
//									ptrNewGridPreemption->machineInfo.usagePrice = ptrAuxMachine->usagePrice;
//									ptrNewGridPreemption->machineInfo.nextMachine = ptrAuxMachine->nextMachine;
//									ptrNewGridPreemption->nextEvent = NULL;
//
//									InsertAfterEvent(ptrEventList, ptrNewGridPreemption, ptrTargetEvent);
//								}
//								else printf("ERROR (task schedule): merdou o malloc!!!\n");
//
//							}

//							found = 1;
							ptrAuxMachine->status = RUNNING;  // SOH PRA CONFIRMAS, POIS JAH ESTAVA RUNNING A UM SEGUNDO ATRAS (VIDE AllocationPlanning())
							ptrAuxTask->status = STARTED;

							InsertTaskAccountList(ptrCurrentEvent, ptrAuxMachine, ptrAuxTask, ptrTaskAccountInfoList);

							// insert a task finish event into the event list
							event *ptrNewEvent;

							if( (ptrNewEvent = malloc(sizeof(event))) ) {
								ptrNewEvent->eventNumber = 0;
								ptrNewEvent->eventID = TASKFINNISHED;
								ptrNewEvent->time = (ptrCurrentEvent->time + ptrAuxTask->runtime);
								ptrNewEvent->taskInfo.taskID = ptrAuxTask->taskID;
								ptrNewEvent->taskInfo.jobID = ptrAuxTask->jobID;
								ptrNewEvent->taskInfo.jobSize = ptrAuxTask->jobSize;
								ptrNewEvent->taskInfo.arrivalTime = ptrAuxTask->arrivalTime;
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

							// insert a job start event into the event list
							unsigned short int howManyTasksAreRunningFromAJob = 0;
							while(ptrTaskAccountInfoList) {

								if (ptrTaskAccountInfoList->jobID == ptrAuxTask->jobID)	howManyTasksAreRunningFromAJob += 1;

								ptrTaskAccountInfoList = ptrTaskAccountInfoList->nextTaskAccountInfo;
							}

							if (howManyTasksAreRunningFromAJob == 1) {

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

					break;
				}

				ptrAuxTask = ptrAuxTask->nextTask;
			}

//			if (found == 0) {
//				printf("eventID %d (Task Scheduled) time %d (NOTHING_TO_DO!!!)\n", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
//			}

		} else printf("ERROR (arrival): there is no machine or task list!!!\n");

	} else printf("ERROR (task scheduling): wrong eventID!!!\n");
}
