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

			machine *ptrAuxMachine;
			ptrAuxMachine = ptrMachineList;
			scheduleQueue *ptrScheduleQueue, **ptrPtrScheduleQueue;
			if ( !(ptrScheduleQueue = malloc(sizeof(scheduleQueue))) )  printf("ERROR (allocation planningOpt): merdou o malloc!!!\n");
			ptrPtrScheduleQueue = &ptrScheduleQueue;
//			schedule *ptrBestScheduleList;
//			float bestProfit = -(ptrCurrentEvent->jobInfo.maxUtility);
			unsigned int scheduleID;
			unsigned short int allocated = 0;

			// creating an optimized machine set list and filling it after
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
					ptrMachineOptSetList->timeLeft = 0;
					ptrMachineOptSetList->nextMachineOptSet = NULL;

				}
				else {

					machineOptSet *ptrAux1;
					if ( (ptrAux1 = malloc(sizeof(machineOptSet))) ) {

						ptrAux1->machineID = ptrAuxMachine->machineID;
						ptrAux1->source = ptrAuxMachine->source;
						ptrAux1->timeLeft = 0;
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

			// filling ptrMachineOptSetList with cloud machines (whithout spot machines!!!)
			ptrAuxMachine = ptrMachineList;
			while(ptrAuxMachine) {

				if ( ptrAuxMachine->source == 2 || ptrAuxMachine->source == 3 ) {

					if (ptrMachineOptSetList->machineID == 0) {  // means there are no machines in the list yet

						ptrMachineOptSetList->machineID = ptrAuxMachine->machineID;
						ptrMachineOptSetList->source = ptrAuxMachine->source;
						ptrMachineOptSetList->timeLeft = 0;
						ptrMachineOptSetList->nextMachineOptSet = NULL;

					}
					else {

						machineOptSet *ptrAux1;
						if ( (ptrAux1 = malloc(sizeof(machineOptSet))) ) {

							ptrAux1->machineID = ptrAuxMachine->machineID;
							ptrAux1->source = ptrAuxMachine->source;
							ptrAux1->timeLeft = 0;
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

			//setbuf(stdout, NULL); // debugguing mode
			// sweeping the time till deadline
			unsigned int targetFinnishTime;
			for (targetFinnishTime = firstTargetFinnishTime; targetFinnishTime <= deadline; targetFinnishTime += timeSteps) {

				printf("\n");
				printf("targetFT %d firstTargetFT %d deadline %d\n", targetFinnishTime, firstTargetFinnishTime, deadline);
				printf("***************************************************\n");

				unsigned short int count = 0;
				unsigned int targetUtility = 0;
				float targetCost = 0.0;

				task *ptrAuxOrderedTask;
				ptrAuxOrderedTask = ptrOrderedTaskList;
				scheduleID = 0;

				// setting the left time to non-grid machines
				machineOptSet *ptrAuxOptSet;
				ptrAuxOptSet = ptrMachineOptSetList;
				while(ptrAuxOptSet) {
					if (ptrAuxOptSet->source != 1) ptrAuxOptSet->timeLeft = (targetFinnishTime - (ptrCurrentEvent->time + 1));
					ptrAuxOptSet = ptrAuxOptSet->nextMachineOptSet;
				}

				// initializing a new node for the ScheduleQueue
				scheduleQueue *ptrNewScheduleQueue;
				if ( (ptrNewScheduleQueue = malloc(sizeof(scheduleQueue))) ) {
					ptrNewScheduleQueue->targetFinnishtime = targetFinnishTime;
					if ( !(ptrNewScheduleQueue->scheduleList = malloc(sizeof(schedule))) ) printf("ERROR (allocation planningOpt): merdou o malloc!!!\n");
					ptrNewScheduleQueue->status = UNFINNISHED;
					ptrNewScheduleQueue->profit = 0.0;
					ptrNewScheduleQueue->previousSchedule = NULL;
				}
				else printf("ERROR (allocation planningOpt): merdou o malloc!!!\n");

				if ( !(targetFinnishTime == firstTargetFinnishTime) ) ptrNewScheduleQueue->previousSchedule = (*ptrPtrScheduleQueue);
				(*ptrPtrScheduleQueue) = ptrNewScheduleQueue;
				ptrNewScheduleQueue = NULL;

				// sweeping ordered task list
				unsigned int roundNumberOfGridMachines = numberOfGridMachines;
				unsigned int roundGridMachinesID = gridMachinesID;
				while(ptrAuxOrderedTask) {

					if (ptrAuxOrderedTask->jobID == ptrCurrentEvent->jobInfo.jobID) {

						machineOptSet *ptrAuxOptSetMachine;
						ptrAuxOptSetMachine = ptrMachineOptSetList;

						// sweeping optimized machine list, with grid machines on it
						while(ptrAuxOptSetMachine) {

							if (ptrAuxOptSetMachine->source != 1) {

								int timeLeft = (ptrAuxOptSetMachine->timeLeft - ptrAuxOrderedTask->runtime);
								if (timeLeft >= 0) {

									scheduleID++;
									count++;
									allocated = 1;

									unsigned int scheduleTime = (targetFinnishTime - timeLeft - ptrAuxOrderedTask->runtime);
									ptrAuxOptSetMachine->timeLeft = timeLeft;

									if (scheduleID == 1) {
										(*ptrPtrScheduleQueue)->scheduleList->scheduleID = scheduleID;
										(*ptrPtrScheduleQueue)->scheduleList->scheduleTime = scheduleTime;
										(*ptrPtrScheduleQueue)->scheduleList->taskID = ptrAuxOrderedTask->taskID;
										(*ptrPtrScheduleQueue)->scheduleList->jobID = ptrAuxOrderedTask->jobID;
										(*ptrPtrScheduleQueue)->scheduleList->machineID = ptrAuxOptSetMachine->machineID;
										(*ptrPtrScheduleQueue)->scheduleList->source = ptrAuxOptSetMachine->source;
									}
									else {

										schedule *ptrNewSchedule;
										if ( (ptrNewSchedule = malloc(sizeof(schedule))) ) {

											ptrNewSchedule->scheduleID = scheduleID;
											ptrNewSchedule->scheduleTime = scheduleTime;
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

									printf("scheduleID %d schedTime %d taskID %d jobID %d RT %d machineID %d source %d timeLeft %d count %d\n",
											scheduleID, scheduleTime, ptrAuxOrderedTask->taskID, ptrAuxOrderedTask->jobID, ptrAuxOrderedTask->runtime,
											ptrAuxOptSetMachine->machineID, ptrAuxOptSetMachine->source, ptrAuxOptSetMachine->timeLeft, count);
									break;

								}

							} // end of if (ptrAuxOptSetMachine->source != GRID)
							else {

								if ( ptrAuxOrderedTask->runtime <= ptrAuxOptSetMachine->timeLeft && roundNumberOfGridMachines > 0 ) {

									roundNumberOfGridMachines--;
									scheduleID++;
									count++;
									allocated = 1;
									roundGridMachinesID++;

									unsigned int scheduleTime = (ptrCurrentEvent->time + 1);
									ptrAuxOptSetMachine->timeLeft = 0;

									if (scheduleID == 1) {
										(*ptrPtrScheduleQueue)->scheduleList->scheduleID = scheduleID;
										(*ptrPtrScheduleQueue)->scheduleList->scheduleTime = scheduleTime;
										(*ptrPtrScheduleQueue)->scheduleList->taskID = ptrAuxOrderedTask->taskID;
										(*ptrPtrScheduleQueue)->scheduleList->jobID = ptrAuxOrderedTask->jobID;
										(*ptrPtrScheduleQueue)->scheduleList->machineID = roundGridMachinesID;
										(*ptrPtrScheduleQueue)->scheduleList->source = ptrAuxOptSetMachine->source;
									}
									else {

										schedule *ptrNewSchedule;
										if ( (ptrNewSchedule = malloc(sizeof(schedule))) ) {

											ptrNewSchedule->scheduleID = scheduleID;
											ptrNewSchedule->scheduleTime = scheduleTime;
											ptrNewSchedule->taskID = ptrAuxOrderedTask->taskID;
											ptrNewSchedule->jobID = ptrAuxOrderedTask->jobID;
											ptrNewSchedule->machineID = roundGridMachinesID;
											ptrNewSchedule->source = ptrAuxOptSetMachine->source;
											ptrNewSchedule->nextSchedule = NULL;

										} else printf("ERROR (allocation planningOpt): merdou o malloc!!!\n");

										schedule *ptrAux;
										ptrAux = (*ptrPtrScheduleQueue)->scheduleList;
										while(ptrAux->nextSchedule != NULL)	ptrAux = ptrAux->nextSchedule;
										ptrAux->nextSchedule = ptrNewSchedule;

									}

									printf("scheduleID %d schedTime %d taskID %d jobID %d RT %d machineID %d source %d timeLeft %d count %d\n",
											scheduleID, scheduleTime, ptrAuxOrderedTask->taskID, ptrAuxOrderedTask->jobID, ptrAuxOrderedTask->runtime,
											ptrAuxOptSetMachine->machineID, ptrAuxOptSetMachine->source, ptrAuxOptSetMachine->timeLeft, count);
									break;

								} // end of if (ptrAuxOrderedTask->runtime <= ptrAuxOptSetMachine->timeLeft)

							} // end else of if (ptrAuxOptSetMachine->source != 1)

							ptrAuxOptSetMachine = ptrAuxOptSetMachine->nextMachineOptSet;

						} // end of while(ptrAuxOptSetMachine)

					} // end of if (ptrAuxOrderedTask->jobID == ptrCurrentEvent->jobInfo.jobID)

					ptrAuxOrderedTask = ptrAuxOrderedTask->nextTask;
				} // end of while(ptrAuxOrderedTask)

				// verifying if all the tasks were computed on this target finish time
				if (count == ptrCurrentEvent->jobInfo.jobSize) {

					(*ptrPtrScheduleQueue)->status = SUCCESSFUL;

					// calculating the utility based on the target finish time
					switch(utilityFunction) {
						case CONSTANT:
							if ( (targetFinnishTime - ptrCurrentEvent->jobInfo.arrivalTime) <= ptrCurrentEvent->jobInfo.deadline ) {
								targetUtility = ptrCurrentEvent->jobInfo.maxUtility;
							}
							else {
								targetUtility = 0;
							}
							break;
						case LINEAR:
							if ( (targetFinnishTime - ptrCurrentEvent->jobInfo.arrivalTime) <= ptrCurrentEvent->jobInfo.deadline ) {
								targetUtility = ( (-1)*(ptrCurrentEvent->jobInfo.maxUtility/ptrCurrentEvent->jobInfo.deadline)*(targetFinnishTime - ptrCurrentEvent->jobInfo.arrivalTime) + ptrCurrentEvent->jobInfo.maxUtility );
							}
							else {
								targetUtility = 0;
							}
							break;
						case STEP:
							if ( (targetFinnishTime - ptrCurrentEvent->jobInfo.arrivalTime) <= (ptrCurrentEvent->jobInfo.deadline/3) ) {
								targetUtility = ptrCurrentEvent->jobInfo.maxUtility;
							}
							else {
								if ( (targetFinnishTime - ptrCurrentEvent->jobInfo.arrivalTime) <= (2*ptrCurrentEvent->jobInfo.deadline/3) ) {
									targetUtility = (2*ptrCurrentEvent->jobInfo.maxUtility/3);
								}
								else {
									if ( (targetFinnishTime - ptrCurrentEvent->jobInfo.arrivalTime) <= ptrCurrentEvent->jobInfo.deadline ) {
										targetUtility = (ptrCurrentEvent->jobInfo.maxUtility/3);
									}
									else {
										if ( (targetFinnishTime - ptrCurrentEvent->jobInfo.arrivalTime) > ptrCurrentEvent->jobInfo.deadline ) {
											targetUtility = 0;
										}
										else {
											printf("ERROR (job finnished): switch STEP!!!\n");
										}
									}
								}
							}
							break;
						default:
							break;
					} // end of switch(utilityFunction)
				}
				else {
					(*ptrPtrScheduleQueue)->status = FAILED;
					targetUtility = 0;
				}

				// CALCULAR OS CUSTOS A PARTIR DA UTILIZACAO DAS MAQUINAS DA CLOUD
				unsigned int totalUsedTime = 0;
				ptrAuxOptSet = ptrMachineOptSetList;
				while(ptrAuxOptSet) {

					totalUsedTime = ( (targetFinnishTime - ptrAuxOptSet->timeLeft) - (ptrCurrentEvent->time + 1) );

					if (ptrAuxOptSet->source == 2) {
						targetCost += ceil( (float)(totalUsedTime) / 60.0 ) * 0.22;
					}
					if (ptrAuxOptSet->source == 3) {
						targetCost += ceil( (float)(totalUsedTime) / 60.0 ) * 0.92;
					}

					ptrAuxOptSet = ptrAuxOptSet->nextMachineOptSet;
				}

				targetCost += reservationPricePerDay;


				// CALCULAR O PROFIT COMO RENDIMENTO - CUSTO TOTAL
				(*ptrPtrScheduleQueue)->profit = (float)targetUtility - targetCost;

			} // end of for (i = firstTargetFinnishTime; i <= deadline; i += timeSteps)


			// COMPARA O PROFIT DO targetFinnishTime ATUAL COM O bestProfit
			// CASO O PROFIT ATUAL SEJA MAIOR,  ATUALIZAR bestProfit E APONTAR ptrTargetScheduleList PARA ptrNewScheduledQueue->scheduleList
			// CRIAR AS MAQUINAS DO GRID (machineArrival() e machineDeparture())
			// DEPOIS QUE EU TIVER O SET DE MAIOR PROFIT, DEVO VARRER A scheduleList E GERAR AS MAQUINAS DA GRADE

			// AJEITAR ISSO AQUI EM BAIXO!!!!

			// freeing the memory allocated for the optimized machine set list
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

			// LEMBRAR DE LIBERAR O ESPACO EM MEMORIA DO ptrPtrScheduleQueue
			scheduleQueue *ptrAuxScheduleQueue;
			ptrAuxScheduleQueue = (*ptrPtrScheduleQueue);
			while(ptrAuxScheduleQueue) {

				printf("targetFT %d status %d profit %.2f status %d\n", ptrAuxScheduleQueue->targetFinnishtime, ptrAuxScheduleQueue->status,
						ptrAuxScheduleQueue->profit, ptrAuxScheduleQueue->status);
				printf("***************************************************\n");

				schedule *ptrAuxSchedule;
				ptrAuxSchedule = ptrAuxScheduleQueue->scheduleList;
				while(ptrAuxSchedule) {

					printf("scheduleID %d schedTime %d taskID %d jobID %d machineID %d source %d\n",
							ptrAuxSchedule->scheduleID, ptrAuxSchedule->scheduleTime, ptrAuxSchedule->taskID, ptrAuxSchedule->jobID,
							ptrAuxSchedule->machineID, ptrAuxSchedule->source);

					ptrAuxSchedule = ptrAuxSchedule->nextSchedule;
				}
				printf("\n");

				ptrAuxScheduleQueue = ptrAuxScheduleQueue->previousSchedule;
			}




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
