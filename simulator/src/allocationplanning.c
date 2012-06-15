/*
 * allocationplanning.c
 *
 *  Created on: Jun 14, 2012
 *      Author: PauloDitarso
 *
 *	What is done:
 *  	1. it tests if it is the correct event;
 *  	2. it tests if there are machine and task lists;
 *  	3. it checks the number of available machines from the grid:
 *  		3.1 it checks the balance
 *  		3.2 it furmulates the number of machines
 *  	4. it looks for each task tagged as queued in the task list;
 *  	   (for the Optimized version, the first task will be the longest one)
 *  	5. it looks for an idle or a donating in-house machine; if positive:
 *  		5.1 if the machine is donating, it creates a grid preemption event;
 *  	(???)	5.2 it changes the machine status to RUNNING and the task status to STARTED;
 *  	(???)	5.3 it creates a task finish event;
 *  	(???)	5.4 it creates a job start event if the task ID is 1 (first task);
 *  		5.5 it creates a taskschedule event, one second ahead;
 *  	    5.6 it jumps to the 8th step;
 *  	6. it looks for available grid machiens; if positive:
 *  		6.1 creates a grid machines arrival event, one second ahead and just before the task schedule event;
 *  		6.2 it creates a taskschedule event, one second ahead;
 *  		6.3 it jumps to the 8th step;
 *  	7. it looks for an idle cloud machine; if positive:
 *  		7.1 it creates a taskschedule event, one second ahead;
 *  	    7.2 it jumps to the 8th step;
 *  	8. ???
 *
 *  	. it plans the schedule map:
 *  		.1. it creates an ordered task list (from the longest to the shortest);
 *  		.2. it creates an ordered (by hours) machine list (in-house, cloud and grid machines);
 *  		.3.
 */

#include "simulation.h"

