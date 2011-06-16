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
		if(curpos>t.size())
		    curpos=t.size();
	}
	string gettext()
	{
	    return ftw(t);
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
	void keyp(int key, int uni, int mod)
	{
	    if((key==SDLK_LEFT) && (curpos > 0))
		curpos--;
	    else if((key==SDLK_RIGHT) && (curpos < t.size()))
		curpos++;
	    else if((key==SDLK_BACKSPACE) && (curpos > 0))
	    {
		t.erase(t.begin()+curpos-1);
		curpos--;
	    }
	    else if((key==SDLK_DELETE) && (curpos < t.size()))
	    {
		t.erase(t.begin()+curpos);
	    }
	    else if((mod&KMOD_RSHIFT)&&(key==SDLK_INSERT))
	    {
		char *ch = rotoclipin(0);
		if(ch)
		{
		    settext(ch);
		    free(ch);
		}
	    }
	    else if(uni)
	    {
		t.insert(t.begin()+curpos, uni);
		curpos++;
	    }
	}
};
