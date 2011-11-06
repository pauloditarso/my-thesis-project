/*
 * returncorrectmachine.c
 *
 *  Created on: Sep 13, 2011
 *      Author: PauloDitarso
 */

#include "simulation.h"

machine *ReturnCorrectMachine(event *ptrCurrentEvent, machine *ptrMachineList) {

	machine *ptrAuxMachine;
	ptrAuxMachine = ptrMachineList;

	while(ptrAuxMachine) {
		if (ptrAuxMachine->machineID == ptrCurrentEvent->machineInfo.machineID && ptrAuxMachine->source == ptrCurrentEvent->machineInfo.source) {
			break;
		}
		ptrAuxMachine = ptrAuxMachine->nextMachine;
	}

	return ptrAuxMachine;
}
