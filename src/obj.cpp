struct obj;
vector<obj *> objects;

obj * active;


struct obj
{
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
//	logit("activating %u", this);
    }
//    void picked(int up, int b,vector<int>&v, int x, int y)
    void picked(int, int,vector<int>&, int, int)
    {
	activate();
//	logit("activating, up=%i, button=%i, v.size=%u, x=%i. y=%i",up,b,v.size(),x,y);
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
