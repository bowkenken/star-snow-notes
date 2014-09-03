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

#ifndef SPACE_H
#define SPACE_H

#include <string>

#include "GameMain.h"
#include "Option.h"
#include "Star.h"

////////////////////////////////////////////////////////////////

namespace StarSnowNotes {

////////////////////////////////////////////////////////////////
// ���� - ���δ���
////////////////////////////////////////////////////////////////

class Space {
public:

private:
	// �����β�®��
	static const double spaceAX = 0.0002;
	static const double spaceAY = 0.0002;
	static const double spaceAZ = 0.0002;

	// �����κǹ�®��
	static const double spaceMaxVX = 0.02;
	static const double spaceMaxVY = 0.02;
	static const double spaceMaxVZ = 0.02;

	// ����κ���Ⱦ��
	static const double spaceMaxXR = 0.5;
	static const double spaceMaxYR = 0.5;
	static const double spaceMaxZR = 1.0;

	// ���Υƥ�������κ����
	static const long fgStarTextureMaxNum = 'z' - 'a' + 1;

	// ���褹�뤫?
	bool flagDraw;
	// ����椫?
	bool flagFreezed;
	// ������ư������?�ޤ�������ư������?
	bool flagMoveCamera;
	// ���ʤ�����ưŪ�����������뤫?
	bool flagAutoBear;

	// �����κ�ɸ
	double x, y, z;
	// �����β�®��
	double ax, ay, az;
	double ax2, ay2, az2;
	// �����ΰ�ư®��
	double vx, vy, vz;
	// �����Υ֥졼��
	bool bx, by, bz;

	// �طʤΥƥ�������
	GLuint bgTexture;
	// ���Υƥ�������
	vector< vector<GLuint> > fgStarTexture;
	vector<GLuint> bgStarTexture;

	// ��
	list<Star> fgStarArray;
	list<Star> bgStarArray;

	// ���ʤ�����ưŪ������������ե졼���
	long autoBearFrame;
	// ľ���������������μ���
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
