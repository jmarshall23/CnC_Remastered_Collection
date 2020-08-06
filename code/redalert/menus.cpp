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

/* $Header: /CounterStrike/MENUS.CPP 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : MENUS.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Phil W. Gorrow                                               *
 *                                                                                             *
 *                   Start Date : September 10, 1993                                           *
 *                                                                                             *
 *                  Last Update : Oct. 24, 1996 Victor Grippi                                  *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Main_Menu -- Menu processing                                                              *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "imgui.h"
#include "function.h"
#ifdef WIN32
#include "ccdde.h"
#else	//WIN32
#include	<sys\timeb.h>
#endif

#include <sdl.h>

// this makes buttons larger on a larger screen
// necessary??

//#undef RESFACTOR
//#define RESFACTOR (2 + (ScreenWidth / 640))

/*****************************
**	Function prototypes
******************************/

PRIVATE int Coordinates_In_Region(int x, int y, int inx1, int iny1, int inx2, int iny2);
PRIVATE int Select_To_Entry(int select, unsigned long bitfield, int index);
PRIVATE void Flash_Line(char const *text, int xpix, int ypix, unsigned nfgc, unsigned hfgc, unsigned bgc);

int UnknownKey;

PRIVATE int MenuUpdate=1;
PRIVATE int MenuSkip;

#ifdef FIXIT_VERSION_3
#include "WolStrng.h"
#endif

/*=========================================================================*/
/*	SELECT_TO_ENTRY:																			*/
/*																									*/
/*		This routine converts a selection to the correct string entry. It	   */
/*	does this by search through a long bitfield starting at position index	*/
/*	until it finds the correct conversion to entries.								*/
/*																									*/
/*	INPUTS:	int selection from menu, long the bit field to search, int 	   */
/*				the starting index within the bit field.								*/
/*	RETURNS:	int the index into the table of entries								*/
/*=========================================================================*/
PRIVATE int Select_To_Entry(int select, unsigned long bitfield, int index)
{
	int placement;

	if (bitfield==0xFFFFFFFFL) 							/* if all bits are set	*/
		return(select);										/*		then it as is		*/

	placement=0;												/* current pos zero		*/
	while (select) {											/* while still ones		*/
		if (bitfield & (1L<<(placement+index)))			/* if this flagged then	*/
			select--;											/* decrement counter		*/
		placement++;											/* and we moved a place	*/
	}
	while (!(bitfield & (1L<<(placement+index)))) {
		placement++;
	}

	return(placement);										/* return the position	*/
}


/*=========================================================================*/
/*	FLASH_LINE:																					*/
/*																									*/
/*		This routine will flash the line at the desired location for the		*/
/*	menu routine. It is way cool awesome!												*/
/*																									*/
/*	INPUTS:	char *text, int x position on line, int y position, char		   */
/*				normal foreground color, char hilight foreground color, char 	*/
/*				background color																*/
/*	RETURNS:	none																				*/
/*=========================================================================*/
PRIVATE void Flash_Line(char const *text, int xpix, int ypix, unsigned nfgc, unsigned hfgc, unsigned bgc)
{
	int loop;

	for (loop=0;loop<3;loop++) {
		Hide_Mouse();
		Plain_Text_Print(text, xpix, ypix, hfgc, bgc, TPF_8POINT|TPF_DROPSHADOW);
		Delay(2);
		Plain_Text_Print(text, xpix, ypix, nfgc, bgc, TPF_8POINT|TPF_DROPSHADOW);
		Show_Mouse();
		Delay(2);
	}
}

/*=========================================================================*/
/*	COORDINATES_IN_REGION:																	*/
/*																									*/
/*		Test to see if a given pair of coordinates are within the given 		*/
/*	rectangular region.																		*/
/*																									*/
/*	INPUTS:	int x to be tested, int y to be tested, int left x pos,			*/
/*				int top y pos, int right x pos, int bottom y pos					*/
/*	RETURNS:	none																				*/
/*=========================================================================*/
PRIVATE int Coordinates_In_Region(int x, int y, int inx1, int iny1, int inx2, int iny2)
{
	return((x>=inx1)&&(x<=inx2)&&(y>=iny1)&&(y<=iny2));
}

