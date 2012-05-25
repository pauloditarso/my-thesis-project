/*
 * removeevent.c
 *
 *  Created on: Sep 13, 2011
 *      Author: PauloDitarso
 */

#include "simulation.h"

void RemoveEvent(event **ptrPtrEventList, event *ptrOldEvent) {

	if (ptrOldEvent->eventID == TASKFINNISHED) {

		event *ptrAux, *ptrActualEvent, *ptrLastEvent;
		ptrAux = ptrActualEvent = ptrLastEvent = (*ptrPtrEventList);
		unsigned short int found = 0;

		while (ptrAux) {

			if (ptrAux->eventID == ptrOldEvent->eventID && ptrAux->time == ptrOldEvent->time &&
					ptrAux->taskInfo.taskID == ptrOldEvent->taskInfo.taskID &&
					ptrAux->taskInfo.jobID == ptrOldEvent->taskInfo.jobID) {

				found = 1;
				break;
			}
			else {
				ptrLastEvent = ptrActualEvent;
				ptrActualEvent = ptrActualEvent->nextEvent;
			}
			ptrAux = ptrAux->nextEvent;
		}

		if (found) {

			if (ptrLastEvent != ptrActualEvent) {

				while(ptrAux) {	// decreasing the event number for the following events
					ptrAux->eventNumber -= 1;
					ptrAux = ptrAux->nextEvent;
				}
				ptrLastEvent->nextEvent = ptrActualEvent->nextEvent;
				ptrActualEvent->nextEvent = NULL;
				free(ptrActualEvent);
				ptrActualEvent = NULL;

			}
			else {

				while(ptrAux) {	// decreasing the event number for the following events
					ptrAux->eventNumber -= 1;
					ptrAux = ptrAux->nextEvent;
				}
				event *ptrPtrAux;
				ptrPtrAux = *ptrPtrEventList;
				*ptrPtrEventList = (*ptrPtrEventList)->nextEvent;
				free(ptrPtrAux);
				ptrPtrAux = NULL;

			}

		} else printf("ERROR (remove event): event not found!!!\n");

	} else printf("ERROR (remove event): wrong eventID!!!\n");

}
