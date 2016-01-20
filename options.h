/*
 * Copyright (C) 2016, CompuLab ltd.
 * Author: Andrey Gelman <andrey.gelman@compulab.co.il>
 * License: GPL-2
 */

#ifndef _OPTIONS_H
#define _OPTIONS_H

#include <stdbool.h>


typedef struct {
	bool help;
	bool info;
	int i2c_bus;
	int loglevel;
	char configfile[128];
	long disable;

	/* _private_ */
	bool i2c_bus_set;
	bool loglevel_set;
} Options;


void options_process_or_abort(Options *opts, int argc, char *argv[]);
void show_options(Options *opts);

#endif	/* _OPTIONS_H */

