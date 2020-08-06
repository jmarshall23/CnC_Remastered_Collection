/*
 * Copyright 2007-2020 The OpenRA Developers (see AUTHORS)
 * This file is part of OpenRA, which is free software. It is made
 * available to you under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version. For more
 * information, see COPYING.
 */

#include	"function.h"
#include    "AUDIOMIX.H"

#include <AL/al.h>
#include <AL/alc.h>

ALCdevice* device = nullptr;
ALCcontext* context = nullptr;

struct AudioBuffer_t {
	AudioBuffer_t() {
		buffer = 0;
		size = 0;
		frequency = 0;
		format = 0;
	}
	ALuint buffer;
	ALsizei size;
	ALsizei frequency;
	ALenum format;
};

enum AudioBufferType_t {
	AUDIO_BUFFER_HOUSE_NONE = 0,
	AUDIO_BUFFER_HOUSE_ALIED,
	AUDIO_BUFFER_HOUSE_SOVIET,
	AUDIO_BUFFER_NUMTYPES
};

#define MAX_PRECACHE_AUDIO				512
#define MAX_STREAM_BUFFERS 20

AudioBuffer_t precacheAudioTable[AUDIO_BUFFER_NUMTYPES][MAX_PRECACHE_AUDIO];
AudioBuffer_t speechPrecacheAudioTable[MAX_PRECACHE_AUDIO];
AudioBuffer_t musicPrecacheAudioTable[MAX_PRECACHE_AUDIO];
ALuint streaming_buffers[MAX_STREAM_BUFFERS];
AudioBuffer_t movieAudioBuffer;

int currentMusicPlaying = -1;
int currentAudioTSource = 0;

struct RIFF_Header {
	char chunkID[4];
	long chunkSize;
	char format[4];
};

struct WAVE_Format {
	char subChunkID[4];
	long subChunkSize;
	short audioFormat;
	short numChannels;
	long sampleRate;
	long byteRate;
	short blockAlign;
	short bitsPerSample;
};

struct WAVE_Data {
	char subChunkID[4];
	long subChunk2Size;
};

#define MAX_AUDIO_SOURCES 32
#define SPEECH_AUDIO_SOURCE (MAX_AUDIO_SOURCES - 3)
#define MOVIE_AUDIO_SOURCE (MAX_AUDIO_SOURCES - 2)
#define MUSIC_AUDIO_SOURCE (MAX_AUDIO_SOURCES - 1)
ALuint audio_sources[MAX_AUDIO_SOURCES];
int current_streaming_audiosource = 0;

bool loadWavFile(const char* filename, AudioBuffer_t &audioBuffer) {
	FILE* soundFile = NULL;
	WAVE_Format wave_format;
	RIFF_Header riff_header;
	WAVE_Data wave_data;
	unsigned char* data;

	ALuint* buffer = &audioBuffer.buffer;
	ALsizei* size = &audioBuffer.size;
	ALsizei* frequency = &audioBuffer.frequency;
	ALenum* format = &audioBuffer.format;

	if(audioBuffer.buffer != 0) {
		alDeleteBuffers(1, &audioBuffer.buffer);
	}

	try {
		soundFile = fopen(filename, "rb");
		if (!soundFile) {
			return false;
		}

		fread(&riff_header, sizeof(RIFF_Header), 1, soundFile);

		if ((riff_header.chunkID[0] != 'R' ||
			riff_header.chunkID[1] != 'I' ||
			riff_header.chunkID[2] != 'F' ||
			riff_header.chunkID[3] != 'F') &&
			(riff_header.format[0] != 'W' ||
				riff_header.format[1] != 'A' ||
				riff_header.format[2] != 'V' ||
				riff_header.format[3] != 'E'))
			throw ("Invalid RIFF or WAVE Header");

		fread(&wave_format, sizeof(WAVE_Format), 1, soundFile);

		if (wave_format.subChunkID[0] != 'f' ||
			wave_format.subChunkID[1] != 'm' ||
			wave_format.subChunkID[2] != 't' ||
			wave_format.subChunkID[3] != ' ')
			throw ("Invalid Wave Format");

		if (wave_format.subChunkSize > 16)
			fseek(soundFile, sizeof(short), SEEK_CUR);

		fread(&wave_data, sizeof(WAVE_Data), 1, soundFile);

		if (wave_data.subChunkID[0] != 'd' ||
			wave_data.subChunkID[1] != 'a' ||
			wave_data.subChunkID[2] != 't' ||
			wave_data.subChunkID[3] != 'a')
			throw ("Invalid data header");

		data = new unsigned char[wave_data.subChunk2Size];

		if (!fread(data, wave_data.subChunk2Size, 1, soundFile))
			throw ("error loading WAVE data into struct!");

		*size = wave_data.subChunk2Size;
		*frequency = wave_format.sampleRate;

		if (wave_format.numChannels == 1) {
			if (wave_format.bitsPerSample == 8)
				*format = AL_FORMAT_MONO8;
			else if (wave_format.bitsPerSample == 16)
				*format = AL_FORMAT_MONO16;
		}
		else if (wave_format.numChannels == 2) {
			if (wave_format.bitsPerSample == 8)
				*format = AL_FORMAT_STEREO8;
			else if (wave_format.bitsPerSample == 16)
				*format = AL_FORMAT_STEREO16;
		}

		alGenBuffers(1, buffer);
		alBufferData(*buffer, *format, (void*)data,
			*size, *frequency);
		fclose(soundFile);

		return true;
	}
	catch (char* error) {
		if (soundFile != NULL)
			fclose(soundFile);
		return false;
	}
}


