#ifdef GL
struct button
{
    string name,content;
    button(string n, string c)
    {
	name=n;content=c;
    }
};
class buttons:public obj
{
    public:
    SAVE(buttons)
    {
	YAML_EMIT_PARENT_MEMBERS(out,obj)
    }
    LOAD{YAML_LOAD_PARENT_MEMBERS(doc,obj)}
    vector<button> buttonz;
    buttons()
    {
	logit("buttons:");
	listdir(btns, &add_button,this);
	overlay=1;
	s.x=s.y=s.z=1/300;
    }
    void show_button(int x, int y, button *b, int picking)
    {
//	cout << b->content << endl;
	glPushMatrix();
	glTranslatef(x,y,0);
	if(picking)
	    glBegin(GL_QUADS);
	else
	{
	    glBegin(GL_LINE_LOOP);
	    glColor4f(1,1,0,alpha);
	}
	int w=b->content.length()*13+13;
	glVertex2f(0,0);
	glVertex2f(w,0);
	glVertex2f(w,26);
	glVertex2f(0,26);
	glEnd();
	if(!picking)
	{
	    setcolor(1,1,0,alpha);
	    draw_text(b->content.c_str());
	}
	glPopMatrix();
    }
    void draw(int picking)
    {
	int n=buttonz.size();
	int y=0;
///	cout <<"+++"<<endl;
	while(n)
	{
		if(picking)glLoadName(n-1);
		show_button(-100,y+=100, &buttonz.at(--n),picking);
	}
//	glLoadName(-1);
    }
    void picked(int up, int b, vector<int> &v, int x, int y)
    {
	if(v.size()&&!up&&b&&is face*>(active))
	{
	    logit("%i %i",x,y);
	    as face*>(active)->type(buttonz.at(v.at(0)).content.c_str());
	    erase(this);
	}
    }
};
void add_button(char *path, char *justname, void *data)
{
	char *b=GetFileIntoCharPointer1(path);
	if(b)
	{
		logit(justname);
		reinterpret_cast<buttons*>(data)->buttonz.push_back(button(justname, b));
	}
}


#endif 
