#include <png.h>

void writePNG(SDL_Surface *surface, const char *filename)
{
    // TODO Maybe someone can make this look nice?
    // No, but i can steal it!
    png_structp png_ptr;
    png_infop info_ptr;
    png_bytep *row_pointers;
    int colortype;

    if (SDL_MUSTLOCK(surface)) {
        SDL_LockSurface(surface);
    }

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
    if (!png_ptr)
    {
        printf("Had trouble creating png_structp");
        return;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
        printf("Could not create png_info");
        return;
    }

    if (setjmp(png_jmpbuf(png_ptr)))
    {
        png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
        printf("problem writing to %s", filename);
        return;
    }

    FILE *fp = fopen(filename, "wb");
    if (!fp)
    {
        printf("could not open file %s for writing", filename);
        return;
    }

    png_init_io(png_ptr, fp);

    colortype = (surface->format->BitsPerPixel == 24) ?
        PNG_COLOR_TYPE_RGB : PNG_COLOR_TYPE_RGB_ALPHA;

    png_set_IHDR(png_ptr, info_ptr, surface->w, surface->h, 8, colortype,
            PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    png_write_info(png_ptr, info_ptr);

    png_set_packing(png_ptr);

    row_pointers = ( png_bytep*)malloc(sizeof( png_bytep)*surface->h);
    if (!row_pointers)
    {
        printf("Had trouble converting surface to row pointers");
        fclose(fp);
        return;
    }
    int i;
    for (i = 0; i < surface->h; i++)
    {
        row_pointers[i] = (png_bytep)(Uint8 *)surface->pixels + i * surface->pitch;
    }

    png_write_image(png_ptr, row_pointers);
    png_write_end(png_ptr, info_ptr);

    fclose(fp);

    free( row_pointers );

    png_destroy_write_struct(&png_ptr, (png_infopp)NULL);

    if (SDL_MUSTLOCK(surface)) {
        SDL_UnlockSurface(surface);
    }
}

#ifdef GL

SDL_Surface* getScreenshot(void)
{
    SDL_Surface *mScreen;
    mScreen=SDL_GetVideoSurface();

    int h = mScreen->h;
    int w = mScreen->w;

    SDL_Surface *screenshot = SDL_CreateRGBSurface(
            SDL_SWSURFACE,
            w, h, 24,
            0xff0000, 0x00ff00, 0x0000ff, 0x000000);

    if (SDL_MUSTLOCK(screenshot))
        SDL_LockSurface(screenshot);

    // Grap the pixel buffer and write it to the SDL surface
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, screenshot->pixels);

    // Flip the screenshot, as OpenGL has 0,0 in bottom left
    unsigned int lineSize = 3 * w;
    GLubyte* buf = (GLubyte*)malloc(lineSize);

    int i;
    for (i = 0; i < (h / 2); i++)
    {
        GLubyte *top = (GLubyte*)screenshot->pixels + lineSize * i;
        GLubyte *bot = (GLubyte*)screenshot->pixels + lineSize * (h - 1 - i);

        memcpy(buf, top, lineSize);
        memcpy(top, bot, lineSize);
        memcpy(bot, buf, lineSize);
    }

    free(buf);

    if (SDL_MUSTLOCK(screenshot))
        SDL_UnlockSurface(screenshot);

    return screenshot;
}

#else



SDL_Surface * getScreenshot(void)
{
//#if SDL_BYTEORDER == SDL_BIG_ENDIAN 
   int rmask = 0xff000000;    int gmask = 0x00ff0000;    int bmask = 0x0000ff00;
//#else  
//  int rmask = 0x000000ff;    int gmask = 0x0000ff00;    int bmask = 0x00ff0000;
  //#endif 
  int amask = 0x00000000;  
  SDL_Surface *screenshot;
    printf("what?\n)");
  screenshot = SDL_CreateRGBSurface(0, SDL_GetVideoSurface()->w, SDL_GetVideoSurface()->h, 32, rmask, gmask, bmask, amask);  
//    printf("what?%i %i\n)",SDL_GetVideoSurface(), screenshot);
    printf("where?\n");
    SDL_Surface *vs;
    vs=SDL_GetVideoSurface();
    SDL_BlitSurface(vs , 0, screenshot, 0);  
    //Segmentation Fault
  printf("why??\"_'\n");

  return screenshot;
}





#endif


void saveScreenshot(void)
{
    SDL_Surface *screenshot;
    screenshot = getScreenshot();
    printf("got shot\n");
    writePNG(screenshot, "fulllemonscreenshot.png");
    printf("wrot shot\n");
    SDL_FreeSurface(screenshot);
}


