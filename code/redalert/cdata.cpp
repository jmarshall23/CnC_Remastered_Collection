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

/* $Header: /CounterStrike/CDATA.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : CDATA.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : May 16, 1994                                                 *
 *                                                                                             *
 *                  Last Update : July 6, 1996 [JLB]                                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   TemplateTypeClass::As_Reference -- Fetches a reference to the template specified.         *
 *   TemplateTypeClass::Create_And_Place -- Creates and places a template object on the map.   *
 *   TemplateTypeClass::Create_One_Of -- Creates an object of this template type.              *
 *   TemplateTypeClass::Display -- Displays a generic representation of template.              *
 *   TemplateTypeClass::From_Name -- Determine template from ASCII name.                       *
 *   TemplateTypeClass::Init -- Loads graphic data for templates.                              *
 *   TemplateTypeClass::Land_Type -- Determines land type from template and icon number.       *
 *   TemplateTypeClass::Occupy_List -- Determines occupation list.                             *
 *   TemplateTypeClass::One_Time -- Performs one-time initialization                           *
 *   TemplateTypeClass::Prep_For_Add -- Prepares to add template to scenario.                  *
 *   TemplateTypeClass::TemplateTypeClass -- Constructor for template type objects.            *
 *   TemplateTypeClass::operator delete -- Deletes a template type object.                     *
 *   TemplateTypeClass::operator new -- Allocates a template type from special heap.           *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include	"image.h"

static TemplateTypeClass const PRoad01(
	TEMPLATE_PROAD01,
	THEATERF_TEMPERATE | THEATERF_SNOW | THEATERF_INTERIOR,
	"PROAD01",
	TXT_CLEAR
);

static TemplateTypeClass const PRoad01a(
	TEMPLATE_PROAD01a,
	THEATERF_TEMPERATE | THEATERF_SNOW | THEATERF_INTERIOR,
	"PROAD01A",
	TXT_CLEAR
);

static TemplateTypeClass const PRoad01b(
	TEMPLATE_PROAD01b,
	THEATERF_TEMPERATE | THEATERF_SNOW | THEATERF_INTERIOR,
	"PROAD01B",
	TXT_CLEAR
);

static TemplateTypeClass const PRoad02(
	TEMPLATE_PROAD02,
	THEATERF_TEMPERATE | THEATERF_SNOW | THEATERF_INTERIOR,
	"PROAD02",
	TXT_CLEAR
);

static TemplateTypeClass const PRoad02a(
	TEMPLATE_PROAD02a,
	THEATERF_TEMPERATE | THEATERF_SNOW | THEATERF_INTERIOR,
	"PROAD02A",
	TXT_CLEAR
);

static TemplateTypeClass const PRoad02b(
	TEMPLATE_PROAD02b,
	THEATERF_TEMPERATE | THEATERF_SNOW | THEATERF_INTERIOR,
	"PROAD02B",
	TXT_CLEAR
);

static TemplateTypeClass const PRoad03(
	TEMPLATE_PROAD03,
	THEATERF_TEMPERATE | THEATERF_SNOW | THEATERF_INTERIOR,
	"PROAD03",
	TXT_CLEAR
);

static TemplateTypeClass const PRoad04(
	TEMPLATE_PROAD04,
	THEATERF_TEMPERATE | THEATERF_SNOW | THEATERF_INTERIOR,
	"PROAD04",
	TXT_CLEAR
);

static TemplateTypeClass const PRoad05(
	TEMPLATE_PROAD05,
	THEATERF_TEMPERATE | THEATERF_SNOW | THEATERF_INTERIOR,
	"PROAD05",
	TXT_CLEAR
);

static TemplateTypeClass const PRoad06(
	TEMPLATE_PROAD06,
	THEATERF_TEMPERATE | THEATERF_SNOW | THEATERF_INTERIOR,
	"PROAD06",
	TXT_CLEAR
);

static TemplateTypeClass const PRoad07(
	TEMPLATE_PROAD07,
	THEATERF_TEMPERATE | THEATERF_SNOW | THEATERF_INTERIOR,
	"PROAD07",
	TXT_CLEAR
);

static TemplateTypeClass const PRoad08(
	TEMPLATE_PROAD08,
	THEATERF_TEMPERATE | THEATERF_SNOW | THEATERF_INTERIOR,
	"PROAD08",
	TXT_CLEAR
);

static TemplateTypeClass const PRoad09(
	TEMPLATE_PROAD09,
	THEATERF_TEMPERATE | THEATERF_SNOW | THEATERF_INTERIOR,
	"PROAD09",
	TXT_CLEAR
);

static TemplateTypeClass const PRoad10(
	TEMPLATE_PROAD10,
	THEATERF_TEMPERATE | THEATERF_SNOW | THEATERF_INTERIOR,
	"PROAD10",
	TXT_CLEAR
);

static TemplateTypeClass const PRoad11(
	TEMPLATE_PROAD11,
	THEATERF_TEMPERATE | THEATERF_SNOW | THEATERF_INTERIOR,
	"PROAD11",
	TXT_CLEAR
);

static TemplateTypeClass const PRoad12(
	TEMPLATE_PROAD12,
	THEATERF_TEMPERATE | THEATERF_SNOW | THEATERF_INTERIOR,
	"PROAD12",
	TXT_CLEAR
);

static TemplateTypeClass const PRoad13(
	TEMPLATE_PROAD13,
	THEATERF_TEMPERATE | THEATERF_SNOW | THEATERF_INTERIOR,
	"PROAD13",
	TXT_CLEAR
);

static TemplateTypeClass const PRoad14(
	TEMPLATE_PROAD14,
	THEATERF_TEMPERATE | THEATERF_SNOW | THEATERF_INTERIOR,
	"PROAD14",
	TXT_CLEAR
);

static TemplateTypeClass const PRoad15(
	TEMPLATE_PROAD15,
	THEATERF_TEMPERATE | THEATERF_SNOW | THEATERF_INTERIOR,
	"PROAD15",
	TXT_CLEAR
);

static TemplateTypeClass const PRoad16(
	TEMPLATE_PROAD16,
	THEATERF_TEMPERATE | THEATERF_SNOW | THEATERF_INTERIOR,
	"PROAD16",
	TXT_CLEAR
);

static TemplateTypeClass const PRoad17(
	TEMPLATE_PROAD17,
	THEATERF_TEMPERATE | THEATERF_SNOW | THEATERF_INTERIOR,
	"PROAD17",
	TXT_CLEAR
);

static TemplateTypeClass const PRoad18(
	TEMPLATE_PROAD18,
	THEATERF_TEMPERATE | THEATERF_SNOW | THEATERF_INTERIOR,
	"PROAD18",
	TXT_CLEAR
);

static TemplateTypeClass const PRoad19(
	TEMPLATE_PROAD19,
	THEATERF_TEMPERATE | THEATERF_SNOW | THEATERF_INTERIOR,
	"PROAD19",
	TXT_CLEAR
);

static TemplateTypeClass const PRoad20(
	TEMPLATE_PROAD20,
	THEATERF_TEMPERATE | THEATERF_SNOW | THEATERF_INTERIOR,
	"PROAD20",
	TXT_CLEAR
);

static TemplateTypeClass const PRoad21(
	TEMPLATE_PROAD21,
	THEATERF_TEMPERATE | THEATERF_SNOW | THEATERF_INTERIOR,
	"PROAD21",
	TXT_CLEAR
);

static TemplateTypeClass const Empty(
	TEMPLATE_CLEAR1,
	THEATERF_TEMPERATE|THEATERF_SNOW|THEATERF_INTERIOR,
	"CLEAR01",
	TXT_CLEAR
);
static TemplateTypeClass const Clear(
	TEMPLATE_CLEAR1,
	THEATERF_TEMPERATE|THEATERF_SNOW|THEATERF_INTERIOR,
	"CLEAR01",
	TXT_CLEAR
);
static TemplateTypeClass const ClearA(
	TEMPLATE_CLEAR1A,
	THEATERF_TEMPERATE | THEATERF_SNOW | THEATERF_INTERIOR,
	"CLEAR01A",
	TXT_CLEAR
);
static TemplateTypeClass const ClearB(
	TEMPLATE_CLEAR1B,
	THEATERF_TEMPERATE | THEATERF_SNOW | THEATERF_INTERIOR,
	"CLEAR01B",
	TXT_CLEAR
);
static TemplateTypeClass const ClearC(
	TEMPLATE_CLEAR1C,
	THEATERF_TEMPERATE | THEATERF_SNOW | THEATERF_INTERIOR,
	"CLEAR01C",
	TXT_CLEAR
);
static TemplateTypeClass const ClearD(
	TEMPLATE_CLEAR1D,
	THEATERF_TEMPERATE | THEATERF_SNOW | THEATERF_INTERIOR,
	"CLEAR01D",
	TXT_CLEAR
);
static TemplateTypeClass const ClearE(
	TEMPLATE_CLEAR1E,
	THEATERF_TEMPERATE | THEATERF_SNOW | THEATERF_INTERIOR,
	"CLEAR01E",
	TXT_CLEAR
);
static TemplateTypeClass const ClearF(
	TEMPLATE_CLEAR1F,
	THEATERF_TEMPERATE | THEATERF_SNOW | THEATERF_INTERIOR,
	"CLEAR01F",
	TXT_CLEAR
);
static TemplateTypeClass const ClearG(
	TEMPLATE_CLEAR1G,
	THEATERF_TEMPERATE | THEATERF_SNOW | THEATERF_INTERIOR,
	"CLEAR01G",
	TXT_CLEAR
);

static TemplateTypeClass const Road01(
	TEMPLATE_ROAD01,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"CLEAR01",
	TXT_ROAD
);
static TemplateTypeClass const Road02(
	TEMPLATE_ROAD02,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC02",
	TXT_ROAD
);
static TemplateTypeClass const Road03(
	TEMPLATE_ROAD03,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC03",
	TXT_ROAD
);
static TemplateTypeClass const Road04(
	TEMPLATE_ROAD04,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC04",
	TXT_ROAD
);
static TemplateTypeClass const Road05(
	TEMPLATE_ROAD05,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC05",
	TXT_ROAD
);
static TemplateTypeClass const Road06(
	TEMPLATE_ROAD06,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC06",
	TXT_ROAD
);
static TemplateTypeClass const Road07(
	TEMPLATE_ROAD07,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC07",
	TXT_ROAD
);
static TemplateTypeClass const Road08(
	TEMPLATE_ROAD08,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC08",
	TXT_ROAD
);
static TemplateTypeClass const Road09(
	TEMPLATE_ROAD09,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC09",
	TXT_ROAD
);
static TemplateTypeClass const Road10(
	TEMPLATE_ROAD10,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC10",
	TXT_ROAD
);
static TemplateTypeClass const Road11(
	TEMPLATE_ROAD11,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC11",
	TXT_ROAD
);
static TemplateTypeClass const Road12(
	TEMPLATE_ROAD12,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC12",
	TXT_ROAD
);
static TemplateTypeClass const Road13(
	TEMPLATE_ROAD13,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC13",
	TXT_ROAD
);
static TemplateTypeClass const Road14(
	TEMPLATE_ROAD14,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC14",
	TXT_ROAD
);
static TemplateTypeClass const Road15(
	TEMPLATE_ROAD15,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC15",
	TXT_ROAD
);
static TemplateTypeClass const Road16(
	TEMPLATE_ROAD16,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC16",
	TXT_ROAD
);
static TemplateTypeClass const Road17(
	TEMPLATE_ROAD17,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC17",
	TXT_ROAD
);
static TemplateTypeClass const Road18(
	TEMPLATE_ROAD18,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC18",
	TXT_ROAD
);
static TemplateTypeClass const Road19(
	TEMPLATE_ROAD19,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC19",
	TXT_ROAD
);
static TemplateTypeClass const Road20(
	TEMPLATE_ROAD20,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC20",
	TXT_ROAD
);
static TemplateTypeClass const Road21(
	TEMPLATE_ROAD21,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC21",
	TXT_ROAD
);
static TemplateTypeClass const Road22(
	TEMPLATE_ROAD22,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC22",
	TXT_ROAD
);
static TemplateTypeClass const Road23(
	TEMPLATE_ROAD23,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC23",
	TXT_ROAD
);
static TemplateTypeClass const Road24(
	TEMPLATE_ROAD24,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC24",
	TXT_ROAD
);
static TemplateTypeClass const Road25(
	TEMPLATE_ROAD25,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC01",
	TXT_ROAD
);
static TemplateTypeClass const Road26(
	TEMPLATE_ROAD26,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC02",
	TXT_ROAD
);
static TemplateTypeClass const Road27(
	TEMPLATE_ROAD27,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC02",
	TXT_ROAD
);
static TemplateTypeClass const Road28(
	TEMPLATE_ROAD28,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC03",
	TXT_ROAD
);
static TemplateTypeClass const Road29(
	TEMPLATE_ROAD29,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC04",
	TXT_ROAD
);
static TemplateTypeClass const Road30(
	TEMPLATE_ROAD30,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC05",
	TXT_ROAD
);
static TemplateTypeClass const Road31(
	TEMPLATE_ROAD31,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC06",
	TXT_ROAD
);
static TemplateTypeClass const Road32(
	TEMPLATE_ROAD32,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC07",
	TXT_ROAD
);
static TemplateTypeClass const Road33(
	TEMPLATE_ROAD33,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC08",
	TXT_ROAD
);
static TemplateTypeClass const Road34(
	TEMPLATE_ROAD34,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC09",
	TXT_ROAD
);
static TemplateTypeClass const Road35(
	TEMPLATE_ROAD35,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC10",
	TXT_ROAD
);
static TemplateTypeClass const Road36(
	TEMPLATE_ROAD36,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC11",
	TXT_ROAD
);
static TemplateTypeClass const Road37(
	TEMPLATE_ROAD37,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC12",
	TXT_ROAD
);
static TemplateTypeClass const Road38(
	TEMPLATE_ROAD38,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC13",
	TXT_ROAD
);
static TemplateTypeClass const Road39(
	TEMPLATE_ROAD39,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC14",
	TXT_ROAD
);
static TemplateTypeClass const Road40(
	TEMPLATE_ROAD40,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC15",
	TXT_ROAD
);
static TemplateTypeClass const Road41(
	TEMPLATE_ROAD41,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC16",
	TXT_ROAD
);
static TemplateTypeClass const Road42(
	TEMPLATE_ROAD42,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC17",
	TXT_ROAD
);
static TemplateTypeClass const Road43(
	TEMPLATE_ROAD43,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADC18",
	TXT_ROAD
);
static TemplateTypeClass const Water(
	TEMPLATE_WATER,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WATER01",
	TXT_WATER
);
static TemplateTypeClass const Water2(
	TEMPLATE_WATER2,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WATER02",
	TXT_WATER
);
static TemplateTypeClass const Shore01(
	TEMPLATE_SHORE01,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE01",
	TXT_SHORE
);
static TemplateTypeClass const Shore02(
	TEMPLATE_SHORE02,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE02",
	TXT_SHORE
);
static TemplateTypeClass const Shore03(
	TEMPLATE_SHORE03,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE03",
	TXT_SHORE
);
static TemplateTypeClass const Shore04(
	TEMPLATE_SHORE04,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE04",
	TXT_SHORE
);
static TemplateTypeClass const Shore05(
	TEMPLATE_SHORE05,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE05",
	TXT_SHORE
);
static TemplateTypeClass const Shore06(
	TEMPLATE_SHORE06,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE06",
	TXT_SHORE
);
static TemplateTypeClass const Shore07(
	TEMPLATE_SHORE07,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE07",
	TXT_SHORE
);
static TemplateTypeClass const Shore08(
	TEMPLATE_SHORE08,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE08",
	TXT_SHORE
);
static TemplateTypeClass const Shore09(
	TEMPLATE_SHORE09,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE09",
	TXT_SHORE
);
static TemplateTypeClass const Shore10(
	TEMPLATE_SHORE10,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE10",
	TXT_SHORE
);
static TemplateTypeClass const Shore11(
	TEMPLATE_SHORE11,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE11",
	TXT_SHORE
);
static TemplateTypeClass const Shore12(
	TEMPLATE_SHORE12,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE12",
	TXT_SHORE
);
static TemplateTypeClass const Shore13(
	TEMPLATE_SHORE13,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE13",
	TXT_SHORE
);
static TemplateTypeClass const Shore14(
	TEMPLATE_SHORE14,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE14",
	TXT_SHORE
);
static TemplateTypeClass const Shore15(
	TEMPLATE_SHORE15,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE15",
	TXT_SHORE
);
static TemplateTypeClass const Shore16(
	TEMPLATE_SHORE16,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE16",
	TXT_SHORE
);
static TemplateTypeClass const Shore17(
	TEMPLATE_SHORE17,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE17",
	TXT_SHORE
);
static TemplateTypeClass const Shore18(
	TEMPLATE_SHORE18,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE18",
	TXT_SHORE
);
static TemplateTypeClass const Shore19(
	TEMPLATE_SHORE19,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE19",
	TXT_SHORE
);
static TemplateTypeClass const Shore20(
	TEMPLATE_SHORE20,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE20",
	TXT_SHORE
);
static TemplateTypeClass const Shore21(
	TEMPLATE_SHORE21,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE21",
	TXT_SHORE
);
static TemplateTypeClass const Shore22(
	TEMPLATE_SHORE22,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE22",
	TXT_SHORE
);
static TemplateTypeClass const Shore23(
	TEMPLATE_SHORE23,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE23",
	TXT_SHORE
);
static TemplateTypeClass const Shore24(
	TEMPLATE_SHORE24,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE24",
	TXT_SHORE
);
static TemplateTypeClass const Shore25(
	TEMPLATE_SHORE25,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE25",
	TXT_SHORE
);
static TemplateTypeClass const Shore26(
	TEMPLATE_SHORE26,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE26",
	TXT_SHORE
);
static TemplateTypeClass const Shore27(
	TEMPLATE_SHORE27,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE27",
	TXT_SHORE
);
static TemplateTypeClass const Shore28(
	TEMPLATE_SHORE28,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE28",
	TXT_SHORE
);
static TemplateTypeClass const Shore29(
	TEMPLATE_SHORE29,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE29",
	TXT_SHORE
);
static TemplateTypeClass const Shore30(
	TEMPLATE_SHORE30,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE30",
	TXT_SHORE
);
static TemplateTypeClass const Shore31(
	TEMPLATE_SHORE31,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE31",
	TXT_SHORE
);
static TemplateTypeClass const Shore32(
	TEMPLATE_SHORE32,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE32",
	TXT_SHORE
);
static TemplateTypeClass const Shore33(
	TEMPLATE_SHORE33,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE33",
	TXT_SHORE
);
static TemplateTypeClass const Shore34(
	TEMPLATE_SHORE34,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE4",
	TXT_SHORE
);
static TemplateTypeClass const Shore35(
	TEMPLATE_SHORE35,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE35",
	TXT_SHORE
);
static TemplateTypeClass const Shore36(
	TEMPLATE_SHORE36,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE36",
	TXT_SHORE
);
static TemplateTypeClass const Shore37(
	TEMPLATE_SHORE37,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE37",
	TXT_SHORE
);
static TemplateTypeClass const Shore38(
	TEMPLATE_SHORE38,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE38",
	TXT_SHORE
);
static TemplateTypeClass const Shore39(
	TEMPLATE_SHORE39,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE39",
	TXT_SHORE
);
static TemplateTypeClass const Shore40(
	TEMPLATE_SHORE40,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE40",
	TXT_SHORE
);
static TemplateTypeClass const Shore41(
	TEMPLATE_SHORE41,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"SHORE41",
	TXT_SHORE
);
// jmarshall
static TemplateTypeClass const Bridge1x(
	TEMPLATE_BRIDGE1X,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_BRIDGE
);
static TemplateTypeClass const Bridge1(
	TEMPLATE_BRIDGE1,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_BRIDGE
);
static TemplateTypeClass const Bridge1h(
	TEMPLATE_BRIDGE1H,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_BRIDGE
);
static TemplateTypeClass const Bridge1d(
	TEMPLATE_BRIDGE1D,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_BRIDGE
);
static TemplateTypeClass const Bridge2x(
	TEMPLATE_BRIDGE2X,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_BRIDGE
);
static TemplateTypeClass const Bridge2(
	TEMPLATE_BRIDGE2,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_BRIDGE
);
static TemplateTypeClass const Bridge2h(
	TEMPLATE_BRIDGE2H,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_BRIDGE
);
static TemplateTypeClass const Bridge2d(
	TEMPLATE_BRIDGE2D,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_BRIDGE
);
static TemplateTypeClass const Bridge1ax(
	TEMPLATE_BRIDGE_1AX,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_BRIDGE
);
static TemplateTypeClass const Bridge1a(
	TEMPLATE_BRIDGE_1A,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_BRIDGE
);
static TemplateTypeClass const Bridge1b(
	TEMPLATE_BRIDGE_1B,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_BRIDGE
);
static TemplateTypeClass const Bridge1c(
	TEMPLATE_BRIDGE_1C,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_BRIDGE
);
static TemplateTypeClass const Bridge2ax(
	TEMPLATE_BRIDGE_2AX,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_BRIDGE
);
static TemplateTypeClass const Bridge2a(
	TEMPLATE_BRIDGE_2A,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_BRIDGE
);
static TemplateTypeClass const Bridge2b(
	TEMPLATE_BRIDGE_2B,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_BRIDGE
);
static TemplateTypeClass const Bridge2c(
	TEMPLATE_BRIDGE_2C,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_BRIDGE
);
static TemplateTypeClass const Bridge3a(
	TEMPLATE_BRIDGE_3A,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_BRIDGE
);
static TemplateTypeClass const Bridge3b(
	TEMPLATE_BRIDGE_3B,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_BRIDGE
);
static TemplateTypeClass const Bridge3c(
	TEMPLATE_BRIDGE_3C,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_BRIDGE
);
static TemplateTypeClass const Bridge3d(
	TEMPLATE_BRIDGE_3D,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_BRIDGE
);
static TemplateTypeClass const Bridge3e(
	TEMPLATE_BRIDGE_3E,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_BRIDGE
);
static TemplateTypeClass const Bridge3f(
	TEMPLATE_BRIDGE_3F,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_BRIDGE
);
static TemplateTypeClass const ShoreCliff01(
	TEMPLATE_SHORECLIFF01,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF01",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff02(
	TEMPLATE_SHORECLIFF02,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF02",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff03(
	TEMPLATE_SHORECLIFF03,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF03",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff04(
	TEMPLATE_SHORECLIFF04,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF04",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff05(
	TEMPLATE_SHORECLIFF05,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF05",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff06(
	TEMPLATE_SHORECLIFF06,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF06",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff07(
	TEMPLATE_SHORECLIFF07,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF07",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff08(
	TEMPLATE_SHORECLIFF08,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF08",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff09(
	TEMPLATE_SHORECLIFF09,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF09",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff10(
	TEMPLATE_SHORECLIFF10,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF10",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff11(
	TEMPLATE_SHORECLIFF11,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF11",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff12(
	TEMPLATE_SHORECLIFF12,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF12",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff13(
	TEMPLATE_SHORECLIFF13,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF13",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff14(
	TEMPLATE_SHORECLIFF14,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF14",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff15(
	TEMPLATE_SHORECLIFF15,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF15",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff16(
	TEMPLATE_SHORECLIFF16,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF16",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff17(
	TEMPLATE_SHORECLIFF17,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF17",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff18(
	TEMPLATE_SHORECLIFF18,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF18",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff19(
	TEMPLATE_SHORECLIFF19,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF19",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff20(
	TEMPLATE_SHORECLIFF20,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF20",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff21(
	TEMPLATE_SHORECLIFF21,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF21",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff22(
	TEMPLATE_SHORECLIFF22,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF22",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff23(
	TEMPLATE_SHORECLIFF23,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF23",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff24(
	TEMPLATE_SHORECLIFF24,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF24",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff25(
	TEMPLATE_SHORECLIFF25,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF25",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff26(
	TEMPLATE_SHORECLIFF26,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF26",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff27(
	TEMPLATE_SHORECLIFF27,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF27",
	TXT_SHORE
);
static TemplateTypeClass const ShoreCliff28(
	TEMPLATE_SHORECLIFF28,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"WCLIFF28",
	TXT_SHORE
);

static TemplateTypeClass const Rough01(
	TEMPLATE_ROUGH01,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"ROUGH01",
	TXT_ROCK
);
static TemplateTypeClass const Rough02(
	TEMPLATE_ROUGH02,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"ROUGH02",
	TXT_ROCK
);
static TemplateTypeClass const Rough03(
	TEMPLATE_ROUGH03,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"ROUGH03",
	TXT_ROCK
);
static TemplateTypeClass const Rough04(
	TEMPLATE_ROUGH04,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"ROUGH04",
	TXT_ROCK
);
static TemplateTypeClass const Rough05(
	TEMPLATE_ROUGH05,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"ROUGH05",
	TXT_ROCK
);
static TemplateTypeClass const Rough06(
	TEMPLATE_ROUGH06,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"ROUGH06",
	TXT_ROCK
);
static TemplateTypeClass const Rough07(
	TEMPLATE_ROUGH07,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"ROUGH07",
	TXT_ROCK
);
static TemplateTypeClass const Rough08(
	TEMPLATE_ROUGH08,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"ROUGH08",
	TXT_ROCK
);
static TemplateTypeClass const Rough09(
	TEMPLATE_ROUGH09,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"ROUGH10",
	TXT_ROCK
);
static TemplateTypeClass const Rough10(
	TEMPLATE_ROADS1,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADS01",
	TXT_ROCK
);
static TemplateTypeClass const Rough11(
	TEMPLATE_ROADS2,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADS02",
	TXT_ROCK
);
static TemplateTypeClass const RiverCliff01(
	TEMPLATE_ROADS3,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADS03",
	TXT_RIVER
);
static TemplateTypeClass const RiverCliff02(
	TEMPLATE_ROADS4,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADS04",
	TXT_RIVER
);
static TemplateTypeClass const RiverCliff03(
	TEMPLATE_ROADS5,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADS05",
	TXT_RIVER
);
static TemplateTypeClass const RiverCliff04(
	TEMPLATE_ROADS6,
	THEATERF_TEMPERATE|THEATERF_SNOW,
	"DROADS06",
	TXT_RIVER
);

/***********************************************************************************************
 * TemplateTypeClass::TemplateTypeClass -- Constructor for template type objects.              *
 *                                                                                             *
 *    This is the constructor for the template types.                                          *
 *                                                                                             *
 * INPUT:   see below...                                                                       *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/29/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
TemplateTypeClass::TemplateTypeClass(
	TemplateType iconset,
	int theater,
	char const * ininame,
	int fullname) :
		ObjectTypeClass(
			RTTI_TEMPLATETYPE,
			int(iconset),
			false,
			true,
			false,
			false,
			true,
			true,
			false,
			fullname,
			ininame),
	Type(iconset),
	Theater(theater),
	Width(0),
	Height(0)
{
}


/***********************************************************************************************
 * TemplateTypeClass::operator new -- Allocates a template type from special heap.             *
 *                                                                                             *
 *    This allocates a template type object from the special heap used for that purpose.       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the newly allocated template type object. If no object   *
 *          could be allocated, then NULL is returned.                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void * TemplateTypeClass::operator new(size_t)
{
	return(TemplateTypes.Alloc());
}


/***********************************************************************************************
 * TemplateTypeClass::operator delete -- Deletes a template type object.                       *
 *                                                                                             *
 *    This routine will return a template type object back to the special heap it was          *
 *    allocated from.                                                                          *
 *                                                                                             *
 * INPUT:   ptr   -- Pointer to the template type object to free.                              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void TemplateTypeClass::operator delete(void * ptr)
{
	TemplateTypes.Free((TemplateTypeClass *)ptr);
}

void TemplateTypeClass::Init_Heap(void)
{
	/*
	**	These template type class objects must be allocated in the exact order that they
	**	are specified in the TemplateType enumeration. This is necessary because the heap
	**	allocation block index serves double duty as the type number index.
	*/
	(void)new TemplateTypeClass(Clear);				// TEMPLATE_CLEAR1
	(void)new TemplateTypeClass(ClearA);				// TEMPLATE_CLEAR1
	(void)new TemplateTypeClass(ClearB);				// TEMPLATE_CLEAR1
	(void)new TemplateTypeClass(ClearC);				// TEMPLATE_CLEAR1
	(void)new TemplateTypeClass(ClearD);				// TEMPLATE_CLEAR1
	(void)new TemplateTypeClass(ClearE);				// TEMPLATE_CLEAR1
	(void)new TemplateTypeClass(ClearF);				// TEMPLATE_CLEAR1
	(void)new TemplateTypeClass(ClearG);				// TEMPLATE_CLEAR1
	(void)new TemplateTypeClass(Water);				// TEMPLATE_WATER
	(void)new TemplateTypeClass(Water2);			// TEMPLATE_WATER2
	(void)new TemplateTypeClass(PRoad01);
	(void)new TemplateTypeClass(PRoad01a);
	(void)new TemplateTypeClass(PRoad01b);;
	(void)new TemplateTypeClass(PRoad02);
	(void)new TemplateTypeClass(PRoad02a);
	(void)new TemplateTypeClass(PRoad02b);
	(void)new TemplateTypeClass(PRoad03);
	(void)new TemplateTypeClass(PRoad04);
	(void)new TemplateTypeClass(PRoad05);
	(void)new TemplateTypeClass(PRoad06);
	(void)new TemplateTypeClass(PRoad07);
	(void)new TemplateTypeClass(PRoad08);
	(void)new TemplateTypeClass(PRoad09);
	(void)new TemplateTypeClass(PRoad10);
	(void)new TemplateTypeClass(PRoad11);
	(void)new TemplateTypeClass(PRoad12);
	(void)new TemplateTypeClass(PRoad13);
	(void)new TemplateTypeClass(PRoad14);
	(void)new TemplateTypeClass(PRoad15);
	(void)new TemplateTypeClass(PRoad16);
	(void)new TemplateTypeClass(PRoad17);
	(void)new TemplateTypeClass(PRoad18);
	(void)new TemplateTypeClass(PRoad19);
	(void)new TemplateTypeClass(PRoad20);
	(void)new TemplateTypeClass(PRoad21);
	(void)new TemplateTypeClass(Shore01);			// TEMPLATE_SHORE1
	(void)new TemplateTypeClass(Shore02);			// TEMPLATE_SHORE2
	(void)new TemplateTypeClass(Shore03);			// TEMPLATE_SHORE3
	(void)new TemplateTypeClass(Shore04);			// TEMPLATE_SHORE4
	(void)new TemplateTypeClass(Shore05);			// TEMPLATE_SHORE5
	(void)new TemplateTypeClass(Shore06);			// TEMPLATE_SHORE6
	(void)new TemplateTypeClass(Shore07);			// TEMPLATE_SHORE7
	(void)new TemplateTypeClass(Shore08);			// TEMPLATE_SHORE8
	(void)new TemplateTypeClass(Shore09);			// TEMPLATE_SHORE9
	(void)new TemplateTypeClass(Shore10);			// TEMPLATE_SHORE10
	(void)new TemplateTypeClass(Shore11);			//	TEMPLATE_SHORE11
	(void)new TemplateTypeClass(Shore12);			// TEMPLATE_SHORE12
	(void)new TemplateTypeClass(Shore13);			// TEMPLATE_SHORE13
	(void)new TemplateTypeClass(Shore14);			// TEMPLATE_SHORE14
	(void)new TemplateTypeClass(Shore15);			// TEMPLATE_SHORE15
	(void)new TemplateTypeClass(Shore16);			//	TEMPLATE_SHORE16
	(void)new TemplateTypeClass(Shore17);			//	TEMPLATE_SHORE17
	(void)new TemplateTypeClass(Shore18);			//	TEMPLATE_SHORE18
	(void)new TemplateTypeClass(Shore19);			// TEMPLATE_SHORE19
	(void)new TemplateTypeClass(Shore20);			// TEMPLATE_SHORE20
	(void)new TemplateTypeClass(Shore21);			// TEMPLATE_SHORE21
	(void)new TemplateTypeClass(Shore22);			//	TEMPLATE_SHORE22
	(void)new TemplateTypeClass(Shore23);			// TEMPLATE_SHORE23
	(void)new TemplateTypeClass(Shore24);			//	TEMPLATE_SHORE24
	(void)new TemplateTypeClass(Shore25);			//	TEMPLATE_SHORE25
	(void)new TemplateTypeClass(Shore26);			//	TEMPLATE_SHORE26
	(void)new TemplateTypeClass(Shore27);			//	TEMPLATE_SHORE27
	(void)new TemplateTypeClass(Shore28);			//	TEMPLATE_SHORE28
	(void)new TemplateTypeClass(Shore29);			//	TEMPLATE_SHORE29
	(void)new TemplateTypeClass(Shore30);			//	TEMPLATE_SHORE30
	(void)new TemplateTypeClass(Shore31);			//	TEMPLATE_SHORE31
	(void)new TemplateTypeClass(Shore32);			// TEMPLATE_SHORE32
	(void)new TemplateTypeClass(Shore33);			// TEMPLATE_SHORE33
	(void)new TemplateTypeClass(Shore34);			//	TEMPLATE_SHORE34
	(void)new TemplateTypeClass(Shore35);			//	TEMPLATE_SHORE35
	(void)new TemplateTypeClass(Shore36);			//	TEMPLATE_SHORE36
	(void)new TemplateTypeClass(Shore37);			//	TEMPLATE_SHORE37
	(void)new TemplateTypeClass(Shore38);			//	TEMPLATE_SHORE38
	(void)new TemplateTypeClass(Shore39);			//	TEMPLATE_SHORE39
	(void)new TemplateTypeClass(Shore40);			//	TEMPLATE_SHORE40
	(void)new TemplateTypeClass(Shore41);			//	TEMPLATE_SHORE41
	(void)new TemplateTypeClass(ShoreCliff01);	//	TEMPLATE_SHORECLIFF01
	(void)new TemplateTypeClass(ShoreCliff02);	//	TEMPLATE_SHORECLIFF02
	(void)new TemplateTypeClass(ShoreCliff03);	//	TEMPLATE_SHORECLIFF03
	(void)new TemplateTypeClass(ShoreCliff04);	//	TEMPLATE_SHORECLIFF04
	(void)new TemplateTypeClass(ShoreCliff05);	//	TEMPLATE_SHORECLIFF05
	(void)new TemplateTypeClass(ShoreCliff06);	//	TEMPLATE_SHORECLIFF06
	(void)new TemplateTypeClass(ShoreCliff07);	//	TEMPLATE_SHORECLIFF07
	(void)new TemplateTypeClass(ShoreCliff08);	//	TEMPLATE_SHORECLIFF08
	(void)new TemplateTypeClass(ShoreCliff09);	//	TEMPLATE_SHORECLIFF09
	(void)new TemplateTypeClass(ShoreCliff10);	//	TEMPLATE_SHORECLIFF10
	(void)new TemplateTypeClass(ShoreCliff11);	//	TEMPLATE_SHORECLIFF11
	(void)new TemplateTypeClass(ShoreCliff12);	//	TEMPLATE_SHORECLIFF12
	(void)new TemplateTypeClass(ShoreCliff13);	//	TEMPLATE_SHORECLIFF13
	(void)new TemplateTypeClass(ShoreCliff14);	//	TEMPLATE_SHORECLIFF14
	(void)new TemplateTypeClass(ShoreCliff15);	//	TEMPLATE_SHORECLIFF15
	(void)new TemplateTypeClass(ShoreCliff16);	//	TEMPLATE_SHORECLIFF16
	(void)new TemplateTypeClass(ShoreCliff17);	//	TEMPLATE_SHORECLIFF17
	(void)new TemplateTypeClass(ShoreCliff18);	//	TEMPLATE_SHORECLIFF18
	(void)new TemplateTypeClass(ShoreCliff19);	//	TEMPLATE_SHORECLIFF19
	(void)new TemplateTypeClass(ShoreCliff20);	//	TEMPLATE_SHORECLIFF20
	(void)new TemplateTypeClass(ShoreCliff21);	//	TEMPLATE_SHORECLIFF21
	(void)new TemplateTypeClass(ShoreCliff22);	//	TEMPLATE_SHORECLIFF22
	(void)new TemplateTypeClass(ShoreCliff23);	//	TEMPLATE_SHORECLIFF23
	(void)new TemplateTypeClass(ShoreCliff24);	//	TEMPLATE_SHORECLIFF24
	(void)new TemplateTypeClass(ShoreCliff25);	//	TEMPLATE_SHORECLIFF25
	(void)new TemplateTypeClass(ShoreCliff26);	//	TEMPLATE_SHORECLIFF26
	(void)new TemplateTypeClass(ShoreCliff27);	//	TEMPLATE_SHORECLIFF27
	(void)new TemplateTypeClass(ShoreCliff28);	//	TEMPLATE_SHORECLIFF28
	(void)new TemplateTypeClass(Bridge1);			//	TEMPLATE_BRIDGE1
	(void)new TemplateTypeClass(Bridge1d);			//	TEMPLATE_BRIDGE1D
	(void)new TemplateTypeClass(Bridge2);			//	TEMPLATE_BRIDGE2
	(void)new TemplateTypeClass(Bridge2d);			//	TEMPLATE_BRIDGE2D
	(void)new TemplateTypeClass(Road01);			//	TEMPLATE_ROAD1
	(void)new TemplateTypeClass(Road02);			//	TEMPLATE_ROAD2
	(void)new TemplateTypeClass(Road03);			//	TEMPLATE_ROAD3
	(void)new TemplateTypeClass(Road04);			//	TEMPLATE_ROAD4
	(void)new TemplateTypeClass(Road05);			//	TEMPLATE_ROAD5
	(void)new TemplateTypeClass(Road06);			//	TEMPLATE_ROAD6
	(void)new TemplateTypeClass(Road07);			//	TEMPLATE_ROAD7
	(void)new TemplateTypeClass(Road08);			//	TEMPLATE_ROAD8
	(void)new TemplateTypeClass(Road09);			//	TEMPLATE_ROAD9
	(void)new TemplateTypeClass(Road10);			//	TEMPLATE_ROAD10
	(void)new TemplateTypeClass(Road11);			//	TEMPLATE_ROAD11
	(void)new TemplateTypeClass(Road12);			//	TEMPLATE_ROAD12
	(void)new TemplateTypeClass(Road13);			//	TEMPLATE_ROAD13
	(void)new TemplateTypeClass(Road14);			//	TEMPLATE_ROAD14
	(void)new TemplateTypeClass(Road15);			//	TEMPLATE_ROAD15
	(void)new TemplateTypeClass(Road16);			//	TEMPLATE_ROAD16
	(void)new TemplateTypeClass(Road17);			//	TEMPLATE_ROAD17
	(void)new TemplateTypeClass(Road18);			//	TEMPLATE_ROAD18
	(void)new TemplateTypeClass(Road19);			//	TEMPLATE_ROAD19
	(void)new TemplateTypeClass(Road20);			//	TEMPLATE_ROAD20
	(void)new TemplateTypeClass(Road21);			//	TEMPLATE_ROAD21
	(void)new TemplateTypeClass(Road22);			//	TEMPLATE_ROAD22
	(void)new TemplateTypeClass(Road23);			//	TEMPLATE_ROAD23
	(void)new TemplateTypeClass(Road24);			//	TEMPLATE_ROAD24
	(void)new TemplateTypeClass(Road25);			//	TEMPLATE_ROAD25
	(void)new TemplateTypeClass(Road26);			//	TEMPLATE_ROAD26
	(void)new TemplateTypeClass(Road27);			//	TEMPLATE_ROAD27
	(void)new TemplateTypeClass(Road28);			//	TEMPLATE_ROAD28
	(void)new TemplateTypeClass(Road29);			//	TEMPLATE_ROAD29
	(void)new TemplateTypeClass(Road30);			//	TEMPLATE_ROAD30
	(void)new TemplateTypeClass(Road31);			//	TEMPLATE_ROAD31
	(void)new TemplateTypeClass(Road32);			//	TEMPLATE_ROAD32
	(void)new TemplateTypeClass(Road33);			//	TEMPLATE_ROAD33
	(void)new TemplateTypeClass(Road34);			//	TEMPLATE_ROAD34
	(void)new TemplateTypeClass(Road35);			//	TEMPLATE_ROAD35
	(void)new TemplateTypeClass(Road36);			//	TEMPLATE_ROAD36
	(void)new TemplateTypeClass(Road37);			//	TEMPLATE_ROAD37
	(void)new TemplateTypeClass(Road38);			//	TEMPLATE_ROAD38
	(void)new TemplateTypeClass(Road39);			//	TEMPLATE_ROAD39
	(void)new TemplateTypeClass(Road40);			//	TEMPLATE_ROAD40
	(void)new TemplateTypeClass(Road41);			//	TEMPLATE_ROAD41
	(void)new TemplateTypeClass(Road42);			//	TEMPLATE_ROAD42
	(void)new TemplateTypeClass(Road43);			//	TEMPLATE_ROAD43
	(void)new TemplateTypeClass(Rough01);			// TEMPLATE_ROUGH01
	(void)new TemplateTypeClass(Rough02);			// TEMPLATE_ROUGH02
	(void)new TemplateTypeClass(Rough03);			// TEMPLATE_ROUGH03
	(void)new TemplateTypeClass(Rough04);			// TEMPLATE_ROUGH04
	(void)new TemplateTypeClass(Rough05);			// TEMPLATE_ROUGH05
	(void)new TemplateTypeClass(Rough06);			// TEMPLATE_ROUGH06
	(void)new TemplateTypeClass(Rough07);			// TEMPLATE_ROUGH07
	(void)new TemplateTypeClass(Rough08);			// TEMPLATE_ROUGH08
	(void)new TemplateTypeClass(Rough09);			// TEMPLATE_ROUGH09
	(void)new TemplateTypeClass(Rough10);			// TEMPLATE_ROUGH10
	(void)new TemplateTypeClass(Rough11);			// TEMPLATE_ROUGH11
