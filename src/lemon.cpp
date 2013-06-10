//#include <boost/python.hpp>
#ifdef v8
#include <v8.h>
#endif
//#include <stdlib.h>
#include <stdio.h>
//OC#include <string.h>
#include <math.h>
#include "SDL.h"
#include "SDL_events.h"
#include "sys/time.h"
#include <stdio.h>
#include <signal.h>
#include <X11/Xlib.h>
#include "SDL_thread.h"
#include "SDL_mutex.h"
#include "SDL_syswm.h"
#include "SDL_opengl.h"
#include <GL/glu.h>
#include <dirent.h>
#include <iostream>
#include <vector>

using namespace std;

#include <../rote-3/rote.h>
#include "../XY.h"
#include "makemessage.c"
#include "initsdl.c"
#include "../rote-3/demo/sdlkeys.c"
#include "ftw.cpp"
#include "v3d.cpp"
#include "logmelogme.h"
#include "../gltext.c"
#include "mathhelp.c"
#include "cam.cpp"
#include "quack.c"
#include "obj.cpp"
#include "settingz.cpp"

camera cam;
int w = 1240;
int h = 700;
float znear = 1;
float zfar = 20;
string fnfl; //font file
string clfl; //colors
string dmps, jsv8;
SDL_Surface* s;
#ifdef v8
vector<v8::Persistent<v8::Function> > afterstart;
vector<string> commandstack;
#endif
float maxvol;

#include "terminal.cpp"
#include "face.cpp"
#include "inpipe.cpp"
#include "logger.cpp"
logger * loggerface;
#include "logmelogme.cpp"
#include "../demos/atlantis/atlantis.c"
#include "../demos/flipflop.c"
#include "demos/spectrum_analyzer.cpp"
//#ifdef nerve
#include "demos/nerverot.cpp"
//#endif
//#include <string>
//#include "mplayer.cpp"
//#include "compositing.cpp"
#ifdef fontwatcher
#include <sys/inotify.h>
#include "fontwatcher.cpp"
#endif
//#include "listdir.c"
//#include "buttons.cpp"
//#include "ggg.cpp"
#include "osk.cpp"

#include "gl.c"


#include <limits>

void pick(int up, int button, int x, int y)
{
//    logit("picking objects...");
	y = h - y;
	GLuint fuf[500];
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glSelectBuffer(500, fuf);
	glRenderMode(GL_SELECT);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	gluPickMatrix(x, y, 10, 10, viewport);
	viewmatrix();
	glInitNames();
	glPushName(-1);
	for_each_object
		glLoadName(i);
		o->translate_and_draw(1);
	endf
	glPopMatrix();
	unsigned int i, j, k;
	GLuint minz = std::numeric_limits<unsigned int>::max();
	int nearest = -1;
	unsigned int numhits = glRenderMode(GL_RENDER);
//    logit("%i hits", numhits);
	vector<vector<int> > hits;
	for (i = 0, k = 0; i < numhits; i++)
	{
		GLuint numnames = fuf[k++];
//	logit("%i names", numnames);
//	logit("%d minz", fuf[k]);
//	logit("%d maxz", fuf[k+1]);
		if (fuf[k] < minz)
		{
//	    logit("%u < %u, nearest = %i", fuf[k],minz,i);
			nearest = i;
			minz = fuf[k];
		}
		k += 2;
		vector<int> v;
		hits.push_back(v);
		for (j = 0; j < numnames; j++)
		{
			hits[hits.size() - 1].push_back(fuf[k]);
			k++;
		}
	}
	if (nearest != -1)
	{
		if (hits.at(nearest).size())
		{
			obj *o = objects.at(hits.at(nearest)[0]);
			hits.at(nearest).erase(hits.at(nearest).begin());
			//lets send the name stack, without the first hit, which identifies object
			o->picked(up, button, hits.at(nearest), x, y);
		}
	}

}
void vispick()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	viewmatrix();

	for_each_object
		o->translate_and_draw(2);
	endf
	glPopMatrix();
}


Uint32 TimerCallback(Uint32 interval, void *param)
{
	SDL_Event e;
	e.type = SDL_USEREVENT;
	e.user.code = CODE_TIMER;
	e.user.data1 = param;
	SDL_PushEvent(&e);
	return interval;
}


void updatelinesmooth()
{
	if (settingz.line_antialiasing)
	{
		logit("antialiasing");
		glEnable(GL_LINE_SMOOTH);
	}
	else
	{
		logit("not antialiasing");
		glDisable(GL_LINE_SMOOTH);
	}
}


void physics(int t)
{
	for_each_object
		o->physics(t);
	endf
}
int anything_dirty()
{
	for_each_object
		if (o->getDirty() || o->spin.x || o->spin.y || o->spin.z)
		{ //cout<<o->class_name()<<endl;
			return 1;
		}endf
	return 0;
}
void nothing_dirty()
{
	for_each_object
		o->setClean();
	endf
}

void showhelp(bool unfold)
{
	if (settingz.givehelp)
	{
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0, 1200, 1200, 0, 0, 1);
		if (!unfold)

			draw_text("\npress right ctrl for more fun...");
		else
			draw_text("\nnow press h to hide help");
		glPopMatrix();
	}
}

obj * rightmost(void)
{
	obj *r=active;
	for_each_object
		if (o->t.x > r->t.x) r=o;
	endf
	return r;
}

Uint8 * k;
int bpp;
int dirty;
int done;
int gofullscreen;
int escaped;
int mousejustmoved;
int lastmousemoved;
int afterglow;

#include "events.cpp"

