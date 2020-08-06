#include "function.H"
#include "mapscript.h"

/**********************************************************************************************
* Red Alert Vanilla Actions                                                                   *
*=============================================================================================*/

static int Script_ToggleFireSale(lua_State* L) {

    int houseType = lua_tointeger(L, 1);
    int fireSaleEnabled = lua_tointeger(L, 2);

    if (houseType != HOUSE_NONE) {

        HouseClass* this_house = HouseClass::As_Pointer((HousesType)houseType);

        if (this_house != NULL) {
            this_house->ToggleFireSaleAbility(fireSaleEnabled != 0);
        }
    }

    return 1;
}


/***********************************************************************************************
 * Script_Win - The specified player wins                                                      *
 *                                                                                             *
 *   SCRIPT INPUT:	houseType (int) - The player (house that wins)                             *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_Win(lua_State* L) {

    int houseType = lua_tointeger(L, -1);

    if (houseType != HOUSE_NONE) {

        HouseClass* this_house = HouseClass::As_Pointer((HousesType)houseType);

        if (this_house != NULL) {

            if (this_house->ID == PlayerPtr->Class->House) {
                PlayerPtr->Flag_To_Win();
            }
            else {
                PlayerPtr->Flag_To_Lose();
            }

            bool success = this_house->Fire_Sale();
            lua_pushboolean(L, int(success));
        }
    }

    return 1;
}

/***********************************************************************************************
 * Script_Lose - The specified player loses                                                    *
 *                                                                                             *
 *   SCRIPT INPUT:	ret (int) - The waypoint index of which to reveal                          *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_Lose(lua_State* L) {

    int houseType = lua_tointeger(L, -1);

    if (houseType != HOUSE_NONE) {

        HouseClass* this_house = HouseClass::As_Pointer((HousesType)houseType);

        if (this_house != NULL) {

            if (this_house->ID == PlayerPtr->Class->House) {
                PlayerPtr->Flag_To_Lose();
            }
            else {
                PlayerPtr->Flag_To_Win();
            }

            lua_pushboolean(L, 1);
        }
    }

    return 1;
}

/***********************************************************************************************
 * Script_BeginProduction - This will enable production to begin for the house specified       *
 *                                                                                             *
 *   SCRIPT INPUT:	houseType (int) - The AI player (house) index to allow winning for         *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_BeginProduction(lua_State* L) {

    int houseType = lua_tointeger(L, -1);

    if (houseType != HOUSE_NONE) {

        HouseClass* this_house = HouseClass::As_Pointer((HousesType)houseType);

        if (this_house != NULL) {
            this_house->Begin_Production();
        }
    }

    return 1;
}

/***********************************************************************************************
 * Script_CreateTeam - Attempts to create team (as defined in map)                             *
 *                                                                                             *
 *   SCRIPT INPUT:	teamType (int) - The team index to attempt to create                       *
 *                                                                                             *
 *   SCRIPT OUTPUT:  success (bool) - Did a team get created?                                  *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_CreateTeam(lua_State* L) {

    const char* teamType = lua_tostring(L, 1);

    TeamTypeClass* teamPtr = TeamTypeClass::From_Name(teamType);

    if (teamPtr != NULL) {
        ScenarioInit++;

        bool success = false;

        TeamClass* new_team = teamPtr->Create_One_Of();

        if (new_team != NULL) {
            success = true;
        }

        lua_pushboolean(L, int(success));

        ScenarioInit--;
    }

    return 1;
}

/***********************************************************************************************
 * Script_DestroyTeam - Destroy all teams of the type specified                                *
 *                                                                                             *
 *   SCRIPT INPUT:	teamType (int) - The team index to attempt to destroy                      *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_DestroyTeam(lua_State* L) {

    const char* teamType = lua_tostring(L, 1);

    TeamTypeClass* teamPtr = TeamTypeClass::From_Name(teamType);

    if (teamPtr != NULL) {

        teamPtr->Destroy_All_Of();

    }

    return 1;
}

/***********************************************************************************************
 * Script_AllHunt - Force all units of specified house to go into hunt mode                    *
 *                                                                                             *
 *   SCRIPT INPUT:	houseType (int) - The AI player (house) index to allow winning for         *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_AllHunt(lua_State* L) {

    int houseType = lua_tointeger(L, -1);

    if (houseType != HOUSE_NONE) {

        HouseClass* this_house = HouseClass::As_Pointer((HousesType)houseType);

        if (this_house != NULL) {
            this_house->Do_All_To_Hunt();
        }
    }

    return 1;
}

/***********************************************************************************************
 * Script_Reinforcements - Attempts to create reinforcements as defined by team                *
 *                                                                                             *
 *   SCRIPT INPUT:	teamType (int) - The team index to attempt to create                       *
 *                                                                                             *
 *   SCRIPT OUTPUT:  success (bool) - Did Do_Reinforcements return true?                       *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_Reinforcements(lua_State* L) {

    const char* teamType = lua_tostring(L, 1);

    TeamTypeClass* teamPtr = TeamTypeClass::From_Name(teamType);

    if (teamPtr != NULL) {
        bool success = Do_Reinforcements(teamPtr);
        lua_pushboolean(L, int(success));
    }
    else {
        lua_pushboolean(L, 0);
    }

    return 1;
}

/***********************************************************************************************
 * Script_DropZoneFlare - Places drop down smoke at specified waypoint location                *
 *                                                                                             *
 *   SCRIPT INPUT:	in_x (int) - The Cell/X location to drop the smoke                         *
 *                	in_y (int) - The Cell/Y location to drop the smoke                         *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_DropZoneFlare(lua_State* L) {

    int in_x = lua_tointeger(L, 1);
    int in_y = lua_tointeger(L, 2);

    new AnimClass(ANIM_LZ_SMOKE, Cell_Coord(XY_Cell(in_x, in_y)));

    return 1;
}

/***********************************************************************************************
 * Script_FireSale - Make AI house give up, selling everything and going all in on attack      *
 *                                                                                             *
 *   SCRIPT INPUT:	houseType (int) - The AI player (house) index to allow winning for         *
 *                                                                                             *
 *   SCRIPT OUTPUT:  success (bool) - Did the action get performed?                            *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_FireSale(lua_State* L) {

    int houseType = lua_tointeger(L, -1);

    if (houseType != HOUSE_NONE) {

        HouseClass* this_house = HouseClass::As_Pointer((HousesType)houseType);

        if (this_house != NULL) {
            bool success = this_house->Fire_Sale();
            lua_pushboolean(L, int(success));
        }
    }

    return 1;
}

/***********************************************************************************************
 * Script_PlayMovie - Plays the given movie                                                    *
 *                                                                                             *
 *   SCRIPT INPUT:	ret (int) - The movie index of which to play                               *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_PlayMovie(lua_State* L) {
    //int ret = lua_tointeger(L, -1);
    const char* movieName = lua_tostring(L, -1);
    Play_Movie(movieName);
    return 1;
}

/***********************************************************************************************
 * Script_TriggerText - Triggers text to display in-game                                       *
 *                                                                                             *
 *   SCRIPT INPUT:	textIndex (int) - The text index to display (from tutorial.ini)            *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_TriggerText(lua_State* L) {
    int textIndex = lua_tointeger(L, -1);

    // TODO: Implement colors / flags

    // JJQUESTION: This appears right but doesn't do anything. Is text broken at the moment?
    if (textIndex > 0 && textIndex < sizeof(TutorialText)) {

        Session.Messages.Add_Message(NULL, textIndex, (char*)TutorialText[textIndex], PCOLOR_GREEN, TPF_6PT_GRAD | TPF_USE_GRAD_PAL | TPF_FULLSHADOW, Rule.MessageDelay * TICKS_PER_MINUTE);
    }

    return 1;
}

/***********************************************************************************************
 * Script_DestroyTrigger - Destroys a given trigger                                            *
 *                                                                                             *
 *   SCRIPT INPUT:	ret (int) - The index/id of the trigger of which to destroy                *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_DestroyTrigger(lua_State* L) {

    int triggerIndex = lua_tointeger(L, -1);

    TriggerClass* this_trigger = Triggers.Ptr(triggerIndex);

    if (this_trigger != NULL) {
        Detach_This_From_All(this_trigger->As_Target());
        delete Triggers.Ptr(triggerIndex);
    }

    return 1;
}

/***********************************************************************************************
 * Script_AutoCreate - Allows house AI to begin creating autocreate teams                      *
 *                                                                                             *
 *  Autocreation means that instead of a teamtype being                                        *
 *  manually created by a Create Team trigger action, that                                     *
 *  it will be subject to the autocreate function. Each                                        *
 *  computer country can have its autocreation function                                        *
 *  turned on through a trigger action, after which the                                        *
 *  creation of the teamtypes that have autocreation turned                                    *
 *  on will be subject to being created, at the whim of the                                    *
 *  autocreate function. See [TeamTypes] Section for more                                      *
 *  details.                                                                                   *
 *                        - The Red Alert Single Player Mission Creation Guide                 *
 *                                                                                             *
 *                                                                                             *
 *   SCRIPT INPUT:	houseType (int) - The AI player (house) index to allow autocreate for      *
 *                  createEnables (bool, default yes) - Allow autocreate on/off                *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_AutoCreate(lua_State* L) {

    int houseType = 0;
    bool autoCreateEnabled = true;

    houseType = lua_tointeger(L, 1);

    // If there is a second argument given, process it
    if (lua_gettop(L) == 2) {
        autoCreateEnabled = lua_toboolean(L, 2);
    }

    if (houseType != HOUSE_NONE) {

        HouseClass* this_house = HouseClass::As_Pointer((HousesType)houseType);

        if (this_house != NULL) {

            // If we are enabling auto creation by the house, clear alert
            if (autoCreateEnabled) {
                this_house->AlertTime = 0; // TODO - don't assume successful pointer
            }

            // Now set the parameter
            this_house->IsAlerted = autoCreateEnabled;

        }

    }

    return 1;
}

/***********************************************************************************************
 * Script_AllowWin - Allows a win                                                              *
 *                                                                                             *
 *   Used when you want a specific objective                                                   *
 *   to be met before the mission is accomplished.  In other words, if the                     *
 *   trigger with this action assigned to it hasn't been fired, but the                        *
 *   actual WIN trigger has been fired, the mission will not end until this                    *
 *   "allow win" trigger is fired.                                                             *
 *                                                                                             *
 *   SCRIPT INPUT:	houseType (int) - The AI player (house) index to allow winning for         *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_AllowWin(lua_State* L) {

    int houseType = lua_tointeger(L, -1);

    if (houseType != HOUSE_NONE) {

        HouseClass* this_house = HouseClass::As_Pointer((HousesType)houseType);

        if (this_house != NULL) {
            this_house->Blockage++;
        }
    }

    return 1;
}

/***********************************************************************************************
 * Script_RevealAll - Reveals the entire map  TODO: input player                               *
 *                                                                                             *
 *   SCRIPT INPUT:	ret (int) - The waypoint index of which to reveal                          *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_RevealAll(lua_State* L) {
    if (!PlayerPtr->IsVisionary) {
        PlayerPtr->IsVisionary = true;
        for (CELL cell = 0; cell < MAP_CELL_TOTAL; cell++) {
            Map.Map_Cell(cell, PlayerPtr);
        }
    }

    return 1;
}

/***********************************************************************************************
 * Script_RevealCell - Reveals map around given waypoint  TODO: input player                   *
 *                                                                                             *
 *   SCRIPT INPUT:	ret (int) - The waypoint index of which to reveal                          *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_RevealCell(lua_State* L) {
    int ret = lua_tointeger(L, -1);

    if (!PlayerPtr->IsVisionary) {
        Map.Sight_From(Scen.Waypoint[ret], Rule.GapShroudRadius, PlayerPtr, false);
    }

    return 1;
}

/***********************************************************************************************
 * Script_RevealZone - Reveals area of map around given waypoint  TODO: input player           *
 *                                                                                             *
 *   Reveal all cells of the zone that the specified waypoint is located in. This can be       *
 *   used to reveal whole islands or bodies of water                                           *
 *                                                                                             *
 *   SCRIPT INPUT:	ret (int) - The waypoint index of which to reveal                          *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_RevealZone(lua_State* L) {
    int ret = lua_tointeger(L, -1);

    if (!PlayerPtr->IsVisionary) {
        int zone = Map[Scen.Waypoint[ret]].Zones[MZONE_CRUSHER];

        for (CELL cell = 0; cell < MAP_CELL_TOTAL; cell++) {
            if (Map[cell].Zones[MZONE_CRUSHER] == zone) {
                Map.Map_Cell(cell, PlayerPtr);
            }
        }

    }

    return 1;
}

/***********************************************************************************************
 * Script_PlaySound - Plays the given Sound                                                    *
 *                                                                                             *
 *   SCRIPT INPUT:	ret (int) - The sound index of which to play                               *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_PlaySound(lua_State* L) {
    int ret = lua_tointeger(L, -1);

    Sound_Effect((VocType)ret);

    return 1;
}

/***********************************************************************************************
 * Script_PlayMusic - Plays the given Music score.                                             *
 *                                                                                             *
 *   Plays music score at given index. Negative numbers stop any music playing.                *
 *                                                                                             *
 *     SCRIPT INPUT:	ret (int) - The music index of which to play                           *
 *                                                                                             *
 *     SCRIPT OUTPUT:  void                                                                    *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_PlayMusic(lua_State* L) {
    int ret = lua_tointeger(L, -1);

    if (ret < 0) {
        Theme.Stop();
    }
    else {
        Theme.Play_Song((ThemeType)ret);
    }

    return 1;
}

/***********************************************************************************************
 * Script_PlaySpeech - Plays the given Speech audio                                            *
 *                                                                                             *
 *   SCRIPT INPUT:	ret (int) - The speech index of which to play                              *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_PlaySpeech(lua_State* L) {
    int ret = lua_tointeger(L, -1);
    Speak((VoxType)ret);
    return 1;
}

/***********************************************************************************************
 * Script_StartMissionTimer - Starts the in-game mission timer                                 *
 *                                                                                             *
 *   SCRIPT INPUT:	none                                                                       *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_StartMissionTimer(lua_State* L) {
    Scen.MissionTimer.Start();
    Map.Redraw_Tab();
    return 1;
}

/***********************************************************************************************
 * Script_StopMissionTimer - Stops the in-game mission timer                                   *
 *                                                                                             *
 *   SCRIPT INPUT:	none                                                                       *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_StopMissionTimer(lua_State* L) {
    Scen.MissionTimer.Stop();
    return 1;
}

/***********************************************************************************************
 * Script_IncreaseMissionTimer - Increases the mission timer by the given amount               *
 *                                                                                             *
 *   SCRIPT INPUT:	ret (int) - amount of time in tenths of a minute to increase by            *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_IncreaseMissionTimer(lua_State* L) {
    int ret = lua_tointeger(L, -1);
    Scen.MissionTimer = Scen.MissionTimer + (ret * (TICKS_PER_MINUTE / 10));
    Map.Redraw_Tab();
    return 1;
}

/***********************************************************************************************
 * Script_DecreaseMissionTimer - Decreases the mission timer by the given amount               *
 *                                                                                             *
 *   SCRIPT INPUT:	ret (int) - amount of time in tenths of a minute to decrease by            *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_DecreaseMissionTimer(lua_State* L) {
    int ret = lua_tointeger(L, -1);
    Scen.MissionTimer = Scen.MissionTimer - (ret * (TICKS_PER_MINUTE / 10));
    if (Scen.MissionTimer < 0) { Scen.MissionTimer = 0; }
    Map.Redraw_Tab();
    return 1;
}

/***********************************************************************************************
 * Script_SetMissionTimer - Sets and starts the in-game mission timer                          *
 *                                                                                             *
 *   SCRIPT INPUT:	ret (int) - amount of time in tenths of a minute                           *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_SetMissionTimer(lua_State* L) {
    int ret = lua_tointeger(L, -1);
    Scen.MissionTimer = Scen.MissionTimer + (ret * (TICKS_PER_MINUTE / 10));
    Scen.MissionTimer.Start();
    Map.Redraw_Tab();
    return 1;
}

/***********************************************************************************************
 * Script_SetGlobalValue - Sets the global value to "set" position (on)                        *
 *                                                                                             *
 *   SCRIPT INPUT:	global_value (int) - The global to set to "set"                            *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_SetGlobalValue(lua_State* L) {
    int global_value = lua_tointeger(L, -1);
    Scen.Set_Global_To(global_value, true);
    return 1;
}

/***********************************************************************************************
 * Script_ClearGlobalValue - Sets the global value to "clear" position (off)                   *
 *                                                                                             *
 *   SCRIPT INPUT:	global_value (int) - The global to set to "off"                            *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_ClearGlobalValue(lua_State* L) {
    int global_value = lua_tointeger(L, -1);
    Scen.Set_Global_To(global_value, false);
    return 1;
}

/***********************************************************************************************
 * Script_AutoBaseBuilding - Sets AI basebuilding on/off for given house                       *
 *                                                                                             *
 *   SCRIPT INPUT:	houseType (int)    - The player (house) index of the AI                    *
 *                	on_off (bool)      - Enable / disable auto base building                   *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_AutoBaseBuilding(lua_State* L) {
    int houseType = lua_tointeger(L, 1);
    bool on_off = lua_toboolean(L, 2);

    HouseClass* this_house = Houses.Ptr(houseType);
    if (this_house != NULL) {
        this_house->IsBaseBuilding = on_off;
    }

    return 1;
}

/***********************************************************************************************
 * Script_StopMissionTimer - Stops the in-game mission timer                                   *
 *                                                                                             *
 *   SCRIPT INPUT:	none                                                                       *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_CreepShadow(lua_State* L) {
    Map.Encroach_Shadow(PlayerPtr);
    return 1;
}

/***********************************************************************************************
 * Script_DestroyTriggerBuilding - Stops the in-game mission timer                             *
 *                                                                                             *
 *   SCRIPT INPUT:	triggerIndex (int) - The index of the trigger previously created           *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_DestroyTriggerBuilding(lua_State* L) {
    int triggerIndex = lua_tointeger(L, -1);

    TriggerClass* this_trigger = Triggers.Ptr(triggerIndex);

    if (this_trigger != NULL) {
        for (int index = 0; index < Buildings.Count(); index++) {

            BuildingClass* this_building = Buildings.Ptr(index);

            if (this_building->Trigger == this_trigger && this_building->Strength > 0) {
                int damage = this_building->Strength;
                this_building->Take_Damage(damage, 0, WARHEAD_AP, 0, true);
            }

        }
    }

    return 1;
}

/***********************************************************************************************
 * Script_GiveOneTimeSpecialWeapon - Gives specified house a one time special weapon           *
 *                                                                                             *
 *   SCRIPT INPUT:	weaponType (int) - The special weapon type to give                         *
 *                	houseType (int)  - The player (house) index to give this to (-1 for all)   *
 *                	immediate(bool)  - should the weapon be fully charged?                     *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_GiveOneTimeSpecialWeapon(lua_State* L) {

    int weaponType = lua_tointeger(L, 1);

    int houseType = -1;
    bool immediate = -1;

    // Optional houseType parameter
    if (lua_gettop(L) >= 2) {
        houseType = lua_tointeger(L, 2);
    }

    // Optional immediate parameter
    if (lua_gettop(L) >= 2) {
        immediate = lua_toboolean(L, 3);
    }

    for (int h_index = 0; h_index < Houses.Count(); h_index++) {

        HouseClass* house = Houses.Ptr(h_index);

        if (house->ID == houseType || houseType == -1) {

            house->SuperWeapon[weaponType].Enable(TACTION_1_SPECIAL, true);

            if (immediate) {
                house->SuperWeapon[weaponType].Forced_Charge(true);
            }

            if (PlayerPtr == house) {
                Map.Add(RTTI_SPECIAL, weaponType);
                Map.Column[1].Flag_To_Redraw();
            }

            if (houseType >= 0) {
                break;
            }
        }

    }

    return 1;
}

/***********************************************************************************************
 * Script_GiveSpecialWeapon - Gives specified house a repeating special weapon                 *
 *                                                                                             *
 *   SCRIPT INPUT:	weaponType (int) - The special weapon type to give                         *
 *                	houseType (int)  - The player (house) index to give this to (-1 for all)   *
 *                	immediate(bool)  - should the weapon be fully charged?                     *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_GiveSpecialWeapon(lua_State* L) {

    int weaponType = lua_tointeger(L, 1);

    int houseType = -1;
    bool immediate = -1;

    // Optional houseType parameter
    if (lua_gettop(L) >= 2) {
        houseType = lua_tointeger(L, 2);
    }

    // Optional immediate parameter
    if (lua_gettop(L) == 3) {
        immediate = lua_toboolean(L, 3);
    }

    for (int h_index = 0; h_index < Houses.Count(); h_index++) {

        HouseClass* house = Houses.Ptr(h_index);

        if (house->ID == houseType || houseType == -1) {

            house->SuperWeapon[weaponType].Enable(false, true, false, true);

            if (immediate) {
                house->SuperWeapon[weaponType].Forced_Charge(true);
            }

            if (PlayerPtr == house) {
                Map.Add(RTTI_SPECIAL, weaponType);
                Map.Column[1].Flag_To_Redraw();
            }

            if (houseType >= 0) {
                break;
            }
        }

    }

    return 1;
}

/***********************************************************************************************
 * Script_DesignatePreferredTarget - Gives specified house a prefferable type of target        *
 *                                                                                             *
 *   SCRIPT INPUT:	houseType (int)  - The player (house) index to set targetting for (or -1)  *
 *                	targetType (int) - The target type index to set to                         *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_DesignatePreferredTarget(lua_State* L) {

    int houseType = lua_tointeger(L, 1);
    unsigned char targetType = lua_tointeger(L, 2);

    if (targetType >= 1 && targetType <= 11) {

        for (int h_index = 0; h_index < Houses.Count(); h_index++) {

            HouseClass* house = Houses.Ptr(h_index);

            if (house->ID == houseType || houseType == -1) {

                house->PreferredTarget = (QuarryType)targetType;

                // Discontinue giving out free cash if a specific house was specified
                if (houseType >= 0) {
                    break;
                }

            }

        }

    }

    return 1;
}

/***********************************************************************************************
 * Script_LaunchFakeNukes - All nuke silos begin launching duds                                *
 *                                                                                             *
 *   SCRIPT INPUT:	none                                                                       *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_LaunchFakeNukes(lua_State* L) {

    for (int index = 0; index < Buildings.Count(); index++) {
        BuildingClass* bldg = Buildings.Ptr(index);
        if (*bldg == STRUCT_MSLO) {
            bldg->Assign_Mission(MISSION_MISSILE);
        }
    }

    return 1;
}

/**********************************************************************************************
* Red Alert Vanilla Events                                                                    *
*=============================================================================================*/

