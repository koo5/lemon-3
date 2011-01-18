int fontwatcherthread(void *data);

class fontwatcher:public obj
{
    public:
    SAVE(fontwatcher)
    {
	YAML_EMIT_PARENT_MEMBERS(out,obj)
    }
    LOAD
    {
	YAML_LOAD_PARENT_MEMBERS(doc,obj)
    }
    SDL_Thread* t;
    int i,f;
    float osize;
    float grow,size;
    void activate()
    {
	grow=0.1;
	obj::activate();
    }
    fontwatcher()
    {
	size=osize=0;
	obj::t.x=0.3;
	obj::t.y=-0.3;
	grow=0;
	alpha=0.1;
	if((i=inotify_init())==-1) cout << "no tengo descriptor\n";
	if((f=inotify_add_watch(i,fnfl,IN_MODIFY))==-1) cout << "no tengo file\n";;
	cout << f <<endl;
	t=SDL_CreateThread(&fontwatcherthread, (void*)this);
    }
    ~fontwatcher()
    {
        SDL_KillThread(t);
        close(i);
    }
    void draw( int picking)
    {
	#ifdef GL
	    size+=grow;
	    if(size>osize)
		grow-=0.003;
	    else 
		size=osize;
	    glColor4f(1,0,0,alpha);
	    if (size)gluSphere(gluNewQuadric(),size,10,10);
	#endif
    }
    void picked(int b,vector<int>&v, int x, int y)
    {
	loadfont(fnfl);
    }
};
int fontwatcherthread(void *data)
{
    while(1)
    {
	char buf[1000];
//	cout << reinterpret_cast<fontwatcher*>(data)->i << endl;
	read(reinterpret_cast<fontwatcher*>(data)->i,&buf,1000);
	//todo:check it wasnt "ignore"
	reinterpret_cast<fontwatcher*>(data)->grow=0.1;
        SDL_Event e;
        e.type=SDL_USEREVENT;
        e.user.code=CODE_FNFLCHANGED;
        SDL_PushEvent(&e);
        cout << "reloading " << endl;
    }
}
