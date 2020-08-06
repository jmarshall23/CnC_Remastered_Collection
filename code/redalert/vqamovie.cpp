// VQAMOVIE.CPP
//

#include "FUNCTION.H"
#include "VQAMOVIE.H"
#include "AUDIOMIX.H"
#include <SDL.h>
#include <ctime>
#include "image.h"


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

long  VQA_Open(_VQAHandle *videoHandle, char const* filename)  {
	int ret;
	char fullpath[512];

	sprintf(fullpath, "movies/%s.mp4", filename);
	if(!videoHandle->InitFromFile(fullpath, false)) {
		return -1;
	}

	sprintf(fullpath, "movies/english/%s.wav", filename);
	AudMix_PlayMovieAudio(fullpath);

	return (0); 
}

void  VQA_Free(_VQAHandle* vqaHandle) {
	delete vqaHandle;
}

void  VQA_Close(_VQAHandle* vqaHandle) {
	
}
long  VQA_Play(_VQAHandle* vqaHandle) { 
	float VideoStartTime = Sys_Milliseconds() * 0.001;
	float VideoEndTime = VideoStartTime + vqaHandle->AnimationLength();


	while (true) {
		SDL_Event event;
		float floatTime = Sys_Milliseconds() * 0.001;

		// Check for any new SDL events.
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				//delete raw_image_buffer;		
				AudMix_StopMovieAudio();
				return 0;
			}
		}

		cinData_t cin = vqaHandle->ImageForTime((int)(1000 * (floatTime - VideoStartTime)));
		if (cin.image && cin.rawbuffer)
		{
			Image_UploadRaw(cin.image, cin.rawbuffer, false, NULL, false);
		}
		if(cin.image == NULL) {
			AudMix_StopMovieAudio();
			return 0;
		}

		GL_RenderImage(cin.image, 0, 0, ScreenWidth, ScreenHeight);

		Device_Present();
	}

	AudMix_StopMovieAudio();
	
	return (0); 
}

