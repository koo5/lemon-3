
#if __GNUC__ > 4 ||(__GNUC__ == 4 && (__GNUC_MINOR__ > 3))

/* atlantis --- Shows moving 3D sea animals */

#if 0
static const char sccsid[] = "@(#)atlantis.c	5.08 2003/04/09 xlockmore";
#endif

/* Copyright (c) E. Lassauge, 1998. */

/*
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies and that
 * both that copyright notice and this permission notice appear in
 * supporting documentation.
 *
 * This file is provided AS IS with no warranties of any kind.  The author
 * shall have no liability with respect to the infringement of copyrights,
 * trade secrets or any patents by this file or any part thereof.  In no
 * event will the author be liable for any lost revenue or profits or
 * other special, indirect and consequential damages.
 *
 * The original code for this mode was written by Mark J. Kilgard
 * as a demo for openGL programming.
 * 
 * Porting it to xlock  was possible by comparing the original Mesa's morph3d 
 * demo with it's ported version to xlock, so thanks for Marcelo F. Vianna 
 * (look at morph3d.c) for his indirect help.
 *
 * Thanks goes also to Brian Paul for making it possible and inexpensive
 * to use OpenGL at home.
 *
 * My e-mail address is lassauge@users.sourceforge.net
 *
 * Eric Lassauge  (May-13-1998)
 *
 * REVISION HISTORY:
 * 
 * Jamie Zawinski, 2-Apr-01:  - The fishies were inside out!  The back faces
 *                              were being drawn, not the front faces.
 *                            - Added a texture to simulate light from the
 *                              surface, like in the SGI version.
 *
 * David A. Bagley - 98/06/17 : Add whalespeed option. Global options to
 *                              initialize local variables are now:
 *                              XLock.atlantis.cycles: 100      ! SharkSpeed
 *                              XLock.atlantis.batchcount: 4    ! SharkNum
 *                              XLock.atlantis.whalespeed: 250  ! WhaleSpeed
 *                              XLock.atlantis.size: 6000       ! SharkSize
 *                              Add random direction for whales/dolphins
 * 
 * E.Lassauge - 98/06/16: Use the following global options to initialize
 *                        local variables :
 *                              XLock.atlantis.delay: 100       ! SharkSpeed
 *                              XLock.atlantis.batchcount: 4    ! SharkNum
 *                              XLock.atlantis.cycles: 250      ! WhaleSpeed
 *                              XLock.atlantis.size: 6000       ! SharkSize
 *                        Add support for -/+ wireframe (t'was so easy to do!)
 *
 * TODO : 
 *        - better handling of sizes and speeds
 *        - test standalone and module modes
 *        - purify it (!)
 */

/* Copyright (c) Mark J. Kilgard, 1994. */

/**
 * (c) Copyright 1993, 1994, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED
 * Permission to use, copy, modify, and distribute this software for
 * any purpose and without fee is hereby granted, provided that the above
 * copyright notice appear in all copies and that both the copyright notice
 * and this permission notice appear in supporting documentation, and that
 * the name of Silicon Graphics, Inc. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission.
 *
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
 * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
 * GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
 * KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
 * LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
 * THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
 * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * US Government Users Restricted Rights
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
 * (c)(1)(ii) of the Rights in Technical Data and Computer Software
 * clause at DFARS 252.227-7013 and/or in similar or successor
 * clauses in the FAR or the DOD or NASA FAR Supplement.
 * Unpublished-- rights reserved under the copyright laws of the
 * United States.  Contractor/manufacturer is Silicon Graphics,
 * Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.
 *
 * OpenGL(TM) is a trademark of Silicon Graphics, Inc.
 */

#include "minixpm.h"
#include "minixpm.c"
#include "sea-texture.xpm"

# include <math.h>
#define RAD 57.295
#include <cstdlib>
#define NRAND(x) rand()%x
#define LRAND rand
#define RRAD 0.01745

struct fishRec {
	float       x, y, z, phi, theta, psi, v;
	float       xt, yt, zt;
	float       htail, vtail;
	float       dtheta;
	int         spurt, attack;
        int         sign;
	int         wire;
};

#include "dolphin.c"
#include "whale.c"
#include "shark.c"

