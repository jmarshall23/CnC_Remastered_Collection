// briefing.cpp
//

#include "function.h"
#include "image.h"

void RenderBriefing(void) {
	Image_t* worldMapImage = Image_LoadImage("ui/briefing/generic_worldmap.png");
	bool exitBreifing = false;
	while (!exitBreifing) {
		// Check for any new SDL events.
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				//delete raw_image_buffer;		
				exitBreifing = true;
				break;
			}
		}

		UserInput.Process_Input();

		GL_RenderImage(worldMapImage, 0, 0, ScreenWidth, ScreenHeight);
		GL_DrawText(5, 32.0f, 255.0f, 255.0f, 255.0f, 20, 0, Scen.Description);
		GL_DrawText(5, 20.0f, 255.0f, 255.0f, 255.0f, 60, 30, Scen.BriefingText);
		GL_DrawText(5, 20, 255.0f, 255.0f, 255.0f, (ScreenWidth / 2) - 100, ScreenHeight - 40, "Press any key to continue");
		Device_Present();
	}
}