#ifdef NEVER
/*=========================================================================*/
/*	FIND_MENU_ITEMS:																			*/
/*																									*/
/*		This routine finds the real total items in a menu when certain items	*/
/*	may be disabled by bit fields and the like. This is done by looping		*/
/*	through the fields, starting at the position passed in index and 			*/
/*	counting the number of bits that are set.											*/
/*																									*/
/*	INPUTS:	int the maximum number of items possible on the menu, long 		*/
/*				the bit field of enabled and disabled items, char the index		*/
/*				point to start at within the list.										*/
/*	RETURNS:	int the total number of items in the menu							*/
/*=========================================================================*/
 int Find_Menu_Items(int maxitems, unsigned long field, char index)
 {
	int loop,ctr;

	if (field==0xFFFFFFFFL) 										/* if all bits are set	*/
		return(maxitems);										/* then maxitems set		*/

	for (loop=ctr=0;loop<maxitems;loop++) {			/* loop through items	*/
		if (field & (1L<<(loop+index))) {				/* if the bit is set		*/
			ctr++;												/*		count the item		*/
		}
	}
	return(ctr);
}
#endif


/*=========================================================================*/
/*	SETUP_EOB_MONITOR_MENU:																	*/
/*																									*/
/*		This routine sets up the eye of the beholder monitor menu.				*/
/*																									*/
/*	INPUTS:	int the menu we are using, char *[] the array of text which		*/
/*				makes up the menu commands, long the info field, int the			*/
/*				index into the field, int the number of lines to skip.			*/
/*	RETURNS:	none																				*/
/*=========================================================================*/
void Setup_Menu(int menu, char const * text[], unsigned long field, int index, int skip)
{
	int * menuptr,lp;
	int menuy,menux,idx,item,num,drawy;

	menuptr=&MenuList[menu][0];							/* get pointer to menu	*/
	menuy=WinY+menuptr[MENUY];								/* get the absolute 		*/
	menux=(WinX+menuptr[MENUX]);						/*		coords of menu		*/
	item=Select_To_Entry(menuptr[MSELECTED], field, index);
	num=menuptr[ITEMSHIGH];

	Plain_Text_Print(0, 0, 0, TBLACK, TBLACK, TPF_8POINT|TPF_DROPSHADOW);
 	Hide_Mouse();
	for (lp=0;lp<num;lp++) {
		idx=Select_To_Entry(lp, field, index);
		drawy=menuy+(lp*FontHeight)+(lp*skip);
		Plain_Text_Print(text[idx], menux, drawy, menuptr[((idx==item) && (MenuUpdate )) ? HILITE : NORMCOL], TBLACK, TPF_8POINT|TPF_DROPSHADOW);
//		if ((idx==item) && (MenuUpdate ))
//			Text_Print(text[idx], menux, drawy, menuptr[HILITE], TBLACK);
	}
	MenuSkip=skip;
	Show_Mouse();
	Keyboard->Clear();
}


