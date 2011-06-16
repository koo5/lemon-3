typedef 
struct 
{
    unsigned char r,g,b;
}
color;
color colors[2][16];

void loadcolors(void)
{
    int i,useless;
    FILE * fp = fopen(clfl.c_str(),"r");
    if (fp == NULL)
    {
        printf("cant load 'colors'\n");
        return;
    }
    useless=fread(&colors,3,16,fp);
    fclose(fp);
    for (i=0;i<16;i++)
    {
	colors[1][i].r=0;
	colors[1][i].g=0;
	colors[1][i].b=colors[0][i].r+colors[0][i].b+colors[0][i].g/3;
    }

}



struct terminal:public obj
{
    double spacing;
    double bgalpha;
    int fontnum;
    char *status;
    RoteTerm *t;
    Uint32 last_resize;
    int lastxresize;
    int lastyresize;
    int oldcrow, oldccol;
    Uint32 lastrotor;
    double rotor;
    unsigned int selstartx,selstarty,selendx,selendy;
    int getDirty(){return dirty||t->dirty;}
    void setClean(){t->dirty=0;obj::setClean();}
    SAVE(terminal)
    {
	YAML_EMIT_PARENT_MEMBERS(out,obj)
	int cols, rows;
	cols=t->cols;
	rows=t->rows;
	vsave(cols)
	vsave(rows)
	vsave(fontnum)
	vsave(bgalpha)
	vsave(spacing)
    }
    LOAD
    {
    	YAML_LOAD_PARENT_MEMBERS(doc,obj)
    	int cols=100;
    	int rows=100;
    	vload(cols)
    	vload(rows)
    	vload(fontnum)
    	vload(bgalpha)
    	vload(spacing)
        cout << cols<<" "<<rows<<endl;
	rote_vt_resize(t,rows,cols);
	rote_vt_clear(t);
    }
    void init()
    {
	bgalpha = 0.3;
	fontnum = 0;
	status=0;
	last_resize=lastxresize=lastyresize=0;
	oldcrow=oldccol=-1;
	lastrotor=rotor=0;
	selstartx=selstarty=selendx=selendy=-1;
	s.x=0.002;
	s.y=-0.005;
	s.z=0.002;
	obj::t.x=obj::t.y=obj::t.z=0;
	r.x=r.y=r.z=0;
    }
    terminal()
    {
	init();
    }
    void type(const char * x)
    {
	while(*x)
	{
	    rote_vt_keypress(t,*x);
	    x++;
	}
    }
    ~terminal()
    {
	rote_vt_destroy(t);
    }
    void resizooo(int xx, int yy, Uint8* duck)
    {
        int up=duck[SDLK_HOME]||(yy==-1);
        int dw=duck[SDLK_END]||(yy==1);
        int lf=duck[SDLK_PAGEUP]||(xx==-1);
        int ri=duck[SDLK_PAGEDOWN]||(xx==1);
        if(up)
	    yy=-1;
	if(dw)
	    yy=1;
        if(lf)
	    xx=-1;
        if(ri)
	    xx=1;
        if((!xx+lastxresize||!yy+lastyresize)||(SDL_GetTicks()-last_resize>100)||!last_resize)
        {
    	    rote_vt_resize(t, t->rows+yy,t->cols+xx);
    	    if (fontnum == 1)
    		//crash me baby one more time
    		rote_vt_clear(t);
	    last_resize=SDL_GetTicks();
	    printstatus("%i x %i", t->rows, t->cols);
	}
	lastxresize=xx;
	lastyresize=yy;
	
    }

    void printstatus(const char * iFormat, ...)
    {
	char *s = (char*)malloc(99);
	va_list argp;
	va_start(argp, iFormat);
	vsnprintf(s,99,iFormat, argp);
	s[98]=0;
	va_end(argp);
	updatestatus(s);
    }


    void updatestatus(char *s)
    {
	if(status)free(status);
	status=s;
	dirty=1;
    }

