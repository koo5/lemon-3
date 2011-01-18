class logger:public terminal
{
    public:
    SAVE(logger)
    {
	YAML_EMIT_PARENT_MEMBERS(out,terminal)
    }
    LOAD
    {
	YAML_LOAD_PARENT_MEMBERS(doc,terminal)
    }
   logger(double x, double y, double z, double a, double b, double c)
    {
	obj::t.x=x;obj::t.y=y;obj::t.z=z;
	r.x=a;r.y=b;r.z=c;
	t = rote_vt_create(100,30);
	t->cursorhidden=1;//
    }
   logger()
    {
	t = rote_vt_create(100,30);
	t->cursorhidden=1;//
    }
	
	
    ~logger()
    {
	rote_vt_destroy(t);
    }
    
    void logit(char *s)
    {
	rote_vt_write(t, s, strlen(s)+1);
	rote_vt_write(t,"\n",1);
	dirty = 1;
    }
    
    void slogit(char *s)
    {
	if(!strcmp(s,status))
	    dirty = 1;
	updatestatus(s);
    }
};


