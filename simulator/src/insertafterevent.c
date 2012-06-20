/*
 * insertafterevent.c
 *
 *  Created on: Mar 15, 2012
 *      Author: PauloDitarso
 */

#include "simulation.h"

void InsertAfterEvent(event *ptrEventList, event *ptrNewEvent, event *ptrTargetEvent) {

	if (ptrEventList) {

		event *ptrActualEvent, *ptrNextEvent;
		ptrActualEvent = ptrEventList;
		ptrNextEvent = NULL;

		while(ptrActualEvent) {

			// looking for the target event in which the new one will be inserted after
			if ( ptrActualEvent->eventNumber == ptrTargetEvent->eventNumber && ptrActualEvent->eventID == ptrTargetEvent->eventID
					&& ptrActualEvent->time == ptrTargetEvent->time ) {
				ptrNextEvent = ptrActualEvent->nextEvent;
				break;
			}

			ptrActualEvent = ptrActualEvent->nextEvent;

		}

		if (ptrNextEvent) {
			ptrNewEvent->eventNumber = ptrNextEvent->eventNumber;
			ptrNewEvent->nextEvent = ptrNextEvent;
			ptrActualEvent->nextEvent = ptrNewEvent;
			while(ptrNextEvent) {
				ptrNextEvent->eventNumber += 1;
				ptrNextEvent = ptrNextEvent->nextEvent;
			}
		}
		else { 			// resolves the last-node problem
			ptrNewEvent->eventNumber = (ptrActualEvent->eventNumber + 1);
			ptrNewEvent->nextEvent = NULL;
			ptrActualEvent->nextEvent = ptrNewEvent;
		}

	} else printf("ERROR (insert event): there is no event list!!!\n");

}
