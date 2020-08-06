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
byte TempTSStampData[48 * 24 * 4];


int Get_IconSet_MapWidth(void const* data)
{
	// jmarshall
	if (data) {
		return ((IsoTile*)data)->NumTilesX();
	}
	// jmarshall end
	return(0);
}

int Get_IconSet_MapHeight(void const* data)
{
	// jmarshall
	if (data) {
        return ((IsoTile*)data)->NumTilesY();
	}
	// jmarshall end
	return(0);
}


Image_t* Load_StampHD(int theater, const char* iconName, const void* icondata) {
    char filename[2048];
    Image_t* image = NULL;

    IsoTile* isoTile = (IsoTile*)icondata;
    if (!isoTile) {
        return NULL;
    }
	

    // This is because were trying to avoid parsing the XML's and the filenames aren't consistant.
    const char* theaterName = Theaters[theater].Name;
    for (int i = 0; i < isoTile->NumTiles(); i++)
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
            //COM_SetExtension(filename, sizeof(filename), ".dds");

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
    IsoTile* isoTile = (IsoTile*)icondata;

	if (!isoTile) {
        for (int i = 0; i < MAX_IMAGE_SHAPES; i++) {
            *width = -1;
            *height = -1;
        }
		return;
	}

    for (int i = 0; i < MAX_IMAGE_SHAPES; i++) {
        width[i] = isoTile->TileImageWidth();
        height[i] = isoTile->TileImageHeight();
    }
}

Image_t * Load_Stamp(const char *name, const void* icondata, void* terrainPalette)
{
    IsoTile* isoTile = (IsoTile*)icondata;
    if (!isoTile) {
        return NULL;
    }
	
    // Check to see if the image is already loaded.
    {
        Image_t* image = Find_Image(name);
        if (image) {
            return image;
        }
    }

    IsoTileImageHeader* initialTile = isoTile->GetTileInfo(0);
    if(initialTile == NULL) {
        return NULL;
    }

    uint8_t* src = initialTile->GetRasterizedData();
	Image_t *image = Image_CreateImageFrom8Bit(name, isoTile->TileImageWidth(), isoTile->TileImageHeight(), (unsigned char*)src, NULL, (unsigned char *)terrainPalette);
    if (image == NULL) {
        return NULL;
    }

    image->isoTileInfo = isoTile;

    for (int i = 1; i < isoTile->NumTiles(); i++) {  
        IsoTileImageHeader* tile = isoTile->GetTileInfo(i);
        if(tile == NULL) {
            continue;
        }

        src = tile->GetRasterizedData();
        Image_Add8BitImage(image, 0, i, isoTile->TileImageWidth(), isoTile->TileImageHeight(), src, NULL, (unsigned char*)terrainPalette);
    }

    return image;
}

int Stamp_GetIconIndex(Image_t* iconImage, int icon) {
    return icon;
}

void __cdecl Buffer_Draw_Stamp_Clip2(GraphicViewPortClass& viewport, const void *icondata, int icon, int x, int y, const void* remapper, int left, int top, int right, int bottom)
{
    Image_t* iconImage = (Image_t*)icondata;
    IsoTile* isoTile = iconImage->isoTileInfo;

    if (isoTile == NULL)
        return;

    if (icon > isoTile->NumTiles())
        icon = isoTile->NumTiles() - 1;
    IsoTileImageHeader* isoStampImage = isoTile->GetTileInfo(icon);
    int icon_index = icon;
	{
        int blit_height = CELL_PIXEL_H;
        int blit_width = CELL_PIXEL_W;
        
        int width = left + right;
        int xstart = left + x;
        int height = top + bottom;
        int ystart = top + y;


        if (xstart < width && ystart < height && iconImage->isoTileInfo->TileImageHeight() + ystart > top && iconImage->isoTileInfo->TileImageWidth() + xstart > left) {
            if (xstart < left) {
//                src += left - xstart;
                blit_width -= left - xstart;
                xstart = left;
            }
    
            int src_pitch = iconImage->isoTileInfo->TileImageWidth() - blit_width;
    
            if (blit_width + xstart > width) {
                src_pitch += blit_width - (width - xstart);
                blit_width = width - xstart;
            }
    
            if (top > ystart) {
                blit_height = iconImage->isoTileInfo->TileImageHeight() - (top - ystart);
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

                if (iconImage->HouseImages[0].image[icon_index][0] == 0) {
                    icon_index = 0;
                }

				int renderTargetScale = ZoomLevel;
				if (Debug_Map) {
					renderTargetScale = 0;
				}

				double scale = ((((float)ScreenWidth / (float)ScreenHeight) * (float)renderTargetScale) * 0.05) + 1;
                float xstart_zoom = xstart * scale;
                float ystart_zoom = ystart * scale;

                if (xstart_zoom >= ScreenWidth || ystart_zoom >= ScreenHeight)
                    return;

               //GL_SetClipRect(xstart, ystart, blit_width, blit_height);
                GL_RenderImage(iconImage, xstart, ystart, blit_width, blit_height, 0, icon_index);
               //GL_ResetClipRect();
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
    // NOT USED
}

uint8_t* Get_Icon_Set_Map(void* temp)
{
    //if (temp != nullptr) {
    //    if ((static_cast<IconControlType*>(temp)->td.icons) == TD_TILESET_CHECK) {
    //        return static_cast<uint8_t*>(temp) + (static_cast<IconControlType*>(temp)->td.icons);
    //    }
    //    else {
    //        return static_cast<uint8_t*>(temp) + (static_cast<IconControlType*>(temp)->ra.icons);
    //    }
    //}

    return nullptr;
}
