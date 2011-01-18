void listdir(char *path, void func(char *, char *, void* ), void *data)
{
	DIR *dir = opendir(path);
	if(dir)
	{	
		path=strdup(path);
		unsigned int maxsize = 50;
		path=(char*)realloc(path, strlen(path)+maxsize+1);
		char* n=strrchr(path, 0);
		struct dirent *ent;
		while((ent = readdir(dir)) != NULL)
		{
			if(strlen(ent->d_name) < maxsize)
			{
				strcat(path,ent->d_name);
				func(path,ent->d_name,data);
				*n=0;
			}
		}
		free(path);
	}
	else
	{
		logit("Error opening directory\n");
	}
}