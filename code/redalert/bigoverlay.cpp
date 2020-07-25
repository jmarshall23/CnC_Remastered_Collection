// bigoverlay.cpp
//

#include "function.h"
#include "bigoverlay.h"
#include "image.h"
#include <string>
#include <imgui.h>

BigOverlayManager bigOverlayManager;

/*
==================
BigOverlay::BigOverlay
==================
*/
BigOverlay::BigOverlay() {
	image = NULL;
	numOverlayedCells = 0;
	enabled = false;
}

/*
======================
BigOverlay::Place
======================
*/
void BigOverlay::Place(int screenx, int screeny) {
	COORDINATE lightCoord;

	Map.FlagBigOverlayCells(this, screenx, screeny);

	CellClass::ConvertIsoCoordsToScreen(screenx, screeny);

	position = Map.Pixel_To_Coord(screenx, screeny);
	enabled = true;	
}

/*
======================
BigOverlay::GetRenderPosition
======================
*/
bool BigOverlay::GetRenderPosition(int& x, int& y) {
	Map.Coord_To_Pixel(position, x, y);
	CellClass::ConvertCoordsToIsometric(x, y);

	return true;
}

/*
======================
BigOverlay::draw_it
======================
*/
void BigOverlay::draw_it(void) {
	CELL cell = Coord_Cell(position);
	CellClass* cellptr = &(Map)[cell];

	if (!(cellptr->Is_Mapped(PlayerPtr) && cellptr->Is_Visible(PlayerPtr)))
		return;

	int screenx, screeny;
	GetRenderPosition(screenx, screeny);
	GL_RenderImage(image, screenx, screeny, image->width, image->height);
}

/*
====================
BigOverlayManager::BigOverlayManager
====================
*/
BigOverlayManager::BigOverlayManager() {
	Reset();
}

/*
====================
BigOverlayManager::Init
====================
*/
void BigOverlayManager::Init(void) {
	for(int i = 1; i <= 22; i++)
	{
		char temp[512];
		sprintf(temp, "BIGOVERLAY_WCLIFF%d", i);
		RegisterBigOverlay(THEATER_SNOW, temp);
	}
}

/*
====================
BigOverlayManager::RegisterBigOverlay
====================
*/
void BigOverlayManager::RegisterBigOverlay(int theater, const char* name) {
	const char* hdname = Tileset_FindHDTexture(theater, name, 0, 0);
	char filename[2048];
	sprintf(filename, "DATA/ART/TEXTURES/SRGB/RED_ALERT/TILESETS/%s/%s", Theaters[theater].Name, hdname);

	BigOverlayType_t type;
	type.image = Image_LoadImage(filename);

	if (type.image == NULL)
		return;

	bigOverlayTypes.push_back(type);
}

/*
====================
BigOverlayManager::BigOverlayManager
====================
*/
void BigOverlayManager::Reset(void) {
	memset(bigOverlays, 0, sizeof(bigOverlays));
}

/*
====================
BigOverlayManager::Render
====================
*/
void BigOverlayManager::Render(void) {
	for(int i = 0; i < MAX_WORLD_BIGOVERLAYS; i++) {
		if (!bigOverlays[i].IsEnabled())
			continue;

		bigOverlays[i].draw_it();
	}
}

/*
====================
BigOverlayManager::AdjustTile
====================
*/
void BigOverlayManager::AdjustTile(BigOverlay* bigOverlay, int direction) {
	int index = bigOverlay->GetIndex() + direction;
	if(index < 0)
	{
		index = bigOverlayTypes.size() - 1;
	}
	else if(index > bigOverlayTypes.size() - 1) {
		index = 0;
	}
	Image_t* image = bigOverlayTypes[index].image;
	bigOverlay->SetImage(index, image);
}

/*
====================
BigOverlayManager::BigOverlayManager
====================
*/
BigOverlay* BigOverlayManager::AllocateBigOverlay(void) {
	for (int i = 0; i < MAX_WORLD_BIGOVERLAYS; i++) {
		if (!bigOverlays[i].IsEnabled()) {
			bigOverlays[i].SetEnabled(true);
			bigOverlays[i].SetOverlayNum(i);
			return &bigOverlays[i];
		}
	}

	assert(!"Too many big overlays in the scene!");
	return NULL;
}

