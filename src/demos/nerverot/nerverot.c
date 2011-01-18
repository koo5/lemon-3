/* nerverot, nervous rotation of random thingies, v1.4
 * by Dan Bornstein, danfuzz@milk.com
 * Copyright (c) 2000-2001 Dan Bornstein. All rights reserved.
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation.  No representations are made about the suitability of this
 * software for any purpose.  It is provided "as is" without express or 
 * implied warranty.
 *
 * The goal of this screensaver is to be interesting and compelling to
 * watch, yet induce a state of nervous edginess in the viewer.
 *
 * See the included man page for more details.
 */

#include <math.h>

#define FLOAT double

/* random float in the range (-1..1) */
#define RAND_FLOAT_PM1 \
        (((FLOAT) ((random() >> 8) & 0xffff)) / ((FLOAT) 0x10000) * 2 - 1)

/* random float in the range (0..1) */
#define RAND_FLOAT_01 \
        (((FLOAT) ((random() >> 8) & 0xffff)) / ((FLOAT) 0x10000))


/* structure of the model */

/* each point-like thingy to draw is represented as a blot */
typedef struct blot_s
{
    FLOAT x;           /* 3d x position (-1..1) */
    FLOAT y;           /* 3d y position (-1..1) */
    FLOAT z;           /* 3d z position (-1..1) */
    FLOAT xoff[3][3];  /* display x offset per drawn point (-1..1) */
    FLOAT yoff[3][3];  /* display x offset per drawn point (-1..1) */
    FLOAT x2,y2,z2;
    FLOAT xnp[3][3];
    FLOAT ynp[3][3];
    FLOAT ip[4];
} Blot;

/* each drawn line is represented as a LineSegment */
typedef struct linesegment_s
{
    int c;
    int x1;
    int y1;
    int x2;
    int y2;
} LineSegment;

/* each blot draws as a simple 2d shape with each coordinate as an int
 * in the range (-1..1); this is the base shape */
static const XPoint blotShape[] = { { 0, 0}, { 1, 0}, { 1, 1}, 
                                    { 0, 1}, {-1, 1}, {-1, 0}, 
                                    {-1,-1}, { 0,-1}, { 1,-1} };
static int blotShapeCount = sizeof (blotShape) / sizeof (XPoint);





struct nerverotstate {
   int requestedBlotCount;	/* number of blots */
   int maxIters;		/* max iterations per model */
   FLOAT nervousness;	/* variability of xoff/yoff per iteration (0..1) */
   FLOAT maxNerveRadius;	/* max nervousness radius (0..1) */
   FLOAT eventChance;	/* chance per iteration that an event will happen */
   FLOAT iterAmt;		/* fraction (0..1) towards rotation target or scale target to move each * iteration */
   FLOAT minScale;		/* min and max scale for drawing, as fraction of baseScale */
   FLOAT maxScale;
   int minRadius;		/* min and max radius of blot drawing */
   int maxRadius;
   int colorCount;

   int baseScale;		/* base scale factor for drawing, calculated as max(screenWidth,screenHeight) *//*min*/

   int windowWidth;		/* width and height of the window */
   int windowHeight;

   int centerX;		/* center position of the window */
   int centerY;

   Blot *blots;	/* array of the blots in the model */
   int blotCount;

   int segCount;		/* two arrays of line segments; one for the ones to erase, and one for the ones to draw */

   LineSegment *segsToDraw;
   LineSegment *segsToErase;
   int cur_num;
   int please_num;


   FLOAT xRot;		/* current rotation values per axis, scale factor, and light position */

   FLOAT yRot;
   FLOAT zRot;
   FLOAT curScale;
   FLOAT lightX;
   FLOAT lightY;
   FLOAT lightZ;

   FLOAT xRotTarget;	/* target rotation values per axis, scale factor, and light position */

   FLOAT yRotTarget;
   FLOAT zRotTarget;
   FLOAT scaleTarget;
   FLOAT lightXTarget;
   FLOAT lightYTarget;
   FLOAT lightZTarget;

   int centerXOff;	/* current absolute offsets from the center */
   int centerYOff;
   int centerZOff;
   int itersTillNext;	/* iterations until the model changes */
   
   FLOAT goofiness;
};


/*
 * generic blot setup and manipulation
 */

/* initialize a blot with the given coordinates and random display offsets */
static void initBlot (Blot *b, FLOAT x, FLOAT y, FLOAT z)
{
    int i, j;

    b->x = x;
    b->y = y;
    b->z = z;

    for (i = 0; i < 3; i++)
    {
	for (j = 0; j < 3; j++)
	{
	    b->xoff[i][j] = RAND_FLOAT_PM1;
	    b->yoff[i][j] = RAND_FLOAT_PM1;
	}
    }
}