/***********************************************************************************************
 * Script_CreateCellCallback - Creates an ENTERED_BY trigger and attaches a callback           *
 *                                                                                             *
 *   SCRIPT INPUT:	houseType (int)       - The house (player) index to test for entry         *
 *                  in_x (int)            - The cell/X location of the cell to test            *
 *                  in_y (int)            - The cell/Y location of the cell to test            *
 *                  in_function (string)  - The callback function to execute upon trigger      *
 *                                                                                             *
 *   SCRIPT OUTPUT:  triggerID - The ID of the trigger created for use with Trigger Functions  *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_CellCallback(lua_State* L) {

    // TODO: option 6 arguments to allow x2,y2 (for a rect)

    int houseType = lua_tointeger(L, 1);
    int in_x = lua_tointeger(L, 2);
    int in_y = lua_tointeger(L, 3);

    char* in_function = "";

    // Optional callback function
    if (lua_gettop(L) >= 4) {
        in_function = (char*)lua_tostring(L, 4);
    }

    // A house must be specified
    if (houseType >= 0 && houseType < HouseTypes.Count()) {

        // Create trigger type
        TriggerTypeClass* this_trigger_type = new TriggerTypeClass();
        this_trigger_type->House = (HousesType)houseType;
        this_trigger_type->IsActive = 1;
        this_trigger_type->IsPersistant = TriggerTypeClass::PERSISTANT;

        this_trigger_type->Event1 = TEVENT_PLAYER_ENTERED;
        this_trigger_type->EventControl = MULTI_ONLY;
        this_trigger_type->Event1.Data.House = (HousesType)houseType;

        // Create instance of trigger type
        TriggerClass* this_trigger = Find_Or_Make(this_trigger_type);

        if (in_function[0] != '\0') {
            // Copy the callback function into the trigger
            strncpy(this_trigger->MapScriptCallback, in_function, sizeof(this_trigger->MapScriptCallback) - 1);
        }

        // Get the cell and set the trigger
        this_trigger->Cell = XY_Cell(in_x, in_y);
        Map[XY_Cell(in_x, in_y)].Trigger = this_trigger;

        // Output the new trigger's ID
        lua_pushnumber(L, this_trigger->ID);

        return 1;

    }

    lua_pushnumber(L, -1);

    return 1;
}

/***********************************************************************************************
 * Script_CreateSpiedByCallback - Creates a SPIED_BY trigger and attaches a callback           *
 *                                                                                             *
 *   SCRIPT INPUT:	objectID (int)        - The building ID being spied                        *
 *                	houseType (int)       - The house (player) index doing the spying          *
 *                  in_function (string)  - The callback function to execute upon trigger      *
 *                                                                                             *
 *   SCRIPT OUTPUT:  triggerID - The ID of the trigger created for use with Trigger Functions  *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_SpiedByCallback(lua_State* L) {

    int objectID = lua_tointeger(L, 1);
    int houseType = lua_tointeger(L, 2);

    char* in_function = "";

    // Optional callback function
    if (lua_gettop(L) >= 3) {
        in_function = (char*)lua_tostring(L, 3);
    }

    BuildingClass* this_building = Buildings.Ptr(objectID);

    // A house must be specified
    if (houseType >= 0 && houseType < HouseTypes.Count() && this_building != NULL) {

        // Create trigger type
        TriggerTypeClass* this_trigger_type = new TriggerTypeClass();
        this_trigger_type->House = (HousesType)this_building->House->ID;
        this_trigger_type->IsActive = 1;
        this_trigger_type->IsPersistant = TriggerTypeClass::PERSISTANT;

        this_trigger_type->Event1 = TEVENT_SPIED;
        this_trigger_type->EventControl = MULTI_ONLY;
        this_trigger_type->Event1.Data.House = (HousesType)houseType;

        // Create instance of trigger type
        TriggerClass* this_trigger = Find_Or_Make(this_trigger_type);

        if (in_function[0] != '\0') {
            // Copy the callback function into the trigger
            strncpy(this_trigger->MapScriptCallback, in_function, sizeof(this_trigger->MapScriptCallback) - 1);
        }

        // Set the building
        this_building->Trigger = this_trigger;

        // Output the new trigger's ID
        lua_pushnumber(L, this_trigger->ID);

        return 1;

    }

    lua_pushnumber(L, -1);

    return 1;
}

/***********************************************************************************************
 * Script_CreateDiscoveryCallback - Creates a TEVENT_DISCOVERED trigger and attaches a callback*
 *                                                                                             *
 *  Trigger is sprung when the given object is discovered                                      *
 *                                                                                             *
 *   SCRIPT INPUT:	objectID (int)        - The object's ID being discovered                   *
 *                  in_function (string)  - The callback function to execute upon trigger      *
 *                                                                                             *
 *   SCRIPT OUTPUT:  triggerID - The ID of the trigger created for use with Trigger Functions  *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_ObjectDiscoveryCallback(lua_State* L) {

    int objectID = lua_tointeger(L, 1);

    char* in_function = "";

    // Optional callback function
    if (lua_gettop(L) >= 2) {
        in_function = (char*)lua_tostring(L, 2);
    }

    // A house must be specified
    if (objectID >= 0) {

        // Create trigger type
        TriggerTypeClass* this_trigger_type = new TriggerTypeClass();
        this_trigger_type->House = HousesType::HOUSE_NONE;
        this_trigger_type->IsActive = 1;
        this_trigger_type->IsPersistant = TriggerTypeClass::PERSISTANT;

        this_trigger_type->Event1 = TEVENT_DISCOVERED;
        this_trigger_type->EventControl = MULTI_ONLY;

        // Create instance of trigger type
        TriggerClass* this_trigger = Find_Or_Make(this_trigger_type);

        if (in_function[0] != '\0') {
            // Copy the callback function into the trigger
            strncpy(this_trigger->MapScriptCallback, in_function, sizeof(this_trigger->MapScriptCallback) - 1);
        }

        Script_SetObjectTrigger(objectID, this_trigger);

        // Output the new trigger's ID
        lua_pushnumber(L, this_trigger->ID);

        return 1;

    }

    lua_pushnumber(L, -1);

    return 1;
}

/***********************************************************************************************
 * Script_HouseDiscoveredCallback - Initiates when given house is discovered                   *
 *                                                                                             *
 *  Trigger is sprung when an object belonging to the given house is discovered                *
 *                                                                                             *
 *   SCRIPT INPUT:	houseType (int)       - The house (player) index that has been discovered  *
 *                  in_function (string)  - The callback function to execute upon trigger      *
 *                                                                                             *
 *   SCRIPT OUTPUT:  triggerID - The ID of the trigger created for use with Trigger Functions  *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_HouseDiscoveredCallback(lua_State* L) {

    int houseType = lua_tointeger(L, 1);

    char* in_function = "";

    // Optional callback function
    if (lua_gettop(L) >= 2) {
        in_function = (char*)lua_tostring(L, 2);
    }

    // A house must be specified
    if (houseType >= 0 && houseType < HouseTypes.Count()) {

        // Create trigger type
        TriggerTypeClass* this_trigger_type = new TriggerTypeClass();
        this_trigger_type->House = HOUSE_NONE;
        this_trigger_type->IsActive = 1;
        this_trigger_type->IsPersistant = TriggerTypeClass::VOLATILE;

        this_trigger_type->Event1 = TEVENT_HOUSE_DISCOVERED;
        this_trigger_type->EventControl = MULTI_ONLY;

        this_trigger_type->Event1.Data.House = (HousesType)houseType;

        // Create instance of trigger type

        TriggerClass* this_trigger = Find_Or_Make(this_trigger_type);

        if (in_function[0] != '\0') {
            // Copy the callback function into the trigger
            strncpy(this_trigger->MapScriptCallback, in_function, sizeof(this_trigger->MapScriptCallback) - 1);
        }

        HouseTriggers[PlayerPtr->Class->House].Add(this_trigger);

        // Output the new trigger's ID
        lua_pushnumber(L, this_trigger->ID);

        return 1;

    }

    lua_pushnumber(L, -1);

    return 1;
}

/***********************************************************************************************
 * Script_ObjectAttackedCallback - Trigger that springs when object is attacked by anyone      *
 *                                                                                             *
 *   SCRIPT INPUT:	objectID (int)        - The object's ID being discovered                   *
 *                  in_function (string)  - The callback function to execute upon trigger      *
 *                                                                                             *
 *   SCRIPT OUTPUT:  triggerID - The ID of the trigger created for use with Trigger Functions  *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_ObjectAttackedCallback(lua_State* L) {

    int objectID = lua_tointeger(L, 1);

    char* in_function = "";

    // Optional callback function
    if (lua_gettop(L) >= 2) {
        in_function = (char*)lua_tostring(L, 2);
    }

    // An object must be specified
    if (objectID >= 0) {

        // Create trigger type
        TriggerTypeClass* this_trigger_type = new TriggerTypeClass();
        this_trigger_type->House = HousesType::HOUSE_NONE;
        this_trigger_type->IsActive = 1;
        this_trigger_type->IsPersistant = TriggerTypeClass::PERSISTANT;

        this_trigger_type->Event1 = TEVENT_ATTACKED;
        this_trigger_type->EventControl = MULTI_ONLY;

        // Create instance of trigger type
        TriggerClass* this_trigger = Find_Or_Make(this_trigger_type);

        if (in_function[0] != '\0') {
            // Copy the callback function into the trigger
            strncpy(this_trigger->MapScriptCallback, in_function, sizeof(this_trigger->MapScriptCallback) - 1);
        }

        Script_SetObjectTrigger(objectID, this_trigger);

        // Output the new trigger's ID
        lua_pushnumber(L, this_trigger->ID);

        return 1;

    }

    lua_pushnumber(L, -1);

    return 1;
}

/***********************************************************************************************
 * Script_ObjectDestroyedCallback - Trigger that springs when object is destroyed by anyone    *
 *                                                                                             *
 *   SCRIPT INPUT:	objectID (int)        - The object's ID being discovered                   *
 *                  in_function (string)  - The callback function to execute upon trigger      *
 *                                                                                             *
 *   SCRIPT OUTPUT:  triggerID - The ID of the trigger created for use with Trigger Functions  *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_ObjectDestroyedCallback(lua_State* L) {

    int objectID = lua_tointeger(L, 1);

    char* in_function = "";

    // Optional callback function
    if (lua_gettop(L) >= 2) {
        in_function = (char*)lua_tostring(L, 2);
    }

    // An object must be specified
    if (objectID >= 0) {

        // Create trigger type
        TriggerTypeClass* this_trigger_type = new TriggerTypeClass();
        this_trigger_type->House = HousesType::HOUSE_NONE;
        this_trigger_type->IsActive = 1;
        this_trigger_type->IsPersistant = TriggerTypeClass::PERSISTANT;

        this_trigger_type->Event1 = TEVENT_DESTROYED;
        this_trigger_type->EventControl = MULTI_ONLY;

        // Create instance of trigger type
        TriggerClass* this_trigger = Find_Or_Make(this_trigger_type);

        if (in_function[0] != '\0') {
            // Copy the callback function into the trigger
            strncpy(this_trigger->MapScriptCallback, in_function, sizeof(this_trigger->MapScriptCallback) - 1);
        }

        Script_SetObjectTrigger(objectID, this_trigger);



        // Output the new trigger's ID
        lua_pushnumber(L, this_trigger->ID);

        return 1;

    }

    lua_pushnumber(L, -1);

    return 1;
}

/***********************************************************************************************
 * Script_AnyEventCallback -                                                                   *
 *                                                                                             *
 *   SCRIPT INPUT:	objectID (int)        - The object's ID being discovered                   *
 *                  houseType (int)       - The house (player) index                           *
 *                  in_function (string)  - The callback function to execute upon trigger      *
 *                                                                                             *
 *   SCRIPT OUTPUT:  triggerID - The ID of the trigger created for use with Trigger Functions  *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_AnyEventCallback(lua_State* L) {

    char* in_function = "";

    // Optional callback function
    if (lua_gettop(L) >= 1) {
        in_function = (char*)lua_tostring(L, 1);
    }

    // Create trigger type
    TriggerTypeClass* this_trigger_type = new TriggerTypeClass();
    this_trigger_type->House = HousesType::HOUSE_NONE;
    this_trigger_type->IsActive = 1;
    this_trigger_type->IsPersistant = TriggerTypeClass::PERSISTANT;

    this_trigger_type->Event1 = TEVENT_ANY;
    this_trigger_type->EventControl = MULTI_ONLY;

    // Create instance of trigger type
    TriggerClass* this_trigger = Find_Or_Make(this_trigger_type);

    if (in_function[0] != '\0') {
        // Copy the callback function into the trigger
        strncpy(this_trigger->MapScriptCallback, in_function, sizeof(this_trigger->MapScriptCallback) - 1);
    }

    LogicTriggers.Add(this_trigger);

    // Output the new trigger's ID
    lua_pushnumber(L, this_trigger->ID);

    return 1;

}

/***********************************************************************************************
 * Script_AllUnitsDestroyedCallback -  Called when all of a house's units are destroyed        *
 *                                                                                             *
 *   SCRIPT INPUT:	houseType (int)       - The house (player) index owner of said units       *
 *                  in_function (string)  - The callback function to execute upon trigger      *
 *                                                                                             *
 *   SCRIPT OUTPUT:  triggerID - The ID of the trigger created for use with Trigger Functions  *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_AllUnitsDestroyedCallback(lua_State* L) {

    int houseType = lua_tointeger(L, 1);

    char* in_function = "";

    // Optional callback function
    if (lua_gettop(L) >= 2) {
        in_function = (char*)lua_tostring(L, 2);
    }

    // A house must be specified
    if (houseType >= 0 && houseType < HouseTypes.Count()) {

        // Create trigger type
        TriggerTypeClass* this_trigger_type = new TriggerTypeClass();
        this_trigger_type->House = HousesType::HOUSE_NONE;
        this_trigger_type->IsActive = 1;
        this_trigger_type->IsPersistant = TriggerTypeClass::PERSISTANT;

        this_trigger_type->Event1 = TEVENT_UNITS_DESTROYED;
        this_trigger_type->EventControl = MULTI_ONLY;
        this_trigger_type->Event1.Data.House = (HousesType)houseType;

        // Create instance of trigger type
        TriggerClass* this_trigger = Find_Or_Make(this_trigger_type);

        if (in_function[0] != '\0') {
            // Copy the callback function into the trigger
            strncpy(this_trigger->MapScriptCallback, in_function, sizeof(this_trigger->MapScriptCallback) - 1);
        }

        HouseTriggers[(HousesType)houseType].Add(this_trigger);

        // Output the new trigger's ID
        lua_pushnumber(L, this_trigger->ID);

        return 1;

    }

    lua_pushnumber(L, -1);

    return 1;
}

/***********************************************************************************************
 * Script_AllBuildingsDestroyedCallback -  Called when all of a house's buildings are destroyed*
 *                                                                                             *
 *   SCRIPT INPUT:	houseType (int)       - The house (player) index owner of said buildings   *
 *                  in_function (string)  - The callback function to execute upon trigger      *
 *                                                                                             *
 *   SCRIPT OUTPUT:  triggerID - The ID of the trigger created for use with Trigger Functions  *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_AllBuildingsDestroyedCallback(lua_State* L) {

    int houseType = lua_tointeger(L, 1);

    char* in_function = "";

    // Optional callback function
    if (lua_gettop(L) >= 2) {
        in_function = (char*)lua_tostring(L, 2);
    }

    // A house must be specified
    if (houseType >= 0 && houseType < HouseTypes.Count()) {

        // Create trigger type
        TriggerTypeClass* this_trigger_type = new TriggerTypeClass();
        this_trigger_type->House = HousesType::HOUSE_NONE;
        this_trigger_type->IsActive = 1;
        this_trigger_type->IsPersistant = TriggerTypeClass::PERSISTANT;

        this_trigger_type->Event1 = TEVENT_BUILDINGS_DESTROYED;
        this_trigger_type->EventControl = MULTI_ONLY;
        this_trigger_type->Event1.Data.House = (HousesType)houseType;

        // Create instance of trigger type
        TriggerClass* this_trigger = Find_Or_Make(this_trigger_type);

        if (in_function[0] != '\0') {
            // Copy the callback function into the trigger
            strncpy(this_trigger->MapScriptCallback, in_function, sizeof(this_trigger->MapScriptCallback) - 1);
        }

        HouseTriggers[(HousesType)houseType].Add(this_trigger);

        // Output the new trigger's ID
        lua_pushnumber(L, this_trigger->ID);

        return 1;

    }

    lua_pushnumber(L, -1);

    return 1;
}

/***********************************************************************************************
 * Script_AllDestroyedCallback -  Called when everything belonging to a house is destyroyed    *
 *                                                                                             *
 *   SCRIPT INPUT:	houseType (int)       - The house (player) index that got pwned            *
 *                  in_function (string)  - The callback function to execute upon trigger      *
 *                                                                                             *
 *   SCRIPT OUTPUT:  triggerID - The ID of the trigger created for use with Trigger Functions  *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_AllDestroyedCallback(lua_State* L) {

    int houseType = lua_tointeger(L, 1);

    char* in_function = "";

    // Optional callback function
    if (lua_gettop(L) >= 2) {
        in_function = (char*)lua_tostring(L, 2);
    }

    // A house must be specified
    if (houseType >= 0 && houseType < HouseTypes.Count()) {

        // Create trigger type
        TriggerTypeClass* this_trigger_type = new TriggerTypeClass();
        this_trigger_type->House = HousesType::HOUSE_NONE;
        this_trigger_type->IsActive = 1;
        this_trigger_type->IsPersistant = TriggerTypeClass::PERSISTANT;

        this_trigger_type->Event1 = TEVENT_ALL_DESTROYED;
        this_trigger_type->EventControl = MULTI_ONLY;
        this_trigger_type->Event1.Data.House = (HousesType)houseType;

        // Create instance of trigger type
        TriggerClass* this_trigger = Find_Or_Make(this_trigger_type);

        if (in_function[0] != '\0') {
            // Copy the callback function into the trigger
            strncpy(this_trigger->MapScriptCallback, in_function, sizeof(this_trigger->MapScriptCallback) - 1);
        }

        HouseTriggers[(HousesType)houseType].Add(this_trigger);

        // Output the new trigger's ID
        lua_pushnumber(L, this_trigger->ID);

        return 1;

    }

    lua_pushnumber(L, -1);

    return 1;
}

/***********************************************************************************************
 * Script_CreditsReachedCallback -  Called when given house meets given amount of credits      *
 *                                                                                             *
 *   SCRIPT INPUT:	houseType (int)       - The house (player) index to test against           *
 *                  in_credits (int)      - The amount of credits                              *
 *                  in_function (string)  - The callback function to execute upon trigger      *
 *                                                                                             *
 *   SCRIPT OUTPUT:  triggerID - The ID of the trigger created for use with Trigger Functions  *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_CreditsReachedCallback(lua_State* L) {

    int houseType = lua_tointeger(L, 1);
    int in_credits = lua_tointeger(L, 2);

    char* in_function = "";

    // Optional callback function
    if (lua_gettop(L) >= 3) {
        in_function = (char*)lua_tostring(L, 3);
    }

    // A house must be specified
    if (houseType >= 0 && houseType < HouseTypes.Count()) {

        // Create trigger type
        TriggerTypeClass* this_trigger_type = new TriggerTypeClass();
        this_trigger_type->House = (HousesType)houseType;
        this_trigger_type->IsActive = 1;
        this_trigger_type->IsPersistant = TriggerTypeClass::PERSISTANT;

        this_trigger_type->Event1 = TEVENT_CREDITS;
        this_trigger_type->EventControl = MULTI_ONLY;
        this_trigger_type->Event1.Data.Value = in_credits;

        // Create instance of trigger type
        TriggerClass* this_trigger = Find_Or_Make(this_trigger_type);

        if (in_function[0] != '\0') {
            // Copy the callback function into the trigger
            strncpy(this_trigger->MapScriptCallback, in_function, sizeof(this_trigger->MapScriptCallback) - 1);
        }

        HouseTriggers[(HousesType)houseType].Add(this_trigger);

        // Output the new trigger's ID
        lua_pushnumber(L, this_trigger->ID);

        return 1;

    }

    lua_pushnumber(L, -1);

    return 1;
}

/***********************************************************************************************
 * Script_TimeReachedCallback -  Called when the given amount of time has elapsed              *
 *                                                                                             *
 *   SCRIPT INPUT:	in_time (int)         - The time (in tenths of a second) that was reached  *
 *                  in_function (string)  - The callback function to execute upon trigger      *
 *                                                                                             *
 *   SCRIPT OUTPUT:  triggerID - The ID of the trigger created for use with Trigger Functions  *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_TimeReachedCallback(lua_State* L) {

    int in_time = lua_tointeger(L, 1);

    char* in_function = "";

    // Optional callback function
    if (lua_gettop(L) >= 2) {
        in_function = (char*)lua_tostring(L, 2);
    }

    // Create trigger type
    TriggerTypeClass* this_trigger_type = new TriggerTypeClass();
    this_trigger_type->House = HOUSE_NONE;
    this_trigger_type->IsActive = 1;
    this_trigger_type->IsPersistant = TriggerTypeClass::PERSISTANT;

    this_trigger_type->Event1 = TEVENT_TIME;
    this_trigger_type->EventControl = MULTI_ONLY;
    this_trigger_type->Event1.Data.Value = in_time;

    // Create instance of trigger type
    TriggerClass* this_trigger = Find_Or_Make(this_trigger_type);

    if (in_function[0] != '\0') {
        // Copy the callback function into the trigger
        strncpy(this_trigger->MapScriptCallback, in_function, sizeof(this_trigger->MapScriptCallback) - 1);
    }

    LogicTriggers.Add(this_trigger);

    // Output the new trigger's ID
    lua_pushnumber(L, this_trigger->ID);

    return 1;

}

/***********************************************************************************************
 * Script_MissionTimerCallback -  Called when the mission timer has expired                    *
 *                                                                                             *
 *   SCRIPT INPUT:	in_function (string)  - The callback function to execute upon trigger      *
 *                                                                                             *
 *   SCRIPT OUTPUT:  triggerID - The ID of the trigger created for use with Trigger Functions  *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_MissionTimerCallback(lua_State* L) {

    char* in_function = "";

    // Optional callback function
    if (lua_gettop(L) >= 1) {
        in_function = (char*)lua_tostring(L, 1);
    }

    // Create trigger type
    TriggerTypeClass* this_trigger_type = new TriggerTypeClass();
    this_trigger_type->House = HOUSE_NONE;
    this_trigger_type->IsActive = 1;
    this_trigger_type->IsPersistant = TriggerTypeClass::PERSISTANT;

    this_trigger_type->Event1 = TEVENT_MISSION_TIMER_EXPIRED;
    this_trigger_type->EventControl = MULTI_ONLY;

    // Create instance of trigger type
    TriggerClass* this_trigger = Find_Or_Make(this_trigger_type);

    if (in_function[0] != '\0') {
        // Copy the callback function into the trigger
        strncpy(this_trigger->MapScriptCallback, in_function, sizeof(this_trigger->MapScriptCallback) - 1);
    }

    LogicTriggers.Add(this_trigger);

    // Output the new trigger's ID
    lua_pushnumber(L, this_trigger->ID);

    return 1;

}

/***********************************************************************************************
 * Script_UnitsDestroyedCallback -  Called when given number of a house's units are destroyed  *
 *                                                                                             *
 *   SCRIPT INPUT:	houseType (int)       - The house (player) index owner of said units       *
 *                  in_number (int)       - The number of units that have must be destroyed    *
 *                  in_function (string)  - The callback function to execute upon trigger      *
 *                                                                                             *
 *   SCRIPT OUTPUT:  triggerID - The ID of the trigger created for use with Trigger Functions  *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_UnitsDestroyedCallback(lua_State* L) {

    int houseType = lua_tointeger(L, 1);
    int in_number = lua_tointeger(L, 2);

    char* in_function = "";

    // Optional callback function
    if (lua_gettop(L) >= 3) {
        in_function = (char*)lua_tostring(L, 3);
    }

    // A house must be specified
    if (houseType >= 0 && houseType < HouseTypes.Count()) {

        // Create trigger type
        TriggerTypeClass* this_trigger_type = new TriggerTypeClass();
        this_trigger_type->House = (HousesType)houseType;
        this_trigger_type->IsActive = 1;
        this_trigger_type->IsPersistant = TriggerTypeClass::PERSISTANT;

        this_trigger_type->Event1 = TEVENT_NUNITS_DESTROYED;
        this_trigger_type->EventControl = MULTI_ONLY;
        this_trigger_type->Event1.Data.Value = in_number;

        // Create instance of trigger type
        TriggerClass* this_trigger = Find_Or_Make(this_trigger_type);

        if (in_function[0] != '\0') {
            // Copy the callback function into the trigger
            strncpy(this_trigger->MapScriptCallback, in_function, sizeof(this_trigger->MapScriptCallback) - 1);
        }

        HouseTriggers[(HousesType)houseType].Add(this_trigger);


        // Output the new trigger's ID
        lua_pushnumber(L, this_trigger->ID);

        return 1;

    }

    lua_pushnumber(L, -1);

    return 1;
}

/************************************************************************************************
 * Script_BuildingsDestroyedCallback -  Called when number of a house's buildings are destroyed *
 *                                                                                              *
 *   SCRIPT INPUT:	houseType (int)       - The house (player) index owner of said buildings    *
 *                  in_number (int)       - The number of buildings that have must be destroyed *
 *                  in_function (string)  - The callback function to execute upon trigger       *
 *                                                                                              *
 *   SCRIPT OUTPUT:  triggerID - The ID of the trigger created for use with Trigger Functions   *
 *                                                                                              *
 * INPUT:  lua_State - The current Lua state                                                    *
 *                                                                                              *
 * OUTPUT:  int; Did the function run successfully? Return 1                                    *
 *                                                                                              *
 * WARNINGS:  ?                                                                                 *
 *                                                                                              *
 *==============================================================================================*/
static int Script_BuildingsDestroyedCallback(lua_State* L) {

    int houseType = lua_tointeger(L, 1);
    int in_number = lua_tointeger(L, 2);

    char* in_function = "";

    // Optional callback function
    if (lua_gettop(L) >= 3) {
        in_function = (char*)lua_tostring(L, 3);
    }

    // A house must be specified
    if (houseType >= 0 && houseType < HouseTypes.Count()) {

        // Create trigger type
        TriggerTypeClass* this_trigger_type = new TriggerTypeClass();
        this_trigger_type->House = (HousesType)houseType;
        this_trigger_type->IsActive = 1;
        this_trigger_type->IsPersistant = TriggerTypeClass::PERSISTANT;

        this_trigger_type->Event1 = TEVENT_NBUILDINGS_DESTROYED;
        this_trigger_type->EventControl = MULTI_ONLY;
        this_trigger_type->Event1.Data.Value = in_number;

        // Create instance of trigger type
        TriggerClass* this_trigger = Find_Or_Make(this_trigger_type);

        if (in_function[0] != '\0') {
            // Copy the callback function into the trigger
            strncpy(this_trigger->MapScriptCallback, in_function, sizeof(this_trigger->MapScriptCallback) - 1);
        }

        HouseTriggers[(HousesType)houseType].Add(this_trigger);

        // Output the new trigger's ID
        lua_pushnumber(L, this_trigger->ID);

        return 1;

    }

    lua_pushnumber(L, -1);

    return 1;
}

