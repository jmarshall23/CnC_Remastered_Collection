for %%f in (*.avi*) do (
	ffmpeg -i %%f -c:v libx264 -s 720x480 -pix_fmt yuv420p %%~nf.mp4
	ffmpeg -i %%~nf.mp4 -bitexact -acodec pcm_s16le -ar 22050 -ac 1 %%~nf.wav
)
pause