//	(void)new TemplateTypeClass(River14);			//	TEMPLATE_RIVER14
//	(void)new TemplateTypeClass(River15);			//	TEMPLATE_RIVER15
	(void)new TemplateTypeClass(RiverCliff01);	//	TEMPLATE_RIVERCLIFF01
	(void)new TemplateTypeClass(RiverCliff02);	//	TEMPLATE_RIVERCLIFF02
	(void)new TemplateTypeClass(RiverCliff03);	//	TEMPLATE_RIVERCLIFF03
	(void)new TemplateTypeClass(RiverCliff04);	//	TEMPLATE_RIVERCLIFF04
	(void)new TemplateTypeClass(Bridge1a);			//	TEMPLATE_BRIDGE_1A
	(void)new TemplateTypeClass(Bridge1b);			//	TEMPLATE_BRIDGE_1B
	(void)new TemplateTypeClass(Bridge1c);			//	TEMPLATE_BRIDGE_1C
	(void)new TemplateTypeClass(Bridge2a);			//	TEMPLATE_BRIDGE_2A
	(void)new TemplateTypeClass(Bridge2b);			//	TEMPLATE_BRIDGE_2B
	(void)new TemplateTypeClass(Bridge2c);			//	TEMPLATE_BRIDGE_2C
	(void)new TemplateTypeClass(Bridge3a);			//	TEMPLATE_BRIDGE_3A
	(void)new TemplateTypeClass(Bridge3b);			//	TEMPLATE_BRIDGE_3B
	(void)new TemplateTypeClass(Bridge3c);			//	TEMPLATE_BRIDGE_3C
	(void)new TemplateTypeClass(Bridge3d);			//	TEMPLATE_BRIDGE_3D
	(void)new TemplateTypeClass(Bridge3e);			//	TEMPLATE_BRIDGE_3E
	(void)new TemplateTypeClass(Bridge3f);			//	TEMPLATE_BRIDGE_3F
	(void)new TemplateTypeClass(Bridge1h);			//	TEMPLATE_BRIDGE1H
	(void)new TemplateTypeClass(Bridge2h);			//	TEMPLATE_BRIDGE2H
	(void)new TemplateTypeClass(Bridge1ax);		//	TEMPLATE_BRIDGE_1AX
	(void)new TemplateTypeClass(Bridge2ax);		//	TEMPLATE_BRIDGE_2AX
	(void)new TemplateTypeClass(Bridge1x);			//	TEMPLATE_BRIDGE1X
	(void)new TemplateTypeClass(Bridge2x);			//	TEMPLATE_BRIDGE2X
}


