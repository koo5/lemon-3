/* flipflop, Copyright (c) 2003 Kevin Ogden <kogden1@hotmail.com>
 *                     (c) 2006 Sergio Gutiérrez "Sergut" <sergut@gmail.com>
 *                     (c) 2008 Andrew Galante <a.drew7@gmail.com>
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation.  No representations are made about the suitability of this
 * software for any purpose.  It is provided "as is" without express or
 * implied warranty.
 *
 *
 * 2003 Kevin Odgen                  First version
 * 2006 Sergio Gutiérrez "Sergut"    Made several parameters dynamic and selectable
 *                                   from the command line: size of the board, 
 *                                   rotation speed and number of free squares; also
 *                                   added the "sticks" mode.
 * 2008 Andrew Galante               Added -textured option: textures the board with
 *                                   an image which gets scrambled as the tiles move
 *
 */

#define DEF_SPIN           "0.1"   /* Default angular velocity: PI/10 rads/s    */

#define DEF_TILE_THICK     4       /* Thickness for the tiles mode (over 100)   */
#define DEF_TILE_RATIO    95       /* Ratio of tiles/total squares (over 100)   */

#undef countof
#define countof(x) (sizeof((x))/sizeof((*x)))
#define progname "flipflop"
class flipflop: public obj
{

int board_x_size, board_y_size, board_avg_size;
int numsquares, freesquares;
float half_thick;
float spin;
int lastticks;

    /* array specifying which squares are where (to avoid collisions) */
    /* -1 means empty otherwise integer represents square index 0 - n-1 */
	/* occupied[x*board_y_size+y] is the tile [x][y] (i.e. that starts at column x and row y)*/
    int *occupied; /* size: size_x * size_y */
    /* an array of xpositions of the squares */
    int *xpos; /* size: numsquares */
    /* array of y positions of the squares */
    int *ypos; /* size: numsquares */
    /* integer representing the direction of movement of a square */
    int *direction; /* 0 not, 1 x+, 2 y+, 3 x-, 4 y-*/ /* size: numsquares */
    /* angle of moving square (during a flip) */
    float *angle; /* size: numsquares */
    /* array of colors for a square (RGB) */
    /* eg. color[ 3*3 + 0 ] is the red   component of square 3 */
    /* eg. color[ 4*3 + 1 ] is the green component of square 4 */
    /* eg. color[ 5*3 + 2 ] is the blue  component of square 5 */
    /*            ^-- n is the number of square */
    float *color; /* size: numsquares * 3 */
    /* array of texcoords for each square */
    /* tex[ n*4 + 0 ] is x texture coordinate of square n's left side */
    /* tex[ n*4 + 1 ] is y texture coordinate of square n's top side */
    /* tex[ n*4 + 2 ] is x texture coordinate of square n's right side */
    /* tex[ n*4 + 3 ] is y texture coordinate of square n's bottom side */

    float flipspeed;  /* amount of flip;  1 is a entire flip           */
    float energy;     /* likelyhood that a square will attempt to move */


/* configure lighting */
void predraw(void)
{
    /*   GLfloat position0[] = { board_avg_size*0.5, board_avg_size*0.1, board_avg_size*0.5, 1.0 }; */

    /*   GLfloat position0[] = { -board_avg_size*0.5, 0.2*board_avg_size, -board_avg_size*0.5, 1.0 }; */
    GLfloat position0[4];
    position0[0] = 0;
    position0[1] = board_avg_size*0.3;
    position0[2] = 0;
    position0[3] = 1;

    GLfloat amb[] = { 0.8, 0.8, 0.8, 1.0 };

    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.2);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.15/board_avg_size );
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.15/board_avg_size );
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb);

    glEnable(GL_LIGHTING);
    glLightfv(GL_LIGHT0, GL_POSITION, position0);
    glEnable(GL_LIGHT0);
    
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);


}
public:

  SAVE(flipflop)
    {
	YAML_EMIT_PARENT_MEMBERS(out,obj)
    }
    LOAD
    {
    	YAML_LOAD_PARENT_MEMBERS(doc,obj)
    }
  
/* draw board */
void draw(int picking)
{
    glTranslatef(-0.5*board_x_size, 0.0, -0.5*board_y_size); /* Center the board */

    if(!picking)
    {
	int i;
	energy = 40*maxvol+1;
	for( i=0; i < (energy) ; i++ ) {
    	    randsheet_new_move();
	}
	randsheet_move( 3.14159 * flipspeed*(float)(SDL_GetTicks()-lastticks));
	lastticks=SDL_GetTicks();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	predraw();
    }
    randsheet_draw();
    if(!picking)
	glPopAttrib();
}



