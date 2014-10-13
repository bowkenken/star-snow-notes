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

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

#include "GameMain.h"
#include "FileList.h"
#include "Option.h"

using namespace StarSnowNotes;

////////////////////////////////////////////////////////////////
// 定数
////////////////////////////////////////////////////////////////

// 設定ファイルのバージョン番号
// 設定ファイルの仕様に変更が有った時だけ
// その時点のプログラムのバージョンと同じ数字に上げて一致させる。
// 変更が無ければ、そのままにして置く。
static const long OPTION_VERSION_MAJOR = 1;
static const long OPTION_VERSION_MINOR = 0;
static const long OPTION_VERSION_PATCH = 0;
// 変更履歴
// 1.0.0 : 2014/10/13 Initial version

static const char *DEFAULT_GRAPH_DIR = "default";
static const char *DEFAULT_MUSIC_DIR = "default";

static const char *COMMON_CONFIG_FILE = "ssn-conf.txt";
static const char *GRAPH_CONFIG_FILE = "ssn-graph-conf.txt";

static const char gStringOption[] = "is:n:f:F:b:A:a:x:y:z:X:Y:Z:R:Vvhd";

#ifdef	HAVE_GETOPT_LONG
static const struct option	gLongOption[] = {
	{ "init",           no_argument,       NULL, 'i' },
	{ "save",           required_argument, NULL, 's' },
	{ "star-number",    required_argument, NULL, 'n' },
	{ "full-screen",    required_argument, NULL, 'f' },
	{ "fps",            required_argument, NULL, 'F' },
	{ "bg-file",        required_argument, NULL, 'b' },
	{ "interval-auto",  required_argument, NULL, 'A' },
	{ "auto-key",       required_argument, NULL, 'a' },
	{ "x-speed",        required_argument, NULL, 'x' },
	{ "y-speed",        required_argument, NULL, 'y' },
	{ "z-speed",        required_argument, NULL, 'z' },
	{ "reverse-x",      required_argument, NULL, 'X' },
	{ "reverse-y",      required_argument, NULL, 'Y' },
	{ "reverse-z",      required_argument, NULL, 'Z' },
	{ "reverse-shift",  required_argument, NULL, 'R' },
	{ "version",        no_argument,       NULL, 'V' },
	{ "help",           no_argument,       NULL, 'h' },
	{ "debug",          no_argument,       NULL, 'd' },
	{ 0, 0, 0, 0 }
};
#endif	// HAVE_GETOPT_LONG

////////////////////////////////////////////////////////////////
// 使用方
////////////////////////////////////////////////////////////////

static const char	gStringUsage[] = {
	"Usage: %s [OPTION] [graphic dir] [music dir]\n"
	"OPTION:\n"
	"  -i, --init                initialize settings\n"
	"  -s, --save=FLAG           save settings\n"
	"  -n, --star-number=NUM     set BG star number\n"
	"  -f, --full-screen=FLAG    set full screen mode\n"
	"  -F, --fps=NUM             set frame per second\n"
	"  -b, --bg-file=FILE        select BG file\n"
	"  -A, --interval-auto=NUM   set interval of auto generate\n"
	"  -a, --auto-key=KEY        auto generate star by key\n"
	"  -x, --x-speed=NUM         set x speed\n"
	"  -y, --y-speed=NUM         set y speed\n"
	"  -z, --z-speed=NUM         set z speed\n"
	"  -X, --reverse-x=FLAG      set x axis reverse\n"
	"  -Y, --reverse-y=FLAG      set y axis reverse\n"
	"  -Z, --reverse-z=FLAG      set z axis reverse\n"
	"  -R, --reverse-shift=FLAG  set shift key reverse\n"
	"  -V, --version             display version and exit\n"
	"  -h, --help                display this help and exit\n"
#if	defined(DEBUG) || !defined(NDEBUG)
	"  -d, --debug               debugging mode\n"
#endif
};

////////////////////////////////////////////////////////////////
// 変数
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
// 引数のコンストラクタ
////////////////////////////////////////////////////////////////

Option::Option()
{
	init();
}

////////////////////////////////////////////////////////////////
// 引数のデストラクタ
////////////////////////////////////////////////////////////////

Option::~Option()
{
}

////////////////////////////////////////////////////////////////
// 引数の初期化
////////////////////////////////////////////////////////////////