/* scale the blots to have a max distance of 1 from the center */
static void scaleBlotsToRadius1 (struct nerverotstate *st)
{
    FLOAT max = 0.0;
    int n;

    for (n = 0; n < st->blotCount; n++)
    {
	FLOAT distSquare = 
	    st->blots[n].x * st->blots[n].x +
	    st->blots[n].y * st->blots[n].y +
	    st->blots[n].z * st->blots[n].z;
	if (distSquare > max)
	{
	    max = distSquare;
	}
    }

    if (max == 0.0)
    {
	return;
    }

    max = sqrt (max);

    for (n = 0; n < st->blotCount; n++)
    {
	st->blots[n].x /= max;
	st->blots[n].y /= max;
	st->blots[n].z /= max;
    }
}

/* randomly reorder the blots */
static void randomlyReorderBlots (struct nerverotstate *st)
{
    int n;

    for (n = 0; n < st->blotCount; n++)
    {
	int m = RAND_FLOAT_01 * (st->blotCount - n) + n;
	Blot tmpBlot = st->blots[n];
	st->blots[n] = st->blots[m];
	st->blots[m] = tmpBlot;
    }
}

/* randomly rotate the blots around the origin */
static void randomlyRotateBlots (struct nerverotstate *st)
{
    int n;

    /* random amounts to rotate about each axis */
    FLOAT xRot = RAND_FLOAT_PM1 * M_PI;
    FLOAT yRot = RAND_FLOAT_PM1 * M_PI;
    FLOAT zRot = RAND_FLOAT_PM1 * M_PI;

    /* rotation factors */
    FLOAT sinX = sin (xRot);
    FLOAT cosX = cos (xRot);
    FLOAT sinY = sin (yRot);
    FLOAT cosY = cos (yRot);
    FLOAT sinZ = sin (zRot);
    FLOAT cosZ = cos (zRot);

    for (n = 0; n < st->blotCount; n++)
    {
	FLOAT x1 = st->blots[n].x;
	FLOAT y1 = st->blots[n].y;
	FLOAT z1 = st->blots[n].z;
	FLOAT x2, y2, z2;

	/* rotate on z axis */
	x2 = x1 * cosZ - y1 * sinZ;
	y2 = x1 * sinZ + y1 * cosZ;
	z2 = z1;

	/* rotate on x axis */
	y1 = y2 * cosX - z2 * sinX;
	z1 = y2 * sinX + z2 * cosX;
	x1 = x2;

	/* rotate on y axis */
	z2 = z1 * cosY - x1 * sinY;
	x2 = z1 * sinY + x1 * cosY;
	y2 = y1;

	st->blots[n].x = x2;
	st->blots[n].y = y2;
	st->blots[n].z = z2;
    }
}



/*
 * blot configurations
 */

/* set up the initial array of blots to be a at the edge of a sphere */
static void setupBlotsSphere (struct nerverotstate *st)
{
    int n;

    st->blotCount = st->requestedBlotCount;
    st->blots = (Blot*)calloc (sizeof (Blot), st->blotCount);

    for (n = 0; n < st->blotCount; n++)
    {
	/* pick a spot, but reject if its radius is < 0.2 or > 1 to
	 * avoid scaling problems */
	FLOAT x, y, z, radius;

	for (;;)
	{
	    x = RAND_FLOAT_PM1;
	    y = RAND_FLOAT_PM1;
	    z = RAND_FLOAT_PM1;

	    radius = sqrt (x * x + y * y + z * z);
	    if ((radius >= 0.2) && (radius <= 1.0))
	    {
		break;
	    }
	}

	x /= radius;
	y /= radius;
	z /= radius;

	initBlot (&st->blots[n], x, y, z);
    }
}

/* set up the initial array of blots to be a simple cube */
static void setupBlotsCube (struct nerverotstate *st)
{
    int i, j, k, n;

    /* derive blotsPerEdge from blotCount, but then do the reverse
     * since roundoff may have changed blotCount */
    int blotsPerEdge = ((st->requestedBlotCount - 8) / 12) + 2;
    FLOAT distBetween;

    if (blotsPerEdge < 2)
    {
	blotsPerEdge = 2;
    }

    distBetween = 2.0 / (blotsPerEdge - 1.0);

    st->blotCount = 8 + (blotsPerEdge - 2) * 12;
    st->blots = (Blot*)calloc (sizeof (Blot), st->blotCount);
    n = 0;

    /* define the corners */
    for (i = -1; i < 2; i += 2)
    {
	for (j = -1; j < 2; j += 2)
	{
	    for (k = -1; k < 2; k += 2)
	    {
		initBlot (&st->blots[n], i, j, k);
		n++;
	    } 
	}
    }

    /* define the edges */
    for (i = 1; i < (blotsPerEdge - 1); i++)
    {
	FLOAT varEdge = distBetween * i - 1;
	initBlot (&st->blots[n++], varEdge, -1, -1);
	initBlot (&st->blots[n++], varEdge,  1, -1);
	initBlot (&st->blots[n++], varEdge, -1,  1);
	initBlot (&st->blots[n++], varEdge,  1,  1);
	initBlot (&st->blots[n++], -1, varEdge, -1);
	initBlot (&st->blots[n++],  1, varEdge, -1);
	initBlot (&st->blots[n++], -1, varEdge,  1);
	initBlot (&st->blots[n++],  1, varEdge,  1);
	initBlot (&st->blots[n++], -1, -1, varEdge);
	initBlot (&st->blots[n++],  1, -1, varEdge);
	initBlot (&st->blots[n++], -1,  1, varEdge);
	initBlot (&st->blots[n++],  1,  1, varEdge);
    }

    scaleBlotsToRadius1 (st);
    randomlyReorderBlots (st);
    randomlyRotateBlots (st);
}

