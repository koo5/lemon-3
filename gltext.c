#include <string>
#include <vector>
#include <fstream>
#include <iostream>

struct glyph
{
    int left, right;
    string s;
    glyph(string ss)
    {
	s=ss;
    }
};
vector<glyph> font;
const char *nullglyph="dzdptuzpznxnxpzp";
double r,g,b,a;
void setcolor(double rr,double gg,double bb,double aa)
{
    r=rr;g=gg;b=bb;a=aa;
}

void tokenize(const string& str, vector<string>& tokens, const string& delimiters = "+")
{
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);
    while (string::npos != pos || string::npos != lastPos)
    {
	// Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}
string prepare(string y)
{
    vector<string>tokens;
    tokenize(y,tokens,"#");
    string ret;
    if(!tokens.size())
	return ret;
    y=tokens.at(0);
    if(y.find('+')==string::npos)
	return y;
    else
    {
        tokens.clear();
	tokenize(y,tokens);
	for(unsigned int i=0;i<tokens.size();i++)
	{
	    char* endptr=0;
	    unsigned int b=strtol(tokens.at(i).c_str(),&endptr,10);
	    if(!endptr)
    	    {
    		if(font.size()<b)
    		    ret.append(font.at(b).s);
	    }
	    else
		ret.append(tokens.at(i));
	}
	return prepare(ret);
    }
}


 
void _spillit(xy lok, const char*x, float offset)
{
    int first=1;
    #ifdef SDLD
	have_first=0;
    #endif
    int xdot;
    int ydot;
    while ((*x)&&(*(x+1)))
    {
        if (((*x)==' ')&&(*(x+1)==' ')) // 2 spaces
        {
            x++;
            x++;
	    #ifdef SDLD
        	have_first=0;
	    #endif
            first=1;
        }
        else
        {
            xdot=*x;
            x++;
            ydot=*x;
            xdot-='a';
            ydot-='a';
            if (xdot<0) // capital
                xdot+=58;
            if (ydot<0)
                ydot+=58;

            x++;
            if(first)
    	    {
    		first=0;
    		glColor4d(0,0,0,0);
                glVertex2f(offset+lok.x+xdot,offset+lok.y+ydot);
                glColor4d(r,g,b,a);
            }

            if ((!*x)||(' '==(*x)))/* last dot*/
            {
                glVertex2f(offset+lok.x+xdot,offset+lok.y+ydot);
    		glColor4d(0,0,0,0);
                glVertex2f(offset+lok.x+xdot,offset+lok.y+ydot);
        	if(!*x)
        	{
            	    return;
        	}	
            }
            else // just ordinary dot
            {
                glVertex2f(offset+lok.x+xdot,offset+lok.y+ydot);
            }
        }
    }
}
void spillit(xy lok, const char *x)
{
    _spillit(lok,x,0);
    _spillit(lok,x,0);
}


xy drawchar(xy lok, unsigned int i)
{
    xy nlok;
    nlok=lok;

    if (i<font.size())
    {
	if(font[i].s.length())
	{
    	    lok.x-=font[i].left;
    	    lok.x+=4;
    	    spillit(lok,font[i].s.c_str());
    	    nlok.x=lok.x;
    	    nlok.x+=4;
    	    nlok.x+=font[i].right-font[i].left;
	    return nlok;
	}
    }
    spillit(lok,"ZZZ~~~~ZZZ~~Z~~Z");
    clogit("unknown char: %u",i);
    nlok.x+=26;
    return nlok;
}
void drawmonospace(xy lok, unsigned int i)
{
    xy nlok;
    nlok.x=nlok.y=0;
    if (i<font.size())
    {
	if(font[i].s.length())
	{
    	    spillit(lok,font[i].s.c_str());
	    return;
	}
    }
    spillit(lok,"ZZZ~~~~ZZZ~~Z~~Z");
    logit("unknown char: %u",i);
    return;
}
void flush_text()
{

}


void draw_text(const char *a)
{
  if(!a)return;
  xy lok;
  lok.x=0;
  lok.y=0;
  setcolor(0,1,0,1);
  glBegin(GL_LINE_STRIP);
  do 
  {
	lok=drawchar(lok,*a);
	if (*a==10)
	{
	    lok.x=0;
	    lok.y=lok.y+30;
	    glEnd();
	    glBegin(GL_LINE_STRIP);
	}
	if (!*a)
	    break;
	a++;
  }
  while(1);
  glEnd();
}

void sizes(glyph&g)
{
    if(strcmp(g.s.c_str(),"  "))
    {
	g.left=26;
	g.right=0;
	for(unsigned int i=0; i<g.s.length();i+=2)
	    if(g.s[i]!=(unsigned char)' ')
	    {
		if(g.left>g.s.c_str()[i]-'a')
		    g.left=g.s.c_str()[i]-'a';
		if(g.right<g.s.c_str()[i]-'a')
		    g.right=g.s.c_str()[i]-'a';
	    }
    }
    else
    {
	g.left=0;
	g.right=13;
    }
}
    
    
void loadfont(string fln)
{
    cout << fln<<endl;
    ifstream ff(fln,std::ios::in);
    if (!ff)
    {
        cout << "cant load font from "<<fln<<endl;
        return;
    }
    font.clear();
    font.push_back(glyph(nullglyph));
    string x;
    while(getline(ff,x))
	font.push_back(glyph(x));
    for(unsigned int i=0;i<font.size();i++)
	font.at(i).s=prepare(font.at(i).s);
    for(unsigned int i=0;i<font.size();i++)
	sizes(font.at(i));
}