/*=========================================================================*/
/*	CHECK_MENU:																					*/
/*																									*/
/*																									*/
/*																									*/
/*	INPUTS:																						*/
/*	RETURNS:																						*/
/*=========================================================================*/
int Check_Menu(int menu, char const * text[], char *, long field, int index, KeyNumType _key)
{
	int maxitem,select,key,menuy,menux;
	int mx1,mx2,my1,my2,tempy;
	int drawy,menuskip,halfskip;
	int normcol,litcol,item,newitem,idx;
	int * menuptr;

	//selection++;												/* get rid of warning	*/

	menuptr = &MenuList[menu][0];							/* get pointer to menu	*/
	maxitem = menuptr[ITEMSHIGH]-1;							/* find max items			*/
	newitem = item = menuptr[MSELECTED]%(maxitem+1);	/* find selected 			*/
	select = -1;													/* no selection made		*/
	menuskip = FontHeight+MenuSkip;							/* calc new font height	*/
	halfskip = MenuSkip>>1;									/* adjustment for menus	*/

	menuy = WinY+menuptr[MENUY];								/* get the absolute 		*/
	menux = (WinX+menuptr[MENUX]);						/*		coords of menu		*/
	normcol = menuptr[NORMCOL];
	litcol = menuptr[HILITE];

	/*
	**	Fetch a pending keystroke from the buffer if there is a keystroke
	**	present. If no keystroke is pending then simple mouse tracking will
	**	be done.
	*/
	if (_key == KN_NONE)
	{
		key = 0;
		UnknownKey = 0;
		if (Keyboard->Check()) {
			key = (Keyboard->Get() & ~(WWKEY_SHIFT_BIT | WWKEY_ALT_BIT | WWKEY_CTRL_BIT));			/* mask off all but release bit	*/
		}
	}
	else {
		key = _key;
	}

	/*
	**	if we are using the mouse and it is installed, then find the mouse
	**	coordinates of the menu and if we are not somewhere on the menu get
	**	the heck outta here. If we are somewhere on the menu, then figure
	**	out the new selected item, and continue forward.
	*/
	mx1=(WinX)+(menuptr[MENUX]*FontWidth);		/* get menu coords		*/
	my1=(WinY)+(menuptr[MENUY])-halfskip;			/*		from the menu		*/
	mx2=mx1+(menuptr[ITEMWIDTH]*FontWidth)-1;		/*		structure as		*/
	my2=my1+(menuptr[ITEMSHIGH]*menuskip)-1;		/*		necessary			*/

		tempy=UserInput.Mouse.Y;
		if (Coordinates_In_Region(UserInput.Mouse.X, tempy, mx1, my1, mx2, my2)&& MenuUpdate) {
			newitem=(tempy-my1)/menuskip;
		}

	switch (key) {

		case KN_UP:												/* if the key moves up	*/
			newitem--;											/* 	new item up one	*/
			if (newitem<0) 									/* if invalid new item	*/
				newitem=maxitem;								/* put at list bottom	*/
			break;
		case KN_DOWN:											/* if key moves down		*/
			newitem++;											/*		new item down one	*/
			if (newitem>maxitem) 							/* if new item past 		*/
				newitem=0;										/*		list end, clear	*/
			break;
		case KN_HOME:											/* if top of list key 	*/
		case KN_PGUP:											/*		is selected then	*/
			newitem=0;											/*		new item = top		*/
			break;
		case KN_END:											/* if bottom of list is	*/
		case KN_PGDN:											/*		selected then		*/
			newitem=maxitem;									/*		new item = bottom	*/
			break;

		/*
		**	Handle mouse button press. Set selection and then fall into the
		**	normal menu item select logic.
		*/
		case KN_RMOUSE:
		case KN_LMOUSE:
			if (Coordinates_In_Region(UserInput.Mouse.X, UserInput.Mouse.Y, mx1, my1, mx2, my2)) {
				newitem = (UserInput.Mouse.Y - my1) / menuskip;
			} else {
				UnknownKey = key;			//	Pass the unprocessed button click back.
				break;
			}

		/*
		**	Normal menu item select logic. Will flash line and exit with menu
		**	selection number.
		*/
		case KN_RETURN:										/* if a selection is 	*/
		case KN_SPACE:											/*		made with key		*/
		case KN_CENTER:
			select=newitem;									/*		flag it made.		*/
			break;

		case 0:
			break;

		/*
		**	When no key was pressed or an unknown key was pressed, set the
		**	global record of the key and exit normally.
		**	EXCEPTION:	If the key matches the first letter of any of the
		**					menu entries, then presume it as a selection of
		**					that entry.
		*/
		default:
			for (idx = 0; idx < menuptr[ITEMSHIGH]; idx++) {
				if (toupper(*(text[Select_To_Entry(idx, field, index)])) == toupper(Keyboard->To_ASCII((KeyNumType)(key&0x0FF)))) {
					newitem = select = idx;
					break;
				}
			}
			UnknownKey = key;
			break;
	}

	if (newitem!=item) {
		Hide_Mouse();
		idx=Select_To_Entry(item, field, index);
		drawy=menuy+(item*menuskip);
		Plain_Text_Print(text[idx], menux, drawy, normcol, TBLACK, TPF_8POINT|TPF_DROPSHADOW);
		idx=Select_To_Entry(newitem, field, index);
		drawy=menuy+(newitem*menuskip);
		Plain_Text_Print(text[idx], menux, drawy, litcol, TBLACK, TPF_8POINT|TPF_DROPSHADOW);
		Show_Mouse();												/* resurrect the mouse	*/
	}

	if (select!=-1) {
		idx=Select_To_Entry(select, field, index);
		Hide_Mouse();												/* get rid of the mouse	*/
		drawy=menuy+(newitem*menuskip);
		Flash_Line(text[idx], menux, drawy, normcol, litcol, TBLACK);
		Show_Mouse();
		select=idx;
	}

	menuptr[MSELECTED]=newitem;							/* update menu select	*/

	return(select);
}


