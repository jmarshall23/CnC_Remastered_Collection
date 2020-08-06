// light.h
//

#include <vector>
#include "vectormath.h"

class CCINIClass;
class RenderTexture;
struct Image_t;

#define MAX_WORLD_LIGHTS			6556

enum LightType_t {
	LIGHT_AMBIENT = 0,
	LIGHT_POINT,
	LIGHT_SPOT
};

class Light_t {
	friend class LightManager;
public:	
	int GetLightNum(void) { return lightNum; }
	void PlaceLight(int screenx, int screeny);
	bool GetRenderPosition(int& x, int& y);

	char name[512];
	unsigned long position;
	float3 color;
	float radius;
	bool isPending;
	LightType_t type;
protected:
	int lightNum;
	bool active;
};

class LightManager {
public:
								LightManager();

	void						Init(void);

	Light_t*					AllocateLight(void);
	Light_t*					PlaceLight(char *name, int x, int y, float r, float g, float b, float radius, LightType_t type);

	void						FreeLight(Light_t* light);
	void						FreeAllLights(void);

	void						Read_Scenerio_Lights(CCINIClass* ini);
	void						Write_Scenerio_Lights(CCINIClass* ini);
public:
	Image_t*					GetLightEditorIcon(void) { return lightEditorIcon; }
	void						RenderLights(void);
private:
	Light_t						lights[MAX_WORLD_LIGHTS];
	int							numLights;
	Image_t*					lightEditorIcon;
	Image_t*					pointLightAttenImage0;

	Image_t*					hdrLightBufferTexture;
	RenderTexture*				hdrRenderTexture;
};

extern LightManager lightManager;