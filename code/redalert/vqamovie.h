// VQAMOVIE.H
//

#pragma once

typedef enum {
	FMV_IDLE,
	FMV_PLAY,			// play
	FMV_EOF,			// all other conditions, i.e. stop/EOF/abort
	FMV_ID_BLT,
	FMV_ID_IDLE,
	FMV_LOOPED,
	FMV_ID_WAIT
} cinStatus_t;

#pragma pack(push,8)
typedef struct {
	int					imageWidth, imageHeight;	// will be a power of 2
	struct Image_t*		image;						// RGBA format, alpha will be 255
	uint8_t*			rawbuffer;
	int					status;
} cinData_t;
#pragma pop

class _VQAHandle {
public:
	_VQAHandle();
	~_VQAHandle();

	// returns false if it failed to load
	virtual bool		InitFromFile(const char* qpath, bool looping) = 0;

	// returns the length of the animation in milliseconds
	virtual int			AnimationLength() = 0;

	// the pointers in cinData_t will remain valid until the next UpdateForTime() call
	virtual cinData_t	ImageForTime(int milliseconds) = 0;

	// closes the file and frees all allocated memory
	virtual void		Close() = 0;

	// closes the file and frees all allocated memory
	virtual void		ResetTime(int time) = 0;
};

void VQA_Init(void);
long  VQA_Play(_VQAHandle* vqaHandle);
void  VQA_Close(_VQAHandle* vqaHandle);
void  VQA_Free(_VQAHandle* vqaHandle);
long  VQA_Open(_VQAHandle* videoHandle, char const* filename);
void VQA_ImageForTime(_VQAHandle* videoHandle, int milliseconds);
void VQA_FFMPEGReset(_VQAHandle* videoHandle);
_VQAHandle* VQA_Alloc(void);