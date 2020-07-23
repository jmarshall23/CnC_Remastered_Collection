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

/* $Header: /CounterStrike/WINSTUB.CPP 3     3/13/97 2:06p Steve_tall $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : WINSTUB.CPP                                                  *
 *                                                                                             *
 *                   Programmer : Steve Tall                                                   *
 *                                                                                             *
 *                   Start Date : 10/04/95                                                     *
 *                                                                                             *
 *                  Last Update : October 4th 1995 [ST]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Overview:                                                                                   *
 *   This file contains stubs for undefined externals when linked under Watcom for Win 95      *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 *                                                                                             *
 * Functions:                                                                                  *
 *   Assert_Failure -- display the line and source file where a failed assert occurred         *
 *   Check_For_Focus_Loss -- check for the end of the focus loss                               *
 *   Create_Main_Window -- opens the MainWindow for C&C                                        *
 *   Focus_Loss -- this function is called when a library function detects focus loss          *
 *   Memory_Error_Handler -- Handle a possibly fatal failure to allocate memory                *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include    "imgui.h"
#include	"examples/imgui_impl_sdl.h"
#include	"function.h"
#include	"il/il.h"
#include	"il/ilu.h"
#include	"image.h"
#include    "texcache.h"
#include    "vqamovie.h"

#ifdef WINSOCK_IPX
#include "WSProto.h"
#else	//WINSOCK_IPX
#include "tcpip.h"
#include "ipx95.h"
#endif	//WINSOCK_IPX

#include <SDL.h>
#include <SDL_syswm.h>
#include <gl/glew.h>

#include <imgui.h>
#include <examples/imgui_impl_opengl3.h>
#include <string>
#include <algorithm>

#include <vector>
#include "HOUSECOLOR.H"
#include "IMGUTIL.H"

GLuint backbuffer_texture = -1;
//byte* backbuffer_data;
unsigned char* backbuffer_data_raw;
byte backbuffer_palette[768];
HWND MainWindow;
SurfaceMonitorClass AllSurfaces;
SDL_Window* game_window;
SDL_GLContext game_context;
int OverlappedVideoBlits = 0;
float animFrameNum = 0;

bool renderConsole = false;

void (*Misc_Focus_Loss_Function)(void) = nullptr;
void (*Misc_Focus_Restore_Function)(void) = nullptr;

void (*Imgui_Dialog_Function)(void) = nullptr;

bool show_oldframebuffer = true;
void Show_OldFrameBuffer(bool show) {
	show_oldframebuffer = show;
	memset(backbuffer_data_raw, 0, ScreenWidth * ScreenHeight * 4);
}

Image_t* overlay_image = NULL;

void Sys_SetOverlayImage(Image_t* image) {
	overlay_image = image;
}

std::vector<Image_t *> loaded_images;

unsigned char* Draw_Dropsample(const unsigned char* in, int inwidth, int inheight, int outwidth, int outheight);

static char* Image_GetGeneratedName(Image_t* image, const char* name, int houseid, int animid, int frameId, int64_t hash) {
	static char generatedFileName[1024];
	sprintf(generatedFileName, "%s.%d.%d.%d.%d.tga", name, frameId, houseid, animid, hash);
	return &generatedFileName[0];
}

static bool Image_LoadFromCache(Image_t* image, const char* name, int houseid, int animid, int frameId, int64_t hash) {
	static byte* Data = NULL;
	if (Data == NULL) {
		Data = new byte[8192 * 8192 * 4];
	}

	int Width, Height, Bpp;
	if (!texCache.FindCachedImage(hash, Width, Height, Bpp, Data)) {
		return false;
	}

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	if (Bpp == 4) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_BGRA, GL_UNSIGNED_BYTE, Data);
	}
	else if (Bpp == 3) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Data);
	}
	else if (Bpp == 1) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_R, GL_UNSIGNED_BYTE, Data);
	}
	else {
		assert(!"Unknown BPP in image!");
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	strcpy(image->name, name);

	if (houseid == -1)
		image->HouseImages[0].image[animid][frameId] = texture;
	else
		image->HouseImages[houseid].image[animid][frameId] = texture;
	//image->namehash = hash;
	image->renderwidth[animid] = image->width = Width;
	image->renderheight[animid] = image->height = Height;
	//image->buffer[houseid][animid] = new unsigned char[Width * Height * Bpp];
	//memcpy(image->buffer[houseid][animid], Data, Width * Height * Bpp);

	return true;
}

static bool Image_loadHDImage(Image_t *image, const char* name, int houseid, int animid, int frameId, int64_t hash) {	
	// Don't load the image if its already loaded.
	if (houseid == -1) {
		if (image->HouseImages[0].image[animid][frameId] != 0) {
			return true;
		}
	}
	else {
		if (image->HouseImages[houseid].image[animid][frameId] != 0) {
			return true;
		}
	}

	const char* generatedName = Image_GetGeneratedName(image, name, houseid, animid, frameId, hash);
	hash = generateHashValue(generatedName, strlen(generatedName));

	// Check to see if we can load a generated image first.
	// We only generate images that need a houseid
	{
		if (Image_LoadFromCache(image, generatedName, houseid, animid, frameId, hash)) {
			return true;
		}
	}

	ILuint ImageName;
	ilGenImages(1, &ImageName);
	ilBindImage(ImageName); 

	if (!ilLoadImage(name)) {
		ilDeleteImages(1, &ImageName);
		return false;
	}

	if (strstr(name, ".TGA") || strstr(name, ".tga")) {
		iluFlipImage();
	}

	ILuint Width, Height, Bpp;
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

	Width = ilGetInteger(IL_IMAGE_WIDTH);
	Height = ilGetInteger(IL_IMAGE_HEIGHT);
	Bpp = ilGetInteger(IL_IMAGE_BPP);	
	ILubyte* Data = ilGetData();

	if (Bpp == 4 && houseid != -1) {
		//Buffer_Enable_HD_Texture(true);
		//Data = Draw_Dropsample(Data, Width, Height, Width / 2, Height / 2);
		//Buffer_Enable_HD_Texture(false);
		//Width = Width / 2;
		//Height = Height / 2;
		for (int i = 0; i < Width * Height; i++) {			
			RenderTeamPalette(&Data[i * 4], *houseColors[houseid]);
		}
	}
	else if (Bpp == 4) {
		for (int i = 0; i < Width * Height; i++) {
			byte b = Data[(i * 4) + 2];
			Data[(i * 4) + 2] = Data[(i * 4) + 0];
			Data[(i * 4) + 0] = b;

			if (Data[(i * 4) + 2] == 0 && Data[(i * 4) + 1] == 0 && Data[(i * 4) + 0] == 117) {
				Data[(i * 4) + 3] = 0;
			}

			if (Data[(i * 4) + 2] == 117 && Data[(i * 4) + 1] == 0 && Data[(i * 4) + 0]) {
				Data[(i * 4) + 3] = 0;
			}
		}
	}
	else if (Bpp == 3) {
		for (int i = 0; i < Width * Height; i++) {
			byte b = Data[(i * 3) + 2];
			Data[(i * 3) + 2] = Data[(i * 4) + 0];
			Data[(i * 3) + 0] = b;

			if (Data[(i * 3) + 2] == 0 && Data[(i * 3) + 1] == 0 && Data[(i * 3) + 0] == 117) {
				Data[(i * 3) + 3] = 0;
			}
		}
	}

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	if (Bpp == 4) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_BGRA, GL_UNSIGNED_BYTE, Data);
	}
	else if (Bpp == 3) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Data);
	}
	else if (Bpp == 1) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_R, GL_UNSIGNED_BYTE, Data);
	}
	else {
		assert(!"Unknown BPP in image!");
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	
	strcpy(image->name, name);

	if(houseid == -1)
		image->HouseImages[0].image[animid][frameId] = texture;
	else
		image->HouseImages[houseid].image[animid][frameId] = texture;
	//image->namehash = hash;
	image->renderwidth[animid] = image->width = Width;
	image->renderheight[animid] = image->height = Height;
	//image->buffer[houseid][animid] = new unsigned char[Width * Height * Bpp];
	//memcpy(image->buffer[houseid][animid], Data, Width * Height * Bpp);

	if (Bpp == 4) {
		texCache.AddImage(generatedName, Width, Height, Bpp, Data);
		Console_Printf("Writing %s to cache %s\n", image->name, name);
		Sleep(0);
	}

	ilDeleteImages(1, &ImageName);

	return true;
}

Image_t* Image_CreateBlankImage(const char* name, int width, int height, bool hasAlpha) {
	int64_t hash = generateHashValue(name, strlen(name));

	int image_table_size = loaded_images.size();
	if (image_table_size > 0)
	{
		Image_t** image_table = &loaded_images[0];

		// Check to see if the image is already loaded.
		for (int i = 0; i < image_table_size; i++) {
			if (image_table[i]->namehash == hash) {
				return image_table[i];
			}
		}
	}
	Image_t* image = new Image_t();


	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	if (hasAlpha)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	strcpy(image->name, name);

	image->HouseImages[0].image[0][0] = texture;
	image->namehash = hash;
	image->renderwidth[0] = image->width = width;
	image->renderheight[0] = image->height = height;

	loaded_images.push_back(image);
	return image;
}

void Image_UploadRaw(Image_t* image, uint8_t* data, bool paletteRebuild, uint8_t* palette, bool hasAlpha) {
	if (image->ScratchBuffer == NULL && paletteRebuild) {
		image->ScratchBuffer = new uint8_t[image->width * image->height * 4];
	}

	if (paletteRebuild) {
		for (int i = 0; i < image->width * image->height; i++) {
			unsigned char c = data[i];
			
			unsigned char r = palette[(c * 3) + 0];
			unsigned char g = palette[(c * 3) + 1];
			unsigned char b = palette[(c * 3) + 2];

			image->ScratchBuffer[(i * 4) + 0] = r;
			image->ScratchBuffer[(i * 4) + 1] = g;
			image->ScratchBuffer[(i * 4) + 2] = b;
			image->ScratchBuffer[(i * 4) + 3] = 255;
		}
	}

	glBindTexture(GL_TEXTURE_2D, image->HouseImages[0].image[0][0]);

	if (hasAlpha)
	{
		if (image->ScratchBuffer)
		{
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image->width, image->height, GL_RGBA, GL_UNSIGNED_BYTE, image->ScratchBuffer);
		}
		else
		{
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image->width, image->height, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
	}
	else
	{
		if (image->ScratchBuffer)
		{
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image->width, image->height, GL_RGB, GL_UNSIGNED_BYTE, image->ScratchBuffer);
		}
		else
		{
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image->width, image->height, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
	}
}

Image_t* Image_LoadImage(const char* name, bool loadAnims, bool loadHouseColor) {
	int64_t hash = generateHashValue(name, strlen(name));

	int image_table_size = loaded_images.size();
	if (image_table_size > 0)
	{
		Image_t** image_table = &loaded_images[0];

		// Check to see if the image is already loaded.
		for (int i = 0; i < image_table_size; i++) {
			if (image_table[i]->namehash == hash) {
				return image_table[i];
			}
		}
	}
	Image_t* image = new Image_t();

	if (!loadAnims) {
		unsigned char remap[3] = { 0, 0, 0 };
		if (!Image_loadHDImage(image, name, -1, 0, 0, hash)) {
			delete image;
			return NULL;
		}
	}
	else {
		byte* palette = (byte *)CCPalette.Get_Data();
		for (int i = 0; i < MAX_MPLAYER_COLORS; i++) {
			if (!Image_loadHDImage(image, name, i, 0, 0, hash)) {
				delete image;
				return NULL;
			}
		}
	}
	image->namehash = hash;
	loaded_images.push_back(image);
	return image;
}

Image_t *Find_Image(const char* name) {
	int64_t hash = generateHashValue(name, strlen(name));

	int image_table_size = loaded_images.size();
	if (image_table_size > 0)
	{
		Image_t** image_table = &loaded_images[0];

		// Check to see if the image is already loaded.
		for (int i = 0; i < image_table_size; i++) {
			if (image_table[i]->namehash == hash) {
				return image_table[i];
			}
		}
	}

	return NULL;
}

Image_t* Image_CreateImageFrom8Bit(const char* name, int Width, int Height, unsigned char *data, unsigned char* remap, unsigned char* ccpalete) {
	int64_t hash = generateHashValue(name, strlen(name));

	int image_table_size = loaded_images.size();
	if (image_table_size > 0)
	{
		Image_t** image_table = &loaded_images[0];

		// Check to see if the image is already loaded.
		for (int i = 0; i < image_table_size; i++) {
			if (image_table[i]->namehash == hash) {
				return image_table[i];
			}
		}
	}

	if (ccpalete == NULL)
		ccpalete = (unsigned char*)CCPalette.Get_Data();


	Image_t* image = new Image_t();
	unsigned char *buffer = new unsigned char[Width * Height * 4];

	for (int i = 0; i < Width * Height; i++) {
		unsigned char c = data[i];
		if (remap) {
			c = remap[c];
		}

		unsigned char r = ccpalete[(c * 3) + 0] << 2;
		unsigned char g = ccpalete[(c * 3) + 1] << 2;
		unsigned char b = ccpalete[(c * 3) + 2] << 2;
		unsigned char a = 0;

		if ((r == 84 && g == 252 && b == 84) || (r == 0 && g == 168 && b == 0)) {
			r = 0;
			g = 0;
			b = 0;			
			a = 128;
		}
		else if (c != 0) {
			a = 255;
		}


		buffer[(i * 4) + 0] = r;
		buffer[(i * 4) + 1] = g;
		buffer[(i * 4) + 2] = b;
		buffer[(i * 4) + 3] = a;
	}


	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	
	strcpy(image->name, name);
	image->HouseImages[0].image[0][0] = texture;
	image->numAnimFrames = -1;
	image->namehash = hash;
	image->renderwidth[0] = image->width = Width;
	image->renderheight[0] = image->height = Height;
	loaded_images.push_back(image);
	delete buffer;
	return image;
}

void Image_Add8BitImage(Image_t *image, int HouseId, int ShapeID, int Width, int Height, unsigned char* data, unsigned char* remap, unsigned char *palette) {	
	unsigned char* buffer = new unsigned char[Width * Height * 4];
	unsigned char* ccpalete = (unsigned char*)CCPalette.Get_Data();
	if (palette) {
		ccpalete = palette;
	}

	if (image->width != Width || image->height != Height) {
		assert(!"Image_Add8BitImage: Invalid new dimensions!");
	}

	for (int i = 0; i < Width * Height; i++) {
		unsigned char c = data[i];
		if (remap) {
			c = remap[c];
		}

		unsigned char r = ccpalete[(c * 3) + 0] << 2;
		unsigned char g = ccpalete[(c * 3) + 1] << 2;
		unsigned char b = ccpalete[(c * 3) + 2] << 2;
		unsigned char a = 0;

		if ((r == 84 && g == 252 && b == 84) || (r == 0 && g == 168 && b == 0)) {
			r = 0;
			g = 0;
			b = 0;
			a = 128;
		}
		else if (c != 0) {
			a = 255;
		}


		buffer[(i * 4) + 0] = r;
		buffer[(i * 4) + 1] = g;
		buffer[(i * 4) + 2] = b;
		buffer[(i * 4) + 3] = a;
	}


	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	image->HouseImages[HouseId].image[ShapeID][0] = texture;
	image->numAnimFrames = -1;
	image->renderwidth[ShapeID] = image->width = Width;
	image->renderheight[ShapeID] = image->height = Height;

	delete buffer;
}

bool Image_Add32BitImage(const char *name, Image_t* image, int HouseId, int ShapeID, int frameId) {
	return Image_loadHDImage(image, name, HouseId, ShapeID, frameId, image->namehash);	
}

BOOL Set_Video_Mode(HWND hwnd, int w, int h, int bits_per_pixel) {
	// Todo implement resoluton scaling.
	return true; // Always return true;
}

unsigned int Get_Free_Video_Memory(void) {
	return 64 * 1024 * 1024;
}

unsigned int Get_Video_Hardware_Capabilities() {
	return 0;
}

void Reset_Video_Mode(void) {

}

void Set_DD_Palette(void* palette, bool raShift)
{
	char* palette_get = (char*)palette; //CCPalette.Get_Data();
	for (int j = 0; j < 768; j++) {
		if (raShift) {
			backbuffer_palette[j] = palette_get[j] << 2;
		}
		else {
			backbuffer_palette[j] = palette_get[j];
		}
	}
}

void ImGui_NewFrame(void) {
	ImGuiIO& io = ImGui::GetIO();
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	io.DisplaySize = ImVec2(ScreenWidth, ScreenHeight);
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame(show_oldframebuffer);

	ImGui_ImplSDL2_NewFrame(game_window);	
}

void Device_Present(void) {	
	if (overlay_image) {
		GL_RenderImage(overlay_image, 0, 0, ScreenWidth, ScreenHeight);
	}

	// Rasterize the palette.
	//for (int i = 0; i < ScreenWidth * ScreenHeight; i++) {
	//	backbuffer_data[(i * 4) + 0] = backbuffer_palette[(backbuffer_data_raw[i] * 3) + 0];
	//	backbuffer_data[(i * 4) + 1] = backbuffer_palette[(backbuffer_data_raw[i] * 3) + 1];
	//	backbuffer_data[(i * 4) + 2] = backbuffer_palette[(backbuffer_data_raw[i] * 3) + 2];
	//	backbuffer_data[(i * 4) + 3] = 255;
	//}
	//glBindTexture(GL_TEXTURE_2D, backbuffer_texture);
	//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, ScreenWidth, ScreenHeight, GL_RGBA, GL_UNSIGNED_BYTE, backbuffer_data_raw);

	bool ScreenActive;
	ImGuiStyle& style = ImGui::GetStyle();
	style.FramePadding = ImVec2(0, 0);
	ImGui::SetNextWindowSize(ImVec2(ScreenWidth, ScreenHeight));
	ImGui::SetNextWindowPos(ImVec2(0, 0));
// jmarshall - I'm removing this because we don't render the legacy buffer anymore.
	//ImGui::Begin("RenderWindow", &ScreenActive, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMouseInputs);
	//	if (show_oldframebuffer)
	//	{
	//		ImVec2 desktopSize(ScreenWidth, ScreenHeight);
	//		ImGui::Image((ImTextureID)backbuffer_texture, desktopSize);
	//	}
	//
	//ImGui::End();
// jmarshall end

	if (Imgui_Dialog_Function) {	
		Map.Render();
		Imgui_Dialog_Function();			
	}

	if (renderConsole) {
		Console_Render();
	}

	WWMouse->RenderMouse();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	SDL_GL_SwapWindow(game_window);

	// Last thing we do is execute any console commands
	Console_Tick();

	ImGui_NewFrame();	
}


void output(short,short)
{}

unsigned long CCFocusMessage = WM_USER+50;	//Private message for receiving application focus
extern	void VQA_PauseAudio(void);
extern	void VQA_ResumeAudio(void);

//#include "WolDebug.h"


/***********************************************************************************************
 * Focus_Loss -- this function is called when a library function detects focus loss            *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    2/1/96 2:10PM ST : Created                                                               *
 *=============================================================================================*/