/***************************************************************************
 * Do_Menu -- Generic menu processor.                                      *
 *                                                                         *
 *    This helper function displays a menu of specified entries and waits  *
 *    for the player to make a selection. If a selection is made, then     *
 *    a whole number (starting at 0) is returned matching the entry        *
 *    selected. If ESC is pressed, then -1 is returned.                    *
 *                                                                         *
 * INPUT:   strings  -- A pointer to an array of pointers to text strings. *
 *                      Each entry in the list will be a menu entry that   *
 *                      can be selected.                                   *
 *                                                                         *
 *          blue     -- Should the special blue color be used to display   *
 *                      the menu?                                          *
 *                                                                         *
 * OUTPUT:  Returns with the cardinal number of the selected menu entry.   *
 *          If ESC was pressed, then -1 is returned.                       *
 *                                                                         *
 * WARNINGS:   none                                                        *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/16/1994 JLB : Created.                                             *
 *=========================================================================*/
int Do_Menu(char const ** strings, bool )
{
	int	count;		// Number of entries in this menu.
	int	length;		// The width of the menu (in pixels).
	char	const ** ptr;		// Working menu text pointer.
	int	selection;	// Selection from user.

	if (!strings) return(-1);
	Set_Logic_Page(SeenBuff);
	Keyboard->Clear();

	/*
	**	Determine the number of entries in this string.
	*/
	ptr = strings;
	count = 0;
	while (*ptr++) {
		count++;
	}
	MenuList[0][ITEMSHIGH] = count;

	/*
	**	Determine the width of the menu by finding the length of the
	**	longest menu entry.
	*/
	Plain_Text_Print(TXT_NONE, 0, 0, 0, 0, TPF_8POINT|TPF_DROPSHADOW);
	length = 0;
	ptr = strings;
	while (*ptr) {
		length = max(length, (int)String_Pixel_Width(*ptr));
		ptr++;
	}
	length += 7;
	MenuList[0][ITEMWIDTH] = length >> 3;

	/*
	**	Adjust the window values to match the size of the
	**	specified menu.
	*/
	WindowList[WINDOW_MENU][WINDOWWIDTH] = (MenuList[0][ITEMWIDTH] + 2) * 8;
	WindowList[WINDOW_MENU][WINDOWX] = ScreenWidth - ((19 - (length >> 4)) * 8);
	WindowList[WINDOW_MENU][WINDOWY] = 174 - (unsigned)(MenuList[0][ITEMSHIGH] * (FontHeight+FontYSpacing));
	WindowList[WINDOW_MENU][WINDOWHEIGHT] = MenuList[0][ITEMSHIGH] * FontHeight + 5 /*11*/;

	/*
	**	Display the menu.
	*/
	Change_Window((int)WINDOW_MENU);
	Show_Mouse();
	Window_Box(WINDOW_MENU, BOXSTYLE_RAISED);
	Setup_Menu(0, strings, 0xFFFFL, 0, 0);

	Keyboard->Clear();
	selection = -1;
	UnknownKey = 0;

	// AI options main selection loop
	while (selection == -1) {
		Call_Back();

		g_globalKeyNumType = KN_NONE;
		g_globalKeyFlags = 0;
		UserInput.Process_Input();

		Device_Present();

		Show_Mouse(true);

		selection = Check_Menu(0, strings, NULL, 0xFFL, 0, (KeyNumType)g_globalKeyFlags);
		if ( UnknownKey != 0 || UnknownKey == KN_ESC || UnknownKey==KN_LMOUSE || UnknownKey==KN_RMOUSE) break;
	}

	Keyboard->Clear();
	Hide_Mouse();

//	HidPage.Blit(SeenPage);
//WindowList[WINDOW_MAIN][2] = SeenBuff.Get_Width();//BG
	Change_Window((int)WINDOW_MAIN);
	Map.Flag_To_Redraw(true);
	return(selection);
}


/***************************************************************************
 * Main_Menu -- Menu processing                                            *
 *                                                                         *
 * INPUT:                                                                  *
 *		none.																						*
 *                                                                         *
 * OUTPUT:                                                                 *
 *		index of item selected, -1 if time out											*
 *                                                                         *
 * WARNINGS:                                                               *
 *		none.																						*
 *                                                                         *
 * HISTORY:                                                                *
 *   05/17/1995 BRR : Created.                                             *
 *=========================================================================*/
