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

void AllocationPlanningOpt(event *ptrCurrentEvent, event *ptrEventList, machine *ptrMachineList, task *ptrOrderedTaskList, job *ptrJobList,
		balanceAccountInfo **ptrPtrBalanceAccountInfo) {

	if (ptrCurrentEvent->eventID == ALLOCATIONPLANNING) {

		if (ptrMachineList && ptrOrderedTaskList) {

			unsigned long int balance, numberOfGridMachines;
			balance = GetBalance(ptrPtrBalanceAccountInfo, ptrCurrentEvent->time);
			numberOfGridMachines = (long int)((balance * gridQoSFactor)/taskAvgTime); // ceiling or trunk???
//			printf("numero de maquinas do grid: %d\n", numberOfGridMachines); //debug mode

			unsigned int firstTargetFinnishTime, deadline, timeSteps;
			firstTargetFinnishTime = (ptrCurrentEvent->jobInfo.arrivalTime + 2 + ptrCurrentEvent->jobInfo.longestTask); // AT + 2min to start a job + LT
			deadline = ptrCurrentEvent->jobInfo.deadline;
			timeSteps = 1; // steps, in minutes, for the optimizing process

			unsigned int scheduleID;
			unsigned short int allocated = 0;

			scheduleQueue *ptrBestScheduleQueue;
			schedule *ptrBestScheduleList;
			if ( (ptrBestScheduleQueue = malloc(sizeof(scheduleQueue))) )  {
				ptrBestScheduleQueue->targetFinnishtime = 0;
				if ( !(ptrBestScheduleQueue->scheduleList = malloc(sizeof(schedule))) ) printf("ERROR (allocation planningOpt): merdou o 1o malloc!!!\n");
				ptrBestScheduleQueue->status = UNFINNISHED;
				ptrBestScheduleQueue->utility = 0;
				ptrBestScheduleQueue->cost = 0.0;
				ptrBestScheduleQueue->profit = -100000000;
				ptrBestScheduleQueue->previousSchedule = NULL;

			} else printf("ERROR (allocation planningOpt): merdou o 2o malloc!!!\n");
			ptrBestScheduleList = ptrBestScheduleQueue->scheduleList;

			// creating an optimized machine set list and filling it after
			machineOptSet *ptrMachineOptSetList;
			if ( (ptrMachineOptSetList = malloc(sizeof(machineOptSet))) ) {
				ptrMachineOptSetList->machineID = 0;
				ptrMachineOptSetList->source = 0;
				ptrMachineOptSetList->timeLeft = 0;
				ptrMachineOptSetList->upTime = 0;
				ptrMachineOptSetList->nextMachineOptSet = NULL;
			}
			else printf("ERROR (allocation planningOpt): merdou o 3o malloc!!!\n");

			// filling ptrMachineOptSetList with local machines
			machine *ptrAuxMachine;
			ptrAuxMachine = ptrMachineList;
			while(ptrAuxMachine->source == 0) {

				if (ptrMachineOptSetList->machineID == 0) {  // means there are no machines in the list yet

					ptrMachineOptSetList->machineID = ptrAuxMachine->machineID;
					ptrMachineOptSetList->source = ptrAuxMachine->source;
					ptrMachineOptSetList->timeLeft = 0;
					ptrMachineOptSetList->upTime = 0;
					ptrMachineOptSetList->nextMachineOptSet = NULL;

				}
				else {

					machineOptSet *ptrAux1;
					if ( (ptrAux1 = malloc(sizeof(machineOptSet))) ) {

						ptrAux1->machineID = ptrAuxMachine->machineID;
						ptrAux1->source = ptrAuxMachine->source;
						ptrAux1->timeLeft = 0;
						ptrAux1->upTime = 0;
						ptrAux1->nextMachineOptSet = NULL;

						machineOptSet *ptrAux2;
						ptrAux2 = ptrMachineOptSetList;
						while(ptrAux2->nextMachineOptSet != NULL) ptrAux2 = ptrAux2->nextMachineOptSet;

						ptrAux2->nextMachineOptSet = ptrAux1;

						ptrAux1 = NULL;
						ptrAux2 = NULL;

					}
					else printf("ERROR (allocation planningOpt): merdou o 4o malloc!!!\n");

				}

				ptrAuxMachine = ptrAuxMachine->nextMachine;
			} // end of while(ptrAuxMachine->source == 0)

			// filling ptrMachineOptSetList with grid machines
			machineOptSet *ptrAux1;
			ptrAux1 = ptrMachineOptSetList;
			while(ptrAux1->nextMachineOptSet != NULL) ptrAux1 = ptrAux1->nextMachineOptSet;

			unsigned long int i;
			for (i = 1; (i <= jobSize && i <= numberOfGridMachines); i++) {

//				 debug mode
//				if (ptrCurrentEvent->eventNumber == 9749) {
//					passou();
//					printf("numberOfGridMachines %ld", i);
//				}

				unsigned int avgUpTime = (int)Randn(gridAvgUptime, (gridAvgUptime*0.1));

				if (ptrMachineOptSetList->machineID == 0) {  // means there are no machines in the list yet

					ptrMachineOptSetList->machineID = 1;
					ptrMachineOptSetList->source = 1;
					ptrMachineOptSetList->timeLeft = avgUpTime;
					ptrMachineOptSetList->upTime = avgUpTime;
					ptrMachineOptSetList->nextMachineOptSet = NULL;

				}
				else {

					machineOptSet *ptrAux2;
					if ( (ptrAux2 = malloc(sizeof(machineOptSet))) ) {

						ptrAux2->machineID = 1;
						ptrAux2->source = 1;
						ptrAux2->timeLeft = avgUpTime;
						ptrAux2->upTime = avgUpTime;
						ptrAux2->nextMachineOptSet = NULL;

						ptrAux1->nextMachineOptSet = ptrAux2;
						ptrAux1 = ptrAux2;
						ptrAux2 = NULL;

					}
					else printf("ERROR (allocation planningOpt): merdou o 5o malloc!!!\n");

				}

			}
//			printf("numero de maquinas do grid: %d\n", i); //debug mode
			ptrAux1 = NULL;

			// filling ptrMachineOptSetList with cloud machines (whith no spot machines!!!)
			ptrAuxMachine = ptrMachineList;
			while(ptrAuxMachine) {

				if ( ptrAuxMachine->source == 2 || ptrAuxMachine->source == 3 ) {

					if (ptrMachineOptSetList->machineID == 0) {  // means there are no machines in the list yet

						ptrMachineOptSetList->machineID = ptrAuxMachine->machineID;
						ptrMachineOptSetList->source = ptrAuxMachine->source;
						ptrMachineOptSetList->timeLeft = 0;
						ptrMachineOptSetList->upTime = 0;
						ptrMachineOptSetList->nextMachineOptSet = NULL;

					}
					else {

						machineOptSet *ptrAux1;
						if ( (ptrAux1 = malloc(sizeof(machineOptSet))) ) {

							ptrAux1->machineID = ptrAuxMachine->machineID;
							ptrAux1->source = ptrAuxMachine->source;
							ptrAux1->timeLeft = 0;
							ptrAux1->upTime = 0;
							ptrAux1->nextMachineOptSet = NULL;

							machineOptSet *ptrAux2;
							ptrAux2 = ptrMachineOptSetList;
							while(ptrAux2->nextMachineOptSet != NULL) ptrAux2 = ptrAux2->nextMachineOptSet;

							ptrAux2->nextMachineOptSet = ptrAux1;
							ptrAux2 = NULL;

						}
						else printf("ERROR (allocation planningOpt): merdou o 6o malloc!!!\n");
						ptrAux1 = NULL;

					}

				}

				ptrAuxMachine = ptrAuxMachine->nextMachine;
			} // end of while(ptrAuxMachine)

			// sweeping the time till deadline
			unsigned int targetFinnishTime;
			for (targetFinnishTime = firstTargetFinnishTime; targetFinnishTime <= deadline; targetFinnishTime += timeSteps) {

//				debug mode
//				printf("\n");
//				printf("targetFT %d firstTargetFT %d deadline %d\n", targetFinnishTime, firstTargetFinnishTime, deadline);
//				printf("***************************************************\n");

				unsigned int count = 0;
				unsigned int targetUtility = 0;
				float targetCost = 0.0;

				task *ptrAuxOrderedTask;
				ptrAuxOrderedTask = ptrOrderedTaskList;
				scheduleID = 0;

				// setting the left time to the machines
				machineOptSet *ptrAuxOptSet;
				ptrAuxOptSet = ptrMachineOptSetList;
				while(ptrAuxOptSet) {
					if (ptrAuxOptSet->source != 1) ptrAuxOptSet->timeLeft = (targetFinnishTime - (ptrCurrentEvent->time + 1));
					if (ptrAuxOptSet->source == 1) ptrAuxOptSet->timeLeft = ptrAuxOptSet->upTime;
					ptrAuxOptSet = ptrAuxOptSet->nextMachineOptSet;
				}

				// initializing a new node for the ScheduleQueue
				scheduleQueue *ptrNewScheduleQueue;
				if ( (ptrNewScheduleQueue = malloc(sizeof(scheduleQueue))) ) {
					ptrNewScheduleQueue->targetFinnishtime = targetFinnishTime;
					if ( !(ptrNewScheduleQueue->scheduleList = malloc(sizeof(schedule))) ) printf("ERROR (allocation planningOpt): merdou o 7o malloc!!!\n");
					ptrNewScheduleQueue->scheduleList->scheduleID = 0;
					ptrNewScheduleQueue->scheduleList->scheduleTime = 0;
					ptrNewScheduleQueue->scheduleList->taskID = 0;
					ptrNewScheduleQueue->scheduleList->jobID = 0;
					ptrNewScheduleQueue->scheduleList->runtime = 0;
					ptrNewScheduleQueue->scheduleList->machineID = 0;
					ptrNewScheduleQueue->scheduleList->source = 0;
					ptrNewScheduleQueue->scheduleList->nextSchedule = NULL;
					ptrNewScheduleQueue->status = UNFINNISHED;
					ptrNewScheduleQueue->utility = 0;
					ptrNewScheduleQueue->cost = 0.0;
					ptrNewScheduleQueue->profit = 0.0;
					ptrNewScheduleQueue->previousSchedule = NULL;
				}
				else printf("ERROR (allocation planningOpt): merdou o 8o malloc!!!\n");

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
                                            ptrNewScheduleQueue->scheduleList->scheduleID = scheduleID;
                                            ptrNewScheduleQueue->scheduleList->scheduleTime = scheduleTime;
                                            ptrNewScheduleQueue->scheduleList->taskID = ptrAuxOrderedTask->taskID;
                                            ptrNewScheduleQueue->scheduleList->jobID = ptrAuxOrderedTask->jobID;
                                            ptrNewScheduleQueue->scheduleList->runtime = ptrAuxOrderedTask->runtime;
                                            ptrNewScheduleQueue->scheduleList->machineID = ptrAuxOptSetMachine->machineID;
                                            ptrNewScheduleQueue->scheduleList->source = ptrAuxOptSetMachine->source;
                                            ptrNewScheduleQueue->scheduleList->nextSchedule = NULL;
                                    }
                                    else {

                                    	schedule *ptrNewSchedule;
                                    	if ( (ptrNewSchedule = malloc(sizeof(schedule))) ) {

                                    		ptrNewSchedule->scheduleID = scheduleID;
                                    		ptrNewSchedule->scheduleTime = scheduleTime;
                                    		ptrNewSchedule->taskID = ptrAuxOrderedTask->taskID;
                                    		ptrNewSchedule->jobID = ptrAuxOrderedTask->jobID;
                                    		ptrNewSchedule->runtime = ptrAuxOrderedTask->runtime;
                                    		ptrNewSchedule->machineID = ptrAuxOptSetMachine->machineID;
                                    		ptrNewSchedule->source = ptrAuxOptSetMachine->source;
                                    		ptrNewSchedule->nextSchedule = NULL;

                                    	} else printf("ERROR (allocation planningOpt): merdou o 9o malloc!!!\n");

                                    	schedule *ptrAux;
                                    	ptrAux = ptrNewScheduleQueue->scheduleList;
                                    	while(ptrAux->nextSchedule != NULL) ptrAux = ptrAux->nextSchedule;
                                    	ptrAux->nextSchedule = ptrNewSchedule;

                                    	ptrAux = NULL;
                                    	ptrNewSchedule = NULL;

                                    }
//									debug mode
//									printf("scheduleID %d schedTime %d taskID %d jobID %d RT %d machineID %d source %d timeLeft %d count %d\n",
//											scheduleID, scheduleTime, ptrAuxOrderedTask->taskID, ptrAuxOrderedTask->jobID, ptrAuxOrderedTask->runtime,
//											ptrAuxOptSetMachine->machineID, ptrAuxOptSetMachine->source, ptrAuxOptSetMachine->timeLeft, count);
									break;

								}

							} // end of if (ptrAuxOptSetMachine->source != 1)
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
                                            ptrNewScheduleQueue->scheduleList->scheduleID = scheduleID;
                                            ptrNewScheduleQueue->scheduleList->scheduleTime = scheduleTime;
                                            ptrNewScheduleQueue->scheduleList->taskID = ptrAuxOrderedTask->taskID;
                                            ptrNewScheduleQueue->scheduleList->jobID = ptrAuxOrderedTask->jobID;
                                            ptrNewScheduleQueue->scheduleList->runtime = ptrAuxOrderedTask->runtime;
                                            ptrNewScheduleQueue->scheduleList->machineID = roundGridMachinesID;
                                            ptrNewScheduleQueue->scheduleList->source = ptrAuxOptSetMachine->source;
                                            ptrNewScheduleQueue->scheduleList->nextSchedule = NULL;
                                    }
                                    else {

                                    	schedule *ptrNewSchedule;
                                    	if ( (ptrNewSchedule = malloc(sizeof(schedule))) ) {

                                    		ptrNewSchedule->scheduleID = scheduleID;
                                    		ptrNewSchedule->scheduleTime = scheduleTime;
                                    		ptrNewSchedule->taskID = ptrAuxOrderedTask->taskID;
                                    		ptrNewSchedule->jobID = ptrAuxOrderedTask->jobID;
                                    		ptrNewSchedule->runtime = ptrAuxOrderedTask->runtime;
                                    		ptrNewSchedule->machineID = roundGridMachinesID;
                                    		ptrNewSchedule->source = ptrAuxOptSetMachine->source;
                                    		ptrNewSchedule->nextSchedule = NULL;

                                    	} else printf("ERROR (allocation planningOpt): merdou o 10o malloc!!!\n");

                                    	schedule *ptrAux;
                                    	ptrAux = ptrNewScheduleQueue->scheduleList;
                                    	while(ptrAux->nextSchedule != NULL) ptrAux = ptrAux->nextSchedule;
                                    	ptrAux->nextSchedule = ptrNewSchedule;

                                    	ptrAux = NULL;
                                    	ptrNewSchedule = NULL;

                                    }