/* set up the initial array of blots to be a cylinder */
static void setupBlotsCylinder (struct nerverotstate *st)
{
    int i, j, n;
    FLOAT distBetween;

    /* derive blotsPerEdge and blotsPerRing from blotCount, but then do the
     * reverse since roundoff may have changed blotCount */
    FLOAT reqRoot = sqrt ((FLOAT) st->requestedBlotCount);
    int blotsPerRing = ceil (RAND_FLOAT_PM1 * reqRoot) / 2 + reqRoot;
    int blotsPerEdge = st->requestedBlotCount / blotsPerRing;

    if (blotsPerRing < 2)
    {
	blotsPerRing = 2;
    }

    if (blotsPerEdge < 2)
    {
	blotsPerEdge = 2;
    }

    distBetween = 2.0 / (blotsPerEdge - 1);

    st->blotCount = blotsPerEdge * blotsPerRing;
    st->blots = (Blot*)calloc (sizeof (Blot), st->blotCount);
    n = 0;

    /* define the edges */
    for (i = 0; i < blotsPerRing; i++)
    {
	FLOAT x = sin (2 * M_PI / blotsPerRing * i);
	FLOAT y = cos (2 * M_PI / blotsPerRing * i);
	for (j = 0; j < blotsPerEdge; j++)
	{
	    initBlot (&st->blots[n], x, y, j * distBetween - 1);
	    n++;
	}
    }

    scaleBlotsToRadius1 (st);
    randomlyReorderBlots (st);
    randomlyRotateBlots (st);
}

/* set up the initial array of blots to be a squiggle */
static void setupBlotsSquiggle (struct nerverotstate *st)
{
    FLOAT x, y, z, xv, yv, zv, len;
    int minCoor, maxCoor;
    int n;

    st->blotCount = st->requestedBlotCount;
    st->blots = (Blot*)calloc (sizeof (Blot), st->blotCount);

    maxCoor = (int) (RAND_FLOAT_01 * 5) + 1;
    minCoor = -maxCoor;

    x = RAND_FLOAT_PM1;
    y = RAND_FLOAT_PM1;
    z = RAND_FLOAT_PM1;

    xv = RAND_FLOAT_PM1;
    yv = RAND_FLOAT_PM1;
    zv = RAND_FLOAT_PM1;
    len = sqrt (xv * xv + yv * yv + zv * zv);
    xv /= len;
    yv /= len;
    zv /= len;
    
    for (n = 0; n < st->blotCount; n++)
    {
	FLOAT newx, newy, newz;
	initBlot (&st->blots[n], x, y, z);

	for (;;)
	{
	    xv += RAND_FLOAT_PM1 * 0.1;
	    yv += RAND_FLOAT_PM1 * 0.1;
	    zv += RAND_FLOAT_PM1 * 0.1;
	    len = sqrt (xv * xv + yv * yv + zv * zv);
	    xv /= len;
	    yv /= len;
	    zv /= len;

	    newx = x + xv * 0.1;
	    newy = y + yv * 0.1;
	    newz = z + zv * 0.1;

	    if (   (newx >= minCoor) && (newx <= maxCoor)
		&& (newy >= minCoor) && (newy <= maxCoor)
		&& (newz >= minCoor) && (newz <= maxCoor))
	    {
		break;
	    }
	}

	x = newx;
	y = newy;
	z = newz;
    }

    scaleBlotsToRadius1 (st);
    randomlyReorderBlots (st);
}

/* set up the initial array of blots to be near the corners of a
 * cube, distributed slightly */
static void setupBlotsCubeCorners (struct nerverotstate *st)
{
    int n;

    st->blotCount = st->requestedBlotCount;
    st->blots = (Blot*)calloc (sizeof (Blot), st->blotCount);

    for (n = 0; n < st->blotCount; n++)
    {
	FLOAT x = rint (RAND_FLOAT_01) * 2 - 1;
	FLOAT y = rint (RAND_FLOAT_01) * 2 - 1;
	FLOAT z = rint (RAND_FLOAT_01) * 2 - 1;

	x += RAND_FLOAT_PM1 * 0.3;
	y += RAND_FLOAT_PM1 * 0.3;
	z += RAND_FLOAT_PM1 * 0.3;

	initBlot (&st->blots[n], x, y, z);
    }

    scaleBlotsToRadius1 (st);
    randomlyRotateBlots (st);
}

/* set up the initial array of blots to be randomly distributed
 * on the surface of a tetrahedron */
