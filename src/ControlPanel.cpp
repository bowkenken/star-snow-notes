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

#include "GameMain.h"
#include "ControlPanel.h"

using namespace StarSnowNotes;

////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
// 制御板のコンストラクタ
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
// 制御板のデストラクタ
////////////////////////////////////////////////////////////////

ControlPanel::~ControlPanel()
{
	flagDraw = false;
	flagFreezed = true;

	starButtonArray.clear();
}

////////////////////////////////////////////////////////////////
// 制御板の初期化
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
// 制御板を更新
////////////////////////////////////////////////////////////////

void ControlPanel::update()
{
}

////////////////////////////////////////////////////////////////
// 制御板を描画
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
// 画面サイズの変更
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

	// 表示上のウィンドウ座標に変換

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
// 硬直のON/OFFを設定
////////////////////////////////////////////////////////////////

bool ControlPanel::setFlagFreezed(bool flag)
{
	bool prevFlag = flagFreezed;
	flagFreezed = flag;
	return prevFlag;
}

////////////////////////////////////////////////////////////////
// 描画のON/OFFを設定
////////////////////////////////////////////////////////////////

bool ControlPanel::setFlagDraw(bool flag)
{
	bool prevFlag = flagDraw;
	flagDraw = flag;
	return prevFlag;
}

////////////////////////////////////////////////////////////////
// 全ての星のボタンを表示するかを設定
////////////////////////////////////////////////////////////////

bool ControlPanel::setFlagDrawAllStarButton(bool flag)
{
	bool prevFlag = flagDrawAllStarButton;
	flagDrawAllStarButton = flag;
	return prevFlag;
}

////////////////////////////////////////////////////////////////
// マウスが移動した時の処理
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
// マウス・ボタンが押され／離された時の処理
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
// 全ての星のボタンを表示するかを調べる
////////////////////////////////////////////////////////////////

bool ControlPanel::checkDrawAllStarButton()
{
	bool flagDrawAll = flagDrawAllStarButton;

	if (!checkMouseInAnyStarButton())
		flagDrawAll = false;

	return flagDrawAll;
}

////////////////////////////////////////////////////////////////
// マウス・カーソルが星ボタンの領域内か調べる
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

