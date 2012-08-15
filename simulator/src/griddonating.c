/*
 * griddonating.c
 *
 *  Created on: Aug 26, 2011
 *      Author: PauloDitarso
 */

#include "simulation.h"

void GridDonating(event *ptrCurrentEvent, event *ptrEventList, machine *ptrMachineList, gridAccountInfo *ptrGridInfoList) {

	if (ptrCurrentEvent->eventID == GRIDDONATING) {

		machine *ptrAuxMachine;
		ptrAuxMachine = ptrMachineList;

		while (ptrAuxMachine) {

			if ( ptrAuxMachine->machineID == ptrCurrentEvent->machineInfo.machineID && ptrAuxMachine->source == ptrCurrentEvent->machineInfo.source ) {

				if ( ptrAuxMachine->source == LOCAL && ptrAuxMachine->status == IDLE ) {

					// it must be DONATING
					if(ptrCurrentEvent->machineInfo.status == DONATING) ptrAuxMachine->status = ptrCurrentEvent->machineInfo.status;
					else printf("ERROR (grid donating): machine status is not DONATING!!!\n");

					InsertGridAccountList(ptrCurrentEvent, ptrAuxMachine, ptrGridInfoList);

					printf("eventID %d (Grid Donating) time %d ", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
					printf("machineID %d source %d status %d\n", ptrAuxMachine->machineID, ptrAuxMachine->source, ptrAuxMachine->status);

					break;
				}

			}

			ptrAuxMachine = ptrAuxMachine->nextMachine;
		}

	} else printf("ERROR (grid donating): wrong eventID!!!\n");
}
