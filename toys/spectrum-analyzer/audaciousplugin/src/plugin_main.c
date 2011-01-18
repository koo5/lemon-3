#include <audacious/plugin.h>
#include "config.h"



static void freq(gint16 data[2][256])
{
     FILE *f;
     if((f=fopen("/tmp/somefunnyname", "w")))
     {
        fwrite(data,2 , 512, f);
	fclose(f);
     }
}



VisPlugin fsanalyzer_vp = {
	.description = "cat freq > /tmp/somefunnyname",
	.num_pcm_chs_wanted = 0,
	.num_freq_chs_wanted = 1,
	.render_freq = freq  /* render_freq */
};

VisPlugin *spectrum_vplist[] = { &fsanalyzer_vp, NULL };

DECLARE_PLUGIN(funnyname, NULL, NULL, NULL, NULL, NULL, NULL, spectrum_vplist,NULL);