void draw()
{
	viewmatrix();
	if (SDL_GetMouseState(0, 0) || afterglow) vispick();
	viewmatrix();

	gle();
	for_each_object
		o->translate_and_draw(0);
	endf
	gle();

//	showhelp(escaped||k[SDLK_RCTRL]);
	SDL_GL_SwapBuffers();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gle();
}

void lemon(void)
{
	bpp = 8;
	dirty = 1;
	done = 0;
	gofullscreen = 0;
	mousejustmoved = 0;
	escaped = 0;
	s = initsdl(&w, &h, &bpp, SDL_OPENGL);
	w=s->w;
	h=s->h;
	printf("%i x %i\n",w,h);
	SDL_EnableUNICODE(1);
	SDL_InitSubSystem(SDL_INIT_TIMER);
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY / 2,
			SDL_DEFAULT_REPEAT_INTERVAL / 3);
	loadfont(fnfl);
	loadcolors();
	gle();
	resetviewport();
	resetmatrices();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	updatelinesmooth();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);    

//      objects.push_back(new atlantis());
	//objects.push_back(new spectrum_analyzer);
//	for(int i=0;i<16;i++)
//	    objects.push_back(new nerverot(0,0,10));
	//objects.push_back(new flipflop);
//	objects.push_back(new spectrum_analyzer);

//	if (!objects.size())
	{
//    	objects.push_back(loggerface=new logger(-8,0,0,0,70,0));
//	objects.push_back(new face("bash"));

//	objects.push_back(active=new face("bash",1.0,0.0,3.0,0.0,90.0,0.0));

	objects.push_back(active = new face("bash", 0.0, 0.0, 0, 0, 0, 0));

//	objects.push_back(active=new face("bash",-1.0,0.0,3.0,0.0,270.0,0.0));
//	objects.push_back(new fontwatcher);

	}

	maxvol = 0;
	int norm = 0;
	afterglow = 1;
	int lastmousemoved = SDL_GetTicks();
	int lastphysics = SDL_GetTicks();
	int lostphysics = 0;

	inpipemutex = SDL_CreateMutex();
	SDL_CreateThread(inpipe, (void*) 0);

	cam.focus(active->t);
	while (!done)
	{
		SDL_TimerID x = 0;
		lockterms();
		k = SDL_GetKeyState(NULL);
		
		dirty |= cam.moving;

		if ((dirty || anything_dirty()) && (SDL_GetAppState() & SDL_APPACTIVE))
		{
			cam.interpolate();
			draw();
			nothing_dirty();
			if (dirty | anything_dirty()) x = SDL_AddTimer(5, TimerCallback, 0);
			dirty=0;
		}

		unlockterms();
		//logit("---------unlocked waiting\n");
		SDL_Event event;
		if (SDL_WaitEvent(&event))
		{
			if (SDL_GetTicks() - lastmousemoved > 300) afterglow = 0;

			lockterms();
			//logit("---------locked goooin %i\n", event.type);
			if (x) SDL_RemoveTimer(x);
			x = 0;
			do
			{
				process_event(event);
			}
			while (SDL_PollEvent(&event));
		}

		if (mousejustmoved)
		{
			int x, y;
			SDL_GetMouseState(&x, &y);
			//pick(0, 0, x, y);
			mousejustmoved = 0;
		}
		if (gofullscreen)
		{
			if (s->flags & SDL_FULLSCREEN)
			{
				s = SDL_SetVideoMode(w, h, bpp, (s->flags & ~SDL_FULLSCREEN));
#ifdef LINUX
				if (norm & 1)
					system("wmctrl -r :ACTIVE: -b remove,maximized_horz");
				if (norm & 2)
					system("wmctrl -r :ACTIVE: -b remove,maximized_vert");
#endif
			}
			else
			{
#ifdef LINUX
				SDL_SysWMinfo i;
				SDL_VERSION(&i.version)
				if (SDL_GetWMInfo(&i))
				{
					char *c = make_message(
							"xprop -id %u |grep _NET_WM_STATE_MAXIMIZED_VERT",
							i.info.x11.window);
					norm = !system(c);
					free(c);
					c = make_message(
							"xprop -id %u |grep _NET_WM_STATE_MAXIMIZED_HORZ",
							i.info.x11.window);
					norm &= (!system(c)) << 1;
					free(c);
				}
				//system("wmctrl -r :ACTIVE: -b add,maximized_vert,maximized_horz");
#endif
				SDL_Surface *ns;
				ns = SDL_SetVideoMode(w, h, bpp, (s->flags | SDL_FULLSCREEN));
				if (ns) s = ns;
			}
			gofullscreen = 0;
			dirty = 1;
		}
		#ifdef v8
			if (afterstart.size())
			{
			v8::Handle<v8::Value>* args = NULL;
			for (unsigned i = 0; i < afterstart.size(); i++)
				afterstart[i]->Call(afterstart[i], 0, args);
			afterstart.clear();
			}
		#endif

		if (!done) unlockterms();

		if (lostphysics) lastphysics = SDL_GetTicks();
		int t = SDL_GetTicks();
		physics(t - lastphysics);
		lastphysics = t;
		lostphysics = 0;

	}
	objects.clear();
	font.clear();
	SDL_Quit();

}

#include "js.cpp"
//#define py
//#include "py.cpp"


#include "getexecname.c"
void compute_data_file_paths()
{
	string path = string(needexepath()) + string("../data/");
	fnfl = path + string("l1");
	clfl = path + string("colors");
	dmps = path + string("errordumps/");
	jsv8 = path + string("javascript.js");
}

int main(int argc, char *argv[])
{
	compute_data_file_paths();
//	init_py();
//	greet();
	lemon();
//	kill_py();
	return 0;
}