/*
====================
BigOverlayManager::Editor_PlaceNewBigOverlay
====================
*/
BigOverlay* BigOverlayManager::Editor_PlaceNewBigOverlay(void) {
	BigOverlay* big = NULL;
	bool processing = true;

	editor_selectedbigoverlay = 0;

	if(bigOverlayTypes.size() == 0) {
		assert(!"bigOverlayTypes size == 0");
	}

	while (processing)
	{
		g_globalKeyNumType = KN_NONE;
		g_globalKeyFlags = 0;
		UserInput.Process_Input();

		Map.Render();

		bool toolActive;
		ImGuiStyle& style = ImGui::GetStyle();
		style.FramePadding = ImVec2(0, 0);
		ImGui::SetNextWindowSize(ImVec2(480, 220));
		ImGui::SetNextWindowPos(ImVec2((ScreenWidth / 2) - 240, (ScreenHeight / 2) - 110));
		ImGui::Begin("Place Big Overlay", &toolActive, ImGuiWindowFlags_AlwaysAutoResize /*| ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground*/);
			Image_t* image = bigOverlayTypes[editor_selectedbigoverlay].image;
			ImVec2 overlayImageSize(image->width, image->height);
			ImGui::Text("Preview");
			ImGui::Image((ImTextureID)image->GetImageForHouse(0, 0, 0), overlayImageSize);
			if(ImGui::Button("Prev")) {
				editor_selectedbigoverlay--;
				if(editor_selectedbigoverlay < 0) {
					editor_selectedbigoverlay = bigOverlayTypes.size() - 1;
				}
			}
			ImGui::SameLine();
			if(ImGui::Button("Next")) {
				editor_selectedbigoverlay++;
				if(editor_selectedbigoverlay > bigOverlayTypes.size() - 1) {
					editor_selectedbigoverlay = 0;
				}
			}
			if(ImGui::Button("Cancel")) {
				processing = false;
			}
			ImGui::SameLine();
			if(ImGui::Button("Ok")) {
				big = AllocateBigOverlay();
				big->SetImage(editor_selectedbigoverlay, image);
				processing = false;
			}
		ImGui::End();

		Call_Back();
	}

	return big;
}

/*
====================
BigOverlayManager::Write_Scenerio_Lights
====================
*/
void BigOverlayManager::Write_Scenerio(CCINIClass* ini) {
	std::vector<COORDINATE> coord_table;
	ini->Clear("BigOverlayCoords");
	for (int i = 0; i < MAX_WORLD_BIGOVERLAYS; i++) {
		if (bigOverlays[i].IsEnabled() == false) {
			continue;
		}

		bigOverlays[i].startOverlayCoord = coord_table.size();
		for (int d = 0; d < bigOverlays[i].numOverlayedCells; d++)
		{
			coord_table.push_back(bigOverlays[i].overlayedCells[d]);
		}
	}

	for (int i = 0; i < coord_table.size(); i++) {
		char buffer[2048*12];
		sprintf(buffer, "%d", coord_table[i]);

		char name[256];
		sprintf(name, "%d", i);
		ini->Put_String("BigOverlayCoords", name, buffer);
	}

	ini->Clear("BigOverlay");

	// Write out the lights.
	for (int i = 0; i < MAX_WORLD_BIGOVERLAYS; i++) {
		if (bigOverlays[i].IsEnabled() == false) {
			continue;
		}

		//char buffer[2048*12];
		//sprintf(buffer, "%d,%d,%d", bigOverlays[i].GetPosition(), bigOverlays[i].GetIndex(), bigOverlays[i].numOverlayedCells);

		std::string buffer;
		buffer += std::to_string(bigOverlays[i].GetPosition());
		buffer += ",";
		buffer += std::to_string(bigOverlays[i].GetIndex());
		buffer += ",";
		buffer += std::to_string(bigOverlays[i].startOverlayCoord);
		buffer += ",";
		buffer += std::to_string(bigOverlays[i].numOverlayedCells);

		char name[256];		
		sprintf(name, "%d", i);
		ini->Put_String("BigOverlay", name, buffer.c_str());
	}
}

/*
====================
BigOverlayManager::Read_Scenerio
====================
*/
void BigOverlayManager::Read_Scenerio(CCINIClass* ini) {
	char buf[2048];
	std::vector<COORDINATE> coord_table;

	Reset();

	{
		int len = ini->Entry_Count("BigOverlayCoords");
		for (int index = 0; index < len; index++) {
			char const* entry = ini->Get_Entry("BigOverlayCoords", index);
			ini->Get_String("BigOverlayCoords", entry, NULL, buf, sizeof(buf));

			coord_table.push_back(atoi(buf));
		}
	}

	int len = ini->Entry_Count("BigOverlay");
	for (int index = 0; index < len; index++) {
		char const* entry = ini->Get_Entry("BigOverlay", index);

		/*
		**	Get a lighting entry.
		*/
		ini->Get_String("BigOverlay", entry, NULL, buf, sizeof(buf));
		BigOverlay* bigOverlay = AllocateBigOverlay();
		bigOverlay->SetPosition(atoi(strtok(buf, ",")));

		int in = atoi(strtok(NULL, ","));
		Image_t* image = bigOverlayTypes[in].image;
		bigOverlay->SetImage(in, image);	

		bigOverlay->startOverlayCoord = atoi(strtok(NULL, ","));
		bigOverlay->numOverlayedCells = atoi(strtok(NULL, ","));
		for (int d = 0; d < bigOverlay->numOverlayedCells; d++) {
			bigOverlay->overlayedCells[d] = coord_table[bigOverlay->startOverlayCoord + d];
			CellClass* cellptr = &(Map)[bigOverlay->overlayedCells[d]];
			cellptr->bigOverlay = bigOverlay;
		}
	}
}