/***********************************************************************************************
 * TemplateTypeClass::Land_Type -- Determines land type from template and icon number.         *
 *                                                                                             *
 *    This routine will convert the specified icon number into the appropriate land type. The  *
 *    land type can be determined from the embedded colors in the "control template" section   *
 *    of the original art file. This control information is encoded into the icon data file    *
 *    to be retrieved and interpreted as the program sees fit. The engine only recognizes      *
 *    the first 16 colors as control colors, so the control map color value serves as an       *
 *    index into a simple lookup table.                                                        *
 *                                                                                             *
 * INPUT:   icon  -- The icon number within this template that is to be examined and used      *
 *                   to determine the land type.                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the land type that corresponds to the icon number specified.          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/12/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
LandType TemplateTypeClass::Land_Type(int icon) const
{
	IsoTile* icontrol = (IsoTile *)Get_Image_Data();

	if (icontrol != NULL) {
		IsoTileImageHeader* image = icontrol->GetTileInfo(icon);
		if (image != NULL) {
			static LandType _land[16] = {
				LAND_CLEAR,
				LAND_ROUGH ,
				LAND_ROAD,
				LAND_WATER,
				LAND_ROCK,
				LAND_WALL,
				LAND_TIBERIUM,
				LAND_WEEDS,
				LAND_BEACH,
				LAND_WATER,
				LAND_CLEAR,
				LAND_ROAD,
				LAND_COUNT,
			};

			return(_land[image->TileType % LAND_COUNT]);
		}
	}
	return(LAND_CLEAR);
}


/***********************************************************************************************
 * TemplateTypeClass::From_Name -- Determine template from ASCII name.                         *
 *                                                                                             *
 *    This routine is used to determine the template number given only                         *
 *    an ASCII representation. The scenario loader uses this routine                           *
 *    to construct the map from the INI control file.                                          *
 *                                                                                             *
 * INPUT:   name  -- Pointer to the ASCII name of the template.                                *
 *                                                                                             *
 * OUTPUT:  Returns with the template number. If the name had no match,                        *
 *          then returns with TEMPLATE_NONE.                                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
TemplateType TemplateTypeClass::From_Name(char const * name)
{
	if (name != NULL) {
		for (TemplateType index = TEMPLATE_FIRST; index < TEMPLATE_COUNT; index++) {
			if (stricmp(As_Reference(index).IniName, name) == 0) {
				return(index);
			}
		}
	}
	return(TEMPLATE_NONE);
}

/***********************************************************************************************
 * TemplateTypeClass::Occupy_List -- Determines occupation list.                               *
 *                                                                                             *
 *    This routine is used to examine the template map and build an                            *
 *    occupation list. This list is used to render a template cursor as                        *
 *    well as placement of icon numbers.                                                       *
 *                                                                                             *
 * INPUT:   placement   -- Is this for placement legality checking only? The normal condition  *
 *                         is for marking occupation flags.                                    *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the template occupation list.                            *
 *                                                                                             *
 * WARNINGS:   The return pointer is valid only until the next time that                       *
 *             this routine is called.                                                         *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1994 JLB : Created.                                                                 *
 *   12/12/1995 JLB : Optimized for direct access to iconset data.                             *
 *=============================================================================================*/
