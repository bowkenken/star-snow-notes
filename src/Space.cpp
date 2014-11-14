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
#include "Option.h"
#include "FileList.h"
#include "Space.h"

using namespace StarSnowNotes;

////////////////////////////////////////////////////////////////

#define	STR_GRAPH_FILE_EXT	\
	"png xpm jpg jpeg tif pnm bmp gif " \
	"PNG XPM JPG JPEG TIF PNM BMP GIF " \
	"Bmp" \
	""

#define	STR_DIR_SPACE_BG	"back-ground"
#define	STR_DIR_STAR_FG	"star"
#define	STR_DIR_STAR_BG	"star/_base"

////////////////////////////////////////////////////////////////
// 宇宙のコンストラクタ
////////////////////////////////////////////////////////////////

Space::Space()
{
	flagDraw = false;
	flagFreezed = true;
	vx = +0.0;
	vy = +0.0;
	vz = +0.1;

	bgTexture = 0;
}

////////////////////////////////////////////////////////////////
// 宇宙のデストラクタ
////////////////////////////////////////////////////////////////

Space::~Space()
{
	flagDraw = false;
	flagFreezed = true;

	killAllStar(&fgStarArray);
	killAllStar(&bgStarArray);

	::glDeleteTextures(1, &bgTexture);
	bgTexture = 0;

	for (unsigned int i = 0; i < fgStarTexture.size(); ++i) {
		for (unsigned int j = 0; j < fgStarTexture[i].size(); ++j) {
			::glDeleteTextures(1, &(fgStarTexture[i][j]));
		}
	}
	for (unsigned int j = 0; j < bgStarTexture.size(); ++j) {
		::glDeleteTextures(1, &(bgStarTexture[j]));
	}
}

////////////////////////////////////////////////////////////////
// 宇宙の初期化
////////////////////////////////////////////////////////////////

void Space::init(Option *opt)
{
	if (opt == NULL) {
		opt = new Option;
		opt->init();
	}

	setting = opt;

	srand(time(NULL));

	flagDraw = false;
	flagFreezed = true;

	flagMoveCamera = false;

	autoBearFrame = setting->getNum(
		StarSnowNotes::OPTION_IDX_INTERVAL_AUTO);
	if (autoBearFrame < 1)
		autoBearFrame = 1;

	flagAutoBear = false;
	prevStarType = setting->getKey(OPTION_IDX_AUTO_KEY);
	if (prevStarType == '\0')
		prevStarType = ' ';
	else
		flagAutoBear = true;

	x = 0.0;
	y = 0.0;
	z = 0.0;
	ax = +-0.0;
	ay = +-0.0;
	az = +-0.0;
	ax2 = +-0.0;
	ay2 = +-0.0;
	az2 = +-0.0;
	bx = false;
	by = false;
	bz = false;
	vx = setting->getNum(OPTION_IDX_X_SPEED);
	vy = setting->getNum(OPTION_IDX_Y_SPEED);
	vz = setting->getNum(OPTION_IDX_Z_SPEED);

	killAllStar(&fgStarArray);
	killAllStar(&bgStarArray);

	initTexture();

	long bgStarMaxNum = setting->getNum(OPTION_IDX_STAR_NUMBER);
	for (long i = 0; i < bgStarMaxNum; ++i)
		bearBgStar();

	reshape(640, 480);

	::glMatrixMode(GL_MODELVIEW);
	::glLoadIdentity();
	::gluLookAt(
		x, y, z,
		x, y, (z + 1.0),
		0.0, 1.0, 0.0);
}

////////////////////////////////////////////////////////////////
// テクスチャの初期化
////////////////////////////////////////////////////////////////

