
struct v3d
{
    double x,y,z;

    v3d(double xx=0,double yy=0, double zz=0)
    {
	x=xx;y=yy;z=zz;
    }
    void set(double xx,double yy, double zz)
    {
	x=xx;
	y=yy;
	z=zz;
    }
};

struct v4d:public v3d
{
	Uint32 depart,arrive;
	bool moving;
	v3d src,dst;

	void interpolate() {
		if (!moving) return;
		Uint32 now_time = SDL_GetTicks();
//		printf("%i %i\n", arrive, now_time);
		if (now_time > arrive)
		{
			moving = false;
			x=dst.x;
			y=dst.y;
			z=dst.z;
		}
		else
		{
			double p = float(arrive-now_time)/(float(depart)-float(arrive));
			x = dst.x + (dst.x - src.x)*p;
			y = dst.y + (dst.y - src.y)*p;
			z = dst.z + (dst.z - src.z)*p;
		}
	}
	void move(v3d d,int milis=100)
	{	printf("moving to %lf %lf %lf in %ims\n", d.x,d.y,d.z,milis);
		depart = SDL_GetTicks();
		arrive = depart + milis;
		moving = true;
		src=v3d(x,y,z);
		dst = d;
	}

};


class camera {
public:
	v4d self,look;
	bool moving;

	camera()
	{	
		self.set(0,0,-1.001);
		look.set(0,0,0);
	}
	
	void interpolate() {
		moving = self.moving || look.moving;
		self.interpolate();
		look.interpolate();
	}

	void focus(v3d f)
	{	printf("focusing %lf %lf %lf\n", f.x,f.y,f.z);
		look.move(f,60);
		self.move(v3d(f.x,f.y,f.z-1.001),50);
	}

};
