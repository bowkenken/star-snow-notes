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

#ifndef BUTTON_H
#define BUTTON_H

#include "GameMain.h"

namespace StarSnowNotes {

////////////////////////////////////////////////////////////////
// �ܥ���ο�
////////////////////////////////////////////////////////////////

struct Color {
	unsigned char r, g, b, a;
};

////////////////////////////////////////////////////////////////
// �ܥ���
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
	// �ǥե���ȤΥ�����
	static const long defaultWidth = 640 / 6;
	static const long defaultHeight = 480 / 6;

	// ���褹�뤫?
	bool flagDraw;
	// ��������ǥݥ���Ȥ���Ƥ��뤫?
	bool flagPointed;
	// ������Ƥ��뤫?
	bool flagPressed;

	// X��ɸ
	long x;
	// Y��ɸ
	long y;
	// ��
	long width;
	// �⤵
	long height;
	// �гѤΥ�����ɥ���ɸ
	long winMinX, winMinY, winMinZ;
	long winMaxX, winMaxY, winMaxZ;

	// ���Ȥο�
	static Color colorFrame[COLOR_IDX_MAX];
	// ������ο�
	static Color colorRect[COLOR_IDX_MAX];

	// ����ץ����
	std::string strCaption;

	// ����å����줿����ȯ�����륭��
	SDLKey key;
	// ����å����줿����ȯ�����뽤������
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