    //FAIL
    void ghost()
    {
	glPushMatrix();
	glTranslated(obj::t.x,obj::t.y,obj::t.z);
	glRotated(r.x,1,0,0);
	glRotated(r.y,0,1,0);
	glRotated(r.z,0,0,1);
	glScalef(s.x,s.y,s.z);
        glTranslatef((t->cols-t->ccol)*26/s.x,(t->rows-t->crow)*26/s.y,0);
	glScalef(30,30,1);
	draw_terminal(1,0.2*alpha);
	glPopMatrix();
    }
	

    void clipin(int noes, int sel)
    {
	char * r=rotoclipin(sel);
	char *s=r;
	if(s)
	{
	    while(*r)
	    {
		if((noes && (*r)!=10 && (*r)!=13 ) || !noes) rote_vt_keypress(t,*r);
		r++;
	    }
	    free(s);
	}
    }
    void keyp(int key,int uni,int mod)
    {
    	if((mod&KMOD_RSHIFT)&&(key==SDLK_INSERT))
    	{
    	    if(mod&KMOD_RCTRL)
    		clipin(0,0);  //clipboard
    	    else
		clipin(0,1);  //selection buffer
	}
	if(mod&KMOD_RCTRL)
	{	
	    Uint8*k = SDL_GetKeyState(0);
	    switch(key)
	    {
		case SDLK_DELETE:
		case SDLK_END:
		    resizooo(0,1,k);
		    break;
		case SDLK_INSERT:
		case SDLK_HOME:
		    resizooo(0,-1,k);
		    break;
		case SDLK_PAGEUP:
		    resizooo(-1,0,k);
		    break;
		case SDLK_PAGEDOWN:
		    resizooo(1,0,k);
		    break;
		case SDLK_o:
		    fontnum++;
		    if (fontnum > 1)
			fontnum = 0;
		    dirty = 1;
		    break;
		case SDLK_F1:
		    spacing -=0.01;
		    dirty = 1;
		    break;
		case SDLK_F2:
		    spacing +=0.01;
		    dirty = 1;
		    break;
		case SDLK_a:
		    bgalpha+= 0.05;
		    if(bgalpha>1)bgalpha=1;
		    dirty=1;
		    break;
		case SDLK_b:
		    bgalpha-= 0.05;
		    if(bgalpha<0)bgalpha=0;
		    dirty=1;
		    break;
	    }
	    return;
	}
	else if(mod&KMOD_RSHIFT&&(key==SDLK_HOME||key==SDLK_END||key==SDLK_PAGEUP||key==SDLK_PAGEDOWN))
	{
	    if(key==SDLK_PAGEUP)
		t->scroll+=9;
	    if(key==SDLK_PAGEDOWN)
	    	t->scroll=max(t->scroll-9,0);
	    if(key==SDLK_END)
		t->scroll=0;
	    if(key==SDLK_HOME)
		t->scroll=t->logl;
	    dirty=1;
	}
	else
	{
	    if((key!=SDLK_LALT)&&(key!=SDLK_RSHIFT))
	    {
	        t->scroll=0;
	        t->stoppedscrollback=0;
	    }
    	    sdlkeys(t,key,uni,mod);
	}
    }
    
    void draw(int picking)
    {
	if(picking)
	{
	    glBegin(GL_QUADS);
	    glVertex2f(-t->cols/2*26,-t->rows/2*26);
	    glVertex2f(-t->cols/2*26,+t->rows/2*26);
	    glVertex2f(+t->cols/2*26,+t->rows/2*26);
	    glVertex2f(+t->cols/2*26,-t->rows/2*26);
	    glEnd();
	}
	else
	{
	    //if((active==this)&&!t->cursorhidden)ghost();
	    if (ftglfont->Error())
		fontnum = 0;
    	    draw_terminal();
    	}
    }

    void do_color(int theme,int attr,float a)
    {
	int c=(attr);//0-15
	if (fontnum == 0)
	    setcolor(colors[theme][c].r/255.0,colors[theme][c].g/255.0,colors[theme][c].b/255.0,a);
	else
	    glColor4f(colors[theme][c].r/255.0,colors[theme][c].g/255.0,colors[theme][c].b/255.0,a);
    }

