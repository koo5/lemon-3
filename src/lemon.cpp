/*******************************************************************
* Description:
* Author: koom,,, <>
* Created at: Sat Jul 25 04:25:26 CEST 2009
* Computer: koom-desktop
* System: Linux 2.6.28-11-generic on i686
*
* Copyright (c) 2009 koom,,,  All rights reserved.
*
********************************************************************/
/*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include <v8.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "SDL.h"
#include "SDL_events.h"
#include "sys/time.h"
#include <stdio.h>
#include <signal.h>
#include <X11/Xlib.h>
#include <../rote-3/rote.h>
#include "../XY.h"
#include "SDL_thread.h"
#include "SDL_mutex.h"
#include "SDL_syswm.h"
#include "makemessage.c"
#ifdef GL
#include "SDL_opengl.h"
#include <GL/glu.h>
#else
#include "SDL_draw.h"
#endif
#ifdef libpng
#include "screenshot.c"
#endif
#include "initsdl.c"
#include <dirent.h>
#include "../rote-3/demo/sdlkeys.c"
#include <iostream>
#include <vector>
#include "../libs/yaml-cpp-0.2.5/include/yaml.h"
#include "serializable.h"

#define for_each_object for(unsigned int i=0;i<objects.size();i++) { obj*o=objects.at(i);
#define endf }
using namespace YAML;
using namespace std;
int w = 1280;
int h = 800;

float znear=1;
float zfar=20;

SDL_Surface* s;

#ifdef SDLD
#include "../sdldlines.c"
#endif

#define SAVE(class) 	YAML_SERIALIZABLE_AUTO(class)\
			void emit_members(Emitter &out)const
#define LOAD	void load_members(const Node& doc)
#define save(x) YAML_EMIT_MEMBER(out, x);
#define load(x) try{YAML_LOAD_MEMBER(doc, x);}catch(...){}
#define vsave(x) YAML_EMIT(out, x);
#define vload(x) try{YAML_LOAD(doc, x);}catch(...){}

#include "v3d.cpp"

v3d cam;
v3d look;
v3d cr;

void clogit(const char * iFormat, ...);
void slogit(const char * iFormat, ...);
void logit(const char  * iFormat, ...);
void clogit(string       iFormat, ...);
void slogit(string       iFormat, ...);
void logit(string        iFormat, ...);

#include "../gltext.c"

vector<v8::Persistent<v8::Function>>afterstart;

string fnfl;//font file
string clfl;//colors
string wrld;//world yaml
string dmps, jsv8;

int min(int a, int b)
{
    return a < b ? a : b;
}
int max(int a, int b)
{
    return a > b ? a : b;
}

int in(int a, int b, int c)
{
    return(a<=b&&c>=b);
}

float floabs(float x)
{
    return x>0 ? x : -x;
}


void resetviewport(void)
{
    #ifdef GL
	glViewport( 0, 0, SDL_GetVideoSurface()->w,SDL_GetVideoSurface()->h );
    #endif
}

void viewmatrix()
{
    #ifdef GL
	glFrustum(-1,1,-1,1,znear,zfar);
	gluLookAt(cam.x,cam.y,cam.z,look.x,look.y,look.z,0,1,0);
	glRotated(cr.x,1,0,0);
	glRotated(cr.y,0,1,0);
	glRotated(cr.z,0,0,1);
    #endif
}
void resetmatrices(void)
{
    #ifdef GL
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();
    #endif
}


void gle(void)
{
    #ifdef GL
	GLenum gl_error;
	gl_error = glGetError( );
	if( gl_error != GL_NO_ERROR )
	{
	    if(gl_error==GL_STACK_UNDERFLOW)
		logit("QUACK QUACK QUACK, UNDERFLOVING STACK\n");
	    else if(gl_error==GL_STACK_OVERFLOW)
		logit("QUACK QUACK QUACK, OVERFLOVING STACK\n");
	    else if(gl_error==GL_INVALID_OPERATION)
		logit("INVALID OPERATION, PATIENT EXPLODED\n");
	    else if(gl_error==GL_INVALID_VALUE)
		logit("GL_INVALID_VALUE");
	    else
		logit("OpenGL error: 0x%X\n", gl_error );
	}
    #endif
}
void sdle(void)
{
	char* sdl_error;
	sdl_error = SDL_GetError( );
	if( sdl_error[0] != '\0' )
	{
		logit("testgl: SDL error '%s'\n", sdl_error);
		SDL_ClearError();
	}
}


struct obj;
vector<obj *> objects;
obj * active;


struct obj:public Serializable
{
    SAVE(obj)
    {
	save(t)
	save(r)
	save(spin)
	save(s)
    }
    LOAD
    {
	load(t)
	load(r)
	load(spin)
	load(s)
    }
    unsigned int runtime_id;
    static unsigned int idcounter;
    int dirty;
    double alpha;
    v3d t,r,spin,s;
    vector <obj*>objects;
    obj()
    {
	t.x=t.y=t.z=r.x=r.y=r.z=spin.x=spin.y=spin.z=dirty=0;
	s.x=s.y=s.z=1;
	alpha=1;
	runtime_id=idcounter++;
    }
    virtual void activate(){
	active=this;
	logit("activating %u", this);
    }
    virtual void picked(int up, int b,vector<int>&v,int x,int y)
    {
	activate();
	logit("activating, up=%i, button=%i, v.size=%u, x=%i. y=%i",up,b,v.size(),x,y);
    }
    virtual void draw(int picking){logit("drawing nothing, picking: %i",picking);};
    virtual int getDirty(){return dirty;}
    virtual void setClean(){dirty=0;}
    void translate_and_draw(int picking)
    {
	#ifdef GL
	    glPushMatrix();
	    glTranslated(t.x,t.y,t.z);
	    glRotated(r.x,1,0,0);
	    glRotated(r.y,0,1,0);
	    glRotated(r.z,0,0,1);
	    glScalef(s.x,s.y,s.z);
	#endif
	if(picking==2)
	    glColor4f(0,0,1,0.04);
	else if(1==picking)
	    glColor4f(1,1,1,1);
	//else define it yourself
	gle();
	draw(picking);
	gle();
	/*picking == 2 == visualizing picking
	picking == 1 == picking*/
	#ifdef GL
	    glPopMatrix();
	#endif
    }
    void physics(int t)
    {
	r.x += spin.x*t;
	r.y += spin.y*t;
	r.z += spin.z*t;
    }
    virtual void keyp(int key,int uni,int mod)
    {                                      
	logit("default keyp handler, %i %i %i",key,uni,mod);
    }
    ~obj(){if(active==this)active=0;}
};
unsigned int obj::idcounter=0;

