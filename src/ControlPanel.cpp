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
#include "ControlPanel.h"

using namespace StarSnowNotes;

////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
// �����ĤΥ��󥹥ȥ饯��
////////////////////////////////////////////////////////////////

ControlPanel::ControlPanel()
{
	flagDraw = false;
	flagFreezed = true;
	flagDrawAllStarButton = false;

	screenWidth = defaultScreenWidth;
	screenHeight = defaultScreenHeight;

	starWinMinX = 0;
	starWinMinY = 0;
	starWinMinZ = 0;
	starWinMaxX = 0;
	starWinMaxY = 0;
	starWinMaxZ = 0;

	mouseX = 0;
	mouseY = 0;
	for (long i = MouseButtonLeft; i < MouseButtonMax; i++)
		flagMouseButton[i] = false;
}

////////////////////////////////////////////////////////////////
// �����ĤΥǥ��ȥ饯��
////////////////////////////////////////////////////////////////

ControlPanel::~ControlPanel()
{
	flagDraw = false;
	flagFreezed = true;

	starButtonArray.clear();
}

////////////////////////////////////////////////////////////////
// �����Ĥν����
////////////////////////////////////////////////////////////////

void ControlPanel::init(Option opt)
{
	optionSetting = opt;

	flagDraw = false;
	flagFreezed = true;
	flagDrawAllStarButton = true;

	screenWidth = defaultScreenWidth;
	screenHeight = defaultScreenHeight;

	for (long i = MouseButtonLeft; i < MouseButtonMax; i++)
		flagMouseButton[i] = false;

	starButtonArray.clear();
	for (long i = 0; i < starButtonMaxNum; i++) {
		starButtonArray.push_back(new Button());
		if (starButtonArray[i] == NULL)
			break;

		starButtonArray[i]->init();
		starButtonArray[i]->setCaption(
			optionSetting.getCaption('a' + i));
		starButtonArray[i]->setKey((SDLKey)(SDLK_a + i));
	}

	do {
		starButtonSpace = new Button();
		if (starButtonSpace == NULL)
			break;

		starButtonSpace->init();
		starButtonSpace->setCaption(
			optionSetting.getCaption(' '));
		starButtonSpace->setKey((SDLKey)SDLK_SPACE);
	} while (false);
	do {
		starButtonEnter = new Button();
		if (starButtonEnter == NULL)
			break;

		starButtonEnter->init();
		starButtonEnter->setCaption(
			optionSetting.getCaption('\n'));
		starButtonEnter->setKey((SDLKey)SDLK_RETURN);
	} while (false);

	reshape(screenWidth, screenHeight);
}

////////////////////////////////////////////////////////////////
// �����Ĥ򹹿�
////////////////////////////////////////////////////////////////

void ControlPanel::update()
{
}

////////////////////////////////////////////////////////////////
// �����Ĥ�����
////////////////////////////////////////////////////////////////

void ControlPanel::draw()
{
	if (!flagDraw)
		return;

	for (long i = 0; i < starButtonMaxNum; i++) {
		starButtonArray[i]->draw();
	}
	starButtonSpace->draw();
	starButtonEnter->draw();
}

////////////////////////////////////////////////////////////////
// ���̥��������ѹ�
////////////////////////////////////////////////////////////////

void ControlPanel::reshape(long w, long h)
{
	screenWidth = w;
	screenHeight = h;

	long sw = screenWidth / gridHorizontalNum;
	long sh = screenHeight / gridVerticalNum;
	long sx = 0;
	long sy = 0;
	long i;
	for (i = 0; i < starButtonMaxNum; i++) {
		sx = sw * (i % gridHorizontalNum);
		sy = sh * (i / gridHorizontalNum);

		starButtonArray[i]->reshape(sx, sy, sw - 2, sh - 2);
	}

	sx = sw * (i % gridHorizontalNum);
	sy = sh * (i / gridHorizontalNum);
	starButtonSpace->reshape(sx, sy, sw - 2, sh - 2);
	i++;

	sx = sw * (i % gridHorizontalNum);
	sy = sh * (i / gridHorizontalNum);
	starButtonEnter->reshape(sx, sy, sw - 2, sh - 2);
	i++;

	// ɽ����Υ�����ɥ���ɸ���Ѵ�

	double ox, oy, oz, ow, oh;
	transWin2ObjPos(0, 0, w, sy + sh, &ox, &oy, &oz, &ow, &oh);

	double minX = ox;
	double minY = oy;
	double minZ = oz;
	double maxX = ox + ow;
	double maxY = oy + oh;
	double maxZ = oz;

	::projectWindowPosition(
		minX, minY, minZ,
		&starWinMinX, &starWinMinY, &starWinMinZ);
	::projectWindowPosition(
		maxX, maxY, maxZ,
		&starWinMaxX, &starWinMaxY, &starWinMaxZ);
}

