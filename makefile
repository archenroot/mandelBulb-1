CC       =  pgCC
CXX	 = 	pgCC
FLAGS    = -O2 -acc -fast -ta=tesla -Minfo=accel  -Msafeptr
CFLAGS   = $(FLAGS)
CXXFLAGS = $(FLAGS)
LDFLAGS  = -lm

PROGRAM_NAME=mandelbulb
PROGRAM_VIDEO=video

$(PROGRAM_NAME): main.o print.o timing.o savebmp.o getparams.o renderer.o init3D.o
	$(CC) -o $@ $? $(CFLAGS) $(LDFLAGS)

$(PROGRAM_VIDEO): video.o print.o timing.o savebmp.o getparams.o renderer.o init3D.o
	$(CC) -o $@ $? $(CFLAGS) $(LDFLAGS)
	
clean:
	rm *.o $(PROGRAM_NAME) $(EXEEXT) *~ image.bmp