void Space::initTexture()
{
	if (setting == NULL)
		return;

	std::string dir = setting->getGraphDir();
	if (dir == "")
		dir = "default/";
	// fprintf(stderr, "[texture=%s]\n", dir.c_str());
	FileList::setStrDirSelGraph(dir);
	FileList fls;
	std::string path = ".";

	// 宇宙の背景

	path = "";
	std::string file = setting->getFile(OPTION_IDX_BG_FILE);
	if (file != "") {
		path = FileList::jointDir(fls.getBaseDir(), STR_DIR_SPACE_BG);
		path = FileList::jointDir(path, file);
	} else {
		path = "";
		fls.reset(STR_DIR_SPACE_BG, STR_GRAPH_FILE_EXT);
		for (long i = 0; i < LOOP_MAX_100; ++i) {
			std::string tmpPath = fls.next();
			if (tmpPath == "")
				break;
			if ((rand() % (i + 1)) == 0)
				path = tmpPath;
		}
		if (path == "") {
			fprintf(stderr,
				"ERROR: No files are in a directory: %s",
				STR_DIR_SPACE_BG);
			::exitGame(EXIT_FAILURE);
		}
	}
	bgTexture = ::loadTexture(path.c_str(), NULL, NULL);
	if (bgTexture == 0)
		::exitGame(EXIT_FAILURE);

	// 前景の星

	char starCStr[1 + 1] = "a";
	std::string starStr = "a";

	fgStarTexture.resize(fgStarTextureMaxNum);
	for (long i = 0; i < fgStarTextureMaxNum; ++i) {
		starCStr[0] = 'a' + i;
		starStr = fls.jointDir(STR_DIR_STAR_FG, starCStr);

		fls.reset(starStr, STR_GRAPH_FILE_EXT);
		for (long j = 0; ; ++j) {
			path = fls.next();
			if (path == "")
				break;

			fgStarTexture[i].push_back(::loadTexture(
				path.c_str(), NULL, NULL));
			if (fgStarTexture[i][j] == 0)
				break;
		}
	}

	// 背景の星

	fls.reset(STR_DIR_STAR_BG, STR_GRAPH_FILE_EXT);
	for (long j = 0; ; ++j) {
		path = fls.next();
		if (path == "")
			break;

		bgStarTexture.push_back(::loadTexture(path.c_str(),
			NULL, NULL));
		if (bgStarTexture[j] == 0)
			break;
	}
	if (bgStarTexture.empty()) {
		fprintf(stderr,
			"ERROR: No files are in a directory: %s",
			STR_DIR_STAR_BG);
		::exitGame(EXIT_FAILURE);
	}
}

////////////////////////////////////////////////////////////////
// 前景の星を生成させる
////////////////////////////////////////////////////////////////

bool Space::bearFgStar(char starType)
{
	if (starType == '/')
		starType = prevStarType;
	else
		prevStarType = starType;

	long n = 0;
	static long prevN = 0;
	do {
		if (('a' <= starType) && (starType <= 'z')) {
			n = starType - 'a';
			break;
		}
		if (starType == ' ') {
			n = randFgStar();
			break;
		}
		if (starType == '\n') {
			n = prevN;
			break;
		}

		return false;
	} while (false);
	prevN = n;

	if (fgStarTexture[n].empty())
		return false;
	long tn = rand() % fgStarTexture[n].size();
	GLuint texture = fgStarTexture[n][tn];
	if (texture == 0)
		return false;

	bool flagPushFront = true;
	// bool flagSort = true;
	double x = 0.0;
	double y = 0.0;
	double z = 0.0;
	double absVX = ::absDouble(this->vx);
	double absVY = ::absDouble(this->vy);
	double absVZ = ::absDouble(this->vz);
	if ((absVZ == absVX) && (absVZ == absVY)) {
		x = resetStarX(1.0);
		y = resetStarY(1.0);
		z = resetStarZ(1.0);
	} else if ((absVZ > absVX) && (absVZ > absVY)) {
		x = resetStarX(0.2);
		y = resetStarY(0.2);

		if (sgnDouble(this->vz) >= 0.0) {
			z = this->z + spaceMaxZR;
		} else {
			z = this->z + 0.0;
			flagPushFront = false;
		}

		// flagSort = false;
	} else if (absVX > absVY) {
		y = resetStarY(1.0);
		z = resetStarZ(1.0);

		x = this->x + (spaceMaxXR * sgnDouble(this->vx));
	} else {
		x = resetStarX(1.0);
		z = resetStarZ(1.0);

		y = this->y + (spaceMaxYR * sgnDouble(this->vy));
	}

	Star st;
	st.born(x, y, z, texture);
	if (flagPushFront)
		fgStarArray.push_front(st);
	else
		fgStarArray.push_back(st);

	return true;
}

