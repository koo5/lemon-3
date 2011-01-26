#include "../rote-3/wtf.h"

struct text: public obj
{
	vector<unsigned>t;
	unsigned curpos;
	bool editable;
	void draw()
	{
		glBegin(GL_LINE_STRIP);
		xy lok;lok.x=0;lok.y=0;
		for(unsigned i = 0; i < t.size(); i++)
		{
			if(editable&&((i==curpos-1)||(curpos==i)))
				setcolor(1,0,0,1);
			else
				setcolor(0,0,1,1);

			lok = drawchar(lok, t[i]);
		}
		glEnd();
	}
	void settext(const char * s)
	{
		t.clear();
		wtfdecoder x;
		memset(&x,0,sizeof(x));
		for(unsigned i = 0; i < strlen(s); i++)
		{
			if(wtf(*(s+i), &x))
				t.push_back(x.etff);
		}
	}
	void seteditable(bool e)
	{
	    editable = e;
	}
	text(string te="")
	{
	    settext(te.c_str());
	    curpos=0;
	    editable=false;
	}
	void keyp(int key, int , int )
	{
	    if((key==SDLK_LEFT) && (curpos > 0))
		curpos--;
	    if((key==SDLK_RIGHT) && (curpos < t.size()))
		curpos++;
	}
};
