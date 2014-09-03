/***************************************************************
* Star Snow Notes
* Copyright (c) 2012 YOSHIMURA Tomohiko All rights reserved.
* 
* Created by bowkenken
*   URL: https://sourceforge.jp/projects/lnl/
* 
* License is GPL
* 
* �ܥץ����ϥե꡼�����եȥ������Ǥ���
* ���ʤ��ϡ� Free Software Foundation ����ɽ����
*  GNU ���̸�ͭ���ѵ����Ρ֥С�����󣲡�
* ���Ϥ���ʹߤγƥС��������椫�餤���줫�����򤷡�
* ���ΥС������������˽��ä��ܥץ�����
* �����ۤޤ����ѹ����뤳�Ȥ��Ǥ��ޤ���
* 
* �ܥץ�����ͭ�ѤȤϻפ��ޤ��������ۤˤ����äƤϡ�
* �Ծ����ڤ�������ŪŬ�����ˤĤ��Ƥΰ��ۤ��ݾڤ�ޤ��,
* �����ʤ��ݾڤ�Ԥʤ��ޤ���
* �ܺ٤ˤĤ��Ƥ� GNU ���̸�ͭ���ѵ�������ɤߤ���������
* 
* ���ʤ��ϡ��ܥץ����Ȱ��� GNU ���̸�ͭ���ѵ�����
* �μ̤��������äƤ���Ϥ��Ǥ��������Ǥʤ����ϡ�
*   Free Software Foundation, Inc.,
*   59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
* �ؼ���񤤤Ƥ���������
* 
* $Id:$
***************************************************************/

#include "GameMain.h"
#include "Button.h"

using namespace StarSnowNotes;

////////////////////////////////////////////////////////////////

Color Button::colorFrame[COLOR_IDX_MAX];
Color Button::colorRect[COLOR_IDX_MAX];

////////////////////////////////////////////////////////////////
// �ܥ���Υ��󥹥ȥ饯��
////////////////////////////////////////////////////////////////

Button::Button()
{
	flagDraw = false;
	flagPointed = false;
	flagPressed = false;

	x = 0;
	y = 0;
	width = defaultWidth;
	height = defaultHeight;

	key = SDLK_a;
	mod = (SDLMod)0;
}

////////////////////////////////////////////////////////////////
// �ܥ���Υǥ��ȥ饯��
////////////////////////////////////////////////////////////////

Button::~Button()
{
	flagDraw = false;
}

////////////////////////////////////////////////////////////////
// �ܥ���ν����
////////////////////////////////////////////////////////////////

void Button::init()
{
	flagDraw = false;
	flagPointed = false;
	flagPressed = false;

	strCaption = "";

	reshape(0, 0, defaultWidth, defaultHeight);
}

////////////////////////////////////////////////////////////////
// �ܥ��������
////////////////////////////////////////////////////////////////

void Button::draw()
{
	if (!flagDraw)
		return;

	if (flagPressed)
		drawPressed();
	else if (flagPointed)
		drawPointed();
	else
		drawNotPointed();
}

////////////////////////////////////////////////////////////////
// �ܥ��������(������Ƥ���)
////////////////////////////////////////////////////////////////

void Button::drawPressed()
{
	if (!flagDraw)
		return;

	drawBox(COLOR_IDX_PRESSED);
	drawName(COLOR_IDX_PRESSED);
}

////////////////////////////////////////////////////////////////
// �ܥ��������(�ݥ���Ȥ���Ƥ���)
////////////////////////////////////////////////////////////////

void Button::drawPointed()
{
	if (!flagDraw)
		return;

	drawBox(COLOR_IDX_POINTED);
	drawName(COLOR_IDX_POINTED);
}

////////////////////////////////////////////////////////////////
// �ܥ��������(�ݥ���Ȥ���Ƥ��ʤ�)
////////////////////////////////////////////////////////////////

void Button::drawNotPointed()
{
	if (!flagDraw)
		return;

	drawBox(COLOR_IDX_NOT_POINTED);
	drawName(COLOR_IDX_NOT_POINTED);
}

