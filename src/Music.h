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

#ifndef MUSIC_H
#define MUSIC_H	1

////////////////////////////////////////////////////////////////
// BGM �δ���
////////////////////////////////////////////////////////////////

#include <string>

#include "GameMain.h"
#include "Option.h"

////////////////////////////////////////////////////////////////

namespace StarSnowNotes {

////////////////////////////////////////////////////////////////

// BGM �Υե�����̾�γ�ĥ��
#define	STR_MUSIC_FILE_EXT	"ogg mp3"

// BGM �Υե�����̾����
typedef vector<std::string> MusicArray;

////////////////////////////////////////////////////////////////

class Music {
public:

private:
#ifdef	HAVE_SDL_SDL_MIXER_H
	Mix_Music *mixMusicData;
#endif

	int audioRate;
	unsigned short audioFormat;
	int audioChannels;
	int audioBuffers;

	long currentIdx;
	std::string currentName;

	MusicArray musicList;

	Option *setting;

public:
	Music();
	~Music();
	void init(Option *opt);
	void close();
	void setVolume(long rate);
	std::string playRandom();
	std::string play(long idx);

private:
};

}

#endif /* MUSIC_H */