/***********************************************************************************************
 * Script_NoFactoriesCallback -  Called when no more factories exist for a house               *
 *                                                                                             *
 *   SCRIPT INPUT:	houseType (int)       - The house (player) index that got pwned            *
 *                  in_function (string)  - The callback function to execute upon trigger      *
 *                                                                                             *
 *   SCRIPT OUTPUT:  triggerID - The ID of the trigger created for use with Trigger Functions  *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_NoFactoriesCallback(lua_State* L) {

    int houseType = lua_tointeger(L, 1);

    char* in_function = "";

    // Optional callback function
    if (lua_gettop(L) >= 2) {
        in_function = (char*)lua_tostring(L, 2);
    }

    // A house must be specified
    if (houseType >= 0 && houseType < HouseTypes.Count()) {

        // Create trigger type
        TriggerTypeClass* this_trigger_type = new TriggerTypeClass();
        this_trigger_type->House = (HousesType)houseType;
        this_trigger_type->IsActive = 1;
        this_trigger_type->IsPersistant = TriggerTypeClass::PERSISTANT;

        this_trigger_type->Event1 = TEVENT_NOFACTORIES;
        this_trigger_type->EventControl = MULTI_ONLY;
        this_trigger_type->Event1.Data.Value = -1;

        // Create instance of trigger type
        TriggerClass* this_trigger = Find_Or_Make(this_trigger_type);

        if (in_function[0] != '\0') {
            // Copy the callback function into the trigger
            strncpy(this_trigger->MapScriptCallback, in_function, sizeof(this_trigger->MapScriptCallback) - 1);
        }

        HouseTriggers[(HousesType)houseType].Add(this_trigger);


        // Output the new trigger's ID
        lua_pushnumber(L, this_trigger->ID);

        return 1;

    }

    lua_pushnumber(L, -1);

    return 1;
}

/***********************************************************************************************
 * Script_CivilianEscapeCallback - Called when civilians get evacuated from map                *
 *                                                                                             *
 *   SCRIPT INPUT:	houseType (int)       - The house (player) owner of civilians escaped      *
 *                  in_function (string)  - The callback function to execute upon trigger      *
 *                                                                                             *
 *   SCRIPT OUTPUT:  triggerID - The ID of the trigger created for use with Trigger Functions  *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_CivilianEscapeCallback(lua_State* L) {

    int houseType = lua_tointeger(L, 1);

    char* in_function = "";

    // Optional callback function
    if (lua_gettop(L) >= 2) {
        in_function = (char*)lua_tostring(L, 2);
    }

    if (houseType >= 0 && houseType < HouseTypes.Count()) {

        // Create trigger type
        TriggerTypeClass* this_trigger_type = new TriggerTypeClass();
        this_trigger_type->House = (HousesType)houseType;
        this_trigger_type->IsActive = 1;
        this_trigger_type->IsPersistant = TriggerTypeClass::PERSISTANT;

        this_trigger_type->Event1 = TEVENT_EVAC_CIVILIAN;
        this_trigger_type->EventControl = MULTI_ONLY;
        this_trigger_type->Event1.Data.Value = -1;

        // Create instance of trigger type
        TriggerClass* this_trigger = Find_Or_Make(this_trigger_type);

        if (in_function[0] != '\0') {
            // Copy the callback function into the trigger
            strncpy(this_trigger->MapScriptCallback, in_function, sizeof(this_trigger->MapScriptCallback) - 1);
        }

        LogicTriggers.Add(this_trigger);

        // Output the new trigger's ID
        lua_pushnumber(L, this_trigger->ID);

        return 1;
    }

    lua_pushnumber(L, -1);

    return 1;

}

/***********************************************************************************************
 * Script_BuildingBuiltCallback - Called when a specified house builds a specified building    *
 *                                                                                             *
 *   SCRIPT INPUT:	houseType (int)       - The house (player) owner that built the building   *
 *                  objectType (int)      - The index / building type to check for             *
 *                  in_function (string)  - The callback function to execute upon trigger      *
 *                                                                                             *
 *   SCRIPT OUTPUT:  triggerID - The ID of the trigger created for use with Trigger Functions  *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_BuildingBuiltCallback(lua_State* L) {

    int houseType = lua_tointeger(L, 1);
    int objectType = lua_tointeger(L, 2);

    char* in_function = "";

    // Optional callback function
    if (lua_gettop(L) >= 3) {
        in_function = (char*)lua_tostring(L, 3);
    }

    if (objectType >= 0 && houseType >= 0 && houseType < HouseTypes.Count()) {

        // Create trigger type
        TriggerTypeClass* this_trigger_type = new TriggerTypeClass();
        this_trigger_type->House = (HousesType)houseType;
        this_trigger_type->IsActive = 1;
        this_trigger_type->IsPersistant = TriggerTypeClass::PERSISTANT;

        this_trigger_type->Event1 = TEVENT_BUILD;
        this_trigger_type->EventControl = MULTI_ONLY;
        this_trigger_type->Event1.Data.Value = objectType;

        // Create instance of trigger type
        TriggerClass* this_trigger = Find_Or_Make(this_trigger_type);

        if (in_function[0] != '\0') {
            // Copy the callback function into the trigger
            strncpy(this_trigger->MapScriptCallback, in_function, sizeof(this_trigger->MapScriptCallback) - 1);
        }

        HouseTriggers[houseType].Add(this_trigger);

        // Output the new trigger's ID
        lua_pushnumber(L, this_trigger->ID);

        return 1;

    }

    lua_pushnumber(L, -1);

    return 1;

}

/***********************************************************************************************
 * Script_UnitBuiltCallback - Called when a specified house builds a specified unit            *
 *                                                                                             *
 *  Does not trigger for units that spawn with buildings (ie. ore truck)                       *
 *                                                                                             *
 *   SCRIPT INPUT:	houseType (int)       - The house (player) owner that built the unit       *
 *                  objectType (int)      - The index / unit type to check for                 *
 *                  in_function (string)  - The callback function to execute upon trigger      *
 *                                                                                             *
 *   SCRIPT OUTPUT:  triggerID - The ID of the trigger created for use with Trigger Functions  *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_UnitBuiltCallback(lua_State* L) {

    int houseType = lua_tointeger(L, 1);
    int objectType = lua_tointeger(L, 2);

    char* in_function = "";

    // Optional callback function
    if (lua_gettop(L) >= 3) {
        in_function = (char*)lua_tostring(L, 3);
    }

    if (objectType >= 0 && houseType >= 0 && houseType < HouseTypes.Count()) {

        // Create trigger type
        TriggerTypeClass* this_trigger_type = new TriggerTypeClass();
        this_trigger_type->House = (HousesType)houseType;
        this_trigger_type->IsActive = 1;
        this_trigger_type->IsPersistant = TriggerTypeClass::PERSISTANT;

        this_trigger_type->Event1 = TEVENT_BUILD_UNIT;
        this_trigger_type->EventControl = MULTI_ONLY;
        this_trigger_type->Event1.Data.Value = objectType;

        // Create instance of trigger type
        TriggerClass* this_trigger = Find_Or_Make(this_trigger_type);

        if (in_function[0] != '\0') {
            // Copy the callback function into the trigger
            strncpy(this_trigger->MapScriptCallback, in_function, sizeof(this_trigger->MapScriptCallback) - 1);
        }

        HouseTriggers[houseType].Add(this_trigger);

        // Output the new trigger's ID
        lua_pushnumber(L, this_trigger->ID);

        return 1;

    }

    lua_pushnumber(L, -1);

    return 1;

}

/***********************************************************************************************
 * Script_InfantryBuiltCallback - Called when a specified house builds a specified infantry    *
 *                                                                                             *
 *  Does not trigger for infantry gained by selling a building                                 *
 *                                                                                             *
 *   SCRIPT INPUT:	houseType (int)       - The house (player) owner that built the infantry   *
 *                  objectType (int)      - The index / infantry type to check for             *
 *                  in_function (string)  - The callback function to execute upon trigger      *
 *                                                                                             *
 *   SCRIPT OUTPUT:  triggerID - The ID of the trigger created for use with Trigger Functions  *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_InfantryBuiltCallback(lua_State* L) {

    int houseType = lua_tointeger(L, 1);
    int objectType = lua_tointeger(L, 2);

    char* in_function = "";

    // Optional callback function
    if (lua_gettop(L) >= 3) {
        in_function = (char*)lua_tostring(L, 3);
    }

    if (objectType >= 0 && houseType >= 0 && houseType < HouseTypes.Count()) {

        // Create trigger type
        TriggerTypeClass* this_trigger_type = new TriggerTypeClass();
        this_trigger_type->House = (HousesType)houseType;
        this_trigger_type->IsActive = 1;
        this_trigger_type->IsPersistant = TriggerTypeClass::PERSISTANT;

        this_trigger_type->Event1 = TEVENT_BUILD_INFANTRY;
        this_trigger_type->EventControl = MULTI_ONLY;
        this_trigger_type->Event1.Data.Value = objectType;

        // Create instance of trigger type
        TriggerClass* this_trigger = Find_Or_Make(this_trigger_type);

        if (in_function[0] != '\0') {
            // Copy the callback function into the trigger
            strncpy(this_trigger->MapScriptCallback, in_function, sizeof(this_trigger->MapScriptCallback) - 1);
        }

        HouseTriggers[houseType].Add(this_trigger);

        // Output the new trigger's ID
        lua_pushnumber(L, this_trigger->ID);

        return 1;

    }

    lua_pushnumber(L, -1);

    return 1;

}

/***********************************************************************************************
 * Script_AircraftBuiltCallback - Called when a specified house builds a specified aircraft    *
 *                                                                                             *
 *  Does not trigger for units that spawn with buildings                                       *
 *                                                                                             *
 *   SCRIPT INPUT:	houseType (int)       - The house (player) owner that built the unit       *
 *                  objectType (int)      - The index / unit type to check for                 *
 *                  in_function (string)  - The callback function to execute upon trigger      *
 *                                                                                             *
 *   SCRIPT OUTPUT:  triggerID - The ID of the trigger created for use with Trigger Functions  *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_AircraftBuiltCallback(lua_State* L) {

    int houseType = lua_tointeger(L, 1);
    int objectType = lua_tointeger(L, 2);

    char* in_function = "";

    // Optional callback function
    if (lua_gettop(L) >= 3) {
        in_function = (char*)lua_tostring(L, 3);
    }

    if (objectType >= 0 && houseType >= 0 && houseType < HouseTypes.Count()) {

        // Create trigger type
        TriggerTypeClass* this_trigger_type = new TriggerTypeClass();
        this_trigger_type->House = (HousesType)houseType;
        this_trigger_type->IsActive = 1;
        this_trigger_type->IsPersistant = TriggerTypeClass::PERSISTANT;

        this_trigger_type->Event1 = TEVENT_BUILD_AIRCRAFT;
        this_trigger_type->EventControl = MULTI_ONLY;
        this_trigger_type->Event1.Data.Value = objectType;

        // Create instance of trigger type
        TriggerClass* this_trigger = Find_Or_Make(this_trigger_type);

        if (in_function[0] != '\0') {
            // Copy the callback function into the trigger
            strncpy(this_trigger->MapScriptCallback, in_function, sizeof(this_trigger->MapScriptCallback) - 1);
        }

        HouseTriggers[houseType].Add(this_trigger);

        // Output the new trigger's ID
        lua_pushnumber(L, this_trigger->ID);

        return 1;

    }

    lua_pushnumber(L, -1);

    return 1;

}

/***********************************************************************************************
 * Script_TeamLeavesMapCallback - Called when the specified team leaves the map                *
 *                                                                                             *
 *   SCRIPT INPUT:	in_team (int)         - The team index that has left the map               *
 *                	in_function (string)  - The callback function to execute upon trigger      *
 *                                                                                             *
 *   SCRIPT OUTPUT:  triggerID - The ID of the trigger created for use with Trigger Functions  *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_TeamLeavesMapCallback(lua_State* L) {

    int in_team = lua_tointeger(L, 1);

    char* in_function = "";

    // Optional callback function
    if (lua_gettop(L) >= 2) {
        in_function = (char*)lua_tostring(L, 2);
    }
    TeamTypeClass* this_team = TeamTypes.Ptr(in_team);

    if (this_team != NULL) {

        // Create trigger type
        TriggerTypeClass* this_trigger_type = new TriggerTypeClass();
        this_trigger_type->House = this_team->House;
        this_trigger_type->IsActive = 1;
        this_trigger_type->IsPersistant = TriggerTypeClass::PERSISTANT;

        this_trigger_type->Event1 = TEVENT_LEAVES_MAP;
        this_trigger_type->EventControl = MULTI_ONLY;


        this_trigger_type->Event1.Team = this_team;

        // Create instance of trigger type
        TriggerClass* this_trigger = Find_Or_Make(this_trigger_type);

        if (in_function[0] != '\0') {
            // Copy the callback function into the trigger
            strncpy(this_trigger->MapScriptCallback, in_function, sizeof(this_trigger->MapScriptCallback) - 1);
        }

        LogicTriggers.Add(this_trigger);

        // Output the new trigger's ID
        lua_pushnumber(L, this_trigger->ID);

        return 1;

    }

    // Output the new trigger's ID
    lua_pushnumber(L, -1);

    return 1;

}

/***********************************************************************************************
 * Script_ZoneEntryCallback - Called when a specified house enters zone of given position      *
 *                                                                                             *
 *  Does not trigger for units that spawn with buildings                                       *
 *                                                                                             *
 *   SCRIPT INPUT:	houseType (int)       - The house (player) owner that built the unit       *
 *                  in_x (int)            - The cell/X location of the cell to test            *
 *                  in_y (int)            - The cell/Y location of the cell to test            *
 *                  in_function (string)  - The callback function to execute upon trigger      *
 *                                                                                             *
 *   SCRIPT OUTPUT:  triggerID - The ID of the trigger created for use with Trigger Functions  *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_ZoneEntryCallback(lua_State* L) {

    int houseType = lua_tointeger(L, 1);
    int in_x = lua_tointeger(L, 2);
    int in_y = lua_tointeger(L, 3);

    char* in_function = "";

    // Optional callback function
    if (lua_gettop(L) >= 4) {
        in_function = (char*)lua_tostring(L, 4);
    }

    if (houseType >= 0 && houseType < HouseTypes.Count()) {

        // Create trigger type
        TriggerTypeClass* this_trigger_type = new TriggerTypeClass();
        this_trigger_type->House = (HousesType)houseType;
        this_trigger_type->IsActive = 1;
        this_trigger_type->IsPersistant = TriggerTypeClass::PERSISTANT;

        this_trigger_type->Event1 = TEVENT_ENTERS_ZONE;
        this_trigger_type->EventControl = MULTI_ONLY;
        this_trigger_type->Event1.Data.House = (HousesType)houseType;

        // Create instance of trigger type
        TriggerClass* this_trigger = Find_Or_Make(this_trigger_type);

        if (in_function[0] != '\0') {
            // Copy the callback function into the trigger
            strncpy(this_trigger->MapScriptCallback, in_function, sizeof(this_trigger->MapScriptCallback) - 1);
        }

        // Get the cell and set the trigger
        this_trigger->Cell = XY_Cell(in_x, in_y);
        MapTriggers.Add(this_trigger);


        // Output the new trigger's ID
        lua_pushnumber(L, this_trigger->ID);

        return 1;

    }

    lua_pushnumber(L, -1);

    return 1;

}

/***********************************************************************************************
 * Script_HorizontalCrossCallback - Called when a specified house's units cross the given X    *
 *                                                                                             *
 *  Does not trigger for units that spawn with buildings                                       *
 *                                                                                             *
 *   SCRIPT INPUT:	houseType (int)       - The house (player) owner that owns the unit        *
 *                  in_x (int)            - The cell/X location that must be crossed           *
 *                  in_function (string)  - The callback function to execute upon trigger      *
 *                                                                                             *
 *   SCRIPT OUTPUT:  triggerID - The ID of the trigger created for use with Trigger Functions  *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_HorizontalCrossCallback(lua_State* L) {

    int houseType = lua_tointeger(L, 1);
    int in_x = lua_tointeger(L, 2);

    char* in_function = "";

    // Optional callback function
    if (lua_gettop(L) >= 3) {
        in_function = (char*)lua_tostring(L, 3);
    }

    if (houseType >= 0 && houseType < HouseTypes.Count()) {

        // Create trigger type
        TriggerTypeClass* this_trigger_type = new TriggerTypeClass();
        this_trigger_type->House = (HousesType)houseType;
        this_trigger_type->IsActive = 1;
        this_trigger_type->IsPersistant = TriggerTypeClass::PERSISTANT;

        this_trigger_type->Event1 = TEVENT_CROSS_VERTICAL;
        this_trigger_type->EventControl = MULTI_ONLY;
        this_trigger_type->Event1.Data.House = (HousesType)houseType;

        // Create instance of trigger type
        TriggerClass* this_trigger = Find_Or_Make(this_trigger_type);

        if (in_function[0] != '\0') {
            // Copy the callback function into the trigger
            strncpy(this_trigger->MapScriptCallback, in_function, sizeof(this_trigger->MapScriptCallback) - 1);
        }

        // Find a free spot and set the trigger
        int addY = 0;
        for (addY = 0; addY < Map.MapCellHeight; addY++) {
            TriggerClass* trigger = Map[XY_Cell(in_x, Map.MapCellY + addY)].Trigger;
            if (trigger == NULL) {
                this_trigger->Cell = XY_Cell(in_x, Map.MapCellY + addY);
                Map[XY_Cell(in_x, Map.MapCellY + addY)].Trigger = this_trigger;
                break;
            }
        }

        // Output the new trigger's ID
        lua_pushnumber(L, this_trigger->ID);

        return 1;

    }

    lua_pushnumber(L, -1);

    return 1;

}

/***********************************************************************************************
 * Script_VerticalCrossCallback - Called when a specified house's units cross the given Y      *
 *                                                                                             *
 *  Does not trigger for units that spawn with buildings                                       *
 *                                                                                             *
 *   SCRIPT INPUT:	houseType (int)       - The house (player) owner that owns the unit        *
 *                  in_y (int)            - The cell/Y location that must be crossed           *
 *                  in_function (string)  - The callback function to execute upon trigger      *
 *                                                                                             *
 *   SCRIPT OUTPUT:  triggerID - The ID of the trigger created for use with Trigger Functions  *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_VerticalCrossCallback(lua_State* L) {

    int houseType = lua_tointeger(L, 1);
    int in_y = lua_tointeger(L, 2);

    char* in_function = "";

    // Optional callback function
    if (lua_gettop(L) >= 3) {
        in_function = (char*)lua_tostring(L, 3);
    }

    if (houseType >= 0 && houseType < HouseTypes.Count()) {

        // Create trigger type
        TriggerTypeClass* this_trigger_type = new TriggerTypeClass();
        this_trigger_type->House = (HousesType)houseType;
        this_trigger_type->IsActive = 1;
        this_trigger_type->IsPersistant = TriggerTypeClass::PERSISTANT;

        this_trigger_type->Event1 = TEVENT_CROSS_HORIZONTAL; // <- Their description doesn't make sense, not mine
        this_trigger_type->EventControl = MULTI_ONLY;
        this_trigger_type->Event1.Data.House = (HousesType)houseType;

        // Create instance of trigger type
        TriggerClass* this_trigger = Find_Or_Make(this_trigger_type);

        if (in_function[0] != '\0') {
            // Copy the callback function into the trigger
            strncpy(this_trigger->MapScriptCallback, in_function, sizeof(this_trigger->MapScriptCallback) - 1);
        }

        // Find a free spot and set the trigger
        int addX = 0;
        for (addX = 0; addX < Map.MapCellWidth; addX++) {
            TriggerClass* trigger = Map[XY_Cell(Map.MapCellX + addX, in_y)].Trigger;
            if (trigger == NULL) {
                this_trigger->Cell = XY_Cell(Map.MapCellX + addX, in_y);
                Map[XY_Cell(Map.MapCellX + addX, in_y)].Trigger = this_trigger;
                break;
            }
        }

        // Output the new trigger's ID
        lua_pushnumber(L, this_trigger->ID);

        return 1;

    }

    lua_pushnumber(L, -1);

    return 1;

}

/***********************************************************************************************
 * Script_GlobalSetCallback -  activated when the global value is in the "set" poisition       *
 *                                                                                             *
 *   SCRIPT INPUT:	in_global             - The global value to set                            *
 *                	in_function (string)  - The callback function to execute upon trigger      *
 *                                                                                             *
 *   SCRIPT OUTPUT:  triggerID - The ID of the trigger created for use with Trigger Functions  *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_GlobalSetCallback(lua_State* L) {

    int in_global = lua_tointeger(L, 1);

    char* in_function = "";

    // Optional callback function
    if (lua_gettop(L) >= 2) {
        in_function = (char*)lua_tostring(L, 2);
    }

    // Create trigger type
    TriggerTypeClass* this_trigger_type = new TriggerTypeClass();
    this_trigger_type->House = HOUSE_NONE;
    this_trigger_type->IsActive = 1;
    this_trigger_type->IsPersistant = TriggerTypeClass::PERSISTANT;

    this_trigger_type->Event1 = TEVENT_GLOBAL_SET;
    this_trigger_type->EventControl = MULTI_ONLY;
    this_trigger_type->Event1.Data.Value = in_global;

    // Create instance of trigger type
    TriggerClass* this_trigger = Find_Or_Make(this_trigger_type);

    if (in_function[0] != '\0') {
        // Copy the callback function into the trigger
        strncpy(this_trigger->MapScriptCallback, in_function, sizeof(this_trigger->MapScriptCallback) - 1);
    }

    LogicTriggers.Add(this_trigger);

    // Output the new trigger's ID
    lua_pushnumber(L, this_trigger->ID);

    return 1;
}

/***********************************************************************************************
 * Script_GlobalSetCallback -  activated when the global value is in the "cleared" poisition   *
 *                                                                                             *
 *   SCRIPT INPUT:	in_global             - The global value to set to "cleared"               *
 *                	in_function (string)  - The callback function to execute upon trigger      *
 *                                                                                             *
 *   SCRIPT OUTPUT:  triggerID - The ID of the trigger created for use with Trigger Functions  *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_GlobalClearedCallback(lua_State* L) {

    int in_global = lua_tointeger(L, 1);

    char* in_function = "";

    // Optional callback function
    if (lua_gettop(L) >= 2) {
        in_function = (char*)lua_tostring(L, 2);
    }

    // Create trigger type
    TriggerTypeClass* this_trigger_type = new TriggerTypeClass();
    this_trigger_type->House = HOUSE_NONE;
    this_trigger_type->IsActive = 1;
    this_trigger_type->IsPersistant = TriggerTypeClass::PERSISTANT;

    this_trigger_type->Event1 = TEVENT_GLOBAL_CLEAR;
    this_trigger_type->EventControl = MULTI_ONLY;
    this_trigger_type->Event1.Data.Value = in_global;

    // Create instance of trigger type
    TriggerClass* this_trigger = Find_Or_Make(this_trigger_type);

    if (in_function[0] != '\0') {
        // Copy the callback function into the trigger
        strncpy(this_trigger->MapScriptCallback, in_function, sizeof(this_trigger->MapScriptCallback) - 1);
    }

    LogicTriggers.Add(this_trigger);

    // Output the new trigger's ID
    lua_pushnumber(L, this_trigger->ID);

    return 1;
}

/***********************************************************************************************
 * Script_LowPowerCallback - Initiates when given house becomes low on power                   *
 *                                                                                             *
 *   SCRIPT INPUT:	houseType (int)       - The house (player) index that has been discovered  *
 *                  in_function (string)  - The callback function to execute upon trigger      *
 *                                                                                             *
 *   SCRIPT OUTPUT:  triggerID - The ID of the trigger created for use with Trigger Functions  *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_LowPowerCallback(lua_State* L) {

    int houseType = lua_tointeger(L, 1);

    char* in_function = "";

    // Optional callback function
    if (lua_gettop(L) >= 2) {
        in_function = (char*)lua_tostring(L, 2);
    }

    // A house must be specified
    if (houseType >= 0 && houseType < HouseTypes.Count()) {

        // Create trigger type
        TriggerTypeClass* this_trigger_type = new TriggerTypeClass();
        this_trigger_type->House = HOUSE_NONE;
        this_trigger_type->IsActive = 1;
        this_trigger_type->IsPersistant = TriggerTypeClass::VOLATILE;

        this_trigger_type->Event1 = TEVENT_LOW_POWER;
        this_trigger_type->EventControl = MULTI_ONLY;

        this_trigger_type->Event1.Data.House = (HousesType)houseType;

        // Create instance of trigger type

        TriggerClass* this_trigger = Find_Or_Make(this_trigger_type);

        if (in_function[0] != '\0') {
            // Copy the callback function into the trigger
            strncpy(this_trigger->MapScriptCallback, in_function, sizeof(this_trigger->MapScriptCallback) - 1);
        }

        HouseTriggers[PlayerPtr->Class->House].Add(this_trigger);

        // Output the new trigger's ID
        lua_pushnumber(L, this_trigger->ID);

        return 1;

    }

    lua_pushnumber(L, -1);

    return 1;
}

/***********************************************************************************************
 * Script_AllBridgesDestroyedCallback - Called when all map's bridges have been destroyed      *
 *                                                                                             *
 *   SCRIPT INPUT:	in_function (string)  - The callback function to execute upon trigger      *
 *                                                                                             *
 *   SCRIPT OUTPUT:  triggerID - The ID of the trigger created for use with Trigger Functions  *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_AllBridgesDestroyedCallback(lua_State* L) {

    char* in_function = "";

    // Optional callback function
    if (lua_gettop(L) >= 1) {
        in_function = (char*)lua_tostring(L, 1);
    }

    // Create trigger type
    TriggerTypeClass* this_trigger_type = new TriggerTypeClass();
    this_trigger_type->House = HousesType::HOUSE_NONE;
    this_trigger_type->IsActive = 1;
    this_trigger_type->IsPersistant = TriggerTypeClass::PERSISTANT;

    this_trigger_type->Event1 = TEVENT_ALL_BRIDGES_DESTROYED;
    this_trigger_type->EventControl = MULTI_ONLY;

    // Create instance of trigger type
    TriggerClass* this_trigger = Find_Or_Make(this_trigger_type);

    if (in_function[0] != '\0') {
        // Copy the callback function into the trigger
        strncpy(this_trigger->MapScriptCallback, in_function, sizeof(this_trigger->MapScriptCallback) - 1);
    }

    LogicTriggers.Add(this_trigger);

    // Output the new trigger's ID
    lua_pushnumber(L, this_trigger->ID);

    return 1;

}

/***********************************************************************************************
 * Script_BuildingExistsCallback - Called if a given building exists for a given house         *
 *                                                                                             *
 *   SCRIPT INPUT:	buildingType (int)    - The building type to check for                     *
 *                  houseType (int)       - The house (player) index owner of said building    *
 *                  in_function (string)  - The callback function to execute upon trigger      *
 *                                                                                             *
 *   SCRIPT OUTPUT:  triggerID - The ID of the trigger created for use with Trigger Functions  *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_BuildingExistsCallback(lua_State* L) {

    int buildingType = lua_tointeger(L, 1);
    int houseType = lua_tointeger(L, 2);

    char* in_function = "";

    // Optional callback function
    if (lua_gettop(L) >= 3) {
        in_function = (char*)lua_tostring(L, 3);
    }

    // A house must be specified
    if (buildingType >= 0 && houseType >= 0 && houseType < HouseTypes.Count()) {

        // Create trigger type
        TriggerTypeClass* this_trigger_type = new TriggerTypeClass();
        this_trigger_type->House = (HousesType)houseType;
        this_trigger_type->IsActive = 1;
        this_trigger_type->IsPersistant = TriggerTypeClass::VOLATILE;
        this_trigger_type->Event1 = TEVENT_BUILDING_EXISTS;
        this_trigger_type->EventControl = MULTI_ONLY;
        this_trigger_type->Event1.Data.Value = buildingType;

        // Create instance of trigger type
        TriggerClass* this_trigger = Find_Or_Make(this_trigger_type);

        if (in_function[0] != '\0') {
            // Copy the callback function into the trigger
            strncpy(this_trigger->MapScriptCallback, in_function, sizeof(this_trigger->MapScriptCallback) - 1);
        }

        HouseTriggers[(HousesType)houseType].Add(this_trigger);

        // Output the new trigger's ID
        lua_pushnumber(L, this_trigger->ID);

        return 1;

    }

    lua_pushnumber(L, -1);

    return 1;
}

/**********************************************************************************************
* Trigger Utility Functions                                                                   *
*=============================================================================================*/

/***********************************************************************************************
 * Script_Script_TriggerAddCell - Adds a cell to any cell based callback                       *
 *                                                                                             *
 *  The map editor will always be the fastest way to do this, but here in case it's needed     *
 *                                                                                             *
 *   SCRIPT INPUT:	triggerIndex (int)       - The trigger ID of which to add a cell           *
 *                  in_x (int)               - The Cell/X location of the new cell             *
 *                  in_y (int)               - The Cell/Y location of the new cell             *
 *                  in_x2 (int) (opt. gr. 2) - The second Cell/X location (for bounding box)   *
 *                  in_y2 (int) (opt. gr. 2) - The second Cell/Y location (for bounding box)   *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_TriggerAddCell(lua_State* L) {

    int triggerIndex = lua_tointeger(L, 1);

    int in_x = lua_tointeger(L, 2);
    int in_y = lua_tointeger(L, 3);

    int in_x2 = in_x;
    int in_y2 = in_y;

    // Optional secondary group bounding box parameter
    if (lua_gettop(L) == 5) {
        in_x2 = lua_tointeger(L, 4);
        in_y2 = lua_tointeger(L, 5);
    }

    TriggerClass* this_trigger = Triggers.Ptr(triggerIndex);

    if (this_trigger != NULL) {
        for (int _x = in_x; _x <= in_x2; _x++) {
            for (int _y = in_y; _y <= in_y2; _y++) {
                Map[XY_Cell(_x, _y)].Trigger = this_trigger;
            }
        }
    }



    return 1;
}

/***********************************************************************************************
 * Script_SetTriggerCallback - Adds a lua callback to an existing trigger                      *
 *                                                                                             *
 *   SCRIPT INPUT:	triggerIndex (int)       - The trigger of which to get                     *
 *                                                                                             *
 *                	callbackName (string)    - The function name to be called when the         *
 *                                             actions have been met                           *
 *                                                                                             *
 *                  actionIndex (int) (opt.) - 0: always callback on any of the given paths    *
 *                                                of the trigger                               *
 *                                             1: only callback on first action                *
 *                                             2: only callback on second action               *
 *                                                                                             *
 *   SCRIPT OUTPUT:  result (number) - The amount of matching buildings for that player        *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_SetTriggerCallback(lua_State* L) {

    int triggerIndex = lua_tointeger(L, 1);

    const char* callbackName = lua_tostring(L, 2);
    char actionIndex = 0;

    // Optional actionIndex parameter
    if (lua_gettop(L) == 3) {
        actionIndex = lua_tointeger(L, 2);
    }

    TriggerClass* trigger = Triggers.Ptr(triggerIndex);

    if (trigger != NULL) {

        strncpy(trigger->MapScriptCallback, callbackName, sizeof(trigger->MapScriptCallback) - 1);
        trigger->MapScriptActionIndex = actionIndex;

    }

    return 1;
}

/***********************************************************************************************
 * Script_GetCellObject - Gets an object (if any) on a cell and returns the ID                 *
 *                                                                                             *
 *   SCRIPT INPUT:	in_x (int)       - The cell/X location of the cell to pick from            *
 *                  in_y (int)       - The cell/Y location of the cell to pick from            *
 *                                                                                             *
 *   SCRIPT OUTPUT:  result (number) - The ID of the building on the cell, or -1 if none       *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_GetCellObject(lua_State* L) {

    int in_x = lua_tointeger(L, 1);
    int in_y = lua_tointeger(L, 2);

    long result = -1;

    ObjectClass* this_object = Map[XY_Cell(in_x, in_y)].Cell_Object();

    if (this_object == NULL) {

        lua_pushnumber(L, -1);
        return 1;
    }

    MapScriptObject* CacheObject = Scen.mapScript->CreateCacheObject();

    CacheObject->ID = this_object->ID;
    CacheObject->RTTI = this_object->RTTI;
    CacheObject->classIndex = -1;

    switch (this_object->RTTI) {
    case RTTI_BUILDING: {
        CacheObject->classIndex = Script_BuildingIndexFromID(this_object->ID);
    }break;
    case RTTI_UNIT: {
        CacheObject->classIndex = Script_UnitIndexFromID(this_object->ID);
    }break;
    case RTTI_AIRCRAFT: {
        CacheObject->classIndex = Script_AircraftIndexFromID(this_object->ID);
    }break;
    case RTTI_INFANTRY: {
        CacheObject->classIndex = Script_InfantryIndexFromID(this_object->ID);
    }break;
    case RTTI_VESSEL: {
        CacheObject->classIndex = Script_VesselIndexFromID(this_object->ID);
    }break;
    }

    if (CacheObject->classIndex == -1 || CacheObject->ID < 0) {
        delete CacheObject;
        lua_pushnumber(L, -1);
    }
    else {
        Scen.mapScript->ObjectCache.push_back(CacheObject);
        lua_pushnumber(L, CacheObject->UID); // Return UID for faster lookup
    }

    return 1;
}

/***********************************************************************************************
 * Script_Merge_Triggers - Adds the event from trigger 2 to trigger 1 and discards trigger 2   *
 *                                                                                             *
 *   SCRIPT INPUT:	triggerIndex (int)     - The trigger ID of which will be the result        *
 *                  mergeTriggerIndex (int)- The trigger ID of which is being merged           *
 *                                                                                             *
 *   SCRIPT OUTPUT:  result (number) - The amount of matching buildings for that player        *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_MergeTriggers(lua_State* L) {

    int triggerIndex = lua_tointeger(L, 1);
    int mergeTriggerIndex = lua_tointeger(L, 2);

    TriggerClass* resulting_trigger = Triggers.Ptr(triggerIndex);
    TriggerTypeClass* resulting_trigger_type = resulting_trigger->Class;
    TriggerClass* merge_trigger = Triggers.Ptr(mergeTriggerIndex);

    if (resulting_trigger != NULL && merge_trigger != NULL) {

        // Set event 2 to event 1 of the merge trigger
        resulting_trigger_type->Event2.Event = (TEventType)merge_trigger->Class->Event1.Event;
        resulting_trigger_type->Event2.Data.Value = (long)merge_trigger->Class->Event1.Data.Value;
        resulting_trigger_type->Event2.Team = (TeamTypeClass*)merge_trigger->Class->Event1.Team;

        // Make this an "AND" coniditional trigger (event 1 AND event 2)
        resulting_trigger->Class->EventControl = MULTI_AND;
        resulting_trigger->Class->ActionControl = MULTI_ONLY;

        // TODO: only do these individual cleanups for actions that they represent (for efficiency)

        // Now switch over any LogicTrigger references (we're getting rid of the second trigger)
        for (int t_index = 0; t_index < LogicTriggers.Count(); t_index++) {

            TriggerClass* _trigger = LogicTriggers[t_index];

            if (_trigger == merge_trigger) {

                LogicTriggers.Delete(t_index);

                bool find_existing_result;
                for (int t2_index = 0; t2_index < LogicTriggers.Count(); t2_index++) {
                    if (LogicTriggers[t2_index] == resulting_trigger) {
                        find_existing_result = true;
                        break;
                    }
                }
                if (!find_existing_result) {
                    LogicTriggers.Add(resulting_trigger);
                }

                break;
            }

        }

        // Now switch over any MapTrigger references (we're getting rid of the second trigger)
        for (int t_index = 0; t_index < MapTriggers.Count(); t_index++) {
            TriggerClass* _trigger = MapTriggers[t_index];

            if (_trigger != NULL) {
                if (_trigger == merge_trigger) {

                    MapTriggers.Delete(t_index);

                    bool find_existing_result;
                    for (int t2_index = 0; t2_index < MapTriggers.Count(); t2_index++) {
                        if (MapTriggers[t2_index] == resulting_trigger) {
                            find_existing_result = true;
                            break;
                        }
                    }
                    if (!find_existing_result) {
                        MapTriggers.Add(resulting_trigger);
                    }

                    break;
                }
            }

        }

        // Now switch over any HouseTrigger references (we're getting rid of the second trigger)
        for (int h_index = 0; h_index < Houses.Count(); h_index++) {
            for (int t_index = 0; t_index < HouseTriggers[h_index].Count(); t_index++) {

                TriggerClass* _trigger = HouseTriggers[h_index][t_index];

                if (_trigger != NULL) {
                    if (_trigger == merge_trigger) {

                        HouseTriggers[h_index].Delete(t_index);

                        bool find_existing_result;
                        for (int t2_index = 0; t2_index < HouseTriggers[h_index].Count(); t2_index++) {
                            if (HouseTriggers[h_index][t2_index] == resulting_trigger) {
                                find_existing_result = true;
                                break;
                            }
                        }
                        if (!find_existing_result) {
                            HouseTriggers[h_index].Add(resulting_trigger);
                        }

                        break;
                    }
                }

            }
        }

        // Go through the map and replace any celltriggers
        for (int _x = Map.MapCellX; _x <= Map.MapCellX + Map.MapCellWidth; _x++) {
            for (int _y = Map.MapCellY; _y <= Map.MapCellY + Map.MapCellHeight; _y++) {
                if (Map[XY_Cell(_x, _y)].Trigger == merge_trigger) {
                    Map[XY_Cell(_x, _y)].Trigger = resulting_trigger;
                }
            }
        }

        // Now that we've merged the contents, and it's no longer being referred to, we can safely delete the merge trigger
        Detach_This_From_All(merge_trigger->As_Target());
        delete merge_trigger;
    }

    return 1;
}

/***********************************************************************************************
 * Script_GetTriggerByName - Gets the trigger with a given name                                *
 *                                                                                             *
 *   SCRIPT INPUT:	triggerName (string); The input trigger name                               *
 *                                                                                             *
 *   SCRIPT OUTPUT:  triggerID (int) - The index of the trigger with this name                 *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_GetTriggerByName(lua_State* L) {

    const char* triggerName = lua_tostring(L, 1);

    // Find the trigger and set up callback
    for (int t_index = 0; t_index < Triggers.Count(); t_index++) {
        TriggerClass* trigger = Triggers.Ptr(t_index);

        if (trigger != NULL) {

            if (strcmp(trigger->Name(), triggerName) == 0) {

                TriggerTypeClass* this_type = trigger->Class;
                lua_pushnumber(L, t_index);

                return 1;

                break;
            }

        }

    }

    lua_pushnumber(L, -1);

    return -1;
}

/***********************************************************************************************
 * Script_TriggerSetPersistence - Sets whether a trigger runs once or indefinitely             *
 *                                                                                             *
 *   SCRIPT INPUT:	triggerIndex (int)     - The trigger ID of which this will apply           *
 *                	isPersistent (int)     - The persistence value                             *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_TriggerSetPersistence(lua_State* L) {

    int triggerIndex = lua_tointeger(L, 1);
    unsigned char isPersistent = lua_tointeger(L, 2);

    TriggerClass* trigger = Triggers.Ptr(triggerIndex);

    if (trigger != NULL) {

        TriggerTypeClass* trigger_type = trigger->Class;

        trigger_type->IsPersistant = (TriggerTypeClass::PersistantType)isPersistent;


    }

    return -1;
}


/**********************************************************************************************
* Modification Utility Functions                                                              *
*=============================================================================================*/