short const * TemplateTypeClass::Occupy_List(bool) const
{
	static short _occupy[13*8+5];
	short	* ptr;

	IsoTile * iconset = (IsoTile *)Get_Image_Data();
	//unsigned char const * map = iconset->Map_Data();

	ptr = &_occupy[0];
	for (int index = 0; index < iconset->NumTiles(); index++) {
		//if (*map++ != 0xFF) {
		//	*ptr++ = (index % Width) + ((index / Width)*MAP_CELL_W);
		//}
		IsoTileImageHeader* tileimg = iconset->GetTileInfo(index);
		if (tileimg == NULL)
			continue;

		*ptr++ = (index % Width) + ((index / Width) * MAP_CELL_W);
	}
	*ptr = REFRESH_EOL;

	return((short const *)&_occupy[0]);
}


/***********************************************************************************************
 * TemplateTypeClass::Init -- Loads graphic data for templates.                                *
 *                                                                                             *
 *    This routine loads the template graphic data for all the template                        *
 *    type supported for the specified theater. This routine is called                         *
 *    whenever the theater for the scenario is first determined.                               *
 *                                                                                             *
 * INPUT:   theater  -- The theater that the template data is to be                            *
 *                      loaded for.                                                            *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   This routine goes to disk!                                                      *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1994 JLB : Created.                                                                 *
 *   06/02/1994 JLB : Only handles iconset loading now (as it should).                         *
 *=============================================================================================*/