    void set_color(int theme,int attr,float a)
    {
	int c=(attr);//0-15
	setcolor(colors[theme][c].r/255.0,colors[theme][c].g/255.0,colors[theme][c].b/255.0,a);
    }

    void tex_letter(xy lok, unsigned ch)
    {
        glPushMatrix();
        glTranslatef(lok.x,lok.y+22,0);
	glScalef(1, -1, 1);
	if (ch)
	    ftglfont->Render("&ch",1);
	glPopMatrix();
    }
    
    void le_bg(int s, int theme, double alpha)
    {
	xy lok;
    	for (int i=0; i<t->rows; i++)
	{
	    lok.y=(s+(i-t->rows/2.0))*26;
	    for (int j=0; j<t->cols; j++)
	    {
		lok.x=(j-t->cols/2.0)*26;
		if((j>0)&&((ROTE_ATTR_XBG(t->cells[i][j-1].attr))!=(ROTE_ATTR_XBG(t->cells[i][j].attr))))
		{
			set_color(theme,ROTE_ATTR_XBG(t->cells[i][j].attr),bgalpha*alpha);
		        _spillit(lok,"aaa{",-0.5);
		}
		if((j<t->cols-1)&&((ROTE_ATTR_XBG(t->cells[i][j+1].attr))!=(ROTE_ATTR_XBG(t->cells[i][j].attr))))
		{
			set_color(theme,ROTE_ATTR_XBG(t->cells[i][j].attr),bgalpha*alpha);
		        _spillit(lok,"{a{{",-0.5);
		}
		if((i<t->rows-1)&&((ROTE_ATTR_XBG(t->cells[i+1][j].attr))!=(ROTE_ATTR_XBG(t->cells[i][j].attr))))
		{
			set_color(theme,ROTE_ATTR_XBG(t->cells[i][j].attr),bgalpha*alpha);
		        _spillit(lok,"a{{{",-0.5);
		}
		if((i>0)&&((ROTE_ATTR_XBG(t->cells[i-1][j].attr))!=(ROTE_ATTR_XBG(t->cells[i][j].attr))))
		{
			set_color(theme,ROTE_ATTR_XBG(t->cells[i][j].attr),bgalpha*alpha);
		        _spillit(lok,"aa{a",-0.5);
		}
	    }
	}
    }
    
    void le_log(int s, int theme, double alpha)
    {
	if(t->log)
	{
	    xy lok;
	    int color = -1;
	    for (int i=t->logl-s;i<(int)t->logl;i++)
	    {
	        if(!t->log[i])break;
	        lok.y=((i-t->rows/2.0)-t->logl+s)*26;
	        if(t->logstart) 
	        {
	    	    lok.y-=100;
		    lok.x-=100;
		}
		for(int j=0;j<(int)t->log[i][0].ch;j++)//len
	        {
		    lok.x=(j-t->cols/2.0)*26;
		    int newcolor = ROTE_ATTR_XFG(t->log[i][j+1].attr);
		    if (newcolor != color)
		    {
			do_color(theme,newcolor,alpha);
			color = newcolor;
		    }
		    unsigned int char_ = t->log[i][j+1].ch;
		    if (fontnum == 0)
	    		drawmonospace(lok,char_);
	    	    else
	    		tex_letter(lok, char_);
		}	
	    }
        }
    }
    
    void le_halo(xy lok, double alpha)
    {
	glPushMatrix();
        glTranslatef(lok.x+13,lok.y+13,0);
	glRotatef(SDL_GetTicks()/42,0,1,0);
	int i;
	int steps=10;
	for (i=0; i<360; i+=steps)
	{
	    glRotatef(steps, 0,0,1);
	    glPushMatrix();
	    glTranslatef(0,100,0);
	    glBegin(GL_QUADS);
	    glColor4f(1,1,0,0.2*alpha);
	    glVertex2f(-1,0);
	    glVertex2f(1,0);
	    glVertex2f(2,10);
	    glVertex2f(-2,10);
	    glEnd();
	    glPopMatrix();
	}
	glPopMatrix();
    }
    
