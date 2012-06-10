void gle(void)
{
#ifdef GL
	GLenum gl_error;
	gl_error = glGetError();
	if (gl_error != GL_NO_ERROR)
	{
		if (gl_error == GL_STACK_UNDERFLOW)
			logit("QUACK QUACK QUACK, UNDERFLOVING STACK\n");
		else
			if (gl_error == GL_STACK_OVERFLOW)
				logit("QUACK QUACK QUACK, OVERFLOVING STACK\n");
			else
				if (gl_error == GL_INVALID_OPERATION)
					logit("INVALID OPERATION, PATIENT EXPLODED\n");
				else
					if (gl_error == GL_INVALID_VALUE)
						logit("GL_INVALID_VALUE");
					else
						logit("OpenGL error: 0x%X\n", gl_error);
	}
#endif
}
void sdle(void)
{
	char* sdl_error;
	sdl_error = SDL_GetError();
	if (sdl_error[0] != '\0')
	{
		logit("testgl: SDL error '%s'\n", sdl_error);
		SDL_ClearError();
	}
}
