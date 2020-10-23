#include <stdio.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "Windows.h"
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"

lua_State* L;
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* texture;
SDL_Surface* gScreenSurface;
SDL_Event evt;

//函数
static int p(lua_State* l)
{
	printf("%s",(char*)lua_tostring(l,1));
	return 0;
}
//SDL
static int init(lua_State* l)
{
	char* title = (char*)lua_tostring(l,1);
	int w = (int)lua_tonumber(l,2);
	int h = (int)lua_tonumber(l,3);
	
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
    {
		printf("err1");
        return -1;
    }
	
	window = SDL_CreateWindow(title,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        w, h, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
		printf("err2");
        return -2;
    }
	gScreenSurface = SDL_GetWindowSurface(window);
	//引入image库
	IMG_Init(IMG_INIT_JPG);
	
	return 0;
}

static int img(lua_State* l)
{
	char* f = (char*)lua_tostring(l,1);
	int x1 = (int)lua_tonumber(l,2);
	int y1 = (int)lua_tonumber(l,3);
	int w1 = (int)lua_tonumber(l,4);
	int h1 = (int)lua_tonumber(l,5);
	int x2 = (int)lua_tonumber(l,6);
	int y2 = (int)lua_tonumber(l,7);
	int w2 = (int)lua_tonumber(l,8);
	int h2 = (int)lua_tonumber(l,9);
	SDL_Surface* pic = IMG_Load(f);
	Uint32 key = SDL_MapRGB(gScreenSurface->format,0xff,0xff,0xff);
	SDL_SetColorKey(pic,LWA_COLORKEY,key);
	SDL_Rect r1;
    r1.x = x1;
    r1.y = y1;
    r1.w = w1;
    r1.h = h1;
	SDL_Rect r2;
    r2.x = x2;
    r2.y = y2;
    r2.w = w2;
    r2.h = h2;
	SDL_BlitSurface(pic, &r1, gScreenSurface, &r2);
	SDL_FreeSurface(pic);
	SDL_UpdateWindowSurface(window);
	return 0;
}

static int cls(lua_State* l)
{
	SDL_RenderClear(renderer);
	return 0;
}

static int getEvt(lua_State* l)
{
	SDL_PollEvent(&evt);
	int e = evt.type;
	lua_pushnumber(l,e);
	return 1;
}

static int getKey(lua_State* l)
{
	int keyPress=-1;
	SDL_PollEvent(&evt);
	switch(evt.type)
	{
		case SDL_KEYDOWN:
			keyPress=evt.key.keysym.sym;
			break;
		case SDL_MOUSEMOTION:
			break;
		default:
			break;
	}
	lua_pushnumber(l,keyPress);
	return 1;
}

static int d(lua_State* l)
{
	int x = lua_tonumber(l,1);
	SDL_Delay(x);
	return 0;
}

static int quit(lua_State* l)
{
	SDL_Quit();
	return 0;
}

//注册结构
static const struct luaL_reg t[]={
	{"p",p},
	{"init",init},
	{"quit",quit},
	{"getEvt",getEvt},
	{"getKey",getKey},
	{"d",d},
	{"img",img},
	{"cls",cls},
	{NULL,NULL}
};

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, INT nCmdShow)
{
	L = lua_open();
	luaL_register(L,"t",t);//注册函数
	luaL_openlibs(L);
	luaL_dofile(L,"main.lua");
	lua_close(L);
	return 0;
}