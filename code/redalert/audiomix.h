// AUDIOMIX.H
//


#pragma once

void AudMix_Init(void);
const char* GetEffectFileName(int index);
const char* GetThemeMusicFileName(int index);
void AudMix_PlayMusic(int musicId);
const char* GetSpeechFileName(int index);
void AudMix_QueueVideoAudio(const char* audio_buffer, int length, int rate);
void AudMix_TickMovieAudio(void);
void AudMix_SetMusicState(bool musicState);
void AudMix_PlayMovieAudio(const char* name);
void AudMix_StopMovieAudio(void);