    void le_square(xy lok, double alpha)
    {
    	glPushMatrix();
	glTranslatef(lok.x+13,lok.y+13,0);
	glRotatef(SDL_GetTicks()/100,0,0,1);
    	glBegin(GL_LINE_LOOP);
        int w=13;
        glColor4f(1,0,0,alpha);
	glVertex2f(-w,-w);
	glVertex2f(+w,-w);
	glVertex2f(+w,+w);
	glVertex2f(-w,+w);
	glEnd();
	glPopMatrix();
    }
    
    void le_cursor(int s, double alpha)
    {
	xy lok;
	lok.y=(s+(t->crow-t->rows/2.0))*26;
	lok.x=   (t->ccol-t->cols/2.0) *26;

	le_halo(lok, alpha);
	le_square(lok, alpha);
    }
    
    void le_letter(xy lok, int theme, double alpha, unsigned int ch, int attr)
    {
    	do_color(theme,attr,alpha);
	
	if (fontnum == 0)
	    drawmonospace(lok,ch);
	else
	    tex_letter(lok, ch);
    }

    void le_body(int s, int theme, double alpha)
    {
	xy lok;
	for (int i=0; i<t->rows; i++)
	{
	    lok.y=(s+(i-t->rows/2.0))*26;
	    for (int j=0; j<t->cols; j++)
	    {
		lok.x=(j-t->cols/2.0)*26;
		unsigned int ch = t->cells[i][j].ch;
		int attr = ROTE_ATTR_XFG(t->cells[i][j].attr);
		if((t->cells[i][j].ch!=32) && (attr != ROTE_ATTR_XBG(t->cells[i][j].attr)))
	    	    le_letter(lok, theme, alpha, ch, attr);
	    }
	}
    }

    void le_flush(vector<unsigned> s, xy lok)
    {
	glPushMatrix();
        glTranslatef(lok.x,lok.y+22,0);
	glScalef(1, -1, 1);
	int renderMode = FTGL::RENDER_FRONT | FTGL::RENDER_BACK;
	ftglfont->Render(ftw(s).c_str(), -1, FTPoint(), FTPoint(spacing,0), renderMode);
	glPopMatrix();
    }
    
    void le_body_lines(int s, int theme, double alpha)
    {
	xy lok;
	vector<unsigned> le_line;
	int oldattr = -1;
	for (int i=0; i<t->rows; i++)
	{
	    le_line.clear();
	    lok.y=(s+(i-t->rows/2.0))*26;
	    lok.x=(0-t->cols/2.0)*26;
	    for (int j=0; j<t->cols; j++)
	    {
		
		unsigned int ch = t->cells[i][j].ch;
		if (ch != 32)
		{
		    int attr = ROTE_ATTR_XFG(t->cells[i][j].attr);
		    if (oldattr != attr)
		    {
			if(oldattr != -1)
			{
			    do_color(theme,oldattr,alpha);    
			    le_flush(le_line, lok);
			    le_line.clear();
			    lok.x = (j-t->cols/2.0)*26;
			}
			oldattr = attr;
		    }
		}
		le_line.push_back(ch);
	    }
	    if(oldattr != -1)
		do_color(theme,oldattr,alpha);    
	    le_flush(le_line, lok);
	}
    }

    void le_tail(int s)
    {
    	if(status)
	{
	    glPushMatrix();
	    glTranslatef(-t->cols/2.0*26,(s+((1+t->rows)/2.0))*26,0);
	    draw_text(status);
	    glPopMatrix();
	}
    }

    void draw_terminal(int theme=0, double alpha=1)
    {
	int s=min(t->scroll,t->logl); //cant be scrolled past log length
	
    	if (fontnum == 0)
	    glBegin(GL_LINE_STRIP);  //preapare the line-cannon
        
        le_log(s, theme, alpha);
        
	if (fontnum != 0)
		glBegin(GL_LINE_STRIP);
		
	le_bg(s, theme, alpha);

	if (fontnum != 0)
		glEnd();

	if (fontnum == 0)
	    le_body(s, theme, alpha);
	else
	    le_body_lines(s, theme, alpha);

	if (fontnum == 0)
	    glEnd();

	le_cursor(s, alpha);

	le_tail(s);
    }
};

