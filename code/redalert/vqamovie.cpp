// VQAMOVIE.CPP
//

#include "FUNCTION.H"
#include "VQAMOVIE.H"
#include "AUDIOMIX.H"
#include <SDL.h>
#include <ctime>

byte* raw_image_buffer = nullptr;
int vqa_upscale_hack_width = 0;
int vqa_upscale_hack_height = 0;
byte* vqa_output_buffer = nullptr;

byte* VQA_Dropsample(const byte* in, int inwidth, int inheight, int outwidth, int outheight) {
	int		i, j, k;
	const byte* inrow;
	const byte* pix1;
	byte* out, * out_p;

	out = vqa_output_buffer;
	out_p = out;

	for (i = 0; i < outheight; i++, out_p += outwidth * 1) {
		inrow = in + 1 * inwidth * (int)((i + 0.25) * inheight / outheight);
		for (j = 0; j < outwidth; j++) {
			k = j * inwidth / outwidth;
			pix1 = inrow + k * 1;
			out_p[j * 1 + 0] = pix1[0];
			//out_p[j * 3 + 1] = pix1[1];
			//out_p[j * 3 + 2] = pix1[2];
		}
	}

	return out;
}

/*
==================
_VQAHandle::_VQAHandle
==================
*/
_VQAHandle::_VQAHandle() {

}

/*
==================
_VQAHandle::~_VQAHandle
==================
*/
_VQAHandle::~_VQAHandle() {

}

/*
==================
_VQAHandle::VQA_Alloc
==================
*/
_VQAHandle* VQA_Alloc(void) { 
	return new _VQAHandle(); 
}

char smk_info_all(const smk object, unsigned long* frame, unsigned long* frame_count, double* usf);
char smk_info_video(const smk object, unsigned long* w, unsigned long* h, unsigned char* y_scale_mode);

long  VQA_Open(_VQAHandle *videoHandle, char const* filename)  {
	int ret;
	char fullpath[512];

	sprintf(fullpath, "movies/%s", filename);
	videoHandle->smacker_video = smk_open_file(fullpath, 0);
	if (videoHandle->smacker_video == NULL)
		return -1;
	unsigned char yscalemode;
	unsigned long frame;
	double usf;
	smk_info_video(videoHandle->smacker_video, &videoHandle->width, &videoHandle->height, &yscalemode);
	smk_info_all(videoHandle->smacker_video, &frame, &videoHandle->frame_count, &usf);
	smk_enable_all(videoHandle->smacker_video, 65536);
	smk_info_audio(videoHandle->smacker_video, &videoHandle->track_mask, &videoHandle->channels[0], &videoHandle->bitdepth[0], &videoHandle->audio_rate[0]);
	videoHandle->video_graphics_buffer = &VisiblePage;//new GraphicBufferClass();
	if (vqa_output_buffer == nullptr)
		vqa_output_buffer = new byte[ScreenWidth * ScreenHeight * 3];

	//videoHandle->video_graphics_buffer->Init(videoHandle->width, videoHandle->height, NULL, 0, (GBC_Enum)(GBC_VISIBLE | GBC_VIDEOMEM));

	return (0); 
}

void  VQA_Free(_VQAHandle* vqaHandle) {
	delete vqaHandle;
}

void  VQA_Close(_VQAHandle* vqaHandle) {
	
}
long  VQA_Play(_VQAHandle* vqaHandle) { 
	int curTime = clock(); //get the current time
	int currentFrame = 0;	
	bool shouldSampleAudio = true;

	smk_first(vqaHandle->smacker_video);

	while (currentFrame < vqaHandle->frame_count) {
		SDL_Event event;

		// Check for any new SDL events.
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				//delete raw_image_buffer;
				raw_image_buffer = nullptr;
				return 0;
			}
		}
		
		smk_render_frame(vqaHandle->smacker_video, currentFrame);
		
		if (shouldSampleAudio) {
			unsigned char* audio_track = smk_get_audio(vqaHandle->smacker_video, 0);
			int audio_track_len = smk_get_audio_size(vqaHandle->smacker_video, 0);

			AudMix_QueueVideoAudio((const char*)audio_track, audio_track_len, vqaHandle->audio_rate[0]);

			shouldSampleAudio = false;
		}
		//// Grab the frame from the movie.
		//VQA_ImageForTime(vqaHandle, Sys_Milliseconds() - vqaHandle->startTime);
		//
		////memcpy(raw_image_buffer, vqaHandle->image, vqaHandle->CIN_WIDTH * vqaHandle->CIN_HEIGHT * 4 * 2);
		//for (int i = 0; i < vqaHandle->CIN_WIDTH * vqaHandle->CIN_HEIGHT; i++) {
		//	raw_image_buffer[(i * 4) + 0] = vqaHandle->image[(i * 3) + 0];
		//	raw_image_buffer[(i * 4) + 1] = vqaHandle->image[(i * 3) + 1];
		//	raw_image_buffer[(i * 4) + 2] = vqaHandle->image[(i * 3) + 2];
		//}

		raw_image_buffer = smk_get_palette(vqaHandle->smacker_video);
		Set_DD_Palette(raw_image_buffer, false);
		vqaHandle->video_graphics_buffer->Lock();

		VQA_Dropsample(smk_get_video(vqaHandle->smacker_video), vqaHandle->width, vqaHandle->height, ScreenWidth, ScreenHeight);

		Buffer_To_Page(0, 0, ScreenWidth, ScreenHeight, vqa_output_buffer, *vqaHandle->video_graphics_buffer);
		vqaHandle->video_graphics_buffer->Unlock();

		Device_Present();
		
		if (clock() - curTime >= 30) {
			currentFrame++;
			curTime = clock();

			shouldSampleAudio = true;
		}

	}

	//delete raw_image_buffer;
	raw_image_buffer = nullptr;

	return (0); 
}