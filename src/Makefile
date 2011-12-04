
DEPS  =  ../rote-3/rote.c lemon.cpp ../* demos/atlantis/* demos/nerverot/*  ../libs/yaml-cpp-0.2.5/libyaml-cpp.so  ../libs/json/lib/libjson.so ../libs/ftgl/lib/libftgl.so
GLLIBS  =  -DGL -DHAVE_OPENGL -lGL
CFLIBS =  -D_REENTRANT -D_XTHREADS -DREENTRANT -DXTHREADS \
`pkg-config ../libs/json/lib/pkgconfig/json.pc --cflags --libs` `pkg-config ../libs/ftgl/lib/pkgconfig/ftgl.pc --cflags --libs` \
  -I/usr/local/include \
 -L/usr/lib -L/usr/X11R6/lib  -lutil -L/usr/local/lib  \
    -L/usr/local/lib   `pkg-config --libs x11`\
      `pkg-config --cflags x11`\
       -I/usr/include -I/usr/local/include -I./ \
        -I/usr/local/include/vte -lncurses -L/usr/X11R6/lib \
          ../rote-3/inject_csi.c ../rote-3/inject.c\
           ../rote-3/rote_keymap.c ../rote-3/rote.c \
           ../rote-3/wtf.c -lncurses  \
           ../libs/yaml-cpp-0.2.5/libyaml-cpp.so

pl=$(shell uname)

ifeq ($(pl),Linux)
#PYLIBS = -Dpython `/usr/bin/python2.6-config --cflags`  `/usr/bin/python2.6-config --libs ` 
CFLIBS += -DLINUX -pthread `sdl-config --libs --cflags` `curl-config --libs` `curl-config --cflags` -lv8 
GLLIBS += -lGL
# -lSDL_ttf 

else ifeq ($(pl), openbsd)
py=/usr/ports/lang/python/2.5/w-Python-2.5.4/Python-2.5.4/
CFLIBS += -I$(py) -I$(py)/Include  $(py)/libpython2.5.a -R/usr/X11R6/lib -lintl  -liconv -lreadline

else ifeq ($(pl),CYGWIN_NT-6.0)
CFLIBS += -DWINDOWS -I/usr/include/ncurses -I/usr/local/include/SDL -I/usr/include/SDL

endif

                                                                       #-std=c++0x
CC      =clang++ -v -v -v -x c++ -Woverflow -o $@ lemon.cpp -g -ggdb $(GLLIBS) $(CFLIBS)  -W -Wall -Wextra  -pedantic -ftrapv\
	 -Dlibpng -lpng -Dnerve -lGLU  -lm -Wno-write-strings  -lXcomposite -lXdamage -lXfixes -lXrender -lX11 



#pixelcity:
PCSRCS    = $(wildcard demos/lemonized-pixel-city/*.cpp)
PCOBJS    = $(patsubst %.cpp,%.o,$(PCSRCS))
CC += -Dhas_pixel_city -Idemos/lemonized-pixel-city/ demos/lemonized-pixel-city/*.o
DEPS += $(PCOBJS)



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
	rm ../libs/yaml-cpp-0.2.5/libyaml-cpp.so*;\
	rm -rf ../libs/ftgl ../libs/json
	
	

../libs/yaml-cpp-0.2.5/libyaml-cpp.so:
	cd ../libs/yaml-cpp-0.2.5&&\
	cmake CMakeLists.txt &&\
	make

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

../libs/ftgl/lib/libftgl.so:
	cd ../libs/ftgl-2.1.3-rc5&&\
	./configure --prefix=`pwd`/../ftgl &&\
	make&&make install


