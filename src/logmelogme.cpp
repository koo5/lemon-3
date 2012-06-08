





void slogit(const char * iFormat, ...) {
	char* s = (char*) malloc(101);
	va_list argp;
	va_start(argp, iFormat);
	vsnprintf(s, 101, iFormat, argp);
	s[100] = 0;
	va_end(argp);
	if (loggerface)
		loggerface->slogit(s);
}
void slogit(string iFormat, ...) {
	char* s = (char*) malloc(101);
	va_list argp;
	va_start(argp, iFormat);
	vsnprintf(s, 101, iFormat.c_str(), argp);
	s[100] = 0;
	va_end(argp);
	if (loggerface)
		loggerface->slogit(s);
}

void clogit(const char * iFormat, ...) {
	char* s = (char*) malloc(101);
	va_list argp;
	va_start(argp, iFormat);
	vsnprintf(s, 101, iFormat, argp);
	s[100] = 0;
	va_end(argp);
	if (loggerface)
		loggerface->logit(s);
}
void clogit(string iFormat, ...) {
	char* s = (char*) malloc(101);
	va_list argp;
	va_start(argp, iFormat);
	vsnprintf(s, 101, iFormat.c_str(), argp);
	s[100] = 0;
	va_end(argp);
	if (loggerface)
		loggerface->logit(s);
}

void logit(const char * iFormat, ...) {
	char* s = (char*) malloc(10101010);
	va_list argp;
	va_start(argp, iFormat);
	vsnprintf(s, 10101010, iFormat, argp);
	s[10101009] = 0;
	va_end(argp);
	if (loggerface)
		loggerface->logit(s);
	else
		printf("%s\n", s);
	free(s);
}

void logit(string iFormat, ...) {
	char* s = (char*) malloc(10101010);
	va_list argp;
	va_start(argp, iFormat);
	vsnprintf(s, 10101010, iFormat.c_str(), argp);
	s[10101009] = 0;
	va_end(argp);
	if (loggerface)
		loggerface->logit(s);
	else
		printf("%s\n", s);
	free(s);
}

void plogit(const char * iFormat, ...) {
	char* s = (char*) malloc(10101010);
	va_list argp;
	va_start(argp, iFormat);
	vsnprintf(s, 10101010, iFormat, argp);
	s[10101009] = 0;
	va_end(argp);
	printf("%s\n", s);
	free(s);
}

void plogit(string iFormat, ...) {
	char* s = (char*) malloc(10101010);
	va_list argp;
	va_start(argp, iFormat);
	vsnprintf(s, 10101010, iFormat.c_str(), argp);
	s[10101009] = 0;
	va_end(argp);
	printf("%s\n", s);
	free(s);
}
