/***************************************************************
* Star Snow Notes
* Copyright (c) 2012 YOSHIMURA Tomohiko All rights reserved.
* 
* Created by bowkenken
*   URL: https://sourceforge.jp/projects/lnl/
* 
* License is GPL
* 
* 本プログラムはフリー・ソフトウェアです。
* あなたは、 Free Software Foundation が公表した
*  GNU 一般公有使用許諾の「バージョン２」
* 或はそれ以降の各バージョンの中からいずれかを選択し、
* そのバージョンが定める条項に従って本プログラムを
* 再頒布または変更することができます。
* 
* 本プログラムは有用とは思いますが、頒布にあたっては、
* 市場性及び特定目的適合性についての暗黙の保証を含めて,
* いかなる保証も行ないません。
* 詳細については GNU 一般公有使用許諾書をお読みください。
* 
* あなたは、本プログラムと一緒に GNU 一般公有使用許諾書
* の写しを受け取っているはずです。そうでない場合は、
*   Free Software Foundation, Inc.,
*   59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
* へ手紙を書いてください。
* 
* $Id:$
***************************************************************/

#include <signal.h>

#include "GameMain.h"
#include "Option.h"
#include "FileList.h"
#include "Space.h"
#include "ControlPanel.h"

using namespace StarSnowNotes;

////////////////////////////////////////////////////////////////
// 定数
////////////////////////////////////////////////////////////////

const long SCREEN_DEFAULT_W = 640;
const long SCREEN_DEFAULT_H = 480;

// 描画時の基準画面サイズ
static const long gBaseScreenWidth = 640;
static const long gBaseScreenHeight = 480;

const double gReduceNum = 160.0;

////////////////////////////////////////////////////////////////
// 変数
////////////////////////////////////////////////////////////////

long gScreenW = SCREEN_DEFAULT_W;
long gScreenH = SCREEN_DEFAULT_H;

Option *gOption, *gSetting;
Space *gMainSpace;
StarSnowNotes::ControlPanel *gMainControlPanel;

////////////////////////////////////////////////////////////////
// プロトタイプ
////////////////////////////////////////////////////////////////

static void initScreen(int *argc, char **argv);
static void mainLoop();
void eventMouseMotion(long x, long y);
void eventMouseButton(bool flagDown, long btn);
static void signal_handler(int sig);

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
	signal( SIGHUP, signal_handler );
	signal( SIGINT, signal_handler );
	signal( SIGKILL, signal_handler );
	signal( SIGBUS, signal_handler );
	signal( SIGSEGV, signal_handler );
	signal( SIGABRT, signal_handler );
	signal( SIGTERM, signal_handler );

	srand((int)time(NULL));

	gOption = new Option;
	gOption->init();
	gOption->parseOption(argc, argv);

	gSetting = new Option;
	gSetting->init();
	*gSetting = *gOption;

	gSetting->saveAllConfig();

	::initScreen(&argc, argv);

	gMainSpace = new Space();
	gMainSpace->init(*gSetting);
	gMainSpace->setFlagFreezed(false);
	gMainSpace->setFlagDraw(true);

	gMainControlPanel = new StarSnowNotes::ControlPanel();
	gMainControlPanel->init(*gSetting);
	gMainControlPanel->setFlagFreezed(false);
	gMainControlPanel->setFlagDraw(true);

	long r = 100;
	long g = 200;
	long b = 200;
	long a = 255;
	Button::setColorFrame(Button::COLOR_IDX_NOT_POINTED,
		r / 2, g / 2, b / 2, a);
	Button::setColorRect(Button::COLOR_IDX_NOT_POINTED,
		r * 0, g * 0, b * 0, a * 0);
	Button::setColorFrame(Button::COLOR_IDX_POINTED,
		r, g, b, a);
	Button::setColorRect(Button::COLOR_IDX_POINTED,
		r * 0, g * 0, b * 0, a * 0);
	Button::setColorFrame(Button::COLOR_IDX_PRESSED,
		r, g, b, a);
	Button::setColorRect(Button::COLOR_IDX_PRESSED,
		r / 2, g / 2, b / 2, a / 2);

	gMainSpace->reshape(gScreenW, gScreenH);
	gMainControlPanel->reshape(gScreenW, gScreenH);

	::mainLoop();

	::exitGame(EXIT_SUCCESS);
	return(EXIT_FAILURE);
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////

