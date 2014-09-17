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

#include "Star.h"

////////////////////////////////////////////////////////////////
// 星のコンストラクタ
////////////////////////////////////////////////////////////////

Star::Star()
{
	flagLiving = false;
	x = 0.0;
	y = 0.0;
	z = 0.0;
}

////////////////////////////////////////////////////////////////
// 星のデストラクタ
////////////////////////////////////////////////////////////////

Star::~Star()
{
	flagLiving = false;
}

////////////////////////////////////////////////////////////////
// 星を生成
////////////////////////////////////////////////////////////////

bool Star::born(double x, double y, double z, GLuint tx)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->texture = tx;

	this->flagLiving = true;

	return true;
}

////////////////////////////////////////////////////////////////
// 星を消滅
////////////////////////////////////////////////////////////////

bool Star::die()
{
	flagLiving = false;

	return true;
}

////////////////////////////////////////////////////////////////
// 星を描画
////////////////////////////////////////////////////////////////

bool Star::draw(double r)
{
	if (!flagLiving)
		return false;

	::glPushMatrix();

	::glColor4d(1.0, 1.0, 1.0, 1.0);
	::glBindTexture(GL_TEXTURE_2D, texture);

	::glDisable(GL_DEPTH_TEST);
	::glDepthMask(0);
	::glEnable(GL_BLEND);
	::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	double x = this->x * 640.0 * 4;
	double y = this->y * 640.0 * 4;
	double z = this->z * 640.0;
	::glBegin(GL_QUADS);
	::glTexCoord2d(0.0, 0.0);
	::glVertex3d(x + r, y + r, z);
	::glTexCoord2d(1.0, 0.0);
	::glVertex3d(x - r, y + r, z);
	::glTexCoord2d(1.0, 1.0);
	::glVertex3d(x - r, y - r, z);
	::glTexCoord2d(0.0, 1.0);
	::glVertex3d(x + r, y - r, z);
	::glEnd();

	::glPopMatrix();

	return true;
}

////////////////////////////////////////////////////////////////
// 星が生きているか?
////////////////////////////////////////////////////////////////

bool Star::getFlagLiving()
{
	return flagLiving;
}

////////////////////////////////////////////////////////////////
// 星を座標を返す
////////////////////////////////////////////////////////////////

void Star::getPos(double *sx, double *sy, double *sz)
{
	if (sx != NULL)
		*sx = x;
	if (sy != NULL)
		*sy = y;
	if (sz != NULL)
		*sz = z;
}

////////////////////////////////////////////////////////////////
// 星を座標を設定
////////////////////////////////////////////////////////////////

void Star::setPos(double sx, double sy, double sz)
{
	x = sx;
	y = sy;
	z = sz;
}

