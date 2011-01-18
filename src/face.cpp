

#define _mutexV( d ) {if(SDL_mutexV( d )) {logit("SDL_mutexV!");}}
#define _mutexP( d ) {if(SDL_mutexP( d )) {logit("SDL_mutexP!");}}
#define CODE_DATA 3
#define CODE_TIMER 0
#define CODE_QUIT 1
#define CODE_FNFLCHANGED 2
#define for_each_face for_each_object if (dynamic_cast< face*>(o)){face*f=dynamic_cast< face*>(o);




typedef struct
{
    RoteTerm* t;
    SDL_mutex *lock;
    SDL_Thread *thr;
}
moomoo;

int update_terminal(void *data)
{
	while(20)
	{
	    moomoo * d = (moomoo *)data;
	    char buf[123456];
	    int br=-1;
	    //logit("UNLOCKED SELECT\n");
	    rote_vt_update_thready(buf, 123456, &br, d->t);
	    //logit("*end SELECT, locking %i*\n", d->lock);
	    _mutexP(d->lock);
	    //logit("LOCKED\n");
	    if (br>0)
	    {
	        //logit("*locked injecting\n");
	        rote_vt_inject(d->t, buf, br);
	        //logit("*locked injected\n");
	        SDL_Event e;
	        e.type=SDL_USEREVENT;
	        e.user.code=CODE_DATA;
	        e.user.data1=d->t;
	        SDL_PushEvent(&e);
	        //wake up main thread.
	    }
	    if(!d->t->childpid)
	    {	//our child died
	        SDL_Event e;
	        e.type=SDL_USEREVENT;
	        e.user.code=CODE_QUIT;
	        e.user.data1=d->t;
	        SDL_PushEvent(&e);
	        _mutexV(d->lock);
	        return 666;
	    }
	    _mutexV(d->lock);
	}
	return 69;
}



struct face:public terminal
{
    moomoo upd_t_data;
    SAVE(face)
    {
	YAML_EMIT_PARENT_MEMBERS(out,terminal)
    }
    LOAD
    {
    	YAML_LOAD_PARENT_MEMBERS(doc,terminal)
    }
    face(char* run, double x, double y, double z, double a, double b, double c)
    {
	obj::t.x=x;obj::t.y=y;obj::t.z=z;
	r.x=a;r.y=b;r.z=c;
	add_terminal(run);
    }
    face(double x, double y, double z, double a, double b, double c)
    {
	obj::t.x=x;obj::t.y=y;obj::t.z=z;
	r.x=a;r.y=b;r.z=c;
	add_terminal("bash");
    }
    face(float x, float y, int c, int r)
    {
	add_term(c,r,"bash");
	obj::t.x=x;
	obj::t.y=y;
    }
    face(const char*run="bash")
    {
	add_terminal(run);
    }
    void type(const char * x)
    {
	while(*x)
	{
	    rote_vt_keypress(t,*x);
	    x++;
	}
    }
    ~face()
    {
	SDL_DestroyMutex(upd_t_data.lock);
	SDL_KillThread(upd_t_data.thr);
	logit(" terminal destroyed.");
    }

    void add_term(int c,int r,const char *run)
    {
	logit("adding terminal");
	t = rote_vt_create(r,c);
	rote_vt_forkpty((RoteTerm*) t, run, "LD_PRELOAD", "");
	upd_t_data.lock=SDL_CreateMutex();
	upd_t_data.t=t;
	logit("upd_t_data.lock=%i",upd_t_data.lock);
	upd_t_data.thr=SDL_CreateThread(update_terminal, (void *)&upd_t_data);
    }
    void add_terminal(const char *run)
    {
	add_term(SDL_GetVideoSurface()->w/26*3,SDL_GetVideoSurface()->h/26,run);
    }
    
	

    void lock()
    {
    	_mutexP(upd_t_data.lock);
    }
    void unlock()
    {
	_mutexV(upd_t_data.lock);
    }
};

void lockterms(void)
{
	for_each_face
	    f->lock();
	endf endf
}
void unlockterms(void)
{
	//logit("unlocking terms");
	for_each_face
	    f->unlock();
	endf endf
	//logit("done");
}