static void initScreen(int *argc, char **argv)
{
	if (::SDL_Init(SDL_INIT_VIDEO) <= -1) {
		fprintf(stderr, "Error: Initialize SDL: %s\n",
			::SDL_GetError());
		exit(EXIT_FAILURE);
	}

	const SDL_VideoInfo *vInfo = SDL_GetVideoInfo();
	if (vInfo == NULL) {
		fprintf(stderr, "Error: Initialize SDL: %s\n",
			::SDL_GetError());
		exitGame(EXIT_FAILURE);
	}

	::glutInit(argc, argv);

	::SDL_WM_SetCaption("Star Snow Notes", NULL);

	::SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	long bpp = vInfo->vfmt->BitsPerPixel;
	SDL_Surface *screenSf = ::SDL_SetVideoMode(
		gScreenW, gScreenH, bpp, SDL_OPENGL);
	if (screenSf == NULL) {
		fprintf(stderr, "Error: Initialize Screen: %s\n",
			::SDL_GetError());
		exitGame(EXIT_FAILURE);
	}

	::glClearColor(0.0, 0.0, 0.0, 1.0);
	::glOrtho(0.0, gScreenW, gScreenH, 0.0, -1.0, 1.0);
	::glEnable(GL_DEPTH);
	::glEnable(GL_TEXTURE_2D);
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////

GLuint loadTexture(const char *fileName, double *w, double *h)
{
	if (fileName == NULL)
		return 0;

	SDL_Surface *sf1 = ::IMG_Load(fileName);
	if (sf1 == NULL) {
		fprintf(stderr, "Error: Load file '%s': %s\n",
			fileName, ::SDL_GetError());
		return 0;
	}

	SDL_Surface *sf2 = sf1;
#if 0 //@@@
	sf2 = ::SDL_CreateRGBSurface(
		SDL_SWSURFACE,
		lToPow2(sf1->w), lToPow2(sf1->h), 32,
		0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
	if (sf2 == NULL) {
		fprintf(stderr, "Error: Create surface: %s\n",
			::SDL_GetError());
		::SDL_FreeSurface(sf1);
		return 0;
	}
	::SDL_BlitSurface(sf1, NULL, sf2, NULL);
#endif

	GLuint texName = 0;
	::glGenTextures(1, &texName);
	::glBindTexture(GL_TEXTURE_2D, texName);
	::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	SDL_PixelFormat *fmt = sf2->format;
	if (fmt->Amask) {
		::gluBuild2DMipmaps(GL_TEXTURE_2D, 4,
			sf2->w, sf2->h,
			GL_RGBA, GL_UNSIGNED_BYTE, sf2->pixels);
	} else {
		::gluBuild2DMipmaps(GL_TEXTURE_2D, 3,
			sf2->w, sf2->h,
			GL_RGB, GL_UNSIGNED_BYTE, sf2->pixels);
	}
	//::glTexImage2D(GL_TEXTURE_2D, 0, 4, sf2->w, sf2->h, 0, GL_RGBA,
	//	GL_UNSIGNED_BYTE, sf2->pixels);

	if (w != NULL)
		*w = sf2->w;
	if (h != NULL)
		*h = sf2->h;

	if (sf1 == sf2) {
		::SDL_FreeSurface(sf1);
	} else {
		::SDL_FreeSurface(sf2);
		::SDL_FreeSurface(sf1);
	}

	return texName;
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////

static void mainLoop()
{
	if (gMainSpace == NULL)
		return;
	if (gMainControlPanel == NULL)
		return;

	double nextTicks = ::SDL_GetTicks();
	double waitDeltaTicks = 1000.0
		/ gSetting->getNum(OPTION_IDX_FPS);

	while (1) {
		SDL_Event ev;
		while (::SDL_PollEvent(&ev)) {
			switch (ev.type) {
			case SDL_QUIT:
				return;
			case SDL_KEYDOWN:
				eventKey(true, ev.key.keysym.sym,
					ev.key.keysym.mod);
				break;
			case SDL_KEYUP:
				eventKey(false, ev.key.keysym.sym,
					ev.key.keysym.mod);
				if (ev.key.keysym.sym == SDLK_ESCAPE)
					return;
				break;
			case SDL_MOUSEMOTION:
				eventMouseMotion(ev.motion.x, ev.motion.y);
				break;
			case SDL_MOUSEBUTTONDOWN:
				eventMouseButton(true, ev.button.button);
				break;
			case SDL_MOUSEBUTTONUP:
				eventMouseButton(false, ev.button.button);
				break;
			}
		}

		if (::SDL_GetTicks() < nextTicks) {
			::SDL_Delay(1);
		} else {
			gMainSpace->update();
			gMainControlPanel->update();

			gMainSpace->draw();
			gMainControlPanel->draw();

			::glFlush();
			::SDL_GL_SwapBuffers();

			nextTicks += waitDeltaTicks;

			::SDL_Delay(1);
		}
	}
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////

void exitGame(int exitCode)
{
	::closeGame(exitCode);
	::exit(exitCode);
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////

void closeGame(int exitCode)
{
	if (gMainSpace != NULL)
		delete gMainSpace;
	if (gMainControlPanel != NULL)
		delete gMainControlPanel;

	::SDL_Quit();
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////

void eventKey(bool flagDown, SDLKey key, SDLMod mod)
{
	switch (key) {
	case SDLK_UP:
		key = SDLK_8;
		break;
	case SDLK_DOWN:
		key = SDLK_2;
		break;
	case SDLK_LEFT:
		key = SDLK_4;
		break;
	case SDLK_RIGHT:
		key = SDLK_6;
		break;
	case SDLK_KP_ENTER:
		key = SDLK_RETURN;
		break;
	case SDLK_DELETE:
	case SDLK_KP0:
		key = SDLK_0;
		break;
	case SDLK_END:
	case SDLK_KP_PERIOD:
	case SDLK_PERIOD:
		key = SDLK_SPACE;
		break;
	case SDLK_INSERT:
	case SDLK_KP_DIVIDE:
		key = SDLK_SLASH;
		break;
	case SDLK_HOME:
	case SDLK_KP_MULTIPLY:
	case SDLK_COLON:
		key = SDLK_ASTERISK;
		break;
	case SDLK_PAGEUP:
	case SDLK_KP_MINUS:
		key = SDLK_MINUS;
		break;
	case SDLK_PAGEDOWN:
	case SDLK_KP_PLUS:
	case SDLK_SEMICOLON:
		key = SDLK_PLUS;
		break;
	case SDLK_KP1:
	case SDLK_KP2:
	case SDLK_KP3:
	case SDLK_KP4:
	case SDLK_KP6:
	case SDLK_KP7:
	case SDLK_KP8:
	case SDLK_KP9:
		key = (SDLKey)(key - SDLK_KP1 + SDLK_1);
		break;
	case SDLK_5:
	case SDLK_KP5:
		key = SDLK_2;
		break;
	default:
		break;
	}

	if (flagDown) {
		if ((key >= SDLK_a) && (key <= SDLK_z)) {
			char k = key - SDLK_a + 'a';
			gMainSpace->bearFgStar(k);
			return;
		}
		if (key == SDLK_SPACE) {
			gMainSpace->bearFgStar(' ');
			return;
		}
		if (key == SDLK_RETURN) {
			gMainSpace->bearFgStar('\n');
			return;
		}
		if (key == SDLK_SLASH) {
			gMainSpace->toggleFlagAutoBear();
			return;
		}
	}

	long ax = +-0;
	long ay = +-0;
	long az = +-0;
	bool flagSpeed = true;

	char k = 0;
	switch (key) {
	case SDLK_MINUS:
		az = +1;
		break;
	case SDLK_PLUS:
		az = -1;
		break;
	case SDLK_1:
	case SDLK_2:
	case SDLK_3:
	case SDLK_4:
	case SDLK_5:
	case SDLK_6:
	case SDLK_7:
	case SDLK_8:
	case SDLK_9:
		k = key - SDLK_1;
		ax = -(k % 3 - 1);
		ay = +(k / 3 - 1);
		break;
	case SDLK_0:
		ax = -1;
		ay = -1;
		az = -1;
		gMainSpace->decelerate(ax, ay, az);
		flagSpeed = false;
		break;
	default:
		flagSpeed = false;
		break;
	}

	if (flagSpeed) {
		if (gSetting->getFlag(OPTION_IDX_REVERSE_X))
			ax = -ax;
		if (gSetting->getFlag(OPTION_IDX_REVERSE_Y))
			ay = -ay;
		if (gSetting->getFlag(OPTION_IDX_REVERSE_Z))
			az = -az;

		bool flagMod = false;
		if (mod & (KMOD_CTRL | KMOD_SHIFT))
			flagMod = true;
		if (gSetting->getFlag(OPTION_IDX_REVERSE_SHIFT))
			flagMod = !flagMod;

		if (flagMod) {
			if (flagDown)
				gMainSpace->incSpeed(ax, ay, az);
			else
				gMainSpace->keepSpeed(ax, ay, az);
		} else {
			if (flagDown)
				gMainSpace->accelerate(ax, ay, az);
			else
				gMainSpace->decelerate(ax, ay, az);
		}
	}
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////

void eventMouseMotion(long x, long y)
{
	gMainControlPanel->eventMouseMotion(x, y);
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////

void eventMouseButton(bool flagDown, long btn)
{
	gMainControlPanel->eventMouseButton(flagDown, btn);
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////

void transWin2ObjPos(
	long wx, long wy, long ww, long wh,
	double *ox, double *oy, double *oz, double *ow, double *oh)
{
	*ox = -(double)(wx - (gBaseScreenWidth / 2)) / gReduceNum;
	*oy = -(double)(wy - (gBaseScreenHeight / 2)) / gReduceNum;
	*oz = 1.0;
	*ow = -(double)(ww) / gReduceNum;
	*oh = -(double)(wh) / gReduceNum;
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////

void transWin2ObjLineWidth(long wlw, double *olw)
{
	*olw = (double)wlw / (double)gBaseScreenWidth / gReduceNum;
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////

void projectWindowPosition(
	GLdouble x, GLdouble y, GLdouble z,
	long *wx, long *wy, long *wz)
{
	GLdouble modelview[16];
	::glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	GLdouble projection[16];
	::glGetDoublev(GL_PROJECTION_MATRIX, projection);
	GLint viewport[4];
	::glGetIntegerv(GL_VIEWPORT, viewport);

	GLdouble winX, winY, winZ;
	::gluProject(x, y, z, modelview, projection, viewport,
		&winX, &winY, &winZ);

	*wx = (long)winX;
	*wy = gScreenH - (long)winY;
	*wz = (long)winZ;
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////

void drawString(GLdouble x, GLdouble y, GLdouble z, const char str[])
{
	void *font = GLUT_BITMAP_HELVETICA_10;

	::glPushMatrix();
	::glPushAttrib(GL_CURRENT_BIT | GL_DEPTH_BUFFER_BIT);

	::glDisable(GL_TEXTURE_2D);
	::glDisable(GL_DEPTH_TEST);
	::glDepthMask(0);

	::glRasterPos3d(x, y, z);
	for (; *str; str++) {
		if (*str == '\n')
			break;
		if (*str == '\r')
			break;

		::glutBitmapCharacter(font, (int)*str);
	}

	::glEnable(GL_TEXTURE_2D);
	::glEnable(GL_DEPTH_TEST);

	::glPopAttrib();
	::glPopMatrix();
}

////////////////////////////////////////////////////////////////
// シグナルの処理ハンドラ
// int sig : シグナル
////////////////////////////////////////////////////////////////

static void signal_handler(int sig)
{
	exitGame(EXIT_FAILURE);
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////

long lToPow2(long n)
{
	long m = 1;
	for (m = 1; m < n; m <<= 1)
		;

	return m;
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////

double sgnDouble(double n)
{
	if (n > 0.0)
		return +1.0;
	else if (n < 0.0)
		return -1.0;
	else
		return +-0.0;
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////

double absDouble(double n)
{
	if (n < 0.0)
		return -n;
	else
		return n;
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////

void swapDouble(double *a, double *b)
{
	double tmp = *a;
	*a = *b;
	*b = tmp;
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////

void swapLong(long *a, long *b)
{
	long tmp = *a;
	*a = *b;
	*b = tmp;
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////

double randDouble()
{
	return (double)(rand()) / (double)RAND_MAX;
}