static void setupBlotsTetrahedron (struct nerverotstate *st)
{
    /* table of corners of the tetrahedron */
    static const FLOAT cor[4][3] = { {  0.0,   1.0,  0.0 },
                                     { -0.75, -0.5, -0.433013 },
                                     {  0.0,  -0.5,  0.866025 },
                                     {  0.75, -0.5, -0.433013 } };

    int n, c;

    /* derive blotsPerSurface from blotCount, but then do the reverse
     * since roundoff may have changed blotCount */
    int blotsPerSurface = st->requestedBlotCount / 4;

    st->blotCount = blotsPerSurface * 4;
    st->blots = (Blot*)calloc (sizeof (Blot), st->blotCount);

    for (n = 0; n < st->blotCount; n += 4)
    {
	/* pick a random point on a unit right triangle */
	FLOAT rawx = RAND_FLOAT_01;
	FLOAT rawy = RAND_FLOAT_01;

	if ((rawx + rawy) > 1)
	{
	    /* swap coords into place */
	    FLOAT t = 1.0 - rawx;
	    rawx = 1.0 - rawy;
	    rawy = t;
	}

	/* translate the point to be on each of the surfaces */
	for (c = 0; c < 4; c++)
	{
	    FLOAT x, y, z;
	    
	    int c1 = (c + 1) % 4;
	    int c2 = (c + 2) % 4;
	    
	    x = (cor[c1][0] - cor[c][0]) * rawx + 
		(cor[c2][0] - cor[c][0]) * rawy + 
		cor[c][0];

	    y = (cor[c1][1] - cor[c][1]) * rawx + 
		(cor[c2][1] - cor[c][1]) * rawy + 
		cor[c][1];

	    z = (cor[c1][2] - cor[c][2]) * rawx + 
		(cor[c2][2] - cor[c][2]) * rawy + 
		cor[c][2];

	    initBlot (&st->blots[n + c], x, y, z);
	}
    }

    randomlyRotateBlots (st);
}

/* set up the initial array of blots to be an almost-evenly-distributed
 * square sheet */
static void setupBlotsSheet (struct nerverotstate *st)
{
    int x, y;

    int blotsPerDimension = floor (sqrt (st->requestedBlotCount));
    FLOAT spaceBetween;

    if (blotsPerDimension < 2)
    {
	blotsPerDimension = 2;
    }

    spaceBetween = 2.0 / (blotsPerDimension - 1);

    st->blotCount = blotsPerDimension * blotsPerDimension;
    st->blots = (Blot*)calloc (sizeof (Blot), st->blotCount);

    for (x = 0; x < blotsPerDimension; x++)
    {
	for (y = 0; y < blotsPerDimension; y++)
	{
	    FLOAT x1 = x * spaceBetween - 1.0;
	    FLOAT y1 = y * spaceBetween - 1.0;
	    FLOAT z1 = 0.0;

	    x1 += RAND_FLOAT_PM1 * spaceBetween / 3;
	    y1 += RAND_FLOAT_PM1 * spaceBetween / 3;
	    z1 += RAND_FLOAT_PM1 * spaceBetween / 2;

	    initBlot (&st->blots[x + y * blotsPerDimension], x1, y1, z1);
	}
    }

    scaleBlotsToRadius1 (st);
    randomlyReorderBlots (st);
    randomlyRotateBlots (st);
}

/* set up the initial array of blots to be a swirlycone */
static void setupBlotsSwirlyCone (struct nerverotstate *st)
{
    FLOAT radSpace = 1.0 / (st->requestedBlotCount - 1);
    FLOAT zSpace = radSpace * 2;
    FLOAT rotAmt = RAND_FLOAT_PM1 * M_PI / 10;

    int n;
    FLOAT rot = 0.0;

    st->blotCount = st->requestedBlotCount;
    st->blots = (Blot*)calloc (sizeof (Blot), st->blotCount);

    for (n = 0; n < st->blotCount; n++)
    {
	FLOAT radius = n * radSpace;
	FLOAT x = cos (rot) * radius;
	FLOAT y = sin (rot) * radius;
	FLOAT z = n * zSpace - 1.0;

	rot += rotAmt;
	initBlot (&st->blots[n], x, y, z);
    }

    scaleBlotsToRadius1 (st);
    randomlyReorderBlots (st);
    randomlyRotateBlots (st);
}

/* forward declaration for recursive use immediately below */
static void randomBlots (struct nerverotstate *st);

/* set up the blots to be two of the other choices, placed next to
 * each other */
