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

void TaskSchedule(event *ptrCurrentEvent, event *ptrEventList, machine *ptrMachineList, task *ptrTaskList, taskAccountInfo *ptrTaskAccountInfoList, schedule *ptrScheduleList) {

	if (ptrCurrentEvent->eventID == TASKSCHEDULE) {

		if (ptrScheduleList) {

			if (ptrScheduleList->scheduleID == 0) { // code for an empty job list

				ptrScheduleList->scheduleID = ptrCurrentEvent->scheduleInfo.scheduleID;
				ptrScheduleList->scheduleTime = ptrCurrentEvent->scheduleInfo.scheduleTime;
				ptrScheduleList->taskID = ptrCurrentEvent->scheduleInfo.taskID;
				ptrScheduleList->jobID = ptrCurrentEvent->scheduleInfo.jobID;
				ptrScheduleList->runtime = ptrCurrentEvent->scheduleInfo.runtime;
				ptrScheduleList->machineID = ptrCurrentEvent->scheduleInfo.machineID;
				ptrScheduleList->source = ptrCurrentEvent->scheduleInfo.source;
				ptrScheduleList->nextSchedule = NULL;

			}
			else {

				schedule *ptrNewSchedule;

				if ( !(ptrNewSchedule = malloc(sizeof(schedule))) ) {
									printf("ERROR (task schedule): merdou o malloc!!!\n");
				}

				ptrNewSchedule->scheduleID = ptrCurrentEvent->scheduleInfo.scheduleID;
				ptrNewSchedule->scheduleTime = ptrCurrentEvent->scheduleInfo.scheduleTime;
				ptrNewSchedule->taskID = ptrCurrentEvent->scheduleInfo.taskID;
				ptrNewSchedule->jobID = ptrCurrentEvent->scheduleInfo.jobID;
				ptrNewSchedule->runtime = ptrCurrentEvent->scheduleInfo.runtime;
				ptrNewSchedule->machineID = ptrCurrentEvent->scheduleInfo.machineID;
				ptrNewSchedule->source = ptrCurrentEvent->scheduleInfo.source;
				ptrNewSchedule->nextSchedule = NULL;

				schedule *ptrAux;
				ptrAux = ptrScheduleList;
				while(ptrAux->nextSchedule != NULL) {
					ptrAux = ptrAux->nextSchedule;
				}
				ptrAux->nextSchedule = ptrNewSchedule;

			}

		} // end of if (ptrScheduleList)

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

							ptrAuxMachine->status = RUNNING;  // SOH PRA CONFIRMAR, POIS JAH ESTAVA RUNNING A UM SEGUNDO ATRAS (VIDE AllocationPlanning())
							ptrAuxTask->status = STARTED;
							ptrAuxTask->numberOfSubmissions++;

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
								ptrNewEvent->taskInfo.numberOfSubmissions = ptrAuxTask->numberOfSubmissions;
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
							printf("taskID %d jobID %d machineID %d source %d submissions %d\n", ptrAuxTask->taskID,
									ptrAuxTask->jobID, ptrAuxMachine->machineID, ptrAuxMachine->source, ptrAuxTask->numberOfSubmissions);

							break;
						}

						ptrAuxMachine = ptrAuxMachine->nextMachine;
					}

					break;
				}

				ptrAuxTask = ptrAuxTask->nextTask;
			}

		} else printf("ERROR (task scheduling): there is no machine or task list!!!\n");

	} else printf("ERROR (task scheduling): wrong eventID!!!\n");
}
