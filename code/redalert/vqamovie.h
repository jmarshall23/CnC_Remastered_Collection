// VQAMOVIE.H
//

#pragma once

extern "C" {
	#include "../external/libsmacker/smacker.h"
};

typedef enum {
	FMV_IDLE,
	FMV_PLAY,			// play
	FMV_EOF,			// all other conditions, i.e. stop/EOF/abort
	FMV_ID_BLT,
	FMV_ID_IDLE,
	FMV_LOOPED,
	FMV_ID_WAIT
} cinStatus_t;

struct _VQAHandle {
	_VQAHandle();
	~_VQAHandle();

	GraphicBufferClass*		video_graphics_buffer;

	unsigned long			width;
	unsigned long			height;
	unsigned long			frame_count;
	unsigned char			track_mask;
	unsigned char			channels[7];
	unsigned char			bitdepth[7];
	unsigned long			audio_rate[7];

	smk						smacker_video;
};

long  VQA_Play(_VQAHandle* vqaHandle);
void  VQA_Close(_VQAHandle* vqaHandle);
void  VQA_Free(_VQAHandle* vqaHandle);
long  VQA_Open(_VQAHandle* videoHandle, char const* filename);
void VQA_ImageForTime(_VQAHandle* videoHandle, int milliseconds);
void VQA_FFMPEGReset(_VQAHandle* videoHandle);
_VQAHandle* VQA_Alloc(void);