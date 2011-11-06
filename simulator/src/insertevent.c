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

		ptrAuxEvent = ptrActualEvent = ptrLastEvent = ptrEventList;

		while(ptrAuxEvent){
			if (ptrActualEvent->time <= ptrNewEvent->time) {
				ptrLastEvent = ptrActualEvent;
				ptrActualEvent = ptrActualEvent->nextEvent;
			}
			ptrAuxEvent = ptrAuxEvent->nextEvent;
		}

		if (ptrActualEvent) {
			ptrNewEvent->nextEvent = ptrActualEvent;
			ptrLastEvent->nextEvent = ptrNewEvent;
		}
		else { 			// resolves the last-node problem
			ptrNewEvent->nextEvent = NULL;
			ptrLastEvent->nextEvent = ptrNewEvent;
		}

	} else printf("ERROR (insert event): there is no event list!!!\n");

}
