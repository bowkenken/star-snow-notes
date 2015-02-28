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

#ifndef MUSIC_H
#define MUSIC_H	1

////////////////////////////////////////////////////////////////
// BGM の管理
////////////////////////////////////////////////////////////////

#include <string>

#include "GameMain.h"
#include "Option.h"

////////////////////////////////////////////////////////////////

namespace StarSnowNotes {

////////////////////////////////////////////////////////////////

// BGM のファイル名の拡張子
#define	STR_MUSIC_FILE_EXT	"ogg mp3"

// BGM のファイル名配列
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