//									debug mode
//									printf("scheduleID %d schedTime %d taskID %d jobID %d RT %d machineID %d source %d timeLeft %d count %d\n",
//											scheduleID, scheduleTime, ptrAuxOrderedTask->taskID, ptrAuxOrderedTask->jobID, ptrAuxOrderedTask->runtime,
//											roundGridMachinesID, ptrAuxOptSetMachine->source, ptrAuxOptSetMachine->timeLeft, count);
									break;

								} // end of if ( ptrAuxOrderedTask->runtime <= ptrAuxOptSetMachine->timeLeft && roundNumberOfGridMachines > 0 )

							} // end else of if (ptrAuxOptSetMachine->source != 1)

							ptrAuxOptSetMachine = ptrAuxOptSetMachine->nextMachineOptSet;

						} // end of while(ptrAuxOptSetMachine)

					} // end of if (ptrAuxOrderedTask->jobID == ptrCurrentEvent->jobInfo.jobID)

					ptrAuxOrderedTask = ptrAuxOrderedTask->nextTask;
				} // end of while(ptrAuxOrderedTask)

				// verifying if all the tasks were computed on this target finish time
				if (count == ptrCurrentEvent->jobInfo.jobSize) {

					ptrNewScheduleQueue->status = SUCCESSFUL;

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
							if ( (targetFinnishTime - ptrCurrentEvent->jobInfo.arrivalTime) < ptrCurrentEvent->jobInfo.deadline ) {
								targetUtility = ( (-1)*(ptrCurrentEvent->jobInfo.maxUtility/(ptrCurrentEvent->jobInfo.deadline-ptrCurrentEvent->jobInfo.arrivalTime))*(targetFinnishTime - ptrCurrentEvent->jobInfo.arrivalTime) + ptrCurrentEvent->jobInfo.maxUtility );
							}
							else {
								targetUtility = 0;
							}
//							debug mode
//							if (ptrCurrentEvent->eventNumber == 9748 && targetFinnishTime == 28654) {
//								printf("targetUtility %d x %d maxUtility %d deadline %d targetFT %d jobArrival %d\n", targetUtility, (targetFinnishTime - ptrCurrentEvent->jobInfo.arrivalTime),
//										ptrCurrentEvent->jobInfo.maxUtility, ptrCurrentEvent->jobInfo.deadline, targetFinnishTime, ptrCurrentEvent->jobInfo.arrivalTime);
//							}
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
											printf("ERROR (allocation planningOpt): switch STEP!!!\n");
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
					ptrNewScheduleQueue->status = FAILED;
					targetUtility = 0;
				}

				// calculating the costs based on the machine used times