////////////////////////////////////////////////////////////////
// 背景の星を生成させる
////////////////////////////////////////////////////////////////

bool Space::bearBgStar()
{
	double x = resetStarX(1.0);
	double y = resetStarY(1.0);
	double z = resetStarY(1.0);

	if (bgStarTexture.empty())
		return false;
	long tn = rand() % bgStarTexture.size();
	GLuint texture = bgStarTexture[tn];
	if (texture == 0)
		return false;

	Star st;
	st.born(x, y, z, texture);
	bgStarArray.push_front(st);

	return true;
}

////////////////////////////////////////////////////////////////
// 前景の星を自動的に生成させる
////////////////////////////////////////////////////////////////

void Space::autoBearFgStar()
{
	static long count = 0;
	if (count <= 0) {
		bearFgStar('/');
		count = (rand() % autoBearFrame) + 1;
	}
	--count;
}

////////////////////////////////////////////////////////////////
// 前景の星のテクスチャ番号をランダムで返す
////////////////////////////////////////////////////////////////

long Space::randFgStar()
{
	const long maxNum = 'z' - 'a' + 1;
	long n = (rand() % maxNum);

	for (long i = 'a'; i <= 'z'; ++i) {
		if (fgStarTexture[n % maxNum][0] != 0)
			return (n % maxNum);
		++n;
	}

	return 0;
}

////////////////////////////////////////////////////////////////
// 星のX座標をリセット
////////////////////////////////////////////////////////////////

double Space::resetStarX(double r)
{
	double n = ::randDouble() * (spaceMaxXR * 2) - spaceMaxXR;
	n *= r;
	n += x;
	return n;
}

////////////////////////////////////////////////////////////////
// 星のY座標をリセット
////////////////////////////////////////////////////////////////

double Space::resetStarY(double r)
{
	double n = ::randDouble() * (spaceMaxYR * 2) - spaceMaxYR;
	n *= r;
	n += y;
	return n;
}

////////////////////////////////////////////////////////////////
// 星のZ座標をリセット
////////////////////////////////////////////////////////////////

double Space::resetStarZ(double r)
{
	double n = ::randDouble() * spaceMaxZR;
	n *= r;
	n += z;
	return n;
}

////////////////////////////////////////////////////////////////
// 全ての星を消滅させる
////////////////////////////////////////////////////////////////

