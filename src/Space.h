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

#ifndef SPACE_H
#define SPACE_H

#include <string>

#include "GameMain.h"
#include "Option.h"
#include "Star.h"

////////////////////////////////////////////////////////////////

namespace StarSnowNotes {

////////////////////////////////////////////////////////////////
// 宇宙 - 星の管理
////////////////////////////////////////////////////////////////

class Space {
public:

private:
	// カメラの加速度
	static const double spaceAX = 0.0002;
	static const double spaceAY = 0.0002;
	static const double spaceAZ = 0.0002;

	// カメラの最高速度
	static const double spaceMaxVX = 0.02;
	static const double spaceMaxVY = 0.02;
	static const double spaceMaxVZ = 0.02;

	// 宇宙の最大半径
	static const double spaceMaxXR = 0.5;
	static const double spaceMaxYR = 0.5;
	static const double spaceMaxZR = 1.0;

	// 星のテクスチャの最大数
	static const long fgStarTextureMaxNum = 'z' - 'a' + 1;

	// 描画するか?
	bool flagDraw;
	// 停止中か?
	bool flagFreezed;
	// カメラを動かすか?または星を動かすか?
	bool flagMoveCamera;
	// 前景の星を自動的に生成させるか?
	bool flagAutoBear;

	// カメラの座標
	double x, y, z;
	// カメラの加速度
	double ax, ay, az;
	double ax2, ay2, az2;
	// カメラの移動速度
	double vx, vy, vz;
	// カメラのブレーキ
	bool bx, by, bz;

	// 背景のテクスチャ
	GLuint bgTexture;
	// 星のテクスチャ
	vector< vector<GLuint> > fgStarTexture;
	vector<GLuint> bgStarTexture;

	// 星
	list<Star> fgStarArray;
	list<Star> bgStarArray;

	// 前景の星を自動的に生成させるフレーム数
	long autoBearFrame;
	// 直前に生成した星の種類
	char prevStarType;

public:
	Space();
	~Space();
	void init(Option opt);
	void initTexture(Option opt);

	bool bearFgStar(char starType);
	bool bearBgStar();
	void autoBearFgStar();

	void incSpeed(long ax, long ay, long az);
	void keepSpeed(long ax, long ay, long az);
	void accelerate(long ax, long ay, long az);
	void decelerate(long ax, long ay, long az);

	void update();
	void draw();
	void reshape(long w, long h);

	bool setFlagFreezed(bool flag);
	bool setFlagDraw(bool flag);
	bool setFlagAutoBear(bool flag);
	bool toggleFlagAutoBear();

private:
	long randFgStar();

	double resetStarX(double r);
	double resetStarY(double r);
	double resetStarZ(double r);

	void killAllStar(list<Star> *listStar);

	void updateSpeed();
	void checkSpeedLimit();
	void updateMoving();
};

}

#endif // SPACE_H