//				FILE *ptrFileDebug;
//				ptrFileDebug = fopen("debug.txt", "a+");
//				fprintf(ptrFileDebug, "********************* targetFT %d *********************\n", targetFinnishTime);
				unsigned int totalUsedTime = 0;
				ptrAuxOptSet = ptrMachineOptSetList;
				while(ptrAuxOptSet) {

					if (ptrAuxOptSet->source == 1 && ptrAuxOptSet->timeLeft != 0) { totalUsedTime = 0; }
					else { totalUsedTime = ( (targetFinnishTime - ptrAuxOptSet->timeLeft) - (ptrCurrentEvent->time + 1) ); }

					if (ptrAuxOptSet->source == 2) {
						targetCost += ceil( (float)(totalUsedTime) / 60.0 ) * reservedUsagePrice;
					}
					if (ptrAuxOptSet->source == 3) {
						targetCost += ceil( (float)(totalUsedTime) / 60.0 ) * ondemandUsagePrice;
					}

//					fprintf(ptrFileDebug, "source %d totalUsedTime %d targetCost %.2f\n", ptrAuxOptSet->source, totalUsedTime, targetCost); // debug mode
					ptrAuxOptSet = ptrAuxOptSet->nextMachineOptSet;
				}

				targetCost += reservationPricePerDay;
				ptrNewScheduleQueue->utility = targetUtility;
				ptrNewScheduleQueue->cost = targetCost;
				ptrNewScheduleQueue->profit = (float)targetUtility - targetCost;
