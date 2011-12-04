#define CODE_INPIPE 4

SDL_mutex * inpipemutex;
vector<string> inform;

int inpipe(void*)
{
while(cin)
{
string x;
cin >> x;
if(cin)
{
_mutexP(inpipemutex);
inform.push_back(x);
_mutexV(inpipemutex);
SDL_Event e;
e.type=SDL_USEREVENT;
e.user.code=CODE_INPIPE;
SDL_PushEvent(&e);
}
}
//cout << "inpipe ended" << endl;
return 0xc0de;
}
