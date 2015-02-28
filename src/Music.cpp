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

////////////////////////////////////////////////////////////////
// BGM の管理
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
// コンストラクタ
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
// デストラクタ
////////////////////////////////////////////////////////////////

Music::~Music()
{
	this->close();
}

////////////////////////////////////////////////////////////////
// 初期化
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

	// BGMを検索
	FileList::setStrDirSelMusic(setting->getMusicDir());
	FileList fls;
	musicList.clear();
	fls.reset(".", STR_MUSIC_FILE_EXT);
	while (1) {
		std::string path = fls.next();
		if (path.empty())
			break;

		// 登録
		musicList.push_back(path);
	}

	// 音量

	setVolume(100);
}

////////////////////////////////////////////////////////////////
// BGM 処理を終了する
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
// BGM の音量を設定
// long rate : 音量 (0 ~ 100%)
////////////////////////////////////////////////////////////////

void Music::setVolume(long rate)
{
#ifdef	HAVE_SDL_SDL_MIXER_H
	Mix_VolumeMusic(MIX_MAX_VOLUME * rate / 100);
#endif
}

////////////////////////////////////////////////////////////////
// リストからランダムに選んで BGM を再生
// void (*func)() : 終了時のコールバック
// return : BGM ファイルのパス
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
// タイトル画面の BGM の再生
// long idx : BGM リストのインデックス
// void (*func)() : 終了時のコールバック
// return : BGM ファイルのパス
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
// BGM の終了時の処理
////////////////////////////////////////////////////////////////

void donePlayMusic()
{
	gMusic->playRandom();
}