/**********************************************************************************************
* Script_SetObjectAttribute - Sets attributes for specific objects or object types            *
*                                                                                             *
*   SCRIPT INPUT:	objectID        (int)  - The ID of the object                             *
*                	attribute_type  (int)  - The attribute to change                          *
*                	attribute_value (int)  - The value to set the attribute to                *
*                                                                                             *
*   SCRIPT OUTPUT:  void                                                                      *
*                                                                                             *
* INPUT:  lua_State - The current Lua state                                                   *
*                                                                                             *
* OUTPUT:  int; Did the function run successfully? Return 1                                   *
*                                                                                             *
* WARNINGS:  ?                                                                                *
*                                                                                             *
*=============================================================================================*/
static int Script_SetObjectAttribute(lua_State* L) {

    int objectID = lua_tointeger(L, 1);
    int attribute_type = lua_tointeger(L, 2);

    ObjectClass* this_object = Script_GetCacheObject(objectID);

    // Get the appropriate technotype
    if (this_object != NULL) {

        // Global attributes / Multi object attributes
        switch ((MapScriptObjectAttributeType)attribute_type) {
        case ATTRIBUTE_TYPE: {

        }break;
        case ATTRIBUTE_STRENGTH: {

            int attribute_value = lua_tointeger(L, 3);
            this_object->Strength = attribute_value;

        }break;

        case ATTRIBUTE_IS_REPAIRING: {

            int attribute_value = lua_tointeger(L, 3);

            switch (this_object->RTTI) {
            case RTTI_BUILDING: {

                BuildingClass* this_building = (BuildingClass*)this_object;
                this_building->IsRepairing = attribute_value;

            }break;
            case RTTI_VESSEL: {

                VesselClass* this_vessel = (VesselClass*)this_object;
                this_vessel->IsSelfRepairing = attribute_value;

            }break;
            }
        }break;

            // Object Class specific
        default: {

            TechnoTypeClass* this_type_class;

            switch (this_object->RTTI) {
            case RTTI_BUILDING:

                this_type_class = BuildingTypes.Ptr(this_object->Class_Of().ID);

                break;
            case RTTI_UNIT:

                this_type_class = UnitTypes.Ptr(this_object->Class_Of().ID);

                break;
            case RTTI_INFANTRY:

                this_type_class = InfantryTypes.Ptr(this_object->Class_Of().ID);

                break;
            case RTTI_VESSEL:

                this_type_class = VesselTypes.Ptr(this_object->Class_Of().ID);

                break;
            case RTTI_AIRCRAFT:

                this_type_class = AircraftTypes.Ptr(this_object->Class_Of().ID);

                break;
            }


            if (this_type_class != NULL) {
                Script_SetObjectTypeAttribute(L, this_type_class, (MapScriptObjectAttributeType)attribute_type);
            }

        }break;

        }

    }


    return -1;
}

/**********************************************************************************************
* Script_SetBuildingTypeAttribute - Sets attributes for a building types                      *
*                                                                                             *
*   SCRIPT INPUT:	buildingTypeID  (int)  - The ID of the building type class                *
*                	attribute_type  (int)  - The attribute to change                          *
*                	attribute_value (int)  - The value to set the attribute to                *
*                                                                                             *
*   SCRIPT OUTPUT:  void                                                                      *
*                                                                                             *
* INPUT:  lua_State - The current Lua state                                                   *
*                                                                                             *
* OUTPUT:  int; Did the function run successfully? Return 1                                   *
*                                                                                             *
* WARNINGS:  ?                                                                                *
*                                                                                             *
*=============================================================================================*/
static int Script_SetBuildingTypeAttribute(lua_State* L) {

    int buildingTypeID = lua_tointeger(L, 1);
    int attribute_type = lua_tointeger(L, 2);

    TechnoTypeClass* this_type_class = BuildingTypes.Ptr(buildingTypeID);

    if (this_type_class != NULL) {
        Script_SetObjectTypeAttribute(L, this_type_class, (MapScriptObjectAttributeType)attribute_type);
    }

    return -1;
}

/**********************************************************************************************
* Script_SetVehicleTypeAttribute - Sets attributes for vehicle types                          *
*                                                                                             *
*   SCRIPT INPUT:	vehicleTypeID  (int)   - The ID of the vehicle type class                 *
*                	attribute_type  (int)  - The attribute to change                          *
*                	attribute_value (int)  - The value to set the attribute to                *
*                                                                                             *
*   SCRIPT OUTPUT:  void                                                                      *
*                                                                                             *
* INPUT:  lua_State - The current Lua state                                                   *
*                                                                                             *
* OUTPUT:  int; Did the function run successfully? Return 1                                   *
*                                                                                             *
* WARNINGS:  ?                                                                                *
*                                                                                             *
*=============================================================================================*/
static int Script_SetVehicleTypeAttribute(lua_State* L) {

    int vehicleTypeID = lua_tointeger(L, 1);
    int attribute_type = lua_tointeger(L, 2);

    TechnoTypeClass* this_type_class = UnitTypes.Ptr(vehicleTypeID);

    if (this_type_class != NULL) {
        Script_SetObjectTypeAttribute(L, this_type_class, (MapScriptObjectAttributeType)attribute_type);
    }

    return -1;
}

/**********************************************************************************************
* Script_SetInfantryTypeAttribute - Sets attributes for infantry types                        *
*                                                                                             *
*   SCRIPT INPUT:	infantryTypeID  (int)  - The ID of the infantry type class                *
*                	attribute_type  (int)  - The attribute to change                          *
*                	attribute_value (int)  - The value to set the attribute to                *
*                                                                                             *
*   SCRIPT OUTPUT:  void                                                                      *
*                                                                                             *
* INPUT:  lua_State - The current Lua state                                                   *
*                                                                                             *
* OUTPUT:  int; Did the function run successfully? Return 1                                   *
*                                                                                             *
* WARNINGS:  ?                                                                                *
*                                                                                             *
*=============================================================================================*/
static int Script_SetInfantryTypeAttribute(lua_State* L) {

    int infantryTypeID = lua_tointeger(L, 1);
    int attribute_type = lua_tointeger(L, 2);

    TechnoTypeClass* this_type_class = InfantryTypes.Ptr(infantryTypeID);

    if (this_type_class != NULL) {
        Script_SetObjectTypeAttribute(L, this_type_class, (MapScriptObjectAttributeType)attribute_type);
    }

    return -1;
}

/**********************************************************************************************
* Script_SetVesselTypeAttribute - Sets attributes for vessel types                            *
*                                                                                             *
*   SCRIPT INPUT:	vesselTypeID    (int)  - The ID of the vessel type class                  *
*                	attribute_type  (int)  - The attribute to change                          *
*                	attribute_value (int)  - The value to set the attribute to                *
*                                                                                             *
*   SCRIPT OUTPUT:  void                                                                      *
*                                                                                             *
* INPUT:  lua_State - The current Lua state                                                   *
*                                                                                             *
* OUTPUT:  int; Did the function run successfully? Return 1                                   *
*                                                                                             *
* WARNINGS:  ?                                                                                *
*                                                                                             *
*=============================================================================================*/
static int Script_SetVesselTypeAttribute(lua_State* L) {

    int vesselTypeID = lua_tointeger(L, 1);
    int attribute_type = lua_tointeger(L, 2);

    TechnoTypeClass* this_type_class = VesselTypes.Ptr(vesselTypeID);

    if (this_type_class != NULL) {
        Script_SetObjectTypeAttribute(L, this_type_class, (MapScriptObjectAttributeType)attribute_type);
    }

    return -1;
}

/**********************************************************************************************
* Script_SetAircraftTypeAttribute - Sets attributes for aircraft types                        *
*                                                                                             *
*   SCRIPT INPUT:	aircraftTypeID  (int)  - The ID of the aircraft type class                *
*                	attribute_type  (int)  - The attribute to change                          *
*                	attribute_value (int)  - The value to set the attribute to                *
*                                                                                             *
*   SCRIPT OUTPUT:  void                                                                      *
*                                                                                             *
* INPUT:  lua_State - The current Lua state                                                   *
*                                                                                             *
* OUTPUT:  int; Did the function run successfully? Return 1                                   *
*                                                                                             *
* WARNINGS:  ?                                                                                *
*                                                                                             *
*=============================================================================================*/
static int Script_SetAircraftTypeAttribute(lua_State* L) {

    int aircraftTypeID = lua_tointeger(L, 1);
    int attribute_type = lua_tointeger(L, 2);

    TechnoTypeClass* this_type_class = AircraftTypes.Ptr(aircraftTypeID);

    if (this_type_class != NULL) {
        Script_SetObjectTypeAttribute(L, this_type_class, (MapScriptObjectAttributeType)attribute_type);
    }

    return -1;
}

/**********************************************************************************************
* Script_SetObjectTypeAttribute - Sets attributes for specific object types globally          *
*                                                                                             *
*  Used internally by MapScript to retrieve input and set attributes for TechnoTypeClass      *
*                                                                                             *
*   SCRIPT INPUT:	this_type_class (TechnoTypeClass*) - The techno type                      *
*                	attribute_type  (MapScriptObjectAttributeType)  - The attribute to change       *
*                                                                                             *
*   SCRIPT OUTPUT:  void                                                                      *
*                                                                                             *
* WARNINGS:  ?                                                                                *
*                                                                                             *
*=============================================================================================*/
static int Script_SetObjectTypeAttribute(lua_State* L, TechnoTypeClass* this_type_class, MapScriptObjectAttributeType attribute_type) {

    switch (attribute_type) {
    case ATTRIBUTE_MAX_STRENGTH: {

        int attribute_value = lua_tointeger(L, 3);

        // Cache default value if first time changing this attribute
        Script_CacheDefaultObjectTypeAttribute(this_type_class->RTTI, this_type_class->ID, attribute_type, this_type_class->MaxStrength);

        // Set the attribute
        this_type_class->MaxStrength = attribute_value;

    }break;

    case ATTRIBUTE_COST: {

        int attribute_value = lua_tointeger(L, 3);

        // Cache default value if first time changing this attribute
        Script_CacheDefaultObjectTypeAttribute(this_type_class->RTTI, this_type_class->ID, attribute_type, this_type_class->Cost);

        // Set the attribute
        this_type_class->Cost = attribute_value;

    }break;
    case ATTRIBUTE_SIGHT_RANGE: {

        int attribute_value = lua_tointeger(L, 3);

        // Cache default value if first time changing this attribute
        Script_CacheDefaultObjectTypeAttribute(this_type_class->RTTI, this_type_class->ID, attribute_type, this_type_class->SightRange);

        // Set the attribute
        this_type_class->SightRange = attribute_value;

    }break;
    case ATTRIBUTE_TECH_LEVEL: {

        int attribute_value = lua_tointeger(L, 3);

        // Cache default value if first time changing this attribute
        Script_CacheDefaultObjectTypeAttribute(this_type_class->RTTI, this_type_class->ID, attribute_type, this_type_class->Level);

        // Set the attribute
        this_type_class->Level = attribute_value;

    }break;
    case ATTRIBUTE_ARMOR: {

        int attribute_value = lua_tointeger(L, 3);

        // Cache default value if first time changing this attribute
        Script_CacheDefaultObjectTypeAttribute(this_type_class->RTTI, this_type_class->ID, attribute_type, this_type_class->Armor);

        // Set the attribute
        this_type_class->Armor = (ArmorType)attribute_value;

    }break;
    case ATTRIBUTE_PRIMARY_WEAPON: {

        int attribute_value = lua_tointeger(L, 3);

        if (this_type_class->PrimaryWeapon != NULL) {
            // Cache default value if first time changing this attribute
            Script_CacheDefaultObjectTypeAttribute(this_type_class->RTTI, this_type_class->ID, attribute_type, this_type_class->PrimaryWeapon->ID);
        }
        else {
            // Cache default value if first time changing this attribute
            Script_CacheDefaultObjectTypeAttribute(this_type_class->RTTI, this_type_class->ID, attribute_type, -1);
        }

        // Set the attribute
        if (attribute_value < 0) {
            this_type_class->PrimaryWeapon = NULL;
        }
        else {
            WeaponTypeClass* this_weapon = Weapons.Ptr(attribute_value);
            if (this_weapon != NULL) {
                this_type_class->PrimaryWeapon = this_weapon;
            }
            else {
                this_type_class->PrimaryWeapon = NULL;
            }
        }

    }break;
    case ATTRIBUTE_SECONDARY_WEAPON: {

        int attribute_value = lua_tointeger(L, 3);

        if (this_type_class->PrimaryWeapon != NULL) {
            // Cache default value if first time changing this attribute
            Script_CacheDefaultObjectTypeAttribute(this_type_class->RTTI, this_type_class->ID, attribute_type, this_type_class->SecondaryWeapon->ID);
        }
        else {
            // Cache default value if first time changing this attribute
            Script_CacheDefaultObjectTypeAttribute(this_type_class->RTTI, this_type_class->ID, attribute_type, -1);
        }

        // Set the attribute
        if (attribute_value < 0) {
            this_type_class->SecondaryWeapon = NULL;
        }
        else {
            WeaponTypeClass* this_weapon = Weapons.Ptr(attribute_value);
            if (this_weapon != NULL) {
                this_type_class->SecondaryWeapon = this_weapon;
            }
            else {
                this_type_class->SecondaryWeapon = NULL;
            }
        }

    }break;
    case ATTRIBUTE_SPEED: {

        int attribute_value = lua_tointeger(L, 3);

        // Cache default value if first time changing this attribute
        Script_CacheDefaultObjectTypeAttribute(this_type_class->RTTI, this_type_class->ID, attribute_type, this_type_class->Speed);

        // Set the attribute
        this_type_class->Speed = (SpeedType)attribute_value;

    }break;
    case ATTRIBUTE_MAX_SPEED: {

        int attribute_value = lua_tointeger(L, 3);

        // Cache default value if first time changing this attribute
        Script_CacheDefaultObjectTypeAttribute(this_type_class->RTTI, this_type_class->ID, attribute_type, this_type_class->MaxSpeed);

        // Set the attribute
        this_type_class->MaxSpeed = (MPHType)attribute_value;

    }break;
    case ATTRIBUTE_MAX_AMMO: {

        int attribute_value = lua_tointeger(L, 3);

        // Cache default value if first time changing this attribute
        Script_CacheDefaultObjectTypeAttribute(this_type_class->RTTI, this_type_class->ID, attribute_type, this_type_class->MaxAmmo);

        // Set the attribute
        this_type_class->MaxAmmo = attribute_value;

    }break;
    case ATTRIBUTE_ROTATION_SPEED: {

        int attribute_value = lua_tointeger(L, 3);

        // Cache default value if first time changing this attribute
        Script_CacheDefaultObjectTypeAttribute(this_type_class->RTTI, this_type_class->ID, attribute_type, this_type_class->ROT);

        // Set the attribute
        this_type_class->ROT = attribute_value;

    }break;
    case ATTRIBUTE_CRUSHABLE: {

        unsigned int attribute_value = lua_tointeger(L, 3);

        // Cache default value if first time changing this attribute
        Script_CacheDefaultObjectTypeAttribute(this_type_class->RTTI, this_type_class->ID, attribute_type, this_type_class->IsCrushable);

        // Set the attribute
        this_type_class->IsCrushable = attribute_value;

    }break;
    case ATTRIBUTE_STEALTHY: {

        unsigned int attribute_value = lua_tointeger(L, 3);

        // Cache default value if first time changing this attribute
        Script_CacheDefaultObjectTypeAttribute(this_type_class->RTTI, this_type_class->ID, attribute_type, this_type_class->IsStealthy);

        // Set the attribute
        this_type_class->IsStealthy = attribute_value;

    }break;
    case ATTRIBUTE_SELECTABLE: {

        unsigned int attribute_value = lua_tointeger(L, 3);

        // Cache default value if first time changing this attribute
        Script_CacheDefaultObjectTypeAttribute(this_type_class->RTTI, this_type_class->ID, attribute_type, this_type_class->IsSelectable);

        // Set the attribute
        this_type_class->IsSelectable = attribute_value;

    }break;
    case ATTRIBUTE_LEGAL_TARGET: {

        unsigned int attribute_value = lua_tointeger(L, 3);

        // Cache default value if first time changing this attribute
        Script_CacheDefaultObjectTypeAttribute(this_type_class->RTTI, this_type_class->ID, attribute_type, this_type_class->IsLegalTarget);

        // Set the attribute
        this_type_class->IsLegalTarget = attribute_value;

    }break;
    case ATTRIBUTE_REPAIRABLE: {

        unsigned int attribute_value = lua_tointeger(L, 3);

        // Cache default value if first time changing this attribute
        Script_CacheDefaultObjectTypeAttribute(this_type_class->RTTI, this_type_class->ID, attribute_type, this_type_class->IsRepairable);

        // Set the attribute
        this_type_class->IsRepairable = attribute_value;

    }break;
    case ATTRIBUTE_SELF_HEALING: {

        unsigned int attribute_value = lua_tointeger(L, 3);

        // Cache default value if first time changing this attribute
        Script_CacheDefaultObjectTypeAttribute(this_type_class->RTTI, this_type_class->ID, attribute_type, this_type_class->IsSelfHealing);

        // Set the attribute
        this_type_class->IsSelfHealing = attribute_value;

    }break;
    case ATTRIBUTE_EXPLODES: {

        unsigned int attribute_value = lua_tointeger(L, 3);

        // Cache default value if first time changing this attribute
        Script_CacheDefaultObjectTypeAttribute(this_type_class->RTTI, this_type_class->ID, attribute_type, this_type_class->IsExploding);

        // Set the attribute
        this_type_class->IsExploding = attribute_value;

    }break;
    case ATTRIBUTE_HAS_CREW: {

        unsigned int attribute_value = lua_tointeger(L, 3);

        // Cache default value if first time changing this attribute
        Script_CacheDefaultObjectTypeAttribute(this_type_class->RTTI, this_type_class->ID, attribute_type, this_type_class->IsCrew);

        // Set the attribute
        this_type_class->IsCrew = attribute_value;

    }break;
    case ATTRIBUTE_MAX_PASSENGERS: {

        int attribute_value = lua_tointeger(L, 3);

        // Cache default value if first time changing this attribute
        Script_CacheDefaultObjectTypeAttribute(this_type_class->RTTI, this_type_class->ID, attribute_type, this_type_class->MaxPassengers);

        // Set the attribute
        this_type_class->MaxPassengers = attribute_value;

    }break;
    default: {

        // Object class specific attributes
        // Welcome to the web of switches]

        // Type of object
        switch (this_type_class->RTTI) {

            // Building Specific
        case RTTI_BUILDINGTYPE: {

            // What type of attribute?
            switch (attribute_type) {
            case ATTRIBUTE_CAPACITY: {

                int attribute_value = lua_tointeger(L, 3);

                BuildingTypeClass* this_building_class = (BuildingTypeClass*)this_type_class;

                // Cache default value if first time changing this attribute
                Script_CacheDefaultObjectTypeAttribute(this_type_class->RTTI, this_type_class->ID, attribute_type, this_building_class->Capacity);

                // Set the attribute
                this_building_class->Capacity = attribute_value;

            }break;
            case ATTRIBUTE_POWER: {

                int attribute_value = lua_tointeger(L, 3);

                // Both power and drain are different variables
                // And so we do some trickery here to fill
                // the right values based on postivie/negative input

                BuildingTypeClass* this_building_class = (BuildingTypeClass*)this_type_class;

                // Cache default value if first time changing this attribute
                Script_CacheDefaultObjectTypeAttribute(this_type_class->RTTI, this_type_class->ID, attribute_type, this_building_class->Power - this_building_class->Drain);

                // Set the attribute
                if (attribute_value > 0) {
                    this_building_class->Power = attribute_value;
                    this_building_class->Drain = 0;
                }
                else {
                    this_building_class->Power = 0;
                    this_building_class->Drain = attribute_value * -1; // Flip negative power input
                }

            }break;
            case ATTRIBUTE_SELLABLE: {

                int attribute_value = lua_tointeger(L, 3);

                BuildingTypeClass* this_building_class = (BuildingTypeClass*)this_type_class;

                // Cache default value if first time changing this attribute
                Script_CacheDefaultObjectTypeAttribute(this_type_class->RTTI, this_type_class->ID, attribute_type, this_building_class->IsUnsellable * -1);

                // Set the attribute
                // Personal opinion; sellable makes more sense than unsellable
                // This replacement could be seen as IsUnNonSellable, or... IsSellable
                if (attribute_value == 1) {
                    this_building_class->IsUnsellable = 0;
                }
                else {
                    this_building_class->IsUnsellable = 1;
                }

            }break;
            }

        }break;


            // Unit Specific
        case RTTI_UNITTYPE: {

            // What type of attribute?
            switch (attribute_type) {
            case ATTRIBUTE_CRATE_GOODIE: {

                int attribute_value = lua_tointeger(L, 3);

                UnitTypeClass* this_unit_class = (UnitTypeClass*)this_type_class;

                // Cache default value if first time changing this attribute
                Script_CacheDefaultObjectTypeAttribute(this_type_class->RTTI, this_type_class->ID, attribute_type, this_unit_class->IsCrateGoodie);

                // Set the attribute
                this_unit_class->IsCrateGoodie = attribute_value;

            }break;
            case ATTRIBUTE_CRUSHER: {

                int attribute_value = lua_tointeger(L, 3);

                UnitTypeClass* this_unit_class = (UnitTypeClass*)this_type_class;

                // Cache default value if first time changing this attribute
                Script_CacheDefaultObjectTypeAttribute(this_type_class->RTTI, this_type_class->ID, attribute_type, this_unit_class->IsCrusher);

                // Set the attribute
                this_unit_class->IsCrusher = attribute_value;

            }break;
            case ATTRIBUTE_HARVEST: {

                int attribute_value = lua_tointeger(L, 3);

                UnitTypeClass* this_unit_class = (UnitTypeClass*)this_type_class;

                // Cache default value if first time changing this attribute
                Script_CacheDefaultObjectTypeAttribute(this_type_class->RTTI, this_type_class->ID, attribute_type, this_unit_class->IsToHarvest);

                // Set the attribute
                this_unit_class->IsToHarvest = attribute_value;

            }break;
            case ATTRIBUTE_RADAR_EQUIPPED: {

                int attribute_value = lua_tointeger(L, 3);

                UnitTypeClass* this_unit_class = (UnitTypeClass*)this_type_class;

                // Cache default value if first time changing this attribute
                Script_CacheDefaultObjectTypeAttribute(this_type_class->RTTI, this_type_class->ID, attribute_type, this_unit_class->IsRadarEquipped);

                // Set the attribute
                this_unit_class->IsRadarEquipped = attribute_value;

            }break;
            case ATTRIBUTE_JAMMER: {

                int attribute_value = lua_tointeger(L, 3);

                UnitTypeClass* this_unit_class = (UnitTypeClass*)this_type_class;

                // Cache default value if first time changing this attribute
                Script_CacheDefaultObjectTypeAttribute(this_type_class->RTTI, this_type_class->ID, attribute_type, this_unit_class->IsJammer);

                // Set the attribute
                this_unit_class->IsJammer = attribute_value;

            }break;
            case ATTRIBUTE_GAPPER: {

                int attribute_value = lua_tointeger(L, 3);

                UnitTypeClass* this_unit_class = (UnitTypeClass*)this_type_class;

                // Cache default value if first time changing this attribute
                Script_CacheDefaultObjectTypeAttribute(this_type_class->RTTI, this_type_class->ID, attribute_type, this_unit_class->IsGapper);

                // Set the attribute
                this_unit_class->IsGapper = attribute_value;

            }break;
            }

        }break;


            // Infantry Specific
        case RTTI_INFANTRYTYPE: {

            // What type of attribute?
            switch (attribute_type) {
            case ATTRIBUTE_FEMALE: {

                int attribute_value = lua_tointeger(L, 3);

                InfantryTypeClass* this_infantry_class = (InfantryTypeClass*)this_type_class;

                // Cache default value if first time changing this attribute
                Script_CacheDefaultObjectTypeAttribute(this_type_class->RTTI, this_type_class->ID, attribute_type, this_infantry_class->IsFemale);

                // Set the attribute
                this_infantry_class->IsFemale = attribute_value;

            }break;
            case ATTRIBUTE_CAPTURE: {

                int attribute_value = lua_tointeger(L, 3);

                InfantryTypeClass* this_infantry_class = (InfantryTypeClass*)this_type_class;

                // Cache default value if first time changing this attribute
                Script_CacheDefaultObjectTypeAttribute(this_type_class->RTTI, this_type_class->ID, attribute_type, this_infantry_class->IsCapture);

                // Set the attribute
                this_infantry_class->IsCapture = attribute_value;

            }break;
            case ATTRIBUTE_FRAIDY_CAT: {

                int attribute_value = lua_tointeger(L, 3);

                InfantryTypeClass* this_infantry_class = (InfantryTypeClass*)this_type_class;

                // Cache default value if first time changing this attribute
                Script_CacheDefaultObjectTypeAttribute(this_type_class->RTTI, this_type_class->ID, attribute_type, this_infantry_class->IsFraidyCat);

                // Set the attribute
                this_infantry_class->IsFraidyCat = attribute_value;

            }break;
            case ATTRIBUTE_CIVILIAN: {

                int attribute_value = lua_tointeger(L, 3);

                InfantryTypeClass* this_infantry_class = (InfantryTypeClass*)this_type_class;

                // Cache default value if first time changing this attribute
                Script_CacheDefaultObjectTypeAttribute(this_type_class->RTTI, this_type_class->ID, attribute_type, this_infantry_class->IsCivilian);

                // Set the attribute
                this_infantry_class->IsCivilian = attribute_value;

            }break;
            case ATTRIBUTE_BOMBER: {

                int attribute_value = lua_tointeger(L, 3);

                InfantryTypeClass* this_infantry_class = (InfantryTypeClass*)this_type_class;

                // Cache default value if first time changing this attribute
                Script_CacheDefaultObjectTypeAttribute(this_type_class->RTTI, this_type_class->ID, attribute_type, this_infantry_class->IsBomber);

                // Set the attribute
                this_infantry_class->IsBomber = attribute_value;

            }break;
            case ATTRIBUTE_DOG: {

                int attribute_value = lua_tointeger(L, 3);

                InfantryTypeClass* this_infantry_class = (InfantryTypeClass*)this_type_class;

                // Cache default value if first time changing this attribute
                Script_CacheDefaultObjectTypeAttribute(this_type_class->RTTI, this_type_class->ID, attribute_type, this_infantry_class->IsDog);

                // Set the attribute
                this_infantry_class->IsDog = attribute_value;

            }break;

            }

        }break;

        } // End of object specific switch

    }break;

    } // End of attribute switch 

    return -1;
}