void TemplateTypeClass::Init(TheaterType theater)
{
	char fullname[_MAX_FNAME+_MAX_EXT];	// Fully constructed iconset name.
	void const * ptr;		// Working loaded iconset pointer.

	void *terrainPalette = (void *)MFCD::Retrieve(Theaters[theater].IsoPalette);

	for (TemplateType index = TEMPLATE_FIRST; index < TEMPLATE_COUNT; index++) {
		TemplateTypeClass& tplate = As_Reference(index);

		((void const *&)tplate.ImageData) = NULL;
		if (tplate.Theater & (1<<theater)) {
			_makepath(fullname, NULL, NULL, tplate.IniName, Theaters[theater].Suffix);
			ptr = MFCD::Retrieve(fullname);
			((void const *&)tplate.ImageData) = ptr;
#ifdef WIN32
			Register_Icon_Set((void*)ptr, TRUE);		//Register icon set for video memory caching
#endif

			((unsigned char &)tplate.Width) = Get_IconSet_MapWidth(ptr);
			((unsigned char &)tplate.Height) = Get_IconSet_MapHeight(ptr);

			// Try and load HD assets first.
			Image_t *hdImage = Load_StampHD(theater, fullname, tplate.ImageData);
			if (hdImage)
			{
				//Get_Stamp_Size(tplate.ImageData, hdImage->renderwidth, hdImage->renderheight);
				hdImage->isoTileInfo = (IsoTile *)tplate.ImageData;
				((void const*&)tplate.HDImageData) = hdImage;
			}
			else
			{
				// If no HD assets, load the legacy assets into a texture.
				char tmp[512];
				sprintf(tmp, "icon_%s", fullname);
				((void const*&)tplate.HDImageData) = Load_Stamp(fullname, tplate.ImageData, terrainPalette);
			}

		}		
	}


}


