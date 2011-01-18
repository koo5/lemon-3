Uint32 dotcolor=255;
int have_first=0;
int gltx,glty;
#define GL_LINE_STRIP 0



void glEnd()
{
}

void glBegin(int aha)
{
    have_first=0;
}

inline void glColor4d(double r,double g,double b,double a)
{
    if (a==0)
	dotcolor=0;
    else
	dotcolor=SDL_MapRGBA( s->format ,r*255.0f,g*255.0f,b*255.0f,a*255.0f);
}
inline void glColor4f(double r,double g,double b,double a)
{
    if (a==0)
	dotcolor=0;
    else
	dotcolor=SDL_MapRGBA( s->format ,r*255.0f,g*255.0f,b*255.0f,a*255.0f);
}

void  glVertex2f(float x,float y)
{
    static Uint32 firstc;
    static Sint16 firstx, firsty;
    if (y<1)y=1;    if (x<1)x=1;
    if(y>s->h-2)y=s->h-2;
    if(x>s->w-2)x=s->w-2;
    if(have_first)
    {
	if(dotcolor&&firstc)
	    Draw_Line(s,firstx,firsty,x,y,dotcolor);
	firstx=x;	firsty=y;
	firstc=dotcolor;
    }
    else
    {
	have_first=1;
	firstx=x;
	firsty=y;
	firstc=dotcolor;
    }
}
