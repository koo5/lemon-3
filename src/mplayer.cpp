class mplayer:public obj
{
    public:
    SAVE(mplayer){
    	YAML_EMIT_PARENT_MEMBERS(out,obj)
    }
    LOAD{
    	YAML_LOAD_PARENT_MEMBERS(doc,obj)
    }
    int pos;
    int twist;
    vector<string>list;    
    void draw()
    {
	
    }
    void keyp(int key,int uni,int mod)
    {
	switch(key)
	{
	    case SDLK_t:
	    twist=!twist;
	    break;
	    case SDLK_UP:
	    pos--;
	    break;
	    case SDLK_DOWN:
	    pos++;
	    break;
	    default:
	    obj::keyp(key,uni,mod);
	}
    }
    void loadlist()
    {
	
    }
};