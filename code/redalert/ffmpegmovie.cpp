

#include "FUNCTION.H"
#include "VQAMOVIE.H"
#include "AUDIOMIX.H"
#include <SDL.h>
#include <ctime>
#include "image.h"

#pragma pack(push,8)
extern "C"
{
	//#ifdef WIN32
#ifndef INT64_C
#define INT64_C(c) (c ## LL)
#define UINT64_C(c) (c ## ULL)
#endif
//#include <inttypes.h>
//#endif

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}
#pragma pack(pop)

/*
=======================================

For Red Alert Allied Force we are using FFMPEG to load H264 movies.

We are using code from
https://github.com/RobertBeckebans/RBDOOM-3-BFG/blob/master/neo/renderer/Cinematic.cpp

=======================================
*/
class FfmpegMovie : public _VQAHandle {
public:
	FfmpegMovie();
	~FfmpegMovie();

	virtual bool			InitFromFile(const char* qpath, bool looping);
	virtual cinData_t		ImageForTime(int milliseconds);
	virtual int				AnimationLength();
	virtual void			Close();
	virtual void			ResetTime(int time);
private:
	int						video_stream_index;
	AVFormatContext* fmt_ctx;
	AVFrame* frame;
	AVFrame* frame2;
	AVCodec* dec;
	AVCodecContext* dec_ctx;
	SwsContext* img_convert_ctx;
	bool					hasFrame;
	long					framePos;

	bool					looping;

	int						startTime;
	int						CIN_WIDTH;
	int						CIN_HEIGHT;

	int						animationLength;
	int						frameRate;

	Image_t* img;

	byte* image;

	cinStatus_t				status;

	cinData_t				ImageForTimeFFMPEG(int milliseconds);
	bool					InitFromFFMPEGFile(const char* qpath, bool looping);
	void					FFMPEGReset();
};


/*
========================
FfmpegMovie::FfmpegMovie
========================
*/
FfmpegMovie::FfmpegMovie() {
	//	fmt_ctx = avformat_alloc_context();
#if LIBAVCODEC_VERSION_INT >= AV_VERSION_INT(55,28,1)
	frame = av_frame_alloc();
	frame2 = av_frame_alloc();
#else
	frame = avcodec_alloc_frame();
	frame2 = avcodec_alloc_frame();
#endif // LIBAVCODEC_VERSION_INT
	dec_ctx = NULL;
	fmt_ctx = NULL;
	video_stream_index = -1;
	img_convert_ctx = NULL;
	hasFrame = false;
	looping = false;
	startTime = 0;
	CIN_WIDTH = 0;
	CIN_HEIGHT = 0;
	animationLength = 0;
	frameRate = 0;
	image = NULL;
	status = FMV_EOF;
	img = NULL;
}

/*
========================
FfmpegMovie::~FfmpegMovie
========================
*/
FfmpegMovie::~FfmpegMovie() {
#if defined(_WIN32) || defined(_WIN64)
	av_frame_free(&frame);
	av_frame_free(&frame2);
#else
	av_freep(&frame);
	av_freep(&frame2);
#endif

	if (fmt_ctx)
	{
		avformat_free_context(fmt_ctx);
	}

	if (img_convert_ctx)
	{
		sws_freeContext(img_convert_ctx);
	}

	if (image) {
		free(image);
		image = NULL;
	}

	//if (img) {
	//	img->PurgeImage();
	//}
}

