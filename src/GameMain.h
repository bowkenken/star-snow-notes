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

#ifndef GAME_MAIN_H
#define GAME_MAIN_H	1

#include "config.h"

#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif // HAVE_STDLIB_H

#ifdef HAVE_STDIO_H
# include <stdio.h>
#endif // HAVE_STDIO_H

#include <string>
#include <iterator>
#include <list>
#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

////////////////////////////////////////////////////////////////
// バージョン
////////////////////////////////////////////////////////////////

#define	STRING_FILE_NAME_GAME	PACKAGE_NAME

#define	STRING_GAME_TITLE	"Star Snow Notes"
#define	STRING_VERSION_GAME	VERSION

#define	STRING_SINCE	"2012"
#define	STRING_LATEST	"2013"
#define	STRING_AUTHOR	"YOSHIMURA, Tomohiko"

#define	STR_TITLE_1	"Created by"
#define	STR_HANDLE_1	"bowkenken"

#define	STRING_WEB	"http://ssn.sourceforge.jp/"
#define	STRING_E_MAIL	"bowkenken@users.sourceforge.jp"

#define	STRING_FORMAT_COPYRIGHT	\
		"%s : version %s\n" \
		"Copyright (c) %s-%s %s All rights reserved.\n" \
		"%s %s\n" \
		"URL   : %s\n" \
		"E-mail: %s\n" \

#define	LS_STRING_COPYRIGHT	\
		STRING_GAME_TITLE, STRING_VERSION_GAME, \
		STRING_SINCE, STRING_LATEST, STRING_AUTHOR, \
		STR_TITLE_1, STR_HANDLE_1, \
		STRING_WEB, \
		STRING_E_MAIL

////////////////////////////////////////////////////////////////

#define LOOP_MAX_100	128

enum MouseButtonNum {
	MouseButtonLeft,
	MouseButtonMiddle,
	MouseButtonRight,
	MouseButtonWheelUp,
	MouseButtonWheelDown,
	MouseButtonMax,
};

////////////////////////////////////////////////////////////////

GLuint loadTexture(const char *fileName, double *w, double *h);
void exitGame(int exitCode);
void closeGame(int exitCode);
void eventKey(bool flagDown, SDLKey key, SDLMod mod);
void transWin2ObjPos(
	long wx, long wy, long ww, long wh,
	double *ox, double *oy, double *oz, double *ow, double *oh);
void transWin2ObjLineWidth(long wlw, double *olw);
void projectWindowPosition(
	double x, double y, double z,
	long *wx, long *wy, long *wz);
void drawString(GLdouble x, GLdouble y, GLdouble z, const char str[]);
long lToPow2(long n);
double sgnDouble(double n);
double absDouble(double n);
void swapDouble(double *a, double *b);
void swapLong(long *a, long *b);
double randDouble();

#endif // GAME_MAIN_H