flipflop()
{
    board_x_size=board_y_size=9;
    board_avg_size = (board_x_size + board_y_size) / 2;
    freesquares=0;
    numsquares=0;
    if ((numsquares == 0) && (freesquares != 0)) {
        numsquares = board_x_size * board_y_size - freesquares; 
    }
    half_thick = 1.0 * DEF_TILE_THICK / 100.0; 
    if (numsquares == 0) {  /* No value defined by user */
        numsquares = board_x_size * board_y_size * DEF_TILE_RATIO/ 100;;
    }
    if (board_avg_size < 2) {
        fprintf (stderr,"%s: the board must be at least 2x2.\n", progname);
        exit(1);
    }
    if ((board_x_size < 1) || (board_y_size < 1) ||	(numsquares < 1)) {
        fprintf (stderr,"%s: the number of elements ('-count') and the dimensions of the board ('-size-x', '-size-y') must be positive integers.\n", progname);
        exit(1);
    }
    if (board_x_size * board_y_size <= numsquares) {
        fprintf (stderr,"%s: the number of elements ('-count') that you specified is too big \n for the dimensions of the board ('-size-x', '-size-y'). Nothing will move.\n", progname);
    }
    flipspeed = 0.0003;
    energy = 40*maxvol+1;
    randsheet_create(); 
    randsheet_initialize();
    lastticks=SDL_GetTicks();
}



void draw_sheet()
{
    glBegin( GL_QUADS );

    glNormal3f( 0, -1, 0 );
    glVertex3f( half_thick,  -half_thick,  half_thick );
    glVertex3f( 1-half_thick,   -half_thick, half_thick );
    glVertex3f( 1-half_thick, -half_thick,  1-half_thick);
    glVertex3f( half_thick, -half_thick, 1-half_thick );

    /* back */
    glNormal3f( 0, 1, 0 );
    glVertex3f( half_thick, half_thick, 1-half_thick );
    glVertex3f( 1-half_thick, half_thick,  1-half_thick);
    glVertex3f( 1-half_thick,   half_thick, half_thick );
    glVertex3f( half_thick,  half_thick,  half_thick );

    /* 4 edges!!! weee.... */
    glNormal3f( 0, 0, -1 );
    glVertex3f( half_thick, half_thick, half_thick );
    glVertex3f( 1-half_thick, half_thick, half_thick );
    glVertex3f( 1-half_thick, -half_thick, half_thick );
    glVertex3f( half_thick, -half_thick, half_thick );
    glNormal3f( 0, 0, 1 );
    glVertex3f( half_thick, half_thick, 1-half_thick );
    glVertex3f( half_thick, -half_thick, 1-half_thick );
    glVertex3f( 1-half_thick, -half_thick, 1-half_thick );
    glVertex3f( 1-half_thick, half_thick, 1-half_thick );
    glNormal3f( 1, 0, 0 );
    glVertex3f( 1-half_thick, half_thick, 1-half_thick );
    glVertex3f( 1-half_thick, -half_thick, 1-half_thick );
    glVertex3f( 1-half_thick, -half_thick, half_thick );
    glVertex3f( 1-half_thick, half_thick, half_thick );
    glNormal3f( -1, 0, 0 );
    glVertex3f( half_thick, half_thick, 1-half_thick );
    glVertex3f( half_thick, half_thick, half_thick );
    glVertex3f( half_thick, -half_thick, half_thick );
    glVertex3f( half_thick, -half_thick, 1-half_thick );
    glEnd();
}

/* Reserve memory for the randsheet */
void randsheet_create()
{
	occupied  = (int*) malloc(board_x_size*board_y_size * sizeof(int));
	xpos      = (int*) malloc(numsquares * sizeof(int));
	ypos      = (int*) malloc(numsquares * sizeof(int));
	direction = (int*) malloc(numsquares * sizeof(int));
	angle     = (float*) malloc(numsquares * sizeof(float));
	color     = (float*) malloc(numsquares*3 * sizeof(float));
}

~flipflop()
{
	free(occupied);
	free(xpos);
	free(ypos);
	free(direction);
	free(angle);
	free(color);
}

void randsheet_initialize()
{
    int i, j, index;
    index = 0;
    /* put the moving sheets on the board */
    for( i = 0; i < board_x_size; i++ )
        {
            for( j = 0; j < board_y_size; j++ )
                {
                    /* initially fill up a corner with the moving squares */
                    if( index < numsquares )
                        {
                            occupied[ i * board_y_size + j ] = index;
                            xpos[ index ] = i;
                            ypos[ index ] = j;
							/* have the square colors start out as a pattern */
							color[ index*3 + 0 ] = ((i+j)%3 == 0)||((i+j+1)%3 == 0);
							color[ index*3 + 1 ] = ((i+j+1)%3 == 0);
							color[ index*3 + 2 ] = ((i+j+2)%3 == 0);
                            index++;
                        }
                    /* leave everything else empty*/
                    else
                        {
                            occupied[ i * board_y_size + j ] = -1;
                        }
                }
        }
    /* initially everything is at rest */
    for( i=0; i<numsquares; i++ )
        {
            direction[ i ] = 0;
            angle[ i ] = 0;
        }
}