void Option::init()
{
	for (int i = 0; i < OPTION_IDX_MAX; i++) {
		flag[i] = false;
		num[i] = 0.0;
		key[i] = ' ';
		file[i] = "err";
	}
	setFlag(OPTION_IDX_INIT, false);
	setFlag(OPTION_IDX_SAVE, false);
	setNum(OPTION_IDX_STAR_NUMBER, 10240);
	setFlag(OPTION_IDX_FULL_SCREEN, false);
	setNum(OPTION_IDX_FPS, 30);
	setFile(OPTION_IDX_BG_FILE, "");
	setNum(OPTION_IDX_INTERVAL_AUTO, 30);
	setKey(OPTION_IDX_AUTO_KEY, '\0');
	setNum(OPTION_IDX_X_SPEED, +0.0);
	setNum(OPTION_IDX_Y_SPEED, +0.0);
	setNum(OPTION_IDX_Z_SPEED, +0.01);
	setFlag(OPTION_IDX_REVERSE_X, false);
	setFlag(OPTION_IDX_REVERSE_Y, true);
	setFlag(OPTION_IDX_REVERSE_Z, false);
	setFlag(OPTION_IDX_REVERSE_SHIFT, false);

	for (int i = 0; i < CAPTION_MAX; i++) {
		char str[128 + 1];
		sprintf(str, "%c:\n%s", (char)('A' + i), "Star");
		caption[i] = str;
	}
	captionSpace = "Space:\n" "random";
	captionEnter = "Enter:\n" "repeat";

	argArray.clear();
	stringGraphDir = DEFAULT_GRAPH_DIR;
	stringMusicDir = DEFAULT_MUSIC_DIR;
}

////////////////////////////////////////////////////////////////
// 全ての設定の保存
////////////////////////////////////////////////////////////////

void Option::saveAllConfig()
{
	saveCommonConfig();
	saveGraphConfig();
}

////////////////////////////////////////////////////////////////
// バージョン番号の保存
////////////////////////////////////////////////////////////////

void Option::saveConfigVersion(FILE *fp)
{
	printfConfig(fp, "# Program Ver.%s\n",
		STRING_VERSION_GAME);

	printfConfig(fp, "# Config Ver.%ld.%ld.%ld\n",
		OPTION_VERSION_MAJOR,
		OPTION_VERSION_MINOR,
		OPTION_VERSION_PATCH);
}

////////////////////////////////////////////////////////////////
// 共通設定の保存
////////////////////////////////////////////////////////////////

void Option::saveCommonConfig()
{
	std::string path = getCommonConfigPath();

	FILE *fp = openConfig(path.c_str(), "w");
	if (fp == NULL)
		return;

	saveCommonConfigContents(fp);

	closeConfig(fp);
}

////////////////////////////////////////////////////////////////
// 共通設定の内容の保存
////////////////////////////////////////////////////////////////

void Option::saveCommonConfigContents(FILE *fp)
{
	if (fp == NULL)
		return;

	printfConfig(fp, "# %s\n", STRING_GAME_TITLE);
	printfConfig(fp, "# Common Config file\n");

	saveConfigVersion(fp);
	printfConfig(fp, "\n");

	printfConfig(fp, "# Chosen Graphic Directly\n");
	printfConfig(fp, "%s\n", quoteString(getStringGraphDir()).c_str());
	printfConfig(fp, "\n");

	printfConfig(fp, "# Chosen Music Directly\n");
	printfConfig(fp, "%s\n", quoteString(getStringMusicDir()).c_str());
	printfConfig(fp, "\n");
}

////////////////////////////////////////////////////////////////
// グラフィック設定の保存
////////////////////////////////////////////////////////////////

void Option::saveGraphConfig()
{
}

////////////////////////////////////////////////////////////////
// グラフィック設定の内容の保存
////////////////////////////////////////////////////////////////

