struct key
{
    float left,right;
    int sdl_constant;
    std::string text;
};

struct osk: public obj
{
    vector<vector<key> >keys;
    

    void draw_key(key k)
    {

    }

    void draw_picking()
    {
    
    }
    
    void draw_keys()
    {
    	unsigned int x,y;
    	key k;
    	for(y=0;y<keys.size();y++)
        	for(x=0;x<keys[y].size();x++)
        		draw_key(keys[y][x]);
    }
    
    void draw_home_row()
    {
    
    }
    
    void draw(int picking)
    {
	if(picking)
	    draw_picking();
	draw_home_row();
	draw_keys();
    }
};
