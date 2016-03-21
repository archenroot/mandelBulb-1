CC       =  pgCC
CXX	 = 	pgCC
FLAGS    = -O2 -acc -Minfo=accel -fast -ta=tesla,time -Msafeptr
CFLAGS   = $(FLAGS)
CXXFLAGS = $(FLAGS)
LDFLAGS  = -lm

PROGRAM_NAME=mandelbulb

$(PROGRAM_NAME): main.o print.o timing.o savebmp.o getparams.o renderer.o init3D.o
	$(CC) -o $@ $? $(CFLAGS) $(LDFLAGS)


clean:
	rm *.o $(PROGRAM_NAME) $(EXEEXT) *~ image.bmp
