// NewBlit.h
//

struct Image_t;
void GL_RenderImage(Image_t* image, int x, int y, int width, int height, int colorRemap = 0, int shapeId = 0);
void GL_RenderForeGroundImage(Image_t* image, int x, int y, int width, int height, int colorRemap = 0, int shapeId = 0);
void GL_DrawText(int color, int x, int y, char* text);
void GL_FillRect(int color, int x, int y, int width, int height);
void GL_DrawLine(int color, int x, int y, int dx, int dy);
void GL_ResetClipRect(void);
void GL_SetClipRect(int x, int y, int width, int height);
void GL_ForceForegroundRender(bool force);