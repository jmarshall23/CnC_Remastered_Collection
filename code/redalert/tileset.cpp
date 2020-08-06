/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Low level functions for loading and rendering C&C terrain tilesets.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
 /*
 ============================

 This file has been heavily modified by IceColdDuke(Justin Marshall) to render with OpenGL.

 ============================
 */

#include "FUNCTION.H"
#include "gbuffer.h"
#include "Image.h"
#include <cstring>

using std::memcpy;

class GraphicViewPortClass;

#define TD_TILESET_CHECK 0x20

/**
* @brief union is to handle the parts of the header which vary between TD and RA format tiles.
*/
struct IconControlType
{
	uint8_t* Get_Icon_Data()
	{
		if (td.icons == TD_TILESET_CHECK) {
			return reinterpret_cast<uint8_t*>(this) + td.icons;
		}
		else {
			return reinterpret_cast<uint8_t*>(this) + ra.icons;
		}
	}

	uint8_t* Get_Icon_Map()
	{
		if (td.icons == TD_TILESET_CHECK) {
			return reinterpret_cast<uint8_t*>(this) + td.map;
		}
		else {
			return reinterpret_cast<uint8_t*>(this) + ra.map;
		}
	}

	int16_t width; // always 24 (ICON_WIDTH)
	int16_t height; // always 24 (ICON_HEIGHT)
	int16_t count; // count of cells in set, not same as images
	int16_t allocated; // is treated like a bool, always 0 in the file?

	union
	{
		struct
		{
			int32_t size; // filesize
			int32_t icons; // always 0x00000020
			int32_t palettes; // seems to always be 0x00000000
			int32_t remaps; // unknown, bitfield?
			int32_t trans_flag; // array of images length, unknown
			int32_t map; // image index for each cell
		} td;

		struct
		{
			int16_t map_width; // tile width in cells
			int16_t map_height; // tile height in cells
			int32_t size; // filesize
			int32_t icons; // always 0x00000028
			int32_t palettes; // seems to always be 0x00000000
			int32_t remaps; // unknown, bitfield?
			int32_t trans_flag; // array of images length, unknown
			int32_t color_map; // terrain type index, ra only
			int32_t map; // image index for each cell
		} ra;
	};
};

static int IconEntry;
static void* IconData;
static IconControlType* LastIconset;
static uint8_t* StampPtr;
static uint8_t* TransFlagPtr;
static uint8_t* MapPtr;
static int IconWidth;
static int IconHeight;
static int IconSize;
static int IconCount;

void __cdecl Init_Stamps(IconControlType* iconset)
{    
    if (iconset && LastIconset != iconset) {
        IconCount = (iconset->count);
        IconWidth = (iconset->width);
        IconHeight = (iconset->height);
        LastIconset = iconset;
        IconSize = IconWidth * IconHeight;

        // TD and RA tileset headers are slightly different, so check a constant that only exists in one type.
        if ((iconset->td.icons) == TD_TILESET_CHECK) {
            MapPtr = reinterpret_cast<uint8_t*>(iconset) + (iconset->td.map);
            StampPtr = reinterpret_cast<uint8_t*>(iconset) + (iconset->td.icons);
            TransFlagPtr = reinterpret_cast<uint8_t*>(iconset) + (iconset->td.trans_flag);
        }
        else {
            MapPtr = reinterpret_cast<uint8_t*>(iconset) + (iconset->ra.map);
            StampPtr = reinterpret_cast<uint8_t*>(iconset) + (iconset->ra.icons);
            TransFlagPtr = reinterpret_cast<uint8_t*>(iconset) + (iconset->ra.trans_flag);
        }
    }
}

void __cdecl Buffer_Draw_Stamp2(GraphicViewPortClass& viewport, IconControlType* tileset, int icon, int x, int y, const void* remapper)
{
    if (!tileset) {
        return;
    }

    if (LastIconset != tileset) {
        Init_Stamps(tileset);
    }

    int32_t icon_index = MapPtr != nullptr ? MapPtr[icon] : icon;

    if (icon_index < IconCount) {

        int32_t fullpitch = viewport.Get_Full_Pitch(); //(viewport.Get_Pitch() + viewport.Get_XAdd() + viewport.Get_Width());
        uint8_t* dst = (x * 4) + (y * 4) * fullpitch + (uint8_t*)(viewport.Get_Offset());
        int32_t blitpitch = fullpitch - IconWidth;
        uint8_t* src = &StampPtr[IconSize * icon_index];

        if (remapper) {
            const uint8_t* remap = static_cast<const uint8_t*>(remapper);
            for (int i = 0; i < IconHeight; ++i) {
                for (int j = 0; j < IconWidth; ++j) {
                    uint8_t cur_byte = remap[*src++];

                    if (cur_byte) {
                        dst[0] = backbuffer_data_raw[(cur_byte * 3) + 0];
                        dst[1] = backbuffer_data_raw[(cur_byte * 3) + 1];
                        dst[2] = backbuffer_data_raw[(cur_byte * 3) + 2];
                        dst[3] = 255;
                    }

                    dst+=4;
                }

                dst += blitpitch;
            }

        }
        else if (TransFlagPtr[icon_index]) {
            for (int i = 0; i < IconHeight; ++i) {
                for (int j = 0; j < IconWidth; ++j) {
                    uint8_t cur_byte = *src++;

					if (cur_byte) {
						dst[0] = backbuffer_data_raw[(cur_byte * 3) + 0];
						dst[1] = backbuffer_data_raw[(cur_byte * 3) + 1];
						dst[2] = backbuffer_data_raw[(cur_byte * 3) + 2];
						dst[3] = 255;
					}

					dst += 4;
                }

                dst += blitpitch;
            }
        }
        else {
            for (int32_t i = 0; i < IconHeight; ++i) {
                FastScanlinePaletteBlit(dst, src, IconWidth);
                dst += fullpitch;
                src += IconWidth * 4;
            }
        }
    }
}