//				fprintf(ptrFileDebug, "profit %.2f targetUtility %d targetCost %.2f\n", ((float)targetUtility - targetCost), targetUtility, targetCost); // debug mode
//				fclose(ptrFileDebug);

				// comparing to find out the best profit scenario
				if (ptrNewScheduleQueue->profit > ptrBestScheduleQueue->profit) {

					scheduleQueue *ptrAuxScheduleQueue;
					ptrAuxScheduleQueue = ptrBestScheduleQueue;
					ptrBestScheduleQueue = ptrNewScheduleQueue;
					ptrBestScheduleList = ptrBestScheduleQueue->scheduleList;
					free(ptrAuxScheduleQueue);
					ptrAuxScheduleQueue = NULL;
					ptrNewScheduleQueue = NULL;

				}
				else {
					free(ptrNewScheduleQueue);
					ptrNewScheduleQueue = NULL;
				}

			} // end of for (i = firstTargetFinnishTime; i <= deadline; i += timeSteps)

			// freeing the memory allocated for the optimized machine set list
//			printf("\n");
			machineOptSet *ptrActual;
			ptrActual = ptrMachineOptSetList;
			while(ptrActual) {
//					printf("machineID %d source %d timeleft %d upTime %d\n", ptrActual->machineID, ptrActual->source, ptrActual->timeLeft, ptrActual->upTime);
				machineOptSet *ptrLast;
				ptrLast = ptrActual;
				ptrActual = ptrActual->nextMachineOptSet;
				free(ptrLast);
				ptrLast = NULL;
			}
