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

#ifndef CONTROL_PANEL_H
#define CONTROL_PANEL_H

#include "GameMain.h"
#include "Option.h"
#include "Button.h"

////////////////////////////////////////////////////////////////
// ������
////////////////////////////////////////////////////////////////

namespace StarSnowNotes {

class ControlPanel {
public:

private:
	// �ǥե���Ȥ�ɽ���ΰ�Υ�����
	static const long defaultScreenWidth = 640;
	static const long defaultScreenHeight = 480;

	// �ܥ���γʻҤο�
	static const long gridHorizontalNum = 6;
	static const long gridVerticalNum = 6;

	// ���Υܥ���κ����
	static const long starButtonMaxNum = 'z' - 'a' + 1;

	Option optionSetting;

	// ����椫?
	bool flagFreezed;
	// ���褹�뤫?
	bool flagDraw;
	// ���ƤΥܥ����ɽ�����뤫?
	bool flagDrawAllStarButton;

	// ɽ���ΰ����
	long screenWidth;
	// ɽ���ΰ�ι⤵
	long screenHeight;

	// ���ܥ�����гѤΥ�����ɥ���ɸ
	long starWinMinX, starWinMinY, starWinMinZ;
	long starWinMaxX, starWinMaxY, starWinMaxZ;

	// �ܥ���
	std::vector<StarSnowNotes::Button *> starButtonArray;
	StarSnowNotes::Button *starButtonSpace;
	StarSnowNotes::Button *starButtonEnter;

	// �ޥ�����X��ɸ
	long mouseX;
	// �ޥ�����Y��ɸ
	long mouseY;
	// �ޥ����Υܥ���β�������
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
