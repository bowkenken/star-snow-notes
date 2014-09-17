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

#ifndef BUTTON_H
#define BUTTON_H

#include "GameMain.h"

namespace StarSnowNotes {

////////////////////////////////////////////////////////////////
// ボタンの色
////////////////////////////////////////////////////////////////

struct Color {
	unsigned char r, g, b, a;
};

////////////////////////////////////////////////////////////////
// ボタン
////////////////////////////////////////////////////////////////

class Button {
public:
	enum ColorIdx {
		COLOR_IDX_NOT_POINTED,
		COLOR_IDX_POINTED,
		COLOR_IDX_PRESSED,
		COLOR_IDX_MAX,
	};

private:
	// デフォルトのサイズ
	static const long defaultWidth = 640 / 6;
	static const long defaultHeight = 480 / 6;

	// 描画するか?
	bool flagDraw;
	// カーソルでポイントされているか?
	bool flagPointed;
	// 押されているか?
	bool flagPressed;

	// X座標
	long x;
	// Y座標
	long y;
	// 幅
	long width;
	// 高さ
	long height;
	// 対角のウィンドウ座標
	long winMinX, winMinY, winMinZ;
	long winMaxX, winMaxY, winMaxZ;

	// 外枠の色
	static Color colorFrame[COLOR_IDX_MAX];
	// タイルの色
	static Color colorRect[COLOR_IDX_MAX];

	// キャプション
	std::string strCaption;

	// クリックされた時に発生するキー
	SDLKey key;
	// クリックされた時に発生する修飾キー
	SDLMod mod;

public:
	Button();
	~Button();
	void init();

	void draw();
	// void drawPressed();
	// void drawPointed();
	// void drawNotPointed();
	// void drawBox(ColorIdx col);
	// void drawName(ColorIdx col);
	void reshape(long x, long y, long w, long h);

	bool setFlagDraw(bool flag);
	bool setFlagPointed(bool flag);
	bool setFlagPressed(bool flag);

	// bool checkPointRect(long x, long y);

	static void setColorFrame(
		ColorIdx idx, long r, long g, long b, long a);
	static void setColorRect(
		ColorIdx idx, long r, long g, long b, long a);

	void eventMouseMotion(
		bool flagDown, long x, long y,
		bool flagDrawAll);
	void eventMouseButton(
		bool flagDown, long btn, long x, long y,
		bool flagDrawAll);

	void setCaption(std::string caption);

	void setKey(SDLKey key);
	void setMod(SDLMod mod, bool flagOn);

private:
	void drawPressed();
	void drawPointed();
	void drawNotPointed();
	void drawBox(ColorIdx col);
	void drawName(ColorIdx col);

	bool checkPointRect(long x, long y);
};

}

#endif // BUTTON_H