/**********************************************************************************************
* Script_GetObjectAttribute - Gets attributes for specific objects or object types            *
*                                                                                             *
*   SCRIPT INPUT:	objectID        (int)  - The ID of the object                             *
*                	attribute_type  (int)  - The attribute to change                          *
*                                                                                             *
*   SCRIPT OUTPUT:  void                                                                      *
*                                                                                             *
* INPUT:  lua_State - The current Lua state                                                   *
*                                                                                             *
* OUTPUT:  int; Did the function run successfully? Return 1                                   *
*                                                                                             *
* WARNINGS:  ?                                                                                *
*                                                                                             *
*=============================================================================================*/
static int Script_GetObjectAttribute(lua_State* L) {

    int objectID = lua_tointeger(L, 1);
    int attribute_type = lua_tointeger(L, 2);

    ObjectClass* this_object = Script_GetCacheObject(objectID);

    // Get the appropriate technotype
    if (this_object != NULL) {

        // Global attributes / Multi object attributes
        switch ((MapScriptObjectAttributeType)attribute_type) {
        case ATTRIBUTE_TYPE: {

            // Send the attribute
            lua_pushinteger(L, this_object->RTTI);
            return 1;

        }break;
        case ATTRIBUTE_STRENGTH: {

            // Send the attribute
            lua_pushinteger(L, this_object->Strength);
            return 1;

        }break;

        case ATTRIBUTE_IS_REPAIRING: {

            switch (this_object->RTTI) {
            case RTTI_BUILDING: {

                BuildingClass* this_building = (BuildingClass*)this_object;

                // Send the attribute
                lua_pushinteger(L, this_building->IsRepairing);
                return 1;

            }break;
            case RTTI_VESSEL: {

                VesselClass* this_vessel = (VesselClass*)this_object;

                // Send the attribute
                lua_pushinteger(L, this_vessel->IsSelfRepairing);
                return 1;

            }break;
            }
        }break;

            // Object Class specific
        default: {

            TechnoTypeClass* this_type_class = NULL;

            switch (this_object->RTTI) {
            case RTTI_BUILDING:

                this_type_class = BuildingTypes.Ptr(this_object->Class_Of().ID);

                break;
            case RTTI_UNIT:

                this_type_class = UnitTypes.Ptr(this_object->Class_Of().ID);

                break;
            case RTTI_INFANTRY:

                this_type_class = InfantryTypes.Ptr(this_object->Class_Of().ID);

                break;
            case RTTI_VESSEL:

                this_type_class = VesselTypes.Ptr(this_object->Class_Of().ID);

                break;
            case RTTI_AIRCRAFT:

                this_type_class = AircraftTypes.Ptr(this_object->Class_Of().ID);

                break;
            }

            // Type attribute retrieval
            if (this_type_class != NULL) {
                return Script_SetObjectTypeAttribute(L, this_type_class, (MapScriptObjectAttributeType)attribute_type);
            }

        }break;

        }

    }


    return -1;
}

/**********************************************************************************************
* Script_GetBuildingTypeAttribute - Gets attributes for a building types                      *
*                                                                                             *
*   SCRIPT INPUT:	buildingTypeID  (int)  - The ID of the building type class                *
*                	attribute_type  (int)  - The attribute to change                          *
*                                                                                             *
*   SCRIPT OUTPUT:  void                                                                      *
*                                                                                             *
* INPUT:  lua_State - The current Lua state                                                   *
*                                                                                             *
* OUTPUT:  int; Did the function run successfully? Return 1                                   *
*                                                                                             *
* WARNINGS:  ?                                                                                *
*                                                                                             *
*=============================================================================================*/
static int Script_GetBuildingTypeAttribute(lua_State* L) {

    int buildingTypeID = lua_tointeger(L, 1);
    int attribute_type = lua_tointeger(L, 2);

    TechnoTypeClass* this_type_class = BuildingTypes.Ptr(buildingTypeID);

    if (this_type_class != NULL) {
        return Script_GetObjectTypeAttribute(L, this_type_class, (MapScriptObjectAttributeType)attribute_type);
    }

    return -1;
}

/**********************************************************************************************
* Script_GetVehicleTypeAttribute - Gets attributes for vehicle types                          *
*                                                                                             *
*   SCRIPT INPUT:	vehicleTypeID  (int)   - The ID of the vehicle type class                 *
*                	attribute_type  (int)  - The attribute to change                          *
*                                                                                             *
*   SCRIPT OUTPUT:  void                                                                      *
*                                                                                             *
* INPUT:  lua_State - The current Lua state                                                   *
*                                                                                             *
* OUTPUT:  int; Did the function run successfully? Return 1                                   *
*                                                                                             *
* WARNINGS:  ?                                                                                *
*                                                                                             *
*=============================================================================================*/
static int Script_GetVehicleTypeAttribute(lua_State* L) {

    int vehicleTypeID = lua_tointeger(L, 1);
    int attribute_type = lua_tointeger(L, 2);

    TechnoTypeClass* this_type_class = UnitTypes.Ptr(vehicleTypeID);

    if (this_type_class != NULL) {
        return Script_GetObjectTypeAttribute(L, this_type_class, (MapScriptObjectAttributeType)attribute_type);
    }

    return -1;
}

/**********************************************************************************************
* Script_GetInfantryTypeAttribute - Gets attributes for infantry types                        *
*                                                                                             *
*   SCRIPT INPUT:	infantryTypeID  (int)  - The ID of the infantry type class                *
*                	attribute_type  (int)  - The attribute to change                          *
*                                                                                             *
*   SCRIPT OUTPUT:  void                                                                      *
*                                                                                             *
* INPUT:  lua_State - The current Lua state                                                   *
*                                                                                             *
* OUTPUT:  int; Did the function run successfully? Return 1                                   *
*                                                                                             *
* WARNINGS:  ?                                                                                *
*                                                                                             *
*=============================================================================================*/
static int Script_GetInfantryTypeAttribute(lua_State* L) {

    int infantryTypeID = lua_tointeger(L, 1);
    int attribute_type = lua_tointeger(L, 2);

    TechnoTypeClass* this_type_class = InfantryTypes.Ptr(infantryTypeID);

    if (this_type_class != NULL) {
        return Script_GetObjectTypeAttribute(L, this_type_class, (MapScriptObjectAttributeType)attribute_type);
    }

    return -1;
}

/**********************************************************************************************
* Script_GetVesselTypeAttribute - Gets attributes for vessel types                            *
*                                                                                             *
*   SCRIPT INPUT:	vesselTypeID    (int)  - The ID of the vessel type class                  *
*                	attribute_type  (int)  - The attribute to change                          *
*                                                                                             *
*   SCRIPT OUTPUT:  void                                                                      *
*                                                                                             *
* INPUT:  lua_State - The current Lua state                                                   *
*                                                                                             *
* OUTPUT:  int; Did the function run successfully? Return 1                                   *
*                                                                                             *
* WARNINGS:  ?                                                                                *
*                                                                                             *
*=============================================================================================*/
static int Script_GetVesselTypeAttribute(lua_State* L) {

    int vesselTypeID = lua_tointeger(L, 1);
    int attribute_type = lua_tointeger(L, 2);

    TechnoTypeClass* this_type_class = VesselTypes.Ptr(vesselTypeID);

    if (this_type_class != NULL) {
        return Script_GetObjectTypeAttribute(L, this_type_class, (MapScriptObjectAttributeType)attribute_type);
    }

    return -1;
}

/**********************************************************************************************
* Script_GetAircraftTypeAttribute - Gets attributes for aircraft types                        *
*                                                                                             *
*   SCRIPT INPUT:	aircraftTypeID  (int)  - The ID of the aircraft type class                *
*                	attribute_type  (int)  - The attribute to change                          *
*                                                                                             *
*   SCRIPT OUTPUT:  void                                                                      *
*                                                                                             *
* INPUT:  lua_State - The current Lua state                                                   *
*                                                                                             *
* OUTPUT:  int; Did the function run successfully? Return 1                                   *
*                                                                                             *
* WARNINGS:  ?                                                                                *
*                                                                                             *
*=============================================================================================*/
static int Script_GetAircraftTypeAttribute(lua_State* L) {

    int aircraftTypeID = lua_tointeger(L, 1);
    int attribute_type = lua_tointeger(L, 2);

    TechnoTypeClass* this_type_class = AircraftTypes.Ptr(aircraftTypeID);

    if (this_type_class != NULL) {
        return Script_GetObjectTypeAttribute(L, this_type_class, (MapScriptObjectAttributeType)attribute_type);
    }

    return -1;
}

/**********************************************************************************************
* Script_GetObjectTypeAttribute - Gets attributes for specific object types globally          *
*                                                                                             *
*  Used internally by MapScript to send input and get attributes for TechnoTypeClass          *
*                                                                                             *
*   SCRIPT INPUT:	this_type_class (TechnoTypeClass*) - The techno type                      *
*                	attribute_type  (MapScriptObjectAttributeType)  - The attribute to change *
*                                                                                             *
*   SCRIPT OUTPUT:  void                                                                      *
*                                                                                             *
* WARNINGS:  ?                                                                                *
*                                                                                             *
*=============================================================================================*/
static int Script_GetObjectTypeAttribute(lua_State* L, TechnoTypeClass* this_type_class, MapScriptObjectAttributeType attribute_type) {

    switch (attribute_type) {
    case ATTRIBUTE_MAX_STRENGTH: {

        // Send the attribute
        lua_pushinteger(L, this_type_class->MaxStrength);
        return 1;

    }break;

    case ATTRIBUTE_COST: {

        // Send the attribute
        lua_pushinteger(L, this_type_class->Cost);
        return 1;

    }break;
    case ATTRIBUTE_SIGHT_RANGE: {

        // Send the attribute
        lua_pushinteger(L, this_type_class->SightRange);
        return 1;

    }break;
    case ATTRIBUTE_TECH_LEVEL: {

        // Send the attribute
        lua_pushinteger(L, this_type_class->Level);
        return 1;

    }break;
    case ATTRIBUTE_ARMOR: {

        // Send the attribute
        lua_pushinteger(L, this_type_class->Armor);
        return 1;

    }break;
    case ATTRIBUTE_PRIMARY_WEAPON: {

        if (this_type_class->PrimaryWeapon != NULL) {
            // Send the attribute
            lua_pushinteger(L, this_type_class->PrimaryWeapon->ID);
            return 1;
        }
        else {
            // Send the attribute
            lua_pushinteger(L, -1);
            return 1;
        }

    }break;
    case ATTRIBUTE_SECONDARY_WEAPON: {

        if (this_type_class->SecondaryWeapon != NULL) {
            // Send the attribute
            lua_pushinteger(L, this_type_class->SecondaryWeapon->ID);
            return 1;
        }
        else {
            // Send the attribute
            lua_pushinteger(L, -1);
            return 1;
        }

    }break;
    case ATTRIBUTE_SPEED: {

        // Send the attribute
        lua_pushinteger(L, this_type_class->Speed);
        return 1;

    }break;
    case ATTRIBUTE_MAX_SPEED: {

        // Send the attribute
        lua_pushinteger(L, this_type_class->MaxSpeed);
        return 1;

    }break;
    case ATTRIBUTE_MAX_AMMO: {

        // Send the attribute
        lua_pushinteger(L, this_type_class->MaxAmmo);
        return 1;

    }break;
    case ATTRIBUTE_ROTATION_SPEED: {

        // Send the attribute
        lua_pushboolean(L, this_type_class->ROT);
        return 1;

    }break;
    case ATTRIBUTE_CRUSHABLE: {

        // Send the attribute
        lua_pushboolean(L, this_type_class->IsCrushable);
        return 1;

    }break;
    case ATTRIBUTE_STEALTHY: {

        // Send the attribute
        lua_pushboolean(L, this_type_class->IsStealthy);
        return 1;

    }break;
    case ATTRIBUTE_SELECTABLE: {

        // Send the attribute
        lua_pushboolean(L, this_type_class->IsSelectable);
        return 1;

    }break;
    case ATTRIBUTE_LEGAL_TARGET: {

        // Send the attribute
        lua_pushboolean(L, this_type_class->IsLegalTarget);
        return 1;

    }break;
    case ATTRIBUTE_REPAIRABLE: {

        // Send the attribute
        lua_pushboolean(L, this_type_class->IsRepairable);
        return 1;

    }break;
    case ATTRIBUTE_SELF_HEALING: {

        // Send the attribute
        lua_pushboolean(L, this_type_class->IsSelfHealing);
        return 1;

    }break;
    case ATTRIBUTE_EXPLODES: {

        // Send the attribute
        lua_pushboolean(L, this_type_class->IsExploding);
        return 1;

    }break;
    case ATTRIBUTE_HAS_CREW: {

        // Send the attribute
        lua_pushboolean(L, this_type_class->IsCrew);
        return 1;

    }break;
    case ATTRIBUTE_MAX_PASSENGERS: {

        // Send the attribute
        lua_pushinteger(L, this_type_class->MaxPassengers);
        return 1;

    }break;
    default: {

        // Object class specific attributes
        // Welcome to the web of switches]

        // Type of object
        switch (this_type_class->RTTI) {

            // Building Specific
        case RTTI_BUILDINGTYPE: {

            // What type of attribute?
            switch (attribute_type) {
            case ATTRIBUTE_CAPACITY: {

                BuildingTypeClass* this_building_class = (BuildingTypeClass*)this_type_class;

                // Send the attribute
                lua_pushinteger(L, this_building_class->Capacity);
                return 1;

            }break;
            case ATTRIBUTE_POWER: {

                // Both power and drain are different variables
                // And so we do some trickery here to fill
                // the right values based on postivie/negative input

                BuildingTypeClass* this_building_class = (BuildingTypeClass*)this_type_class;

                int this_value = this_building_class->Power - this_building_class->Drain;

                // Send the attribute
                lua_pushinteger(L, this_value);
                return 1;

            }break;
            case ATTRIBUTE_SELLABLE: {

                BuildingTypeClass* this_building_class = (BuildingTypeClass*)this_type_class;

                // Set the attribute
                // Personal opinion; sellable makes more sense than unsellable
                // This replacement could be seen as IsUnNonSellable, or... IsSellable
                if (this_building_class->IsUnsellable == 1) {
                    // Send the attribute
                    lua_pushinteger(L, 0);
                    return 1;
                }
                else {
                    // Send the attribute
                    lua_pushinteger(L, 1);
                    return 1;
                }

            }break;
            }

        }break;


            // Unit Specific
        case RTTI_UNITTYPE: {

            // What type of attribute?
            switch (attribute_type) {
            case ATTRIBUTE_CRATE_GOODIE: {

                UnitTypeClass* this_unit_class = (UnitTypeClass*)this_type_class;

                // Send the attribute
                lua_pushboolean(L, this_unit_class->IsCrateGoodie);
                return 1;

            }break;
            case ATTRIBUTE_CRUSHER: {

                UnitTypeClass* this_unit_class = (UnitTypeClass*)this_type_class;

                // Send the attribute
                lua_pushboolean(L, this_unit_class->IsCrusher);
                return 1;

            }break;
            case ATTRIBUTE_HARVEST: {

                UnitTypeClass* this_unit_class = (UnitTypeClass*)this_type_class;

                // Send the attribute
                lua_pushboolean(L, this_unit_class->IsToHarvest);
                return 1;

            }break;
            case ATTRIBUTE_RADAR_EQUIPPED: {

                UnitTypeClass* this_unit_class = (UnitTypeClass*)this_type_class;

                // Send the attribute
                lua_pushboolean(L, this_unit_class->IsRadarEquipped);
                return 1;

            }break;
            case ATTRIBUTE_JAMMER: {

                UnitTypeClass* this_unit_class = (UnitTypeClass*)this_type_class;

                // Send the attribute
                lua_pushboolean(L, this_unit_class->IsJammer);
                return 1;

            }break;
            case ATTRIBUTE_GAPPER: {

                UnitTypeClass* this_unit_class = (UnitTypeClass*)this_type_class;

                // Send the attribute
                lua_pushboolean(L, this_unit_class->IsGapper);
                return 1;

            }break;
            }

        }break;


            // Infantry Specific
        case RTTI_INFANTRYTYPE: {

            // What type of attribute?
            switch (attribute_type) {
            case ATTRIBUTE_FEMALE: {

                InfantryTypeClass* this_infantry_class = (InfantryTypeClass*)this_type_class;

                // Send the attribute
                lua_pushboolean(L, this_infantry_class->IsFemale);
                return 1;

            }break;
            case ATTRIBUTE_CAPTURE: {

                InfantryTypeClass* this_infantry_class = (InfantryTypeClass*)this_type_class;

                // Send the attribute
                lua_pushboolean(L, this_infantry_class->IsCapture);
                return 1;

            }break;
            case ATTRIBUTE_FRAIDY_CAT: {

                InfantryTypeClass* this_infantry_class = (InfantryTypeClass*)this_type_class;

                // Send the attribute
                lua_pushboolean(L, this_infantry_class->IsFraidyCat);
                return 1;

            }break;
            case ATTRIBUTE_CIVILIAN: {

                InfantryTypeClass* this_infantry_class = (InfantryTypeClass*)this_type_class;

                // Send the attribute
                lua_pushboolean(L, this_infantry_class->IsCivilian);
                return 1;

            }break;
            case ATTRIBUTE_BOMBER: {

                InfantryTypeClass* this_infantry_class = (InfantryTypeClass*)this_type_class;

                // Send the attribute
                lua_pushboolean(L, this_infantry_class->IsBomber);
                return 1;

            }break;
            case ATTRIBUTE_DOG: {

                InfantryTypeClass* this_infantry_class = (InfantryTypeClass*)this_type_class;

                // Send the attribute
                lua_pushboolean(L, this_infantry_class->IsDog);
                return 1;

            }break;

            }

        }break;

        } // End of object specific switch

    }break;

    } // End of attribute switch 


    // Didn't find result
    return -1;

}

/**********************************************************************************************
* Script_SetHouseAttribute - Sets attributes for specific houses                              *
*                                                                                             *
*   SCRIPT INPUT:	houseID         (int)  - The ID of the house                              *
*                	attribute_type  (int)  - The attribute to change                          *
*                	attribute_value (int)  - The value to set the attribute to                *
*                                                                                             *
*   SCRIPT OUTPUT:  void                                                                      *
*                                                                                             *
* INPUT:  lua_State - The current Lua state                                                   *
*                                                                                             *
* OUTPUT:  int; Did the function run successfully? Return 1                                   *
*                                                                                             *
* WARNINGS:  ?                                                                                *
*                                                                                             *
*=============================================================================================*/
static int Script_SetHouseAttribute(lua_State* L) {

    int houseID = lua_tointeger(L, 1);
    int attribute_type = lua_tointeger(L, 2);

    HouseClass* this_house = Houses.Ptr(houseID);

    // Get the appropriate technotype
    if (this_house != NULL) {

        switch (attribute_type) {
        case HOUSE_ATTRIBUTE_IQ: {

            int attribute_value = lua_tointeger(L, 3);

            this_house->Control.IQ = attribute_value;
            this_house->IQ = attribute_value;

        }break;
        case HOUSE_ATTRIBUTE_TECH_LEVEL: {

            int attribute_value = lua_tointeger(L, 3);

            this_house->Control.TechLevel = attribute_value;

        }break;
        case HOUSE_ATTRIBUTE_IS_HUMAN: {

            int attribute_value = lua_tointeger(L, 3);

            if (attribute_value == 1) {
                this_house->IsHuman = 1;
                this_house->WasHuman = 1;
            }
            else {
                this_house->IsHuman = 0;
            }


        }break;
        case HOUSE_ATTRIBUTE_FIREPOWER_BIAS: {

            float attribute_value = lua_tonumber(L, 3);

            this_house->FirepowerBias = (fixed)attribute_value * Rule.Diff[this_house->Difficulty].FirepowerBias;

        }break;
        case HOUSE_ATTRIBUTE_GROUNDSPEED_BIAS: {

            float attribute_value = lua_tonumber(L, 3);

            this_house->GroundspeedBias = (fixed)attribute_value * Rule.Diff[this_house->Difficulty].GroundspeedBias;

        }break;
        case HOUSE_ATTRIBUTE_ARMOR_BIAS: {

            float attribute_value = lua_tonumber(L, 3);

            this_house->ArmorBias = (fixed)attribute_value * Rule.Diff[this_house->Difficulty].ArmorBias;

        }break;
        case HOUSE_ATTRIBUTE_ROF_BIAS: {

            float attribute_value = lua_tonumber(L, 3);

            this_house->ROFBias = (fixed)attribute_value * Rule.Diff[this_house->Difficulty].ROFBias;

        }break;
        case HOUSE_ATTRIBUTE_COST_BIAS: {

            float attribute_value = lua_tonumber(L, 3);

            this_house->CostBias = (fixed)attribute_value * Rule.Diff[this_house->Difficulty].CostBias;

        }break;
        case HOUSE_ATTRIBUTE_REPAIR_DELAY: {

            float attribute_value = lua_tonumber(L, 3);

            this_house->RepairDelay = (fixed)attribute_value;

        }break;
        case HOUSE_ATTRIBUTE_BUILD_DELAY: {

            float attribute_value = lua_tonumber(L, 3);

            this_house->BuildDelay = (fixed)attribute_value;

        }break;
        case HOUSE_ATTRIBUTE_STARTED: {

            int attribute_value = lua_tointeger(L, 3);

            this_house->IsStarted = attribute_value;

        }break;
        case HOUSE_ATTRIBUTE_ALERTED: {

            int attribute_value = lua_tointeger(L, 3);

            this_house->IsAlerted = attribute_value;

        }break;
        case HOUSE_ATTRIBUTE_BASE_BUILDING: {

            int attribute_value = lua_tointeger(L, 3);

            this_house->IsBaseBuilding = attribute_value;

        }break;
        case HOUSE_ATTRIBUTE_DISCOVERED: {

            int attribute_value = lua_tointeger(L, 3);

            this_house->IsDiscovered = attribute_value;

        }break;
        case HOUSE_ATTRIBUTE_CAPACITY: {

            int attribute_value = lua_tointeger(L, 3);

            this_house->Capacity = attribute_value;

        }break;
        case HOUSE_ATTRIBUTE_TIBERIUM: {

            int attribute_value = lua_tointeger(L, 3);

            this_house->Tiberium = attribute_value;

        }break;
        case HOUSE_ATTRIBUTE_CREDITS: {

            int attribute_value = lua_tointeger(L, 3);

            this_house->Credits = attribute_value;

        }break;
        case HOUSE_ATTRIBUTE_ALERT_TIMER: {

            float attribute_value = lua_tonumber(L, 3);

            this_house->AlertTime = attribute_value;

        }break;
        case HOUSE_ATTRIBUTE_REPAIR_TIMER: {

            float attribute_value = lua_tonumber(L, 3);

            this_house->RepairTimer = attribute_value;

        }break;
        case HOUSE_ATTRIBUTE_POINTS: {

            int attribute_value = lua_tointeger(L, 3);

            this_house->PointTotal = attribute_value;

        }break;
        case HOUSE_ATTRIBUTE_DESTROYED_BUILDINGS: {

            // Non writable

        }break;
        case HOUSE_ATTRIBUTE_DESTROYED_INFANTRY: {

            // Non writable

        }break;
        case HOUSE_ATTRIBUTE_DESTROYED_VEHICLES: {

            // Non writable

        }break;
        case HOUSE_ATTRIBUTE_DESTROYED_AIRCRAFT: {

            // Non writable

        }break;
        case HOUSE_ATTRIBUTE_DESTROYED_VESSELS: {

            // Non writable

        }break;
        case HOUSE_ATTRIBUTE_CAPTURED_BUILDINGS: {

            // Non writable

        }break;
        case HOUSE_ATTRIBUTE_CRATES: {

            // Non writable

        }break;

        case HOUSE_ATTRIBUTE_BUILDING_FACTORIES: {

            int attribute_value = lua_tointeger(L, 3);

            this_house->BuildingFactories = attribute_value;

        }break;
        case HOUSE_ATTRIBUTE_UNIT_FACTORIES: {

            int attribute_value = lua_tointeger(L, 3);

            this_house->UnitFactories = attribute_value;

        }break;
        case HOUSE_ATTRIBUTE_INFANTRY_FACTORIES: {

            int attribute_value = lua_tointeger(L, 3);

            this_house->InfantryFactories = attribute_value;

        }break;
        case HOUSE_ATTRIBUTE_AIRCRAFT_FACTORIES: {

            int attribute_value = lua_tointeger(L, 3);

            this_house->AircraftFactories = attribute_value;

        }break;
        case HOUSE_ATTRIBUTE_VESSEL_FACTORIES: {

            int attribute_value = lua_tointeger(L, 3);

            this_house->VesselFactories = attribute_value;

        }break;

        case HOUSE_ATTRIBUTE_POWER: {

            int attribute_value = lua_tointeger(L, 3);

            this_house->Power = attribute_value;

        }break;
        case HOUSE_ATTRIBUTE_DRAIN: {

            int attribute_value = lua_tointeger(L, 3);

            this_house->Drain = attribute_value;

        }break;
        case HOUSE_ATTRIBUTE_LAST_ATTACKER: {

            int attribute_value = lua_tointeger(L, 3);

            this_house->WhoLastHurtMe = (HousesType)attribute_value;

        }break;
        }

    }


    return -1;
}