/*
========================
FfmpegMovie::InitFromFile
========================
*/
bool FfmpegMovie::InitFromFile(const char* qpath, bool looping) {
	int ret;

	this->looping = looping;
	startTime = 0;

	if ((ret = avformat_open_input(&fmt_ctx, qpath, NULL, NULL)) < 0)
	{
		Console_Printf("idCinematic: Cannot open FFMPEG video file: '%s', %d\n", qpath, looping);
		return false;
	}
	if ((ret = avformat_find_stream_info(fmt_ctx, NULL)) < 0)
	{
		Console_Printf("idCinematic: Cannot find stream info: '%s', %d\n", qpath, looping);
		return false;
	}
	/* select the video stream */
	ret = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, &dec, 0);
	if (ret < 0)
	{
		Console_Printf("idCinematic: Cannot find a video stream in: '%s', %d\n", qpath, looping);
		return false;
	}
	video_stream_index = ret;
	dec_ctx = fmt_ctx->streams[video_stream_index]->codec;
	/* init the video decoder */
	if ((ret = avcodec_open2(dec_ctx, dec, NULL)) < 0)
	{
		Console_Printf("idCinematic: Cannot open video decoder for: '%s', %d\n", qpath, looping);
		return false;
	}

	CIN_WIDTH = dec_ctx->width;
	CIN_HEIGHT = dec_ctx->height;
	/** Calculate Duration in seconds
	  * This is the fundamental unit of time (in seconds) in terms
	  * of which frame timestamps are represented. For fixed-fps content,
	  * timebase should be 1/framerate and timestamp increments should be
	  * identically 1.
	  * - encoding: MUST be set by user.
	  * - decoding: Set by libavcodec.
	  */
	AVRational avr = dec_ctx->time_base;
	/**
	 * For some codecs, the time base is closer to the field rate than the frame rate.
	 * Most notably, H.264 and MPEG-2 specify time_base as half of frame duration
	 * if no telecine is used ...
	 *
	 * Set to time_base ticks per frame. Default 1, e.g., H.264/MPEG-2 set it to 2.
	 */
	int ticksPerFrame = dec_ctx->ticks_per_frame;
	float durationSec = static_cast<double>(fmt_ctx->streams[video_stream_index]->duration) * static_cast<double>(ticksPerFrame) / static_cast<double>(avr.den);
	animationLength = durationSec * 1000;
	frameRate = av_q2d(fmt_ctx->streams[video_stream_index]->avg_frame_rate);
	hasFrame = false;
	framePos = -1;
	Console_Printf("Loaded FFMPEG file: '%s', looping=%d%dx%d, %f FPS, %f sec\n", qpath, looping, CIN_WIDTH, CIN_HEIGHT, frameRate, durationSec);
	image = (byte*)malloc(CIN_WIDTH * CIN_HEIGHT * 4 * 2);
	avpicture_fill((AVPicture*)frame2, image, AV_PIX_FMT_BGR32, CIN_WIDTH, CIN_HEIGHT);
	if (img_convert_ctx)
	{
		sws_freeContext(img_convert_ctx);
	}
	img_convert_ctx = sws_getContext(dec_ctx->width, dec_ctx->height, dec_ctx->pix_fmt, CIN_WIDTH, CIN_HEIGHT, AV_PIX_FMT_BGR32, SWS_BICUBIC, NULL, NULL, NULL);
	status = FMV_PLAY;

	img = Image_CreateBlankImage(qpath, dec_ctx->width, dec_ctx->height, false); //renderSystem->CreateImage(va("_%s_cinematic", fileFullPath.c_str()), &opts, TF_LINEAR);

	startTime = 0;
	ImageForTime(0);
	status = (looping) ? FMV_PLAY : FMV_IDLE;

	//startTime = Sys_Milliseconds();

	return true;
}

/*
======================
idCinematic::FFMPEGReset
======================
*/
void FfmpegMovie::FFMPEGReset()
{
	// RB: don't reset startTime here because that breaks video replays in the PDAs
	//startTime = 0;

	framePos = -1;

	if (av_seek_frame(fmt_ctx, video_stream_index, 0, 0) >= 0)
	{
		status = FMV_LOOPED;
	}
	else
	{
		status = FMV_EOF;
	}
}

