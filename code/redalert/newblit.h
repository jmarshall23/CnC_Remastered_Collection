// NewBlit.h
//

struct Image_t;
class RenderTexture;

enum GLBlendType {
	GL_BLEND_NONE = 0,
	GL_BLEND_MULT,
	GL_BLEND_ADD
};

void GL_RenderImage(Image_t* image, int x, int y, int width, int height, int colorRemap = 0, int shapeId = 0, bool ignoreOutOfScreenPixels = false, bool flipUV = false);
void GL_RenderForeGroundImage(Image_t* image, int x, int y, int width, int height, int colorRemap = 0, int shapeId = 0);
void GL_DrawText(int color, int x, int y, char* text);
void GL_FillRect(int color, int x, int y, int width, int height);
void GL_FillRect(int _r, int _g, int _b, int x, int y, int width, int height);
void GL_DrawLine(int color, int x, int y, int dx, int dy);
void GL_ResetClipRect(void);
void GL_SetClipRect(int x, int y, int width, int height);
void GL_ForceForegroundRender(bool force);
void GL_DrawForegroundText(int color, int x, int y, char* text);
void GL_DrawForegroundText(int color, float fontsize, float r, float g, float b, int x, int y, char* text);
void GL_SetColor(float r, float g, float b);
void GL_SetRenderTexture(RenderTexture* renderTexture);
void GL_EnableBlend(GLBlendType blendType);
void GL_DrawText(int color, float fontsize, float r, float g, float b, int x, int y, char* text);
void GL_RenderLine(int x1, int y1, int x2, int y2, int r, int g, int b, float thickness);