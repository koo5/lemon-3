
DEPS  =  ../rote-3/rote.c demos/* *.cpp *h *c ../*  ../demos/atlantis/* ../demos/nerverot/* ../libs/json/lib/libjson.so
GLLIBS  =  -DGL -DHAVE_OPENGL -lGL
CFLIBS =  -D_REENTRANT -D_XTHREADS -DREENTRANT -DXTHREADS \
`pkg-config ../libs/json/lib/pkgconfig/json.pc --cflags --libs` \
  -I/usr/local/include \
 -L/usr/lib -L/usr/X11R6/lib  -lutil -L/usr/local/lib  \
    -L/usr/local/lib   `pkg-config --libs x11`\
      `pkg-config --cflags x11`\
       -I/usr/include -I/usr/local/include -I./ \
        -I/usr/local/include/vte -lncurses -L/usr/X11R6/lib \
          ../rote-3/inject_csi.c ../rote-3/inject.c\
           ../rote-3/rote_keymap.c ../rote-3/rote.c \
           ../rote-3/wtf.c -lncurses \
           -lutil `python-config --cflags` `python-config --libs`\
           -lboost_python-py27

pl=$(shell uname)

ifeq ($(pl),Linux)
#PYLIBS = -Dpython `/usr/bin/python2.6-config --cflags`  `/usr/bin/python2.6-config --libs ` 
CFLIBS += -DLINUX -pthread `sdl-config --libs --cflags` -lv8  
#`curl-config --libs` `curl-config --cflags` 
GLLIBS += -lGL
# -lSDL_ttf 

else ifeq ($(pl), openbsd)
py=/usr/ports/lang/python/2.5/w-Python-2.5.4/Python-2.5.4/
CFLIBS += -I$(py) -I$(py)/Include  $(py)/libpython2.5.a -R/usr/X11R6/lib -lintl  -liconv -lreadline

else ifeq ($(pl),CYGWIN_NT-6.0)
CFLIBS += -DWINDOWS -I/usr/include/ncurses -I/usr/local/include/SDL -I/usr/include/SDL

endif

                                                                       #-std=c++0x
CC      =clang++ -v -v -v -std=c++11 -Woverflow -o $@ lemon.cpp -g -ggdb $(GLLIBS) $(CFLIBS)  -W -Wall -Wextra  -pedantic -ftrapv\
	-Dnerve -lGLU  -lm -Wno-write-strings  -lXcomposite -lXdamage -lXfixes -lXrender -lX11 



#pixelcity:
#PCSRCS    = $(wildcard ../demos/lemonized-pixel-city/*.cpp)
#PCOBJS    = $(patsubst %.cpp,%.o,$(PCSRCS))
#CC += -Dhas_pixel_city -I../demos/lemonized-pixel-city/ demos/lemonized-pixel-city/*.o
#DEPS += $(PCOBJS)



TARGETS = lsdl lgl
all: lgl
a: $(TARGETS)
lgl$(EXE): $(DEPS)
	$(CC) 

test :
	g++ -o test test.cpp  

lsdl$(EXE):  $(DEPS)
	$(CC)  -DSDLD  -lpng   -Xlinker   \
	    -g  /usr/local/lib/libSDL_draw.a

clean:
	rm ./lgl ./lsdl ./lglpy;\
	rm -rf ../libs/ftgl ../libs/json
	
	

submodules:
	cd ../&&\
	git submodule init&&\
	git submodule update

../rote-3/rote.c: submodules

demos/lemonized-pixel-city/%.cpp: submodules

demos/lemonized-pixel-city/%.o: demos/lemonized-pixel-city/%.cpp
	${CXX} -D_lemon  -Wno-write-strings  -c -o $@ $^

../libs/json/lib/libjson.so:
	cd ../libs/json-c-0.9&&\
	./configure --prefix=`pwd`/../json &&\
	make&&make install