#ifdef has_pixel_city
#include "lemon-pixel-city.c"
#endif

struct Settingz: public Serializable
{
    SAVE(Settingz)
    {       
	save(line_antialiasing)
	save(givehelp)
	save(lv)
	vsave(w)
	vsave(h)
	vsave(znear)
	vsave(zfar)
	vsave(cam)
	vsave(look)
	vsave(cr)
#ifdef has_pixel_city
	vsave(pixel_city_ini)
#endif
    }
    LOAD
    {
	load(line_antialiasing)
	load(givehelp)
	load(lv)
	vload(w)
	vload(h)
	vload(znear)
	vload(zfar)
	vload(cam)
	vload(look)
	vload(cr)
#ifdef has_pixel_city
	vload(pixel_city_ini)
#endif
    }
    int32_t line_antialiasing;
    int32_t givehelp;
    double lv;//glLineWidth
    Settingz()
    {
	line_antialiasing=0;
	givehelp=1;
	lv=1;
    }
}settingz;


#include "terminal.cpp"
#include "face.cpp"
#include "logger.cpp"
logger * loggerface;


void slogit(const char * iFormat, ...)
{
    char* s=(char*)malloc(101);
    va_list argp;
    va_start(argp, iFormat);
    vsnprintf(s,101,iFormat, argp);
    s[100]=0;
    va_end(argp);
    if(loggerface)
	loggerface->slogit(s);
}
void slogit(string iFormat, ...)
{
    char* s=(char*)malloc(101);
    va_list argp;
    va_start(argp, iFormat);
    vsnprintf(s,101,iFormat.c_str(), argp);
    s[100]=0;
    va_end(argp);
    if(loggerface)
	loggerface->slogit(s);
}