static void setupBlotsDuo (struct nerverotstate *st)
{
    int origRequest = st->requestedBlotCount;
    FLOAT tx, ty, tz, radius;
    Blot *blots1, *blots2;
    int count1, count2;
    int n;

    if (st->requestedBlotCount < 15)
    {
	/* special case bottom-out */
	setupBlotsSphere (st);
	return;
    }

    tx = RAND_FLOAT_PM1;
    ty = RAND_FLOAT_PM1;
    tz = RAND_FLOAT_PM1;
    radius = sqrt (tx * tx + ty * ty + tz * tz);
    tx /= radius;
    ty /= radius;
    tz /= radius;

    /* recursive call to setup set 1 */
    st->requestedBlotCount = origRequest / 2;
    randomBlots (st);

    if (st->blotCount >= origRequest)
    {
	/* return immediately if this satisfies the original count request */
	st->requestedBlotCount = origRequest;
	return;
    }

    blots1 = st->blots;
    count1 = st->blotCount;
    st->blots = NULL;
    st->blotCount = 0;
    
    /* translate to new position */
    for (n = 0; n < count1; n++)
    {
	blots1[n].x += tx;
	blots1[n].y += ty;
	blots1[n].z += tz;
    }

    /* recursive call to setup set 2 */
    st->requestedBlotCount = origRequest - count1;
    randomBlots (st);
    blots2 = st->blots;
    count2 = st->blotCount;

    /* translate to new position */
    for (n = 0; n < count2; n++)
    {
	blots2[n].x -= tx;
	blots2[n].y -= ty;
	blots2[n].z -= tz;
    }

    /* combine the two arrays */
    st->blotCount = count1 + count2;
    st->blots = (Blot*)calloc (sizeof (Blot), st->blotCount);
    memcpy (&st->blots[0],      blots1, sizeof (Blot) * count1);
    memcpy (&st->blots[count1], blots2, sizeof (Blot) * count2);
    free (blots1);
    free (blots2);

    scaleBlotsToRadius1 (st);
    randomlyReorderBlots (st);

    /* restore the original requested count, for future iterations */
    st->requestedBlotCount = origRequest;
}



/*
 * main blot setup
 */

/* free the blots, in preparation for a new shape */
static void freeBlots (struct nerverotstate *st)
{
    if (st->blots != NULL)
    {
	free (st->blots);
	st->blots = NULL;
    }

    if (st->segsToErase != NULL)
    {
	free (st->segsToErase);
	st->segsToErase = NULL;
    }

    if (st->segsToDraw != NULL)
    {
	free (st->segsToDraw);
	st->segsToDraw = NULL;
    }
}

#include "setupBlots"
static void randomBlots (struct nerverotstate *st)
{
    int which = RAND_FLOAT_01 * 11;
    freeBlots (st);
    setupBlots(st,which);
    st->cur_num=which;
}    
static void commandBlots (struct nerverotstate *st)
{
    freeBlots (st);
    setupBlots(st, st->please_num);
    st->cur_num=st->please_num;
}

/* set up the segments arrays */
static void setupSegs (struct nerverotstate *st)
{
    /* there are blotShapeCount - 1 line segments per blot */
    st->segCount = st->blotCount * (blotShapeCount - 1);
    st->segsToErase = (LineSegment*)calloc (sizeof (LineSegment), st->segCount);
    st->segsToDraw = (LineSegment*)calloc (sizeof (LineSegment), st->segCount);
}



/*
 * color setup stuff
 */

/* set up the colormap */
/*
static void setupColormap (struct nerverotstate *st)
{
    int n;
    XGCValues gcv;
    XColor *colors = (XColor *) calloc (sizeof (XColor), st->colorCount + 1);

    unsigned short r, g, b;
    int h1, h2;
    double s1, s2, v1, v2;

    r = RAND_FLOAT_01 * 0x10000;
    g = RAND_FLOAT_01 * 0x10000;
    b = RAND_FLOAT_01 * 0x10000;
    rgb_to_hsv (r, g, b, &h1, &s1, &v1);
    v1 = 1.0;
    s1 = 1.0;

    r = RAND_FLOAT_01 * 0x10000;
    g = RAND_FLOAT_01 * 0x10000;
    b = RAND_FLOAT_01 * 0x10000;
    rgb_to_hsv (r, g, b, &h2, &s2, &v2);
    s2 = 0.7;
    v2 = 0.7;
    
    colors[0].pixel = 0;
    
    st->gcs = (GC *) calloc (sizeof (GC), st->colorCount + 1);

    for (n = 0; n <= st->colorCount; n++) 
    {
	gcv.foreground = colors[n].pixel;
	gcv.line_width = st->lineWidth;
	st->gcs[n] = XCreateGC (st->dpy, st->window, GCForeground | GCLineWidth, &gcv);
    }

    free (colors);
}
*/


/*
 * overall setup stuff
 */

/* set up the system */
static void setup (struct nerverotstate *st,int w,int h)
{
    st->colorCount = 2000;
    st->windowWidth = w;
    st->windowHeight = h;
    st->centerX = st->windowWidth / 2;
    st->centerY = st->windowHeight / 2;
    st->baseScale = (st->windowHeight < st->windowWidth) ? st->windowHeight : st->windowWidth;
/*    setupColormap (st, &xgwa);*/
    randomBlots (st);
    setupSegs (st);

    /* set up the initial rotation, scale, and light values as random, but
     * with the targets equal to where it is */
    st->xRot = st->xRotTarget = RAND_FLOAT_01 * M_PI;
    st->yRot = st->yRotTarget = RAND_FLOAT_01 * M_PI;
    st->zRot = st->zRotTarget = RAND_FLOAT_01 * M_PI;
    st->curScale = st->scaleTarget = RAND_FLOAT_01 * (st->maxScale - st->minScale) + st->minScale;
    st->lightX = st->lightXTarget = RAND_FLOAT_PM1;
    st->lightY = st->lightYTarget = RAND_FLOAT_PM1;
    st->lightZ = st->lightZTarget = RAND_FLOAT_PM1;

    st->itersTillNext = RAND_FLOAT_01 * st->maxIters;
}



