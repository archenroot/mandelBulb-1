CC       =  pgCC
CXX	 = 	pgCC
FLAGS    = -O2 -acc -fast -ta=tesla -Minfo=accel  -Msafeptr
CFLAGS   = $(FLAGS)
CXXFLAGS = $(FLAGS)
LDFLAGS  = -lm

PROGRAM_NAME=mandelbulb
PROGRAM_FLOAT=floatbulb
PROGRAM_VIDEO=video

$(PROGRAM_NAME): mainBulb.o print.o timing.o savebmp.o getparams.o renderer.o init3D.o
	$(CC) -o $@ $? $(CFLAGS) $(LDFLAGS)

$(PROGRAM_FLOAT): mainFloatBulb.o print.o timing.o savebmp.o getparams.o renderFloat.o init3D.o
	$(CC) -o $@ $? $(CFLAGS) $(LDFLAGS)
	
$(PROGRAM_VIDEO): mainVideo.o print.o timing.o savebmp.o getparams.o rendererVideo.o init3D.o
	$(CC) -o $@ $? $(CFLAGS) $(LDFLAGS)
	
clean:
	rm *.o $(PROGRAM_NAME) $(PROGRAM_VIDEO) $(PROGRAM_FLOAT) $(EXEEXT) *~ image.bmp
