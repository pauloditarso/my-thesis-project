/*
 ============================================================================
 Name        : simulator.c
 Author      : Paulo Ditarso
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "simulation.h"

//#define CPU_TIME (getrusage(RUSAGE_SELF,&ruse), ruse.ru_utime.tv_sec + ruse.ru_stime.tv_sec + 1e-6 * (ruse.ru_utime.tv_usec + ruse.ru_stime.tv_usec))

int main(int argc, char *argv[]) {

	time_t newStart, newEnd;
	time(&newStart);
//	double first, second;
//	first = CPU_TIME;
//	clock_t start = clock();

	setbuf(stdout, NULL); // debugguing mode

	event *eventList, *ptrAuxList, *ptrLastNode;
	machine *machineList, *ptrAuxMachine;
	task *taskList, *orderedTaskList;
	job *jobList;
	taskAccountInfo *taskAccountInfoList;
	gridAccountInfo *gridInfoList;
	jobAccountInfo *jobAccountInfoList;
	balanceAccountInfo *balanceAccountInfoList;
	schedule *scheduleList;

	unsigned long int count = 0;
	unsigned long int totalNumberOfEvents = 0;
	gridMachinesID = 0; scheduleID = 0;
	aggregatedUtility = 0; aggregatedCost =0; aggregatedProfit = 0;
//	balanceCredit = 0;

//	unsigned long int totalUtility = 0;
//	float totalCost = 0.0, totalProfit = 0.0;

	if (argc == 14) {

		optFlag = (atoi(argv[1]));
		simulationTime = ((atoi(argv[2]) * 24 * 60) + 1440); // simulationTime is a global variable (minutes) based on the input parameter (days)
		jobSize = (atoi(argv[3]));
		gridQoSFactor = (atof(argv[4]));
		ondemandPriceFactor = (atof(argv[5]));
		inhouseFactor = (atof(argv[6]));
		numberOfLocalMachines = (atoi(argv[7]));
		numberOfReservedMachines = atoi(argv[8]);
		numberOfOnDemandMachines = atoi(argv[9]);
		if (atoi(argv[10]) == 0) {
			taskAvgTime = 10;
			taskSdvTime = 5;
		} else {
			taskAvgTime = 47;
			taskSdvTime = 26;
		}
		gridAvgUptime = (atoi(argv[11]));
		utilityFunction = (atoi(argv[12]));
		simSeed = (atoi(argv[13]));

		ondemandUsagePrice = (0.08*ondemandPriceFactor); reservedUsagePrice = 0.064; reservationPrice = 124.00;
		reservationPricePerDay = (numberOfReservedMachines * reservationPrice)/365;

		srand(simSeed);

		// starting a new event list
		if ( (eventList = malloc(sizeof(event))) ) {
			// start simulation event
			eventList->eventNumber = 0; eventList->eventID = 0; eventList->time = 0; eventList->flag = -1; eventList->nextEvent = NULL;
		} else printf("ERROR (main simulator): merdou o malloc!!!\n");

		// finish simulation event
		if ( (ptrLastNode = malloc(sizeof(event))) ) {
			ptrLastNode->eventNumber = 1; ptrLastNode->eventID = SIMFINNISHED; ptrLastNode->time = (simulationTime+1); ptrLastNode->flag = -1; ptrLastNode->nextEvent = NULL;
		} else printf("ERROR (main simulator): merdou o malloc!!!\n");

		eventList->nextEvent = ptrLastNode;
		ptrThisEvent = eventList;

		// filling the event list with local and grid (???) machines, and workload jobs and tasks
		FillEmptyEventList(eventList);

		// IMPLEMENTAR AS VARIANTES PARA O FILLEMPTYEVENTLIST();
		//	ptrAuxAux = eventList;
		//	int count1 = 0;
		//	while(ptrAuxAux) {
		//		count1++;
		//		if (ptrAuxAux->eventID == 1 || ptrAuxAux->eventID == 2) {
		//			printf("eventID %d time %d machineID %d source %d status %d AT %d DT %d UP %.2f RP %.2f\n", ptrAuxAux->eventID, ptrAuxAux->time, ptrAuxAux->machineInfo.machineID,
		//					ptrAuxAux->machineInfo.source, ptrAuxAux->machineInfo.status, ptrAuxAux->machineInfo.arrivalTime, ptrAuxAux->machineInfo.departureTime,
		//					ptrAuxAux->machineInfo.usagePrice, ptrAuxAux->machineInfo.reservationPrice);
		//		}
		//		ptrAuxAux = ptrAuxAux->nextEvent;
		//	}
		//	printf("a lista tem %d eventos\n", count1);
		//	printf("\n");

		// starting a machine list
		if ( (machineList = malloc(sizeof(machine))) ) {
			machineList->machineID = 0;		// 0 means code for an empty machine list
			machineList->nextMachine = NULL;
		}
		else {
			printf("ERROR (main simulator): merdou o malloc!!!\n");
		}

		// starting a task list
		if ( (taskList = malloc(sizeof(task))) ) {
			taskList->taskID = 0;			// 0 means code for an empty task list
			taskList->nextTask = NULL;
		}
		else {
			printf("ERROR (main simulator): merdou o malloc!!!\n");
		}

		// starting an ordered task list
		if ( (orderedTaskList = malloc(sizeof(task))) ) {
			orderedTaskList->taskID = 0;			// 0 means code for an empty task list
			orderedTaskList->nextTask = NULL;
		}
		else {
			printf("ERROR (main simulator): merdou o malloc!!!\n");
		}

		// starting a job list
		if ( (jobList = malloc(sizeof(job))) ) {
			jobList->jobID = 0;			// 0 means code for an empty job list
			jobList->nextJob = NULL;
		}
		else {
			printf("ERROR (main simulator): merdou o malloc!!!\n");
		}

		// starting an account list to register the execution of tasks
		if ( (taskAccountInfoList = malloc(sizeof(taskAccountInfo))) ) {
			taskAccountInfoList->taskAccountID = 0;			// 0 means code for an empty task list
			taskAccountInfoList->status = ACCOUNTUNFINNISHED;
			taskAccountInfoList->nextTaskAccountInfo = NULL;
		}
		else {
			printf("ERROR (main simulator): merdou o malloc!!!\n");
		}

		// starting an account list to register the execution of jobs
		if ( (jobAccountInfoList = malloc(sizeof(jobAccountInfo))) ) {
			jobAccountInfoList->jobAccountID = 0;			// 0 means code for an empty task list
			jobAccountInfoList->nextJobAccountInfo = NULL;
		}
		else {
			printf("ERROR (main simulator): merdou o malloc!!!\n");
		}

		// starting an account list to register the donations to the grid
		if ( (gridInfoList = malloc(sizeof(gridAccountInfo))) ) {
			gridInfoList->gridAccountID = 0;			// 0 means code for an empty task list
			gridInfoList->nextGridAccountInfo = NULL;
		}
		else {
			printf("ERROR (main simulator): merdou o malloc!!!\n");
		}

		// starting a balance account list
		if ( (balanceAccountInfoList = malloc(sizeof(balanceAccountInfo))) ) {
			balanceAccountInfoList->balanceAccountID = 0;
			balanceAccountInfoList->time = 0;
			balanceAccountInfoList->value = 0;
			balanceAccountInfoList->nextBalanceAccountInfo = NULL;
		}
		else {
			printf("ERROR (main simulator): merdou o malloc!!!\n");
		}

		// starting a schedule list
		if ( (scheduleList = malloc(sizeof(schedule))) ) {
			scheduleList->scheduleID = 0;
			scheduleList->nextSchedule = NULL;
		}
		else {
			printf("ERROR (main simulator): merdou o malloc!!!\n");
		}

		ptrLastBalance = balanceAccountInfoList;
		ptrAuxList = eventList;
		long int lastEventNumber = -1;
		unsigned long int lastEventTime = 0;
		unsigned long int numberOfArrivals = 0, numberOfDepartures = 0;
		//	int count = 0;
		while(ptrAuxList) {

//			if(ptrAuxList->time > (simulationTime + 1)) break;
			ptrThisEvent = ptrAuxList;

//			printf("event# %d eventID %d ", ptrAuxList->eventNumber, ptrAuxList->eventID);
			printf("event# %ld ", ptrAuxList->eventNumber);


			switch (ptrAuxList->eventID) {
			case 0:
				EventHandler(ptrAuxList, &eventList, &machineList, &taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						&balanceAccountInfoList, &orderedTaskList, scheduleList);
				break;
			case 1:				// Machine Arrival
				EventHandler(ptrAuxList, &eventList, &machineList, &taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						&balanceAccountInfoList, &orderedTaskList, scheduleList);
				break;
			case 2:				// Machine Departure
				EventHandler(ptrAuxList, &eventList, &machineList, &taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						&balanceAccountInfoList, &orderedTaskList, scheduleList);
				break;
			case 3:				// Grid Donating
				EventHandler(ptrAuxList, &eventList, &machineList, &taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						&balanceAccountInfoList, &orderedTaskList, scheduleList);
				break;
			case 4:				// Grid Preempted
				EventHandler(ptrAuxList, &eventList, &machineList, &taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						&balanceAccountInfoList, &orderedTaskList, scheduleList);
				break;
			case 5:				// Task Arrival
				EventHandler(ptrAuxList, &eventList, &machineList, &taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						&balanceAccountInfoList, &orderedTaskList, scheduleList);
				break;
			case 6:				// Task Schedule
				EventHandler(ptrAuxList, &eventList, &machineList, &taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						&balanceAccountInfoList, &orderedTaskList, scheduleList);
				break;
			case 7:				// Task Preempted
				EventHandler(ptrAuxList, &eventList, &machineList, &taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						&balanceAccountInfoList, &orderedTaskList, scheduleList);
				break;
			case 8:				// Task Finnished
				EventHandler(ptrAuxList, &eventList, &machineList, &taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						&balanceAccountInfoList, &orderedTaskList, scheduleList);
				break;
			case 9:				// Job Arrival
				EventHandler(ptrAuxList, &eventList, &machineList, &taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						&balanceAccountInfoList, &orderedTaskList, scheduleList);
				break;
			case 10:			// Job Started
				EventHandler(ptrAuxList, &eventList, &machineList, &taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						&balanceAccountInfoList, &orderedTaskList, scheduleList);
				break;
			case 11:			// Job Finnished
				EventHandler(ptrAuxList, &eventList, &machineList, &taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						&balanceAccountInfoList, &orderedTaskList, scheduleList);
				break;
			case 12:			// Allocation Planning
				EventHandler(ptrAuxList, &eventList, &machineList, &taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						&balanceAccountInfoList, &orderedTaskList, scheduleList);
				break;
			case 13:			// Simulation Finnished
				EventHandler(ptrAuxList, &eventList, &machineList, &taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						&balanceAccountInfoList, &orderedTaskList, scheduleList);
				break;
			default:
				printf("unkwonk event!!!\n");
				break;
			}

			// ##### INVARIANTES PARA A LISTA DE EVENTOS #####
			totalNumberOfEvents++;

			// testar se o atual eh o anterior - 1
			if ( (ptrAuxList->eventNumber - 1) != lastEventNumber ) {
				//			printf("\n");
				printf("(INVARIANTES) NUMERACAO DOS EVENTOS ESTA ERRADA!!!\n");
				break;
			}
			lastEventNumber = ptrAuxList->eventNumber;

			// testas se o tempo estah crescente
			if (ptrAuxList->time < lastEventTime) {
				//			printf("\n");
				printf("(INVARIANTES) EVENTOS NAO ESTAO EM ORDEM DECRESCENTE!!!\n");
				break;
			}
			lastEventTime = ptrAuxList->time;

			// testar se o numero de chegadas de maquinas eh igual ao de partida
			if (ptrAuxList->eventID == MACHARRIVAL) numberOfArrivals++;
			if (ptrAuxList->eventID == MACHDEPARTURE) numberOfDepartures++;

			// atualizar o ponteiro para lista de eventos e remover o noh anterior

			event *ptrLastEvent;
			ptrLastEvent = ptrAuxList;
			ptrAuxList = ptrAuxList->nextEvent; // passa pro proximo evento

			if ( ptrAuxList != NULL && (ptrAuxList->time > ptrLastEvent->time) ) {

				event *ptrAuxEvent;
				ptrAuxEvent = eventList;
				eventList = ptrAuxList;
				while(ptrAuxEvent != ptrLastEvent) {
					event *ptrRemove;
					ptrRemove = ptrAuxEvent;
					ptrAuxEvent = ptrAuxEvent->nextEvent;
					free(ptrRemove);
					ptrRemove = NULL;
				}
				if (ptrAuxEvent == ptrLastEvent) {
					free(ptrLastEvent);
					ptrLastEvent = NULL;
					ptrAuxEvent = NULL;
				}

			}

		} // end of while(ptrAuxList)
		//	printf("\n");

		// testando a numeracao de eventos
		if ( totalNumberOfEvents != (lastEventNumber+1) ) {
			//		printf("\n");
			printf("(INVARIANTES) NUMERO ERRADO DE EVENTOS!!!\n");
			//		printf("\n");
		}

		// testando o numero de chegadas e partidas de maquinas
		if (numberOfArrivals != numberOfDepartures) {
			//		printf("\n");
			printf("(INVARIANTES) NUMERO DE CHEGADAS DIFERENTE DO NUMERO DE SAIDAS!!!\n");
		}
		// ##### FIM DAS INVARIANTES PARA A LISTA DE EVENTOS #####


		// CIRAR UM TESTE DEPOIS PARA SABER SE TODAS AS TASKS FORAM EXECUTADAS OU NAO

//		FILE *ptrFileJobAccountInfo;
//		ptrFileJobAccountInfo = fopen("jobaccountinfo.txt", "a+");
//		jobAccountInfo *ptrAuxJobAccountInfo;
//		ptrAuxJobAccountInfo = jobAccountInfoList;
//		count = 0;
//		while(ptrAuxJobAccountInfo) {
//			count++;
//			fprintf(ptrFileJobAccountInfo, "jobAccountID %d jobID %d ST %ld FT %ld\n", ptrAuxJobAccountInfo->jobAccountID, ptrAuxJobAccountInfo->jobID, ptrAuxJobAccountInfo->startTime, ptrAuxJobAccountInfo->finnishTime);
//			ptrAuxJobAccountInfo = ptrAuxJobAccountInfo->nextJobAccountInfo;
//		}
//		//	printf("a lista de jobs tem %d registros\n", count);
//		//	printf("\n");
//		fclose(ptrFileJobAccountInfo);

//		FILE *ptrFileJobList;
//		ptrFileJobList = fopen("joblist.txt", "a+");
//		job *ptrAuxJob;
//		ptrAuxJob = jobList;
//		count = 0;
//		while(ptrAuxJob) {
//			count++;
//			totalCost += ptrAuxJob->cost;
//			totalUtility += ptrAuxJob->utility;
//			totalProfit += (ptrAuxJob->utility - ptrAuxJob->cost);
//			fprintf(ptrFileJobList, "jobID %d jobSize %d AR %ld FT %ld LT %d DL %ld MU %ld Utility %ld Cost %.2f Profit %.2f\n", ptrAuxJob->jobID, ptrAuxJob->jobSize, ptrAuxJob->arrivalTime, ptrAuxJob->finnishTime, ptrAuxJob->longestTask,
//					ptrAuxJob->deadline, ptrAuxJob->maxUtility, ptrAuxJob->utility, ptrAuxJob->cost, (ptrAuxJob->utility - ptrAuxJob->cost));
//			ptrAuxJob = ptrAuxJob->nextJob;
//		}
//		//	printf("\n");
//		fclose(ptrFileJobList);

//		FILE *ptrFileTaskList;
//		ptrFileTaskList = fopen("tasklist.txt", "a+");
//		task *ptrAuxTask;
//		ptrAuxTask = taskList;
//		count = 0;
//		while(ptrAuxTask) {
//			count++;
//			fprintf(ptrFileTaskList, "taskID %d jobID %d jobSize %d AR %ld RT %d status %d submissions %d\n", ptrAuxTask->taskID, ptrAuxTask->jobID, ptrAuxTask->jobSize,
//					ptrAuxTask->arrivalTime, ptrAuxTask->runtime, ptrAuxTask->status, ptrAuxTask->numberOfSubmissions);
//			ptrAuxTask = ptrAuxTask->nextTask;
//		}
//		//	printf("\n");

//		task *ptrAuxOrderedTask;
//		ptrAuxOrderedTask = orderedTaskList;
//		count = 0;
//		while(ptrAuxOrderedTask) {
//			count++;
//			fprintf(ptrFileTaskList, "taskID %d jobID %d jobSize %d AR %ld RT %d status %d submissions %d\n", ptrAuxOrderedTask->taskID, ptrAuxOrderedTask->jobID, ptrAuxOrderedTask->jobSize,
//					ptrAuxOrderedTask->arrivalTime, ptrAuxOrderedTask->runtime, ptrAuxOrderedTask->status, ptrAuxOrderedTask->numberOfSubmissions);
//			ptrAuxOrderedTask = ptrAuxOrderedTask->nextTask;
//		}
//		//	printf("\n");
//		fclose(ptrFileTaskList);

//		FILE *ptrFileScheduleList;
//		ptrFileScheduleList = fopen("schedulelist.txt", "a+");
//		schedule *ptrAuxSchedule;
//		ptrAuxSchedule = scheduleList;
//		while(ptrAuxSchedule) {
//			fprintf(ptrFileScheduleList, "scheduleID %d scheduleTime %d machineID %d source %d taskID %d jobID %d RT %d\n", ptrAuxSchedule->scheduleID, ptrAuxSchedule->scheduleTime, ptrAuxSchedule->machineID,
//					ptrAuxSchedule->source, ptrAuxSchedule->taskID, ptrAuxSchedule->jobID, ptrAuxSchedule->runtime);
//			ptrAuxSchedule = ptrAuxSchedule->nextSchedule;
//		}
//		fclose(ptrFileScheduleList);

//		// ##### INVARIANTES PARA A LISTA DE EVENTOS #####
//		event *ptrAuxEventList;
//		ptrAuxEventList = eventList;
//		int lastEventNumber = -1;
//		unsigned int lastEventTime = 0;
//		unsigned short int numberOfArrivals = 0, numberOfDepartures = 0;
//		while(ptrAuxEventList) {
//			totalNumberOfEvents++;
//
//			// testar se o atual eh o anterior - 1
//			if ( (ptrAuxEventList->eventNumber - 1) != lastEventNumber ) {
//				//			printf("\n");
//				printf("(INVARIANTES) NUMERACAO DOS EVENTOS ESTA ERRADA!!!\n");
//				break;
//			}
//			lastEventNumber = ptrAuxEventList->eventNumber;
//
//			// testas se o tempo estah crescente
//			if (ptrAuxEventList->time < lastEventTime) {
//				//			printf("\n");
//				printf("(INVARIANTES) EVENTOS NAO ESTAO EM ORDEM DECRESCENTE!!!\n");
//				break;
//			}
//
//			// testar se o numero de chegadas de maquinas eh igual ao de partida
//			if (ptrAuxEventList->eventID == MACHARRIVAL) numberOfArrivals++;
//			if (ptrAuxEventList->eventID == MACHDEPARTURE) numberOfDepartures++;
//
//			ptrAuxEventList = ptrAuxEventList->nextEvent;
//
//		}
//
//		// testando a numeracao de eventos
//		if ( totalNumberOfEvents != (ptrLastNode->eventNumber+1) || lastEventNumber != ptrLastNode->eventNumber ) {
//			//		printf("\n");
//			printf("(INVARIANTES) NUMERO ERRADO DE EVENTOS!!!\n");
//			//		printf("\n");
//		}
//
//		// testando o numero de chegadas e partidas de maquinas
//		if (numberOfArrivals != numberOfDepartures) {
//			//		printf("\n");
//			printf("(INVARIANTES) NUMERO DE CHEGADAS DIFERENTE DO NUMERO DE SAIDAS!!!\n");
//		}
//		// ##### FIM DAS INVARIANTES PARA A LISTA DE EVENTOS #####

		// ##### INVARIANTES PARA A LISTA DE MAQUINAS (CHEGADAS E PARTIDAS) #####
		ptrAuxMachine = machineList;
		if (ptrAuxMachine != NULL) {

			printf("\n");
			printf("(INVARIANTES) LISTA NAO VAZIA DE MAQUINAS!!!\n");

			count = 0;
			while(ptrAuxMachine) {
				count++;
				printf("machineID %d source %d status %d AT %ld DT %ld UP %.2f RP %.2f\n", ptrAuxMachine->machineID,
						ptrAuxMachine->source, ptrAuxMachine->status, ptrAuxMachine->arrivalTime, ptrAuxMachine->departureTime,
						ptrAuxMachine->usagePrice, ptrAuxMachine->reservationPrice);
				ptrAuxMachine = ptrAuxMachine->nextMachine;
			}
			printf("a lista tem %ld maquinas\n", count);
			printf("\n");

		}
		// ##### FIM INVARIANTES PARA A LISTA DE MAQUINAS (CHEGADAS E PARTIDAS) #####

//		// ##### INVARIANTES PARA A LISTA DE EXECUCAO DAS TASKS #####
//		FILE *ptrFileTaskAccountInfo;
//		ptrFileTaskAccountInfo = fopen("taskaccountinfo.txt", "a+");
//		taskAccountInfo *ptrAuxTaskAccountInfo;
//		ptrAuxTaskAccountInfo = taskAccountInfoList;
//		unsigned long int totalExecutionTime = 0;
//		while(ptrAuxTaskAccountInfo) {
//
//			if (ptrAuxTaskAccountInfo->source == 1) {
//				totalExecutionTime += (ptrAuxTaskAccountInfo->finnishTime-ptrAuxTaskAccountInfo->startTime);
//			}
//			if ( ptrAuxTaskAccountInfo->runtime < (ptrAuxTaskAccountInfo->finnishTime-ptrAuxTaskAccountInfo->startTime) ) {
//				//			printf("\n");
//				printf("(INVARIANTES) TEMPO DE EXECUCAO MAIOR DO QUE O RUNTIME!!!\n");
//			}
//
//			if ( (ptrAuxTaskAccountInfo->finnishTime-ptrAuxTaskAccountInfo->startTime) < 0 ) {
//				//			printf("\n");
//				printf("(INVARIANTES) FINNISHTIME MENOR DO QUE STARTTIME!!!\n");
//			}
//
//					fprintf(ptrFileTaskAccountInfo,"taskAccountID %d machineID %d source %d taskID %d jobID %d RT %d ET %ld ST %ld FT %ld Status %d Cost %.2f\n", ptrAuxTaskAccountInfo->taskAccountID,
//							ptrAuxTaskAccountInfo->machineID, ptrAuxTaskAccountInfo->source, ptrAuxTaskAccountInfo->taskID, ptrAuxTaskAccountInfo->jobID,
//							ptrAuxTaskAccountInfo->runtime, (ptrAuxTaskAccountInfo->finnishTime-ptrAuxTaskAccountInfo->startTime),
//							ptrAuxTaskAccountInfo->startTime, ptrAuxTaskAccountInfo->finnishTime, ptrAuxTaskAccountInfo->status, ptrAuxTaskAccountInfo->cost);
//
//			ptrAuxTaskAccountInfo = ptrAuxTaskAccountInfo->nextTaskAccountInfo;
//		}
//		//	printf("a lista tem %d tasks\n", count);
//		//	printf("\n");
//		fclose(ptrFileTaskAccountInfo);
//		// ##### FIM INVARIANTES PARA A LISTA DE EXECUCAO DAS TASKS #####

//		// ##### INVARIANTES PARA O CONSUMO DA GRADE E PARA BALANCO DE CREDITOS #####
//		FILE *ptrFileGridAccountInfo;
//		ptrFileGridAccountInfo = fopen("gridaccountinfo.txt", "a+");
//		gridAccountInfo *ptrAuxGridInfo;
//		ptrAuxGridInfo = gridInfoList;
//		unsigned long int credit = 0;
//		while(ptrAuxGridInfo) {
//			credit += (ptrAuxGridInfo->finnishTime - ptrAuxGridInfo->startTime);
//					fprintf(ptrFileGridAccountInfo, "gridAccountID %d machineID %d source %d ST %ld FT %ld\n", ptrAuxGridInfo->gridAccountID,
//							ptrAuxGridInfo->machineID, ptrAuxGridInfo->source, ptrAuxGridInfo->startTime,
//							ptrAuxGridInfo->finnishTime);
//			ptrAuxGridInfo = ptrAuxGridInfo->nextGridAccountInfo;
//		}
//		//	printf("a lista tem %d balances, e o total de creditos doados e %d\n", count, credit);
//		//	printf("\n");
//		fclose(ptrFileGridAccountInfo);
//
//		FILE *ptrFileBalanceAccountInfo;
//		ptrFileBalanceAccountInfo = fopen("balanceaccountinfo.txt", "a+");
//		balanceAccountInfo *ptrAuxBalanceAccountInfo;
//		ptrAuxBalanceAccountInfo = balanceAccountInfoList;
//		unsigned long int consumed = 0, lastValue = 0;
//		while(ptrAuxBalanceAccountInfo->nextBalanceAccountInfo != NULL) {
//
//			if (ptrAuxBalanceAccountInfo->consumed > lastValue) {
//				consumed += lastValue;
//			}
//			else {
//				consumed += ptrAuxBalanceAccountInfo->consumed;
//			}
//			lastValue = ptrAuxBalanceAccountInfo->value;
//
//			fprintf(ptrFileBalanceAccountInfo, "balanceAccountID %ld time %ld consumed %ld value %ld\n", ptrAuxBalanceAccountInfo->balanceAccountID,
//					ptrAuxBalanceAccountInfo->time, ptrAuxBalanceAccountInfo->consumed, ptrAuxBalanceAccountInfo->value);
//
//			ptrAuxBalanceAccountInfo = ptrAuxBalanceAccountInfo->nextBalanceAccountInfo;
//
//		}
//		consumed += ptrAuxBalanceAccountInfo->consumed;
//		fprintf(ptrFileBalanceAccountInfo, "balanceAccountID %ld time %ld consumed %ld value %ld\n", ptrAuxBalanceAccountInfo->balanceAccountID, ptrAuxBalanceAccountInfo->time,
//				ptrAuxBalanceAccountInfo->consumed, ptrAuxBalanceAccountInfo->value);
//		//	printf("a lista de balances tem %d registros\n", count);
//		//	printf("\n");
//		fclose(ptrFileBalanceAccountInfo);
//
//		if ( credit != (ptrAuxBalanceAccountInfo->value + consumed) ) {
//			//		printf("\n");
//			printf("(INVARIANTES) BALANCE DO GRID ERRADO!!!\n");
//			printf("credit %ld (balance+consumed) %ld consumed %ld totalET %ld\n", credit, (ptrAuxBalanceAccountInfo->value + consumed), consumed, totalExecutionTime);
//		}
//		//	printf("doado %d consumido %d balance %d\n", credit, consumed, ptrAuxBalanceAccountInfo->value);
//		// ##### FIM INVARIANTES PARA O CONSUMO DA GRADE E PARA BALANCO DE CREDITOS #####

	} else printf("ERROR (Main Simulator): invalid number of input parameters!!!\n");

	time(&newEnd);
//   second = CPU_TIME;

//	printf("Opt Flag: %d Grid QoS-Factor: %.2f Simulation Period (days): %d Machines ([InH, Res, OnD]): [%d, %d, %d] Seed: %d UF: %d Time elapsed (seconds): %ld\n",
//			optFlag, gridQoSFactor, (int)(simulationTime/1440), numberOfLocalMachines, numberOfReservedMachines, numberOfOnDemandMachines,
//			simSeed, utilityFunction,(clock() - start) / CLOCKS_PER_SEC);

	printf("SimMode: %d SimTime (days): %d JobSize: %d GridQoS: %.2f OnDPFactor: %.2f IhFactor: %.2f Machines ([InH, Res, OnD]): [%d, %d, %d] TaskAvg: %d "
			"GridAvg: %d UF: %d Seed: %d #Events: %ld ExecTime (seconds): %ld AggUtility: %.2f AggCost %.2f AggProfit %.2f\n", optFlag, (int)(simulationTime/1440), jobSize, gridQoSFactor, ondemandPriceFactor,
			inhouseFactor, numberOfLocalMachines, numberOfReservedMachines, numberOfOnDemandMachines, taskAvgTime, gridAvgUptime, utilityFunction, simSeed,
			totalNumberOfEvents, (unsigned long int)(newEnd - newStart), aggregatedUtility, aggregatedCost, aggregatedProfit);

//	event teste;
//	printf("tamanho das estruturas: %d\n", sizeof(teste));

	return EXIT_SUCCESS;
}

