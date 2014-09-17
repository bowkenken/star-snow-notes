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

#ifndef STAR_H
#define STAR_H

#include "GameMain.h"

////////////////////////////////////////////////////////////////
// 星や星雲の座標や画像
////////////////////////////////////////////////////////////////

class Star {
public:

private:
	// 生きているか?
	bool flagLiving;

	// 座標
	double x, y, z;
	GLuint texture;

public:
	Star();
	~Star();

	bool born(double x, double y, double z, GLuint tx);
	bool die();

	bool draw(double r);

	bool getFlagLiving();
	void getPos(double *sx, double *sy, double *sz);
	void setPos(double sx, double sy, double sz);

private:
};

#endif // STAR_H
