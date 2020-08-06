#ifndef __RENDERTEXTURE_H__
#define __RENDERTEXTURE_H__

struct Image_t;

/*
================================================================================================

	Render Texture

================================================================================================
*/

class RenderTexture {
public:
							RenderTexture(Image_t*colorImage, Image_t *depthImage);
							~RenderTexture();

	int						GetWidth() const;
	int						GetHeight() const;

	Image_t*				GetColorImage(int idx) const { return colorImages[idx]; }
	Image_t*				GetDepthImage() const { return depthImage; }

	int						GetNumColorImages() const { return colorImages.size(); }

	void					Resize( int width, int height );

	void					MakeCurrent( void );
	static void				BindNull(void);

	unsigned int			GetDeviceHandle(void) { return deviceHandle; }

	void					AddRenderImage(Image_t*image);
	void					InitRenderTexture(void);
private:

	std::vector<Image_t *>	colorImages;
	Image_t	*				depthImage;
	unsigned int			deviceHandle;
};

#endif //!__RENDERTEXTURE_H__