/*
=============
AudMix_IsEnabled
=============
*/
bool AudMix_IsEnabled(void) {
	return device != NULL && context != NULL;
}

/*
=============
AudMix_PlayMusic
=============
*/
void AudMix_PlayMusic(int musicId) {
	if (currentMusicPlaying != -1) {
		//alSourcei(musicPrecacheAudioTable[currentMusicPlaying].sourceId, AL_LOOPING, 0);
		alSourceStop(audio_sources[MAX_AUDIO_SOURCES - 1]);			
	}

	alSourcei(audio_sources[MAX_AUDIO_SOURCES - 1], AL_BUFFER, musicPrecacheAudioTable[musicId].buffer);
	alSourcei(audio_sources[MAX_AUDIO_SOURCES - 1], AL_LOOPING, 1);
	alSourcePlay(audio_sources[MAX_AUDIO_SOURCES - 1]);
	currentMusicPlaying = musicId;
}

/*
=============
AudMix_PlayMovieAudio
=============
*/
void AudMix_PlayMovieAudio(const char *name) {
	if(!loadWavFile(name, movieAudioBuffer)) {
		return;
	}

	alSourcei(audio_sources[MAX_AUDIO_SOURCES - 1], AL_BUFFER, movieAudioBuffer.buffer);
	alSourcei(audio_sources[MAX_AUDIO_SOURCES - 1], AL_LOOPING, 0);
	alSourcePlay(audio_sources[MAX_AUDIO_SOURCES - 1]);
}

void AudMix_StopMovieAudio(void) {
	alSourceStop(audio_sources[MAX_AUDIO_SOURCES - 1]);
}

bool AudMix_IsSourcePlaying(ALuint source) 
{ 
	ALenum state;        
	alGetSourcei(source, AL_SOURCE_STATE, &state);        
	return (state == AL_PLAYING); 
}

void AudMix_TickMovieAudio(void) {
	int numBuffers = 0;

	alGetSourcei(audio_sources[MOVIE_AUDIO_SOURCE], AL_BUFFERS_PROCESSED, &numBuffers);
	while (numBuffers--)
	{
		ALuint buffer;
		alSourceUnqueueBuffers(audio_sources[MOVIE_AUDIO_SOURCE], 1, &buffer);
	}
}

/*
=============
AudMix_QueueVideoAudio
=============
*/
void AudMix_QueueVideoAudio(const char* audio_buffer, int length, int rate) {
	int numBuffers = 0;

	alGetSourcei(audio_sources[MOVIE_AUDIO_SOURCE], AL_BUFFERS_PROCESSED, &numBuffers);
	while (numBuffers--)
	{
		ALuint buffer;
		alSourceUnqueueBuffers(audio_sources[MOVIE_AUDIO_SOURCE], 1, &buffer);
	}

	numBuffers = 0;
	alGetSourcei(audio_sources[MOVIE_AUDIO_SOURCE], AL_BUFFERS_QUEUED, &numBuffers);
	if (numBuffers == MAX_STREAM_BUFFERS)
	{
	//	assert(!"Steam dropping raw samples, reached MAX_STREAM_BUFFERS");		
		return;
	}

	if (current_streaming_audiosource >= MAX_STREAM_BUFFERS)
		current_streaming_audiosource = 0;
	
	alBufferData(streaming_buffers[current_streaming_audiosource], AL_FORMAT_MONO16, (ALvoid*)audio_buffer, length, 22050);
	alSourceQueueBuffers(audio_sources[MOVIE_AUDIO_SOURCE], 1, &streaming_buffers[current_streaming_audiosource]);
	if(!AudMix_IsSourcePlaying(audio_sources[MOVIE_AUDIO_SOURCE]))
		alSourcePlay(audio_sources[MOVIE_AUDIO_SOURCE]);

	//MOVIE_AUDIO_SOURCE
	current_streaming_audiosource++;
}

