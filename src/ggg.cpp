#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <../libs/pstreams-0.7.0/pstream.h>
#include "json.h"
#include "text.cpp"

string char2hex( char dec )
{
    char dig1 = (dec&0xF0)>>4;
    char dig2 = (dec&0x0F);
    if ( 0<= dig1 && dig1<= 9) dig1+=48;    //0,48inascii
    if (10<= dig1 && dig1<=15) dig1+=97-10; //a,97inascii
    if ( 0<= dig2 && dig2<= 9) dig2+=48;
    if (10<= dig2 && dig2<=15) dig2+=97-10;

    string r;
    r.append( &dig1, 1);
    r.append( &dig2, 1);
    return r;
}


string urlencode(const string &c)
{
    //http://www.zedwood.com/article/111/cpp-urlencode-function
    string escaped="";
    int max = c.length();
    for(int i=0; i<max; i++)
    {
        if ( (48 <= c[i] && c[i] <= 57) ||//0-9
             (65 <= c[i] && c[i] <= 90) ||//abc...xyz
             (97 <= c[i] && c[i] <= 122) || //ABC...XYZ
             (c[i]=='~' || c[i]=='_' || c[i]=='-' || c[i]=='.')
        )
        {
            escaped.append( &c[i], 1);
        }
        else
        {
            escaped.append("%");
            escaped.append( char2hex(c[i]) );//converts char 255 to string "ff"
        }
    }
    return escaped;
}

struct gtext:public text
{
    int level;
    string url;
    gtext(int l=0, string u="", string te=""):text(te)
    {
	level=l;
	url=u;
    }
};

struct gggw:public obj
{
    unsigned active,count;
    json_object*root;
    string morph;
    string url;
    json_object *labels;
    stringstream raw;
    text uri;
    vector<gtext*>c;
    gtext * authors;
    gtext * items;
    double down;
    double side;
    SAVE(gggw)
    {
	YAML_EMIT_PARENT_MEMBERS(out,obj)
	save(url)
	save(down)
	save(side)
	save(morph)
    }
    LOAD
    {
    	YAML_LOAD_PARENT_MEMBERS(doc,obj)
    	load(url)
    	load(down)
    	load(side)
    	load(morph)
    	uri.settext(url.c_str());
    }
    gggw(const string uurl="root.cz")
    {
	active=0;
	url=uurl;
	root=0;
	s.x=0.01;
	s.y=-0.03;
	s.z=0.002;
	down=25.5;
	side=85.5;
	uri.settext(url.c_str());
	uri.seteditable(true);
	morph = "http://morph.talis.com/?input=&output=json&data-uri[]=";
    }
    size_t write_raw(void *ptr, size_t size)
    {
	raw.write((char*)ptr, size);
	return size;
    }
    void one_down()
    {
	glTranslatef(0,down,0);
    }
    void one_left()
    {
	glTranslatef(-side,0,0);
    }
    void one_right()
    {
	glTranslatef(side,0,0);
    }
    void draw_item(json_object *o)
    {
	stringstream dummy;
	if(json_object_is_type(o, json_type_boolean))
	{
	    json_object_get_boolean(o)?draw_text("1"):draw_text("0");
	    one_down();
	}
	if(json_object_is_type(o, json_type_double))
	{
	    dummy << json_object_get_double(o);
	    draw_text(dummy.str().c_str());
	    one_down();
	}
	if(json_object_is_type(o, json_type_int))
	{
	    dummy << json_object_get_int(o);
	    draw_text(dummy.str().c_str());
	    one_down();
	}
	if(json_object_is_type(o, json_type_object))
	{
	    one_right();
	    json_object_object_foreach(o, key,val)
	    {
		draw_text(key);
		one_down();
		draw_item(val);
		one_down();
	    }
	    one_left();
	}
	if(json_object_is_type(o, json_type_array))
	{
	    one_right();
	    int k=json_object_array_length(o);
	    for(int i = 0; i< k; i++)
	    {
		draw_item(json_object_array_get_idx(o, i));
		one_down();
	    }
	    one_left();
	}
	if(json_object_is_type(o, json_type_string))
	{
	    draw_text(json_object_get_string(o));
	    one_down();
	}
    }
    const char * label(char * key)
    {
	if(labels)
	{
	    json_object*j=json_object_object_get(labels, key);
	    return j?json_object_get_string(j):0;
	}
	return key;
    }
    void draw_(gtext *t,unsigned*pos)
    {
	glPushMatrix();
	if(active==(*pos))glScalef(1.4,1,1);
	t->draw();
	glPopMatrix();
	one_down();
	one_right();
	(*pos)++;
	for(unsigned i=0;i<t->objects.size();i++)
	{
	    draw_(dynamic_cast<gtext*>(t->objects[i]),pos);
	}
	one_left();
    }
    void draw(int)
    {
	glPushMatrix();
	if(!active)glScalef(1.4,1,1);
	uri.draw();
	glPopMatrix();
	glTranslatef(0,100,0);
	unsigned pos=1;
	if(root&&!is_error(root))
	{
	    for(unsigned i = 0; i < c.size(); i++)//subject
	    {
		draw_(c[i],&pos);
		one_down();
	    }
	}
	else
	    draw_text(raw.str().c_str());
    }
    gtext * get_authors()
    {
	    if(!authors)
	    {
		    c.push_back(authors = new gtext);
		    c.back()->settext("Authors");
	    }
	    return authors;
    }
    gtext * get_items()
    {
	    if(!items)
	    {
		    c.push_back(items = new gtext);
		    c.back()->settext("Items");
	    }
	    return items;
    }
		    
