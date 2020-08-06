set PATH="C:\Program Files\ImageMagick-7.0.10-Q16-HDRI"

magick.exe convert sno1.png -alpha set -virtual-pixel transparent +distort Affine "0,512 0,0   0,0 -87,-50  512,512 87,-50" -background none -compose plus  -bordercolor black -compose over -border 5x2 sno1_iso.png
pause