/**********************************************************************************************
* Script_GetHouseAttribute - Sets attributes for specific houses                              *
*                                                                                             *
*   SCRIPT INPUT:	houseID         (int)  - The ID of the house                              *
*                	attribute_type  (int)  - The attribute to change                          *
*                                                                                             *
*   SCRIPT OUTPUT:  (mixed) - The value of the attribute                                      *
*                                                                                             *
* INPUT:  lua_State - The current Lua state                                                   *
*                                                                                             *
* OUTPUT:  int; Did the function run successfully? Return 1                                   *
*                                                                                             *
* WARNINGS:  ?                                                                                *
*                                                                                             *
*=============================================================================================*/
static int Script_GetHouseAttribute(lua_State* L) {

    int houseID = lua_tointeger(L, 1);
    int attribute_type = lua_tointeger(L, 2);

    HouseClass* this_house = Houses.Ptr(houseID);

    // Get the appropriate technotype
    if (this_house != NULL) {

        switch (attribute_type) {
        case HOUSE_ATTRIBUTE_IQ: {

            lua_pushnumber(L, this_house->IQ);
            return 1;

        }break;
        case HOUSE_ATTRIBUTE_TECH_LEVEL: {

            lua_pushnumber(L, this_house->Control.TechLevel);
            return 1;

        }break;
        case HOUSE_ATTRIBUTE_IS_HUMAN: {

            lua_pushboolean(L, this_house->IsHuman);
            return 1;

        }break;
        case HOUSE_ATTRIBUTE_FIREPOWER_BIAS: {

            lua_pushnumber(L, (float)this_house->FirepowerBias.To_Float());
            return 1;

        }break;
        case HOUSE_ATTRIBUTE_GROUNDSPEED_BIAS: {

            lua_pushnumber(L, (float)this_house->GroundspeedBias.To_Float());
            return 1;

        }break;
        case HOUSE_ATTRIBUTE_ARMOR_BIAS: {

            lua_pushnumber(L, (float)this_house->ArmorBias.To_Float());
            return 1;

        }break;
        case HOUSE_ATTRIBUTE_ROF_BIAS: {

            lua_pushnumber(L, this_house->ROFBias.To_Float());
            return 1;

        }break;
        case HOUSE_ATTRIBUTE_COST_BIAS: {

            lua_pushnumber(L, (float)this_house->CostBias.To_Float());
            return 1;

        }break;
        case HOUSE_ATTRIBUTE_REPAIR_DELAY: {

            lua_pushnumber(L, (float)this_house->RepairDelay.To_Float());
            return 1;

        }break;
        case HOUSE_ATTRIBUTE_BUILD_DELAY: {

            lua_pushnumber(L, (float)this_house->BuildDelay.To_Float());
            return 1;

        }break;
        case HOUSE_ATTRIBUTE_STARTED: {

            lua_pushboolean(L, this_house->IsStarted);
            return 1;

        }break;
        case HOUSE_ATTRIBUTE_ALERTED: {

            lua_pushboolean(L, this_house->IsAlerted);
            return 1;

        }break;
        case HOUSE_ATTRIBUTE_BASE_BUILDING: {

            lua_pushboolean(L, this_house->IsBaseBuilding);
            return 1;

        }break;
        case HOUSE_ATTRIBUTE_DISCOVERED: {

            lua_pushboolean(L, this_house->IsDiscovered);
            return 1;

        }break;
        case HOUSE_ATTRIBUTE_CAPACITY: {

            lua_pushinteger(L, this_house->Capacity);
            return 1;

        }break;
        case HOUSE_ATTRIBUTE_TIBERIUM: {

            lua_pushinteger(L, this_house->Tiberium);
            return 1;

        }break;
        case HOUSE_ATTRIBUTE_CREDITS: {

            lua_pushinteger(L, this_house->Credits);
            return 1;

        }break;
        case HOUSE_ATTRIBUTE_ALERT_TIMER: {

            lua_pushnumber(L, this_house->AlertTime);
            return 1;

        }break;
        case HOUSE_ATTRIBUTE_REPAIR_TIMER: {

            lua_pushnumber(L, this_house->RepairTimer);
            return 1;

        }break;
        case HOUSE_ATTRIBUTE_POINTS: {

            lua_pushinteger(L, this_house->PointTotal);
            return 1;

        }break;
        case HOUSE_ATTRIBUTE_DESTROYED_BUILDINGS: {

            lua_pushinteger(L, (int)this_house->DestroyedBuildings->Get_All_Totals());
            return 1;

        }break;
        case HOUSE_ATTRIBUTE_DESTROYED_INFANTRY: {

            lua_pushinteger(L, (int)this_house->DestroyedInfantry->Get_All_Totals());
            return 1;

        }break;
        case HOUSE_ATTRIBUTE_DESTROYED_VEHICLES: {

            lua_pushinteger(L, (int)this_house->DestroyedUnits->Get_All_Totals());
            return 1;

        }break;
        case HOUSE_ATTRIBUTE_DESTROYED_AIRCRAFT: {

            lua_pushinteger(L, (int)this_house->DestroyedAircraft->Get_All_Totals());
            return 1;

        }break;
        case HOUSE_ATTRIBUTE_DESTROYED_VESSELS: {

            lua_pushinteger(L, (int)this_house->DestroyedVessels->Get_All_Totals());
            return 1;

        }break;
        case HOUSE_ATTRIBUTE_CAPTURED_BUILDINGS: {

            lua_pushinteger(L, (int)this_house->CapturedBuildings->Get_All_Totals());
            return 1;

        }break;
        case HOUSE_ATTRIBUTE_CRATES: {

            lua_pushinteger(L, (int)this_house->TotalCrates->Get_All_Totals());
            return 1;

        }break;
        case HOUSE_ATTRIBUTE_AIRCRAFT_FACTORIES: {

            lua_pushinteger(L, this_house->AircraftFactories);
            return 1;

        }break;
        case HOUSE_ATTRIBUTE_INFANTRY_FACTORIES: {

            lua_pushinteger(L, this_house->InfantryFactories);
            return 1;

        }break;
        case HOUSE_ATTRIBUTE_UNIT_FACTORIES: {

            lua_pushinteger(L, this_house->UnitFactories);
            return 1;

        }break;
        case HOUSE_ATTRIBUTE_VESSEL_FACTORIES: {

            lua_pushinteger(L, this_house->VesselFactories);
            return 1;

        }break;
        case HOUSE_ATTRIBUTE_BUILDING_FACTORIES: {

            lua_pushinteger(L, this_house->BuildingFactories);
            return 1;

        }break;
        case HOUSE_ATTRIBUTE_POWER: {

            lua_pushnumber(L, this_house->Power);
            return 1;

        }break;
        case HOUSE_ATTRIBUTE_DRAIN: {

            lua_pushnumber(L, this_house->Drain);
            return 1;

        }break;
        case HOUSE_ATTRIBUTE_LAST_ATTACKER: {

            lua_pushnumber(L, this_house->WhoLastHurtMe);
            return 1;

        }break;
        }

    }

    lua_pushnumber(L, -1);

    return -1;
}

/***********************************************************************************************
 * Script_BuildUnit - Builds a given unit in a given factory                                   *
 *                                                                                             *
 *   SCRIPT INPUT:	factoryObject (int) - The factory object id                                *
 *                                                                                             *
 *                	unitType (int)    - The unit type index for the unit to make (unit in a    *
 *                                      general sense, not a vehicle)                          *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_BuildUnit(lua_State* L) {

    int factoryObject = lua_tointeger(L, 1);
    int unitType = lua_tointeger(L, 2);

    BuildingClass* this_factory = (BuildingClass*)Script_GetCacheObject(factoryObject);

    // Get the appropriate technotype
    if (this_factory != NULL && this_factory->IsActive) {

        int result = true;

        switch (this_factory->Class->ID) {
        case STRUCT_WEAP: {

            UnitTypeClass* unit_to_place = UnitTypes.Ptr(unitType);
            UnitClass* new_unit = (UnitClass*)unit_to_place->Create_One_Of(this_factory->House);

            if (this_factory->Exit_Object(new_unit)) {

                int _objectIndex = -1;
                MapScriptObject* CacheObject = Scen.mapScript->CreateCacheObject();
                CacheObject->ID = -1;

                _objectIndex = Script_UnitIndexFromID(new_unit->ID);

                CacheObject->ID = new_unit->ID;
                CacheObject->RTTI = new_unit->RTTI;
                CacheObject->classIndex = _objectIndex;
                Scen.mapScript->ObjectCache.push_back(CacheObject);

                lua_pushnumber(L, CacheObject->UID);
                return 1;

            }
            else {
                delete new_unit;
            }

            lua_pushnumber(L, -1);
            return 1;

        }break;
        case STRUCT_TENT:
        case STRUCT_BARRACKS: {

            InfantryTypeClass* unit_to_place = InfantryTypes.Ptr(unitType);
            InfantryClass* new_unit = (InfantryClass*)unit_to_place->Create_One_Of(this_factory->House);

            if (this_factory->Exit_Object(new_unit)) {

                int _objectIndex = -1;
                MapScriptObject* CacheObject = Scen.mapScript->CreateCacheObject();
                CacheObject->ID = -1;

                _objectIndex = Script_InfantryIndexFromID(new_unit->ID);

                CacheObject->ID = new_unit->ID;
                CacheObject->RTTI = new_unit->RTTI;
                CacheObject->classIndex = _objectIndex;
                Scen.mapScript->ObjectCache.push_back(CacheObject);

                lua_pushnumber(L, CacheObject->UID);
                return 1;

            }
            else {
                delete new_unit;
            }

            lua_pushnumber(L, -1);
            return 1;

        }break;
        case STRUCT_KENNEL: {

            InfantryTypeClass* unit_to_place = InfantryTypes.Ptr(unitType);
            InfantryClass* new_unit = (InfantryClass*)unit_to_place->Create_One_Of(this_factory->House);

            if (this_factory->Exit_Object(new_unit)) {

                int _objectIndex = -1;
                MapScriptObject* CacheObject = Scen.mapScript->CreateCacheObject();
                CacheObject->ID = -1;

                _objectIndex = Script_InfantryIndexFromID(new_unit->ID);

                CacheObject->ID = new_unit->ID;
                CacheObject->RTTI = new_unit->RTTI;
                CacheObject->classIndex = _objectIndex;
                Scen.mapScript->ObjectCache.push_back(CacheObject);

                lua_pushnumber(L, CacheObject->UID);
                return 1;

            }
            else {
                delete new_unit;
            }

            lua_pushnumber(L, -1);
            return 1;

        }break;
        case STRUCT_HELIPAD:
        case STRUCT_AIRSTRIP: {

            AircraftTypeClass* unit_to_place = AircraftTypes.Ptr(unitType);
            AircraftClass* new_unit = (AircraftClass*)unit_to_place->Create_One_Of(this_factory->House);

            if (this_factory->Exit_Object(new_unit)) {

                int _objectIndex = -1;
                MapScriptObject* CacheObject = Scen.mapScript->CreateCacheObject();
                CacheObject->ID = -1;

                _objectIndex = Script_AircraftIndexFromID(new_unit->ID);

                CacheObject->ID = new_unit->ID;
                CacheObject->RTTI = new_unit->RTTI;
                CacheObject->classIndex = _objectIndex;
                Scen.mapScript->ObjectCache.push_back(CacheObject);

                lua_pushnumber(L, CacheObject->UID);
                return 1;

            }
            else {
                delete new_unit;
            }

            lua_pushnumber(L, -1);
            return 1;

        }break;
        case STRUCT_SUB_PEN:
        case STRUCT_SHIP_YARD: {

            VesselTypeClass* unit_to_place = VesselTypes.Ptr(unitType);
            VesselClass* new_unit = (VesselClass*)unit_to_place->Create_One_Of(this_factory->House);

            if (this_factory->Exit_Object(new_unit)) {

                int _objectIndex = -1;
                MapScriptObject* CacheObject = Scen.mapScript->CreateCacheObject();
                CacheObject->ID = -1;

                _objectIndex = Script_VesselIndexFromID(new_unit->ID);

                CacheObject->ID = new_unit->ID;
                CacheObject->RTTI = new_unit->RTTI;
                CacheObject->classIndex = _objectIndex;
                Scen.mapScript->ObjectCache.push_back(CacheObject);

                lua_pushnumber(L, CacheObject->UID);
                return 1;

            }
            else {
                delete new_unit;
            }

            lua_pushnumber(L, -1);
            return 1;

        }break;
        }

    }


    lua_pushnumber(L, -1);
    return 1;
}

/***********************************************************************************************
 * Script_CreateVehicle - Creates a given unit out of thin air                                 *
 *                                                                                             *
 *   SCRIPT INPUT:	objectIndex(int) - The unit type to create                                 *
 *                                                                                             *
 *                  houseType  (int) - The player (house) index to create the unit for         *
 *                                                                                             *
 *                  in_x       (int) - X location to create object                             *
 *                                                                                             *
 *                  in_y       (int) - Y location to create object                             *
 *                                                                                             *
 *   SCRIPT OUTPUT:  (int) the cached object ID for use with Mapscript Functions               *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_CreateVehicle(lua_State* L) {

    int objectIndex = lua_tointeger(L, 1);
    int houseType = lua_tointeger(L, 2);
	int in_x = 0;
	int in_y = 0;

	COORDINATE coord;
	if (lua_gettop(L) == 3) {
		CELL cell = lua_tointeger(L, 3);
		coord = Cell_Coord(cell);
	}
	else {
		in_x = lua_tointeger(L, 3);
		in_y = lua_tointeger(L, 4);
		coord = Cell_Coord(XY_Cell(in_x, in_y));
	}

    HouseClass* this_house = Houses.Ptr(houseType);

    UnitTypeClass* unit_to_place = UnitTypes.Ptr(objectIndex);

    if (this_house != NULL && unit_to_place != NULL) {
        UnitClass* new_unit = (UnitClass*)unit_to_place->Create_One_Of(this_house);
        if (new_unit != NULL) {

            bool new_unit_placed = false;

            if (new_unit->Unlimbo(coord)) {

                new_unit_placed = true;
            }

            /*
            **	Try to place the object into a nearby cell if something is preventing
            **	placement at the given location.
            */
            if (!new_unit_placed) {
                CELL cell = Map.Nearby_Location(coord, new_unit->Class->Speed);
                if (new_unit->Unlimbo(::Cell_Coord(cell))) {
                    new_unit_placed = true;
                }
            }


            if (new_unit_placed) {
                int _objectIndex = -1;
                MapScriptObject* CacheObject = Scen.mapScript->CreateCacheObject();
                CacheObject->ID = -1;

                _objectIndex = Script_UnitIndexFromID(new_unit->ID);

                CacheObject->ID = new_unit->ID;
                CacheObject->RTTI = new_unit->RTTI;
                CacheObject->classIndex = _objectIndex;
                Scen.mapScript->ObjectCache.push_back(CacheObject);

                lua_pushnumber(L, CacheObject->UID);
                return 1;
            }
        }
    }

    lua_pushnumber(L, -1);
    return 1;
}


/***********************************************************************************************
 * Script_CreateInfantry - Creates a given Infantry out of thin air                            *
 *                                                                                             *
 *   SCRIPT INPUT:	objectIndex(int) - The Infantry type to create                             *
 *                                                                                             *
 *                  houseType  (int) - The player (house) index to create the Infantry for     *
 *                                                                                             *
 *                  in_x       (int) - X location to create object                             *
 *                                                                                             *
 *                  in_y       (int) - Y location to create object                             *
 *                                                                                             *
 *   SCRIPT OUTPUT:  (int) the cached object ID for use with Mapscript Functions               *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_CreateInfantry(lua_State* L) {

    int objectIndex = lua_tointeger(L, 1);
    int houseType = lua_tointeger(L, 2);
    int in_x = 0;
    int in_y = 0;
    
    COORDINATE coord;
	if (lua_gettop(L) == 3) {
        CELL cell = lua_tointeger(L, 3);
        coord = Cell_Coord(cell);
	}
    else {
        in_x = lua_tointeger(L, 3);
        in_y = lua_tointeger(L, 4);
        coord = Cell_Coord(XY_Cell(in_x, in_y));
    }

    HouseClass* this_house = Houses.Ptr(houseType);

    InfantryTypeClass* unit_to_place = InfantryTypes.Ptr(objectIndex);

    if (this_house != NULL && unit_to_place != NULL) {
        InfantryClass* new_unit = (InfantryClass*)unit_to_place->Create_One_Of(this_house);
        if (new_unit != NULL) {

            bool new_unit_placed = false;

            if (new_unit->Unlimbo(coord, DIR_S)) {

                new_unit_placed = true;
            }

            /*
            **	Try to place the object into a nearby cell if something is preventing
            **	placement at the given location.
            */
            if (!new_unit_placed) {
                CELL cell = Map.Nearby_Location(coord, new_unit->Class->Speed);
                if (new_unit->Unlimbo(::Cell_Coord(cell), DIR_S)) {
                    new_unit_placed = true;
                }
            }


            if (new_unit_placed) {
                int _objectIndex = -1;
                MapScriptObject* CacheObject = Scen.mapScript->CreateCacheObject();
                CacheObject->ID = -1;

                _objectIndex = Script_InfantryIndexFromID(new_unit->ID);

                CacheObject->ID = new_unit->ID;
                CacheObject->RTTI = new_unit->RTTI;
                CacheObject->classIndex = _objectIndex;
                Scen.mapScript->ObjectCache.push_back(CacheObject);

                lua_pushnumber(L, CacheObject->UID);
                return 1;
            }
        }
    }

    lua_pushnumber(L, -1);
    return 1;
}

/***********************************************************************************************
 * Script_CreateAircraft - Creates a given Aircraft out of thin air                            *
 *                                                                                             *
 *   SCRIPT INPUT:	objectIndex(int) - The Aircraft type to create                             *
 *                                                                                             *
 *                  houseType  (int) - The player (house) index to create the Aircraft for     *
 *                                                                                             *
 *                  in_x       (int) - X location to create object                             *
 *                                                                                             *
 *                  in_y       (int) - Y location to create object                             *
 *                                                                                             *
 *   SCRIPT OUTPUT:  (int) the cached object ID for use with Mapscript Functions               *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_CreateAircraft(lua_State* L) {

    int objectIndex = lua_tointeger(L, 1);
    int houseType = lua_tointeger(L, 2);
    int in_x = lua_tointeger(L, 3);
    int in_y = lua_tointeger(L, 4);

    HouseClass* this_house = Houses.Ptr(houseType);

    AircraftTypeClass* unit_to_place = AircraftTypes.Ptr(objectIndex);

    if (this_house != NULL && unit_to_place != NULL) {
        AircraftClass* new_unit = (AircraftClass*)unit_to_place->Create_One_Of(this_house);
        if (new_unit != NULL) {

            bool new_unit_placed = false;

            if (new_unit->Unlimbo(Cell_Coord(XY_Cell(in_x, in_y)))) {

                new_unit_placed = true;
            }

            /*
            **	Try to place the object into a nearby cell if something is preventing
            **	placement at the given location.
            */
            if (!new_unit_placed) {
                CELL cell = Map.Nearby_Location(Cell_Coord(XY_Cell(in_x, in_y)), new_unit->Class->Speed);
                if (new_unit->Unlimbo(::Cell_Coord(cell))) {
                    new_unit_placed = true;
                }
            }


            if (new_unit_placed) {
                int _objectIndex = -1;
                MapScriptObject* CacheObject = Scen.mapScript->CreateCacheObject();
                CacheObject->ID = -1;

                _objectIndex = Script_AircraftIndexFromID(new_unit->ID);

                CacheObject->ID = new_unit->ID;
                CacheObject->RTTI = new_unit->RTTI;
                CacheObject->classIndex = _objectIndex;
                Scen.mapScript->ObjectCache.push_back(CacheObject);

                lua_pushnumber(L, CacheObject->UID);
                return 1;
            }
        }
    }

    lua_pushnumber(L, -1);
    return 1;
}

/***********************************************************************************************
 * Script_CreateVessel - Creates a given Vessel out of thin air                                *
 *                                                                                             *
 *   SCRIPT INPUT:	objectIndex(int) - The Vessel type to create                               *
 *                                                                                             *
 *                  houseType  (int) - The player (house) index to create the Vessel for       *
 *                                                                                             *
 *                  in_x       (int) - X location to create object                             *
 *                                                                                             *
 *                  in_y       (int) - Y location to create object                             *
 *                                                                                             *
 *   SCRIPT OUTPUT:  (int) the cached object ID for use with Mapscript Functions               *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_CreateVessel(lua_State* L) {

    int objectIndex = lua_tointeger(L, 1);
    int houseType = lua_tointeger(L, 2);
    int in_x = lua_tointeger(L, 3);
    int in_y = lua_tointeger(L, 4);

    HouseClass* this_house = Houses.Ptr(houseType);

    VesselTypeClass* unit_to_place = VesselTypes.Ptr(objectIndex);

    if (this_house != NULL && unit_to_place != NULL) {
        VesselClass* new_unit = (VesselClass*)unit_to_place->Create_One_Of(this_house);
        if (new_unit != NULL) {

            bool new_unit_placed = false;

            if (new_unit->Unlimbo(Cell_Coord(XY_Cell(in_x, in_y)))) {

                new_unit_placed = true;
            }

            /*
            **	Try to place the object into a nearby cell if something is preventing
            **	placement at the given location.
            */
            if (!new_unit_placed) {
                CELL cell = Map.Nearby_Location(Cell_Coord(XY_Cell(in_x, in_y)), new_unit->Class->Speed);
                if (new_unit->Unlimbo(::Cell_Coord(cell))) {
                    new_unit_placed = true;
                }
            }


            if (new_unit_placed) {
                int _objectIndex = -1;
                MapScriptObject* CacheObject = Scen.mapScript->CreateCacheObject();
                CacheObject->ID = -1;

                _objectIndex = Script_VesselIndexFromID(new_unit->ID);

                CacheObject->ID = new_unit->ID;
                CacheObject->RTTI = new_unit->RTTI;
                CacheObject->classIndex = _objectIndex;
                Scen.mapScript->ObjectCache.push_back(CacheObject);

                lua_pushnumber(L, CacheObject->UID);
                return 1;
            }
        }
    }

    lua_pushnumber(L, -1);
    return 1;
}


/**********************************************************************************************
* Object Orders Functions                                                                     *
*=============================================================================================*/

/***********************************************************************************************
 * Script_OrderMove - Assigns a movement order to an object                                    *
 *                                                                                             *
 *   SCRIPT INPUT:	objectID   (int) - The object ID (retrieved via MS functions)              *
 *                                                                                             *
 *                  in_x       (int) - X location to move object to                            *
 *                                                                                             *
 *                  in_y       (int) - Y location to move object to                            *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_OrderMove(lua_State* L) {

    int objectID = lua_tointeger(L, 1);
    int in_x = lua_tointeger(L, 2);
    int in_y = lua_tointeger(L, 3);

    ObjectClass* this_object = Script_GetCacheObject(objectID);

    if (this_object != NULL) {

        if (this_object->IsRecentlyCreated) {
            Queue_Mission(TargetClass(this_object), MISSION_QMOVE, 0, As_Target(XY_Cell(in_x, in_y)));
        }

        Queue_Mission(TargetClass(this_object), MISSION_QMOVE, 0, As_Target(XY_Cell(in_x, in_y)));

        lua_pushnumber(L, 1);
        return 1;

    }

    lua_pushnumber(L, -1);
    return 1;

}

/***********************************************************************************************
 * Script_OrderAttackCell - Assigns an attack order to an object for a specific cell           *
 *                                                                                             *
 *   SCRIPT INPUT:	objectID   (int) - The object ID (retrieved via MS functions)              *
 *                                                                                             *
 *                  in_x       (int) - X location to attack                                    *
 *                                                                                             *
 *                  in_y       (int) - Y location to attack                                    *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_OrderAttackCell(lua_State* L) {

    int objectID = lua_tointeger(L, 1);

    int in_x = lua_tointeger(L, 2);
    int in_y = lua_tointeger(L, 3);

    ObjectClass* this_object = Script_GetCacheObject(objectID);

    if (this_object != NULL) {

        TARGET this_target = 0;
        if (in_x >= 0 && in_y >= 0) {
            this_target = As_Target(XY_Cell(in_x, in_y));
        }

        if (this_object->IsRecentlyCreated) {
            Queue_Mission(TargetClass(this_object), MISSION_ATTACK, this_target, this_target);
        }

        Queue_Mission(TargetClass(this_object), MISSION_ATTACK, this_target, this_target);

        lua_pushnumber(L, 1);
        return 1;

    }

    lua_pushnumber(L, -1);
    return 1;

}

/***********************************************************************************************
 * Script_OrderAttack - Assigns an attack order to an object                                   *
 *                                                                                             *
 *   SCRIPT INPUT:	objectID   (int) - The object ID (retrieved via MS functions)              *
 *                                                                                             *
 *                  in_x       (int) - X location to attack                                    *
 *                                                                                             *
 *                  in_y       (int) - Y location to attack                                    *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_OrderAttack(lua_State* L) {

    int objectID = lua_tointeger(L, 1);

    int in_x = -1;
    int in_y = -1;
    if (lua_gettop(L) == 3) {
        in_x = lua_tointeger(L, 2);
        in_y = lua_tointeger(L, 3);
    }

    ObjectClass* this_object = Script_GetCacheObject(objectID);

    if (this_object != NULL) {

        TARGET this_target = 0;
        if (in_x >= 0 && in_y >= 0) {
            this_target = As_Target(XY_Cell(in_x, in_y));
        }

        if (this_object->IsRecentlyCreated) {
            Queue_Mission(TargetClass(this_object), MISSION_ATTACK, 0, this_target);
        }

        Queue_Mission(TargetClass(this_object), MISSION_ATTACK, 0, this_target);

        lua_pushnumber(L, 1);
        return 1;

    }

    lua_pushnumber(L, -1);
    return 1;

}

/***********************************************************************************************
 * Script_OrderHunt - Assigns a hunt order to an object                                        *
 *                                                                                             *
 *   SCRIPT INPUT:	objectID   (int) - The object ID (retrieved via MS functions)              *
 *                                                                                             *
 *                  in_x  (opt)(int) - X location to attack                                    *
 *                                                                                             *
 *                  in_y  (opt)(int) - Y location to attack                                    *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_OrderHunt(lua_State* L) {

    int objectID = lua_tointeger(L, 1);

    int in_x = -1;
    int in_y = -1;
    if (lua_gettop(L) == 3) {
        in_x = lua_tointeger(L, 2);
        in_y = lua_tointeger(L, 3);
    }

    ObjectClass* this_object = Script_GetCacheObject(objectID);

    if (this_object != NULL) {

        TARGET this_target = 0;
        if (in_x >= 0 && in_y >= 0) {
            this_target = As_Target(XY_Cell(in_x, in_y));
        }

        if (this_object->IsRecentlyCreated) {
            Queue_Mission(TargetClass(this_object), MISSION_HUNT, 0, this_target);
        }

        Queue_Mission(TargetClass(this_object), MISSION_HUNT, 0, this_target);

        lua_pushnumber(L, 1);
        return 1;

    }

    lua_pushnumber(L, -1);
    return 1;

}

/***********************************************************************************************
 * Script_OrderGuard - Assigns a guard order to an object                                      *
 *                                                                                             *
 *   SCRIPT INPUT:	objectID   (int) - The object ID (retrieved via MS functions)              *
 *                                                                                             *
 *                  in_x  (opt)(int) - X location to attack                                    *
 *                                                                                             *
 *                  in_y  (opt)(int) - Y location to attack                                    *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_OrderGuard(lua_State* L) {

    int objectID = lua_tointeger(L, 1);

    int in_x = -1;
    int in_y = -1;
    if (lua_gettop(L) == 3) {
        in_x = lua_tointeger(L, 2);
        in_y = lua_tointeger(L, 3);
    }

    ObjectClass* this_object = Script_GetCacheObject(objectID);

    if (this_object != NULL) {

        TARGET this_target = 0;
        if (in_x >= 0 && in_y >= 0) {
            this_target = As_Target(XY_Cell(in_x, in_y));
        }

        if (this_object->IsRecentlyCreated) {
            Queue_Mission(TargetClass(this_object), MISSION_GUARD, 0, this_target);
        }

        Queue_Mission(TargetClass(this_object), MISSION_GUARD, 0, this_target);

        lua_pushnumber(L, 1);
        return 1;

    }

    lua_pushnumber(L, -1);
    return 1;

}

/***********************************************************************************************
 * Script_OrderRetreat - Assigns a retreat order to an object                                  *
 *                                                                                             *
 *   SCRIPT INPUT:	objectID   (int) - The object ID (retrieved via MS functions)              *
 *                                                                                             *
 *                  in_x  (opt)(int) - X location to attack                                    *
 *                                                                                             *
 *                  in_y  (opt)(int) - Y location to attack                                    *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_OrderRetreat(lua_State* L) {

    int objectID = lua_tointeger(L, 1);

    int in_x = -1;
    int in_y = -1;
    if (lua_gettop(L) == 3) {
        in_x = lua_tointeger(L, 2);
        in_y = lua_tointeger(L, 3);
    }

    ObjectClass* this_object = Script_GetCacheObject(objectID);

    if (this_object != NULL) {

        TARGET this_target = 0;
        if (in_x >= 0 && in_y >= 0) {
            this_target = As_Target(XY_Cell(in_x, in_y));
        }

        if (this_object->IsRecentlyCreated) {
            Queue_Mission(TargetClass(this_object), MISSION_RETREAT, 0, this_target);
        }

        Queue_Mission(TargetClass(this_object), MISSION_RETREAT, 0, this_target);

        lua_pushnumber(L, 1);
        return 1;

    }

    lua_pushnumber(L, -1);
    return 1;

}

/***********************************************************************************************
 * Script_OrderEnter - Assigns an enter order to an object                                     *
 *                                                                                             *
 *   SCRIPT INPUT:	objectID   (int) - The object ID (retrieved via MS functions)              *
 *                                                                                             *
 *                  in_x  (opt)(int) - X location to enter (building / unit)                   *
 *                                                                                             *
 *                  in_y  (opt)(int) - Y location to enter (building / unit)                   *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_OrderEnter(lua_State* L) {

    int objectID = lua_tointeger(L, 1);

    int in_x = -1;
    int in_y = -1;
    if (lua_gettop(L) == 3) {
        in_x = lua_tointeger(L, 2);
        in_y = lua_tointeger(L, 3);
    }

    ObjectClass* this_object = Script_GetCacheObject(objectID);

    if (this_object != NULL) {

        TARGET this_target = 0;
        if (in_x >= 0 && in_y >= 0) {
            this_target = As_Target(XY_Cell(in_x, in_y));
        }

        if (this_object->IsRecentlyCreated) {
            Queue_Mission(TargetClass(this_object), MISSION_ENTER, this_target, this_target);
        }

        Queue_Mission(TargetClass(this_object), MISSION_ENTER, this_target, this_target);

        lua_pushnumber(L, 1);
        return 1;

    }

    lua_pushnumber(L, -1);
    return 1;

}

/***********************************************************************************************
 * Script_OrderCapture - Assigns a capture order to an object                                  *
 *                                                                                             *
 *   SCRIPT INPUT:	objectID   (int) - The object ID (retrieved via MS functions)              *
 *                                                                                             *
 *                  in_x  (opt)(int) - X location to capture (building)                        *
 *                                                                                             *
 *                  in_y  (opt)(int) - Y location to capture (building)                        *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_OrderCapture(lua_State* L) {

    int objectID = lua_tointeger(L, 1);

    int in_x = -1;
    int in_y = -1;
    if (lua_gettop(L) == 3) {
        in_x = lua_tointeger(L, 2);
        in_y = lua_tointeger(L, 3);
    }

    ObjectClass* this_object = Script_GetCacheObject(objectID);

    if (this_object != NULL) {

        TARGET this_target = 0;
        if (in_x >= 0 && in_y >= 0) {
            this_target = As_Target(XY_Cell(in_x, in_y));
        }

        if (this_object->IsRecentlyCreated) {
            Queue_Mission(TargetClass(this_object), MISSION_CAPTURE, this_target, this_target);
        }

        Queue_Mission(TargetClass(this_object), MISSION_CAPTURE, this_target, this_target);

        lua_pushnumber(L, 1);
        return 1;

    }

    lua_pushnumber(L, -1);
    return 1;

}

/**********************************************************************************************
* Mission Utility Functions                                                                   *
*=============================================================================================*/

