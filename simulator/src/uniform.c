/*
 * uniform.c
 *
 *  Created on: Apr 17, 2013
 *      Author: paulo
 */

#include "simulation.h"

double Uniform(double a, double b) {
	return rand() / (RAND_MAX + 1.0) * (b - a) + a;
}