void Option::saveGraphConfigContents(FILE *fp)
{
	if (fp == NULL)
		return;

#if 0 //@@@
	std::string dir = opt.getStringGraphDir();
	if (dir == "")
		dir = "default/";
	// fprintf(stderr, "[texture=%s]\n", dir.c_str());
	FileList::setStrDirSelGraph(dir);
	FileList fls;
	std::string path = ".";

	// 宇宙の背景

	path = "";	
	std::string file = opt.getFile(OPTION_IDX_BG_FILE);
	if (file != "") {
		path = FileList::jointDir(fls.getBaseDir(), STR_DIR_SPACE_BG);
		path = FileList::jointDir(path, file);
	} else {
		path = "";
		fls.reset(STR_DIR_SPACE_BG, STR_GRAPH_FILE_EXT);
		for (long i = 0; i < LOOP_MAX_100; ++i) {
			std::string tmpPath = fls.next();
			if (tmpPath == "")
				break;
			if ((rand() % (i + 1)) == 0)
				path = tmpPath;
		}
		if (path == "") {
			fprintf(stderr,
				"ERROR: No files are in a directory: %s",
				STR_DIR_SPACE_BG);
			::exitGame(EXIT_FAILURE);
		}
	}
	bgTexture = ::loadTexture(path.c_str(), NULL, NULL);
	if (bgTexture == 0)
		::exitGame(EXIT_FAILURE);

	// 前景の星

	char starCStr[1 + 1] = "a";
	std::string starStr = "a";

	fgStarTexture.resize(fgStarTextureMaxNum);
	for (long i = 0; i < fgStarTextureMaxNum; ++i) {
		starCStr[0] = 'a' + i;
		starStr = fls.jointDir(STR_DIR_STAR_FG, starCStr);

		fls.reset(starStr, STR_GRAPH_FILE_EXT);
		for (long j = 0; ; ++j) {
			path = fls.next();
			if (path == "")
				break;

			fgStarTexture[i].push_back(::loadTexture(
				path.c_str(), NULL, NULL));
			if (fgStarTexture[i][j] == 0)
				break;
		}
	}

	// 背景の星

	fls.reset(STR_DIR_STAR_BG, STR_GRAPH_FILE_EXT);
	for (long j = 0; ; ++j) {
		path = fls.next();
		if (path == "")
			break;

		bgStarTexture.push_back(::loadTexture(path.c_str(),
			NULL, NULL));
		if (bgStarTexture[j] == 0)
			break;
	}
	if (bgStarTexture.empty()) {
		fprintf(stderr,
			"ERROR: No files are in a directory: %s",
			STR_DIR_STAR_BG);
		::exitGame(EXIT_FAILURE);
	}
#endif //@@@
}

////////////////////////////////////////////////////////////////
// 設定ファイルをオープン
// const char *path : 設定ファイルのパス
// const char *mode : オープン・モード
// return : ファイル・ポインタ
////////////////////////////////////////////////////////////////

FILE *Option::openConfig(const char *path, const char *mode)
{
	FILE *fp = fopen(path, mode);

	if (fp == NULL)
		perror("ERROR");

	return fp;
}

////////////////////////////////////////////////////////////////
// 設定ファイルをクローズ
// FILE *fp : 設定ファイルのファイル・ポインタ
// return : エラーならEOF
////////////////////////////////////////////////////////////////

int Option::closeConfig(FILE *fp)
{
	int res = fclose(fp);

	if (res == EOF)
		perror("ERROR");

	return res;
}

////////////////////////////////////////////////////////////////
// フォーマット文字列をセーブ
// FILE *fp : ファイル
// const char *fmt : フォーマット文字列
// ... : 引数
// return : エラーならEOF
////////////////////////////////////////////////////////////////

int Option::printfConfig(FILE *fp, const char *fmt, ...)
{
	va_list argptr;

	va_start(argptr, fmt);
	int res = vfprintf(fp, fmt, argptr);
	va_end(argptr);

	if (res == EOF)
		perror("ERROR");

	return res;
}

////////////////////////////////////////////////////////////////
// 共通設定のパスを取得
////////////////////////////////////////////////////////////////

std::string Option::getCommonConfigPath()
{
	std::string path = "";
	path = FileList::jointDir( FileList::getHomeDir(), STR_DIR_BASE );
	path = FileList::jointDir( path, COMMON_CONFIG_FILE );
	// fprintf(stderr, "[common option file path=%s]\n", path.c_str());

	return path;
}

////////////////////////////////////////////////////////////////
// グラフィック設定のパスを取得
////////////////////////////////////////////////////////////////

std::string Option::getGraphConfigPath()
{
	std::string dir = getStringGraphDir();

	std::string path = "";
	path = FileList::jointDir( FileList::getHomeDir(), STR_DIR_BASE );
	path = FileList::jointDir( path, dir );
	path = FileList::jointDir( path, GRAPH_CONFIG_FILE );
	// fprintf(stderr, "[graph option file path=%s]\n", path.c_str());

	return path;
}

////////////////////////////////////////////////////////////////
// オプションの処理
// int argc : 引数の数
// char **argv : 引数のリスト
////////////////////////////////////////////////////////////////

