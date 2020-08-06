// Image.h
//

#define MAX_IMAGE_SHAPES 1024
#define MAX_IMAGE_FRAMES 12
#define MAX_HOUSE_COLORS 8

#include "rgb.h"

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
	uint8_t* ScratchBuffer;
	struct IsoTile* isoTileInfo;
};

__forceinline Image_t::Image_t() {
	isoTileInfo = NULL;
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
Image_t* Image_CreateImageFrom8Bit(const char* name, int Width, int Height, unsigned char* data, unsigned char *remap = NULL, unsigned char *palette = NULL);
Image_t* Find_Image(const char* name);
void Image_Add8BitImage(Image_t* image, int HouseId, int ShapeID, int Width, int Height, unsigned char* data, unsigned char* remap, unsigned char* palette = NULL);
bool Image_Add32BitImage(const char* name, Image_t* image, int HouseId, int ShapeID, int frameId);

Image_t* Image_CreateBlankImage(const char* name, int width, int height, bool hasAlpha);
void Image_UploadRaw(Image_t* image, uint8_t* data, bool paletteRebuild, uint8_t *palette, bool hasAlpha);
Image_t* Image_CreateDepthImage(const char* name, int width, int height);

void Sys_SetOverlayImage(Image_t* image);

extern byte TempTSStampData[48 * 24 * 4];

struct IsoTileHeader_t {
	int TileWidth;
	int TileHeight;
	int TileImageWidth;
	int TileImageHeight;
};

#define WESTWOOD_ISO_NOTSET_FLAG -842150451 // Westwood didn't zero everything out in there tile exporter, so this is required.

struct IsoTileImageHeader
{
	uint8_t* GetExtraData(void)
	{		
		if(ExtraOffset == 0 || ExtraOffset == WESTWOOD_ISO_NOTSET_FLAG) {
			return NULL;
		}
		return reinterpret_cast<uint8_t*>(this) + ExtraOffset + sizeof(IsoTileImageHeader);
	}

	uint8_t* GetZData(IsoTile* tile)
	{
		return reinterpret_cast<uint8_t*>(tile) + ZDataoffset;
	}

	uint8_t* GetExtraZData(IsoTile* tile)
	{
		return reinterpret_cast<uint8_t*>(tile) + ExtraZOffset;
	}

	uint8_t* GetData() {
		return reinterpret_cast<uint8_t*>(this) + sizeof(IsoTileImageHeader);
	}

	// https://xhp.xwis.net/documents/TMP_TS_Format.html
	uint8_t* GetRasterizedData() {
		memset(TempTSStampData, 0, sizeof(TempTSStampData));
		uint8_t* tileData = GetData();
		byte* w_line = TempTSStampData;
		int x = 24;
		int global_cx = 48;
		int cx = 0;
		for (int y = 0; y < 12; y++)
		{
			cx += 4;
			x -= 2;
			memcpy(w_line + x, tileData, cx);
			tileData += cx;
			w_line += global_cx;
		}
		for (; y < 23; y++)
		{
			cx -= 4;
			x += 2;
			memcpy(w_line + x, tileData, cx);
			tileData += cx;
			w_line += global_cx;
		}
		w_line = TempTSStampData;

		//uint8_t *extraData = GetExtraData();
		//if (extraData != NULL)
		//{
		//	tileData += 576;
		//	int cx = ExtraX;
		//	int cy = ExtraY;
		//	for (y = 0; y < cy; y++)
		//	{
		//		byte* w = w_line;
		//		for (int i = 0; i < cx; i++)
		//		{
		//			int v = *tileData++;
		//			if (v)
		//				*w = v;
		//			w++;
		//		}
		//		w_line += global_cx;
		//	}
		//}
		return TempTSStampData;
	}

	int TileX;
	int TileY;
	int32_t ExtraOffset;
	int32_t ZDataoffset;
	int32_t ExtraZOffset;
	signed int ExtraX;
	signed int ExtraY;
	int ExtraWidth;
	int ExtraHeight;
	struct Flags
	{
		unsigned int HasExtraData : 1; //1
		unsigned int HasZData : 1; //2
		unsigned int HasDamagedData : 1; //4
		// invalid cause memory wasn'y cleared so all bitfields are at 0xCD by default
		//unsigned int Bit8 : 1; //8
		//unsigned int Bit16 : 1; //16
		//unsigned int Bit32 : 1; //32
		//unsigned int Bit64 : 1; //64
		//unsigned int Bit128 : 1; //128

	};
	Flags flags;
	unsigned char Height;
	unsigned char TileType;
	unsigned char RampType;
	RGBClass LowRGB;
	RGBClass HighRGB;
	unsigned char Padding[3];
};

class IsoTile {
public:
	int TileImageWidth() { return header.TileImageWidth; }
	int TileImageHeight() { return header.TileImageHeight; }

	IsoTileImageHeader* GetTileInfo(int idx) {
		uint8_t* ptr = reinterpret_cast<uint8_t*>(this);
		ptr += sizeof(IsoTileHeader_t);
		uint32_t* offsets = (uint32_t*)ptr;
		if (offsets[idx] == 0 || offsets[idx] == WESTWOOD_ISO_NOTSET_FLAG) {
			return NULL;
		}

		IsoTileImageHeader* tileImageHeaders = (IsoTileImageHeader*)(((uint8_t*)this) + offsets[idx]);
		return tileImageHeaders;
	}
	int NumTiles(void) { return header.TileWidth * header.TileHeight; }
	int NumTilesX(void) { return header.TileWidth; }
	int NumTilesY(void) { return header.TileHeight; }
private:
	IsoTileHeader_t header;
};