/***********************************************************************************************
 * Script_SetBriefingText - Sets the text on the mission briefing screen                       *
 *                                                                                             *
 *   SCRIPT INPUT:	(char*); The input text                                                    *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_SetBriefingText(lua_State* L) {
    strcpy(Scen.BriefingText, lua_tostring(L, -1));
    return -1;
}

/**********************************************************************************************
* Status / Location Utility Functions                                                         *
*=============================================================================================*/

/***********************************************************************************************
 * Script_GiveCredits - Gives or takes a given amount of credits to / from a player            *
 *                                                                                             *
 *   SCRIPT INPUT:	cashToGive (int) - The amount of cold hard credits to give the player.     *
 *                                     Negative numbers may be used to *take* money            *
 *                                                                                             *
 *                	houseType (int)  - The player (house) index to complete this transaction   *
 *                                     for, or -1 to give to all players                       *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   6/11/2020 - JM Created                                                                    *
 *   6/12/2020 - JJ Added player (house) index input                                           *
 *=============================================================================================*/
static int Script_GiveCredits(lua_State* L) {

    int cashToGive = lua_tointeger(L, 1);

    int houseType = -1;

    // Optional houseType parameter
    if (lua_gettop(L) == 2) {
        houseType = lua_tointeger(L, 2);
    }

    for (int h_index = 0; h_index < Houses.Count(); h_index++) {

        HouseClass* house = Houses.Ptr(h_index);

        if (house->ID == houseType || houseType == -1) {

            if (cashToGive < 0) {
                house->Spend_Money(max(0, cashToGive * -1));
            }
            else {
                house->Refund_Money(cashToGive);
            }

            // Discontinue giving out free cash if a specific house was specified
            if (houseType >= 0) {
                break;
            }
        }

    }

    return 1;
}

/***********************************************************************************************
 * Script_GetCredits - Gets how many credits the given player has                              *
 *                                                                                             *
 *   SCRIPT INPUT:	houseType (int)  - The player (house) index to get credits for             *
 *                                                                                             *
 *   SCRIPT OUTPUT:  result (number) - The amount of credits the player has                    *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_GetCredits(lua_State* L) {

    int houseType = lua_tointeger(L, -1);

    long result = -1;

    for (int h_index = 0; h_index < Houses.Count(); h_index++) {

        HouseClass* house = Houses.Ptr(h_index);

        if (house->ID == houseType || houseType == -1) {
            result = house->Credits;

            break;
        }

    }

    lua_pushnumber(L, result);

    return 1;
}

/***********************************************************************************************
 * Script_CountBuildings - Returns the amount of specific buildings for a player               *
 *                                                                                             *
 *   Optionally, you can return only building at a particular cell or in a rectangular area    *
 *   using the two groups of optional coordinate inputs                                        *
 *                                                                                             *
 *   SCRIPT INPUT:	structType (int) - The structure index to count buildings for              *
 *                                     or -1 for all structures                                *
 *                                                                                             *
 *                	houseType (int)  - The player (house) index to count buildings for         *
 *                	                   or -1 to for all players                                *
 *                                                                                             *
 *                                     NOTE: -1 for either index input acts as ALL             *
 *                                                                                             *
 *                  in_x1 (int) (optional A) - Only include buildings at or beginning at this  *
 *                                             Cell/X location                                 *
 *                                                                                             *
 *                  in_y1 (int) (optional A) - Only include buildings at or beginning at this  *
 *                                             Cell/Y location                                 *
 *                                                                                             *
 *                  in_x2 (int) (optional B) - Only include buildings at or before this Cell/X *
 *                                             location                                        *
 *                                                                                             *
 *                  in_y2 (int) (optional B) - Only include buildings at or before this Cell/Y *
 *                                             location                                        *
 *                                                                                             *
 *   SCRIPT OUTPUT:  result (number) - The amount of matching buildings for that player        *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_CountBuildings(lua_State* L) {
    int structType = lua_tointeger(L, 1);

    int houseType = -1;

    // Optional houseType parameter
    if (lua_gettop(L) == 2) {
        houseType = lua_tointeger(L, 2);
    }

    // Optional search area
    bool within_area = false;
    int in_x1, in_x2, in_y1, in_y2;
    if (lua_gettop(L) == 4) {
        in_x1 = lua_tointeger(L, 3);
        in_y1 = lua_tointeger(L, 4);
        in_x2 = in_x1;
        in_y2 = in_y1;
        within_area = true;
    }
    else if (lua_gettop(L) == 6) {
        in_x1 = lua_tointeger(L, 3);
        in_y1 = lua_tointeger(L, 4);
        in_x2 = lua_tointeger(L, 5);
        in_y2 = lua_tointeger(L, 6);
        within_area = true;
    }

    int result = 0;
    for (int b_index = 0; b_index < Buildings.Count(); b_index++) {
        BuildingClass* building = Buildings.Ptr(b_index);

        if (building->Owner() == houseType || houseType == -1) {
            if (building->Class->Type == structType || structType == -1) {

                // Count, in general
                if (within_area == false) {
                    result++;

                    // Within search area
                }
                else {

                    int this_x = Cell_X(Coord_Cell(building->Coord));
                    int this_y = Cell_Y(Coord_Cell(building->Coord));

                    if (this_x >= in_x1 && this_x <= in_x2 && this_y >= in_y1 && this_y <= in_y2) {
                        result++;
                    }
                }

            }
        }
    }

    lua_pushnumber(L, result);
    return 1;
}

/***********************************************************************************************
 * Script_CountAircraft - Returns the amount of specific aircraft for a player                 *
 *                                                                                             *
 *   Optionally, you can return only aircraft at a particular cell or in a rectangular area    *
 *   using the two groups of optional coordinate inputs                                        *
 *                                                                                             *
 *   SCRIPT INPUT:	aircraftType (int) - The aircraft index to count aircraft for              *
 *                                       or -1 for all aircraft                                *
 *                                                                                             *
 *                	houseType (int)    - The player (house) index to count aircraft for        *
 *                                       or -1 for all players                                 *
 *                                                                                             *
 *                                       NOTE: -1 for either index input acts as ALL           *
 *                                                                                             *
 *                  in_x1 (int) (optional A) - Only include aircraft at or beginning at this   *
 *                                             Cell/X location                                 *
 *                                                                                             *
 *                  in_y1 (int) (optional A) - Only include aircraft at or beginning at this   *
 *                                             Cell/Y location                                 *
 *                                                                                             *
 *                  in_x2 (int) (optional B) - Only include aircraft at or before this Cell/X  *
 *                                             location                                        *
 *                                                                                             *
 *                  in_y2 (int) (optional B) - Only include aircraft at or before this Cell/Y  *
 *                                             location                                        *
 *                                                                                             *
 *   SCRIPT OUTPUT:  result (number) - The amount of matching aircraft for that player         *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_CountAircraft(lua_State* L) {
    int aircraftType = lua_tointeger(L, 1);

    int houseType = -1;

    // Optional houseType parameter
    if (lua_gettop(L) == 2) {
        houseType = lua_tointeger(L, 2);
    }

    // Optional search area
    bool within_area = false;
    int in_x1, in_x2, in_y1, in_y2;
    if (lua_gettop(L) == 4) {
        in_x1 = lua_tointeger(L, 3);
        in_y1 = lua_tointeger(L, 4);
        in_x2 = in_x1;
        in_y2 = in_y1;
        within_area = true;
    }
    else if (lua_gettop(L) == 6) {
        in_x1 = lua_tointeger(L, 3);
        in_y1 = lua_tointeger(L, 4);
        in_x2 = lua_tointeger(L, 5);
        in_y2 = lua_tointeger(L, 6);
        within_area = true;
    }

    int result = 0;
    for (int a_index = 0; a_index < Aircraft.Count(); a_index++) {
        AircraftClass* aircraft = Aircraft.Ptr(a_index);

        if (aircraft->Owner() == houseType || houseType == -1) {
            if (aircraft->Class->Type == aircraftType || aircraftType == -1) {

                // Count, in general
                if (within_area == false) {
                    result++;

                    // Within search area
                }
                else {

                    int this_x = Cell_X(Coord_Cell(aircraft->Coord));
                    int this_y = Cell_Y(Coord_Cell(aircraft->Coord));

                    if (this_x >= in_x1 && this_x <= in_x2 && this_y >= in_y1 && this_y <= in_y2) {
                        result++;
                    }
                }

            }
        }
    }

    lua_pushnumber(L, result);
    return 1;
}

/***********************************************************************************************
 * Script_CountUnits - Returns the amount of specific units for a player                       *
 *                                                                                             *
 *   Optionally, you can return only units at a particular cell or in a rectangular area       *
 *   using the two groups of optional coordinate inputs                                        *
 *                                                                                             *
 *   SCRIPT INPUT:	unitType (int)  - The unit index to count units for                        *
 *                                    or -1 for all units                                      *
 *                                                                                             *
 *                	houseType (int) - The player (house) index to count units for              *
 *                                    or -1 for all players                                    *
 *                                                                                             *
 *                                    NOTE: -1 for either index input acts as ALL              *
 *                                                                                             *
 *                  in_x1 (int) (optional A) - Only include units at or beginning at this      *
 *                                             Cell/X location                                 *
 *                                                                                             *
 *                  in_y1 (int) (optional A) - Only include units at or beginning at this      *
 *                                             Cell/Y location                                 *
 *                                                                                             *
 *                  in_x2 (int) (optional B) - Only include units at or before this Cell/X     *
 *                                             location                                        *
 *                                                                                             *
 *                  in_y2 (int) (optional B) - Only include units at or before this Cell/Y     *
 *                                             location                                        *
 *                                                                                             *
 *   SCRIPT OUTPUT:  result (number) - The amount of matching units for that player            *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_CountUnits(lua_State* L) {
    int unitType = lua_tointeger(L, 1);

    int houseType = -1;

    // Optional houseType parameter
    if (lua_gettop(L) == 2) {
        houseType = lua_tointeger(L, 2);
    }

    // Optional search area
    bool within_area = false;
    int in_x1, in_x2, in_y1, in_y2;
    if (lua_gettop(L) == 4) {
        in_x1 = lua_tointeger(L, 3);
        in_y1 = lua_tointeger(L, 4);
        in_x2 = in_x1;
        in_y2 = in_y1;
        within_area = true;
    }
    else if (lua_gettop(L) == 6) {
        in_x1 = lua_tointeger(L, 3);
        in_y1 = lua_tointeger(L, 4);
        in_x2 = lua_tointeger(L, 5);
        in_y2 = lua_tointeger(L, 6);
        within_area = true;
    }

    int result = 0;
    for (int u_index = 0; u_index < Units.Count(); u_index++) {
        UnitClass* unit = Units.Ptr(u_index);

        if (unit->Owner() == houseType || houseType == -1) {
            if (unit->Class->Type == unitType || unitType == -1) {

                // Count, in general
                if (within_area == false) {
                    result++;

                    // Within search area
                }
                else {

                    int this_x = Cell_X(Coord_Cell(unit->Coord));
                    int this_y = Cell_Y(Coord_Cell(unit->Coord));

                    if (this_x >= in_x1 && this_x <= in_x2 && this_y >= in_y1 && this_y <= in_y2) {
                        result++;
                    }
                }

            }
        }
    }

    lua_pushnumber(L, result);
    return 1;
}

/***********************************************************************************************
 * Script_CountInfantry - Returns the amount of specific infantry for a player                 *
 *                                                                                             *
 *   Optionally, you can return only infantry at a particular cell or in a rectangular area    *
 *   using the two groups of optional coordinate inputs                                        *
 *                                                                                             *
 *   SCRIPT INPUT:	infantryType (int) - The infantry index to count infantry for              *
 *                                       or -1 for all infantry                                *
 *                                                                                             *
 *                	houseType (int)    - The player (house) index to count infantry for        *
 *                                       or -1 for all players                                 *
 *                                                                                             *
 *                                       NOTE: -1 for either index input acts as ALL           *
 *                                                                                             *
 *                  in_x1 (int) (optional A) - Only include infantry at or beginning at this   *
 *                                             Cell/X location                                 *
 *                                                                                             *
 *                  in_y1 (int) (optional A) - Only include infantry at or beginning at this   *
 *                                             Cell/Y location                                 *
 *                                                                                             *
 *                  in_x2 (int) (optional B) - Only include infantry at or before this Cell/X  *
 *                                             location                                        *
 *                                                                                             *
 *                  in_y2 (int) (optional B) - Only include infantry at or before this Cell/Y  *
 *                                             location                                        *
 *                                                                                             *
 *   SCRIPT OUTPUT:  result (number) - The amount of matching infantry for that player         *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_CountInfantry(lua_State* L) {
    int infantryType = lua_tointeger(L, 1);

    int houseType = -1;

    // Optional houseType parameter
    if (lua_gettop(L) == 2) {
        houseType = lua_tointeger(L, 2);
    }

    // Optional search area
    bool within_area = false;
    int in_x1, in_x2, in_y1, in_y2;
    if (lua_gettop(L) == 4) {
        in_x1 = lua_tointeger(L, 3);
        in_y1 = lua_tointeger(L, 4);
        in_x2 = in_x1;
        in_y2 = in_y1;
        within_area = true;
    }
    else if (lua_gettop(L) == 6) {
        in_x1 = lua_tointeger(L, 3);
        in_y1 = lua_tointeger(L, 4);
        in_x2 = lua_tointeger(L, 5);
        in_y2 = lua_tointeger(L, 6);
        within_area = true;
    }

    int result = 0;
    for (int u_index = 0; u_index < Infantry.Count(); u_index++) {
        InfantryClass* infantry = Infantry.Ptr(u_index);

        if (infantry->Owner() == houseType || houseType == -1) {
            if (infantry->Class->Type == infantryType || infantryType == -1) {

                // Count, in general
                if (within_area == false) {
                    result++;

                    // Within search area
                }
                else {

                    int this_x = Cell_X(Coord_Cell(infantry->Coord));
                    int this_y = Cell_Y(Coord_Cell(infantry->Coord));

                    if (this_x >= in_x1 && this_x <= in_x2 && this_y >= in_y1 && this_y <= in_y2) {
                        result++;
                    }
                }

            }
        }
    }

    lua_pushnumber(L, result);
    return 1;
}

/***********************************************************************************************
 * Script_CountVessels - Returns the amount of specific vessels for a player                   *
 *                                                                                             *
 *   Optionally, you can return only vessels at a particular cell or in a rectangular area     *
 *   using the two groups of optional coordinate inputs                                        *
 *                                                                                             *
 *   SCRIPT INPUT:	vesselType (int) - The vessel index to count vessels for                   *
 *                                     or -1 for all vessels                                   *
 *                                                                                             *
 *                	houseType (int)  - The player (house) index to count vessels for           *
 *                                     or -1 for all players                                   *
 *                                                                                             *
 *                                     NOTE: -1 for either index input acts as ALL             *
 *                                                                                             *
 *                  in_x1 (int) (optional A) - Only include vessels at or beginning at this    *
 *                                             Cell/X location                                 *
 *                                                                                             *
 *                  in_y1 (int) (optional A) - Only include vessels at or beginning at this    *
 *                                             Cell/Y location                                 *
 *                                                                                             *
 *                  in_x2 (int) (optional B) - Only include vessels at or before this Cell/X   *
 *                                             location                                        *
 *                                                                                             *
 *                  in_y2 (int) (optional B) - Only include vessels at or before this Cell/X   *
 *                                             location                                        *
 *                                                                                             *
 *   SCRIPT OUTPUT:  result (number) - The amount of matching vessels for that player          *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_CountVessels(lua_State* L) {
    int vesselType = lua_tointeger(L, 1);

    int houseType = -1;

    // Optional houseType parameter
    if (lua_gettop(L) == 2) {
        houseType = lua_tointeger(L, 2);
    }

    // Optional search area
    bool within_area = false;
    int in_x1, in_x2, in_y1, in_y2;
    if (lua_gettop(L) == 4) {
        in_x1 = lua_tointeger(L, 3);
        in_y1 = lua_tointeger(L, 4);
        in_x2 = in_x1;
        in_y2 = in_y1;
        within_area = true;
    }
    else if (lua_gettop(L) == 6) {
        in_x1 = lua_tointeger(L, 3);
        in_y1 = lua_tointeger(L, 4);
        in_x2 = lua_tointeger(L, 5);
        in_y2 = lua_tointeger(L, 6);
        within_area = true;
    }

    int result = 0;
    for (int u_index = 0; u_index < Vessels.Count(); u_index++) {
        VesselClass* vessel = Vessels.Ptr(u_index);

        if (vessel->Owner() == houseType || houseType == -1) {
            if (vessel->Class->Type == vesselType || vesselType == -1) {

                // Count, in general
                if (within_area == false) {
                    result++;

                    // Within search area
                }
                else {

                    int this_x = Cell_X(Coord_Cell(vessel->Coord));
                    int this_y = Cell_Y(Coord_Cell(vessel->Coord));

                    if (this_x >= in_x1 && this_x <= in_x2 && this_y >= in_y1 && this_y <= in_y2) {
                        result++;
                    }
                }

            }
        }
    }

    lua_pushnumber(L, result);
    return 1;
}

/***********************************************************************************************
 * Script_GetWaypointX - Gets the Cell/X Coordinate of Waypoint                                *
 *                                                                                             *
 *   SCRIPT INPUT:	waypointIndex (int) - The waypoint's index to grab the X coordinate for    *
 *                                                                                             *
 *   SCRIPT OUTPUT:  outX (int) - The Cell/X coordinate of the waypoint                        *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_GetWaypointX(lua_State* L) {

    int ret = lua_tointeger(L, -1);

    if (ret > 0 && ret < sizeof(Scen.Waypoint)) {

        lua_pushnumber(L, Cell_X(Scen.Waypoint[ret]));

        return 1;

    }

    lua_pushnumber(L, -1);

    return -1;
}

/***********************************************************************************************
 * Script_GetWaypointY - Gets the Cell/Y Coordinate of Waypoint                                *
 *                                                                                             *
 *   SCRIPT INPUT:	waypointIndex (int) - The waypoint's index to grab the Y coordinate for    *
 *                                                                                             *
 *   SCRIPT OUTPUT:  outX (int) - The Cell/Y coordinate of the waypoint                        *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_GetWaypointY(lua_State* L) {

    int ret = lua_tointeger(L, -1);

    if (ret > 0 && ret < sizeof(Scen.Waypoint)) {

        lua_pushnumber(L, Cell_Y(Scen.Waypoint[ret]));

        return 1;

    }

    lua_pushnumber(L, -1);

    return -1;
}

/**********************************************************************************************
* Special / Debug Functions                                                                   *
*=============================================================================================*/

/***********************************************************************************************
 * Script_Console_Print - Prints text to the console                                           *
 *                                                                                             *
 *   SCRIPT INPUT:	inputText (string) - The text to print to the console                      *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * INPUT:  lua_State - The current Lua state                                                   *
 *                                                                                             *
 * OUTPUT:  int; Did the function run successfully? Return 1                                   *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
static int Script_Console_Print(lua_State* L) {

    const char* inputText = lua_tostring(L, -1); // grab path string from top of stack

    Console_Printf(inputText);

    return -1;
}


/**********************************************************************************************
* MapScript Internal Functions                                                                *
*=============================================================================================*/

/***********************************************************************************************
 * MapScript::GetCacheObject -- Returns ObjectClass from cache with given ID                   *
 *                                                                                             *
 * INPUT:  input_object_id (int)         - Input object ID                                     *
 *                                                                                             *
 * OUTPUT:  this_object (ObjectClass*)   - The object with the ID or NULL                      *
 *                                                                                             *
 * WARNINGS:  none                                                                             *
 *                                                                                             *
 *=============================================================================================*/
ObjectClass* Script_GetCacheObject(int input_object_id) {

    if (input_object_id < 0) {
        return false;
    }

    // Look through cache for object with ID, and return it
    for (std::vector<MapScriptObject*>::iterator it = Scen.mapScript->ObjectCache.begin(); it != Scen.mapScript->ObjectCache.end(); ++it) {//Error 2-4

        MapScriptObject* thisCacheObject = (MapScriptObject*)(*it);

        // Here it is
        if (thisCacheObject->UID == input_object_id) {

            // Determine type of object and return it from cache. If it doesn't exist any longer, remove it from cache
            switch (thisCacheObject->RTTI) {
            case RTTI_BUILDING: {
                BuildingClass* this_building = Buildings.Ptr(thisCacheObject->classIndex);
                if (this_building != NULL) {
                    return this_building;
                }
                else {
                    it = Scen.mapScript->ObjectCache.erase(it); // Cache item exists but is invalidated; remove from cache
                    return NULL;
                }
            }break;
            case RTTI_UNIT: {
                UnitClass* this_unit = Units.Ptr(thisCacheObject->classIndex);
                if (this_unit != NULL) {
                    return this_unit;
                }
                else {
                    it = Scen.mapScript->ObjectCache.erase(it); // Cache item exists but is invalidated; remove from cache
                    return NULL;
                }
            }break;
            case RTTI_AIRCRAFT: {
                AircraftClass* this_aircraft = Aircraft.Ptr(thisCacheObject->classIndex);
                if (this_aircraft != NULL) {
                    return this_aircraft;
                }
                else {
                    it = Scen.mapScript->ObjectCache.erase(it); // Cache item exists but is invalidated; remove from cache
                    return NULL;
                }
            }break;
            case RTTI_INFANTRY: {
                InfantryClass* this_infantry = Infantry.Ptr(thisCacheObject->classIndex);
                if (this_infantry != NULL) {
                    return this_infantry;
                }
                else {
                    it = Scen.mapScript->ObjectCache.erase(it); // Cache item exists but is invalidated; remove from cache
                    return NULL;
                }
            }break;
            case RTTI_VESSEL: {
                VesselClass* this_vessel = Vessels.Ptr(thisCacheObject->classIndex);
                if (this_vessel != NULL) {
                    return this_vessel;
                }
                else {
                    it = Scen.mapScript->ObjectCache.erase(it); // Cache item exists but is invalidated; remove from cache
                    return NULL;
                }
            }break;
            }

        }

    }

    // Not found in cache; let's find the object by ID the hard way... If scripting is done well, this doesn't get hit
    int _objectIndex = -1;
    MapScriptObject* CacheObject = Scen.mapScript->CreateCacheObject();
    CacheObject->ID = -1;

    _objectIndex = Script_BuildingIndexFromID(input_object_id);
    BuildingClass* this_building = Buildings.Ptr(_objectIndex);
    if (this_building != NULL) {

        CacheObject->ID = this_building->ID;
        CacheObject->RTTI = this_building->RTTI;
        CacheObject->classIndex = _objectIndex;
        Scen.mapScript->ObjectCache.push_back(CacheObject);

        return this_building;

    }
    else {
        _objectIndex = Script_UnitIndexFromID(input_object_id);
        UnitClass* this_unit = Units.Ptr(_objectIndex);
        if (this_unit != NULL) {

            CacheObject->ID = this_unit->ID;
            CacheObject->RTTI = this_unit->RTTI;
            CacheObject->classIndex = _objectIndex;
            Scen.mapScript->ObjectCache.push_back(CacheObject);

            return this_unit;

        }
        else {
            _objectIndex = Script_AircraftIndexFromID(input_object_id);
            AircraftClass* this_aircraft = Aircraft.Ptr(_objectIndex);
            if (this_aircraft != NULL) {

                CacheObject->ID = this_aircraft->ID;
                CacheObject->RTTI = this_aircraft->RTTI;
                CacheObject->classIndex = _objectIndex;
                Scen.mapScript->ObjectCache.push_back(CacheObject);

                return this_aircraft;

            }
            else {
                _objectIndex = Script_VesselIndexFromID(input_object_id);
                VesselClass* this_vessel = Vessels.Ptr(_objectIndex);
                if (this_vessel != NULL) {

                    CacheObject->ID = this_vessel->ID;
                    CacheObject->RTTI = this_vessel->RTTI;
                    CacheObject->classIndex = _objectIndex;
                    Scen.mapScript->ObjectCache.push_back(CacheObject);

                    return this_vessel;

                }
                else {
                    _objectIndex = Script_InfantryIndexFromID(input_object_id);
                    InfantryClass* this_infantry = Infantry.Ptr(_objectIndex);
                    if (this_infantry != NULL) {

                        CacheObject->ID = this_infantry->ID;
                        CacheObject->RTTI = this_infantry->RTTI;
                        CacheObject->classIndex = _objectIndex;
                        Scen.mapScript->ObjectCache.push_back(CacheObject);

                        return this_infantry;

                    }
                }
            }
        }
    }

    // Looks like we didn't find anything. They probably shouldn't have gotten this far in the first place so it was worth a try.
    delete CacheObject;

    return NULL;
}

/***********************************************************************************************
 * Script_SetObjectTrigger -- Figures out what kind of object is given and sets up trigger     *
 *                                                                                             *
 * INPUT:  input_object (ObjectClass*)   - Input object                                        *
 *         input_trigger (TriggerClass*) - Input Trigger                                       *
 *                                                                                             *
 * OUTPUT:  bool; Was the object found? Does the trigger exist?                                *
 *                                                                                             *
 * WARNINGS:  none                                                                             *
 *                                                                                             *
 *=============================================================================================*/