void clogit(const char * iFormat, ...)
{
    char* s=(char*)malloc(101);
    va_list argp;
    va_start(argp, iFormat);
    vsnprintf(s,101,iFormat, argp);
    s[100]=0;
    va_end(argp);
    if(loggerface)
	loggerface->logit(s);
}
void clogit(string iFormat, ...)
{
    char* s=(char*)malloc(101);
    va_list argp;
    va_start(argp, iFormat);
    vsnprintf(s,101,iFormat.c_str(), argp);
    s[100]=0;
    va_end(argp);
    if(loggerface)
	loggerface->logit(s);
}

void logit(const char * iFormat, ...)
{
    char* s=(char*)malloc(10101010);
    va_list argp;
    va_start(argp, iFormat);
    vsnprintf(s,10101010,iFormat, argp);
    s[10101009]=0;
    va_end(argp);
    if(loggerface)
	loggerface->logit(s);
    else
	printf("%s\n",s);
    free(s);
}

void logit(string iFormat, ...)
{
    char* s=(char*)malloc(10101010);
    va_list argp;
    va_start(argp, iFormat);
    vsnprintf(s,10101010,iFormat.c_str(), argp);
    s[10101009]=0;
    va_end(argp);
    if(loggerface)
	loggerface->logit(s);
    else
	printf("%s\n",s);
    free(s);
}



float maxvol;


#include "demos/atlantis/atlantis.c"
#include "demos/flipflop.c"
#ifdef GL
#include "spectrum_analyzer.cpp"
#ifdef nerve
#include "nerverot.cpp"
#endif
#endif
//#include <string>
//#include "mplayer.cpp"
//#include "compositing.cpp"
#ifdef fontwatcher
#include <sys/inotify.h>
#include "fontwatcher.cpp"
#endif
//#include "listdir.c"
//#include "buttons.cpp"
#include "ggg.cpp"

void erase(obj * o)
{
    if(active==o)active=0;
    for(unsigned int i=0;i<objects.size();i++)
	if(objects.at(i)==o)
	    objects.erase(objects.begin()+i);
}
void erase(int i)
{
    if(active==objects.at(i))active=0;
    objects.erase(objects.begin()+i);
}

SDL_Rect *SDLRect(Uint16 x,Uint16 y,Uint16 w,Uint16 h)
{
    static SDL_Rect xx ;
    xx.x=x;
    xx.y=y;
    xx.w=w;
    xx.h=h;
    return &xx;
}


#define loado(y,x) if(!strcmp(n , #x )) {\
if(online&&hasid) {\
 for_each_object\
  if(o->runtime_id==runtimeid)\
   if(dynamic_cast< x*>(o))\
    o->load_members(doc[i]); \
 endf }\
else\
 {  obj*o;y=dynamic_cast< x*>(o=new x); o->load_members(doc[i]); objects.push_back(o); }}

void loadobjects(int online=0)
{
    std::ifstream fin(wrld);
    YAML::Parser parser(fin);
    YAML::Node doc;
    while(parser.GetNextDocument(doc)) {
        for(unsigned int i=0;i<doc.size();i++) {
            string nn;
            doc[i]["Class"]>>nn;
            const char *n=nn.c_str();
            unsigned int runtimeid=0;
            int hasid=0;
            if(online)
        	try
        	{
	    	    runtimeid = doc[i]["runtime_id"];
	    	    hasid=1;
	    	}
	    	catch(YAML::Exception){}
            obj * d;//dummy
	    loado(d,face)
	    #ifdef has_atlantis
            loado(d,atlantis)
	    #endif
	    #ifdef has_pixel_city
	    loado(d,pixel_city)
	    #endif
	    loado(loggerface,logger)
	    loado(d,flipflop)
	    loado(d,nerverot)
	    loado(d,spectrum_analyzer)
	    loado(d,ggg)
        }
    }
    if(objects.size())
	active = objects.back();
}
void loadsettingz()
{
    std::ifstream fin(wrld);
    YAML::Parser parser(fin);
    YAML::Node doc;
    while(parser.GetNextDocument(doc)) {
        for(unsigned int i=0;i<doc.size();i++) {
            string nn;
            doc[i]["Class"]>>nn;
            const char *n=nn.c_str();
	    if(!strcmp(n, "Settingz"))
		doc[i]>>settingz;
        }
    }
}

