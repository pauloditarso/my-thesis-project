/*
 * insertevent.c
 *
 *  Created on: Aug 26, 2011
 *      Author: PauloDitarso
 */

#include "simulation.h"

void InsertEvent(event *ptrEventList, event *ptrNewEvent) {

	if (ptrEventList) {

		event *ptrAuxEvent, *ptrActualEvent, *ptrLastEvent;

//		ptrAuxEvent = ptrActualEvent = ptrLastEvent = ptrEventList;
		ptrAuxEvent = ptrActualEvent = ptrLastEvent = ptrThisEvent;

		while(ptrAuxEvent){

			if (ptrActualEvent->time > ptrNewEvent->time)	break;

			ptrLastEvent = ptrActualEvent;
			ptrActualEvent = ptrActualEvent->nextEvent;

			ptrAuxEvent = ptrAuxEvent->nextEvent;
		}

		if (ptrActualEvent) {

			ptrNewEvent->eventNumber = ptrActualEvent->eventNumber;
			while(ptrAuxEvent) {
				ptrAuxEvent->eventNumber += 1;
				ptrAuxEvent = ptrAuxEvent->nextEvent;
			}
			ptrNewEvent->nextEvent = ptrActualEvent;
			ptrLastEvent->nextEvent = ptrNewEvent;
		}
		else { 			// resolves the last-node problem
			ptrNewEvent->eventNumber = (ptrLastEvent->eventNumber + 1);
			ptrNewEvent->nextEvent = NULL;
			ptrLastEvent->nextEvent = ptrNewEvent;
		}

	} else printf("ERROR (insert event): there is no event list!!!\n");

}
