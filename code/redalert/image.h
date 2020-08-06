// Image.h
//

#define MAX_IMAGE_SHAPES 1024
#define MAX_IMAGE_FRAMES 12
#define MAX_HOUSE_COLORS 8

struct HouseImage_t {
	unsigned int image[MAX_IMAGE_SHAPES][MAX_IMAGE_FRAMES];
};

struct Image_t {
	Image_t();
	~Image_t();

	unsigned int GetImageForHouse(int houseId, int shapeId, int frame = 0) {
		return HouseImages[houseId].image[shapeId][frame];
	}

	char name[512];
	int64_t namehash;
	HouseImage_t HouseImages[MAX_HOUSE_COLORS];
	unsigned int numAnimFrames;
	int width;
	int height;
	int renderwidth[MAX_IMAGE_SHAPES];
	int renderheight[MAX_IMAGE_SHAPES];
	bool perFrameRenderDimen;
	int numFrames;
	void* IconMapPtr;
	uint8_t* ScratchBuffer;
};

__forceinline Image_t::Image_t() {
	IconMapPtr = NULL;
	numAnimFrames = 0;
	ScratchBuffer = NULL;
	perFrameRenderDimen = false;
	memset(HouseImages, 0, sizeof(HouseImages));
}

__forceinline Image_t::~Image_t() {
	if (ScratchBuffer) {
		delete ScratchBuffer;
		ScratchBuffer = NULL;
	}
}

Image_t* Image_LoadImage(const char* name, bool loadAnims = false, bool loadHouseColor = false);
Image_t* Image_CreateImageFrom8Bit(const char* name, int Width, int Height, unsigned char* data, unsigned char *remap = NULL);
Image_t* Find_Image(const char* name);
void Image_Add8BitImage(Image_t* image, int HouseId, int ShapeID, int Width, int Height, unsigned char* data, unsigned char* remap);
bool Image_Add32BitImage(const char* name, Image_t* image, int HouseId, int ShapeID, int frameId);

Image_t* Image_CreateBlankImage(const char* name, int width, int height);
void Image_UploadRaw(Image_t* image, uint8_t* data, bool paletteRebuild, uint8_t *palette);

void Sys_SetOverlayImage(Image_t* image);