#include <xmms/plugin.h>
#include "config.h"
#include <stdio.h>

static void render_freq(gint16 freq_data[2][256]);

VisPlugin example2_vp =
{
 NULL,NULL,0,
 "Autotools example 2",
 0,1,
 NULL,
 NULL,
 NULL,
 NULL,
 NULL,
 NULL,
 NULL,
 NULL,
 render_freq
};

VisPlugin *get_vplugin_info(void) {
 printf("Data directory: %s\n",DATADIR);
 return &example2_vp; 
}


static void render_freq(gint16 freq_data[2][256])
{
     FILE *f;
     if(f=fopen("/tmp/somefunnyname", "w"))
     {
        fwrite(freq_data,2 , 512, f);
	fclose(f);
     }
}
  