int Main_Menu(unsigned long )
{
	/*
	**	Dialog & button dimensions
	*/
	int	d_dialog_w = 152 * RESFACTOR;
	int	d_dialog_h = 100 * RESFACTOR;

	int	d_dialog_x = ((SeenBuff.Get_Width() - d_dialog_w) / 2);
	int	d_dialog_y = ((SeenBuff.Get_Height() - d_dialog_h) / 2);

	int d_button_w = 140 * RESFACTOR;
	int d_button_h = 11 * RESFACTOR;
	int d_button_dy = d_button_h + 4 * RESFACTOR;		// spacing between top of two buttons
	int d_button_x = d_dialog_x + ( ( d_dialog_w - d_button_w ) / 2 );
	int d_button_y = d_dialog_y + (24 * RESFACTOR);

	static int	max_buttons = 7;

	g_loadingPlague = false;

	/*
	**	Button enumerations:
	*/
	//	Enums in Select_Game() must match order of buttons in Main_Menu().
	enum {
		BUTTON_EXPAND=100,			//	(CS)
		BUTTON_EXPAND_AM,
		BUTTON_START,
		BUTTON_LOAD,
		BUTTON_MULTI,
		BUTTON_INTRO,
		BUTTON_EXIT,
	};

	/*
	**	Dialog variables:
	*/
	bool bExpansionCS = Expansion_CS_Present();
	bool bExpansionAM = Expansion_AM_Present();

	KeyNumType input;								// input from user
	int retval;										// return value
	int curbutton;
	TextButtonClass * buttons[7];
	unsigned long starttime;

	/*
	**	Buttons
	*/
	ControlClass * commands = NULL;				// the button list

	TextButtonClass startbtn(BUTTON_START, TXT_START_NEW_GAME, TPF_BUTTON, d_button_x, d_button_y, d_button_w, d_button_h);
	d_button_y += d_button_dy;

	TextButtonClass loadbtn(BUTTON_LOAD, TXT_LOAD_MISSION, TPF_BUTTON, d_button_x, d_button_y, d_button_w, d_button_h);
	d_button_y += d_button_dy;

	TextButtonClass multibtn(BUTTON_MULTI, TXT_MULTIPLAYER_GAME, TPF_BUTTON, d_button_x, d_button_y, d_button_w, d_button_h);
	d_button_y += d_button_dy;
// jmarshall - Intro button is going to be the scenario editor button.
#ifdef SCENARIO_EDITOR
	TextButtonClass introbtn(BUTTON_INTRO, "Scenario Editor", TPF_BUTTON, d_button_x, d_button_y, d_button_w, d_button_h);
#else
	TextButtonClass introbtn(BUTTON_INTRO, TXT_INTRO, TPF_BUTTON, d_button_x, d_button_y, d_button_w, d_button_h);
#endif
// jmarshall end
	d_button_y += d_button_dy;

	TextButtonClass exitbtn(BUTTON_EXIT, TXT_EXIT_GAME, TPF_BUTTON, d_button_x, d_button_y, d_button_w, d_button_h);
	d_button_y += d_button_dy;

	int version_x = d_button_x + d_button_w;
	int version_y = d_button_y + ( 5 * RESFACTOR );

	/*
	**	Initialize
	*/
	if (RequiredCD != -2) {
		RequiredCD = -1;
		Force_CD_Available(RequiredCD);
	}
	Set_Logic_Page(SeenBuff);
	Keyboard->Clear();
	starttime = TickCount;

	/*
	**	Create the list
	*/
	commands = &startbtn;

	loadbtn.Add_Tail(*commands);
	multibtn.Add_Tail(*commands);
	introbtn.Add_Tail(*commands);
	exitbtn.Add_Tail(*commands);

	/*
	**	Fill array of button ptrs
	*/
	curbutton = 0;

	buttons[0] = &startbtn;
	buttons[1] = &loadbtn;
	buttons[2] = &multibtn;
	buttons[3] = &introbtn;
	buttons[4] = &exitbtn;

	buttons[0]->Turn_On();

	Keyboard->Clear();

	Fancy_Text_Print(TXT_NONE, 0, 0, GadgetClass::Get_Color_Scheme(), TBLACK, TPF_CENTER|TPF_6PT_GRAD|TPF_USE_GRAD_PAL|TPF_NOSHADOW);

	fixed oldvolume = Options.ScoreVolume;
	if (oldvolume == 0) {
		Options.Set_Score_Volume(fixed(4, 10), false);
	}
	Theme.Play_Song(THEME_INTRO);

	/*
	**	Main Processing Loop.
	*/
	bool display = true;
	bool process = true;
	while (process) {
		/*
		** If we have just received input focus again after running in the background then
		** we need to redraw.
		*/
		if (AllSurfaces.SurfacesRestored) {
			AllSurfaces.SurfacesRestored=FALSE;
			display = true;
		}

		/*
		**	Invoke game callback.
		*/
		Call_Back();

		/*
		**	Load the background picture.
		*/
		Load_Title_Page();
		CCPalette.Set();

		/*
		**	Display the title and text overlay for the menu.
		*/
		Set_Logic_Page(HidPage);
		commands->Draw_All();

		Fancy_Text_Print("V%s", 
			version_x,
			version_y,
			GadgetClass::Get_Color_Scheme(), TBLACK,
			TPF_EFNT|TPF_NOSHADOW|TPF_RIGHT,
			Version_Name());

		/*
		**	Copy the menu to the visible page.
		*/
		Hide_Mouse();
//		HidPage.Blit(SeenPage);
		Show_Mouse();

		Set_Logic_Page(SeenBuff);
		display = false;

		/*
		**	Get and process player input.
		*/		
		input = (KeyNumType)0;
		input = commands->Input();

		/*
		**	If there is input, then take this opportunity to seed some bits
		**	to the cryptographic random number generator.
		*/
		if (input != 0) {
			SYSTEMTIME t;
			GetSystemTime(&t);
			CryptRandom.Seed_Byte(t.wMilliseconds);
		}

		/*
		**	Dispatch the input to be processed.
		*/
		switch (input) {
			case (BUTTON_START | KN_BUTTON):
				retval = (input & 0x7FFF) - BUTTON_EXPAND;
				process = false;
				break;

			case (BUTTON_LOAD | KN_BUTTON):
				retval = (input & 0x7FFF) - BUTTON_EXPAND;
				process = false;
				break;

			case (BUTTON_MULTI | KN_BUTTON):
				retval = (input & 0x7FFF) - BUTTON_EXPAND;
				process = false;
				break;

			case (BUTTON_INTRO | KN_BUTTON):
				retval = (input & 0x7FFF) - BUTTON_EXPAND;
				process = false;
				break;

			case (BUTTON_EXIT | KN_BUTTON):
				retval = (input & 0x7FFF) - BUTTON_EXPAND;
				process = false;
				break;

			case KN_UP:
				buttons[curbutton]->Turn_Off();
				buttons[curbutton]->Flag_To_Redraw();
				curbutton--;

				switch( curbutton )
				{
				case -1:
					curbutton = max_buttons - 1;
					break;
				case 0:
					if( !bExpansionCS )
						curbutton = max_buttons - 1;
					break;
				case 1:
					if( !bExpansionAM )
					{
						if( bExpansionCS )
							curbutton = 0;
						else
							curbutton = max_buttons - 1;
					}
					break;
				}

				buttons[curbutton]->Turn_On();
				buttons[curbutton]->Flag_To_Redraw();
				break;

			case KN_DOWN:
				buttons[curbutton]->Turn_Off();
				buttons[curbutton]->Flag_To_Redraw();
				curbutton++;

				if( curbutton == max_buttons )
				{
					if( bExpansionCS )
						curbutton = 0;
					else if( bExpansionAM )
						curbutton = 1;
					else
						curbutton = 2;
				}
				else if (curbutton == 1 && !bExpansionAM)
				{
					curbutton = 2;
				}

				buttons[curbutton]->Turn_On();
				buttons[curbutton]->Flag_To_Redraw();
				break;

			case KN_RETURN:
				buttons[curbutton]->IsPressed = true;
				buttons[curbutton]->Draw_Me(true);
				retval = curbutton;
				process = false;
				break;			

			default:
				break;
		}

		//Device_Present();
	}

	Options.Set_Score_Volume(oldvolume, false);

	return(retval);
}
