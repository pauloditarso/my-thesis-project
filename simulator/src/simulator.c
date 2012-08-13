/*
 ============================================================================
 Name        : simulator.c
 Author      : Paulo Ditarso
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "simulation.h"

int main(int argc, char *argv[]) {

	event *eventList, *ptrAuxList, *ptrLastNode;
	machine *machineList, *ptrAuxMachine;
	task *taskList, *orderedTaskList;
	job *jobList;
	taskAccountInfo *taskAccountInfoList;
	gridAccountInfo *gridInfoList;
	jobAccountInfo *jobAccountInfoList;
	balanceAccountInfo *balanceAccountInfoList;
	schedule *scheduleList;

	unsigned int count = 0;
	gridMachinesID = 0; scheduleID = 0;

	if (argc == 9) {

		optFlag = (atoi(argv[1]));
		gridQoSFactor = (atof(argv[2]));
		simulationTime = (atoi(argv[3]) * 60); // simulationTime is a global variable (minutes) based on the input parameter (in hours)
		numberOfLocalMachines = (atoi(argv[4]));
		numberOfReservedMachines = atoi(argv[5]);
		numberOfOnDemandMachines = atoi(argv[6]);
		simSeed = (atoi(argv[7]));
		utilityFunction = (atoi(argv[8]));

		ondemandUsagePrice = 0.92; reservedUsagePrice = 0.22; reservationPrice = 2984.00;
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

		ptrAuxList = eventList;
		//	int count = 0;
		while(ptrAuxList) {

//			if(ptrAuxList->time > (simulationTime + 1)) break;

			printf("event# %d ", ptrAuxList->eventNumber);

			switch (ptrAuxList->eventID) {
			case 0:
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						balanceAccountInfoList, &orderedTaskList, scheduleList);
				break;
			case 1:				// Machine Arrival
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						balanceAccountInfoList, &orderedTaskList, scheduleList);
				break;
			case 2:				// Machine Departure
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						balanceAccountInfoList, &orderedTaskList, scheduleList);
				break;
			case 3:				// Grid Donating
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						balanceAccountInfoList, &orderedTaskList, scheduleList);
				break;
			case 4:				// Grid Preempted
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						balanceAccountInfoList, &orderedTaskList, scheduleList);
				break;
			case 5:				// Task Arrival
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						balanceAccountInfoList, &orderedTaskList, scheduleList);
				break;
			case 6:				// Task Schedule
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						balanceAccountInfoList, &orderedTaskList, scheduleList);
				break;
			case 7:				// Task Preempted
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						balanceAccountInfoList, &orderedTaskList, scheduleList);
				break;
			case 8:				// Task Finnished
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						balanceAccountInfoList, &orderedTaskList, scheduleList);
				break;
			case 9:				// Job Arrival
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						balanceAccountInfoList, &orderedTaskList, scheduleList);
				break;
			case 10:			// Job Started
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						balanceAccountInfoList, &orderedTaskList, scheduleList);
				break;
			case 11:			// Job Finnished
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						balanceAccountInfoList, &orderedTaskList, scheduleList);
				break;
			case 12:			// Allocation Planning
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						balanceAccountInfoList, &orderedTaskList, scheduleList);
				break;
			case 13:			// Simulation Finnished
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						balanceAccountInfoList, &orderedTaskList, scheduleList);
				break;
			default:
				printf("unkwonk event!!!\n");
				break;
			}

			ptrAuxList = ptrAuxList->nextEvent;
		}
		//	printf("\n");

		// CIRAR UM TESTE DEPOIS PARA SABER SE TODAS AS TASKS FORAM EXECUTADAS OU NAO

		jobAccountInfo *ptrAuxJobAccountInfo;
		ptrAuxJobAccountInfo = jobAccountInfoList;
		count = 0;
		while(ptrAuxJobAccountInfo) {
			count++;
			printf("jobAccountID %d jobID %d ST %d FT %d\n", ptrAuxJobAccountInfo->jobAccountID, ptrAuxJobAccountInfo->jobID, ptrAuxJobAccountInfo->startTime, ptrAuxJobAccountInfo->finnishTime);
			ptrAuxJobAccountInfo = ptrAuxJobAccountInfo->nextJobAccountInfo;
		}
		//	printf("a lista de jobs tem %d registros\n", count);
		//	printf("\n");

		job *ptrAuxJob;
		ptrAuxJob = jobList;
		count = 0;
		while(ptrAuxJob) {
			count++;
			printf("jobID %d jobSize %d AR %d FT %d LT %d DL %d MU %d Utility %d Cost %.2f Profit %.2f\n", ptrAuxJob->jobID, ptrAuxJob->jobSize, ptrAuxJob->arrivalTime, ptrAuxJob->finnishTime, ptrAuxJob->longestTask,
					ptrAuxJob->deadline, ptrAuxJob->maxUtility, ptrAuxJob->utility, ptrAuxJob->cost, (ptrAuxJob->utility - ptrAuxJob->cost));
			ptrAuxJob = ptrAuxJob->nextJob;
		}
		//	printf("a lista de jobs tem %d registros\n", count);
		//	printf("\n");
		//
		task *ptrAuxTask;
		ptrAuxTask = taskList;
		count = 0;
		while(ptrAuxTask) {
			count++;
			printf("taskID %d jobID %d jobSize %d AR %d RT %d status %d submissions %d\n", ptrAuxTask->taskID, ptrAuxTask->jobID, ptrAuxTask->jobSize,
					ptrAuxTask->arrivalTime, ptrAuxTask->runtime, ptrAuxTask->status, ptrAuxTask->numberOfSubmissions);
			ptrAuxTask = ptrAuxTask->nextTask;
		}
		//	printf("a lista de tasks tem %d registros\n", count);
		//	printf("\n");

		task *ptrAuxOrderedTask;
		ptrAuxOrderedTask = orderedTaskList;
		count = 0;
		while(ptrAuxOrderedTask) {
			count++;
			printf("taskID %d jobID %d jobSize %d AR %d RT %d status %d submissions %d\n", ptrAuxOrderedTask->taskID, ptrAuxOrderedTask->jobID, ptrAuxOrderedTask->jobSize,
					ptrAuxOrderedTask->arrivalTime, ptrAuxOrderedTask->runtime, ptrAuxOrderedTask->status, ptrAuxOrderedTask->numberOfSubmissions);
			ptrAuxOrderedTask = ptrAuxOrderedTask->nextTask;
		}
		//	printf("a lista de tasks tem %d registros\n", count);
		//	printf("\n");

		schedule *ptrAuxSchedule;
		ptrAuxSchedule = scheduleList;
		while(ptrAuxSchedule) {
			printf("scheduleID %d scheduleTime %d machineID %d source %d taskID %d jobID %d RT %d\n", ptrAuxSchedule->scheduleID, ptrAuxSchedule->scheduleTime, ptrAuxSchedule->machineID,
					ptrAuxSchedule->source, ptrAuxSchedule->taskID, ptrAuxSchedule->jobID, ptrAuxSchedule->runtime);
			ptrAuxSchedule = ptrAuxSchedule->nextSchedule;
		}

		// ##### INVARIANTES PARA A LISTA DE EVENTOS #####
		event *ptrAuxEventList;
		ptrAuxEventList = eventList;
		int lastEventNumber = -1;
		unsigned int lastEventTime = 0;
		unsigned short int numberOfArrivals = 0, numberOfDepartures = 0;
		count = 0;
		while(ptrAuxEventList) {
			count++;

			// testar se o atual eh o anterior - 1
			if ( (ptrAuxEventList->eventNumber - 1) != lastEventNumber ) {
				//			printf("\n");
				printf("(INVARIANTES) NUMERACAO DOS EVENTOS ESTA ERRADA!!!\n");
				break;
			}
			lastEventNumber = ptrAuxEventList->eventNumber;

			// testas se o tempo estah crescente
			if (ptrAuxEventList->time < lastEventTime) {
				//			printf("\n");
				printf("(INVARIANTES) EVENTOS NAO ESTAO EM ORDEM DECRESCENTE!!!\n");
				break;
			}

			// testar se o numero de chegadas de maquinas eh igual ao de partida
			if (ptrAuxEventList->eventID == MACHARRIVAL) numberOfArrivals++;
			if (ptrAuxEventList->eventID == MACHDEPARTURE) numberOfDepartures++;

			ptrAuxEventList = ptrAuxEventList->nextEvent;

		}

		// testando a numeracao de eventos
		if ( count != (ptrLastNode->eventNumber+1) || lastEventNumber != ptrLastNode->eventNumber ) {
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

		// ##### INVARIANTES PARA A LISTA DE MAQUINAS (CHEGADAS E PARTIDAS) #####
		ptrAuxMachine = machineList;
		if (ptrAuxMachine != NULL) {

			printf("\n");
			printf("(INVARIANTES) LISTA NAO VAZIA DE MAQUINAS!!!\n");

			count = 0;
			while(ptrAuxMachine) {
				count++;
				printf("machineID %d source %d status %d AT %d DT %d UP %.2f RP %.2f\n", ptrAuxMachine->machineID,
						ptrAuxMachine->source, ptrAuxMachine->status, ptrAuxMachine->arrivalTime, ptrAuxMachine->departureTime,
						ptrAuxMachine->usagePrice, ptrAuxMachine->reservationPrice);
				ptrAuxMachine = ptrAuxMachine->nextMachine;
			}
			printf("a lista tem %d maquinas\n", count);
			printf("\n");

		}
		// ##### FIM INVARIANTES PARA A LISTA DE MAQUINAS (CHEGADAS E PARTIDAS) #####

		// ##### INVARIANTES PARA A LISTA DE EXECUCAO DAS TASKS #####
		taskAccountInfo *ptrAuxTaskAccountInfo;
		ptrAuxTaskAccountInfo = taskAccountInfoList;
		unsigned int totalExecutionTime = 0;
		while(ptrAuxTaskAccountInfo) {

			if (ptrAuxTaskAccountInfo->source == 1) {
				totalExecutionTime += (ptrAuxTaskAccountInfo->finnishTime-ptrAuxTaskAccountInfo->startTime);
			}
			if ( ptrAuxTaskAccountInfo->runtime < (ptrAuxTaskAccountInfo->finnishTime-ptrAuxTaskAccountInfo->startTime) ) {
				//			printf("\n");
				printf("(INVARIANTES) TEMPO DE EXECUCAO MAIOR DO QUE O RUNTIME!!!\n");
			}

			if ( (ptrAuxTaskAccountInfo->finnishTime-ptrAuxTaskAccountInfo->startTime) < 0 ) {
				//			printf("\n");
				printf("(INVARIANTES) FINNISHTIME MENOR DO QUE STARTTIME!!!\n");
			}

					printf("taskAccountID %d machineID %d source %d taskID %d jobID %d RT %d ET %d ST %d FT %d Status %d Cost %.2f\n", ptrAuxTaskAccountInfo->taskAccountID,
							ptrAuxTaskAccountInfo->machineID, ptrAuxTaskAccountInfo->source, ptrAuxTaskAccountInfo->taskID, ptrAuxTaskAccountInfo->jobID,
							ptrAuxTaskAccountInfo->runtime, (ptrAuxTaskAccountInfo->finnishTime-ptrAuxTaskAccountInfo->startTime),
							ptrAuxTaskAccountInfo->startTime, ptrAuxTaskAccountInfo->finnishTime, ptrAuxTaskAccountInfo->status, ptrAuxTaskAccountInfo->cost);

			ptrAuxTaskAccountInfo = ptrAuxTaskAccountInfo->nextTaskAccountInfo;
		}
		//	printf("a lista tem %d tasks\n", count);
		//	printf("\n");
		// ##### FIM INVARIANTES PARA A LISTA DE EXECUCAO DAS TASKS #####

		// ##### INVARIANTES PARA O CONSUMO DA GRADE E PARA BALANCO DE CREDITOS #####
		gridAccountInfo *ptrAuxGridInfo;
		ptrAuxGridInfo = gridInfoList;
		unsigned int credit = 0;
		while(ptrAuxGridInfo) {
			credit += (ptrAuxGridInfo->finnishTime - ptrAuxGridInfo->startTime);
					printf("gridAccountID %d machineID %d source %d ST %d FT %d\n", ptrAuxGridInfo->gridAccountID,
							ptrAuxGridInfo->machineID, ptrAuxGridInfo->source, ptrAuxGridInfo->startTime,
							ptrAuxGridInfo->finnishTime);
			ptrAuxGridInfo = ptrAuxGridInfo->nextGridAccountInfo;
		}
		//	printf("a lista tem %d balances, e o total de creditos doados e %d\n", count, credit);
		//	printf("\n");

		balanceAccountInfo *ptrAuxBalanceAccountInfo;
		ptrAuxBalanceAccountInfo = balanceAccountInfoList;
		unsigned int consumed = 0, lastValue = 0;
		while(ptrAuxBalanceAccountInfo->nextBalanceAccountInfo != NULL) {

			if (ptrAuxBalanceAccountInfo->consumed > lastValue) {
				consumed += lastValue;
			}
			else {
				consumed += ptrAuxBalanceAccountInfo->consumed;
			}
			lastValue = ptrAuxBalanceAccountInfo->value;

			printf("balanceAccountID %d time %d consumed %d value %d\n", ptrAuxBalanceAccountInfo->balanceAccountID,
					ptrAuxBalanceAccountInfo->time, ptrAuxBalanceAccountInfo->consumed, ptrAuxBalanceAccountInfo->value);

			ptrAuxBalanceAccountInfo = ptrAuxBalanceAccountInfo->nextBalanceAccountInfo;

		}
		consumed += ptrAuxBalanceAccountInfo->consumed;
		printf("balanceAccountID %d time %d consumed %d value %d\n", ptrAuxBalanceAccountInfo->balanceAccountID, ptrAuxBalanceAccountInfo->time,
				ptrAuxBalanceAccountInfo->consumed, ptrAuxBalanceAccountInfo->value);
		//	printf("a lista de balances tem %d registros\n", count);
		//	printf("\n");

		if ( credit != (ptrAuxBalanceAccountInfo->value + consumed) ) {
			//		printf("\n");
			printf("(INVARIANTES) BALANCE DO GRID ERRADO!!!\n");
			printf("credit %d (balance+consumed) %d consumed %d totalET %d\n", credit, (ptrAuxBalanceAccountInfo->value + consumed), consumed, totalExecutionTime);
		}
		//	printf("doado %d consumido %d balance %d\n", credit, consumed, ptrAuxBalanceAccountInfo->value);
		// ##### FIM INVARIANTES PARA O CONSUMO DA GRADE E PARA BALANCO DE CREDITOS #####

	} else printf("ERROR (Main Simulator): invalid number of input parameters!!!\n");

	return EXIT_SUCCESS;
}
