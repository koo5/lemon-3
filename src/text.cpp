#include "../rote-3/wtf.h"

struct text
{
	vector<unsigned>text;
	void draw()
	{
		setcolor(0,0,1,1);
		glBegin(GL_LINE_STRIP);
		xy lok;lok.x=0;lok.y=0;
		for(unsigned i = 0; i < text.size(); i++)
		{
			lok = drawchar(lok, text[i]);
		}
		glEnd();
	}
	void settext(const char * s)
	{
		text.clear();
		wtfdecoder x;
		memset(&x,0,sizeof(x));
		cout << "hmm" << endl;
		for(unsigned i = 0; i < strlen(s); i++)
		{
			cout << "+" << endl;
			if(wtf(*(s+i), &x))
			{
				text.push_back(x.etff);
				cout << "..." << endl;
			}
		}
	}
};