//			printf("\n");
			ptrMachineOptSetList = NULL;

			// adding job information into the jobList (utility and cost)
			job *ptrAuxJobList;
			ptrAuxJobList = ptrJobList;
			while(ptrAuxJobList) {

				if (ptrAuxJobList->jobID == ptrBestScheduleQueue->scheduleList->jobID) break;

				ptrAuxJobList = ptrAuxJobList->nextJob;
			}

			if (ptrAuxJobList != NULL) {
				ptrAuxJobList->utility = ptrBestScheduleQueue->utility;
				ptrAuxJobList->cost = ptrBestScheduleQueue->cost;
			}
			else printf("ERROR (allocation planningOpt): job not found!!!\n");

////			debug mode
//			if (ptrCurrentEvent->eventNumber == 9748) {
//
//				FILE *ptrFileDebug;
//				ptrFileDebug = fopen("debug.txt", "a+");
//
//				scheduleQueue *fila;
//				fila = ptrBestScheduleQueue;
//				schedule *teste;
//				teste = ptrBestScheduleList;
//
//				fprintf(ptrFileDebug, "********************* targetFT %d *********************\n", fila->targetFinnishtime);
//				while(teste) {
//					fprintf(ptrFileDebug, "scheduleID %d schedTime %d taskID %d jobID %d RT %d machineID %d source %d\n",
//							teste->scheduleID, teste->scheduleTime, teste->taskID,
//							teste->jobID, teste->runtime,
//							teste->machineID, teste->source);
//					teste = teste->nextSchedule; // degub mode
//				}
//				fprintf(ptrFileDebug, "status %d utility %d cost %.2f profit %.2f\n", fila->status, fila->utility, fila->cost, fila->profit);
//				fclose(ptrFileDebug);
//
//			}

			// CRIAR AS MAQUINAS DO GRID (machineArrival() e machineDeparture())
			// DEPOIS QUE EU TIVER O SET DE MAIOR PROFIT, DEVO VARRER A scheduleList E GERAR AS MAQUINAS DA GRADE
			// DESALOCAR A MEMORIA DE ptrBestScheduleList
			while(ptrBestScheduleList) {

				// creating the grid machines: arrivals and departures
				if (ptrBestScheduleList->source == 1) {

					// insert a machine arrival event into the event list
					event *ptrNewGridMachine;
					if( (ptrNewGridMachine = malloc(sizeof(event))) ) {
						ptrNewGridMachine->eventNumber = 0;
						ptrNewGridMachine->eventID = MACHARRIVAL;
						ptrNewGridMachine->time = ptrCurrentEvent->time;
						ptrNewGridMachine->machineInfo.machineID = ptrBestScheduleList->machineID;
						ptrNewGridMachine->machineInfo.source = ptrBestScheduleList->source;
						ptrNewGridMachine->machineInfo.status = RUNNING;
						ptrNewGridMachine->machineInfo.arrivalTime = ptrCurrentEvent->time;
						ptrNewGridMachine->machineInfo.departureTime = (ptrCurrentEvent->time + 1 + ptrBestScheduleList->runtime);
						ptrNewGridMachine->machineInfo.usagePrice = 0.0;
						ptrNewGridMachine->machineInfo.reservationPrice = 0.0;
						ptrNewGridMachine->machineInfo.nextMachine = NULL;
						ptrNewGridMachine->nextEvent = NULL;

						InsertEvent(ptrEventList, ptrNewGridMachine);
					}
					else printf("ERROR (allocation planning): merdou o 11o malloc!!!\n");

					ptrNewGridMachine = NULL;

					// insert a machine arrival event into the event list
					event *ptrOutGridMachine;
					if( (ptrOutGridMachine = malloc(sizeof(event))) ) {
						ptrOutGridMachine->eventNumber = 0;
						ptrOutGridMachine->eventID = MACHDEPARTURE;
						ptrOutGridMachine->time = (ptrCurrentEvent->time + 1 + ptrBestScheduleList->runtime);
						ptrOutGridMachine->machineInfo.machineID = ptrBestScheduleList->machineID;
						ptrOutGridMachine->machineInfo.source = ptrBestScheduleList->source;
						ptrOutGridMachine->machineInfo.status = IDLE;
						ptrOutGridMachine->machineInfo.arrivalTime = ptrCurrentEvent->time;
						ptrOutGridMachine->machineInfo.departureTime = (ptrCurrentEvent->time + 1 + ptrBestScheduleList->runtime);
						ptrOutGridMachine->machineInfo.usagePrice = 0.0;
						ptrOutGridMachine->machineInfo.reservationPrice = 0.0;
						ptrOutGridMachine->machineInfo.nextMachine = NULL;
						ptrOutGridMachine->nextEvent = NULL;

						InsertEvent(ptrEventList, ptrOutGridMachine);
					}
					else printf("ERROR (allocation planning): merdou o 12o malloc!!!\n");

					ptrOutGridMachine = NULL;

					// insert a new task schedule into the event list
					event *ptrNewEvent;
					if( (ptrNewEvent = malloc(sizeof(event))) ) {
						ptrNewEvent->eventNumber = 0;
						ptrNewEvent->eventID = TASKSCHEDULE;
						ptrNewEvent->time = ptrBestScheduleList->scheduleTime;
						ptrNewEvent->scheduleInfo.scheduleID = ptrBestScheduleList->scheduleID;
						ptrNewEvent->scheduleInfo.scheduleTime = ptrBestScheduleList->scheduleTime;
						ptrNewEvent->scheduleInfo.taskID = ptrBestScheduleList->taskID;
						ptrNewEvent->scheduleInfo.jobID = ptrBestScheduleList->jobID;
						ptrNewEvent->scheduleInfo.runtime = ptrBestScheduleList->runtime;
						ptrNewEvent->scheduleInfo.machineID = ptrBestScheduleList->machineID;
						ptrNewEvent->scheduleInfo.source = ptrBestScheduleList->source;
						ptrNewEvent->scheduleInfo.nextSchedule = ptrBestScheduleList->nextSchedule;
						ptrNewEvent->nextEvent = NULL;

						InsertEvent(ptrEventList, ptrNewEvent);
					}
					else printf("ERROR (allocation planning): merdou o 13o malloc!!!\n");

					ptrNewEvent = NULL;

				} // end of if (ptrBestScheduleList->source == 1)
				else {

					// insert a new task schedule into the event list
					event *ptrNewEvent;
					if( (ptrNewEvent = malloc(sizeof(event))) ) {
						ptrNewEvent->eventNumber = 0;
						ptrNewEvent->eventID = TASKSCHEDULE;
						ptrNewEvent->time = ptrBestScheduleList->scheduleTime;
						ptrNewEvent->scheduleInfo.scheduleID = ptrBestScheduleList->scheduleID;
						ptrNewEvent->scheduleInfo.scheduleTime = ptrBestScheduleList->scheduleTime;
						ptrNewEvent->scheduleInfo.taskID = ptrBestScheduleList->taskID;
						ptrNewEvent->scheduleInfo.jobID = ptrBestScheduleList->jobID;
						ptrNewEvent->scheduleInfo.runtime = ptrBestScheduleList->runtime;
						ptrNewEvent->scheduleInfo.machineID = ptrBestScheduleList->machineID;
						ptrNewEvent->scheduleInfo.source = ptrBestScheduleList->source;
						ptrNewEvent->scheduleInfo.nextSchedule = ptrBestScheduleList->nextSchedule;
						ptrNewEvent->nextEvent = NULL;

						InsertEvent(ptrEventList, ptrNewEvent);
					}
					else printf("ERROR (allocation planningOpt): merdou o 14o malloc!!!\n");

					ptrNewEvent = NULL;

				}

//				printf("***************************************************\n");
//				printf("scheduleID %d schedTime %d taskID %d jobID %d RT %d machineID %d source %d\n",
//						ptrBestScheduleList->scheduleID, ptrBestScheduleList->scheduleTime, ptrBestScheduleList->taskID, ptrBestScheduleList->jobID,
//						ptrBestScheduleList->runtime, ptrBestScheduleList->machineID, ptrBestScheduleList->source);


				ptrBestScheduleList = ptrBestScheduleList->nextSchedule;
			}

			free(ptrBestScheduleQueue);
			ptrBestScheduleQueue = NULL;
			ptrBestScheduleList = NULL;

			// donating to the grid unscheduled local machines
			event *ptrAuxEventList;
			ptrAuxEventList = ptrEventList;
			ptrAuxMachine = ptrMachineList;
			while(ptrAuxMachine) {

				if (ptrAuxMachine->source == LOCAL) {

					unsigned short int found = 0;
					ptrAuxEventList = ptrEventList;
					while(ptrAuxEventList) {

						if ( ptrAuxEventList->time == (ptrCurrentEvent->time + 1) && ptrAuxEventList->eventID == TASKSCHEDULE &&
							   ptrAuxEventList->scheduleInfo.machineID == ptrAuxMachine->machineID ) {
							found = 1;
							break;
						}

						ptrAuxEventList = ptrAuxEventList->nextEvent;
					}

					if (!found) {

						event *ptrNewDonation;

						if( (ptrNewDonation = malloc(sizeof(event))) ) {

							ptrNewDonation->eventNumber = 0;
							ptrNewDonation->eventID = GRIDDONATING;
							ptrNewDonation->time = ptrCurrentEvent->time;
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

						} else printf("ERROR (allocation planningOpt): merdou o 15o malloc!!!\n");

						ptrNewDonation = NULL;

					}

				}

				ptrAuxMachine = ptrAuxMachine->nextMachine;
			}

			printf("eventID %d (Allocation Planning) time %ld ", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
			printf("Allocation %d\n", allocated);

		} else printf("ERROR (allocation planning): there is no machine or task list!!!\n");

	} else printf("ERROR (allocation planning): wrong eventID!!!\n");
} // end of AllocationPlanningOpt()
