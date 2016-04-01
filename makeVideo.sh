rm out.mp4
ffmpeg -framerate 30 -i ./output/output_%05d.bmp -c:v libx264 -pix_fmt yuv420p out.mp4