bool Script_SetObjectTrigger(int input_object_id, TriggerClass* input_trigger) {

    if (input_object_id < 0) {
        return false;
    }

    if (input_trigger == NULL) {
        return false;
    }

    ObjectClass* this_object = Script_GetCacheObject(input_object_id);

    if (this_object != NULL) {

        switch (this_object->RTTI) {
        case RTTI_BUILDING: {
            input_trigger->Class->Event1.Data.Structure = dynamic_cast<BuildingClass*>(this_object)->Class->Type;
            dynamic_cast<BuildingClass*>(this_object)->Trigger = input_trigger;
        }break;
        case RTTI_UNIT: {
            input_trigger->Class->Event1.Data.Unit = dynamic_cast<UnitClass*>(this_object)->Class->Type;
            dynamic_cast<UnitClass*>(this_object)->Trigger = input_trigger;
        }break;
        case RTTI_AIRCRAFT: {
            input_trigger->Class->Event1.Data.Aircraft = dynamic_cast<AircraftClass*>(this_object)->Class->Type;
            dynamic_cast<AircraftClass*>(this_object)->Trigger = input_trigger;
        }break;
        case RTTI_INFANTRY: {
            input_trigger->Class->Event1.Data.Infantry = dynamic_cast<InfantryClass*>(this_object)->Class->Type;
            dynamic_cast<InfantryClass*>(this_object)->Trigger = input_trigger;
        }break;
        }

    }

    return false;
}

/***********************************************************************************************
 * Script_BuildingIndexFromID -- Returns BuildingClass or NULL depending on the input               *
 *                                                                                             *
 * INPUT:  input_object_id (int)   - Input object ID                                           *
 *                                                                                             *
 * OUTPUT:  bool; Was the object found? Does the trigger exist?                                *
 *                                                                                             *
 * WARNINGS:  none                                                                             *
 *                                                                                             *
 *=============================================================================================*/
int Script_BuildingIndexFromID(int input_object_id) {

    if (input_object_id < 0) {
        return false;
    }

    for (int _index = 0; _index < Buildings.Count(); _index++) {
        BuildingClass* _object = Buildings.Ptr(_index);

        if (_object->ID == input_object_id) {
            return _index;
        }
    }

    return -1;
}

/***********************************************************************************************
 * Script_UnitIndexFromID -- Returns UnitClass* or NULL depending on the input                      *
 *                                                                                             *
 * INPUT:  input_unit_id (int)   - Input Unit ID                                               *
 *                                                                                             *
 * OUTPUT:  bool; Was the object found? Does the trigger exist?                                *
 *                                                                                             *
 * WARNINGS:  none                                                                             *
 *                                                                                             *
 *=============================================================================================*/
int Script_UnitIndexFromID(int input_object_id) {

    if (input_object_id < 0) {
        return false;
    }

    for (int _index = 0; _index < Units.Count(); _index++) {
        UnitClass* _object = Units.Ptr(_index);

        if (_object->ID == input_object_id) {
            return _index;
        }
    }

    return -1;
}

/***********************************************************************************************
 * Script_AircraftIndexFromID -- Returns AircraftClass* or NULL depending on the input              *
 *                                                                                             *
 * INPUT:  input_object_id (int)   - Input object ID                                           *
 *                                                                                             *
 * OUTPUT:  bool; Was the object found? Does the trigger exist?                                *
 *                                                                                             *
 * WARNINGS:  none                                                                             *
 *                                                                                             *
 *=============================================================================================*/
int Script_AircraftIndexFromID(int input_object_id) {

    if (input_object_id < 0) {
        return false;
    }

    for (int _index = 0; _index < Aircraft.Count(); _index++) {
        AircraftClass* _object = Aircraft.Ptr(_index);

        if (_object->ID == input_object_id) {
            return _index;
        }
    }

    return -1;
}

/***********************************************************************************************
 * Script_InfantryIndexFromID -- Returns InfantryClass* or NULL depending on the input              *
 *                                                                                             *
 * INPUT:  input_unit_id (int)   - Input Infantry ID                                           *
 *                                                                                             *
 * OUTPUT:  bool; Was the object found? Does the trigger exist?                                *
 *                                                                                             *
 * WARNINGS:  none                                                                             *
 *                                                                                             *
 *=============================================================================================*/
int Script_InfantryIndexFromID(int input_object_id) {

    if (input_object_id < 0) {
        return false;
    }

    for (int _index = 0; _index < Infantry.Count(); _index++) {
        InfantryClass* _object = Infantry.Ptr(_index);

        if (_object->ID == input_object_id) {
            return _index;
        }
    }

    return -1;
}

/***********************************************************************************************
 * Script_VesselIndexFromID -- Returns VesselClass* or NULL depending on the input                  *
 *                                                                                             *
 * INPUT:  input_unit_id (int)   - Input vessel ID                                             *
 *                                                                                             *
 * OUTPUT:  bool; Was the object found? Does the trigger exist?                                *
 *                                                                                             *
 * WARNINGS:  none                                                                             *
 *                                                                                             *
 *=============================================================================================*/
int Script_VesselIndexFromID(int input_object_id) {

    if (input_object_id < 0) {
        return false;
    }

    for (int _index = 0; _index < Vessels.Count(); _index++) {
        VesselClass* _object = Vessels.Ptr(_index);

        if (_object->ID == input_object_id) {
            return _index;
        }
    }

    return -1;
}

/***********************************************************************************************
 * Script_CacheDefaultAttribute - Caches an attribute value upon first usage for restoration   *
 *                                                                                             *
 *   SCRIPT INPUT:	input_rtti (RTTIType) - The RTTI type for the attrtibute                   *
 *                	input_id   (int)      - The ID of the typeclass                            *
 *                	input_attribute (int) - The attribute to cache                             *
 *                	input_value (int)     - The value if the cache item doesn't exist          *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
void Script_CacheDefaultObjectTypeAttribute(RTTIType input_rtti, int input_id, MapScriptObjectAttributeType input_attribute, int input_value) {

    bool cache_item_found = false;
    for (std::vector<MapScriptDefaultObjectAttribute*>::iterator it = Scen.mapScript->DefaultObjectTypeAttributeCache.begin(); it != Scen.mapScript->DefaultObjectTypeAttributeCache.end(); ++it) {//Error 2-4

        MapScriptDefaultObjectAttribute* cachedAttribute = (MapScriptDefaultObjectAttribute*)(*it);

        if (input_rtti == cachedAttribute->RTTI && input_id == cachedAttribute->ID && input_attribute == cachedAttribute->attribute) {
            cache_item_found = true;
            break;
        }

    }

    // Cached attribute does not exist; let's cache!
    if (!cache_item_found) {

        MapScriptDefaultObjectAttribute* cachedAttribute = new MapScriptDefaultObjectAttribute;
        cachedAttribute->RTTI = input_rtti;
        cachedAttribute->ID = input_id;
        cachedAttribute->attribute = input_attribute;
        cachedAttribute->value = input_value;
        Scen.mapScript->DefaultObjectTypeAttributeCache.push_back(cachedAttribute);

    }

}

/***********************************************************************************************
 * MapScript::CallFunction - Calls a given function within the current script                  *
 *                                                                                             *
 * INPUT:  functionName - The name of the function to call                                     *
 *                                                                                             *
 * OUTPUT:  void                                                                               *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
void MapScript::CallFunction(const char* functionName) {

    lua_getglobal(L, functionName);
    lua_pcall(L, 0, 0, 0);

}

/***********************************************************************************************
 * MapScript::setLuaPath - Sets Lua's path / extensions to search for scripts                  *
 *                                                                                             *
 *   This helps Lua know where/how to look for files when using require(), otherwise we could  *
 *   hard code path/extension as we did previously                                             *
 *                                                                                             *
 * INPUT:  pathvalue - See lua documentation RE: LUA_PATH to know how to deal with this        *
 *                                                                                             *
 * OUTPUT:  void                                                                               *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
void MapScript::SetLuaPath(const char* input_path)
{

    // Needs space to keep the initial paths
    char new_path[1024];

    // Set new GLOBALS.package.path
    lua_getglobal(L, "package");
    lua_getfield(L, -1, "path"); // get field "path" from table at top of stack (-1)
    const char* cur_path = lua_tostring(L, -1); // grab path string from top of stack

    sprintf(new_path, "%s;%s", cur_path, input_path);

    lua_pop(L, 1);
    lua_pushstring(L, new_path);
    lua_setfield(L, -2, "path");
    lua_pop(L, 1);
}

/***********************************************************************************************
 * MapScript::CreateCacheObject -- Initializes a cached object for a new cache entry           *
 *                                                                                             *
 * INPUT:  mapName - The script path                                                           *
 *                                                                                             *
 * OUTPUT:  bool; did the map script load and initialize?                                      *
 *                                                                                             *
 * WARNINGS:  none                                                                             *
 *                                                                                             *
 *=============================================================================================*/
MapScriptObject* MapScript::CreateCacheObject() {

    MapScriptObject* CacheObject = new MapScriptObject;

    cUID++;
    CacheObject->UID = cUID;

    return CacheObject;

}

/***********************************************************************************************
 * MapScript::Deinit - Deinitialize/free any data used by MapScript (used within destructor)   *
 *                                                                                             *
 *   SCRIPT INPUT:	none                                                                       *
 *                                                                                             *
 *   SCRIPT OUTPUT:  void                                                                      *
 *                                                                                             *
 * WARNINGS:  ?                                                                                *
 *                                                                                             *
 *=============================================================================================*/
void MapScript::Deinit() {

    // Remove any cached objects
    for (int i = 0; i < ObjectCache.size(); ++i)
    {
        delete ObjectCache[i];
    }

    ObjectCache.clear();

    // We will now restore any default Object attributes that were changed in the script
    for (std::vector<MapScriptDefaultObjectAttribute*>::iterator it = DefaultObjectTypeAttributeCache.begin(); it != DefaultObjectTypeAttributeCache.end(); ++it) {//Error 2-4

        MapScriptDefaultObjectAttribute* cachedAttribute = (MapScriptDefaultObjectAttribute*)(*it);

        switch (cachedAttribute->RTTI) {

        case RTTI_BUILDINGTYPE: {
            lua_getglobal(L, "SetBuildingTypeAttribute");
            lua_pushnumber(L, cachedAttribute->ID);
            lua_pushnumber(L, cachedAttribute->attribute);
            lua_pushnumber(L, cachedAttribute->value);
            lua_pcall(L, 3, 1, 0);
        }break;
        case RTTI_UNITTYPE: {
            lua_getglobal(L, "SetUnitTypeAttribute");
            lua_pushnumber(L, cachedAttribute->ID);
            lua_pushnumber(L, cachedAttribute->attribute);
            lua_pushnumber(L, cachedAttribute->value);
            lua_pcall(L, 3, 1, 0);
        }break;
        case RTTI_INFANTRYTYPE: {
            lua_getglobal(L, "SetInfantryTypeAttribute");
            lua_pushnumber(L, cachedAttribute->ID);
            lua_pushnumber(L, cachedAttribute->attribute);
            lua_pushnumber(L, cachedAttribute->value);
            lua_pcall(L, 3, 1, 0);
        }break;
        case RTTI_AIRCRAFTTYPE: {
            lua_getglobal(L, "SetAircraftTypeAttribute");
            lua_pushnumber(L, cachedAttribute->ID);
            lua_pushnumber(L, cachedAttribute->attribute);
            lua_pushnumber(L, cachedAttribute->value);
            lua_pcall(L, 3, 1, 0);
        }break;
        case RTTI_VESSELTYPE: {
            lua_getglobal(L, "SetVesselTypeAttribute");
            lua_pushnumber(L, cachedAttribute->ID);
            lua_pushnumber(L, cachedAttribute->attribute);
            lua_pushnumber(L, cachedAttribute->value);
            lua_pcall(L, 3, 1, 0);
        }break;

        }

    }

    // Remove any cached Object Attributes
    for (int i = 0; i < DefaultObjectTypeAttributeCache.size(); ++i)
    {
        delete DefaultObjectTypeAttributeCache[i];
    }

    DefaultObjectTypeAttributeCache.clear();


    for (int index = 0; index < Warheads.Count(); index++) {
        Warheads.Ptr(index)->Read_INI(RuleINI);
    }

    for (int proj = 0; proj < BulletTypes.Count(); proj++) {
        BulletTypes.Ptr(proj)->Read_INI(RuleINI);
    }

    for (int windex = 0; windex < Weapons.Count(); windex++) {
        Weapons.Ptr(windex)->Read_INI(RuleINI);
    }

    for (int uindex = 0; uindex < UnitTypes.Count(); uindex++) {
        UnitTypes.Ptr(uindex)->Read_INI(RuleINI);
    }

    for (int iindex = 0; iindex < InfantryTypes.Count(); iindex++) {
        InfantryTypes.Ptr(iindex)->Read_INI(RuleINI);
    }

    for (int vindex = 0; vindex < VesselTypes.Count(); vindex++) {
        VesselTypes.Ptr(vindex)->Read_INI(RuleINI);
    }

    for (int aindex = 0; aindex < AircraftTypes.Count(); aindex++) {
        AircraftTypes.Ptr(aindex)->Read_INI(RuleINI);
    }

    for (int bindex = 0; bindex < BuildingTypes.Count(); bindex++) {
        BuildingTypes.Ptr(bindex)->Read_INI(RuleINI);
    }

}


/***********************************************************************************************
 * MapScript::Init -- Loads and initializes script for a given map                             *
 *                                                                                             *
 *    This routine is called at the beginning of a mission                                     *
 *                                                                                             *
 * INPUT:  mapName - The script path                                                           *
 *                                                                                             *
 * OUTPUT:  bool; did the map script load and initialize?                                      *
 *                                                                                             *
 * WARNINGS:  none                                                                             *
 *                                                                                             *
 *=============================================================================================*/
bool MapScript::Init(const char* mapName) {

    this->cUID = 0;

    char scriptName[256];

    // Build map path
    sprintf(scriptName, "scripts/%s.script", mapName);
    L = luaL_newstate();

    luaL_openlibs(L);

    // Load file
    if (luaL_loadfile(L, scriptName)) {
        Console_Printf(lua_tostring(L, -1));
        L = NULL;
        return false;
    }

    // TODO: I've tried several combinations of paths (including "scripts/?"), 
    // but I can only get "require 'basefilename'" to work within a 
    // script, not "require 'basefilename.script'". This may be completely 
    // normal within lua, but it seems odd.

    // This allows require() to work
    SetLuaPath(";scripts/?.script;");

    if (lua_pcall(L, 0, 0, 0)) {
        Console_Printf(lua_tostring(L, -1));
        L = NULL;
        return false;
    }

    // Reserve object cache memory
    ObjectCache.reserve(2000);

    // Reserve type attribute cache memory (to restore rules after script)
    DefaultObjectTypeAttributeCache.reserve(1000);

    /**********************************************************************************************
    * Red Alert Vanilla Actions                                                                   *
    *=============================================================================================*/

    lua_register(L, "Win", Script_Win);                                             // player wins!
    lua_register(L, "Lose", Script_Lose);                                           // player loses.
    lua_register(L, "BeginProduction", Script_BeginProduction);                     // computer begins factory production.
    lua_register(L, "CreateTeam", Script_CreateTeam);                               // computer creates a certain type of team
    lua_register(L, "DestroyTeam", Script_DestroyTeam);                             // destroy team (comment was redacted. was something offensive originally written?)
    lua_register(L, "AllHunt", Script_AllHunt);                                     // all enemy units go into hunt mode (teams destroyed).
    lua_register(L, "Reinforcements", Script_Reinforcements);                       // player gets reinforcements (house that gets them is determined by the Reinforcement instance)
    lua_register(L, "DropZoneFlare", Script_DropZoneFlare);                         // Deploy drop zone smoke.
    lua_register(L, "FireSale", Script_FireSale);	                                // Sell all buildings and go on rampage.
    lua_register(L, "PlayMovie", Script_PlayMovie);							        // Play movie (temporarily suspend game).
    lua_register(L, "TriggerText", Script_TriggerText);                             // Triggers a text message display.
    lua_register(L, "DestroyTrigger", Script_DestroyTrigger);                       // Destroy specified trigger.
    lua_register(L, "AutoCreate", Script_AutoCreate);                               // Computer to autocreate teams.
    // Win if captured, lose if destroyed                                           // function does not exist
    lua_register(L, "AllowWin", Script_AllowWin);                                   // Allows winning if triggered.
    lua_register(L, "RevealAll", Script_RevealAll);                                 // Reveal the entire map.
    lua_register(L, "RevealCell", Script_RevealCell);                               // Reveal map around cell #.
    lua_register(L, "RevealZone", Script_RevealZone);                               // Reveal all of specified zone.
    lua_register(L, "PlaySound", Script_PlaySound);                                 // Play sound effect.
    lua_register(L, "PlayMusic", Script_PlayMusic);                                 // Play musical score.
    lua_register(L, "PlaySpeech", Script_PlaySpeech);						        // Play EVA speech.
        // TODO: Force trigger to activate.
    lua_register(L, "StartMissionTimer", Script_StartMissionTimer);			        // Start mission timer.
    lua_register(L, "StopMissionTimer", Script_StopMissionTimer);			        // Stop mission timer.
    lua_register(L, "IncreaseMissionTimer", Script_IncreaseMissionTimer);           // Increase mission timer time.
    lua_register(L, "DecreaseMissionTimer", Script_DecreaseMissionTimer);           // Decrease mission timer time.
    lua_register(L, "SetMissionTimer", Script_SetMissionTimer);                     // Set and start the mission timer.
    lua_register(L, "SetGlobalValue", Script_SetGlobalValue);	                    // Set global variable.
    lua_register(L, "ClearGlobalValue", Script_ClearGlobalValue);                   // Clear global variable.
    lua_register(L, "AutoBaseBuilding", Script_AutoBaseBuilding);                   // Automated base building.
    lua_register(L, "CreepShadow", Script_CreepShadow);                             // Shadow grows back one 'step'.
    lua_register(L, "DestroyTriggerBuilding", Script_DestroyTriggerBuilding);       // Destroys the building this trigger is attached to. (in the case of scripging, we can supply the trigger)
    lua_register(L, "GiveOneTimeSpecialWeapon", Script_GiveOneTimeSpecialWeapon);   // Add a one-time special weapon ability to house.
    lua_register(L, "GiveSpecialWeapon", Script_GiveSpecialWeapon);                 // Add a repeating special weapon ability to house.
    lua_register(L, "DesignatePreferredTarget", Script_DesignatePreferredTarget);   // Designates preferred target for house.
    lua_register(L, "LaunchFakeNukes", Script_LaunchFakeNukes);                     // Launch fake nuclear missiles from all silos

/**********************************************************************************************
* Red Alert Vanilla Events                                                                    *
*=============================================================================================*/

    lua_register(L, "CellCallback", Script_CellCallback);                                   // player enters this square (or group of squares)
    lua_register(L, "SpiedByCallback", Script_SpiedByCallback);                             // Spied by.
    // Thieved by (raided or stolen vehicle).                                               // This doesn't appear to be implemented in the engine (?)
    lua_register(L, "ObjectDiscoveryCallback", Script_ObjectDiscoveryCallback);             // player discovers this object
    lua_register(L, "HouseDiscoveredCallback", Script_HouseDiscoveredCallback);             // House has been discovered.
    lua_register(L, "ObjectAttackedCallback", Script_ObjectAttackedCallback);               // player attacks this object
    lua_register(L, "ObjectDestroyedCallback", Script_ObjectDestroyedCallback);             // player destroys this object
    lua_register(L, "AnyEventCallback", Script_AnyEventCallback);                           // Any object event will cause the trigger.
    lua_register(L, "AllUnitsDestroyedCallback", Script_AllUnitsDestroyedCallback);         // all house's units destroyed
    lua_register(L, "AllBuildingsDestroyedCallback", Script_AllBuildingsDestroyedCallback); // all house's buildings destroyed
    lua_register(L, "AllDestroyedCallback", Script_AllDestroyedCallback);                   // all house's units & buildings destroyed
    lua_register(L, "CreditsReachedCallback", Script_CreditsReachedCallback);               // house reaches this many credits
    lua_register(L, "TimeReachedCallback", Script_TimeReachedCallback);                     // Scenario elapsed time from start.
    lua_register(L, "MissionTimerCallback", Script_MissionTimerCallback);                   // Pre expired mission timer.
    lua_register(L, "BuildingsDestroyedCallback", Script_BuildingsDestroyedCallback);       // Number of buildings destroyed.
    lua_register(L, "UnitsDestroyedCallback", Script_UnitsDestroyedCallback);               // Number of units destroyed.
    lua_register(L, "NoFactoriesCallback", Script_NoFactoriesCallback);                     // No factories left.
    lua_register(L, "CivilianEscapeCallback", Script_CivilianEscapeCallback);               // Civilian has been evacuated.
    lua_register(L, "BuildingBuiltCallback", Script_BuildingBuiltCallback);                 // Specified building has been built.
    lua_register(L, "UnitBuiltCallback", Script_UnitBuiltCallback);                         // Specified unit has been built.
    lua_register(L, "InfantryBuiltCallback", Script_InfantryBuiltCallback);                 // Specified infantry has been built.
    lua_register(L, "AircraftBuiltCallback", Script_AircraftBuiltCallback);                 // Specified aircraft has been built.
    lua_register(L, "TeamLeavesMapCallback", Script_TeamLeavesMapCallback);                 // Specified team member leaves map.
    lua_register(L, "ZoneEntryCallback", Script_ZoneEntryCallback);                         // Enters same zone as waypoint 'x'.
    lua_register(L, "HorizontalCrossCallback", Script_HorizontalCrossCallback);             // Crosses horizontal trigger line.
    lua_register(L, "VerticalCrossCallback", Script_VerticalCrossCallback);                 // Crosses vertical trigger line.
    lua_register(L, "GlobalSetCallback", Script_GlobalSetCallback);                         // If specified global has been set. (somewhat useless with Scripting)
    lua_register(L, "GlobalClearedCallback", Script_GlobalClearedCallback);                 // If specified global has been cleared. (somewhat useless with Scripting)
    // If all fake structures are gone.                                                     // This doesn't appear to be implemented in the engine (?)
    lua_register(L, "LowPowerCallback", Script_LowPowerCallback);                           // When power drops below 100%.
    lua_register(L, "AllBridgesDestroyedCallback", Script_AllBridgesDestroyedCallback);     // All bridges destroyed.
    lua_register(L, "BuildingExistsCallback", Script_BuildingExistsCallback);               // Check for building existing.

/**********************************************************************************************
* Trigger Utility Functions                                                                   *
*=============================================================================================*/

    lua_register(L, "SetTriggerCallback", Script_SetTriggerCallback);	            // Initiates a given [callback] on an existing [trigger] // TODO: Make this work via name OR ID
    lua_register(L, "TriggerAddCell", Script_TriggerAddCell);	                    // Initiates a given [callback] on an existing [trigger]
    lua_register(L, "MergeTriggers", Script_MergeTriggers);	                        // Combines the events of two triggers (for multi-event triggers :)
    lua_register(L, "GetTriggerByName", Script_GetTriggerByName);	                // Gets the ID of the trigger with a given name
    lua_register(L, "TriggerSetPersistence", Script_TriggerSetPersistence);	        // Does a trigger go on and on or just run once?

/**********************************************************************************************
* Object Functions                                                                            *
*=============================================================================================*/
    lua_register(L, "GetCellObject", Script_GetCellObject);	                        // Gets an object (ID - building/vehicle/aircraft/infantry/vessel), if any, at [Cell/X],[Cell/Y]

    lua_register(L, "SetObjectAttribute", Script_SetObjectAttribute);	            // Sets an object's attributes (strength, etc.)
    lua_register(L, "SetBuildingTypeAttribute", Script_SetBuildingTypeAttribute);	// Sets the attribute of a Building type
    lua_register(L, "SetVehicleTypeAttribute", Script_SetVehicleTypeAttribute);	    // Sets the attribute of a Vehicle type
    lua_register(L, "SetInfantryTypeAttribute", Script_SetInfantryTypeAttribute);	// Sets the attribute of an Infantry type
    lua_register(L, "SetVesselTypeAttribute", Script_SetVesselTypeAttribute);	    // Sets the attribute of a Vessel type
    lua_register(L, "SetAircraftTypeAttribute", Script_SetAircraftTypeAttribute);	// Sets the attribute of an Aircraft type

    lua_register(L, "GetObjectAttribute", Script_GetObjectAttribute);	            // Sets an object's attributes (strength, etc.)
    lua_register(L, "GetBuildingTypeAttribute", Script_GetBuildingTypeAttribute);	// Sets the attribute of a Building type
    lua_register(L, "GetVehicleTypeAttribute", Script_GetVehicleTypeAttribute);	    // Sets the attribute of a Vehicle type
    lua_register(L, "GetInfantryTypeAttribute", Script_GetInfantryTypeAttribute);	// Sets the attribute of an Infantry type
    lua_register(L, "GetVesselTypeAttribute", Script_GetVesselTypeAttribute);	    // Sets the attribute of a Vessel type
    lua_register(L, "GetAircraftTypeAttribute", Script_GetAircraftTypeAttribute);	// Sets the attribute of an Aircraft type

    lua_register(L, "SetHouseAttribute", Script_SetHouseAttribute);	                // Sets a house's attributes
    lua_register(L, "GetHouseAttribute", Script_GetHouseAttribute);	                // Gets a house's attributes

    lua_register(L, "BuildUnit", Script_BuildUnit);	                                // Builds a unit in a given factory / barracks / naval yard / etc
    lua_register(L, "CreateVehicle", Script_CreateVehicle);                         // Creates a unit out of thin air
    lua_register(L, "CreateInfantry", Script_CreateInfantry);                       // Creates a unit out of thin air
    lua_register(L, "CreateAircraft", Script_CreateAircraft);                       // Creates a unit out of thin air
    lua_register(L, "CreateVessel", Script_CreateVessel);                           // Creates a unit out of thin air

/**********************************************************************************************
* Object Orders Functions                                                                     *
*=============================================================================================*/

    lua_register(L, "OrderMove", Script_OrderMove);                                 // Assigns a movement order to the object
    lua_register(L, "OrderAttack", Script_OrderAttack);                             // Assigns an attack order to the object
    lua_register(L, "OrderHunt", Script_OrderHunt);                                 // Assigns a hunt order to the object
    lua_register(L, "OrderGuard", Script_OrderGuard);                               // Assigns a guard order to the object
    lua_register(L, "OrderEnter", Script_OrderEnter);                               // Assigns a enter order to the object
    lua_register(L, "OrderCapture", Script_OrderCapture);                           // Assigns a capture order to the object

/**********************************************************************************************
* Mission Utility Functions                                                                   *
*=============================================================================================*/

    lua_register(L, "SetBriefingText", Script_SetBriefingText);						// Sets the [text] on the mission briefing screen

/**********************************************************************************************
* Status / Location Utility Functions                                                         *
*=============================================================================================*/

    lua_register(L, "GiveCredits", Script_GiveCredits);                             // Give [credits] to [player] 
    lua_register(L, "GetCredits", Script_GetCredits);                               // Get [player]'s [credits]

    lua_register(L, "CountBuildings", Script_CountBuildings);	                    // Number of buildings of [type] for given [player]
    lua_register(L, "CountAircraft", Script_CountAircraft);	                        // Number of units of [type] for given [player]
    lua_register(L, "CountUnits", Script_CountUnits);	                            // Number of units of [type] for given [player]
    lua_register(L, "CountInfantry", Script_CountInfantry);	                        // Number of infantry of [type] for given [player]
    lua_register(L, "CountVessels", Script_CountVessels);	                        // Number of vessels of [type] for given [player]

    lua_register(L, "GetWaypointX", Script_GetWaypointX);	                        // Get Cell/X of waypoint
    lua_register(L, "GetWaypointY", Script_GetWaypointY);	                        // Get Cell/Y of waypoint

/**********************************************************************************************
* Special / Debug Functions                                                                   *
*=============================================================================================*/

    lua_register(L, "PrintConsole", Script_Console_Print);	                        // Print text to console 

/**********************************************************************************************
* Script Globals                                                                              *
*=============================================================================================*/

    lua_register(L, "ToggleFireSale", Script_ToggleFireSale);

    lua_pushnumber(L, PlayerPtr->ID);                                                // Local player (house) index
    lua_setglobal(L, "_localPlayer");


    return true;
}