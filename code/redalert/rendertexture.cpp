// RenderTexture.cpp
//

#include "function.h"
#include "image.h"
#include "gl/glew.h"

/*
========================
RenderTexture::RenderTexture
========================
*/
RenderTexture::RenderTexture(Image_t *colorImage, Image_t *depthImage) {
	deviceHandle = -1;
	if (colorImage != nullptr)
	{
		AddRenderImage(colorImage);
	}
	this->depthImage = depthImage;
}

/*
========================
RenderTexture::~RenderTexture
========================
*/
RenderTexture::~RenderTexture() {
	if (deviceHandle != -1)
	{
		glDeleteFramebuffers(1, &deviceHandle);
		deviceHandle = -1;
	}
}
/*
================
RenderTexture::AddRenderImage
================
*/
void RenderTexture::AddRenderImage(Image_t *image) {
	if (deviceHandle != -1) {
		assert(!"RenderTexture::AddRenderImage: Can't add render image after FBO has been created!");
	}

	colorImages.push_back(image);
}

/*
================
RenderTexture::InitRenderTexture
================
*/
void RenderTexture::InitRenderTexture(void) {
	glGenFramebuffers(1, &deviceHandle);
	glBindFramebuffer(GL_FRAMEBUFFER, deviceHandle);

	for (int i = 0; i < colorImages.size(); i++) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorImages[i]->HouseImages[0].image[0][0], 0);
	}

	if (depthImage != nullptr) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthImage->HouseImages[0].image[0][0], 0);
	}

	if (colorImages.size() > 0)
	{
		GLenum DrawBuffers[5] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
		if (colorImages.size() >= 5) {
			assert(!"InitRenderTextures: Too many render targets!");
		}
		glDrawBuffers(colorImages.size(), &DrawBuffers[0]);
	}


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		assert(!"RenderTexture::InitRenderTexture: Failed to create rendertexture!");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/*
================
RenderTexture::MakeCurrent
================
*/
void RenderTexture::MakeCurrent(void) {
	glBindFramebuffer(GL_FRAMEBUFFER, deviceHandle);
}

/*
================
RenderTexture::BindNull
================
*/
void RenderTexture::BindNull(void) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

int	RenderTexture::GetWidth() const { return (colorImages.size() > 0) ? colorImages[0]->renderwidth[0] : depthImage->renderwidth[0]; }
int	RenderTexture::GetHeight() const { return (colorImages.size() > 0) ? colorImages[0]->renderheight[0] : depthImage->renderheight[0]; }

/*
================
RenderTexture::Resize
================
*/
void RenderTexture::Resize(int width, int height) {
	//Image_t *target = nullptr;
	//
	//if (colorImages.Num() > 0) {
	//	target = colorImages[0];
	//}
	//else {
	//	target = depthImage;
	//}
	//
	//if (target->GetOpts().width == width && target->GetOpts().height == height) {
	//	return;
	//}
	//
	//for(int i = 0; i < colorImages.Num(); i++) {
	//	colorImages[i]->Resize(width, height);
	//}
	//
	//if (depthImage != nullptr) {
	//	depthImage->Resize(width, height);
	//}
	//
	//if (deviceHandle != -1)
	//{
	//	glDeleteFramebuffers(1, &deviceHandle);
	//	deviceHandle = -1;
	//}
	//
	//InitRenderTexture();
}
