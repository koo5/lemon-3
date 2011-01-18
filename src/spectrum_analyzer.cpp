    struct spectrum_analyzer:public obj
    {
	SAVE(spectrum_analyzer)
	{
	    YAML_EMIT_PARENT_MEMBERS(out,obj)
	    save(rotx)
	    save(roty)
	    save(rotz)
	    save(rx)
	    save(ry)
	    save(rz)
	}
	LOAD
	{
	    YAML_LOAD_PARENT_MEMBERS(doc,obj)
	    try{
	    load(rotx)
	    load(roty)
	    load(rotz)
	    load(rx)
	    load(ry)
	    load(rz)
	    }catch(Exception){}
	}
	static GLfloat heights[16][16];
	GLfloat  scale;
	float rx,ry,rz;
	float rotx,roty,rotz;
	int getDirty(){return 1;}
	spectrum_analyzer()
	{
	    scale = 1.0 / log(256.0);
	    alpha=0.05;
	    rz=ry=rz=rotx=roty=rotz=0;
	    dirty = 16;
	}
	void picked(int up, int  b,vector<int>&v,int x, int y)
	{
	    if(!up)
	    {
		if(b==SDL_BUTTON_LEFT)
		    ry-=2;
		else if (b==SDL_BUTTON_RIGHT)
		    ry+=2;
		else if(b==SDL_BUTTON_MIDDLE)
		    rx=0;
		else
		    obj::picked(up,b,v,x,y);
	    }
	}
	void keyp(int key,int uni,int mod)
	{
	    switch(key)
	    {
		case SDLK_UP:
		    rx-=0.2;
		    break;
		case SDLK_DOWN:
		    rx+=0.2;
		    break;
		case SDLK_LEFT:
		    rz-=0.2;
		    break;
		case SDLK_RIGHT:
		    rz+=0.2;
		    break;
		default:
		    obj::keyp(key,uni,mod);
	    }
        }
	void draw_rectangle(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2)
	{
	    if(y1 == y2)
	    {
		glVertex3f(x1, y1, z1);
		glVertex3f(x2, y1, z1);
		glVertex3f(x2, y2, z2);
		glVertex3f(x2, y2, z2);
		glVertex3f(x1, y2, z2);
		glVertex3f(x1, y1, z1);
	    }
	    else
	    {
		glVertex3f(x1, y1, z1);
		glVertex3f(x2, y1, z2);
		glVertex3f(x2, y2, z2);
		glVertex3f(x2, y2, z2);
		glVertex3f(x1, y2, z1);
		glVertex3f(x1, y1, z1);
	    }
	}
	void draw_bar(GLfloat x_offset, GLfloat z_offset, GLfloat height, GLfloat red, GLfloat green, GLfloat blue )
	{
	    GLfloat width = 0.1;
	    glColor4f(red,green,blue,alpha);
	    draw_rectangle(x_offset, height, z_offset, x_offset + width, height, z_offset + 0.1);
	    draw_rectangle(x_offset, 0, z_offset, x_offset + width, 0, z_offset + 0.1);
	
	    glColor4f(0.5 * red, 0.5 * green, 0.5 * blue,alpha);
	    draw_rectangle(x_offset, 0.0, z_offset + 0.1, x_offset + width, height, z_offset + 0.1);
	    draw_rectangle(x_offset, 0.0, z_offset, x_offset + width, height, z_offset );

	    glColor4f(0.25 * red, 0.25 * green, 0.25 * blue,alpha);
	    draw_rectangle(x_offset, 0.0, z_offset , x_offset, height, z_offset + 0.1);	
	    draw_rectangle(x_offset + width, 0.0, z_offset , x_offset + width, height, z_offset + 0.1);
	}
	
	int oglspectrum_gen_heights(int16_t data[2][256])
	{
	    int i,c;
	    int y;
	    GLfloat val;
	    
	    int xscale[] = {0, 1, 2, 3, 5, 7, 10, 14, 20, 28, 40, 54, 74, 101, 137, 187, 255};
	    int noise = 16;
	    for(i = 0; i < 16; i++)
	    {
		for(c = xscale[i], y = 0; c < xscale[i + 1]; c++)
		{
		    if(data[0][c] > y)
		    y = data[0][c];
		}
		y >>= 7;
		if(y > 0)
		    val = (log(y) * scale);
		else
		{
		    val = 0;
		    noise--;
		}
		spectrum_analyzer::heights[0][i] = val;
	    }
	    return noise;
	}
	void setClean(){}
	void draw(int picking)
	{
	    int i,y;
    	    for(y = 15; y > 0; y--)
	    {
	        for(i = 0; i < 16; i++)
	        {
		    spectrum_analyzer::heights[y][i] = heights[y - 1][i];
		}
	    }

	    FILE *f;
	    int noise=0;
	    if((f=fopen("/tmp/somefunnyname", "r")))
	    {
	    	int16_t buf[2][256];
		fread(buf,256,2,f);
    		fclose(f);
		remove("/tmp/somefunnyname");
		noise = oglspectrum_gen_heights(buf);
	    }
	    else
	        for(i = 0; i < 16; i++)
		    spectrum_analyzer::heights[0][i] = 0;
	    
	    if (!noise && dirty) 
		dirty--;
	    else 
		dirty=16;

	    int x;
	    GLfloat x_offset, z_offset, r_base, b_base;
	    glPushMatrix();
	    rotx+=rx;
	    roty+=ry;
	    rotz+=rz;
	    glRotatef(rotx,1,0,0);
	    glRotatef(roty,0,1,0);
	    glRotatef(rotz,0,0,1);
    	    glBegin(GL_TRIANGLES);
    	    float oldalpha=alpha;
    	    if(picking==1)
    		alpha=1;
	    for(y = 0; y < 16; y++)
	    {
		z_offset = -1.6 + ((15 - y) * 0.2);
		b_base = y * (1.0 / 15);
		r_base = 1.0 - b_base;
		for(x = 0; x < 16; x++)
		{
		    x_offset = -1.6 + (x * 0.2);
		    draw_bar(x_offset, z_offset, spectrum_analyzer::heights[y][x], r_base - (x * (r_base / 15.0)), x * (1.0 / 15), b_base);
		}
	    }
	    alpha=oldalpha;
	    glEnd();
	    glPopMatrix();
	    
	    float sx=fmax(fmax(fmax(fmax(spectrum_analyzer::heights[0][0],spectrum_analyzer::heights[0][1]),spectrum_analyzer::heights[0][2]),spectrum_analyzer::heights[0][3]),spectrum_analyzer::heights[0][4]);
	    float sy=fmax(sx,fmax(fmax(fmax(fmax(spectrum_analyzer::heights[0][5],spectrum_analyzer::heights[0][6]),spectrum_analyzer::heights[0][7]),spectrum_analyzer::heights[0][8]),spectrum_analyzer::heights[0][9]));
	    maxvol=fmax(sy,fmax(fmax(fmax(fmax(fmax(spectrum_analyzer::heights[0][10],spectrum_analyzer::heights[0][11]),spectrum_analyzer::heights[0][12]),spectrum_analyzer::heights[0][13]),spectrum_analyzer::heights[0][14]),spectrum_analyzer::heights[0][15]));
//	    logit("maxvol:%f",maxvol);

	}
    };
    GLfloat spectrum_analyzer::heights[16][16];