/*
=============
AudMix_Init
=============
*/
void AudMix_Init(void) {
	// Open up the audio device.
	device = alcOpenDevice(NULL);
	if (!device) {
		return;
	}

	context = alcCreateContext(device, NULL);
	if (!alcMakeContextCurrent(context)) {		
		return;
	}

	// Precache all of the audio.
	for (int i = 0; i < VOC_COUNT; i++) {
		char expandedFilePath[512];

		sprintf(expandedFilePath, "sound/%s.wav", GetEffectFileName(i));

		if (!loadWavFile(expandedFilePath, precacheAudioTable[AUDIO_BUFFER_HOUSE_NONE][i]))
		{
			sprintf(expandedFilePath, "sound/alied/%s.wav", GetEffectFileName(i));
			if (!loadWavFile(expandedFilePath, precacheAudioTable[AUDIO_BUFFER_HOUSE_ALIED][i])) {
			//	assert(!"failed to load audio!");
			}

			sprintf(expandedFilePath, "sound/russian/%s.wav", GetEffectFileName(i));
			if (!loadWavFile(expandedFilePath, precacheAudioTable[AUDIO_BUFFER_HOUSE_SOVIET][i])) {
				//assert(!"failed to load audio!");
			}
		}
	}
	
	// Precache all the Speech
	for (int i = 0; i < VOX_COUNT; i++) {
		char expandedFilePath[512];

		sprintf(expandedFilePath, "sound/speech/%s.wav", GetSpeechFileName(i));
		if (!loadWavFile(expandedFilePath, speechPrecacheAudioTable[i])) {
			//assert(!"failed to load music!");
		}
	}

	// Precache the music.
	for (int i = 0; i < THEME_COUNT; i++) {
		char expandedFilePath[512];

		sprintf(expandedFilePath, "sound/music/%s.wav", GetThemeMusicFileName(i));
		if (!loadWavFile(expandedFilePath, musicPrecacheAudioTable[i])) {
			//assert(!"failed to load music!");
		}
	}

	// Allocate the audio sources.
	alGenSources(MAX_AUDIO_SOURCES, &audio_sources[0]);
	for(int i = 0; i < MAX_AUDIO_SOURCES; i++)
		alSource3f(audio_sources[i], AL_POSITION, 0.0f, 0.0f, 0.0f);

	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);	

	alGenBuffers(MAX_STREAM_BUFFERS, streaming_buffers);
}

void PlayAudio(AudioBuffer_t& buffer, bool speech) {
	// Only one speech sound can play at once.
	if (speech) {
		//if (AudMix_IsSourcePlaying(SPEECH_AUDIO_SOURCE))
		//	return;

		alSourceStop(audio_sources[SPEECH_AUDIO_SOURCE]);
		alSourcei(audio_sources[SPEECH_AUDIO_SOURCE], AL_BUFFER, buffer.buffer);
		alSourcePlay(audio_sources[SPEECH_AUDIO_SOURCE]);
		return;
	}

	if (currentAudioTSource >= MAX_AUDIO_SOURCES - 3)
		currentAudioTSource = 0;

	alSourcei(audio_sources[currentAudioTSource], AL_BUFFER, buffer.buffer);
	alSourcePlay(audio_sources[currentAudioTSource]);

	currentAudioTSource++;
}

void AudMix_SetMusicState(bool musicState) {
	if (!musicState) {
		alSourcePause(audio_sources[MUSIC_AUDIO_SOURCE]);
	}
	else {
		alSourcePlay(audio_sources[MUSIC_AUDIO_SOURCE]);
	}
}

void On_Sound_Effect(int sound_index, int variation, COORDINATE coord, int house)
{
	// MBL 06.17.2019
	int voc = sound_index;
	if (voc == VOC_NONE)
	{
		return;
	}

	if (precacheAudioTable[AUDIO_BUFFER_HOUSE_NONE][sound_index].buffer != 0) {
		PlayAudio(precacheAudioTable[AUDIO_BUFFER_HOUSE_NONE][sound_index], false);
	}
	else if(PlayerPtr != NULL) {
		// We have to play a faction audio.
		if (PlayerPtr->Class->House == HOUSE_USSR || PlayerPtr->Class->House == HOUSE_UKRAINE || PlayerPtr->Class->House == HOUSE_TURKEY) {
			PlayAudio(precacheAudioTable[AUDIO_BUFFER_HOUSE_SOVIET][sound_index], false);
		}
		else {
			PlayAudio(precacheAudioTable[AUDIO_BUFFER_HOUSE_ALIED][sound_index], false);
		}
	}
}


// void On_Speech(int speech_index) // MBL 02.06.2020
void On_Speech(int speech_index, HouseClass* house)
{
	//if (house == NULL) {
		PlayAudio(speechPrecacheAudioTable[speech_index], true);
	//}

	// DLLExportClass::On_Speech(PlayerPtr, speech_index); // MBL 02.06.2020
	//if (house == NULL) {
	//	DLLExportClass::On_Speech(PlayerPtr, speech_index);
	//}
	//else
	//{
	//	DLLExportClass::On_Speech(house, speech_index);
	//}
}


/***********************************************************************************************
 * Is_Speaking -- Checks to see if the eva voice is still playing.                             *
 *                                                                                             *
 *    Call this routine when the EVA voice being played needs to be checked. A typical use     *
 *    of this would be when some action needs to be delayed until the voice has finished --    *
 *    say the end of the game.                                                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Is the EVA voice still playing?                                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/12/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool Is_Speaking(void)
{
	Speak_AI();
	return AudMix_IsSourcePlaying(SPEECH_AUDIO_SOURCE);
}