void saveobjects(int online=0)
{
    ofstream fout(wrld);
    Emitter out;
    out << BeginSeq;
    for_each_object
	if(online)
	{
	    out << YAML::BeginMap;
	    std::string my_class_name = o->class_name();
	    if (my_class_name.size()>0) {
	      out << YAML::Key << CLASS_TAG << YAML::Value << my_class_name;
	    }
	    out<<Key<<"runtime_id"<<Value<<o->runtime_id;
	    o->emit_members(out);
		out << YAML::EndMap;
	}
	else
	    out << *o;
    endf
    out << settingz;
    out << EndSeq;
    fout << out.c_str();
    clogit("%s",out.c_str());
}


#ifdef GL
#include <limits>

void pick(int up, int button, int x, int y)
{         
    logit("picking objects...");
    y=h-y;
    GLuint fuf[500];
    GLint viewport[4];
    glGetIntegerv (GL_VIEWPORT, viewport);
    glSelectBuffer(500, fuf);
    glRenderMode (GL_SELECT);
    glMatrixMode (GL_MODELVIEW);
    glPushMatrix ();
    glLoadIdentity ();
    gluPickMatrix (x,y,10,10, viewport);
    viewmatrix();
    glInitNames();
    glPushName(-1);
    for_each_object
        glLoadName(i);
	o->translate_and_draw(1);
    endf
    glPopMatrix();
    unsigned int i,j, k;
    GLuint minz = std::numeric_limits<unsigned int>::max() ;
    int nearest=-1;
    unsigned int numhits = glRenderMode(GL_RENDER);
    logit("%i hits", numhits);
    vector<vector<int> > hits;
    for(i=0,k=0;i<numhits;i++)
    {
	GLuint numnames=fuf[k++];
	logit("%i names", numnames);
	logit("%d minz", fuf[k]);
	logit("%d maxz", fuf[k+1]);
	if(fuf[k]<minz)
	{
	    logit("%u < %u, nearest = %i", fuf[k],minz,i);
	    nearest = i;
	    minz = fuf[k];
	}
	k+=2;
	vector<int> v;
	hits.push_back(v);
	for(j=0;j<numnames;j++)
	{
	    hits[hits.size()-1].push_back(fuf[k]);
	    k++;
	}
    }
    if(nearest != -1)
    {
        if(hits.at(nearest).size())
        {
	    obj *o=objects.at(hits.at(nearest)[0]);
	    hits.at(nearest).erase(hits.at(nearest).begin());
	    //lets send the name stack, without the first hit, which identifies object
	    o->picked(up, button,hits.at(nearest),x,y);
	}
    }

}
void vispick()
{
    glMatrixMode (GL_MODELVIEW);
    glPushMatrix ();
    glLoadIdentity ();
    viewmatrix();

    for_each_object
	o->translate_and_draw(2);
    endf
    glPopMatrix();
}
                          
#endif

Uint32 TimerCallback(Uint32 interval, void *param)
{
	SDL_Event e;
	e.type=SDL_USEREVENT;
	e.user.code=CODE_TIMER;
	e.user.data1=param;
	SDL_PushEvent(&e);
	return interval;
}

void updatelinesmooth()
{
    #ifdef GL
	if(settingz.line_antialiasing){
	    logit("antialiasing");
	    glEnable(GL_LINE_SMOOTH);
	}
	else
	{
	    logit("not antialiasing");
    	    glDisable(GL_LINE_SMOOTH);
    	}
    #endif
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
	if(o->getDirty()||o->spin.x||o->spin.y||o->spin.z)
	{//cout<<o->class_name()<<endl;
	    return 1;
	}
    endf
    return 0;
}
void  nothing_dirty()
{
    for_each_object
	o->setClean();
    endf
}

