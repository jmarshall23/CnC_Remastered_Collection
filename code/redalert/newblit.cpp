// NEWBLIT.CPP
//

#include <imgui.h>
#include "FUNCTION.H"
#include "Image.h"
#include "gl/glew.h"

extern byte backbuffer_palette[768];
extern uint8_t g_ColorXlat[16];

bool forceForgegroundRender = false;
int32_t g_currentColor;

static void GL_SetRenderTextureCallback(const ImDrawList* parent_list, const ImDrawCmd* cmd) {
	RenderTexture* renderTexture = (RenderTexture*)cmd->UserCallbackData;
	if(renderTexture == NULL) {
		RenderTexture::BindNull();
	}
	else {
		renderTexture->MakeCurrent();
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
	}
}

static void GL_EnableBlendCallback(const ImDrawList* parent_list, const ImDrawCmd* cmd) {
	GLBlendType blendType = (GLBlendType)(int64_t)cmd->UserCallbackData;
	glEnable(GL_BLEND);
	if(blendType == GL_BLEND_NONE) {		
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else if(blendType == GL_BLEND_ADD) {
		glBlendEquation(GL_ADD);
		glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);
	}
	else {
		//glBlendEquation(GL_MULT);
		glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
	}
}

void GL_EnableBlend(GLBlendType blendType) {
	ImGui::GetBackgroundDrawList()->AddCallback(GL_EnableBlendCallback, (void *)blendType);
}

void GL_SetRenderTexture(RenderTexture *renderTexture) {
	ImGui::GetBackgroundDrawList()->AddCallback(GL_SetRenderTextureCallback, renderTexture);
}

void GL_ForceForegroundRender(bool force) {
	forceForgegroundRender = force;
}

void GL_SetColor(float r, float g, float b) {
	g_currentColor = ImGui::ColorConvertFloat4ToU32(ImVec4(r, g, b, 1));
}

void GL_SetClipRect(int x, int y, int width, int height) {
	ImVec2 mi(x, y);
	ImVec2 ma(x + width, y + height);
	ImGui::GetBackgroundDrawList()->PushClipRect(mi, ma);
}

void GL_ResetClipRect(void) {
	ImGui::GetBackgroundDrawList()->PopClipRect();
}

void GL_RenderImage(Image_t* image, int x, int y, int width, int height, int colorRemap, int shapeId, bool ignoreOutOfScreenPixels, bool flipUV) {
	if (!ignoreOutOfScreenPixels)
	{
		if (x < 0 || y < 0) {
			return;
		}

		if (x >= ScreenWidth || y >= ScreenHeight) {
			return;
		}
	}

	ImVec2 mi(x, y);
	ImVec2 ma(x + width, y + height);
	ImVec2 uv(0, 0);
	ImVec2 uv2(1, 1);

	if(flipUV) {
		uv.y = 1;
		uv2.y = 0;
	}

	if (forceForgegroundRender) {
		GL_RenderForeGroundImage(image, x, y, width, height, colorRemap, shapeId);
		return;
	}

	if (image->numFrames > 0) {
		if(image->HouseImages[colorRemap].image[shapeId][((int)animFrameNum) % image->numFrames] == 0)
			ImGui::GetBackgroundDrawList()->AddImage((ImTextureID)image->HouseImages[colorRemap].image[shapeId][0], mi, ma, uv, uv2, g_currentColor);
		else
			ImGui::GetBackgroundDrawList()->AddImage((ImTextureID)image->HouseImages[colorRemap].image[shapeId][((int)animFrameNum) % image->numFrames], mi, ma, uv, uv2, g_currentColor);
	}
	else {
		ImGui::GetBackgroundDrawList()->AddImage((ImTextureID)image->HouseImages[colorRemap].image[shapeId][0], mi, ma, uv, uv2, g_currentColor);
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

void GL_FillRect(int _r, int _g, int _b, int x, int y, int width, int height) {
	ImVec2 mi(x, y);
	ImVec2 ma(x + width, y + height);
	float r = _r / 255.0f;
	float g = _g / 255.0f;
	float b = _b / 255.0f;
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

void GL_DrawText(int color, float fontsize, float r, float g, float b, int x, int y, char* text) {
	ImVec2 pos(x, y);
	float _r = r / 255.0f;
	float _g = r / 255.0f;
	float _b = r / 255.0f;
	ImGui::GetBackgroundDrawList()->AddText(NULL, fontsize, pos, ImGui::ColorConvertFloat4ToU32(ImVec4(_r, _g, _b, 1)), text);
}

void GL_DrawForegroundText(int color, int x, int y, char* text) {
	ImVec2 pos(x, y);
	if (color == 0 || color == 160) { // This is a hack!
		color = g_ColorXlat[color % 15];
	}
	float r = backbuffer_palette[(color * 3) + 0] / 255.0f;
	float g = backbuffer_palette[(color * 3) + 1] / 255.0f;
	float b = backbuffer_palette[(color * 3) + 2] / 255.0f;
	ImGui::GetForegroundDrawList()->AddText(pos, ImGui::ColorConvertFloat4ToU32(ImVec4(r, g, b, 1)), text);
}

void GL_DrawForegroundText(int color, float fontsize, float r, float g, float b, int x, int y, char* text) {
	ImVec2 pos(x, y);
	float _r = r / 255.0f;
	float _g = r / 255.0f;
	float _b = r / 255.0f;
	ImGui::GetForegroundDrawList()->AddText(NULL, fontsize, pos, ImGui::ColorConvertFloat4ToU32(ImVec4(_r, _g, _b, 1)), text);
}

void GL_DrawLine(int color, int x, int y, int dx, int dy) {
	ImVec2 pos(x, y);
	ImVec2 pos2(dx, dy);
	float r = backbuffer_palette[(color * 3) + 0] / 255.0f;
	float g = backbuffer_palette[(color * 3) + 1] / 255.0f;
	float b = backbuffer_palette[(color * 3) + 2] / 255.0f;
	ImGui::GetBackgroundDrawList()->AddLine(pos, pos2, ImGui::ColorConvertFloat4ToU32(ImVec4(r, g, b, 1)));
}