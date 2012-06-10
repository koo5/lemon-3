
void resetviewport(void)
{
	glViewport(0, 0, SDL_GetVideoSurface()->w, SDL_GetVideoSurface()->h);
}

void viewmatrix()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glFrustum(1, -1, 1, -1, 1, 20);
	gluPerspective(90,1,1,20);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(  cam.now.self.x, cam.now.self.y,
				cam.now.self.z, cam.now.look.x,
				cam.now.look.y, cam.now.look.z, 0, -1, 0);

	plogit("gluLookAt(%lf, %lf, %lf, %lf, %lf, %lf", cam.now.self.x, cam.now.self.y, cam.now.self.z, cam.now.look.x,
			cam.now.look.y, cam.now.look.z);
}

void resetmatrices(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