////////////////////////////////////////////////////////////////
// Box������
////////////////////////////////////////////////////////////////

void Button::drawBox(ColorIdx col)
{
	long lineWidth = 1;
	double r, g, b, a;
	double z;

	double ox, oy, oz, ow, oh;
	::transWin2ObjPos(x, y, width, height, &ox, &oy, &oz, &ow, &oh);

	double objLineWidth;
	::transWin2ObjLineWidth(lineWidth, &objLineWidth);

	// Begin

	::glPushMatrix();

	::glDisable(GL_TEXTURE_2D);
	::glDisable(GL_DEPTH_TEST);
	::glDepthMask(0);
	::glEnable(GL_BLEND);
	::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// ������

	z = oz + 0.001;

	::glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	r = (double)(colorRect[col].r) / 255.0;
	g = (double)(colorRect[col].g) / 255.0;
	b = (double)(colorRect[col].b) / 255.0;
	a = (double)(colorRect[col].a) / 255.0;
	::glColor4d(r, g, b, a);

	::glBegin(GL_QUADS);
	::glVertex3d(ox + ow, oy + oh, z);
	::glVertex3d(ox,      oy + oh, z);
	::glVertex3d(ox,      oy,      z);
	::glVertex3d(ox + ow, oy,      z);
	::glEnd();

	// ����

	z = oz;

	::glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	::glLineWidth(objLineWidth);

	r = (double)(colorFrame[col].r) / 255.0;
	g = (double)(colorFrame[col].g) / 255.0;
	b = (double)(colorFrame[col].b) / 255.0;
	a = (double)(colorFrame[col].a) / 255.0;
	::glColor4d(r, g, b, a);

	::glBegin(GL_QUADS);
	::glVertex3d(ox + ow, oy + oh, z);
	::glVertex3d(ox,      oy + oh, z);
	::glVertex3d(ox,      oy,      z);
	::glVertex3d(ox + ow, oy,      z);
	::glEnd();

	// End

	::glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	::glEnable(GL_TEXTURE_2D);

	::glPopMatrix();
}

////////////////////////////////////////////////////////////////
// ����̾��������
////////////////////////////////////////////////////////////////

void Button::drawName(ColorIdx col)
{
	double r, g, b, a;
	r = (double)(colorFrame[col].r) / 255.0;
	g = (double)(colorFrame[col].g) / 255.0;
	b = (double)(colorFrame[col].b) / 255.0;
	a = (double)(colorFrame[col].a) / 255.0;
	::glColor4d(r, g, b, a);

	long x = this->x + 5;
	long y = this->y + 15;
	const long fontHeight = 15;

	const char *str = strCaption.c_str();
	while (1) {
		if (str == NULL)
			break;
		if (*str == '\0')
			break;

		double ox, oy, oz, ow, oh;
		::transWin2ObjPos(
			x, y, width, height,
			&ox, &oy, &oz, &ow, &oh);
		::drawString(ox, oy, oz, str);

		y += fontHeight;

		str = strchr(str, '\n');
		if (str == NULL)
			break;
		if (*str == '\0')
			break;
		str++;
	}
}

////////////////////////////////////////////////////////////////
// ���������ѹ�
////////////////////////////////////////////////////////////////

void Button::reshape(long x, long y, long w, long h)
{
	this->x = x;
	this->y = y;
	this->width = w;
	this->height = h;

	// ɽ����Υ�����ɥ���ɸ���Ѵ�

	double ox, oy, oz, ow, oh;
	::transWin2ObjPos(x, y, width, height, &ox, &oy, &oz, &ow, &oh);

	::projectWindowPosition(
		ox, oy, oz,
		&winMinX, &winMinY, &winMinZ);
	::projectWindowPosition(
		ox + ow, oy + oh, oz,
		&winMaxX, &winMaxY, &winMaxZ);
}

////////////////////////////////////////////////////////////////
// �����ON/OFF������
////////////////////////////////////////////////////////////////