////////////////////////////////////////////////////////////////
// ��ľ��ON/OFF������
////////////////////////////////////////////////////////////////

bool ControlPanel::setFlagFreezed(bool flag)
{
	bool prevFlag = flagFreezed;
	flagFreezed = flag;
	return prevFlag;
}

////////////////////////////////////////////////////////////////
// �����ON/OFF������
////////////////////////////////////////////////////////////////

bool ControlPanel::setFlagDraw(bool flag)
{
	bool prevFlag = flagDraw;
	flagDraw = flag;
	return prevFlag;
}

////////////////////////////////////////////////////////////////
// ���Ƥ����Υܥ����ɽ�����뤫������
////////////////////////////////////////////////////////////////

bool ControlPanel::setFlagDrawAllStarButton(bool flag)
{
	bool prevFlag = flagDrawAllStarButton;
	flagDrawAllStarButton = flag;
	return prevFlag;
}

////////////////////////////////////////////////////////////////
// �ޥ�������ư�������ν���
////////////////////////////////////////////////////////////////

void ControlPanel::eventMouseMotion(long x, long y)
{
	mouseX = x;
	mouseY = y;

	bool flagDown = false;
	for (long i = MouseButtonLeft; i < MouseButtonMax; i++) {
		if (flagMouseButton[i]) {
			flagDown = true;
			break;
		}
	}

	bool flagDrawAll = checkDrawAllStarButton();
	for (long i = 0; i < starButtonMaxNum; i++) {
		starButtonArray[i]->eventMouseMotion(
			flagDown, mouseX, mouseY, flagDrawAll);
	}
	starButtonSpace->eventMouseMotion(
		flagDown, mouseX, mouseY, flagDrawAll);
	starButtonEnter->eventMouseMotion(
		flagDown, mouseX, mouseY, flagDrawAll);
}

////////////////////////////////////////////////////////////////
// �ޥ������ܥ��󤬲����졿Υ���줿���ν���
////////////////////////////////////////////////////////////////

void ControlPanel::eventMouseButton(bool flagDown, long btn)
{
	if (btn < 0)
		return;
	if (btn >= MouseButtonMax)
		return;

	flagMouseButton[btn] = flagDown;

	bool flagDrawAll = checkDrawAllStarButton();
	for (long i = 0; i < starButtonMaxNum; i++) {
		starButtonArray[i]->eventMouseButton(
			flagDown, btn, mouseX, mouseY, flagDrawAll);
	}
	starButtonSpace->eventMouseButton(
		flagDown, btn, mouseX, mouseY, flagDrawAll);
	starButtonEnter->eventMouseButton(
		flagDown, btn, mouseX, mouseY, flagDrawAll);
}

////////////////////////////////////////////////////////////////
// ���Ƥ����Υܥ����ɽ�����뤫��Ĵ�٤�
////////////////////////////////////////////////////////////////

bool ControlPanel::checkDrawAllStarButton()
{
	bool flagDrawAll = flagDrawAllStarButton;

	if (!checkMouseInAnyStarButton())
		flagDrawAll = false;

	return flagDrawAll;
}

////////////////////////////////////////////////////////////////
// �ޥ������������뤬���ܥ�����ΰ��⤫Ĵ�٤�
////////////////////////////////////////////////////////////////

bool ControlPanel::checkMouseInAnyStarButton()
{
	if (mouseX < 0)
		return false;
	if (mouseY < 0)
		return false;
	if (mouseX >= screenWidth)
		return false;
	if (mouseY >= screenHeight)
		return false;

	if (mouseX < starWinMinX)
		return false;
	if (mouseY < starWinMinY)
		return false;
	if (mouseX >= starWinMaxX)
		return false;
	if (mouseY >= starWinMaxY)
		return false;

	return true;
}

