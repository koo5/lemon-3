/*
 * cam.cpp
 *
 *  Created on: Jun 8, 2012
 *      Author: kook
 */

#include "cam.h"

camera::camera() 
{	
	now.self.set(0,0,-10);
	now.look.set(0,0,0);
}
	int camera::set() {
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
			printf("true\n");
		}
		return true;
	}
	void camera::focus(v3d f)
	{
		src=now;
		dst.look=f;
		dst.self=f;
		dst.self.z-=1.2;
		depart = SDL_GetTicks();
		arrive = depart + 1000;
		still=false;
	}