bool Button::setFlagDraw(bool flag)
{
	bool prevFlag = flagDraw;
	flagDraw = flag;
	return prevFlag;
}

////////////////////////////////////////////////////////////////
// ��������ǥݥ���Ȥ���Ƥ��뤫�Υե饰������
////////////////////////////////////////////////////////////////

bool Button::setFlagPointed(bool flag)
{
	bool prevFlag = flagPointed;
	flagPointed = flag;
	return prevFlag;
}

////////////////////////////////////////////////////////////////
// ������Ƥ��뤫�Υե饰������
////////////////////////////////////////////////////////////////

bool Button::setFlagPressed(bool flag)
{
	bool prevFlag = flagPressed;
	flagPressed = flag;
	return prevFlag;
}

////////////////////////////////////////////////////////////////
// ��ɸ���ܥ������¦��ؤ��Ƥ��뤫��Ĵ�٤�
////////////////////////////////////////////////////////////////

bool Button::checkPointRect(long x, long y)
{
	if (x < winMinX)
		return false;
	if (y < winMinY)
		return false;
	if (x >= winMaxX)
		return false;
	if (y >= winMaxY)
		return false;

	return true;
}

////////////////////////////////////////////////////////////////
// �ܥ���ο�������(����)
////////////////////////////////////////////////////////////////

void Button::setColorFrame(ColorIdx idx, long r, long g, long b, long a)
{
	colorFrame[idx].r = (unsigned char)r;
	colorFrame[idx].g = (unsigned char)g;
	colorFrame[idx].b = (unsigned char)b;
	colorFrame[idx].a = (unsigned char)a;
}

////////////////////////////////////////////////////////////////
// �ܥ���ο�������(��¦)
////////////////////////////////////////////////////////////////

void Button::setColorRect(ColorIdx idx, long r, long g, long b, long a)
{
	colorRect[idx].r = (unsigned char)r;
	colorRect[idx].g = (unsigned char)g;
	colorRect[idx].b = (unsigned char)b;
	colorRect[idx].a = (unsigned char)a;
}

////////////////////////////////////////////////////////////////
// �ܥ���Υ���ץ���������
////////////////////////////////////////////////////////////////

void Button::setCaption(std::string caption)
{
	this->strCaption = caption;
}

////////////////////////////////////////////////////////////////
// ����å����줿����ȯ�����륭��������
////////////////////////////////////////////////////////////////

void Button::setKey(SDLKey key)
{
	this->key = key;
}

////////////////////////////////////////////////////////////////
// ����å����줿����ȯ�����뽤��������ON/OFF������
////////////////////////////////////////////////////////////////

void Button::setMod(SDLMod mod, bool flagOn)
{
	if (flagOn)
		this->mod = (SDLMod)(this->mod | mod);
	else
		this->mod = (SDLMod)(this->mod & ~mod);
}

////////////////////////////////////////////////////////////////
// �ܥ��󤬥ݥ���Ȥ��줿�������å�
////////////////////////////////////////////////////////////////

void Button::eventMouseMotion(
	bool flagDown, long x, long y, bool flagDrawAll)
{
	if (checkPointRect(x, y)) {
		setFlagDraw(true);
		setFlagPointed(true);
		setFlagPressed(flagDown);
	} else {
		setFlagDraw(flagDrawAll);
		setFlagPointed(false);
		setFlagPressed(false);
	}
}

////////////////////////////////////////////////////////////////
// �ܥ��󤬥���å����줿�������å�
////////////////////////////////////////////////////////////////

void Button::eventMouseButton(
	bool flagDown, long btn, long x, long y, bool flagDrawAll)
{
	if (btn < 0)
		return;
	if (btn >= MouseButtonMax)
		return;

	eventMouseMotion(flagDown, x, y, flagDrawAll);

	if (checkPointRect(x, y)) {
		if (!flagDown) {
			::eventKey(true, key, mod);
			::eventKey(false, key, mod);
		}
	}
}

