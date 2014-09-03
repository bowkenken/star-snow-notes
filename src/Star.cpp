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

#include "Star.h"

////////////////////////////////////////////////////////////////
// ���Υ��󥹥ȥ饯��
////////////////////////////////////////////////////////////////

Star::Star()
{
	flagLiving = false;
	x = 0.0;
	y = 0.0;
	z = 0.0;
}

////////////////////////////////////////////////////////////////
// ���Υǥ��ȥ饯��
////////////////////////////////////////////////////////////////

Star::~Star()
{
	flagLiving = false;
}

////////////////////////////////////////////////////////////////
// ��������
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
// �������
////////////////////////////////////////////////////////////////

bool Star::die()
{
	flagLiving = false;

	return true;
}

////////////////////////////////////////////////////////////////
// ��������
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
// ���������Ƥ��뤫?
////////////////////////////////////////////////////////////////

bool Star::getFlagLiving()
{
	return flagLiving;
}

////////////////////////////////////////////////////////////////
// �����ɸ���֤�
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
// �����ɸ������
////////////////////////////////////////////////////////////////

void Star::setPos(double sx, double sy, double sz)
{
	x = sx;
	y = sy;
	z = sz;
}

