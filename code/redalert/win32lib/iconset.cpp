//
// Copyright 2020 Electronic Arts Inc.
//
// TiberianDawn.DLL and RedAlert.dll and corresponding source code is free 
// software: you can redistribute it and/or modify it under the terms of 
// the GNU General Public License as published by the Free Software Foundation, 
// either version 3 of the License, or (at your option) any later version.

// TiberianDawn.DLL and RedAlert.dll and corresponding source code is distributed 
// in the hope that it will be useful, but with permitted additional restrictions 
// under Section 7 of the GPL. See the GNU General Public License in LICENSE.TXT 
// distributed with this program. You should have received a copy of the 
// GNU General Public License along with permitted additional restrictions 
// with this program. If not, see https://github.com/electronicarts/CnC_Remastered_Collection

/***************************************************************************
 **   C O N F I D E N T I A L --- W E S T W O O D   A S S O C I A T E S   **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Library                                  *
 *                                                                         *
 *                    File Name : ICONSET.C                                *
 *                                                                         *
 *                   Programmer : Joe L. Bostic                            *
 *                                                                         *
 *                   Start Date : June 9, 1991                             *
 *                                                                         *
 *                  Last Update : September 15, 1993   [JLB]               *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *   Load_Icon_Set -- Loads an icons set and initializes it.               *
 *   Free_Icon_Set -- Frees allocations made by Load_Icon_Set().           *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

//#include	"function.h"

//#define		_WIN32
//#define		WIN32_LEAN_AND_MEAN

//#include	<dos.h>
#include	<stdlib.h>
#include	<stdio.h>
//#include	<mem.h>
#include	<wwstd.h>
#include	<file.h>
#include	"tile.h"
#include	<iff.h>