#ifdef SCENARIO_EDITOR
/***********************************************************************************************
 * TemplateTypeClass::Display -- Displays a generic representation of template.                *
 *                                                                                             *
 *    This routine is used to display a generic view of the template                           *
 *    object. This is necessary for selection in the scenario editor.                          *
 *                                                                                             *
 * INPUT:   x,y   -- The coordinates to center the display about.                              *
 *                                                                                             *
 *          window-- The window to base the coordinates upon.                                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void TemplateTypeClass::Display(int x, int y, WindowNumberType window, HousesType ) const
{
	int	w,h;
	int	index;
	bool	scale;		// Should the template be half sized?

	Image_t* img = Get_HDImage_Data();
	if (img == NULL)
		return;

	w = Bound(img->width, 1, 13);
	h = Bound(img->height, 1, 8);
	//scale = (w > 3 || h > 3);
	//if (scale) {
	//	x -= (w * ICON_PIXEL_W) / 4;
	//	y -= (h * ICON_PIXEL_H) / 4;
	//} else {
	//	x -= (w * ICON_PIXEL_W) / 2;
	//	y -= (h * ICON_PIXEL_H) / 2;
	//}
	//x += WindowList[window][WINDOWX];
	//y += WindowList[window][WINDOWY];

	//IconsetClass const * iconset = (IconsetClass const *)Get_Image_Data();
	//unsigned char const * map = iconset->Map_Data();

	for (index = 0; index < img->isoTileInfo->NumTiles(); index++) {
		IsoTileImageHeader* isoImage = img->isoTileInfo->GetTileInfo(index);
		if (isoImage == NULL)
			continue;

		GL_RenderForeGroundImage(img, x + isoImage->TileX, y + isoImage->TileY, ICON_PIXEL_W, ICON_PIXEL_H, 0, index);	
	}
}


/***********************************************************************************************
 * TemplateTypeClass::Prep_For_Add -- Prepares to add template to scenario.                    *
 *                                                                                             *
 *    This routine prepares a list of template objects so that the                             *
 *    scenario editor can use this list to display a dialog box. The                           *
 *    selection of a template object will allow its placement upon the                         *
 *    map.                                                                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/23/1994 JLB : Created.                                                                 *
 *   05/28/1994 JLB : Only handles real templates now.                                         *
 *   06/04/1994 JLB : Uses map editing interface functions.                                    *
 *=============================================================================================*/
