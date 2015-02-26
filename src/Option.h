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
	OPTION_IDX_WIDTH,
	OPTION_IDX_HEIGHT,
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

	OPTION_IDX_BEGIN = 0,
	OPTION_IDX_END = OPTION_IDX_MAX - 1,
};

enum OptionType {
	OPTION_TYPE_FLAG,
	OPTION_TYPE_NUM,
	OPTION_TYPE_KEY,
	OPTION_TYPE_FILE,
	OPTION_TYPE_MAX,
};

enum ArgArrayIdx {
	ARG_ARRAY_IDX_GRAPH,
	ARG_ARRAY_IDX_MUSIC,
	ARG_ARRAY_IDX_MAX,
};

typedef std::vector<std::string> ArgStrArray;

////////////////////////////////////////////////////////////////
// 引数
////////////////////////////////////////////////////////////////

class Option {
public:

private:
	static const int CAPTION_MAX = 'z' - 'a' + 1;

	bool flagModifiedOption[OPTION_IDX_MAX];
	bool flagModifiedCaption[CAPTION_MAX];
	bool flagModifiedCaptionSpace;
	bool flagModifiedCaptionEnter;
	bool flagModifiedGraphDir;
	bool flagModifiedMusicDir;

	enum OptionType OptionTypeArray[OPTION_IDX_MAX];

	bool flag[OPTION_IDX_MAX];
	double num[OPTION_IDX_MAX];
	int key[OPTION_IDX_MAX];
	std::string file[OPTION_IDX_MAX];

	std::string caption[CAPTION_MAX];
	std::string captionSpace;
	std::string captionEnter;

	std::string graphDir;
	std::string musicDir;

	std::vector<std::string> argArray;

public:
	Option();
	~Option();
	void init();

	void loadAllConfig(Option *opt);
	void saveAllConfig();

	void parseOption(int argc, char **argv);

	void setFlag(OptionIdx idx, bool flag);
	void setNum(OptionIdx idx, double num);
	void setKey(OptionIdx idx, int key);
	void setFile(OptionIdx idx, const std::string &file);

	void setCaption(char key, const std::string &str);
	void setGraphDir(const std::string &dir);
	void setMusicDir(const std::string &dir);

	std::string getFlagString(OptionIdx idx);
	bool getFlag(OptionIdx idx);
	double getNum(OptionIdx idx);
	std::string getKeyString(OptionIdx idx);
	int getKey(OptionIdx idx);
	std::string getFile(OptionIdx idx);

	std::string getCaption(char key);
	std::string getGraphDir();
	std::string getMusicDir();

	std::string convertKeyToString(int key);
	std::string quoteString(std::string str);
	std::string escapeString(std::string str, bool flagEscapeSpace);

	void usage(FILE *fp);
	void version(FILE *fp);

private:
	// Option();
	// ~Option();
	// void init();

	// void loadAllConfig(Option *opt);
	void loadCommonConfig();
	void loadGraphConfig();
	void loadConfig(std::string path);
	void loadConfigContents(ArgStrArray *argStr, FILE *fp);

	bool loadConfigToken(std::string *str, FILE *fp);
	char loadConfigEscapeChar(FILE *fp);

	void transConfigStringToArgv(
		int *argc, char ***argv, ArgStrArray *argStr);

	void mergeCommonConfig(Option *opt);
	void mergeGraphConfig(Option *opt);
	void mergeGraphConfigOption(Option *opt, OptionIdx n);
	void mergeGraphConfigCaption(Option *opt, char key);

	// void saveAllConfig();
	void saveConfigVersion(FILE *fp);
	void saveCommonConfig();
	void saveCommonConfigContents(FILE *fp);
	void saveGraphConfig();
	void saveGraphConfigContents(FILE *fp);

	FILE *openConfig(const char *path, const char *mode);
	int closeConfig(FILE *fp);

	int printfConfig(FILE *fp, const char *fmt, ...);

	std::string getCommonConfigPath();
	std::string getGraphConfigPath();

	// void parseOption(int argc, char **argv);
	void parseArg(int argc, char **argv, int optind);
	void parseKeyValue(
		const std::string &key, const std::string &value);

	bool parseFlag(const char *optarg);
	double parseNum(const char *optarg);
	char parseChar(const char *optarg);

	// void setFlag(OptionIdx idx, bool flag);
	// void setNum(OptionIdx idx, double num);
	// void setKey(OptionIdx idx, int key);
	// void setFile(OptionIdx idx, const std::string &file);

	// std::string getFlagString(OptionIdx idx);
	// bool getFlag(OptionIdx idx);
	// double getNum(OptionIdx idx);
	// std::string getKeyString(OptionIdx idx);
	// int getKey(OptionIdx idx);
	// std::string getFile(OptionIdx idx);

	// std::string getCaption(char key);
	// std::string getStringGraphDir();
	// std::string getStringMusicDir();

	bool getFlagModifiedCaption(char key);;

	// std::string convertKeyToString(int key);
	// std::string quoteString(std::string str);
	// std::string escapeString(std::string str, bool flagEscapeSpace);

	// void usage(FILE *fp);
	// void version(FILE *fp);

};

}

#endif // OPTION_H
