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

#ifndef OPTION_H
#define OPTION_H

#include "string"

////////////////////////////////////////////////////////////////

namespace StarSnowNotes {

enum OptionIdx {
	OPTION_IDX_INIT,
	OPTION_IDX_SAVE,
	OPTION_IDX_STAR_NUMBER,
	OPTION_IDX_FULL_SCREEN,
	OPTION_IDX_FPS,
	OPTION_IDX_BG_FILE,
	OPTION_IDX_INTERVAL_AUTO,
	OPTION_IDX_AUTO_KEY,
	OPTION_IDX_X_SPEED,
	OPTION_IDX_Y_SPEED,
	OPTION_IDX_Z_SPEED,
	OPTION_IDX_REVERSE_X,
	OPTION_IDX_REVERSE_Y,
	OPTION_IDX_REVERSE_Z,
	OPTION_IDX_REVERSE_SHIFT,
	OPTION_IDX_VERSION,
	OPTION_IDX_HELP,
	OPTION_IDX_DEBUG,
	OPTION_IDX_MAX,
};

enum ArgArrayIdx {
	ARG_ARRAY_IDX_GRAPH,
	ARG_ARRAY_IDX_MUSIC,
	ARG_ARRAY_IDX_MAX,
};

////////////////////////////////////////////////////////////////
// 引数
////////////////////////////////////////////////////////////////

class Option {
public:

private:
	bool flagModified[OPTION_IDX_MAX];

	bool flag[OPTION_IDX_MAX];
	double num[OPTION_IDX_MAX];
	int key[OPTION_IDX_MAX];
	std::string file[OPTION_IDX_MAX];

	static const int CAPTION_MAX = 'z' - 'a' + 1;
	std::string caption[CAPTION_MAX];
	std::string captionSpace;
	std::string captionEnter;

	std::vector<std::string> argArray;
	std::string stringGraphDir;
	std::string stringMusicDir;

public:
	Option();
	~Option();
	void init();

	void saveAllConfig();
	// void saveCommonConfig();
	// void saveGraphConfig();
	// FILE *openConfig(const char *path, const char *mode);
	// int closeConfig(const FILE *fp);
	// int printfConfig(FILE *fp, const char *fmt, ...);
	// std::string getCommonConfigPath();
	// std::string getGraphConfigPath();

	void checkOption(int argc, char **argv);
	// void checkArg(int argc, char **argv, int optind);
	// void checkKeyValue(
		// const std::string &key, const std::string &value);

	// bool checkFlag(const char *optarg);
	// double checkNum(const char *optarg);
	// char checkChar(const char *optarg);

	void setFlag(OptionIdx idx, bool flag);
	void setNum(OptionIdx idx, double num);
	void setKey(OptionIdx idx, int key);
	void setFile(OptionIdx idx, const std::string &file);

	bool getFlag(OptionIdx idx);
	double getNum(OptionIdx idx);
	int getKey(OptionIdx idx);
	const std::string &getFile(OptionIdx idx);

	std::string getCaption(char key);
	std::string getStringGraphDir();
	std::string getStringMusicDir();

	std::string quoteString(std::string str);

	void usage(FILE *fp);
	void version(FILE *fp);

private:
	void saveCommonConfig();
	void saveGraphConfig();
	FILE *openConfig(const char *path, const char *mode);
	int closeConfig(FILE *fp);
	int printfConfig(FILE *fp, const char *fmt, ...);
	std::string getCommonConfigPath();
	std::string getGraphConfigPath();

	void checkArg(int argc, char **argv, int optind);
	void checkKeyValue(const std::string &key, const std::string &value);

	bool checkFlag(const char *optarg);
	double checkNum(const char *optarg);
	char checkChar(const char *optarg);

};

}

#endif // OPTION_H
