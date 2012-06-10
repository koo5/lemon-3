/*
 * cam.cpp
 *
 *  Created on: Jun 8, 2012
 *      Author: kook
 */


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

struct cam_pos {
	v3d self,look;
	cam_pos()
	{
	}
	~cam_pos()
	{
	}
};


class camera {
public:
	Uint32 depart,arrive;
	cam_pos now;
	cam_pos src;//= { {0,0,-3}, {0,0,0}};
	cam_pos dst;//= { {0,0,-1.1}, {0,0,0}};
	bool still;

	camera()
	{	
		now.self.set(0,0,-10);
		now.look.set(0,0,0);
	}
	
	int set() {
		if (still) return false;
		Uint32 now_time = SDL_GetTicks();
		if (now_time > arrive)
		{
			still = true;
			now = dst;
		}
		else
		{
			double p = float(arrive-now_time)/(float(depart)-float(arrive));
			now.look.x = dst.look.x + (dst.look.x - src.look.x)*p;
			now.look.y = dst.look.y + (dst.look.y - src.look.y)*p;
			now.look.z = dst.look.z + (dst.look.z - src.look.z)*p;
			now.self.x = dst.self.x + (dst.self.x - src.self.x)*p;
			now.self.y = dst.self.y + (dst.self.y - src.self.y)*p;
			now.self.z = dst.self.z + (dst.self.z - src.self.z)*p;
//			printf("true\n");
		}
		return true;
	}
	void move(int milis=400)
	{
		depart = SDL_GetTicks();
		arrive = depart + milis;
		still=false;
		src=now;
	}
	void focus(v3d f)
	{
		dst.look=f;
		dst.self=f;
		dst.self.z-=1.2;
		move(700);
	}

};
