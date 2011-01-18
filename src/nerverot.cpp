#include "demos/nerverot/stolenfromglu"
#include "demos/nerverot/nerverot.c"

    GLfloat fmin(GLfloat a, GLfloat b)
    {
	return a>b?b:a;
    }
    GLfloat fmax(GLfloat a, GLfloat b)
    {
	return a<b?b:a;
    }
    class nerverot:public obj
    {
        public:
        int band;
	SAVE(nerverot)
	{
	    YAML_EMIT_PARENT_MEMBERS(out,obj)
	    save(nerv->please_num)
	    save(band)
	}
	LOAD
	{
	    YAML_LOAD_PARENT_MEMBERS(doc,obj)
	    try
	    {
		load(nerv->please_num)
		load(band)
	    }catch(Exception){}
	}
        void draw(int picking)
        {
	    glPushMatrix();
	    if(band==-2)
	    {
	    
	    }
	    else if(band==-1)
	    {
		glScalef(1+maxvol,1+maxvol,1);
	    }
	    else
		glScalef(1+spectrum_analyzer::heights[0][band],1+spectrum_analyzer::heights[0][band],1+spectrum_analyzer::heights[0][band]);
	    
    	    if(picking)
    		gluSphere(gluNewQuadric(),1,10,10);
    	    else
    	    {
        	nerverot_update(nerv);
        	nerverot_draw(3,nerv,alpha);
    	    }
    	    glPopMatrix();
        }
	nerverot(float x=0,float y=0,float z=0,int b = -1)
	{
	    nerv=nerverot_init(SDL_GetVideoSurface()->w,SDL_GetVideoSurface()->h);
	    t.x=x;t.y=y;t.z=z;
	    alpha=0.5;
	    band=b;
	}
	~nerverot()
	{
	    nerverot_free(nerv);
	}
	void keyp(int key,int uni,int mod)
	{
	    switch(key)
	    {
		case SDLK_LEFT:
		    nerverot_cycledown(nerv);
		    break;
		case SDLK_RIGHT:
		    nerverot_cycleup(nerv);
		    break;
		default:
		    obj::keyp(key,uni,mod);
	    }
	    
	}
	void picked(int up, int b,vector<int>&v,int x, int y)
	{
	    if(!up)
		switch(b)
		{
		    case SDL_BUTTON_LEFT:
			nerverot_cycleup(nerv);
			break;
		    case SDL_BUTTON_RIGHT:
		        nerverot_cycledown(nerv);
		        break;
		    case SDL_BUTTON_MIDDLE:
		    default:
		        obj::picked(up,b,v,x,y);
		}
	}
	int getDirty(){return 1;}
	protected:
        struct nerverotstate *nerv;

    };