/*
 * events.cpp
 *
 *  Created on: Jun 8, 2012
 *      Author: kook
 */


const int super_key = SDLK_MENU;//SDLK_RCTRL, SDLK_RMETA
int pre_escaped = 0;
int escape_used = 1;

int control(SDLKey key, int uni, int mod)
{
	switch (key)
	{
		case SDLK_TAB:
		case SDLK_PAGEDOWN:
		case SDLK_PAGEUP:
			dirty = 1;
			if ((key==SDLK_PAGEUP) || (mod & (KMOD_RSHIFT|KMOD_LSHIFT)))
			{
			    for(int i=objects.size()-1;i>-1;i--) 
			    { 
				obj*o=objects.at(i);
				if (active == o)
				{
					if (--i == -1) i = objects.size()-1;
					objects.at(i)->activate();
					cam.focus(active->t);
					break;
				}
			    }
			}
			else
			{
			    for_each_object
				if (active == o)
				{
					if (++i == objects.size()) i = 0;
					objects.at(i)->activate();
					cam.focus(active->t);
					break;
				}
			    endf
			}
			if (!active && objects.size()) objects.at(0)->activate();
			break;
		case SDLK_f:
		case SDLK_F11:
			if (mod & KMOD_RSHIFT)
			{
				if (dynamic_cast<face*>(active))
				{
					face *f = dynamic_cast<face*>(active);
					rote_vt_resize(f->t, 28, 160);
				}
			}
			else
				gofullscreen = 1;
			break;
		case SDLK_r:
			loadfont(fnfl);
			break;
		case SDLK_n:
			objects.push_back(new face);
			active = objects.at(objects.size() - 1);
			break;
		case SDLK_t:
			objects.push_back(active = new face(rightmost()->t.x + 2, 0, 0));
			cam.focus(active->t);
			dirty = 1;
			break;
		case SDLK_UP:
		{
//			cam.self.move(v3d(cam.self.dst.x,cam.self.dst.y,cam.self.dst.z+1));
			dirty = 1;
			break;
		}
		case SDLK_DOWN:
		{
//			cam.self.move(v3d(cam.self.dst.x,cam.self.dst.y,cam.self.dst.z-1));
			dirty = 1;
			break;
		}
		case SDLK_LEFT:
		{
			//if (active)
//			{
//			    cam.focus(new v3d(active.t.x-1,active.t.y,active.t.z))
			dirty = 1;
			break;
		}
		case SDLK_RIGHT:
		{
			//if (active) active->spin.z -= 0.03;
			dirty = 1;
			break;
		}
			/*			case SDLK_v:
			 {
			 cr.y += 3.4;
			 dirty = 1;
			 break;
			 }
			 case SDLK_c:
			 {
			 cr.y -= 3.4;
			 dirty = 1;
			 break;
			 }
			 */
		case SDLK_F5:
			if (active)
			{
				active->s.x = active->s.x * 0.95;
				active->s.y = active->s.y * 0.95;
				dirty = 1;
			}
			break;
		case SDLK_F6:
			if (active)
			{
				active->s.x = active->s.x * 1.05;
				active->s.y = active->s.y * 1.05;
				dirty = 1;
			}
			break;
		case SDLK_EQUALS:
			if (active)
			{
				active->alpha += 0.05;
				if (active->alpha > 1) active->alpha = 1;
				break;
			}
			break;
		case SDLK_MINUS:
			if (active)
			{
				active->alpha -= 0.05;
				if (active->alpha < 0) active->alpha = 0;
				break;
			}
			break;
		case SDLK_h:
			settingz.givehelp = !settingz.givehelp;
			break;
		default:
			return 0;
	}
	return 1;
}

void process_event(SDL_Event event)
{
//	dirty = 1;
	switch (event.type)
	{
		case SDL_MOUSEMOTION:
			lastmousemoved = SDL_GetTicks();
			mousejustmoved = 1;
			break;
//		case SDL_MOUSEWHEEL:
//			event.wheel
//			break;
		case SDL_KEYUP:
			if (event.key.keysym.sym == super_key)
			{
				escaped = 0;
				if (!escape_used)
					pre_escaped = 1;
			}
			break;
		case SDL_KEYDOWN:
		{
			SDLKey key= event.key.keysym.sym;
			int uni = event.key.keysym.unicode;
			int mod = event.key.keysym.mod;
		
			logit("key %d uni %d mod %x", key, uni, mod);

			if (key == super_key)
			{
				escaped = 1;
				escape_used = 0;
			}
			else if (escaped || pre_escaped)
			{
				if (!control(key, uni, mod))
					if (active) active->keyp(key, uni, mod, 1);
				escape_used = 1;
				pre_escaped = 0;
			}
			else
				if (active) active->keyp(key, uni, mod, 0);
			break;
		}
		case SDL_QUIT:
			done = 1;
			break;
		case SDL_VIDEOEXPOSE:
			dirty = 1;
			break;
		case SDL_VIDEORESIZE:
			w = event.resize.w;
			h = event.resize.h;
			logit("videoresize %ix%i", w, h);
			dirty = 1;
			s = SDL_SetVideoMode(w, h, bpp, s->flags);
			resetviewport();
			dirty = 1;
			break;
		case SDL_USEREVENT:
			if (event.user.code == CODE_QUIT)
			{
				for_each_face
						if (f->t == event.user.data1) erase(i);
					endfendf
				dirty = 1;
			}
			else
				if (event.user.code == CODE_FNFLCHANGED)
					loadfont(fnfl);
				else
					if (event.user.code == CODE_DATA)
					{
						//do nothing. be happy that we broke out of WaitEvent and could redraw ourselves
					}
					else
						if (event.user.code == CODE_INPIPE)
						{
							_mutexP(inpipemutex);
							if (inform.size() > 0)
							{
								cout << inform.front();
								inform.erase(inform.begin());
							}
							_mutexV(inpipemutex);
						}
			break;
	}
}

