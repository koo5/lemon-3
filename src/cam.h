/*
 * cam.h
 *
 *  Created on: Jun 8, 2012
 *      Author: kook
 */

#ifndef CAM_H_
#define CAM_H_

struct v3d
{
    double x,y,z;
    void set(double xx,double yy, double zz)
    {
	x=xx;
	y=yy;
	z=zz;
    }
};

struct cam_pos {
	v3d self,look;
};


class camera {
public:
	Uint32 depart,arrive;
	cam_pos now;
	cam_pos src= { {0,0,-3}, {0,0,0}};
	cam_pos dst= { {0,0,-1.1}, {0,0,0}};
	bool still;
	camera();
	
	void focus(v3d f);
	int set(void);
};

#endif /* CAM_H_ */
