// NEWBLIT.CPP
//

#include <imgui.h>
#include "FUNCTION.H"
#include "Image.h"

extern byte backbuffer_palette[768];
extern uint8_t g_ColorXlat[16];

bool forceForgegroundRender = false;

void GL_ForceForegroundRender(bool force) {
	forceForgegroundRender = force;
}

void GL_SetClipRect(int x, int y, int width, int height) {
	ImVec2 mi(x, y);
	ImVec2 ma(x + width, y + height);
	ImGui::GetBackgroundDrawList()->PushClipRect(mi, ma);
}

void GL_ResetClipRect(void) {
	ImGui::GetBackgroundDrawList()->PopClipRect();
}

void GL_RenderImage(Image_t* image, int x, int y, int width, int height, int colorRemap, int shapeId) {
	ImVec2 mi(x, y);
	ImVec2 ma(x + width, y + height);

	if (forceForgegroundRender) {
		GL_RenderForeGroundImage(image, x, y, width, height, colorRemap, shapeId);
		return;
	}

	if (image->numFrames > 0) {
		if(image->HouseImages[colorRemap].image[shapeId][((int)animFrameNum) % image->numFrames] == 0)
			ImGui::GetBackgroundDrawList()->AddImage((ImTextureID)image->HouseImages[colorRemap].image[shapeId][0], mi, ma);
		else
			ImGui::GetBackgroundDrawList()->AddImage((ImTextureID)image->HouseImages[colorRemap].image[shapeId][((int)animFrameNum) % image->numFrames], mi, ma);
	}
	else {
		ImGui::GetBackgroundDrawList()->AddImage((ImTextureID)image->HouseImages[colorRemap].image[shapeId][0], mi, ma);
	}

}

void GL_RenderForeGroundImage(Image_t* image, int x, int y, int width, int height, int colorRemap, int shapeId) {
	ImVec2 mi(x, y);
	ImVec2 ma(x + width, y + height);

	if (image->numFrames > 0) {
		if (image->HouseImages[colorRemap].image[shapeId][((int)animFrameNum) % image->numFrames] == 0)
			ImGui::GetForegroundDrawList()->AddImage((ImTextureID)image->HouseImages[colorRemap].image[shapeId][0], mi, ma);
		else
			ImGui::GetForegroundDrawList()->AddImage((ImTextureID)image->HouseImages[colorRemap].image[shapeId][((int)animFrameNum) % image->numFrames], mi, ma);
	}
	else {
		ImGui::GetForegroundDrawList()->AddImage((ImTextureID)image->HouseImages[colorRemap].image[shapeId][0], mi, ma);
	}

}

void GL_FillRect(int color, int x, int y, int width, int height) {
	ImVec2 mi(x, y);
	ImVec2 ma(x + width, y + height);
	float r = backbuffer_palette[(color * 3) + 0] / 255.0f;
	float g = backbuffer_palette[(color * 3) + 1] / 255.0f;
	float b = backbuffer_palette[(color * 3) + 2] / 255.0f;
	if (forceForgegroundRender) {
		ImGui::GetForegroundDrawList()->AddRectFilled(mi, ma, ImGui::ColorConvertFloat4ToU32(ImVec4(r, g, b, 1)));
	}
	else {
		ImGui::GetBackgroundDrawList()->AddRectFilled(mi, ma, ImGui::ColorConvertFloat4ToU32(ImVec4(r, g, b, 1)));
	}
}

void GL_DrawText(int color, int x, int y, char* text) {
	ImVec2 pos(x, y);
	if (color == 0 || color == 160) { // This is a hack!
		color = g_ColorXlat[color % 15];
	}
	float r = backbuffer_palette[(color * 3) + 0] / 255.0f;
	float g = backbuffer_palette[(color * 3) + 1] / 255.0f;
	float b = backbuffer_palette[(color * 3) + 2] / 255.0f;
	ImGui::GetBackgroundDrawList()->AddText(pos, ImGui::ColorConvertFloat4ToU32(ImVec4(r, g, b, 1)), text);
}

void GL_DrawLine(int color, int x, int y, int dx, int dy) {
	ImVec2 pos(x, y);
	ImVec2 pos2(dx, dy);
	float r = backbuffer_palette[(color * 3) + 0] / 255.0f;
	float g = backbuffer_palette[(color * 3) + 1] / 255.0f;
	float b = backbuffer_palette[(color * 3) + 2] / 255.0f;
	ImGui::GetBackgroundDrawList()->AddLine(pos, pos2, ImGui::ColorConvertFloat4ToU32(ImVec4(r, g, b, 1)));
}