void showhelp(bool unfold)
{
	if(settingz.givehelp)
	{	
	    glPushMatrix();
	    glLoadIdentity();
    	    glOrtho(0,1200,1200,0,0,1);
	    if(!unfold)
	    
		draw_text("\npress right ctrl for more fun...");
	    else
		draw_text("\nnow press h to hide help");
	    glPopMatrix();
	}
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


void process_event(SDL_Event event)
{
	switch( event.type )
	{
	    case SDL_MOUSEMOTION:
		lastmousemoved=SDL_GetTicks();
		mousejustmoved=1;
	        break;
	    case SDL_KEYDOWN:
	    {
		int key=event.key.keysym.sym;
		int uni=event.key.keysym.unicode;
		int mod=event.key.keysym.mod;

		if(escaped||(mod&KMOD_RCTRL)||(key==SDLK_RCTRL))
		{
		    escaped=0;
		    switch (key)
		    {
			case SDLK_TAB:
			    for_each_object
				if(active==o)
			        {
				    if(++i==objects.size())
				        i=0;
				    objects.at(i)->activate();
				    break;
				}
			    endf
			    if(!active&&objects.size()) objects.at(0)->activate();
			    break;
			case SDLK_f:
			    if(mod&KMOD_RSHIFT)
			    {
			        if(dynamic_cast< face*>(active))
			        {
				    face *f = dynamic_cast< face*>(active);
				    rote_vt_resize(f->t,28,160);
				}
			    }
			    else
				gofullscreen=1;
			    break;
			case SDLK_s:
			    saveobjects(1);
			    break;
			case SDLK_l:
			    loadobjects(1);
			    break;
			case SDLK_r:
		    	    loadfont(fnfl);
		    	    break;
			case SDLK_p:
			    #ifdef libpng
			        saveScreenshot();
			    #endif
			    break;
			case SDLK_n:
			    objects.push_back(new face);
			    active=objects.at(objects.size()-1);
			    break;
			#ifdef GL
			    case SDLK_t:
				if(dynamic_cast< face*>(active))
				{
				    face *f = dynamic_cast< face*>(active);
				    int step=1;
				    int c=70;
				    f->obj::t.x=-step;
				    rote_vt_resize(f->t,28,c);
				    objects.push_back(new face(step,0,c,28));
				}
				break;
			    case SDLK_LEFT:
				if(active)
				    active->spin.z+=0.03;
				break;             
			    case SDLK_RIGHT:
				if(active)
				    active->spin.z-=0.03;
				break;             
			    case SDLK_v:
				cr.y+=3.4;
				break;             
			    case SDLK_c:
				cr.y-=3.4;
				break;
			    case SDLK_EQUALS:
				if(active)
				{
				    active->alpha+=0.05;
				    if(active->alpha>1) active->alpha=1;
				    break;
				}
				break;
			    case SDLK_MINUS:
				if(active)
				{
				    active->alpha-=0.05;
				    if(active->alpha<0) active->alpha=0;
				    break;
				}
				break;
			#endif
			case SDLK_h:
			    settingz.givehelp=!settingz.givehelp;
			    break;
			case SDLK_RCTRL:
			    escaped=1;
			    break;
			default:
			    if(active)
				active->keyp(key,uni,mod|KMOD_RCTRL);//escaped
		    }
		}
		else if(active)
		    active->keyp(key,uni,mod);
		break;
	    }
	    case SDL_QUIT:
	        done = 1;
	        break;
	    case SDL_VIDEOEXPOSE:
		dirty=1;
		break;
	    case SDL_VIDEORESIZE:
		w=event.resize.w;h=event.resize.h;
                logit("videoresize %ix%i", w,h);
		dirty=1;
		s=SDL_SetVideoMode( w,h, bpp, s->flags);
		resetviewport();
		dirty=1;
		break;
	    case SDL_USEREVENT:
		if(event.user.code==CODE_QUIT)
		{
		    for_each_face
			if (f->t == event.user.data1)
			    erase(i);
		    endf endf
		    dirty=1;
		}
		else if(event.user.code==CODE_FNFLCHANGED)
		    loadfont(fnfl);
		else if(event.user.code==CODE_DATA)
		{
		    //do nothing. be happy that we broke out of WaitEvent and could redraw ourselves
		}
		break;
	}
}


void draw()
{
    #ifdef GL
	glLoadIdentity();
        viewmatrix();
        if(SDL_GetMouseState(0,0)||afterglow)
    	    vispick();
    	gle();
    #else
	SDL_FillRect    ( s, NULL, SDL_MapRGB( s->format, 0,0,0) );
    #endif
    for_each_object
	o->translate_and_draw(0);
    endf
    gle();

    #ifdef GL
	showhelp(escaped||k[SDLK_RCTRL]);
	SDL_GL_SwapBuffers( );
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    #else
	SDL_UpdateRect(s,0,0,0,0);
    #endif
    gle();
}


void lemon (void)
{
    bpp=8;
    done = 0;
    dirty=1;
    cam.z=10;
    cam.x=cam.y=0;
    gofullscreen=0;
    mousejustmoved=0;
    escaped = 0;
    loadsettingz();
    #ifdef GL
	s=initsdl(w,h,&bpp,SDL_OPENGL
    #else
	s=initsdl(w,h,&bpp,0
    #endif
    );
    SDL_EnableUNICODE(1);
    SDL_InitSubSystem( SDL_INIT_TIMER);
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY/2, SDL_DEFAULT_REPEAT_INTERVAL*2);
    loadfont(fnfl);
    loadcolors();
    #ifdef GL
	gle();
	resetviewport();
        resetmatrices();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glClearColor( 0.0, 0.0, 0.0, 0.0 );
	updatelinesmooth();
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    #endif
    loadobjects();
//    objects.push_back(new nerverot);
    if(!objects.size())
    {
    	objects.push_back(loggerface=new logger(-8,0,0,0,70,0));
	#ifdef GL
//	    objects.push_back(new spectrum_analyzer);
//	    for(int i=0;i<16;i++)
//		objects.push_back(new nerverot(-10.0f+20.0f/16.0f*(float)i,0,0,i));
//	    objects.push_back(new flipflop);
	objects.push_back(new spectrum_analyzer);

	#endif
	objects.push_back(new face("bash"));
#ifdef has_atlantis
        objects.push_back(new atlantis);
#endif
        objects.push_back(active=new ggg);

//	objects.push_back(active=new face("bash",1.0,0.0,3.0,0.0,90.0,0.0));
//	objects.push_back(active=new face("bash",0.0,0.0,6.0,0,180.0,0.0));
//	objects.push_back(active=new face("bash",-1.0,0.0,3.0,0.0,270.0,0.0));
//	objects.push_back(new fontwatcher);
	
    }
    maxvol=0;
    int norm=0;
    afterglow=1;
    int lastmousemoved=SDL_GetTicks();
    int lastphysics=SDL_GetTicks();
    while( !done )
    {
	SDL_TimerID x=0;
	lockterms();
	k=SDL_GetKeyState(NULL);
	if(dirty||anything_dirty())
	{
	    draw();
	    nothing_dirty();
    	    dirty=0;
	    if(anything_dirty())
		x= SDL_AddTimer(5, TimerCallback, 0);
	}
	unlockterms();
	//logit("---------unlocked waiting\n");
    	SDL_Event event;
	if(SDL_WaitEvent( &event ))
	{
	    if(SDL_GetTicks()-lastmousemoved>300)afterglow=0;
	    lockterms();
	    //logit("---------locked goooin %i\n", event.type);
	    if(x)SDL_RemoveTimer(x);x=0;
	    do
	    {
		process_event(event);
	    }
	    while (SDL_PollEvent(&event));
	}
	
	if(mousejustmoved)
	{
		int x,y;
		SDL_GetMouseState(&x,&y);
		pick(0,0,x,y);
		mousejustmoved=0;
	}
	if(gofullscreen)
	{
		if(s->flags & SDL_FULLSCREEN )
		{
		    s=SDL_SetVideoMode( w,h, bpp, (s->flags & ~SDL_FULLSCREEN ));
		    #ifdef LINUX
		    if(norm&1)
			system("wmctrl -r :ACTIVE: -b remove,maximized_horz");
		    if(norm&2)
			system("wmctrl -r :ACTIVE: -b remove,maximized_vert");
		    #endif
		}
		else
		{
		    #ifdef LINUX
		    SDL_SysWMinfo i;
		    SDL_VERSION(&i.version)
		    if(SDL_GetWMInfo(&i))
		    {
			char *c=make_message("xprop -id %u |grep _NET_WM_STATE_MAXIMIZED_VERT", i.info.x11.window);
			norm=!system(c);
			free(c);
			c=make_message("xprop -id %u |grep _NET_WM_STATE_MAXIMIZED_HORZ", i.info.x11.window);
			norm&=(!system(c))<<1;
			free(c);
		    }
		    //system("wmctrl -r :ACTIVE: -b add,maximized_vert,maximized_horz");
		    #endif
		    SDL_Surface *ns;
		    ns=SDL_SetVideoMode( w,h, bpp, (s->flags | SDL_FULLSCREEN ));
		    if(ns)s=ns;
		}
		gofullscreen=0;
	}
	if(afterstart.size())
	{
		v8::Handle<v8::Value>* args = NULL;
		for (unsigned i=0;i<afterstart.size();i++)
			afterstart[i]->Call(afterstart[i],0,args);
		afterstart.clear();
	}
	if(!done)
		unlockterms();
	else
		saveobjects();
	int t= SDL_GetTicks();
	physics(t-lastphysics);
	lastphysics=t;
    }
    objects.clear();
    font.clear();
    SDL_Quit( );
}                      

string tostring(const v8::String::Utf8Value& value) {
  return *value ? string(*value): string("<string conversion failed>");
}


v8::Handle<v8::Value> goCallback(const v8::Arguments& args)
{
    v8::HandleScope scope;

    for_each_object
	if(dynamic_cast<ggg*>(o))
	{
	    ggg*g=dynamic_cast<ggg*>(o);
	    if (args.Length() == 1)
	    {
		v8::Handle<v8::Value> arg = args[0];
		v8::String::Utf8Value value(arg);
		logit(tostring (value));
		g->url=tostring(value);
	    }
	    g->go();
	}
    }
    return v8::Undefined();
}

