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

////////////////////////////////////////////////////////////////
// BGM �δ���
////////////////////////////////////////////////////////////////

#include "GameMain.h"
#include "Option.h"
#include "FileList.h"
#include "Music.h"

using namespace StarSnowNotes;

////////////////////////////////////////////////////////////////

static Music *gMusic;
static void donePlayMusic();

////////////////////////////////////////////////////////////////
// ���󥹥ȥ饯��
////////////////////////////////////////////////////////////////

Music::Music()
{
	gMusic = this;

#ifdef	HAVE_SDL_SDL_MIXER_H
	mixMusicData = NULL;
#endif

#ifdef	HAVE_SDL_SDL_MIXER_H
	audioRate = 22050;
	audioFormat = AUDIO_S16;
	audioChannels = 2;
	audioBuffers = 1024;
#else
	audioRate = 22050;
	audioFormat = 0;
	audioChannels = 2;
	audioBuffers = 1024;
#endif

	currentIdx = 0;
	currentName = "";
}

////////////////////////////////////////////////////////////////
// �ǥ��ȥ饯��
////////////////////////////////////////////////////////////////

Music::~Music()
{
	this->close();
}

////////////////////////////////////////////////////////////////
// �����
////////////////////////////////////////////////////////////////

void Music::init(Option *opt)
{
	if (opt == NULL) {
		opt = new Option;
		opt->init();
	}
	setting = opt;

#ifdef	HAVE_SDL_SDL_MIXER_H
	if (Mix_OpenAudio(audioRate, audioFormat,
		audioChannels, audioBuffers))
	{
		return;
	}
	Mix_QuerySpec(&audioRate, &audioFormat, &audioChannels);
#endif

	// BGM�򸡺�
	FileList::setStrDirSelMusic(setting->getMusicDir());
	FileList fls;
	musicList.clear();
	fls.reset(".", STR_MUSIC_FILE_EXT);
	while (1) {
		std::string path = fls.next();
		if (path.empty())
			break;

		// ��Ͽ
		musicList.push_back(path);
	}

	// ����

	setVolume(100);
}

////////////////////////////////////////////////////////////////
// BGM ������λ����
////////////////////////////////////////////////////////////////

void Music::close()
{
#ifdef	HAVE_SDL_SDL_MIXER_H
	if (mixMusicData != NULL) {
		Mix_HaltMusic();
		Mix_FreeMusic(mixMusicData);
		mixMusicData = NULL;
	}
#endif
}

////////////////////////////////////////////////////////////////
// BGM �β��̤�����
// long rate : ���� (0 ~ 100%)
////////////////////////////////////////////////////////////////

void Music::setVolume(long rate)
{
#ifdef	HAVE_SDL_SDL_MIXER_H
	Mix_VolumeMusic(MIX_MAX_VOLUME * rate / 100);
#endif
}

////////////////////////////////////////////////////////////////
// �ꥹ�Ȥ�������������� BGM �����
// void (*func)() : ��λ���Υ�����Хå�
// return : BGM �ե�����Υѥ�
////////////////////////////////////////////////////////////////

std::string Music::playRandom()
{
	long maxIdx = musicList.size();
	if (maxIdx <= 0)
		return "";

	long idx = rand() % maxIdx;

	return play(idx);
}

////////////////////////////////////////////////////////////////
// �����ȥ���̤� BGM �κ���
// long idx : BGM �ꥹ�ȤΥ���ǥå���
// void (*func)() : ��λ���Υ�����Хå�
// return : BGM �ե�����Υѥ�
////////////////////////////////////////////////////////////////

std::string Music::play(long idx)
{
	if (idx <= -1)
		return "";
	if (idx >= (long)(musicList.size()))
		return "";

	currentIdx = idx;
	currentName = musicList[idx];
	long nRepeat = 1;

#ifdef	HAVE_SDL_SDL_MIXER_H
	Mix_HaltMusic();

	mixMusicData = Mix_LoadMUS(currentName.c_str());
	Mix_PlayMusic(mixMusicData, nRepeat);

	Mix_HookMusicFinished(donePlayMusic);
#endif

	return currentName;
}

////////////////////////////////////////////////////////////////
// BGM �ν�λ���ν���
////////////////////////////////////////////////////////////////

void donePlayMusic()
{
	gMusic->playRandom();
}
