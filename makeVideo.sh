rm video.mkv
ffmpeg -i ./output/output_%06d.jpg -c:v libx264 -b:v 40000k -minrate 35000k -maxrate 45000k -bufsize 2000k -r 30 video.mkv