void Option::checkOption(int argc, char **argv)
{
#if	!defined(HAVE_GETOPT) && !defined(HAVE_GETOPT_LONG)
	long	n = 0;
	char	*optarg;
	long	optind = argc;
#endif	// !defined(HAVE_GETOPT) && !defined(HAVE_GETOPT_LONG)

	if (argv == NULL)
		return;

	while (1) {
		long	c;
#ifdef	HAVE_GETOPT_LONG
		int	optIdx;
#endif	// HAVE_GETOPT_LONG

#if	defined(HAVE_GETOPT_LONG)
		c = getopt_long(argc, argv,
			gStringOption, gLongOption, &optIdx);
#elif	defined(HAVE_GETOPT)
		c = getopt(argc, argv, gStringOption);
#else	// HAVE_GETOPT_LONG
		n++;
		if (n > argc - 1)
			break;
		optarg = argv[n + 1];

		if (argv[n][0] == '-')
			c = argv[n][1];
		else
			continue;
#endif	// HAVE_GETOPT_LONG

		if (c <= -1)
			break;

		switch (c) {
		case 'i':
			setFlag(OPTION_IDX_INIT, true);
			break;
		case 's':
			setFlag(OPTION_IDX_SAVE, checkFlag(optarg));
			break;
		case 'n':
			setNum(OPTION_IDX_STAR_NUMBER, checkNum(optarg));
			break;
		case 'f':
			setFlag(OPTION_IDX_FULL_SCREEN, checkFlag(optarg));
			break;
		case 'F':
			setNum(OPTION_IDX_FPS, checkNum(optarg));
			break;
		case 'b':
			setFile(OPTION_IDX_BG_FILE, optarg);
			break;
		case 'A':
			setNum(OPTION_IDX_INTERVAL_AUTO, checkNum(optarg));
			break;
		case 'a':
			setKey(OPTION_IDX_AUTO_KEY, checkChar(optarg));
			break;
		case 'x':
			setNum(OPTION_IDX_X_SPEED, checkNum(optarg));
			break;
		case 'y':
			setNum(OPTION_IDX_Y_SPEED, checkNum(optarg));
			break;
		case 'z':
			setNum(OPTION_IDX_Z_SPEED, checkNum(optarg));
			break;
		case 'X':
			setFlag(OPTION_IDX_REVERSE_X, checkFlag(optarg));
			break;
		case 'Y':
			setFlag(OPTION_IDX_REVERSE_Y, checkFlag(optarg));
			break;
		case 'Z':
			setFlag(OPTION_IDX_REVERSE_Z, checkFlag(optarg));
			break;
		case 'R':
			setFlag(OPTION_IDX_REVERSE_SHIFT, checkFlag(optarg));
			break;
		case 'V':
		case 'v':
			version(stdout);
			exitGame(EXIT_SUCCESS);
			break;
		case 'h':
			usage(stdout);
			exitGame(EXIT_SUCCESS);
			break;
		case '\0':
			break;
		case '?':
		case ':':
		default:
			usage(stderr);
			exitGame(EXIT_FAILURE);
			break;
		}
	}

	checkArg(argc, argv, optind);
}

////////////////////////////////////////////////////////////////
// 引数の処理
// int argc : 引数の数
// char **argv : 引数のリスト
// int optind : 引数のインデックス
////////////////////////////////////////////////////////////////

void Option::checkArg(int argc, char **argv, int optind)
{
	for (; optind < argc; optind++) {
		char *p = strchr(argv[optind], '=');
		if (p == NULL) {
			argArray.push_back(argv[optind]);
		} else {
			long len = p - argv[optind];
			if (len <= 0)
				continue;

			std::string key = argv[optind];
			key.erase(len);
			std::string value = argv[optind];
			value.erase(0, len + 1);

			checkKeyValue(key, value);
		}
	}

	long size = argArray.size();

	if (size > ARG_ARRAY_IDX_MAX) {
		usage(stderr);
		exitGame(EXIT_FAILURE);
	}

	if (size > ARG_ARRAY_IDX_GRAPH)
		stringGraphDir = argArray[ARG_ARRAY_IDX_GRAPH];
	if (size > ARG_ARRAY_IDX_MUSIC)
		stringMusicDir = argArray[ARG_ARRAY_IDX_MUSIC];

	// fprintf(stderr, "[graph=%s]\n", stringGraphDir.c_str());
	// fprintf(stderr, "[music=%s]\n", stringMusicDir.c_str());
}