v8::Handle<v8::Value> LogCallback(const v8::Arguments& args)
{
    if (args.Length() < 1) return v8::Undefined();
    v8::HandleScope scope;
    v8::Handle<v8::Value> arg = args[0];
    v8::String::Utf8Value value(arg);
    cout << tostring (value)<<endl;
    return v8::Undefined();
}

v8::Handle<v8::Value> registerAfterStart(const v8::Arguments& args)
{
    if (args.Length() < 1) return v8::Undefined();
    v8::HandleScope scope;
    v8::Handle<v8::Value> arg = args[0];
    if (!arg->IsFunction())
    {
	cout << "must register a function" <<endl;
	return v8::Undefined();
    }
    v8::Handle<v8::Function> fun = v8::Handle<v8::Function>::Cast(arg);
    afterstart.push_back(v8::Persistent<v8::Function>::New(fun));
    cout << "registered."<<endl;
    return v8::Undefined();
}



#include "getexecname.c"
int main(int argc, char *argv[])
{
	string path = string(needexepath()) + string("../data/");
	string world = "world";
	if(argc==2)
		world = argv[1];

	wrld=path + string(world+".yml");
	fnfl=path + string("l1");
	clfl=path + string("colors");
	dmps=path + string("errordumps/");
	jsv8=path + string("javascript.js");
	
	v8::HandleScope handle_scope;
	v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New();
	global->Set(v8::String::New("go"), v8::FunctionTemplate::New(goCallback));
	global->Set(v8::String::New("log"), v8::FunctionTemplate::New(LogCallback));
	global->Set(v8::String::New("registerafterstart"), v8::FunctionTemplate::New(registerAfterStart));
	v8::Persistent<v8::Context> context = v8::Context::New(NULL, global);
	v8::Context::Scope context_scope(context);
	ifstream in(jsv8);
	if(!in.fail())
	{
		string iin, line;
		while(in.good())
		{
		    getline(in, line);
		    iin += line;
		}
		v8::Handle<v8::String> source = v8::String::New(iin.c_str());
		v8::Handle<v8::Script> script = v8::Script::Compile(source);
		v8::Handle<v8::Value> result = script->Run();
	}
	
	lemon();
	context.Dispose();
	return 0;
}