Image_t* Load_StampHD(int theater, const char* iconName, const void* icondata) {
    char filename[2048];
    Image_t* image = NULL;

	IconControlType* tileset = (IconControlType*)icondata;    

	if (!tileset) {
		return NULL;
	}

	if (LastIconset != tileset) {
		Init_Stamps(tileset);
	}

    // This is because were trying to avoid parsing the XML's and the filenames aren't consistant.
    const char* theaterName = Theaters[theater].Name;
    for (int i = 0; i < IconCount; i++)
    {        
        char filename[512];
        char fixedIconName[512];
        strcpy(fixedIconName, iconName);
        COM_SetExtension(fixedIconName, strlen(fixedIconName), "");

        int numFrames = Tileset_GetNumFramesForTile(theater, fixedIconName, i);

        for (int d = 0; d < numFrames; d++)
        {
            const char* tileEntryName = Tileset_FindHDTexture(theater, fixedIconName, i, d);
            sprintf(filename, "DATA/ART/TEXTURES/SRGB/RED_ALERT/TERRAIN/%s/%s", Theaters[theater].Name, tileEntryName);
            COM_SetExtension(filename, sizeof(filename), ".dds");

            if (image == NULL) {
                // If the image is already loaded, then we are done.
                image = Find_Image(filename);
                if (image) {
                    return image;
                }

                // Load the initial terrain image.
                image = Image_LoadImage(filename);

                if (image != NULL && i != 0) {
                    image->HouseImages[0].image[i][d] = image->HouseImages[0].image[0][d];
                    image->HouseImages[0].image[0][d] = 0;
                }

                if (image != NULL) {
                    image->numFrames = numFrames;
                }
            }
            else {
                if (!Image_Add32BitImage(filename, image, -1, i, d)) {
                    continue;
                }
            }
        }
    }

	//if (image) {
	//	image->IconMapPtr = MapPtr;
	//}

    return image;
}

void Get_Stamp_Size(const void* icondata, int* width, int* height) {
	IconControlType* tileset = (IconControlType*)icondata;

	if (!tileset) {
        for (int i = 0; i < MAX_IMAGE_SHAPES; i++) {
            *width = -1;
            *height = -1;
        }
		return;
	}

	if (LastIconset != tileset) {
		Init_Stamps(tileset);
	}

    for (int i = 0; i < MAX_IMAGE_SHAPES; i++) {
        width[i] = IconWidth;
        height[i] = IconHeight;
    }
}

Image_t * Load_Stamp(const char *name, const void* icondata)
{
	IconControlType* tileset = (IconControlType*)icondata;

	if (!tileset) {
		return NULL;
	}

	if (LastIconset != tileset) {
		Init_Stamps(tileset);
	}


	int icon_index = 0;

    if (icon_index >= IconCount) {
        return NULL;
    }
	
    uint8_t* src = &StampPtr[IconSize * icon_index]; 
    
    // Check to see if the image is already loaded.
    {
        Image_t* image = Find_Image(name);
        if (image) {
            return image;
        }
    }

	Image_t *image = Image_CreateImageFrom8Bit(name, IconWidth, IconHeight, (unsigned char*)src);
    image->IconMapPtr = MapPtr;
    if (IconCount == 1)
        return image;    

    for (int i = 1; i < IconCount; i++) {
        icon_index = i;
        src = &StampPtr[IconSize * icon_index];
        Image_Add8BitImage(image, 0, i, IconWidth, IconHeight, src, NULL);
    }

    return image;
}

int Stamp_GetIconIndex(Image_t* iconImage, int icon) {
    uint8_t* MapPtr = (uint8_t*)iconImage->IconMapPtr;
    return MapPtr != nullptr ? MapPtr[icon] : icon;
}

