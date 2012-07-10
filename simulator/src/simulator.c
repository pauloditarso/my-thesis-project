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
	task *taskList;
	job *jobList;
	taskAccountInfo *taskAccountInfoList;
	gridAccountInfo *gridInfoList;
	jobAccountInfo *jobAccountInfoList;
	balanceAccountInfo *balanceAccountInfoList;

	unsigned int count = 0;
	gridMachinesID = 0;

	if (argc == 8) {

		optFlag = (atoi(argv[1]));
		gridQoSFactor = (atof(argv[2]));
		simulationTime = (atoi(argv[3]) * 3600); // simulationTime is a global variable (seconds) based on the input parameter (in hours)
		numberMachinesP1 = (atoi(argv[4]));
		numberMachinesP3 = atoi(argv[5]);
		simSeed = (atoi(argv[6]));
		utilityFunction = (atoi(argv[7]));

		srand(simSeed);

		// starting a new event list
		if ( (eventList = malloc(sizeof(event))) ) {
			// start simulation event
			eventList->eventNumber = 0; eventList->eventID = 0; eventList->time = 0; eventList->flag = -1; eventList->nextEvent = NULL;
		} else printf("ERROR: merdou o malloc!!!\n");

		// finish simulation event
		if ( (ptrLastNode = malloc(sizeof(event))) ) {
			ptrLastNode->eventNumber = 1; ptrLastNode->eventID = SIMFINNISHED; ptrLastNode->time = (simulationTime+1); ptrLastNode->flag = -1; ptrLastNode->nextEvent = NULL;
		} else printf("ERROR: merdou o malloc!!!\n");

		eventList->nextEvent = ptrLastNode;

		// filling the event list with local and grid (???) machines, and workload jobs and tasks
		FillEmptyEventList(eventList);

		// IMPLEMENTAR AS VARIANTES PARA O FILLEMPTYEVENTLIST();
		//	ptrAuxAux = eventList;
		//	int count1 = 0;
		//	while(ptrAuxAux) {
		//		count1++;
		//		if (ptrAuxAux->eventID == 1 || ptrAuxAux->eventID == 2) {
		//			printf("eventID %d time %d machineID %d source %d status %d AT %d DT %d UP %f RP %f\n", ptrAuxAux->eventID, ptrAuxAux->time, ptrAuxAux->machineInfo.machineID,
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
			printf("ERROR: merdou o malloc!!!\n");
		}

		// starting a task list
		if ( (taskList = malloc(sizeof(task))) ) {
			taskList->taskID = 0;			// 0 means code for an empty task list
			taskList->nextTask = NULL;
		}
		else {
			printf("ERROR: merdou o malloc!!!\n");
		}

		// starting a job list
		if ( (jobList = malloc(sizeof(job))) ) {
			jobList->jobID = 0;			// 0 means code for an empty job list
			jobList->nextJob = NULL;
		}
		else {
			printf("ERROR: merdou o malloc!!!\n");
		}

		// starting an account list to register the execution of tasks
		if ( (taskAccountInfoList = malloc(sizeof(taskAccountInfo))) ) {
			taskAccountInfoList->taskAccountID = 0;			// 0 means code for an empty task list
			taskAccountInfoList->status = ACCOUNTUNFINNISHED;
			taskAccountInfoList->nextTaskAccountInfo = NULL;
		}
		else {
			printf("ERROR: merdou o malloc!!!\n");
		}

		// starting an account list to register the execution of jobs
		if ( (jobAccountInfoList = malloc(sizeof(jobAccountInfo))) ) {
			jobAccountInfoList->jobAccountID = 0;			// 0 means code for an empty task list
			jobAccountInfoList->nextJobAccountInfo = NULL;
		}
		else {
			printf("ERROR: merdou o malloc!!!\n");
		}

		// starting an account list to register the donations to the grid
		if ( (gridInfoList = malloc(sizeof(gridAccountInfo))) ) {
			gridInfoList->gridAccountID = 0;			// 0 means code for an empty task list
			gridInfoList->nextGridAccountInfo = NULL;
		}
		else {
			printf("ERROR: merdou o malloc!!!\n");
		}

		// starting a balance account list
		if ( (balanceAccountInfoList = malloc(sizeof(balanceAccountInfo))) ) {
			balanceAccountInfoList->balanceAccountID = 0;
			balanceAccountInfoList->time = 0;
			balanceAccountInfoList->value = 0;
			balanceAccountInfoList->nextBalanceAccountInfo = NULL;
		}
		else {
			printf("ERROR: merdou o malloc!!!\n");
		}

		ptrAuxList = eventList;
		//	int count = 0;
		while(ptrAuxList) {
			//		count++;
			printf("event# %d ", ptrAuxList->eventNumber);

			switch (ptrAuxList->eventID) {
			case 0:
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						balanceAccountInfoList);
				break;
			case 1:				// Machine Arrival
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						balanceAccountInfoList);
				break;
			case 2:				// Machine Departure
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						balanceAccountInfoList);
				break;
			case 3:				// Grid Donating
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						balanceAccountInfoList);
				break;
			case 4:				// Grid Preempted
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						balanceAccountInfoList);
				break;
			case 5:				// Task Arrival
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						balanceAccountInfoList);
				break;
			case 6:				// Task Schedule
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						balanceAccountInfoList);
				break;
			case 7:				// Task Preempted
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						balanceAccountInfoList);
				break;
			case 8:				// Task Finnished
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						balanceAccountInfoList);
				break;
			case 9:				// Job Arrival
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						balanceAccountInfoList);
				break;
			case 10:			// Job Started
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						balanceAccountInfoList);
				break;
			case 11:			// Job Finnished
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						balanceAccountInfoList);
				break;
			case 12:			// Allocation Planning
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						balanceAccountInfoList);
				break;
			case 13:			// Simulation Finnished
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						balanceAccountInfoList);
				break;
			default:
				printf("unkwonk event!!!\n");
				break;
			}

			ptrAuxList = ptrAuxList->nextEvent;
		}
		//	printf("\n");

		// CIRAR UM TESTE DEPOIS PARA SABER SE TODAS AS TASKS FORAM EXECUTADAS OU NAO

		//	jobAccountInfo *ptrAuxJobAccountInfo;
		//	ptrAuxJobAccountInfo = jobAccountInfoList;
		//	count = 0;
		//	while(ptrAuxJobAccountInfo) {
		//		count++;
		//		printf("jobAccountID %d jobID %d ST %d FT %d\n", ptrAuxJobAccountInfo->jobAccountID, ptrAuxJobAccountInfo->jobID, ptrAuxJobAccountInfo->startTime, ptrAuxJobAccountInfo->finnishTime);
		//		ptrAuxJobAccountInfo = ptrAuxJobAccountInfo->nextJobAccountInfo;
		//	}
		//	printf("a lista de jobs tem %d registros\n", count);
		//	printf("\n");
		//
		//	job *ptrAuxJob;
		//	ptrAuxJob = jobList;
		//	count = 0;
		//	while(ptrAuxJob) {
		//		count++;
		//		printf("jobID %d AR %d FT %d LT %d DL %d\n", ptrAuxJob->jobID, ptrAuxJob->arrivalTime, ptrAuxJob->finnishTime, ptrAuxJob->longestTask,
		//				ptrAuxJob->deadline);
		//		ptrAuxJob = ptrAuxJob->nextJob;
		//	}
		//	printf("a lista de jobs tem %d registros\n", count);
		//	printf("\n");
		//
		//	task *ptrAuxTask;
		//	ptrAuxTask = taskList;
		//	count = 0;
		//	while(ptrAuxTask) {
		//		count++;
		//		printf("taskID %d jobID %d jobSize %d AR %d RT %d status %d UF %f\n", ptrAuxTask->taskID, ptrAuxTask->jobID, ptrAuxTask->jobSize,
		//				ptrAuxTask->arrivalTime, ptrAuxTask->runtime, ptrAuxTask->status, ptrAuxTask->utilityFunction);
		//		ptrAuxTask = ptrAuxTask->nextTask;
		//	}
		//	printf("a lista de tasks tem %d registros\n", count);
		//	printf("\n");

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
				printf("machineID %d source %d status %d AT %d DT %d UP %f RP %f\n", ptrAuxMachine->machineID,
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
		unsigned int consumed = 0;
		while(ptrAuxTaskAccountInfo) {

			if (ptrAuxTaskAccountInfo->source == 1) {
				consumed += (ptrAuxTaskAccountInfo->finnishTime-ptrAuxTaskAccountInfo->startTime);
			}
			if ( ptrAuxTaskAccountInfo->runtime < (ptrAuxTaskAccountInfo->finnishTime-ptrAuxTaskAccountInfo->startTime) ) {
				//			printf("\n");
				printf("(INVARIANTES) TEMPO DE EXECUCAO MAIOR DO QUE O RUNTIME!!!\n");
			}

			if ( (ptrAuxTaskAccountInfo->finnishTime-ptrAuxTaskAccountInfo->startTime) < 0 ) {
				//			printf("\n");
				printf("(INVARIANTES) FINNISHTIME MENOR DO QUE STARTTIME!!!\n");
			}

			//		printf("taskAccountID %d machineID %d source %d taskID %d jobID %d RT %d ET %d ST %d FT %d\n", ptrAuxTaskAccountInfo->taskAccountID,
			//				ptrAuxTaskAccountInfo->machineID, ptrAuxTaskAccountInfo->source, ptrAuxTaskAccountInfo->taskID, ptrAuxTaskAccountInfo->jobID,
			//				ptrAuxTaskAccountInfo->runtime, (ptrAuxTaskAccountInfo->finnishTime-ptrAuxTaskAccountInfo->startTime),
			//				ptrAuxTaskAccountInfo->startTime, ptrAuxTaskAccountInfo->finnishTime);

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
			//		printf("gridAccountID %d machineID %d source %d ST %d FT %d\n", ptrAuxGridInfo->gridAccountID,
			//				ptrAuxGridInfo->machineID, ptrAuxGridInfo->source, ptrAuxGridInfo->startTime,
			//				ptrAuxGridInfo->finnishTime);
			ptrAuxGridInfo = ptrAuxGridInfo->nextGridAccountInfo;
		}
		//	printf("a lista tem %d balances, e o total de creditos doados e %d\n", count, credit);
		//	printf("\n");

		balanceAccountInfo *ptrAuxBalanceAccountInfo;
		ptrAuxBalanceAccountInfo = balanceAccountInfoList;
		while(ptrAuxBalanceAccountInfo->nextBalanceAccountInfo != NULL) {
			//		printf("balanceAccountID %d time %d value %d\n", ptrAuxBalanceAccountInfo->balanceAccountID, ptrAuxBalanceAccountInfo->time, ptrAuxBalanceAccountInfo->value);
			ptrAuxBalanceAccountInfo = ptrAuxBalanceAccountInfo->nextBalanceAccountInfo;
		}
		//	printf("a lista de balances tem %d registros\n", count);
		//	printf("\n");

		if ( credit != (ptrAuxBalanceAccountInfo->value + consumed) ) {
			//		printf("\n");
			printf("(INVARIANTES) BALANCE DO GRID ERRADO!!!\n");
			printf("credit %d (balance-consumed) %d\n", credit, (ptrAuxBalanceAccountInfo->value + consumed));
		}
		//	printf("doado %d consumido %d balance %d\n", credit, consumed, ptrAuxBalanceAccountInfo->value);
		// ##### FIM INVARIANTES PARA O CONSUMO DA GRADE E PARA BALANCO DE CREDITOS #####

	} else printf("ERROR (Main Simulator): input error!!!\n");

	return EXIT_SUCCESS;
}