void Focus_Loss(void)
{
	Theme.Suspend();
	Stop_Primary_Sound_Buffer();
	if (WWMouse) WWMouse->Clear_Cursor_Clip();
}

void Focus_Restore(void)
{
	Restore_Cached_Icons();
	Map.Flag_To_Redraw(true);
	Start_Primary_Sound_Buffer(TRUE);
	if (WWMouse) WWMouse->Set_Cursor_Clip();
	VisiblePage.Clear();
	HiddenPage.Clear();
}



/***********************************************************************************************
 * Check_For_Focus_Loss -- check for the end of the focus loss                                 *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    2/2/96 10:49AM ST : Created                                                              *
 *=============================================================================================*/

void Check_For_Focus_Loss(void)
{
#if (0)//PG
	static BOOL	focus_last_time = 1;
			 MSG	msg;


	if ( !GameInFocus ) {
		Focus_Loss();
		while ( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE | PM_NOYIELD ) ) {
	  		if (!GetMessage( &msg, NULL, 0, 0 ) ) {
				return;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	if (!focus_last_time && GameInFocus) {

		VQA_PauseAudio();
		CountDownTimerClass cd;
		cd.Set(60*1);

		do {
			while (PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE )) {
	  			if (!GetMessage( &msg, NULL, 0, 0 ) ) {
					return;
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

		} while (cd.Time());
		VQA_ResumeAudio();
		PostMessage (MainWindow, CCFocusMessage, 0, 0);
//		AllSurfaces.Restore_Surfaces();
//		VisiblePage.Clear();
//		HiddenPage.Clear();
//		Map.Flag_To_Redraw(true);
	}

	focus_last_time = GameInFocus;
#endif
}

extern bool InMovie;

HANDLE DebugFile = INVALID_HANDLE_VALUE;

/***********************************************************************************************
 * WWDebugString -- sends a string to the debugger and echos it to disk                        *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    string                                                                            *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    10/28/96 12:48PM ST : Created                                                              *
 *=============================================================================================*/
void WWDebugString (char *string)
{
	Console_Printf(string);
}









/***********************************************************************************************
 * Create_Main_Window -- opens the MainWindow for C&C                                          *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    instance -- handle to program instance                                            *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    10/10/95 4:08PM ST : Created                                                             *
 *=============================================================================================*/

#define CC_ICON 1

#define WINDOW_NAME		"Red Alert"

void Create_Main_Window ( HANDLE instance , int command_show , int width , int height )

{	
	HWND        	hwnd ;

	ilInit();

	GL_SetColor(1, 1, 1);

	texCache.Init();

	SDL_Init(SDL_INIT_VIDEO);
	game_window = SDL_CreateWindow(WINDOW_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);	
	game_context = SDL_GL_CreateContext(game_window);
	SDL_GL_SetSwapInterval(0);

	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWindowWMInfo(game_window, &wmInfo);

	SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);

	hwnd = wmInfo.info.win.window;
	GameInFocus = TRUE;

	ShowWindow(hwnd, command_show );
	ShowCommand = command_show;
	UpdateWindow(hwnd);
	SetFocus(hwnd);
	MainWindow = hwnd;
	hInstance = instance;

	CCFocusMessage = RegisterWindowMessage("CC_GOT_FOCUS");

	//Audio_Focus_Loss_Function = &Focus_Loss;
	Misc_Focus_Loss_Function = &Focus_Loss;
	Misc_Focus_Restore_Function = &Focus_Restore;
	Gbuffer_Focus_Loss_Function = &Focus_Loss;

	glGenTextures(1, &backbuffer_texture);
	glBindTexture(GL_TEXTURE_2D, backbuffer_texture);
	// Give the image to OpenGL	
	backbuffer_data_raw = new byte[width * height * 4];
	memset(backbuffer_data_raw, 0, width * height * 4);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, backbuffer_data_raw);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glewInit();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForD3D(game_window);
	ImGui_ImplOpenGL3_Init();

	io.Fonts->AddFontFromFileTTF("fonts/Arial.ttf", 16.0f);

	ImGui_NewFrame();

	VQA_Init();

	lightManager.Init();
}


void Window_Dialog_Box(HANDLE  hinst, LPCTSTR  lpszTemplate, HWND  hwndOwner, DLGPROC  dlgprc)
{
#if (0)//PG
	MSG					msg;
	/*
	** Get rid of the Westwood mouse cursor because we are showing a
	** dialog box and we want to have the right windows cursor showing
	** for it.
	*/
	Hide_Mouse();
	ShowCursor(TRUE);

	/*
	** Pop up the dialog box and then run a standard message handler
	** until the dialog box is closed.
	*/

	DialogBox(hinst, lpszTemplate, hwndOwner, dlgprc);
	while (GetMessage(&msg, NULL, 0, 0) && !AllDone) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	/*
	** Restore the westwood mouse cursor and get rid of the windows one
	** because it is now time to restore back to the westwood way of
	** doing things.
	*/
	ShowCursor(FALSE);
	Show_Mouse();
#endif
}



typedef struct tColourList {

	char	Red;
	char	Green;
	char	Blue;
} ColourList;

ColourList	ColourLookup[9]={
	0,0,0,
	63,0,0,
	0,63,0,
	0,0,63,
	63,0,63,
	63,63,0,
	0,63,63,
	32,32,32,
	63,63,63
};




int	DebugColour=1;


extern "C" void Set_Palette_Register(int number, int red, int green, int blue);
//#pragma off (unreferenced)
void Colour_Debug (int call_number)
{
#if (0)//PG
	//#if 0
	//if (DebugColour==call_number || !call_number) {

		//if (call_number) {
		//	Wait_Vert_Blank();
		//}

		Set_Palette_Register (0,ColourLookup[call_number].Red ,
										ColourLookup[call_number].Green,
										ColourLookup[call_number].Blue);
	//}
	//#endif
#endif
}

//#pragma on (unreferenced)





BOOL Any_Locked (void)
{
	if (SeenBuff.Get_LockCount() ||
		HidPage.Get_LockCount()) {
		return (TRUE);
	} else {
		return(FALSE);
	}
}








//
// Miscellaneous stubs. Mainly for multi player stuff
//
//
//

//IPXAddressClass::IPXAddressClass(void) {
//	int i;
//	i++;
//}
//int IPXManagerClass::Num_Connections(void) { return (0); }
//int IPXManagerClass::Connection_ID( int ) { return (0); }
//IPXAddressClass * IPXManagerClass::Connection_Address( int ) { return ((IPXAddressClass*)0); }
//char * IPXManagerClass::Connection_Name( int ) { return ((char*)0); }
//int IPXAddressClass::Is_Broadcast() { return (0); }
//int IPXManagerClass::Send_Global_Message( void *, int, int, IPXAddressClass * ) { return (0); }
//int IPXManagerClass::Service() { return (0); }
//int IPXManagerClass::Get_Global_Message( void  *, int  *, IPXAddressClass  *, short unsigned  * ) { return (0); }
//int IPXAddressClass::operator ==( IPXAddressClass  & ) { return (0); }
//IPXManagerClass::IPXManagerClass( int, int, int, int, short unsigned, short unsigned ) {}
//IPXManagerClass::~IPXManagerClass() {
//	int i;
//	i++;
//	}
//int  IPXManagerClass::Delete_Connection( int ) { return (0); }
//IPXAddressClass::IPXAddressClass( char unsigned  *, char unsigned  * ) {}
//void  IPXManagerClass::Set_Socket( short unsigned ) {}
//int  IPXManagerClass::Is_IPX() { return (0); }
//int  IPXManagerClass::Init() { return (0); }
//void  IPXAddressClass::Get_Address( char unsigned  *, char unsigned  * ) {}
//void  IPXManagerClass::Set_Bridge( char unsigned  * ) {}
//int  IPXManagerClass::Global_Num_Send() { return (0); }
//void  IPXManagerClass::Set_Timing( long unsigned, long unsigned, long unsigned ) {}
//unsigned long IPXManagerClass::Global_Response_Time() { return (0); }
//int  IPXManagerClass::Create_Connection( int, char  *, IPXAddressClass  * ) { return (0); }
//int  IPXAddressClass::operator !=( IPXAddressClass  & ) { return (0); }
//int  IPXManagerClass::Send_Private_Message( void  *, int, int, int ) { return (0); }
//int  IPXManagerClass::Get_Private_Message( void  *, int  *, int  * ) { return (0); }
//int  IPXManagerClass::Connection_Index( int ) { return (0); }
//void  IPXManagerClass::Reset_Response_Time() {}
//long unsigned  IPXManagerClass::Response_Time() { return (0); }
//int  IPXManagerClass::Private_Num_Send( int ) { return (0); }

//void VQA_Init(VQAHandle *, long(*)(VQAHandle *vqa, long action,	void *buffer, long nbytes)) {}

//long VQA_Open(VQAHandle *, char const *, VQAConfig *)
//{
//	return (0);
//}

//void VQA_Close(VQAHandle *) {}

//long VQA_Play(VQAHandle *, long)
//{
//	return (0);
//}


unsigned char	*VQPalette;
long				VQNumBytes;
unsigned long	VQSlowpal;
bool				VQPaletteChange = false;


extern "C"{
	void __cdecl SetPalette(unsigned char *palette, long numbytes, unsigned long slowpal);
}



void Flag_To_Set_Palette(unsigned char *palette, long numbytes, unsigned long slowpal)
{
	VQPalette = palette;
	VQNumBytes = numbytes;
	VQSlowpal = slowpal;
	VQPaletteChange = true;
}



void Check_VQ_Palette_Set(void)
{
	if (VQPaletteChange) {
		SetPalette(VQPalette, VQNumBytes, VQSlowpal);
		VQPaletteChange = false;
	}
}





void __cdecl SetPalette(unsigned char *palette, long, unsigned long)
{
	for (int i=0 ; i<256*3 ; i++) {
		*(palette+i)&=63;
	}
	Increase_Palette_Luminance(palette , 15 , 15 , 15 ,63);

	if (PalettesRead) {
		memcpy (&PaletteInterpolationTable[0][0] , InterpolatedPalettes[PaletteCounter++] , 65536);
	}
	Set_Palette(palette);
}


void WWSDL_ProcessEvents(KeyNumType& key, int& flags, bool presentBuffer) {
	//UserInput.Process_Input(key, flags);
}

#ifndef NDEBUG
/***********************************************************************************************
 * Assert_Failure -- display the line and source file where a failed assert occurred           *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    line number in source file                                                        *
 *           name of source file                                                               *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    4/17/96 9:58AM ST : Created                                                              *
 *=============================================================================================*/

void Assert_Failure (char *expression, int line, char *file)
{
	char	assertbuf[256];
	char   timebuff[512];
	SYSTEMTIME	time;

	sprintf (assertbuf, "assert '%s' failed at line %d in module %s.\n", expression, line, file);

	if (!MonoClass::Is_Enabled()) MonoClass::Enable();

	Mono_Clear_Screen();
	Mono_Printf("%s", assertbuf);

	WWDebugString(assertbuf);

	GetLocalTime(&time);

	sprintf (timebuff, "%02d/%02d/%04d %02d:%02d:%02d - %s", time.wMonth, time.wDay, time.wYear,
																		time.wHour, time.wMinute, time.wSecond,
																		assertbuf);


	HMMIO handle = mmioOpen("ASSERT.TXT", NULL, MMIO_WRITE);
	if (!handle) {
		handle = mmioOpen("ASSERT.TXT", NULL, MMIO_CREATE | MMIO_WRITE);
		//mmioClose(handle, 0);
		//handle = mmioOpen("ASSERT.TXT", NULL, MMIO_WRITE);
	}

	if (handle) {

		mmioWrite(handle, timebuff, strlen(timebuff));
		mmioClose(handle, 0);
	}

	WWMessageBox().Process(assertbuf);

	Prog_End(assertbuf, false);
	Invalidate_Cached_Icons();
}
#endif







/***********************************************************************************************
 * Memory_Error_Handler -- Handle a possibly fatal failure to allocate memory                  *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    5/22/96 3:57PM ST : Created                                                              *
 *=============================================================================================*/
void Memory_Error_Handler(void)
{
	VisiblePage.Clear();
	CCPalette.Set();
	while (Get_Mouse_State()) {Show_Mouse();};
	WWMessageBox().Process(TEXT_MEMORY_ERROR, TEXT_ABORT, false);

	ReadyToQuit = 1;

	PostMessage(MainWindow, WM_DESTROY, 0, 0);
	do
	{
		Keyboard->Check();
	}while (ReadyToQuit == 1);

	ExitProcess(0);
}








GraphicBufferClass* Read_PCX_File(char* name, char* Palette, void *Buff, long Size);
void Load_Title_Screen(char *name, GraphicViewPortClass *video_page, unsigned char *palette) {
	char filename[512];
	sprintf(filename, "ui/%s", name);
	COM_SetExtension(filename, sizeof(filename), ".png");
	Image_t* image = Image_LoadImage(filename);

	GL_RenderImage(image, 0, 0, ScreenWidth, ScreenHeight);

	// We still have to do this for palette reasons. 
	{
		GraphicBufferClass* load_buffer;
		load_buffer = Read_PCX_File(name, (char*)palette, NULL, 0);
		delete load_buffer;
		//if (load_buffer) {
		//	load_buffer->Scale(*video_page, 0, 0, 0, 0, load_buffer->Get_Width(), load_buffer->Get_Height(), ScreenWidth, ScreenHeight, NULL);
		//	delete load_buffer;
		//}
	}
}



#include "filepcx.h"

/***************************************************************************
 * READ_PCX_FILE -- read a pcx file into a Graphic Buffer                  *
 *                                                                         *
 *	GraphicBufferClass* Read_PCX_File (char* name, char* palette ,void *Buff, long size );	*
 *  																								*
 *                                                                         *
 * INPUT: name is a NULL terminated string of the format [xxxx.pcx]        *
 *        palette is optional, if palette != NULL the the color palette of *
 *					 the pcx file will be place in the memory block pointed	   *
 *               by palette.																*
 *			 Buff is optional, if Buff == NULL a new memory Buffer		 		*
 *					 will be allocated, otherwise the file will be placed 		*
 *					 at location pointed by Buffer;										*
 *			Size is the size in bytes of the memory block pointed by Buff		*
 *				  is also optional;															*                                                                         *
 * OUTPUT: on success a pointer to a GraphicBufferClass containing the     *
 *         pcx file, NULL otherwise.                                       *
 *																									*
 * WARNINGS:                                                               *
 *         Appears to be a comment-free zone                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/03/1995 JRJ : Created.                                             *
 *   04/30/1996 ST : Tidied up and modified to use CCFileClass             *
 *=========================================================================*/

#define	POOL_SIZE 2048
#define	READ_CHAR()  *file_ptr++ ; \
							 if ( file_ptr	>= & pool [ POOL_SIZE ]	) { \
								 file_handle.Read (pool , POOL_SIZE ); \
								 file_ptr = pool ; \
							 }


GraphicBufferClass* Read_PCX_File(char* name, char* palette, void *Buff, long Size)
{
	unsigned					i, j;
	unsigned					rle;
	unsigned					color;
	unsigned					scan_pos;
	char						*file_ptr;
	int						width;
	int						height;
	char						*buffer;
	PCX_HEADER				header;
	RGB						*pal;
	char						pool [POOL_SIZE];
	GraphicBufferClass	*pic;

	CCFileClass file_handle(name);

	if (!file_handle.Is_Available()) return (NULL);

	file_handle.Open(READ);

	file_handle.Read (&header, sizeof (PCX_HEADER));

	if (header.id != 10 &&  header.version != 5 && header.pixelsize != 8 ) return NULL ;

	width = header.width - header.x + 1;
	height = header.height - header.y + 1;

	if (Buff) {
    	buffer = (char *)Buff;
    	i = Size / width;
    	height = MIN ((int)i - 1, height);
    	pic = new GraphicBufferClass(width, height, buffer, Size);
    	if ( !(pic && pic->Get_Buffer()) ) return NULL ;
	} else {
    	pic = new GraphicBufferClass(width, height, NULL, width*(height+4));
    	if ( !(pic && pic->Get_Buffer()) ) return NULL ;
	}

	buffer = (char *) pic->Get_Buffer();
	file_ptr = pool ;
	file_handle.Read (pool , POOL_SIZE);

	if ( header.byte_per_line != width ) {

		for ( scan_pos = j = 0 ; j < (unsigned) height ; j ++, scan_pos += width ) {
			for ( i = 0 ; i < (unsigned)width ; ) {
				rle = READ_CHAR ();
				if ( rle > 192 ) {
					rle -= 192 ;
					color =	READ_CHAR (); ;
					memset ( buffer + scan_pos + i , color , rle );
					i += rle;
				} else {
					*(buffer+scan_pos + i++ ) = (char)rle;
				}
			}
      	}

		if ( i == width ) rle = READ_CHAR ();
		if ( rle > 192 )  rle = READ_CHAR ();

	} else {

		for ( i = 0 ; i < (unsigned)width * height ; ) {
  			rle = READ_CHAR ();
  			rle &= 0xff;
  			if ( rle > 192 ) {
        		rle -= 192 ;
        		color = READ_CHAR ();
  				memset ( buffer + i , color , rle );
        		i += rle ;
     		} else {
				*(buffer + i++) = (char)rle;
			}
		}
	}

	if ( palette ) {
		file_handle.Seek (- (256 * (int)sizeof ( RGB )) , SEEK_END );
		file_handle.Read (palette , 256L * sizeof ( RGB ));

		pal = ( RGB * ) palette;
		for (i = 0 ; i < 256 ; i ++) {
			pal ->red	>>= 2;
			pal ->green	>>= 2;
			pal ->blue	>>= 2;
			pal ++ ;
		}
	}

	file_handle.Close();
	return pic;
}

/*
================
Sys_Milliseconds
================
*/
int Sys_Milliseconds(void) {
	int sys_curtime;
	static int sys_timeBase;
	static bool	initialized = false;

	if (!initialized) {
		sys_timeBase = timeGetTime();
		initialized = true;
	}
	sys_curtime = timeGetTime() - sys_timeBase;

	return sys_curtime;
}