void AllocationPlanning(event *ptrCurrentEvent, event *ptrEventList, machine *ptrMachineList, task *ptrTaskList, balanceAccountInfo *ptrBalanceAccountInfo) {

	if (ptrCurrentEvent->eventID == ALLOCATIONPLANNING) {

		if (ptrMachineList && ptrTaskList) {

			//	printf("o balance na grade agora e %d\n", GetBalance(ptrBalanceAccountInfo, ptrCurrentEvent->time));
			unsigned int balance, numberOfGridMachines;
			balance = GetBalance(ptrBalanceAccountInfo, ptrCurrentEvent->time);
			numberOfGridMachines = (balance * gridQoSFactor)/TASK_AVG_TIME; // ceiling or trunk???

			task *ptrAuxTask;
			ptrAuxTask = ptrTaskList;
			unsigned short int found = 0;

			while(ptrAuxTask) {

				if ( ptrAuxTask->taskID > 0 && ptrAuxTask->arrivalTime <= ptrCurrentEvent->time &&
						ptrAuxTask->status == QUEUED ) {  // taskID 0 means code for an empty task list

					// treating the allocation on in-house machines
					machine *ptrAuxLocalMachine;
					ptrAuxLocalMachine = ptrMachineList;

					while(ptrAuxLocalMachine) {

						if ( ptrAuxLocalMachine->source == LOCAL && (ptrAuxLocalMachine->status == IDLE || ptrAuxLocalMachine->status == DONATING) ) {

							found = 1;

							// insert a new task schedule into the event list
							event *ptrNewEvent;

							if( (ptrNewEvent = malloc(sizeof(event))) ) {
								ptrNewEvent->eventNumber = 0;
								ptrNewEvent->eventID = TASKSCHEDULE;
								ptrNewEvent->time = (ptrCurrentEvent->time + 1);
								ptrNewEvent->scheduleInfo.taskID = ptrAuxTask->taskID;
								ptrNewEvent->scheduleInfo.jobID = ptrAuxTask->jobID;
								ptrNewEvent->scheduleInfo.machineID = ptrAuxLocalMachine->machineID;
								ptrNewEvent->scheduleInfo.source = ptrAuxLocalMachine->source;
								ptrNewEvent->nextEvent = NULL;

								InsertEvent(ptrEventList, ptrNewEvent);
							}
							else printf("ERROR (allocation planning): merdou o malloc!!!\n");

							printf("eventID %d (Allocation Planning) time %d ", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
							printf("taskID %d jobID %d machineID %d source %d\n", ptrAuxTask->taskID,
									ptrAuxTask->jobID, ptrAuxLocalMachine->machineID, ptrAuxLocalMachine->source);

							break;
						}

					} // end of while(ptrAuxLocalMachine)

					// treating the allocation on grid machines
					if (numberOfGridMachines > 0) {

						numberOfGridMachines -= 1;
						found = 1;

						// CRIAR UMA MAQUINA DA GRADE E ALOCAR A TASK NESTA MAQUINA (TASKSCHEDULE)
						event *ptrAuxGridMachine;

						if( (ptrAuxGridMachine = malloc(sizeof(event))) ) {
							ptrAuxGridMachine->eventNumber = 0;
							ptrAuxGridMachine->eventID = MACHARRIVAL;
							ptrAuxGridMachine->time = ptrCurrentEvent->time;
							ptrAuxGridMachine->machineInfo.machineID = 0; // PRECISO ME PREOCUPAR COM ISSO!!! QUAL ID PARA AS MAQUINAS DO GRID??? TALVEZ AJEITAR NO MachineArrival()
							ptrAuxGridMachine->machineInfo.source = GRID;
							ptrAuxGridMachine->machineInfo.status = IDLE;
							ptrAuxGridMachine->machineInfo.arrivalTime = ptrCurrentEvent->time;
							ptrAuxGridMachine->machineInfo.departureTime = (ptrCurrentEvent->time + TASK_AVG_TIME); // DETERMINISTICO, MEIA HORA DEPOIS DE CHEGAR!!! TRANSFORMAR EM ALEATORIO!!!
							ptrAuxGridMachine->machineInfo.usagePrice = 0.0;
							ptrAuxGridMachine->machineInfo.reservationPrice = 0.0;
							ptrAuxGridMachine->machineInfo.nextMachine = NULL;
							ptrAuxGridMachine->nextEvent = NULL;

							InsertEvent(ptrEventList, ptrAuxGridMachine);
						}
						else {
							printf("ERROR (allocation planning): merdou o malloc!!!\n");
						}

						printf("eventID %d (Allocation Planning) time %d ", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
						printf("taskID %d jobID %d machineID %d source %d\n", ptrAuxTask->taskID,
								ptrAuxTask->jobID, ptrAuxGridMachine->machineInfo.machineID, ptrAuxGridMachine->machineInfo.source);

						break;
					}

					// treating the allocation on cloud machines
					machine *ptrAuxCloudMachine;
					ptrAuxCloudMachine = ptrMachineList;

					while(ptrAuxCloudMachine) {

						if (ptrAuxCloudMachine->source == CLOUD && ptrAuxCloudMachine->status == IDLE) {

							found = 1;

							// insert a new task schedule into the event list
							event *ptrNewEvent;

							if( (ptrNewEvent = malloc(sizeof(event))) ) {
								ptrNewEvent->eventNumber = 0;
								ptrNewEvent->eventID = TASKSCHEDULE;
								ptrNewEvent->time = (ptrCurrentEvent->time + 1);
								ptrNewEvent->scheduleInfo.taskID = ptrAuxTask->taskID;
								ptrNewEvent->scheduleInfo.jobID = ptrAuxTask->jobID;
								ptrNewEvent->scheduleInfo.machineID = ptrAuxCloudMachine->machineID;
								ptrNewEvent->scheduleInfo.source = ptrAuxCloudMachine->source;
								ptrNewEvent->nextEvent = NULL;

								InsertEvent(ptrEventList, ptrNewEvent);
							}
							else printf("ERROR (allocation planning): merdou o malloc!!!\n");

							printf("eventID %d (Allocation Planning) time %d ", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
							printf("taskID %d jobID %d machineID %d source %d\n", ptrAuxTask->taskID,
									ptrAuxTask->jobID, ptrAuxCloudMachine->machineID, ptrAuxCloudMachine->source);

							break;
						}

						ptrAuxCloudMachine = ptrAuxCloudMachine->nextMachine;

					} // end of while(ptrAuxCloudMachine)

				} // end if (looking for a queued task)

				ptrAuxTask = ptrAuxTask->nextTask;

			} // end of while(ptrAuxTask)

			if (found == 0) {
				printf("eventID %d (Allocation Planning) time %d (NOTHING_TO_DO!!!)\n", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
			}


		} else printf("ERROR (arrival): there is no machine or task list!!!\n");

	} else printf("ERROR (allocation planning): wrong eventID!!!\n");
} // end of AllocationPlanning()
