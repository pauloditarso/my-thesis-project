/*
 * allocationplanningopt.c
 *
 *  Created on: Jul 24, 2012
 *      Author: PauloDitarso
 *
 *	What is done: (???) IT SHOULD BE REVISED TO REPRESENT THE DIFFERENCES BTW THIS AND THE NON-OPT!!!
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

void AllocationPlanningOpt(event *ptrCurrentEvent, event *ptrEventList, machine *ptrMachineList, task *ptrOrderedTaskList, balanceAccountInfo *ptrBalanceAccountInfo) {

	if (ptrCurrentEvent->eventID == ALLOCATIONPLANNING) {

		if (ptrMachineList && ptrOrderedTaskList) {

			unsigned int balance, numberOfGridMachines;
			balance = GetBalance(ptrBalanceAccountInfo, ptrCurrentEvent->time);
			numberOfGridMachines = (int)(balance * gridQoSFactor)/TASK_AVG_TIME; // ceiling or trunk???

			unsigned int firstTargetFinnishTime, deadline, timeSteps;
			firstTargetFinnishTime = (ptrCurrentEvent->jobInfo.arrivalTime + 2 + ptrCurrentEvent->jobInfo.longestTask); // AT + 2min to start a job + LT
			deadline = (ptrCurrentEvent->jobInfo.arrivalTime + ptrCurrentEvent->jobInfo.deadline);
			timeSteps = 200; // steps, in minutes, for the optimizing process

			task *ptrAuxOrderedTask;
			machine *ptrAuxMachine;
			scheduleQueue **ptrPtrScheduleQueue;
//			schedule *ptrBestScheduleList;
//			float bestProfit = -(ptrCurrentEvent->jobInfo.maxUtility);
			unsigned int scheduleID;
			unsigned short int allocated = 0;


			// ***************************************
			// PREENCHER ptrNewScheduledQueue->scheduleList COM OS SCHEDULES PARA CADA TEMPO
			// ***************************************


			//setbuf(stdout, NULL); // debugguing mode
			// sweeping the time till deadline
			unsigned int targetFinnishTime;
			for (targetFinnishTime = firstTargetFinnishTime; targetFinnishTime <= deadline; targetFinnishTime += timeSteps) {

				printf("\n");
				printf("targetFT %d firstTargetFT %d deadline %d\n", targetFinnishTime, firstTargetFinnishTime, deadline);
				printf("***************************************************\n");

				// initializing a ScheduleQueue
				scheduleQueue *ptrNewScheduleQueue;

				if (targetFinnishTime == firstTargetFinnishTime) {
					if ( (ptrNewScheduleQueue = malloc(sizeof(scheduleQueue))) ) {
						ptrNewScheduleQueue->targetFinnishtime = targetFinnishTime;
						if ( !(ptrNewScheduleQueue->scheduleList = malloc(sizeof(schedule))) ) printf("ERROR (allocation planningOpt): merdou o malloc!!!\n");
						ptrNewScheduleQueue->status = UNFINNISHED;
						ptrNewScheduleQueue->profit = 0.0;
						ptrNewScheduleQueue->previousSchedule = NULL;
					}
					else printf("ERROR (allocation planningOpt): merdou o malloc!!!\n");
				}
				else {
					if ( (ptrNewScheduleQueue = malloc(sizeof(scheduleQueue))) ) {
						ptrNewScheduleQueue->targetFinnishtime = targetFinnishTime;
						if ( !(ptrNewScheduleQueue->scheduleList = malloc(sizeof(schedule))) ) printf("ERROR (allocation planningOpt): merdou o malloc!!!\n");
						ptrNewScheduleQueue->status = UNFINNISHED;
						ptrNewScheduleQueue->profit = 0.0;
						ptrNewScheduleQueue->previousSchedule = (*ptrPtrScheduleQueue);
					}
					else printf("ERROR (allocation planningOpt): merdou o malloc!!!\n");
				}

				ptrAuxMachine = ptrMachineList;
				ptrAuxOrderedTask = ptrOrderedTaskList;
				ptrPtrScheduleQueue = &ptrNewScheduleQueue;
				scheduleID = 0;

				machineOptSet *ptrMachineOptSetList;
				if ( (ptrMachineOptSetList = malloc(sizeof(machineOptSet))) ) {
					ptrMachineOptSetList->machineID = 0;
					ptrMachineOptSetList->source = 0;
					ptrMachineOptSetList->timeLeft = 0;
					ptrMachineOptSetList->nextMachineOptSet = NULL;
				}
				else printf("ERROR (allocation planningOpt): merdou o malloc!!!\n");

				// filling ptrMachineOptSetList with local machines
				while(ptrAuxMachine->source == 0) {

					if (ptrMachineOptSetList->machineID == 0) {  // means there are no machines in the list yet

						ptrMachineOptSetList->machineID = ptrAuxMachine->machineID;
						ptrMachineOptSetList->source = ptrAuxMachine->source;
						ptrMachineOptSetList->timeLeft = (targetFinnishTime - (ptrCurrentEvent->time + 1));
						ptrMachineOptSetList->nextMachineOptSet = NULL;

					}
					else {

						machineOptSet *ptrAux1;
						if ( (ptrAux1 = malloc(sizeof(machineOptSet))) ) {

							ptrAux1->machineID = ptrAuxMachine->machineID;
							ptrAux1->source = ptrAuxMachine->source;
							ptrAux1->timeLeft = (targetFinnishTime - (ptrCurrentEvent->time + 1));
							ptrAux1->nextMachineOptSet = NULL;

							machineOptSet *ptrAux2;
							ptrAux2 = ptrMachineOptSetList;
							while(ptrAux2->nextMachineOptSet != NULL) ptrAux2 = ptrAux2->nextMachineOptSet;

							ptrAux2->nextMachineOptSet = ptrAux1;

						}
						else printf("ERROR (allocation planningOpt): merdou o malloc!!!\n");

					}

					ptrAuxMachine = ptrAuxMachine->nextMachine;
				} // end of while(ptrAuxMachine->source == 0)

				// filling ptrMachineOptSetList with grid machines
				unsigned short int i;
				for (i = 1; i <= numberOfGridMachines; i++) {

					unsigned int avgUpTime = (int)Randn(GRID_AVG_TIME, GRID_SDV_TIME);

					if (ptrMachineOptSetList->machineID == 0) {  // means there are no machines in the list yet

						ptrMachineOptSetList->machineID = 1;
						ptrMachineOptSetList->source = 1;
						ptrMachineOptSetList->timeLeft = avgUpTime;
						ptrMachineOptSetList->nextMachineOptSet = NULL;

					}
					else {

						machineOptSet *ptrAux1;
						if ( (ptrAux1 = malloc(sizeof(machineOptSet))) ) {

							ptrAux1->machineID = 1;
							ptrAux1->source = 1;
							ptrAux1->timeLeft = avgUpTime;
							ptrAux1->nextMachineOptSet = NULL;

							machineOptSet *ptrAux2;
							ptrAux2 = ptrMachineOptSetList;
							while(ptrAux2->nextMachineOptSet != NULL) ptrAux2 = ptrAux2->nextMachineOptSet;

							ptrAux2->nextMachineOptSet = ptrAux1;

						}
						else printf("ERROR (allocation planningOpt): merdou o malloc!!!\n");

					}

				}

				// filling ptrMachineOptSetList with cloud machines
				ptrAuxMachine = ptrMachineList;
				while(ptrAuxMachine) {

					if ( ptrAuxMachine->source == 2 || ptrAuxMachine->source == 3 || ptrAuxMachine->source == 4 ) {

						if (ptrMachineOptSetList->machineID == 0) {  // means there are no machines in the list yet

							ptrMachineOptSetList->machineID = ptrAuxMachine->machineID;
							ptrMachineOptSetList->source = ptrAuxMachine->source;
							ptrMachineOptSetList->timeLeft = (targetFinnishTime - (ptrCurrentEvent->time + 1));
							ptrMachineOptSetList->nextMachineOptSet = NULL;

						}
						else {

							machineOptSet *ptrAux1;
							if ( (ptrAux1 = malloc(sizeof(machineOptSet))) ) {

								ptrAux1->machineID = ptrAuxMachine->machineID;
								ptrAux1->source = ptrAuxMachine->source;
								ptrAux1->timeLeft = (targetFinnishTime - (ptrCurrentEvent->time + 1));
								ptrAux1->nextMachineOptSet = NULL;

								machineOptSet *ptrAux2;
								ptrAux2 = ptrMachineOptSetList;
								while(ptrAux2->nextMachineOptSet != NULL) ptrAux2 = ptrAux2->nextMachineOptSet;

								ptrAux2->nextMachineOptSet = ptrAux1;

							}
							else printf("ERROR (allocation planningOpt): merdou o malloc!!!\n");

						}

					}

					ptrAuxMachine = ptrAuxMachine->nextMachine;
				} // end of while(ptrAuxMachine)


				while(ptrAuxOrderedTask) {

					if (ptrAuxOrderedTask->jobID == ptrCurrentEvent->jobInfo.jobID) {

						machineOptSet *ptrAuxOptSetMachine;
						ptrAuxOptSetMachine = ptrMachineOptSetList;

						while(ptrAuxOptSetMachine) {

							if (ptrAuxOptSetMachine->source != 1) {

								int timeLeft = (ptrAuxOptSetMachine->timeLeft - ptrAuxOrderedTask->runtime);
								if (timeLeft >= 0) {

									unsigned int scheduleTime = (targetFinnishTime - timeLeft - ptrAuxOrderedTask->runtime);
									ptrAuxOptSetMachine->timeLeft = timeLeft;
									scheduleID++;

									if (scheduleID == 1) {
										(*ptrPtrScheduleQueue)->scheduleList->scheduleID = scheduleID;
										(*ptrPtrScheduleQueue)->scheduleList->time = scheduleTime;
										(*ptrPtrScheduleQueue)->scheduleList->taskID = ptrAuxOrderedTask->taskID;
										(*ptrPtrScheduleQueue)->scheduleList->jobID = ptrAuxOrderedTask->jobID;
										(*ptrPtrScheduleQueue)->scheduleList->machineID = ptrAuxOptSetMachine->machineID;
										(*ptrPtrScheduleQueue)->scheduleList->source = ptrAuxOptSetMachine->source;
									}
									else {

										schedule *ptrNewSchedule;
										if ( (ptrNewSchedule = malloc(sizeof(schedule))) ) {

											ptrNewSchedule->scheduleID = scheduleID;
											ptrNewSchedule->time = scheduleTime;
											ptrNewSchedule->taskID = ptrAuxOrderedTask->taskID;
											ptrNewSchedule->jobID = ptrAuxOrderedTask->jobID;
											ptrNewSchedule->machineID = ptrAuxOptSetMachine->machineID;
											ptrNewSchedule->source = ptrAuxOptSetMachine->source;
											ptrNewSchedule->nextSchedule = NULL;

										} else printf("ERROR (allocation planningOpt): merdou o malloc!!!\n");

										schedule *ptrAux;
										ptrAux = (*ptrPtrScheduleQueue)->scheduleList;
										while(ptrAux->nextSchedule != NULL)	ptrAux = ptrAux->nextSchedule;
										ptrAux->nextSchedule = ptrNewSchedule;

									}

									printf("scheduleID %d schedTime %d taskID %d jobID %d RT %d machineID %d source %d timeLeft %d\n",
											scheduleID, scheduleTime, ptrAuxOrderedTask->taskID, ptrAuxOrderedTask->jobID, ptrAuxOrderedTask->runtime,
											ptrAuxOptSetMachine->machineID, ptrAuxOptSetMachine->source, ptrAuxOptSetMachine->timeLeft);
									break;

								}

							} // end of if (ptrAuxOptSetMachine->source != 1)
							else {

								if (ptrAuxOrderedTask->runtime <= ptrAuxOptSetMachine->timeLeft) {

									scheduleID++;

									unsigned int scheduleTime = (ptrCurrentEvent->time + 1);
									ptrAuxOptSetMachine->timeLeft = 0;
									printf("scheduleID %d schedTime %d taskID %d jobID %d RT %d machineID %d source %d timeLeft %d\n",
											scheduleID, scheduleTime, ptrAuxOrderedTask->taskID, ptrAuxOrderedTask->jobID, ptrAuxOrderedTask->runtime,
											ptrAuxOptSetMachine->machineID, ptrAuxOptSetMachine->source, ptrAuxOptSetMachine->timeLeft);
									break;

								} // end of if (ptrAuxOrderedTask->runtime <= ptrAuxOptSetMachine->timeLeft)

							} // end of else

							ptrAuxOptSetMachine = ptrAuxOptSetMachine->nextMachineOptSet;

						} // end of while(ptrAuxOptSet)

					}

					ptrAuxOrderedTask = ptrAuxOrderedTask->nextTask;
				} // end of while(ptrAuxOrderedTask)


//
//
//				// VERIFICAR SE CONSEGUIU ESCALONAR TODAS AS TAREFAS E ALTERAR ptrNewScheduledQueue->status
//				// CALCULAR O RENDIMENTO BASEADO NO targetFinnishTime ATUAL, CASO ptrNewScheduledQueue->status == SUCCESSFUL
//				// (CASO CONTRARIO, RENDIMENTO == 0)
//				// CALCULAR OS CUSTOS A PARTIR DA UTILIZACAO DAS MAQUINAS DA CLOUD
//				// CALCULAR O PROFIT COMO RENDIMENTO - CUSTO TOTAL
//				// COMPARA O PROFIT DO targetFinnishTime ATUAL COM O bestProfit
//				// CASO O PROFIT ATUAL SEJA MAIOR,  ATUALIZAR bestProfit E APONTAR ptrTargetScheduleList PARA ptrNewScheduledQueue->scheduleList
//


				// LEMBRAR DE LIBERAR O ESPACO EM MEMORIA!!!
				printf("\n");
				machineOptSet *ptrActual;
				ptrActual = ptrMachineOptSetList;
				while(ptrActual) {
//					printf("machineID %d source %d timeleft %d\n", ptrActual->machineID, ptrActual->source, ptrActual->timeLeft);
					machineOptSet *ptrLast;
					ptrLast = ptrActual;
					ptrActual = ptrActual->nextMachineOptSet;
					free(ptrLast);
					ptrLast = NULL;
				}
				printf("\n");
				ptrMachineOptSetList = NULL;

			} // end of for (i = firstTargetFinnishTime; i <= deadline; i += timeSteps)

			// LEMBRAR DE LIBERAR O ESPACO EM MEMORIA DO ptrPtrScheduleQueue
//			scheduleQueue *ptrAuxScheduleQueue;
//			ptrAuxScheduleQueue = (*ptrPtrScheduleQueue);
//			while(ptrAuxScheduleQueue) {
//
////				schedule *ptrAuxSchedule;
////				ptrAuxSchedule = ptrAuxScheduleQueue->scheduleList;
////				while(ptrAuxSchedule) {
////
////					printf("scheduleID %d schedTime %d taskID %d jobID %d machineID %d source %d\n",
////							ptrAuxSchedule->scheduleID, ptrAuxSchedule->time, ptrAuxSchedule->taskID, ptrAuxSchedule->jobID,
////							ptrAuxSchedule->machineID, ptrAuxSchedule->source);
////
////					ptrAuxSchedule = ptrAuxSchedule->nextSchedule;
////				}
//
//				printf("targetFT %d status %d profit %f\n", ptrAuxScheduleQueue->targetFinnishtime, ptrAuxScheduleQueue->status, ptrAuxScheduleQueue->profit);
//
//				ptrAuxScheduleQueue = ptrAuxScheduleQueue->previousSchedule;
//			}




//			A PARTIR DAQUI EH O ALLOCATIONPLANNING() ORIGINAL!!!
//			while(ptrAuxTask) {
//
////				printf("taskID %d\n", ptrAuxTask->taskID);
//				unsigned short int found = 0;
//
//				if ( ptrAuxTask->taskID > 0 && ptrAuxTask->arrivalTime <= ptrCurrentEvent->time &&
//						ptrAuxTask->status == QUEUED ) {  // taskID 0 means code for an empty task list
//
////					passou();
//					// treating the allocation on in-house machines
//					machine *ptrAuxLocalMachine;
//					ptrAuxLocalMachine = ptrMachineList;
//
//					while(ptrAuxLocalMachine) {
//
//						if ( ptrAuxLocalMachine->source == LOCAL && (ptrAuxLocalMachine->status == IDLE || ptrAuxLocalMachine->status == DONATING) ) {
//
//							found = 1;
//							allocated = 1;
//							ptrAuxLocalMachine->status = RUNNING; // LEMBAR QUE ESTOU AQUI A UM SEGUNDO DE COMECAR A EXECUCAO EFETIVAMENTE
//							ptrAuxTask->status = STARTED; // LEMBAR QUE ESTOU AQUI A UM SEGUNDO DE COMECAR A EXECUCAO EFETIVAMENTE
//
//							// insert a new task schedule into the event list
//							event *ptrNewEvent;
//							if( (ptrNewEvent = malloc(sizeof(event))) ) {
//								ptrNewEvent->eventNumber = 0;
//								ptrNewEvent->eventID = TASKSCHEDULE;
//								ptrNewEvent->time = (ptrCurrentEvent->time + 1);
//								ptrNewEvent->scheduleInfo.taskID = ptrAuxTask->taskID;
//								ptrNewEvent->scheduleInfo.jobID = ptrAuxTask->jobID;
//								ptrNewEvent->scheduleInfo.machineID = ptrAuxLocalMachine->machineID;
//								ptrNewEvent->scheduleInfo.source = ptrAuxLocalMachine->source;
//								ptrNewEvent->nextEvent = NULL;
//
//								InsertEvent(ptrEventList, ptrNewEvent);
//							}
//							else printf("ERROR (allocation planning): merdou o malloc!!!\n");
//
////							printf("eventID %d (Allocation Planning) time %d ", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
////							printf("taskID %d jobID %d machineID %d source %d\n", ptrAuxTask->taskID,
////									ptrAuxTask->jobID, ptrAuxLocalMachine->machineID, ptrAuxLocalMachine->source);
//
//							break; // finishing while(ptrAuxLocalMachine)
//						}
//
//						ptrAuxLocalMachine = ptrAuxLocalMachine->nextMachine;
//
//					} // end of while(ptrAuxLocalMachine)
//
//					// treating the allocation on grid machines
//					if (found == 0 && numberOfGridMachines > 0) {
//
//						numberOfGridMachines -= 1;
//						gridMachinesID += 1;
//						found = 1;
//						allocated = 1;
//						ptrAuxTask->status = STARTED; // LEMBAR QUE ESTOU AQUI A UM SEGUNDO DE COMECAR A EXECUCAO EFETIVAMENTE
//						unsigned int avgUpTime = (int)Randn(GRID_AVG_TIME, GRID_SDV_TIME);
////						printf("DT %d\n", departureTime); // debug mode
//
//						// insert a machine arrival event into the event list
//						event *ptrNewGridMachine;
//						if( (ptrNewGridMachine = malloc(sizeof(event))) ) {
//							ptrNewGridMachine->eventNumber = 0;
//							ptrNewGridMachine->eventID = MACHARRIVAL;
//							ptrNewGridMachine->time = ptrCurrentEvent->time;
//							ptrNewGridMachine->machineInfo.machineID = gridMachinesID;
//							ptrNewGridMachine->machineInfo.source = GRID;
//							ptrNewGridMachine->machineInfo.status = RUNNING;
//							ptrNewGridMachine->machineInfo.arrivalTime = ptrCurrentEvent->time;
//							ptrNewGridMachine->machineInfo.departureTime = (ptrCurrentEvent->time + avgUpTime);
//							ptrNewGridMachine->machineInfo.usagePrice = 0.0;
//							ptrNewGridMachine->machineInfo.reservationPrice = 0.0;
//							ptrNewGridMachine->machineInfo.nextMachine = NULL;
//							ptrNewGridMachine->nextEvent = NULL;
//
//							InsertEvent(ptrEventList, ptrNewGridMachine);
//						}
//						else {
//							printf("ERROR (allocation planning): merdou o malloc!!!\n");
//						}
//
//						event *ptrOutGridMachine;
//						if( (ptrOutGridMachine = malloc(sizeof(event))) ) {
//							ptrOutGridMachine->eventNumber = 0;
//							ptrOutGridMachine->eventID = MACHDEPARTURE;
//							ptrOutGridMachine->time = (ptrCurrentEvent->time + avgUpTime);
//							ptrOutGridMachine->machineInfo.machineID = gridMachinesID;
//							ptrOutGridMachine->machineInfo.source = GRID;
//							ptrOutGridMachine->machineInfo.status = IDLE;
//							ptrOutGridMachine->machineInfo.arrivalTime = ptrCurrentEvent->time;
//							ptrOutGridMachine->machineInfo.departureTime = (ptrCurrentEvent->time + avgUpTime);
//							ptrOutGridMachine->machineInfo.usagePrice = 0.0;
//							ptrOutGridMachine->machineInfo.reservationPrice = 0.0;
//							ptrOutGridMachine->machineInfo.nextMachine = NULL;
//							ptrOutGridMachine->nextEvent = NULL;
//
//							InsertEvent(ptrEventList, ptrOutGridMachine);
//						}
//						else {
//							printf("ERROR (allocation planning): merdou o malloc!!!\n");
//						}
//
//						// insert a new task schedule into the event list
//						event *ptrNewEvent;
//						if( (ptrNewEvent = malloc(sizeof(event))) ) {
//							ptrNewEvent->eventNumber = 0;
//							ptrNewEvent->eventID = TASKSCHEDULE;
//							ptrNewEvent->time = (ptrCurrentEvent->time + 1);
//							ptrNewEvent->scheduleInfo.taskID = ptrAuxTask->taskID;
//							ptrNewEvent->scheduleInfo.jobID = ptrAuxTask->jobID;
//							ptrNewEvent->scheduleInfo.machineID = ptrNewGridMachine->machineInfo.machineID;
//							ptrNewEvent->scheduleInfo.source = ptrNewGridMachine->machineInfo.source;
//							ptrNewEvent->nextEvent = NULL;
//
//							InsertEvent(ptrEventList, ptrNewEvent);
//						}
//						else printf("ERROR (allocation planning): merdou o malloc!!!\n");
//
//
////						printf("eventID %d (Allocation Planning) time %d ", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
////						printf("taskID %d jobID %d machineID %d source %d\n", ptrAuxTask->taskID,
////								ptrAuxTask->jobID, ptrNewGridMachine->machineInfo.machineID, ptrNewGridMachine->machineInfo.source);
//
//					}
//
//					// treating the allocation on cloud machines
//					if (found == 0) {
//
//						machine *ptrAuxCloudMachine;
//						ptrAuxCloudMachine = ptrMachineList;
//
//						while(ptrAuxCloudMachine) {
//
//							// TALVEZ TENHA QUE ADICIONAR UM NOVO IF PARA AS MAQUINAS ON-DEMAND
//							if ( (ptrAuxCloudMachine->source == RESERVED || ptrAuxCloudMachine->source == ONDEMAND)	&& ptrAuxCloudMachine->status == IDLE) {
//
//								found = 1;
//								allocated = 1;
//								ptrAuxCloudMachine->status = RUNNING; // LEMBAR QUE ESTOU AQUI A UM SEGUNDO DE COMECAR A EXECUCAO EFETIVAMENTE
//								ptrAuxTask->status = STARTED; // LEMBAR QUE ESTOU AQUI A UM SEGUNDO DE COMECAR A EXECUCAO EFETIVAMENTE
//
//								// insert a new task schedule into the event list
//								event *ptrNewEvent;
//								if( (ptrNewEvent = malloc(sizeof(event))) ) {
//									ptrNewEvent->eventNumber = 0;
//									ptrNewEvent->eventID = TASKSCHEDULE;
//									ptrNewEvent->time = (ptrCurrentEvent->time + 1);
//									ptrNewEvent->scheduleInfo.taskID = ptrAuxTask->taskID;
//									ptrNewEvent->scheduleInfo.jobID = ptrAuxTask->jobID;
//									ptrNewEvent->scheduleInfo.machineID = ptrAuxCloudMachine->machineID;
//									ptrNewEvent->scheduleInfo.source = ptrAuxCloudMachine->source;
//									ptrNewEvent->nextEvent = NULL;
//
//									InsertEvent(ptrEventList, ptrNewEvent);
//								}
//								else printf("ERROR (allocation planning): merdou o malloc!!!\n");
//
//								//							printf("eventID %d (Allocation Planning) time %d ", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
//								//							printf("taskID %d jobID %d machineID %d source %d\n", ptrAuxTask->taskID,
//								//									ptrAuxTask->jobID, ptrAuxCloudMachine->machineID, ptrAuxCloudMachine->source);
//
//								break;
//							}
//
//							ptrAuxCloudMachine = ptrAuxCloudMachine->nextMachine;
//
//						} // end of while(ptrAuxCloudMachine)
//
//					} // end if (found == 0)
//
//				} // end if (looking for a queued task)
//
//				ptrAuxTask = ptrAuxTask->nextTask;
//
//			} // end of while(ptrAuxTask)
//
//			// if there is any unallocated in-house machine, creates a new donation event
//			machine *ptrAuxMachine;
//			ptrAuxMachine = ptrMachineList;
//			while(ptrAuxMachine) {
//
//				if ( ptrAuxMachine->source == LOCAL && ptrAuxMachine->status == IDLE ) {
//
//					event *ptrNewDonation;
//
//					if( (ptrNewDonation = malloc(sizeof(event))) ) {
//						ptrNewDonation->eventNumber = 0;
//						ptrNewDonation->eventID = GRIDDONATING;
//						ptrNewDonation->time = (ptrCurrentEvent->time); // one second after this event
//						ptrNewDonation->machineInfo.machineID = ptrAuxMachine->machineID;
//						ptrNewDonation->machineInfo.source = ptrAuxMachine->source;
//						ptrNewDonation->machineInfo.status = DONATING;
//						ptrNewDonation->machineInfo.arrivalTime = ptrAuxMachine->arrivalTime;
//						ptrNewDonation->machineInfo.departureTime = ptrAuxMachine->departureTime;
//						ptrNewDonation->machineInfo.usagePrice = ptrAuxMachine->usagePrice;
//						ptrNewDonation->machineInfo.reservationPrice = ptrAuxMachine->reservationPrice;
//						ptrNewDonation->machineInfo.nextMachine = ptrAuxMachine->nextMachine;
//						ptrNewDonation->nextEvent = NULL;
//
//						InsertEvent(ptrEventList, ptrNewDonation);
//
//					} else printf("ERROR (task finnished): merdou o malloc!!!\n");
//
//				}
//
//				ptrAuxMachine = ptrAuxMachine->nextMachine;
//			}

			printf("eventID %d (Allocation Planning) time %d ", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
			printf("Allocation %d\n", allocated);

		} else printf("ERROR (arrival): there is no machine or task list!!!\n");

	} else printf("ERROR (allocation planning): wrong eventID!!!\n");
} // end of AllocationPlanning()
