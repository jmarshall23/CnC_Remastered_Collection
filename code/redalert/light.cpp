// light.cpp
//

#include "function.h"
#include "image.h"

void Light_t::PlaceLight(int screenx, int screeny) {
	isPending = false;
	COORDINATE lightCoord;

	CellClass::ConvertIsoCoordsToScreen(screenx, screeny);

	position = Map.Closest_Free_Spot(Map.Pixel_To_Coord(screenx, screeny));
}

bool Light_t::GetRenderPosition(int& x, int& y) {
	Map.Coord_To_Pixel(position, x, y);
	CellClass::ConvertCoordsToIsometric(x, y);
	
	return true;
}


LightManager lightManager;

LightManager::LightManager() {
	FreeAllLights();
}

void LightManager::Init(void) {
	lightEditorIcon = Image_LoadImage("ui/edwin/lightbulb.png");
	pointLightAttenImage0 = Image_LoadImage("ui/lights/light_point_atten.png");

	hdrLightBufferTexture = Image_CreateBlankImage("hdrRenderTexture", ScreenWidth, ScreenHeight, true);
	hdrRenderTexture = new RenderTexture(hdrLightBufferTexture, NULL);
	hdrRenderTexture->InitRenderTexture();
}

void LightManager::RenderLights(void) {
	//if(Debug_Map) {
	//	for (int i = 0; i < MAX_WORLD_LIGHTS; i++) {
	//		if (!lights[i].active) {
	//			continue;
	//		}
	//
	//		if (!lights[i].isPending) {
	//			int screenx, screeny;
	//			lights[i].GetRenderPosition(screenx, screeny);
	//			GL_RenderImage(lightEditorIcon, screenx, screeny, 30, 30);
	//		}
	//	}
	//}
	//
	//GL_SetRenderTexture(hdrRenderTexture);
	//
	//for (int i = 0; i < MAX_WORLD_LIGHTS; i++) {
	//	if (!lights[i].active) {
	//		continue;
	//	}
	//
	//	if (!lights[i].isPending) {
	//		int screenx, screeny;
	//		lights[i].GetRenderPosition(screenx, screeny);
	//		int halfRadius = lights[i].radius / 2;
	//		GL_RenderImage(pointLightAttenImage0, screenx - halfRadius, ScreenHeight - (screeny + halfRadius), lights[i].radius, lights[i].radius);
	//	}
	//}
	//
	//GL_SetRenderTexture(NULL);
	//
	//GL_EnableBlend(GL_BLEND_ADD);
	//	int renderTargetScale = ZoomLevel;
	//	if (Debug_Map) {
	//		renderTargetScale = 0;
	//	}
	//	double scale = ((((float)ScreenWidth / (float)ScreenHeight) * (float)renderTargetScale) * 0.05) + 1;
	//
	//	GL_RenderImage(hdrLightBufferTexture, 0, 0, ScreenWidth * scale, ScreenHeight * scale, 0, 0, true, true);
	//GL_EnableBlend(GL_BLEND_NONE);
}

void LightManager::FreeAllLights(void) {
	memset(lights, 0, sizeof(lights));
}

Light_t* LightManager::AllocateLight(void) {
	for(int i = 0; i < MAX_WORLD_LIGHTS; i++) {
		if(!lights[i].active) {
			lights[i].active = true;
			lights[i].lightNum = i;
			return &lights[i];
		}
	}

	assert(!"Too many lights in the scene!");
	return NULL;
}

Light_t *LightManager::PlaceLight(char *name, int x, int y, float r, float g, float b, float radius, LightType_t type) {
	Light_t* light = AllocateLight();

	if(!light) {
		return NULL;
	}

	if(name == NULL) {
		sprintf(light->name, "light%d", light->lightNum);
	}
	else {
		strcpy(light->name, name);
	}
	light->position = XYP_Coord(x, y);
	light->color.x = r;
	light->color.y = g;
	light->color.z = b;
	light->radius = radius;
	light->type = type;	

	return light;
}

void LightManager::FreeLight(Light_t* light) {
	light->active = false;
}

void LightManager::Write_Scenerio_Lights(CCINIClass* ini) {
	ini->Clear("Lights");

	// Write out the lights.
	for (int i = 0; i < MAX_WORLD_LIGHTS; i++) {
		if (lights[i].active == false) {
			continue;
		}

		char buffer[2048];
		char name[256];
		sprintf(buffer, "%s,%d,%f,%f,%f,%f,%d", lights[i].name, lights[i].position, lights[i].color.x, lights[i].color.y, lights[i].color.z, lights[i].radius, lights[i].type);
		sprintf(name, "%d", i);
		ini->Put_String("Lights",name,buffer);
	}
}

void LightManager::Read_Scenerio_Lights(CCINIClass* ini) {
	char buf[2048];

	int len = ini->Entry_Count("Lights");
	for (int index = 0; index < len; index++) {
		char const* entry = ini->Get_Entry("Lights", index);

		/*
		**	Get a lighting entry.
		*/
		ini->Get_String("Lights", entry, NULL, buf, sizeof(buf));
		Light_t* light = AllocateLight();
		strcpy(light->name, strtok(buf, ","));
		light->position = atoi(strtok(NULL, ","));
		light->color.x = atof(strtok(NULL, ","));
		light->color.y = atof(strtok(NULL, ","));
		light->color.z = atof(strtok(NULL, ","));
		light->radius = atof(strtok(NULL, ","));
		light->type = (LightType_t)atoi(strtok(NULL, ","));
	}
}