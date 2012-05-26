/*
 * eventhandler.c
 *
 *  Created on: Aug 26, 2011
 *      Author: PauloDitarso
 */

#include "simulation.h"

void EventHandler(event *ptrCurrentEvent, event **ptrPtrEventList, machine **ptrPtrMachineList, task *ptrTaskList, taskAccountInfo **ptrPtrTaskAccountInfoList,
		gridAccountInfo *ptrGridInfoList, job *ptrJobList, jobAccountInfo *ptrJobAccountInfo, balanceAccountInfo *ptrBalanceAccountInfo) {

	switch (ptrCurrentEvent->eventID) {
		case 0:
			printf("eventID %d (Simulation Started) time %d\n", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
			break;
		case 1:
			// MACHINE ARRIVAL
			MachineArrival(ptrCurrentEvent, *ptrPtrEventList, *ptrPtrMachineList, ptrTaskList);
			break;
		case 2:
			// MACHINE DEPARTURE
			MachineDeparture(ptrCurrentEvent, *ptrPtrEventList, ptrPtrMachineList);
			break;
		case 3:
			// GRID DONATING
			GridDonating(ptrCurrentEvent, *ptrPtrEventList, *ptrPtrMachineList, ptrGridInfoList);
			break;
		case 4:
			// GRID PREEMPTED
			GridPreempted(ptrCurrentEvent, *ptrPtrEventList, *ptrPtrMachineList, ptrGridInfoList, ptrBalanceAccountInfo);
			break;
		case 5:
			// TASK ARRIVAL
			TaskArrival(ptrCurrentEvent, *ptrPtrEventList, ptrTaskList, *ptrPtrMachineList);
			break;
		case 6:
			// TASK SCHEDULED
			TaskSchedule(ptrCurrentEvent, *ptrPtrEventList, *ptrPtrMachineList, ptrTaskList, *ptrPtrTaskAccountInfoList);
			break;
		case 7:
			// TASK PREEMPTED
			TaskUnSchedule(ptrCurrentEvent, ptrPtrEventList, *ptrPtrMachineList, ptrTaskList, ptrPtrTaskAccountInfoList, ptrBalanceAccountInfo);
			break;
		case 8:
			// TASK FINNISHED
			TaskFinnished(ptrCurrentEvent, *ptrPtrEventList, ptrTaskList, *ptrPtrTaskAccountInfoList, *ptrPtrMachineList, ptrBalanceAccountInfo);
			break;
		case 9:
			// JOB ARRIVAL
			JobArrival(ptrCurrentEvent, *ptrPtrEventList, ptrJobList, ptrBalanceAccountInfo);
			break;
		case 10:
			// JOB STARTED
			JobStarted(ptrCurrentEvent, ptrJobAccountInfo, ptrJobList);
			break;
		case 11:
			// JOB FINNISHED
			JobFinnished(ptrCurrentEvent, ptrJobAccountInfo, ptrJobList);
			break;
		case 12:
			printf("eventID %d (Simulation Finnished) time %d", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
			break;
		default:
			printf("unkwonk event!!!\n");
			break;
	}

//	printf("eventID %d\n", ptrCurrentEvent->eventID);
}