/*
 * the simulation
 */

/* "render" the blots into segsToDraw, with the current rotation factors */
static void renderSegs (struct nerverotstate *st)
{
    int n;
    int m = 0;

    /* rotation factors */
    FLOAT sinX = sin (st->xRot);
    FLOAT cosX = cos (st->xRot);
    FLOAT sinY = sin (st->yRot);
    FLOAT cosY = cos (st->yRot);
    FLOAT sinZ = sin (st->zRot);
    FLOAT cosZ = cos (st->zRot);

    for (n = 0; n < st->blotCount; n++)
    {
	Blot *b = &st->blots[n];
	int i, j;
	int baseX, baseY;
	FLOAT radius;
	int x[3][3];
	int y[3][3];
	int color;

	FLOAT x1 = st->blots[n].x;
	FLOAT y1 = st->blots[n].y;
	FLOAT z1 = st->blots[n].z;
	FLOAT x2, y2, z2;

	/* rotate on z axis */
	x2 = x1 * cosZ - y1 * sinZ;
	y2 = x1 * sinZ + y1 * cosZ;
	z2 = z1;

	/* rotate on x axis */
	y1 = y2 * cosX - z2 * sinX;
	z1 = y2 * sinX + z2 * cosX;
	x1 = x2;

	/* rotate on y axis */
	z2 = z1 * cosY - x1 * sinY;
	x2 = z1 * sinY + x1 * cosY;
	y2 = y1;
	/* the color to draw is based on the distance from the light of
	 * the post-rotation blot*/
	x1 = x2 - st->lightX;
	y1 = y2 - st->lightY;
	z1 = z2 - st->lightZ;
	color = 1 + (x1 * x1 + y1 * y1 + z1 * z1) / 4 * st->colorCount;
	if (color > st->colorCount)
	{
	    color = st->colorCount;
	}
	color = st->colorCount - color;

	/* set up the base screen coordinates for drawing */
	baseX = x2 / 2 * st->baseScale * st->curScale + st->centerX + st->centerXOff;
	baseY = y2 / 2 * st->baseScale * st->curScale + st->centerY + st->centerYOff;
    int baseZ = z2 / 2 * st->baseScale * st->curScale + (st->baseScale/2);
	b->x2=((FLOAT)baseX/(FLOAT)st->windowWidth-0.5)*2.0;
	b->y2=((FLOAT)baseY/(FLOAT)st->windowHeight-0.5)*2.0;
	b->z2=((FLOAT)baseZ/(FLOAT)st->baseScale-0.5)*2.0;
	if (st->goofiness==0)
	    z2=z2*st->curScale;/*+(1-st->moofiness)*(st->centerZOff/st->baseScale)*/
	radius = (z2 + 1) / 2 * (st->maxRadius - st->minRadius) + st->minRadius;
	for (i = 0; i < 3; i++)
	{
	    for (j = 0; j < 3; j++)
	    {
		b->xnp[i][j]=x[i][j] = baseX + radius * ((i - 1) + (b->xoff[i][j] * st->maxNerveRadius));
		b->ynp[i][j]=y[i][j] = baseY + radius * ((j - 1) + (b->yoff[i][j] * st->maxNerveRadius));
	    }
	}

	for (i = 1; i < blotShapeCount; i++)
	{
	    st->segsToDraw[m].c = color;
	    st->segsToDraw[m].x1 = x[blotShape[i-1].x + 1][blotShape[i-1].y + 1];
	    st->segsToDraw[m].y1 = y[blotShape[i-1].x + 1][blotShape[i-1].y + 1];
	    st->segsToDraw[m].x2 = x[blotShape[i].x   + 1][blotShape[i].y   + 1];
	    st->segsToDraw[m].y2 = y[blotShape[i].x   + 1][blotShape[i].y   + 1];
	    m++;
	}
    }
}