void TemplateTypeClass::Prep_For_Add(void)
{
	for (TemplateType index = TEMPLATE_CLEAR1; index < TEMPLATE_COUNT; index++) {
		// jmarshall: get hdimage data. 
		if (As_Reference(index).Get_HDImage_Data()) {
			Map.Add_To_List(&As_Reference(index));
		}
	}
}
#endif


/***********************************************************************************************
 * TemplateTypeClass::Create_And_Place -- Creates and places a template object on the map.     *
 *                                                                                             *
 *    This support routine is used by the scenario editor to add a template object to the map  *
 *    and to the game.                                                                         *
 *                                                                                             *
 * INPUT:   cell  -- The cell to place the template object.                                    *
 *                                                                                             *
 * OUTPUT:  bool; Was the template object placed successfully?                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   05/28/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
bool TemplateTypeClass::Create_And_Place(CELL cell, HousesType ) const
{
	if (new TemplateClass(Type, cell)) {
		return(true);
	}
	return(false);
}


/***********************************************************************************************
 * TemplateTypeClass::Create_One_Of -- Creates an object of this template type.                *
 *                                                                                             *
 *    This routine will create an object of this type. For certain template objects, such      *
 *    as walls, it is actually created as a building. The "building" wall is converted into    *
 *    a template at the moment of placing down on the map.                                     *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the appropriate object for this template type.           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/18/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
ObjectClass * TemplateTypeClass::Create_One_Of(HouseClass *) const
{
	return(new TemplateClass(Type, -1));
}


/***********************************************************************************************
 * TemplateTypeClass::One_Time -- Performs one-time initialization                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   08/12/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void TemplateTypeClass::One_Time(void)
{
}


/***********************************************************************************************
 * TemplateTypeClass::As_Reference -- Fetches a reference to the template specified.           *
 *                                                                                             *
 *    This will return a reference to the TemplateTypeClass requested.                         *
 *                                                                                             *
 * INPUT:   type  -- The template type to fetch a reference to.                                *
 *                                                                                             *
 * OUTPUT:  Returns with a reference to the template type class specified.                     *
 *                                                                                             *
 * WARNINGS:   Be sure to pass a valid type parameter. This routine doesn't check it for       *
 *             legality.                                                                       *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
TemplateTypeClass & TemplateTypeClass::As_Reference(TemplateType type)
{
	return(*TemplateTypes.Ptr(type));
}


COORDINATE TemplateTypeClass::Coord_Fixup(COORDINATE coord) const
{
	return Coord_Whole(coord);
}
