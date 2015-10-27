/*
 * Copyright (C) 2015 by Yanping Chen <ypench@gmail.com>
 * Begin       : 01-Oct-2015
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser GPL (LGPL) as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program.
 */


//#define CONSENSUS_CONFIG

#ifdef CONSENSUS_CONFIG
	#define CONSENSUS_CEDT
	#define CONSENSUS_CRDC
	#define CONSENSUS_ECOR
	#define CONSENSUS_CDOC
	#define CONSENSUS_ENET
	//#define CONSENSUS_OTHER
	//#define CONSENSUS_CRDC

#else
	//#undef CONSENSUS_CEDT
	//#undef CONSENSUS_CRDC

#endif

//#define CONSENSUS_CDOC
#define CONSENSUS_CRDC
//#define CONSENSUS_ECOR
//#define CONSENSUS_CEDT
//#define CONSENSUS_ENET

//#define ECOR_DATA_CHECKING