/* update blots, adjusting the offsets and rotation factors. */
static void updateWithFeeling (struct nerverotstate *st)
{
    int n, i, j;

    /* pick a new model if the time is right */
    st->itersTillNext--;

    if ((st->please_num!=-1)&&(st->cur_num!=st->please_num))
    {
	commandBlots (st);
	setupSegs (st);
    	renderSegs (st);
    }
    if (st->itersTillNext < 0)
    {
	st->itersTillNext = RAND_FLOAT_01 * st->maxIters;
	randomBlots (st);
	setupSegs (st);
	renderSegs (st);
    }

    /* update the rotation factors by moving them a bit toward the targets */
    st->xRot = st->xRot + (st->xRotTarget - st->xRot) * st->iterAmt; 
    st->yRot = st->yRot + (st->yRotTarget - st->yRot) * st->iterAmt;
    st->zRot = st->zRot + (st->zRotTarget - st->zRot) * st->iterAmt;

    /* similarly the scale factor */
    st->curScale = st->curScale + (st->scaleTarget - st->curScale) * st->iterAmt;

    /* and similarly the light position */
    st->lightX = st->lightX + (st->lightXTarget - st->lightX) * st->iterAmt; 
    st->lightY = st->lightY + (st->lightYTarget - st->lightY) * st->iterAmt; 
    st->lightZ = st->lightZ + (st->lightZTarget - st->lightZ) * st->iterAmt; 

    /* for each blot... */
    for (n = 0; n < st->blotCount; n++)
    {
	/* add a bit of random jitter to xoff/yoff */
	for (i = 0; i < 3; i++)
	{
	    for (j = 0; j < 3; j++)
	    {
		FLOAT newOff;

		newOff = st->blots[n].xoff[i][j] + RAND_FLOAT_PM1 * st->nervousness;
		if (newOff < -1) newOff = -(newOff + 1) - 1;
		else if (newOff > 1) newOff = -(newOff - 1) + 1;
		st->blots[n].xoff[i][j] = newOff;

		newOff = st->blots[n].yoff[i][j] + RAND_FLOAT_PM1 * st->nervousness;
		if (newOff < -1) newOff = -(newOff + 1) - 1;
		else if (newOff > 1) newOff = -(newOff - 1) + 1;
		st->blots[n].yoff[i][j] = newOff;
	    }
	}
    }

#include "factors"
}

/* erase segsToErase and draw segsToDraw */
static void eraseAndDraw (struct nerverotstate *st)
{
    glPushMatrix();
    glTranslatef(-1,-1,0);
    glScalef(2.0/(float)st->windowWidth, 2.0/(float)st->windowHeight,1);
    glBegin(GL_LINES);
    int n;
    LineSegment *seg;
    float fun;
    for (n = 0; n < st->segCount; n++)
    {
	seg = &st->segsToDraw[n];
	fun=(float)seg->c/(float)st->colorCount;
	glColor3f(fun,fun,0);
	glVertex2i(seg->x2, seg->y2);	glVertex2i(seg->x1, seg->y1);
/*
	seg = &st->segsToErase[n];
	glColor3f(0,0,fun/2);
	glVertex2i(seg->x2, seg->y2);	glVertex2i(seg->x1, seg->y1);
*/
    }
    glEnd();
    glPopMatrix();
}



static void twonhalfd_draw (struct nerverotstate *st)
{
    int n;
    int m = 0;
    float fun;
    for (n = 0; n < st->blotCount; n++)
    {
	Blot *b = &st->blots[n];
	glBegin(GL_POINTS);
	glColor3f(0,1,0);
	glVertex3f(b->x2, b->y2, b->z2);
	glEnd();
	fun=(float)st->segsToDraw[m].c/(float)st->colorCount;
	glPushMatrix();
	glTranslatef(-1,-1,0);
	glScalef(2.0/(float)st->windowWidth, 2.0/(float)st->windowHeight,1);
	glBegin(GL_LINES);
	glColor3f(0,fun,0);
	int i;
	for (i = 1; i < blotShapeCount; i++)
	{
	    glVertex3f(
		b->xnp[blotShape[i-1].x + 1][blotShape[i-1].y + 1],
		b->ynp[blotShape[i-1].x + 1][blotShape[i-1].y + 1],b->z2);
	    glVertex3f(
		b->xnp[blotShape[i].x + 1][blotShape[i].y + 1],
		b->ynp[blotShape[i].x + 1][blotShape[i].y + 1],b->z2);
	    m++;
	}
	glEnd();
	glPopMatrix();
    }
}


//#include "stolenfromglu"

static void threed_draw (struct nerverotstate *st, double alpha)
{
    int n;
    int m = 0;
    float fun=0.5;

    GLint viewport[4];// Space For Viewport xywh
    GLdouble winx, winy, winz;// Space For Returned Projected Coords
    GLdouble mvmatrix[16], projmatrix[16];// Space For Transform Matrix
    glGetIntegerv (GL_VIEWPORT, (GLint *)&viewport);// Get Actual Viewport
    glGetDoublev (GL_MODELVIEW_MATRIX, (GLdouble*)&mvmatrix);// Get Actual Model View Matrix
    glGetDoublev (GL_PROJECTION_MATRIX,  (GLdouble*)&projmatrix);// Get Actual Projection Matrix

    for (n = 0; n < st->blotCount; n++)
    {
	Blot *b = &st->blots[n];
	double in[4];

	in[0]=b->x2;
	in[1]=b->y2;
	in[2]=b->z2;
	in[3]=1.0;
	__gluMultMatrixVecd(mvmatrix, in, b->ip);
    }
    glPushMatrix();
    glLoadIdentity();
    for (n = 0; n < st->blotCount; n++)
    {
	Blot *b = &st->blots[n];
	fun=(float)st->segsToDraw[m].c/(float)st->colorCount;
	glBegin(GL_LINE_STRIP);
	glColor4f(0.1,fun/2,0,alpha);
        if (gluProject(b->x2, b->y2, b->z2, mvmatrix, projmatrix, viewport, &winx, &winy, &winz))
	{
	    int i;
	    for (i = 1; i < blotShapeCount; i++)
	    {
		FLOAT rx=(st->minRadius + (winz)*(st->maxRadius - st->minRadius))/st->windowWidth*2.0;
		FLOAT ry=(st->minRadius + (winz)*(st->maxRadius - st->minRadius))/st->windowHeight*2.0;

		glVertex3f(
		    b->ip[0]+(blotShape[i-1].x+ b->xoff[1+blotShape[i-1].x][1+blotShape[i-1].y]* st->maxNerveRadius) *rx,
		    b->ip[1]+(blotShape[i-1].y+ b->yoff[1+blotShape[i-1].x][1+blotShape[i-1].y]* st->maxNerveRadius) *ry,b->ip[2]);
		glVertex3f(
		    b->ip[0]+(blotShape[i].x+ b->xoff[1+blotShape[i].x][1+blotShape[i].y]* st->maxNerveRadius) *rx,
		    b->ip[1]+(blotShape[i].y+ b->yoff[1+blotShape[i].x][1+blotShape[i].y]* st->maxNerveRadius) *ry,b->ip[2]);
		m++;
	    }
	}
	glEnd();
    }
    glPopMatrix();
}

