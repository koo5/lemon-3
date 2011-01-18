

int tex;
int material=0;
int  firstline;
int have_first=0;
typedef struct
{
    void * buf;
    int siz;
    int pos;
    int dat;
    int pep;
}
abuffer;

abuffer vb;
abuffer ib;

void nulizze(abuffer * b)
{
//	b->siz=0;
    b->pos=0;
//	b->buf=0;
}



void frees(void)
{
    if (ib.buf)
        free(ib.buf);
    if (vb.buf)
        free(vb.buf);
}

void * realloc_abuffer(abuffer * b)
{
    void *tmp;
    if (tmp=realloc(b->buf, (b->siz+5000)*b->dat))
    {
        b->buf=tmp;
        b->siz+=5000;
//		printf("%i\n", b->siz);
    }
    return tmp;
}
int grow(abuffer * b)
{

    int r= b->pos < b->siz || realloc_abuffer(b);
    if (!r)
    {
        printf ("no realoco\n");
        exit(0);
    }
    return r;

}
void add(abuffer * b)
{
    b->pos++;
}
void addvert(float x,float y, float z)
{

    if (!grow(&vb))
        return;
    float*p=vb.buf;
    p+=vb.pos*3;
    p[0]=x;
    p[1]=y;
    p[2]=z;
    add(&vb);
}
void addinde(int a,int b)
{

    if (!grow(&ib))
        return;
    uint32_t *p=ib.buf;
    p+=ib.pos*3;
    p[0]=a;
    p[1]=b;
    p[2]=material;
    add(&ib);
}
void initbufs(void )
{
    vb.siz=0;
    ib.siz=0;
    vb.pep=ib.pep=0;
    vb.buf=ib.buf=0;
    nulizze(&vb);
    nulizze(&ib);
    vb.dat=sizeof(float)*3;
    ib.dat=sizeof(int)*3;
}



void dooooot(float x,float y)
{
    static float wtf[4];
    int j;
    for (j=0;j<4;j++)
	wtf[j]=!wtf[j];
    for (j=0;j<have_first+wtf[0]+wtf[1]+wtf[2]+wtf[3];j++)
	wtf[j]=!wtf[j];
    for (j=0;j<4;j++)
	wtf[j]=wtf[j]/50;

//    x=x*xoom;
 //   y=y*yoom;
    x+=wtf[0]-wtf[2];
    y+=wtf[1]-wtf[3];
    static float firstx, firsty;
    if (have_first)
    {
        addvert(x,y,0);
        addinde(vb.pos-1,vb.pos-2);
        firstx=x;
        firsty=y;
    }
    else
    {
        firstx=x;
        firsty=y;
        have_first=1;
        addvert(x,y,0);
    }
}


void bpep()
{
    s3d_pop_vertex(tex,vb.pep);
    s3d_push_vertices(tex, vb.buf, vb.pos);

    if (1)
    {
	s3d_pop_line(tex,ib.pep);
	s3d_push_lines(tex, ib.buf, ib.pos);
	ib.pep=ib.pos;

    }
    else
    {
	s3d_pap_lines(tex,ib.buf, ib.pos);
    }
    vb.pep=vb.pos;
    nulizze(&ib);
    nulizze(&vb);
}

void glColor4d(double a,double b,double c,double d)
{

}