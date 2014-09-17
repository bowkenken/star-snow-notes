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

#ifndef CONTROL_PANEL_H
#define CONTROL_PANEL_H

#include "GameMain.h"
#include "Option.h"
#include "Button.h"

////////////////////////////////////////////////////////////////
// 制御板
////////////////////////////////////////////////////////////////

namespace StarSnowNotes {

class ControlPanel {
public:

private:
	// デフォルトの表示領域のサイズ
	static const long defaultScreenWidth = 640;
	static const long defaultScreenHeight = 480;

	// ボタンの格子の数
	static const long gridHorizontalNum = 6;
	static const long gridVerticalNum = 6;

	// 星のボタンの最大数
	static const long starButtonMaxNum = 'z' - 'a' + 1;

	Option optionSetting;

	// 停止中か?
	bool flagFreezed;
	// 描画するか?
	bool flagDraw;
	// 全てのボタンを表示するか?
	bool flagDrawAllStarButton;

	// 表示領域の幅
	long screenWidth;
	// 表示領域の高さ
	long screenHeight;

	// 星ボタンの対角のウィンドウ座標
	long starWinMinX, starWinMinY, starWinMinZ;
	long starWinMaxX, starWinMaxY, starWinMaxZ;

	// ボタン
	std::vector<StarSnowNotes::Button *> starButtonArray;
	StarSnowNotes::Button *starButtonSpace;
	StarSnowNotes::Button *starButtonEnter;

	// マウスのX座標
	long mouseX;
	// マウスのY座標
	long mouseY;
	// マウスのボタンの押下状態
	bool flagMouseButton[MouseButtonMax];

public:
	ControlPanel();
	~ControlPanel();
	void init(Option opt);

	void update();
	void draw();
	void reshape(long w, long h);

	bool setFlagFreezed(bool flag);
	bool setFlagDraw(bool flag);
	bool setFlagDrawAllStarButton(bool flag);

	void eventMouseMotion(long x, long y);
	void eventMouseButton(bool flagDown, long btn);

	bool checkDrawAllStarButton();
	bool checkMouseInAnyStarButton();

private:
};

}

#endif // CONTROL_PANEL_H