    void parse_subject(string skey, json_object*s)
    {
	gtext * parent = 0;
	string title = skey;
	vector<bool>vis;
	json_object_object_foreach(s, key,val)//predicate
	{
	    bool hide = false;
	    const char * l = label(key);
	    if(l)
	    {
		string value=string(json_object_get_string(json_object_object_get(json_object_array_get_idx(val, 0), "value")));
		if(!strcmp(l, "Type")&&(!value.compare("http://poshrdf.org/ns/mf#Author")))
		{
		    hide = true;
		    parent = get_authors();
		}
		if(!strcmp(l, "Type")&&(!value.compare("http://poshrdf.org/ns/mf#Item")))
		{
		    hide = true;
		    parent = get_items();
		}
		if(!strcmp(l, "Title"))
		{
		    title = value;
		    hide=true;
		}
		if(!strcmp(l, "Format"))
		    hide=true;
	    }
	    vis.push_back(!hide);
	}

	gtext * x;
	if(parent)
	    parent->objects.push_back(x=new gtext(1,skey,!title.size()?skey:title));
	else
	    c.push_back(x=new gtext(0,skey,!title.size()?skey:title));
	count++;
    
	int i=0;
	{
	json_object_object_foreach(s, key2,val2)//predicate
	{
	    if(vis[i])
	    {
		gtext*g;
		x->objects.push_back(g=new gtext(x->level+1, key2, label(key2)?string(label(key2)):key2));
		count++;
		int k=json_object_array_length(val2);
		for(int j = 0; j< k; j++)
		{
		    string uri;
		    string type= string((json_object_get_string(json_object_object_get(json_object_array_get_idx(val2, j), "type"))));
		    string value=string((json_object_get_string(json_object_object_get(json_object_array_get_idx(val2, j), "value"))));
		    if(!type.compare("uri"))
			uri=value;
		    g->objects.push_back(new gtext(x->level+1, uri, value));
		    count++;
		}
	    }
	    i++;
	}
	}
    }
    void parse()
    {
	    count = 0;
	    authors=items=0;
	    json_object * data=json_object_object_get(root, "data");
	    labels=json_object_object_get(root, "labels");
	    if(!data || !labels)	data=root;
	    json_object_object_foreach(data, key,val)//subject
		parse_subject(key, val);
    }
};


static size_t write_raw_callback(void *ptr, size_t size, size_t nmemb, void *data)
{
    return static_cast<gggw*>(data)->write_raw(ptr, size * nmemb);
}

struct ggg:public gggw
{
    SAVE(ggg)
    {
	YAML_EMIT_PARENT_MEMBERS(out,gggw)
    }
    LOAD
    {
    	YAML_LOAD_PARENT_MEMBERS(doc,gggw)
    }
    ggg(){}
    void go()
    {
	CURL *curl_handle;
	curl_global_init(CURL_GLOBAL_ALL);
	curl_handle = curl_easy_init();
	string x = morph + urlencode(url);
	curl_easy_setopt(curl_handle, CURLOPT_URL, x.c_str());
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, &write_raw_callback);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)this);
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "lemon-ggg/0.01");
	raw.clear();
	raw.str("");
	curl_easy_perform(curl_handle);
	curl_easy_cleanup(curl_handle);
	curl_global_cleanup();

	root = json_tokener_parse(raw.str().c_str());
	if(root&&!is_error(root))
	{
	    parse();
	    return;
	}
	
	ostringstream f;
	f << dmps << runtime_id << ".html";
	string fn(f.str());
	ofstream o(fn);
	o << raw.str();
	o.close();
	string cmd;
	cmd = "links -dump "+fn+" || w3m -dump " +fn+ " || lynx -dump -force_html " +fn+ " || cat " +fn;
	redi::ipstream in(cmd);
	string str;
	raw.clear();
	raw.str("");
	string y;
	while (getline(in, y)) {
        raw << y <<endl;
	}

    }
    void keyp(int key,int uni,int mod)
    {
	if(key==SDLK_UP)
	{
	    if(active > 0) active--;
	}
	else if(key==SDLK_DOWN)
	{
	    if(active < count-1) active++;
	}
	else if(!active)
	    uri.keyp(key, uni, mod);
	
	dirty=1;
    }
};


