/*
 * eventhandler.c
 *
 *  Created on: Aug 26, 2011
 *      Author: PauloDitarso
 */

#include "simulation.h"

void EventHandler(event *ptrCurrentEvent, event **ptrPtrEventList, machine **ptrPtrMachineList, task **ptrPtrTaskList, taskAccountInfo **ptrPtrTaskAccountInfoList,
		gridAccountInfo *ptrGridInfoList, job *ptrJobList, jobAccountInfo *ptrJobAccountInfo, balanceAccountInfo **ptrPtrBalanceAccountInfo, task **ptrPtrOrderedTaskList,
		schedule *ptrScheduleList) {

	switch (ptrCurrentEvent->eventID) {
		case 0:
			printf("eventID %d (Simulation Started) time %ld\n", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
			break;
		case 1:
			// MACHINE ARRIVAL
			MachineArrival(ptrCurrentEvent, *ptrPtrEventList, *ptrPtrMachineList, *ptrPtrTaskList);
			break;
		case 2:
			// MACHINE DEPARTURE
			MachineDeparture(ptrCurrentEvent, *ptrPtrEventList, ptrPtrMachineList, *ptrPtrTaskAccountInfoList);
			break;
		case 3:
			// GRID DONATING
			GridDonating(ptrCurrentEvent, *ptrPtrEventList, *ptrPtrMachineList, ptrGridInfoList);
			break;
		case 4:
			// GRID PREEMPTED
			GridPreempted(ptrCurrentEvent, *ptrPtrEventList, *ptrPtrMachineList, ptrGridInfoList, *ptrPtrBalanceAccountInfo);
			break;
		case 5:
			// TASK ARRIVAL
			TaskArrival(ptrCurrentEvent, *ptrPtrEventList, *ptrPtrTaskList, *ptrPtrMachineList, ptrPtrOrderedTaskList);
			break;
		case 6:
			// TASK SCHEDULED
			TaskSchedule(ptrCurrentEvent, *ptrPtrEventList, *ptrPtrMachineList, *ptrPtrTaskList, *ptrPtrTaskAccountInfoList, ptrScheduleList);
			break;
		case 7:
			// TASK PREEMPTED
			TaskUnSchedule(ptrCurrentEvent, ptrPtrEventList, *ptrPtrMachineList, *ptrPtrTaskList, ptrPtrTaskAccountInfoList, *ptrPtrBalanceAccountInfo, ptrJobList);
			break;
		case 8:
			// TASK FINNISHED
			if (optFlag == 0) {
				TaskFinnished(ptrCurrentEvent, ptrPtrEventList, *ptrPtrTaskList, *ptrPtrTaskAccountInfoList, *ptrPtrMachineList, *ptrPtrBalanceAccountInfo, ptrJobList);
			} else {
				TaskFinnishedOpt(ptrCurrentEvent, ptrPtrEventList, *ptrPtrTaskList, *ptrPtrTaskAccountInfoList, *ptrPtrMachineList, *ptrPtrBalanceAccountInfo, ptrJobList);
			}
			break;
		case 9:
			// JOB ARRIVAL
			JobArrival(ptrCurrentEvent, *ptrPtrEventList, ptrJobList, *ptrPtrTaskList, *ptrPtrMachineList, *ptrPtrBalanceAccountInfo);
			break;
		case 10:
			// JOB STARTED
			JobStarted(ptrCurrentEvent, ptrJobAccountInfo, ptrJobList);
			break;
		case 11:
			// JOB FINNISHED
			if (optFlag == 0) {
				JobFinnished(ptrCurrentEvent, ptrJobAccountInfo, ptrJobList, ptrPtrTaskList);
			} else {
				JobFinnishedOpt(ptrCurrentEvent, ptrJobAccountInfo, ptrJobList, ptrPtrTaskList, ptrPtrOrderedTaskList);
			}
			break;
		case 12:
			// ALLOCATION PLANNING
//			printf("optflag %d\n", optFlag);
			if (optFlag == 0) {
				AllocationPlanning(ptrCurrentEvent, *ptrPtrEventList, *ptrPtrMachineList, *ptrPtrTaskList, ptrPtrBalanceAccountInfo);
			} else {
				AllocationPlanningOpt(ptrCurrentEvent, *ptrPtrEventList, *ptrPtrMachineList, *ptrPtrOrderedTaskList, ptrJobList, ptrPtrBalanceAccountInfo);
			}
			break;
		case 13:
			printf("eventID %d (Simulation Finnished) time %ld\n", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
			break;
		default:
			printf("unkwonk event!!!\n");
			break;
	}

//	printf("eventID %d\n", ptrCurrentEvent->eventID);
}