class atlantis: public obj
{
	float       sharkspeed, whalespeed;
	int         sharksize;
	int         wire;
	int ghost;
	Bool        whaledir;
	vector<shark>sharks;
	whale     momWhale;
	whale     babyWhale;
	dolphin     dolph;
        XImage     *texture;	   /* water distortion overlay bits */
        GLfloat texscale;
        GLuint gltexture;
        GLfloat aspect;
        int do_texture;
	int do_gradient;
//	{"-whalespeed num", "speed of whales and the dolphin"},
//	{"-texture",        "whether to introduce water-like distortion"},
//	{"-gradient",       "whether to introduce gradient-filled background"},


void InitFishs()
{
	unsigned int         i;

	for (i = 0; i < sharks.size(); i++) {
		sharks[i].x = 70000.0 + NRAND(sharksize);
		sharks[i].y = NRAND(sharksize);
		sharks[i].z = NRAND(sharksize);
		sharks[i].psi = NRAND(360) - 180.0;
		sharks[i].v = 1.0;
	}

	/* Random whale direction */
	whaledir = LRAND() & 1;

	dolph.x = 30000.0;
	dolph.y = 0.0;
	dolph.z = (float) (sharksize);
	dolph.psi = (whaledir) ? 90.0 : -90.0;
	dolph.theta = 0.0;
	dolph.v = 6.0;

	momWhale.x = 70000.0;
	momWhale.y = 0.0;
	momWhale.z = 0.0;
	momWhale.psi = (whaledir) ? 90.0 : -90.0;
	momWhale.theta = 0.0;
	momWhale.v = 3.0;

	babyWhale.x = 60000.0;
	babyWhale.y = -2000.0;
	babyWhale.z = -2000.0;
	babyWhale.psi = (whaledir) ? 90.0 : -90.0;
	babyWhale.theta = 0.0;
	babyWhale.v = 3.0;
}

void predraw(int picking)
{
	static const float ambient[]        = {0.1, 0.1, 0.1, 1.0};
	static const float diffuse[]        = {1.0, 1.0, 1.0, 1.0};
	static const float position[]       = {0.0, 1.0, 0.0, 0.0};
	static const float mat_shininess[]  = {90.0};
	static const float mat_specular[]   = {0.8, 0.8, 0.8, 1.0};
	static const float mat_diffuse[]    = {0.46, 0.66, 0.795, 1.0};
	static const float mat_ambient[]    = {0.0, 0.1, 0.2, 1.0};
	static const float lmodel_ambient[] = {0.4, 0.4, 0.4, 1.0};
	static const float lmodel_localviewer[] = {0.0};

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glFrontFace(GL_CCW);
        glEnable(GL_CULL_FACE);


    	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(400.0, aspect, 1.0, 2000000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(0,0,0.1);
	if(picking||ghost)return;
	glColor3f(1,1,1);
        if (wire)
          {
            glDisable(GL_DEPTH_TEST);
            glDisable(GL_CULL_FACE);
            glDisable(GL_LIGHTING);
            glDisable(GL_NORMALIZE);
          }
        else
          {
            
            glDepthFunc(GL_LEQUAL);
            
            glEnable(GL_DEPTH_TEST);
            
            glEnable(GL_CULL_FACE);

            glEnable(GL_NORMALIZE);

            glShadeModel(GL_SMOOTH);

            glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
            glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
            glLightfv(GL_LIGHT0, GL_POSITION, position);
            glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
            glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, lmodel_localviewer);
            glEnable(GL_LIGHTING);
            glEnable(GL_LIGHT0);

            glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);

	     if (do_texture)
            {
            glBindTexture(GL_TEXTURE_2D, gltexture);


    	    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
            
            GLfloat s_plane[] = { 1, 0, 0, 0 };
            GLfloat t_plane[] = { 0, 0, 1, 0 };
           
    	    glTexGeni (GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    	    glTexGeni (GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    	    glTexGenfv(GL_S, GL_EYE_PLANE, s_plane);
    	    glTexGenfv(GL_T, GL_EYE_PLANE, t_plane);

    	    glEnable(GL_TEXTURE_GEN_S);
    	    glEnable(GL_TEXTURE_GEN_T);
    	    glEnable(GL_TEXTURE_2D);
            
    	    glMatrixMode(GL_TEXTURE);
    	    glLoadIdentity();
    	    glScalef(texscale, texscale, 1);
    	    glMatrixMode(GL_MODELVIEW);
    	    }          
          }
}

void postdraw()
{
        glMatrixMode(GL_TEXTURE);
        glLoadIdentity();
    	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
	glPopAttrib();
}

void inittexture()
{
	    glGenTextures(1,&gltexture);
            glBindTexture(GL_TEXTURE_2D, gltexture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                         texture->width, texture->height, 0,
                         GL_RGBA, GL_UNSIGNED_BYTE,
                         texture->data);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

}


	/* Add a little randomness */
/*	fblue = ((float) (NRAND(30)) / 100.0) + 0.70;
	fgreen = fblue * 0.56;
	glClearColor(0.0, fgreen, fblue, 0.0);
*/




/* Fill the background with a gradient -- thanks to 
   Phil Carrig <pod@internode.on.net> for figuring out
   how to do this more efficiently!
 */                /*
static void
clear_tank (atlantisstruct * ap)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (do_gradient && !wire)
    {
      GLfloat top[3] = { 0.00, 0.40, 0.70 };
      GLfloat bot[3] = { 0.00, 0.05, 0.18 };

      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
      {
        glLoadIdentity();
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        {
          glLoadIdentity();
                 */
          /* save GL_COLOR_MATERIAL, GL_COLOR_MATERIAL_FACE, etc.
             This stalls the pipeline, so it would be better to do this
             with explicit enable/disable calls, but I can't figure
             out how to undo the glEnable() and glColor() calls below!
             Simply calling glDisable(GL_COLOR_MATERIAL) is insufficient!
           */  /*
          glPushAttrib (GL_LIGHTING_BIT);
          {
            glEnable (GL_COLOR_MATERIAL);

            glShadeModel(GL_SMOOTH);
            glBegin(GL_QUADS);
            glColor3f (bot[0], bot[1], bot[2]); glVertex3f (-1, -1, 1);
            glColor3f (bot[0], bot[1], bot[2]); glVertex3f ( 1, -1, 1);
            glColor3f (top[0], top[1], top[2]); glVertex3f ( 1,  1, 1);
            glColor3f (top[0], top[1], top[2]); glVertex3f (-1,  1, 1);
            glEnd();
          }
          glPopAttrib();
        }
        glPopMatrix();
      }
      glMatrixMode(GL_PROJECTION);
      glPopMatrix();

      glMatrixMode(GL_MODELVIEW);
    }
}
    */



void
FishTransform(fishRec * fish)
{

	glTranslatef(fish->y, fish->z, -fish->x);
	glRotatef(-fish->psi, 0.0, 1.0, 0.0);
	glRotatef(fish->theta, 1.0, 0.0, 0.0);
	glRotatef(-fish->phi, 0.0, 0.0, 1.0);
}

void
WhalePilot(fishRec * fish, float whalespeed, Bool whaledir)
{

	fish->phi = -20.0;
	fish->theta = 0.0;
	fish->psi += ((whaledir) ? -0.5 : 0.5);

	fish->x += whalespeed * fish->v * cos(fish->psi / RAD) * cos(fish->theta / RAD);
	fish->y += whalespeed * fish->v * sin(fish->psi / RAD) * cos(fish->theta / RAD);
	fish->z += whalespeed * fish->v * sin(fish->theta / RAD);
}

void
SharkPilot(fishRec * fish, float sharkspeed)
{
	float       X, Y, Z, tpsi, ttheta, thetal;

	fish->xt = 60000.0;
	fish->yt = 0.0;
	fish->zt = 0.0;

	X = fish->xt - fish->x;
	Y = fish->yt - fish->y;
	Z = fish->zt - fish->z;

	thetal = fish->theta;

	ttheta = RAD * atan(Z / (sqrt(X * X + Y * Y)));

	if (ttheta > fish->theta + 0.25) {
		fish->theta += 0.5;
	} else if (ttheta < fish->theta - 0.25) {
		fish->theta -= 0.5;
	}
	if (fish->theta > 90.0) {
		fish->theta = 90.0;
	}
	if (fish->theta < -90.0) {
		fish->theta = -90.0;
	}
	fish->dtheta = fish->theta - thetal;

	tpsi = RAD * atan2(Y, X);

	fish->attack = 0;

	if (fabs(tpsi - fish->psi) < 10.0) {
		fish->attack = 1;
	} else if (fabs(tpsi - fish->psi) < 45.0) {
		if (fish->psi > tpsi) {
			fish->psi -= 0.5;
			if (fish->psi < -180.0) {
				fish->psi += 360.0;
			}
		} else if (fish->psi < tpsi) {
			fish->psi += 0.5;
			if (fish->psi > 180.0) {
				fish->psi -= 360.0;
			}
		}
	} else {
		if (NRAND(100) > 98) {
			fish->sign = (fish->sign < 0 ? 1 : -1);
		}
		fish->psi += (fish->sign ? 1 : -1);
		if (fish->psi > 180.0) {
			fish->psi -= 360.0;
		}
		if (fish->psi < -180.0) {
			fish->psi += 360.0;
		}
	}

	if (fish->attack) {
		if (fish->v < 1.1) {
			fish->spurt = 1;
		}
		if (fish->spurt) {
			fish->v += 0.2;
		}
		if (fish->v > 5.0) {
			fish->spurt = 0;
		}
		if ((fish->v > 1.0) && (!fish->spurt)) {
			fish->v -= 0.2;
		}
	} else {
		if (!(NRAND(400)) && (!fish->spurt)) {
			fish->spurt = 1;
		}
		if (fish->spurt) {
			fish->v += 0.05;
		}
		if (fish->v > 3.0) {
			fish->spurt = 0;
		}
		if ((fish->v > 1.0) && (!fish->spurt)) {
			fish->v -= 0.05;
		}
	}

	fish->x += sharkspeed * fish->v * cos(fish->psi / RAD) * cos(fish->theta / RAD);
	fish->y += sharkspeed * fish->v * sin(fish->psi / RAD) * cos(fish->theta / RAD);
	fish->z += sharkspeed * fish->v * sin(fish->theta / RAD);
}

void
SharkMiss(unsigned int i)
{
	unsigned int         j;
	float       avoid, thetal;
	float       X, Y, Z, R;

	for (j = 0; j < sharks.size(); j++) {
		if (j != i) {
			X = sharks[j].x - sharks[i].x;
			Y = sharks[j].y - sharks[i].y;
			Z = sharks[j].z - sharks[i].z;

			R = sqrt(X * X + Y * Y + Z * Z);

			avoid = 1.0;
			thetal = sharks[i].theta;

			if (R < sharksize) {
				if (Z > 0.0) {
					sharks[i].theta -= avoid;
				} else {
					sharks[i].theta += avoid;
				}
			}
			sharks[i].dtheta += (sharks[i].theta - thetal);
		}
	}
}
void
Animate()
{
	unsigned int         i;

	for (i = 0; i < sharks.size(); i++) {
		SharkPilot(&(sharks[i]), sharkspeed);
		SharkMiss(i);
	}
	WhalePilot(&(dolph), whalespeed, whaledir);
	dolph.phi++;
	WhalePilot(&(momWhale), whalespeed, whaledir);
	momWhale.phi++;
	WhalePilot(&(babyWhale), whalespeed, whaledir);
	babyWhale.phi++;
}

void AllDisplay()
{
	unsigned int         i;

	for (i = 0; i < sharks.size(); i++) {
		glPushMatrix();
		FishTransform(&(sharks[i]));
		sharks[i].Draw();
		glPopMatrix();
	}

	glPushMatrix();
	FishTransform(&(dolph));
	dolph.Draw();
	glPopMatrix();

	glPushMatrix();
	FishTransform(&(momWhale));
	momWhale.Draw();
	glPopMatrix();

	glPushMatrix();
	FishTransform(&(babyWhale));
	glScalef(0.45, 0.45, 0.3);
	babyWhale.Draw();
	glPopMatrix();
}

/*
 *-----------------------------------------------------------------------------
 *    Initialize atlantis.  Called each time the window changes.
 *-----------------------------------------------------------------------------
 */
public:
atlantis(float asp = 1)
{

        SDL_SysWMinfo i;
        SDL_VERSION(&i.version)
        if(SDL_GetWMInfo(&i))
        {
    	    Display*dpy=i.info.x11.display;
    	    int scr=DefaultScreen(dpy);
    	    Window root=RootWindow(dpy,scr);
	    XWindowAttributes wgwa;
	    XGetWindowAttributes( dpy, root, &wgwa );
	    texture = minixpm_to_ximage (dpy,wgwa.visual,wgwa.colormap,wgwa.depth,
	    BlackPixelOfScreen(wgwa.screen), sea_texture,0);
	    inittexture();
	}
	aspect=asp;
	do_texture=0;
	texscale = 0.0005;
	sharks.resize(5);
	sharkspeed = 100;		/* has influence on the "width"
						   of the movement */
	sharksize = 6000;	/* has influence on the "distance"
					   of the sharks */
	whalespeed = 250;
	wire = 0;
	ghost=0;
	dirty = 1;
        InitFishs();
        setwire(wire);
}

void setwire(unsigned int w)
{
    	for (unsigned int i = 0; i < sharks.size(); i++) {
    		sharks[i].wire=w;
    	}
    	momWhale.wire=w;
    	babyWhale.wire=w;
    	dolph.wire=w;
}

virtual void keyp(int key,int uni,int mod)
{
    cout << uni << endl;
    if(uni=='w')
    {
	wire=!wire;
	setwire(wire);
    }
    else if(uni=='t')
	do_texture=!do_texture;
    else if(uni=='g')
	ghost=!ghost;
    else obj::keyp(key,uni,mod);
    
}
/*
 *-----------------------------------------------------------------------------
 *    Called by the mainline code periodically to update the display.
 *-----------------------------------------------------------------------------
 */
void draw(int picking)
{
	predraw(picking);
        AllDisplay();
        postdraw();
        if (!picking)
    		Animate();
}


/*
 *-----------------------------------------------------------------------------
 *    The display is being taken away from us.  Free up malloc'ed 
 *      memory and X resources that we've alloc'ed.  Only called
 *      once, we must zap everything for every screen.
 *-----------------------------------------------------------------------------
 */

~atlantis()
{
    sharks.clear();
}


    SAVE(atlantis)
    {
	YAML_EMIT_PARENT_MEMBERS(out,obj)
    }
    LOAD
    {
	YAML_LOAD_PARENT_MEMBERS(doc,obj)
    }

    void setClean(){}


};

#define has_atlantis
#endif