/*
======================
idCinematic::ImageForTime
======================
*/
cinData_t FfmpegMovie::ImageForTime(int milliseconds) {
	cinData_t	cinData;

	//if (milliseconds <= 0)
	{
		milliseconds = Sys_Milliseconds();
	}

	memset(&cinData, 0, sizeof(cinData));
	if (status == FMV_EOF)
	{
		return cinData;
	}

	if (!fmt_ctx)
	{
		// RB: .bik requested but not found
		return cinData;
	}

	if ((!hasFrame) || startTime == -1)
	{
		if (startTime == -1)
		{
			FFMPEGReset();
		}
		startTime = milliseconds;
	}

	long desiredFrame = ((milliseconds - startTime) * frameRate) / 1000;
	if (desiredFrame < 0)
	{
		desiredFrame = 0;
	}

	if (desiredFrame < framePos)
	{
		FFMPEGReset();
	}

	if (hasFrame && desiredFrame == framePos)
	{
		cinData.imageWidth = CIN_WIDTH;
		cinData.imageHeight = CIN_HEIGHT;
		cinData.status = status;
		cinData.image = img;
		return cinData;
	}

	AVPacket packet;
	while (framePos < desiredFrame)
	{
		int frameFinished = 0;

		// Do a single frame by getting packets until we have a full frame
		while (!frameFinished)
		{
			// if we got to the end or failed
			if (av_read_frame(fmt_ctx, &packet) < 0)
			{
				// can't read any more, set to EOF
				status = FMV_EOF;
				if (looping)
				{
					desiredFrame = 0;
					FFMPEGReset();
					framePos = -1;
					startTime = milliseconds;
					if (av_read_frame(fmt_ctx, &packet) < 0)
					{
						status = FMV_IDLE;
						return cinData;
					}
					status = FMV_PLAY;
				}
				else
				{
					status = FMV_IDLE;
					return cinData;
				}
			}
			// Is this a packet from the video stream?
			if (packet.stream_index == video_stream_index)
			{
				// Decode video frame
				avcodec_decode_video2(dec_ctx, frame, &frameFinished, &packet);
			}
			// Free the packet that was allocated by av_read_frame
			av_free_packet(&packet);
		}

		framePos++;
	}

	// We have reached the desired frame
	// Convert the image from its native format to RGB
	sws_scale(img_convert_ctx, frame->data, frame->linesize, 0, dec_ctx->height, frame2->data, frame2->linesize);
	cinData.imageWidth = CIN_WIDTH;
	cinData.imageHeight = CIN_HEIGHT;
	cinData.status = status;	
	hasFrame = true;
	cinData.rawbuffer = image;
	cinData.image = img;

	return cinData;
}

/*
======================
idCinematic::AnimationLength
======================
*/
int FfmpegMovie::AnimationLength() {
	return animationLength;
}

/*
======================
idCinematic::Close
======================
*/
void FfmpegMovie::Close() {
	if (image)
	{
		free((void*)image);
		image = NULL;
		status = FMV_EOF;
	}

	hasFrame = false;

	if (img_convert_ctx)
	{
		sws_freeContext(img_convert_ctx);
	}

	img_convert_ctx = NULL;

	if (dec_ctx)
	{
		avcodec_close(dec_ctx);
	}

	if (fmt_ctx)
	{
		avformat_close_input(&fmt_ctx);
	}
	status = FMV_EOF;
}

/*
======================
idCinematic::ResetTime
======================
*/
void FfmpegMovie::ResetTime(int time) {
	startTime = time; //originally this was: ( backEnd.viewDef ) ? 1000 * backEnd.viewDef->floatTime : -1;
	status = FMV_PLAY;
}

/*
==================
_VQAHandle::VQA_Alloc
==================
*/
_VQAHandle* VQA_Alloc(void) {
	return new FfmpegMovie();
}


/*
==================
VQA_Init
==================
*/
void VQA_Init(void) {
	avcodec_register_all();
	av_register_all();
}
