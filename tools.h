/*
 *  tools.h
 *  Momentum
 *
 *  Created by Peter Harrington on 6/28/11.
 *  Copyright 2011 Clean Micro, LLC. All rights reserved.
 *
 *	This file contains general purpose tools.
 *
 */

//general debugging print macro
#ifndef   DEBUG
	#define   PRINTD(arg)
#else
	#define   PRINTD(arg)     printf(arg)
#endif