void __cdecl Buffer_Draw_Stamp_Clip2(GraphicViewPortClass& viewport, const void *icondata, int icon, int x, int y, const void* remapper, int left, int top, int right, int bottom)
{
    Image_t* iconImage = (Image_t*)icondata;
    uint8_t* MapPtr = (uint8_t * )iconImage->IconMapPtr;
	int icon_index = MapPtr != nullptr ? MapPtr[icon] : icon;
	{
        int blit_height = iconImage->renderheight[0];
        int blit_width = iconImage->renderwidth[0];
        
        int width = left + right;
        int xstart = left + x;
        int height = top + bottom;
        int ystart = top + y;


        if (xstart < width && ystart < height && IconHeight + ystart > top && IconWidth + xstart > left) {
            if (xstart < left) {
//                src += left - xstart;
                blit_width -= left - xstart;
                xstart = left;
            }
    
            int src_pitch = IconWidth - blit_width;
    
            if (blit_width + xstart > width) {
                src_pitch += blit_width - (width - xstart);
                blit_width = width - xstart;
            }
    
            if (top > ystart) {
                blit_height = IconHeight - (top - ystart);
  //              src += IconWidth * (top - ystart);
                ystart = top;
            }
    
            if (blit_height + ystart > height) {
                blit_height = height - ystart;
            }
    
            int full_pitch = viewport.Get_Full_Pitch(); //(viewport.Get_Pitch() + viewport.Get_XAdd() + viewport.Get_Width());
            uint8_t* dst = (xstart * 4) + (ystart * 4) * full_pitch + (uint8_t*)(viewport.Get_Offset());
            int dst_pitch = full_pitch - blit_width;
    
            //if (remapper) {
            //    const uint8_t* remap = static_cast<const uint8_t*>(remapper);
            //    for (int i = 0; i < blit_height; ++i) {
            //        for (int j = 0; j < blit_width; ++j) {
            //            uint8_t cur_byte = remap[*src++];
			//			if (cur_byte) {
			//				dst[0] = backbuffer_data_raw[(cur_byte * 3) + 0];
			//				dst[1] = backbuffer_data_raw[(cur_byte * 3) + 1];
			//				dst[2] = backbuffer_data_raw[(cur_byte * 3) + 2];
			//				dst[3] = 255;
			//			}
            //
			//			dst += 4;
            //        }
            //        dst += dst_pitch * 4;
            //    }
            //
            //}
            //else if (TransFlagPtr[icon_index]) {
            //    for (int i = 0; i < blit_height; ++i) {
            //        for (int j = 0; j < blit_width; ++j) {
            //            uint8_t cur_byte = *src++;
			//			if (cur_byte) {
			//				dst[0] = backbuffer_data_raw[(cur_byte * 3) + 0];
			//				dst[1] = backbuffer_data_raw[(cur_byte * 3) + 1];
			//				dst[2] = backbuffer_data_raw[(cur_byte * 3) + 2];
			//				dst[3] = 255;
			//			}
            //
			//			dst += 4;
            //        }
            //        src += src_pitch;
            //        dst += dst_pitch * 4;
            //    }
            //
            //}
            //else {
            {
                //for (int i = 0; i < blit_height; ++i) {
                //    FastScanlinePaletteBlit(dst, src, blit_width);
                //    dst += full_pitch * 4;
                //    src += IconWidth;
                //}

                if (iconImage->HouseImages[0].image[icon_index] == 0)
                    return;

               GL_SetClipRect(xstart, ystart, blit_width, blit_height);
                GL_RenderImage(iconImage, xstart, ystart, blit_width, blit_height, 0, icon_index);
               GL_ResetClipRect();
            }
        }
    }
}

extern "C" void __cdecl Buffer_Draw_Stamp_Clip(void const* this_object, void const* icondata, int icon, int x_pixel, int y_pixel, void const* remap, int min_x, int min_y, int max_x, int max_y) {
    if (icondata == NULL)
        return;

	Buffer_Draw_Stamp_Clip2(*((GraphicViewPortClass*)this_object), icondata, icon, x_pixel, y_pixel, remap, min_x, min_y, max_x, max_y);
}

extern "C" void __cdecl Buffer_Draw_Stamp(void const* thisptr, void const* icondata, int icon, int x_pixel, int y_pixel, void const* remap) {
    Buffer_Draw_Stamp2(*((GraphicViewPortClass*)thisptr), (IconControlType*)icondata, icon, x_pixel, y_pixel, remap);
}

uint8_t* Get_Icon_Set_Map(void* temp)
{
    if (temp != nullptr) {
        if ((static_cast<IconControlType*>(temp)->td.icons) == TD_TILESET_CHECK) {
            return static_cast<uint8_t*>(temp) + (static_cast<IconControlType*>(temp)->td.icons);
        }
        else {
            return static_cast<uint8_t*>(temp) + (static_cast<IconControlType*>(temp)->ra.icons);
        }
    }

    return nullptr;
}