/* do one iteration */
static void nerverot_update (struct nerverotstate *closure)
{
  struct nerverotstate *st = (struct nerverotstate *) closure;
    /* switch segsToErase and segsToDraw */
    LineSegment *temp = st->segsToDraw;
    st->segsToDraw = st->segsToErase;
    st->segsToErase = temp;

    /* update the model */
    updateWithFeeling (st);

    /* render new segments */
    renderSegs (st);
}
    /* erase old segments and draw new ones */
 
static void nerverot_draw(float d, struct nerverotstate *st, double alpha)
{ 
  if(d==2)
    eraseAndDraw (st);
  if(d==3)
    threed_draw(st,alpha);
  if((d>2)&&(d<3))
    twonhalfd_draw(st);

}

/* initialize the user-specifiable params */
static void initParams (struct nerverotstate *st)
{
    int problems = 0;

    st->maxIters = 1200;
    if (st->maxIters < 0)
    {
	fprintf (stderr, "error: maxIters must be at least 0\n");
	problems = 1;
    }
    st->requestedBlotCount = 250;
    if (st->requestedBlotCount <= 0)
    {
	fprintf (stderr, "error: count must be at least 0\n");
	problems = 1;
    }
    st->nervousness = 0.3;
    if ((st->nervousness < 0) || (st->nervousness > 1))
    {
	fprintf (stderr, "error: nervousness must be in the range 0..1\n");
	problems = 1;
    }
    st->maxNerveRadius = 0.7;
    if ((st->maxNerveRadius < 0) || (st->maxNerveRadius > 1))
    {
	fprintf (stderr, "error: maxNerveRadius must be in the range 0..1\n");
	problems = 1;
    }
    st->eventChance = 0.2;
    if ((st->eventChance < 0) || (st->eventChance > 1))
    {
	fprintf (stderr, "error: eventChance must be in the range 0..1\n");
	problems = 1;
    }
    st->iterAmt = 0.01;
    if ((st->iterAmt < 0) || (st->iterAmt > 1))
    {
	fprintf (stderr, "error: iterAmt must be in the range 0..1\n");
	problems = 1;
    }
    st->minScale = 0.6;
    if ((st->minScale < 0) || (st->minScale > 10))
    {
	fprintf (stderr, "error: minScale must be in the range 0..10\n");
	problems = 1;
    }
    st->maxScale = 1.75;
    if ((st->maxScale < 0) || (st->maxScale > 10))
    {
	fprintf (stderr, "error: maxScale must be in the range 0..10\n");
	problems = 1;
    }
    st->minRadius = 3;
    if ((st->minRadius < 1) || (st->minRadius > 100))
    {
	fprintf (stderr, "error: minRadius must be in the range 1..100\n");
	problems = 1;
    }
    st->maxRadius = 25;
    if ((st->maxRadius < 1) || (st->maxRadius > 100))
    {
	fprintf (stderr, "error: maxRadius must be in the range 1..100\n");
	problems = 1;
    }
    if (st->maxScale < st->minScale)
    {
	fprintf (stderr, "error: maxScale must be >= minScale\n");
	problems = 1;
    }	
    if (st->maxRadius < st->minRadius)
    {
	fprintf (stderr, "error: maxRadius must be >= minRadius\n");
	problems = 1;
    }
    if (problems)
    {
	exit (1);
    }
    st->please_num=-1;
}

static struct nerverotstate
* nerverot_init (int w, int h)
{
  struct nerverotstate *st = (struct nerverotstate *) calloc (1, sizeof(*st));

    initParams (st);
    setup (st,w,h);

    /* make a valid set to erase at first */
    renderSegs (st);
    return st;
}

static void nerverot_cycleup(struct nerverotstate *st)
{
    st->please_num++;
    if (st->please_num > 10)
	st->please_num = -1;
}
static void nerverot_cycledown(struct nerverotstate *st)
{
    st->please_num--;
    if (st->please_num < -1)
	st->please_num = 10;
}

static void
nerverot_free (struct nerverotstate *st)
{
  freeBlots (st);
  free (st);
}