/* Pick and random square and direction and try to move it. */
/* May not actually move anything, just attempt a random move. */
/* Returns true if move was sucessful. */
/* This could probably be implemented faster in a dequeue */
/* to avoid trying to move a square which is already moving */
/* but speed is most likely bottlenecked by rendering anyway... */
int randsheet_new_move()
{
    int i, j;
    int num, dir;
    /* pick a random square */
    num = random( ) % numsquares;
    i = xpos[ num ];
    j = ypos[ num ];
    /* pick a random direction */
    dir = ( random( )% 4 ) + 1;

    if( direction[ num ] == 0 )
        {
            switch( dir )
                {
                case 1:
                    /* move up in x */
                    if( ( i + 1 ) < board_x_size )
                        {
                            if( occupied[ (i + 1) * board_y_size + j ] == -1 )
                                {
                                    direction[ num ] = dir;
                                    occupied[ (i + 1) * board_y_size + j ] = num;
                                    occupied[ i * board_y_size + j ] = -1;
                                    return 1;
                                }
                        }
                    return 0;
                    break;
                case 2:
                    /* move up in y */
                    if( ( j + 1 ) < board_y_size )
                        {
                            if( occupied[ i * board_y_size + (j + 1) ] == -1 )
                                {
                                    direction[ num ] = dir;
                                    occupied[ i * board_y_size + (j + 1) ] = num;
                                    occupied[ i * board_y_size + j ] = -1;
                                    return 1;
                                }
                        }
                    return 0;
                    break;
                case 3:
                    /* move down in x */
                    if( ( i - 1 ) >= 0 )
                        {
                            if( occupied[ (i - 1) * board_y_size + j ] == -1 )
                                {
                                    direction[ num ] = dir;
                                    occupied[ (i - 1) * board_y_size + j ] = num;
                                    occupied[ i * board_y_size + j ] = -1;
                                    return 1;
                                }
                        }
                    return 0;
                    break;
                case 4:
                    /* move down in y */
                    if( ( j - 1 ) >= 0 )
                        {
                            if( occupied[ i * board_y_size + (j - 1) ] == -1 )
                                {
                                    direction[ num ] = dir;
                                    occupied[ i * board_y_size + (j - 1) ] = num;
                                    occupied[ i * board_y_size + j ] = -1;
                                    return 1;
                                }
                        }
                    return 0;
                    break;
                default:
                    break;
                }
        }
    return 0;
}

/*   move a single frame.  */
/*   Pass in the angle in rads the square rotates in a frame. */
void randsheet_move( float rot )
{
    int i, j, index;
    for( index = 0 ; index < numsquares; index++ )
        {
            i = xpos[ index ];
            j = ypos[ index ];
            switch( direction[ index ] )
                {
                case 0:
                    /* not moving */
                    break;
                case 1:
                    /* move up in x */
                    angle[ index ] += rot;
                    /* check to see if we have finished moving */
                    if( angle[ index ] >= M_PI  )
                        {
                            xpos[ index ] += 1;
                            direction[ index ] = 0;
                            angle[ index ] = 0;
                        }
                    break;
                case 2:
                    /* move up in y */
                    angle[ index ] += rot;
                    /* check to see if we have finished moving */
                    if( angle[ index ] >= M_PI  )
                        {
                            ypos[ index ] += 1;
                            direction[ index ] = 0;
                            angle[ index ] = 0;
                        }
                    break;
                case 3:
                    /* down in x */
                    angle[ index ] += rot;
                    /* check to see if we have finished moving */
                    if( angle[ index ] >= M_PI  )
                        {
                            xpos[ index ] -= 1;
                            direction[ index ] = 0;
                            angle[ index ] = 0;
                        }
                    break;
                case 4:
                    /* down in y */
                    angle[ index ] += rot;
                    /* check to see if we have finished moving */
                    if( angle[ index ] >= M_PI  )
                        {
                            ypos[ index ] -= 1;
                            direction[ index ] = 0;
                            angle[ index ] = 0;
                        }
                    break;
                default:
                    break;
                }
        }
}


/* draw all the moving squares  */
void randsheet_draw()
{
    int i, j;
    int index;

    /* for all moving squares ... */
    for( index = 0; index < numsquares; index++ )
        {
            /* set color */
            glColor3f( color[ index*3 + 0 ],
                       color[ index*3 + 1 ],
                       color[ index*3 + 2 ] );
            /* find x and y position */
            i = xpos[ index ];
            j = ypos[ index ];
            glPushMatrix();
            switch( direction[ index ] )
                {
                case 0:

                    /* not moving */
                    /* front */
                    glTranslatef( i, 0, j );
                    break;
                case 1:
                    glTranslatef( i+1, 0, j );
                    glRotatef( 180 - angle[ index ]*180/M_PI, 0, 0, 1 );

                    break;
                case 2:
                    glTranslatef( i, 0, j+1 );
                    glRotatef( 180 - angle[ index ]*180/M_PI, -1, 0, 0 );

                    break;
                case 3:
                    glTranslatef( i, 0, j );
                    glRotatef( angle[ index ]*180/M_PI, 0, 0, 1 );
                    break;
                case 4:
                    glTranslatef( i, 0, j );
                    glRotatef( angle[ index ]*180/M_PI, -1, 0, 0 );
                    break;
                default:
                    break;
                }
            draw_sheet();
            glPopMatrix();
        }
}

};