////////////////////////////////////////////////////////////////
// 引数のキー値処理
// const std::string &key : キー引数
// const std::string &value : 値引数
////////////////////////////////////////////////////////////////

void Option::checkKeyValue(
	const std::string &key, const std::string &value)
{
	long len = key.length();
	if (len >= 2) {
		std::string str = key;
		str.erase(len - 1);

		char c = key[len - 1];
		int idx = tolower(c) - 'a';

		if (str == "caption-") {
			if (idx < 0)
				return;
			if (idx >= CAPTION_MAX)
				return;

			// fprintf(stderr, "[caption-%c=%s]\n",
			//	c, value.c_str());
			caption[idx] = value;
			return;
		}
	}

	if (key == "caption-space") {
		captionSpace = value;
		return;
	}
	if (key == "caption-enter") {
		captionEnter = value;
		return;
	}

	if ((key == "graph") || (key == "graphic")) {
		// fprintf(stderr, "[graph=%s]\n", value.c_str());
		stringGraphDir = value;
		return;
	}
	if (key == "music") {
		// fprintf(stderr, "[music=%s]\n", value.c_str());
		stringMusicDir = value;
		return;
	}

	usage(stderr);
	exitGame(EXIT_FAILURE);
}

////////////////////////////////////////////////////////////////
// オプションの引数を処理(フラグ)
// const char *optarg : オプションの引数
// return : フラグ
////////////////////////////////////////////////////////////////

bool Option::checkFlag(const char *optarg)
{
	std::string str = "";

	if (optarg == NULL) {
		usage(stderr);
		exitGame(EXIT_FAILURE);

		return false;
	}

	for (int i = 0; optarg[i] != '\0'; i++)
		str += toupper(optarg[i]);

	if (str == "ON")
		return true;
	if (str == "TRUE")
		return true;
	if (str == "T")
		return true;
	if (str == "YES")
		return true;
	if (str == "1")
		return true;

	if (str == "OFF")
		return false;
	if (str == "FALSE")
		return false;
	if (str == "NIL")
		return false;
	if (str == "NO")
		return false;
	if (str == "0")
		return false;

	// フラグでなかった

	usage(stderr);
	exitGame(EXIT_FAILURE);

	return false;
}

////////////////////////////////////////////////////////////////
// オプションの引数を処理(数値)
// const char *optarg : オプションの引数
// return : 数値
////////////////////////////////////////////////////////////////

double Option::checkNum(const char *optarg)
{
	double num = 0.0;

	if (optarg == NULL) {
		usage(stderr);
		exitGame(EXIT_FAILURE);

		return 0.0;
	}

	do {
		if (isdigit(optarg[0]))
			break;
		if (optarg[0] == '+')
			break;
		if (optarg[0] == '-')
			break;

		usage(stderr);
		exitGame(EXIT_FAILURE);

		return 0.0;
	} while(0);

	sscanf(optarg, "%lf", &num);

	return num;
}

////////////////////////////////////////////////////////////////
// オプションの引数を処理(文字)
// const char *optarg : オプションの引数
// return : 文字
////////////////////////////////////////////////////////////////

char Option::checkChar(const char *optarg)
{
	const char errChar = 'a';

	if (optarg == NULL) {
		usage(stderr);
		exitGame(EXIT_FAILURE);

		return errChar;
	}
	if (optarg[1] != '\0') {
		usage(stderr);
		exitGame(EXIT_FAILURE);

		return errChar;
	}

	char c = tolower(optarg[0]);
	if (c < 'a')
		return errChar;
	if (c > 'z')
		return errChar;

	return c;
}

////////////////////////////////////////////////////////////////
// フラグの配列に値を設定
// OptionIdx idx : 配列のインデックス
// bool flag : フラグ
////////////////////////////////////////////////////////////////

void Option::setFlag(OptionIdx idx, bool flag)
{
	if (idx < 0)
		return;
	if (idx >= OPTION_IDX_MAX)
		return;

	this->flag[idx] = flag;
	this->num[idx] = (double)flag;
	this->key[idx] = ' ';
	if (flag)
		this->file[idx] = "true";
	else
		this->file[idx] = "false";

	this->flagModified[idx] = true;
}

////////////////////////////////////////////////////////////////
// 数値の配列に値を設定
// OptionIdx idx : 配列のインデックス
// double num : 数値
////////////////////////////////////////////////////////////////

