struct v3d:public Serializable
{
    SAVE(v3d)
    {
	save(x)
	save(y)
	save(z)
    }
    LOAD
    {
	load(x)
	load(y)
	load(z)
    }
    double x,y,z;
    v3d()
    {
    
    }
};