void Space::killAllStar(list<Star> *listStar)
{
	for (list<Star>::iterator i = listStar->begin();
		i != listStar->end();++ i)
	{
		i->die();
	}

	listStar->clear();
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////

void Space::incSpeed(long ax, long ay, long az)
{
	if (ax != 0) {
		this->ax2 = spaceAX * ax;
		this->bx = false;
	}
	if (ay != 0) {
		this->ay2 = spaceAY * ay;
		this->by = false;
	}
	if (az != 0) {
		this->az2 = spaceAZ * az;
		this->bz = false;
	}
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////

void Space::keepSpeed(long ax, long ay, long az)
{
	if (ax != 0) {
		this->ax2 = 0.0;
		this->bx = false;
	}
	if (ay != 0) {
		this->ay2 = 0.0;
		this->by = false;
	}
	if (az != 0) {
		this->az2 = 0.0;
		this->bz = false;
	}
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////

void Space::accelerate(long ax, long ay, long az)
{
	if (ax != 0) {
		this->ax = spaceAX * ax;
		this->bx = false;
	}
	if (ay != 0) {
		this->ay = spaceAY * ay;
		this->by = false;
	}
	if (az != 0) {
		this->az = spaceAZ * az;
		this->bz = false;
	}
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////

void Space::decelerate(long ax, long ay, long az)
{
	if (ax != 0) {
		this->ax = +-0.0;
		this->bx = true;
	}
	if (ay != 0) {
		this->ay = +-0.0;
		this->by = true;
	}
	if (az != 0) {
		this->az = +-0.0;
		this->bz = true;
	}
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////

void Space::updateSpeed()
{
	vx += ax + ax2;
	vy += ay + ay2;
	vz += az + az2;

	if (bx)
		vx += -(::sgnDouble(vx) * spaceAX * 0.5);
	if (by)
		vy += -(::sgnDouble(vy) * spaceAY * 0.5);
	if (bz)
		vz += -(::sgnDouble(vz) * spaceAZ * 0.5);

	checkSpeedLimit();
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////

void Space::checkSpeedLimit()
{
	if (vx > +spaceMaxVX)
		vx = +spaceMaxVX;
	if (vx < -spaceMaxVX)
		vx = -spaceMaxVX;

	if (vy > +spaceMaxVY)
		vy = +spaceMaxVY;
	if (vy < -spaceMaxVY)
		vy = -spaceMaxVY;

	if (vz > +spaceMaxVZ)
		vz = +spaceMaxVZ;
	if (vz < -spaceMaxVZ)
		vz = -spaceMaxVZ;

	if (::absDouble(vx) < spaceAX)
		vx = +-0.0;
	if (::absDouble(vy) < spaceAY)
		vy = +-0.0;
	if (::absDouble(vz) < spaceAZ)
		vz = +-0.0;
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////

void Space::updateMoving()
{
	if (flagMoveCamera) {
		x += vx;
		y += vy;
		z += vz;
	} else {
		for (list<Star>::iterator i = bgStarArray.begin();
			i != bgStarArray.end();++ i)
		{
			double stx, sty, stz;
			i->getPos(&stx, &sty, &stz);
			stx -= vx;
			sty -= vy;
			stz -= vz;
			i->setPos(stx, sty, stz);
		}

		for (list<Star>::iterator i = fgStarArray.begin();
			i != fgStarArray.end();++ i)
		{
			double stx, sty, stz;
			i->getPos(&stx, &sty, &stz);
			stx -= vx;
			sty -= vy;
			stz -= vz;
			i->setPos(stx, sty, stz);
		}
	}
}

////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////

void Space::update()
{
	if (flagAutoBear)
		autoBearFgStar();

	updateSpeed();
	updateMoving();

	double spx = x * 640.0 * 4;
	double spy = y * 640.0 * 4;
	double spz = z * 640.0;

	::glMatrixMode(GL_MODELVIEW);
	::glLoadIdentity();
	::gluLookAt(
		spx, spy, spz,
		spx, spy, (spz + 1.0),
		0.0, 1.0, 0.0);

	for (list<Star>::iterator i = bgStarArray.begin();
		i != bgStarArray.end(); ++i)
	{
		double stx, sty, stz;
		i->getPos(&stx, &sty, &stz);

		if ((stx - x) < -spaceMaxXR)
			stx += +spaceMaxXR * 2;
		if ((stx - x) > +spaceMaxXR)
			stx += -spaceMaxXR * 2;

		if ((sty - y) < -spaceMaxYR)
			sty += +spaceMaxYR * 2;
		if ((sty - y) > +spaceMaxYR)
			sty += -spaceMaxYR * 2;

		bool flagReset = false;
		if ((stz - z) < 0.0) {
			stz += spaceMaxZR;
			flagReset = true;
		}
		if ((stz - z) > spaceMaxZR) {
			stz -= spaceMaxZR;
			flagReset = true;
		}
		if (flagReset) {
			stx = resetStarX(1.0);
			sty = resetStarY(1.0);
		}

		i->setPos(stx, sty, stz);
	}

	for (list<Star>::iterator i = fgStarArray.begin();
		i != fgStarArray.end(); ++i)
	{
		if (!i->getFlagLiving())
			continue;

		double stx, sty, stz;
		i->getPos(&stx, &sty, &stz);

		if ((stz - z) < 0.0) {
			i->die();
			i = fgStarArray.erase(i);
			continue;
		}
		if ((stz - z) > spaceMaxZR) {
			i->die();
			i = fgStarArray.erase(i);
			continue;
		}

		if (::absDouble(stx - x) > spaceMaxXR) {
			i->die();
			i = fgStarArray.erase(i);
			continue;
		}
		if (::absDouble(sty - y) > spaceMaxYR) {
			i->die();
			i = fgStarArray.erase(i);
			continue;
		}
	}
}

////////////////////////////////////////////////////////////////
// 宇宙を描画
////////////////////////////////////////////////////////////////

void Space::draw()
{
	if (!flagDraw)
		return;

	::glPushMatrix();

	::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	::glColor4d(1.0, 1.0, 1.0, 0.0);
	::glClearColor(0.0, 0.0, 0.0, 1.0);
	::glBindTexture(GL_TEXTURE_2D, bgTexture);

	::glDisable(GL_BLEND);
	::glDepthMask(1);
	::glEnable(GL_DEPTH);

	double bgw = 640.0 * 2.4;
	double bgh = 480.0 * 2.4;
	double bgx = x * 640.0 * 4;
	double bgy = y * 640.0 * 4;
	double bgz = (z * 640.0 + 640.0);
	::glBegin(GL_QUADS);
	::glTexCoord2d(0.0, 0.0);
	::glVertex3d(bgx + bgw, bgy + bgh, bgz);
	::glTexCoord2d(1.0, 0.0);
	::glVertex3d(bgx - bgw, bgy + bgh, bgz);
	::glTexCoord2d(1.0, 1.0);
	::glVertex3d(bgx - bgw, bgy - bgh, bgz);
	::glTexCoord2d(0.0, 1.0);
	::glVertex3d(bgx + bgw, bgy - bgh, bgz);
	::glEnd();

	::glPopMatrix();

	for (list<Star>::iterator i = bgStarArray.begin();
		i != bgStarArray.end(); ++i)
	{
		i->draw(8.0);
	}

	for (list<Star>::iterator i = fgStarArray.begin();
		i != fgStarArray.end(); ++i)
	{
		i->draw(32.0);
	}
}

////////////////////////////////////////////////////////////////
// 画面サイズの変更
////////////////////////////////////////////////////////////////

void Space::reshape(long w, long h)
{
	::glViewport(0, 0, w, h);
	::glMatrixMode(GL_PROJECTION);
	::glLoadIdentity();

	::gluPerspective(480.0, (double)w / (double)h, 1.0, 640.0 + 1.0);

	::glMatrixMode(GL_MODELVIEW);
}

////////////////////////////////////////////////////////////////
// 硬直のON/OFFを設定
////////////////////////////////////////////////////////////////

bool Space::setFlagFreezed(bool flag)
{
	bool prevFlag = flagFreezed;
	flagFreezed = flag;
	return prevFlag;
}

////////////////////////////////////////////////////////////////
// 描画のON/OFFを設定
////////////////////////////////////////////////////////////////

bool Space::setFlagDraw(bool flag)
{
	bool prevFlag = flagDraw;
	flagDraw = flag;
	return prevFlag;
}

////////////////////////////////////////////////////////////////
// 前景の星を自動的に生成させるかを設定
////////////////////////////////////////////////////////////////

bool Space::setFlagAutoBear(bool flag)
{
	bool prevFlag = flagAutoBear;
	flagAutoBear = flag;
	return prevFlag;
}

////////////////////////////////////////////////////////////////
// 前景の星を自動的に生成させるかを反転
////////////////////////////////////////////////////////////////

bool Space::toggleFlagAutoBear()
{
	flagAutoBear = !flagAutoBear;
	return flagAutoBear;
}
