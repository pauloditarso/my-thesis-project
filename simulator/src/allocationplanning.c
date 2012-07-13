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
 *  		5.1 it creates a task schedule event, one second ahead;
 *  	    5.2 it jumps to the 8th step;
 *  	6. it looks for available grid machiens; if positive:
 *  		6.1 creates a grid machines arrival event at this very same time;
 *  		6.2 it creates a taskschedule event, one second ahead;
 *  		6.3 it jumps to the 8th step;
 *  	7. it looks for an idle cloud machine; if positive:
 *  		7.1 it creates a taskschedule event, one second ahead;
 *  	    7.2 it jumps to the 8th step;
 *  	8. if no schedule was created, it informs NOTHING_TO_DO!!!
 */

#include "simulation.h"

void AllocationPlanning(event *ptrCurrentEvent, event *ptrEventList, machine *ptrMachineList, task *ptrTaskList, balanceAccountInfo *ptrBalanceAccountInfo) {

	if (ptrCurrentEvent->eventID == ALLOCATIONPLANNING) {

		if (ptrMachineList && ptrTaskList) {

			//	printf("o balance na grade agora e %d\n", GetBalance(ptrBalanceAccountInfo, ptrCurrentEvent->time));
			unsigned int balance, numberOfGridMachines;
			balance = GetBalance(ptrBalanceAccountInfo, ptrCurrentEvent->time);
//			printf("\n");
//			printf("balance %d\n", balance);
//			printf("gridFactor %f\n", gridQoSFactor);
			numberOfGridMachines = (int)(balance * gridQoSFactor)/TASK_AVG_TIME; // ceiling or trunk???
//			printf("numberofGridMachines %d\n", numberOfGridMachines);

			task *ptrAuxTask;
			ptrAuxTask = ptrTaskList;
			unsigned short int allocated = 0;

			while(ptrAuxTask) {

//				printf("taskID %d\n", ptrAuxTask->taskID);
				unsigned short int found = 0;

				if ( ptrAuxTask->taskID > 0 && ptrAuxTask->arrivalTime <= ptrCurrentEvent->time &&
						ptrAuxTask->status == QUEUED ) {  // taskID 0 means code for an empty task list

//					passou();
					// treating the allocation on in-house machines
					machine *ptrAuxLocalMachine;
					ptrAuxLocalMachine = ptrMachineList;

					while(ptrAuxLocalMachine) {

						if ( ptrAuxLocalMachine->source == LOCAL && (ptrAuxLocalMachine->status == IDLE || ptrAuxLocalMachine->status == DONATING) ) {

							found = 1;
							allocated = 1;
							ptrAuxLocalMachine->status = RUNNING; // LEMBAR QUE ESTOU AQUI A UM SEGUNDO DE COMECAR A EXECUCAO EFETIVAMENTE
							ptrAuxTask->status = STARTED; // LEMBAR QUE ESTOU AQUI A UM SEGUNDO DE COMECAR A EXECUCAO EFETIVAMENTE

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

//							printf("eventID %d (Allocation Planning) time %d ", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
//							printf("taskID %d jobID %d machineID %d source %d\n", ptrAuxTask->taskID,
//									ptrAuxTask->jobID, ptrAuxLocalMachine->machineID, ptrAuxLocalMachine->source);

							break; // finishing while(ptrAuxLocalMachine)
						}

						ptrAuxLocalMachine = ptrAuxLocalMachine->nextMachine;

					} // end of while(ptrAuxLocalMachine)

					// treating the allocation on grid machines
					if (found == 0 && numberOfGridMachines > 0) {

						numberOfGridMachines -= 1;
						gridMachinesID += 1;
						found = 1;
						allocated = 1;
						ptrAuxTask->status = STARTED; // LEMBAR QUE ESTOU AQUI A UM SEGUNDO DE COMECAR A EXECUCAO EFETIVAMENTE
						unsigned int avgUpTime = (int)Randn(GRID_AVG_TIME, GRID_SDV_TIME);
//						printf("DT %d\n", departureTime); // debug mode

						// insert a machine arrival event into the event list
						event *ptrNewGridMachine;
						if( (ptrNewGridMachine = malloc(sizeof(event))) ) {
							ptrNewGridMachine->eventNumber = 0;
							ptrNewGridMachine->eventID = MACHARRIVAL;
							ptrNewGridMachine->time = ptrCurrentEvent->time;
							ptrNewGridMachine->machineInfo.machineID = gridMachinesID;
							ptrNewGridMachine->machineInfo.source = GRID;
							ptrNewGridMachine->machineInfo.status = RUNNING;
							ptrNewGridMachine->machineInfo.arrivalTime = ptrCurrentEvent->time;
							ptrNewGridMachine->machineInfo.departureTime = (ptrCurrentEvent->time + avgUpTime);
							ptrNewGridMachine->machineInfo.usagePrice = 0.0;
							ptrNewGridMachine->machineInfo.reservationPrice = 0.0;
							ptrNewGridMachine->machineInfo.nextMachine = NULL;
							ptrNewGridMachine->nextEvent = NULL;

							InsertEvent(ptrEventList, ptrNewGridMachine);
						}
						else {
							printf("ERROR (allocation planning): merdou o malloc!!!\n");
						}

						event *ptrOutGridMachine;
						if( (ptrOutGridMachine = malloc(sizeof(event))) ) {
							ptrOutGridMachine->eventNumber = 0;
							ptrOutGridMachine->eventID = MACHDEPARTURE;
							ptrOutGridMachine->time = (ptrCurrentEvent->time + avgUpTime);
							ptrOutGridMachine->machineInfo.machineID = gridMachinesID;
							ptrOutGridMachine->machineInfo.source = GRID;
							ptrOutGridMachine->machineInfo.status = IDLE;
							ptrOutGridMachine->machineInfo.arrivalTime = ptrCurrentEvent->time;
							ptrOutGridMachine->machineInfo.departureTime = (ptrCurrentEvent->time + avgUpTime);
							ptrOutGridMachine->machineInfo.usagePrice = 0.0;
							ptrOutGridMachine->machineInfo.reservationPrice = 0.0;
							ptrOutGridMachine->machineInfo.nextMachine = NULL;
							ptrOutGridMachine->nextEvent = NULL;

							InsertEvent(ptrEventList, ptrOutGridMachine);
						}
						else {
							printf("ERROR (allocation planning): merdou o malloc!!!\n");
						}

						// insert a new task schedule into the event list
						event *ptrNewEvent;
						if( (ptrNewEvent = malloc(sizeof(event))) ) {
							ptrNewEvent->eventNumber = 0;
							ptrNewEvent->eventID = TASKSCHEDULE;
							ptrNewEvent->time = (ptrCurrentEvent->time + 1);
							ptrNewEvent->scheduleInfo.taskID = ptrAuxTask->taskID;
							ptrNewEvent->scheduleInfo.jobID = ptrAuxTask->jobID;
							ptrNewEvent->scheduleInfo.machineID = ptrNewGridMachine->machineInfo.machineID;
							ptrNewEvent->scheduleInfo.source = ptrNewGridMachine->machineInfo.source;
							ptrNewEvent->nextEvent = NULL;

							InsertEvent(ptrEventList, ptrNewEvent);
						}
						else printf("ERROR (allocation planning): merdou o malloc!!!\n");


//						printf("eventID %d (Allocation Planning) time %d ", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
//						printf("taskID %d jobID %d machineID %d source %d\n", ptrAuxTask->taskID,
//								ptrAuxTask->jobID, ptrNewGridMachine->machineInfo.machineID, ptrNewGridMachine->machineInfo.source);

					}

					// treating the allocation on cloud machines
					if (found == 0) {

						machine *ptrAuxCloudMachine;
						ptrAuxCloudMachine = ptrMachineList;

						while(ptrAuxCloudMachine) {

							// TALVEZ TENHA QUE ADICIONAR UM NOVO IF PARA AS MAQUINAS ON-DEMAND
							if ( (ptrAuxCloudMachine->source == RESERVED || ptrAuxCloudMachine->source == ONDEMAND)	&& ptrAuxCloudMachine->status == IDLE) {

								found = 1;
								allocated = 1;
								ptrAuxCloudMachine->status = RUNNING; // LEMBAR QUE ESTOU AQUI A UM SEGUNDO DE COMECAR A EXECUCAO EFETIVAMENTE
								ptrAuxTask->status = STARTED; // LEMBAR QUE ESTOU AQUI A UM SEGUNDO DE COMECAR A EXECUCAO EFETIVAMENTE

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

								//							printf("eventID %d (Allocation Planning) time %d ", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
								//							printf("taskID %d jobID %d machineID %d source %d\n", ptrAuxTask->taskID,
								//									ptrAuxTask->jobID, ptrAuxCloudMachine->machineID, ptrAuxCloudMachine->source);

								break;
							}

							ptrAuxCloudMachine = ptrAuxCloudMachine->nextMachine;

						} // end of while(ptrAuxCloudMachine)

					} // end if (found == 0)

				} // end if (looking for a queued task)

				ptrAuxTask = ptrAuxTask->nextTask;

			} // end of while(ptrAuxTask)

			// if there is any unallocated in-house machine, creates a new donation event
			machine *ptrAuxMachine;
			ptrAuxMachine = ptrMachineList;
			while(ptrAuxMachine) {

				if ( ptrAuxMachine->source == LOCAL && ptrAuxMachine->status == IDLE ) {

					event *ptrNewDonation;

					if( (ptrNewDonation = malloc(sizeof(event))) ) {
						ptrNewDonation->eventNumber = 0;
						ptrNewDonation->eventID = GRIDDONATING;
						ptrNewDonation->time = (ptrCurrentEvent->time); // one second after this event
						ptrNewDonation->machineInfo.machineID = ptrAuxMachine->machineID;
						ptrNewDonation->machineInfo.source = ptrAuxMachine->source;
						ptrNewDonation->machineInfo.status = DONATING;
						ptrNewDonation->machineInfo.arrivalTime = ptrAuxMachine->arrivalTime;
						ptrNewDonation->machineInfo.departureTime = ptrAuxMachine->departureTime;
						ptrNewDonation->machineInfo.usagePrice = ptrAuxMachine->usagePrice;
						ptrNewDonation->machineInfo.reservationPrice = ptrAuxMachine->reservationPrice;
						ptrNewDonation->machineInfo.nextMachine = ptrAuxMachine->nextMachine;
						ptrNewDonation->nextEvent = NULL;

						InsertEvent(ptrEventList, ptrNewDonation);

					} else printf("ERROR (task finnished): merdou o malloc!!!\n");

				}

				ptrAuxMachine = ptrAuxMachine->nextMachine;
			}

			printf("eventID %d (Allocation Planning) time %d ", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
			printf("Allocation %d\n", allocated);

		} else printf("ERROR (arrival): there is no machine or task list!!!\n");

	} else printf("ERROR (allocation planning): wrong eventID!!!\n");
} // end of AllocationPlanning()