void Option::setNum(OptionIdx idx, double num)
{
	if (idx < 0)
		return;
	if (idx >= OPTION_IDX_MAX)
		return;

	this->flag[idx] = (bool)(long)num;
	this->num[idx] = num;
	this->key[idx] = ' ';
	std::ostringstream str;
	str << num;
	this->file[idx] = str.str();

	this->flagModified[idx] = true;
}

////////////////////////////////////////////////////////////////
// キーの配列に値を設定
// OptionIdx idx : 配列のインデックス
// int key : キー
////////////////////////////////////////////////////////////////

void Option::setKey(OptionIdx idx, int key)
{
	if (idx < 0)
		return;
	if (idx >= OPTION_IDX_MAX)
		return;

	this->flag[idx] = (bool)key;
	this->num[idx] = (double)key;
	this->key[idx] = key;
	std::ostringstream str;
	str << (char)key;
	this->file[idx] = str.str();

	this->flagModified[idx] = true;
}

////////////////////////////////////////////////////////////////
// ファイル名の配列に値を設定
// OptionIdx idx : 配列のインデックス
// std::string file : ファイル名
////////////////////////////////////////////////////////////////

void Option::setFile(OptionIdx idx, const std::string &file)
{
	if (idx < 0)
		return;
	if (idx >= OPTION_IDX_MAX)
		return;

	this->flag[idx] = true;
	this->num[idx] = 0.0;
	this->key[idx] = ' ';
	this->file[idx] = file;

	this->flagModified[idx] = true;
}

////////////////////////////////////////////////////////////////
// フラグの配列の値を取得
// OptionIdx idx : 配列のインデックス
// return : フラグ
////////////////////////////////////////////////////////////////

bool Option::getFlag(OptionIdx idx)
{
	if (idx < 0)
		return false;
	if (idx >= OPTION_IDX_MAX)
		return false;

	return this->flag[idx];
}

////////////////////////////////////////////////////////////////
// 数値の配列の値を取得
// OptionIdx idx : 配列のインデックス
// return : 数値
////////////////////////////////////////////////////////////////

double Option::getNum(OptionIdx idx)
{
	if (idx < 0)
		return 0.0;
	if (idx >= OPTION_IDX_MAX)
		return 0.0;

	return this->num[idx];
}

////////////////////////////////////////////////////////////////
// キーの配列の値を取得
// OptionIdx idx : 配列のインデックス
// return : キー
////////////////////////////////////////////////////////////////

int Option::getKey(OptionIdx idx)
{
	if (idx < 0)
		return ' ';
	if (idx >= OPTION_IDX_MAX)
		return ' ';

	return this->key[idx];
}

////////////////////////////////////////////////////////////////
// ファイル名の配列の値を取得
// OptionIdx idx : 配列のインデックス
// return : ファイル名
////////////////////////////////////////////////////////////////

const std::string &Option::getFile(OptionIdx idx)
{
	static const std::string strErr = "";

	if (idx < 0)
		return strErr;
	if (idx >= OPTION_IDX_MAX)
		return strErr;

	return this->file[idx];
}

////////////////////////////////////////////////////////////////
// キャプションの値を取得
// char key : キャプションのキー
// return : キャプションの値
////////////////////////////////////////////////////////////////

std::string Option::getCaption(char key)
{
	if (key == ' ')
		return captionSpace;
	if (key == '\n')
		return captionEnter;
	if (key == '\r')
		return captionEnter;

	int n = tolower(key) - 'a';
	if (n < 0)
		return "";
	if (n >= CAPTION_MAX)
		return "";

	return caption[n];
}

std::string Option::getStringGraphDir()
{
	return stringGraphDir;
}

std::string Option::getStringMusicDir()
{
	return stringMusicDir;
}

////////////////////////////////////////////////////////////////
// 文字列をクォーティング
// std::string str : 文字列
// return : クォーティング後の文字列
////////////////////////////////////////////////////////////////

std::string Option::quoteString(std::string str)
{
	//@@@
	return str;
}

////////////////////////////////////////////////////////////////
// 使用方の表示
// FILE *fp : 出力先ファイル
////////////////////////////////////////////////////////////////

void Option::usage(FILE *fp)
{
	fprintf(fp, gStringUsage, STRING_FILE_NAME_GAME);
	fflush(fp);
}

////////////////////////////////////////////////////////////////
// ゲームのバージョンを表示
// FILE *fp : 出力先ファイル
////////////////////////////////////////////////////////////////

void Option::version(FILE *fp)
{
	fprintf(fp, STRING_FORMAT_COPYRIGHT, LS_STRING_COPYRIGHT);
	fflush(fp);
}

