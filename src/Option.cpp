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

	OptionTypeArray[OPTION_IDX_INIT] = OPTION_TYPE_FLAG;
	OptionTypeArray[OPTION_IDX_SAVE] = OPTION_TYPE_FLAG;
	OptionTypeArray[OPTION_IDX_STAR_NUMBER] = OPTION_TYPE_NUM;
	OptionTypeArray[OPTION_IDX_FULL_SCREEN] = OPTION_TYPE_FLAG;
	OptionTypeArray[OPTION_IDX_FPS] = OPTION_TYPE_NUM;
	OptionTypeArray[OPTION_IDX_BG_FILE] = OPTION_TYPE_FILE;
	OptionTypeArray[OPTION_IDX_INTERVAL_AUTO] = OPTION_TYPE_NUM;
	OptionTypeArray[OPTION_IDX_AUTO_KEY] = OPTION_TYPE_KEY;
	OptionTypeArray[OPTION_IDX_X_SPEED] = OPTION_TYPE_NUM;
	OptionTypeArray[OPTION_IDX_Y_SPEED] = OPTION_TYPE_NUM;
	OptionTypeArray[OPTION_IDX_Z_SPEED] = OPTION_TYPE_NUM;
	OptionTypeArray[OPTION_IDX_REVERSE_X] = OPTION_TYPE_FLAG;
	OptionTypeArray[OPTION_IDX_REVERSE_Y] = OPTION_TYPE_FLAG;
	OptionTypeArray[OPTION_IDX_REVERSE_Z] = OPTION_TYPE_FLAG;
	OptionTypeArray[OPTION_IDX_REVERSE_SHIFT] = OPTION_TYPE_FLAG;

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

	for (char c = 'a'; c <= 'z'; c++) {
		char str[128 + 1];
		sprintf(str, "%c:\n%s", c, "Star");
		setCaption(c, str);
	}
	setCaption(' ', "Space:\n" "Random");
	setCaption('\n', "Enter:\n" "Repeat");

	setGraphDir(DEFAULT_GRAPH_DIR);
	setMusicDir(DEFAULT_MUSIC_DIR);

	// 設定が変更されなかった事にする

	for (int i = 0; i < OPTION_IDX_MAX; i++)
		flagModifiedOption[i] = false;
	for (int i = 0; i < CAPTION_MAX; i++)
		flagModifiedCaption[i] = false;
	flagModifiedCaptionSpace = false;
	flagModifiedCaptionEnter = false;
	flagModifiedGraphDir = false;
	flagModifiedMusicDir = false;

	argArray.clear();
}

////////////////////////////////////////////////////////////////
// 全ての設定を読み込み
// Option *opt : 共通設定の参照元
////////////////////////////////////////////////////////////////

void Option::loadAllConfig(Option *opt)
{
	loadCommonConfig();
	mergeCommonConfig(opt);

	loadGraphConfig();
	mergeGraphConfig(opt);
}

////////////////////////////////////////////////////////////////
// 共通設定の読み込み
////////////////////////////////////////////////////////////////

void Option::loadCommonConfig()
{
}

////////////////////////////////////////////////////////////////
// グラフィック設定の読み込み
////////////////////////////////////////////////////////////////

void Option::loadGraphConfig()
{
}

////////////////////////////////////////////////////////////////
// 共通設定をマージ
// Option *opt : マージ元の設定
////////////////////////////////////////////////////////////////

void Option::mergeCommonConfig(Option *opt)
{
	if (opt == NULL)
		return;

	if (opt->flagModifiedGraphDir)
		setGraphDir(opt->getGraphDir());
	if (opt->flagModifiedMusicDir)
		setMusicDir(opt->getMusicDir());
}

////////////////////////////////////////////////////////////////
// グラフィック設定をマージ
// Option *opt : マージ元の設定
////////////////////////////////////////////////////////////////

void Option::mergeGraphConfig(Option *opt)
{
	if (opt == NULL)
		return;

	for (int i = OPTION_IDX_BEGIN; i <= OPTION_IDX_END; i++)
		mergeGraphConfigOption(opt, (OptionIdx)i);

	for (char c = 'a'; c <= 'z'; c++)
		mergeGraphConfigCaption(opt, c);

	char c;
	c = ' ';
	mergeGraphConfigCaption(opt, c);

	c = '\n';
	mergeGraphConfigCaption(opt, c);
}

////////////////////////////////////////////////////////////////
// グラフィック設定のオプションをマージ
// Option *opt : マージ元の設定
// OptionIdx idx : オプション
////////////////////////////////////////////////////////////////

void Option::mergeGraphConfigOption(Option *opt, OptionIdx idx)
{
	if (opt == NULL)
		return;

	if (!opt->flagModifiedOption[idx])
		return;

	switch (OptionTypeArray[idx]) {
	case OPTION_TYPE_FLAG:
		setFlag(idx, opt->getFlag(idx));
		break;
	case OPTION_TYPE_NUM:
		setNum(idx, opt->getNum(idx));
		break;
	case OPTION_TYPE_KEY:
		setKey(idx, opt->getKey(idx));
		break;
	case OPTION_TYPE_FILE:
		setFile(idx, opt->getFile(idx));
		break;
	case OPTION_TYPE_MAX:
		break;
	}
}

////////////////////////////////////////////////////////////////
// グラフィック設定のキャプションをマージ
// Option *opt : マージ元の設定
// char key : キャプションのキー
////////////////////////////////////////////////////////////////

void Option::mergeGraphConfigCaption(Option *opt, char key)
{
	if (opt == NULL)
		return;

	if (opt->getFlagModifiedCaption(key))
		setCaption(key, opt->getCaption(key));
}

////////////////////////////////////////////////////////////////
// 全ての設定を保存
////////////////////////////////////////////////////////////////

void Option::saveAllConfig()
{
	if (!getFlag(OPTION_IDX_SAVE))
		return;

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
	printfConfig(fp, "%s\n", quoteString(getGraphDir()).c_str());
	printfConfig(fp, "\n");

	printfConfig(fp, "# Chosen Music Directly\n");
	printfConfig(fp, "%s\n", quoteString(getMusicDir()).c_str());
	printfConfig(fp, "\n");
}

////////////////////////////////////////////////////////////////
// グラフィック設定の保存
////////////////////////////////////////////////////////////////

void Option::saveGraphConfig()
{
	std::string path = getGraphConfigPath();

	FILE *fp = openConfig(path.c_str(), "w");
	if (fp == NULL)
		return;

	saveGraphConfigContents(fp);

	closeConfig(fp);
}

////////////////////////////////////////////////////////////////
// グラフィック設定の内容の保存
////////////////////////////////////////////////////////////////

void Option::saveGraphConfigContents(FILE *fp)
{
	if (fp == NULL)
		return;

	printfConfig(fp, "# %s\n", STRING_GAME_TITLE);
	printfConfig(fp, "# Graphic Config file\n");

	saveConfigVersion(fp);
	printfConfig(fp, "\n");

	printfConfig(fp, "# number of star\n");
	printfConfig(fp, "--star-number %ld\n",
		(long)getNum(OPTION_IDX_STAR_NUMBER));
	printfConfig(fp, "\n");

	printfConfig(fp, "# flag of full screen\n");
	printfConfig(fp, "--full-screen %s\n",
		quoteString(getFlagString(OPTION_IDX_FULL_SCREEN)).c_str());
	printfConfig(fp, "\n");

	printfConfig(fp, "# frame per second\n");
	printfConfig(fp, "--fps %ld\n",
		(long)getNum(OPTION_IDX_FPS));
	printfConfig(fp, "\n");

	printfConfig(fp, "# file of back ground\n");
	printfConfig(fp, "--bg-file %s\n",
		quoteString(getFile(OPTION_IDX_BG_FILE)).c_str());
	printfConfig(fp, "\n");

	printfConfig(fp, "# frame of interval of auto inputed key\n");
	printfConfig(fp, "--interval-auto %ld\n",
		(long)getNum(OPTION_IDX_INTERVAL_AUTO));
	printfConfig(fp, "\n");

	printfConfig(fp, "# auto inputed key\n");
	printfConfig(fp, "--auto-key %s\n",
		quoteString(getKeyString(OPTION_IDX_AUTO_KEY)).c_str());
	printfConfig(fp, "\n");

	printfConfig(fp, "# speed of the x-axis direction\n");
	printfConfig(fp, "--x-speed %lf\n",
		getNum(OPTION_IDX_X_SPEED));
	printfConfig(fp, "\n");

	printfConfig(fp, "# speed of the y-axis direction\n");
	printfConfig(fp, "--y-speed %lf\n",
		getNum(OPTION_IDX_Y_SPEED));
	printfConfig(fp, "\n");

	printfConfig(fp, "# speed of the z-axis direction\n");
	printfConfig(fp, "--z-speed %lf\n",
		getNum(OPTION_IDX_Z_SPEED));
	printfConfig(fp, "\n");

	printfConfig(fp, "# flag of reversing x-axis\n");
	printfConfig(fp, "--reverse-x %s\n",
		quoteString(getFlagString(OPTION_IDX_REVERSE_X)).c_str());
	printfConfig(fp, "\n");

	printfConfig(fp, "# flag of reversing y-axis\n");
	printfConfig(fp, "--reverse-y %s\n",
		quoteString(getFlagString(OPTION_IDX_REVERSE_Y)).c_str());
	printfConfig(fp, "\n");

	printfConfig(fp, "# flag of reversing z-axis\n");
	printfConfig(fp, "--reverse-z %s\n",
		quoteString(getFlagString(OPTION_IDX_REVERSE_Z)).c_str());
	printfConfig(fp, "\n");

	printfConfig(fp, "# flag of reversing shift key\n");
	printfConfig(fp, "--reverse-shift %s\n",
		quoteString(getFlagString(OPTION_IDX_REVERSE_SHIFT)).c_str());
	printfConfig(fp, "\n");

	printfConfig(fp, "# captions of key\n");
	for (int i = 0; i < CAPTION_MAX; i++) {
		char c = (char)('a' + i);
		printfConfig(fp, "caption-%c=%s\n",
			c, quoteString(getCaption(c)).c_str());
	}
	printfConfig(fp, "\n");

	printfConfig(fp, "caption-space=%s\n",
		quoteString(getCaption(' ')).c_str());
	printfConfig(fp, "caption-enter=%s\n",
		quoteString(getCaption('\n')).c_str());
	printfConfig(fp, "\n");
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
	std::string dir = getGraphDir();

	std::string path = "";
	path = FileList::jointDir( FileList::getHomeDir(),
		STR_DIR_BASE_GRAPH );
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

void Option::parseOption(int argc, char **argv)
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
			setFlag(OPTION_IDX_SAVE, parseFlag(optarg));
			break;
		case 'n':
			setNum(OPTION_IDX_STAR_NUMBER, parseNum(optarg));
			break;
		case 'f':
			setFlag(OPTION_IDX_FULL_SCREEN, parseFlag(optarg));
			break;
		case 'F':
			setNum(OPTION_IDX_FPS, parseNum(optarg));
			break;
		case 'b':
			setFile(OPTION_IDX_BG_FILE, optarg);
			break;
		case 'A':
			setNum(OPTION_IDX_INTERVAL_AUTO, parseNum(optarg));
			break;
		case 'a':
			setKey(OPTION_IDX_AUTO_KEY, parseChar(optarg));
			break;
		case 'x':
			setNum(OPTION_IDX_X_SPEED, parseNum(optarg));
			break;
		case 'y':
			setNum(OPTION_IDX_Y_SPEED, parseNum(optarg));
			break;
		case 'z':
			setNum(OPTION_IDX_Z_SPEED, parseNum(optarg));
			break;
		case 'X':
			setFlag(OPTION_IDX_REVERSE_X, parseFlag(optarg));
			break;
		case 'Y':
			setFlag(OPTION_IDX_REVERSE_Y, parseFlag(optarg));
			break;
		case 'Z':
			setFlag(OPTION_IDX_REVERSE_Z, parseFlag(optarg));
			break;
		case 'R':
			setFlag(OPTION_IDX_REVERSE_SHIFT, parseFlag(optarg));
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

	parseArg(argc, argv, optind);
}

////////////////////////////////////////////////////////////////
// 引数の処理
// int argc : 引数の数
// char **argv : 引数のリスト
// int optind : 引数のインデックス
////////////////////////////////////////////////////////////////

void Option::parseArg(int argc, char **argv, int optind)
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

			parseKeyValue(key, value);
		}
	}

	long size = argArray.size();

	if (size > ARG_ARRAY_IDX_MAX) {
		usage(stderr);
		exitGame(EXIT_FAILURE);
	}

	if (size > ARG_ARRAY_IDX_GRAPH)
		setGraphDir(argArray[ARG_ARRAY_IDX_GRAPH]);
	if (size > ARG_ARRAY_IDX_MUSIC)
		setMusicDir(argArray[ARG_ARRAY_IDX_MUSIC]);

	// fprintf(stderr, "[graph=%s]\n", getGraphDir().c_str());
	// fprintf(stderr, "[music=%s]\n", getMusicDir().c_str());
}

////////////////////////////////////////////////////////////////
// 引数のキー値処理
// const std::string &key : キー引数
// const std::string &value : 値引数
////////////////////////////////////////////////////////////////

void Option::parseKeyValue(
	const std::string &key, const std::string &value)
{
	long len = key.length();
	if (len >= 2) {
		std::string str = key;
		str.erase(len - 1);

		char c = key[len - 1];

		if (str == "caption-") {
			if (c < 'a')
				return;
			if (c > 'z')
				return;

			// fprintf(stderr, "[caption-%c=%s]\n",
			//	c, value.c_str());
			setCaption(c, value);
			return;
		}
	}

	if (key == "caption-space") {
		setCaption(' ', value);
		return;
	}
	if (key == "caption-enter") {
		setCaption('\n', value);
		return;
	}

	if ((key == "graph") || (key == "graphic")) {
		// fprintf(stderr, "[graph=%s]\n", value.c_str());
		setGraphDir(value);
		return;
	}
	if (key == "music") {
		// fprintf(stderr, "[music=%s]\n", value.c_str());
		setMusicDir(value);
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

bool Option::parseFlag(const char *optarg)
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

double Option::parseNum(const char *optarg)
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

char Option::parseChar(const char *optarg)
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

	this->flagModifiedOption[idx] = true;
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

	this->flagModifiedOption[idx] = true;
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

	this->flagModifiedOption[idx] = true;
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

	this->flagModifiedOption[idx] = true;
}

////////////////////////////////////////////////////////////////
// キャプションの値を設定
// char key : キャプションのキー
// const std::string &str : キャプションの値
////////////////////////////////////////////////////////////////

void Option::setCaption(char key, const std::string &str)
{
	if (key == ' ') {
		captionSpace = str;
		flagModifiedCaptionSpace = true;
		return;
	}
	if ((key == '\n') || (key == '\r')) {
		captionEnter = str;
		flagModifiedCaptionEnter = true;
		return;
	}

	int n = tolower(key) - 'a';
	if (n < 0)
		return;
	if (n >= CAPTION_MAX)
		return;

	caption[n] = str;
	flagModifiedCaption[n] = true;
}

////////////////////////////////////////////////////////////////
// グラフィック・ディレクトリを設定
// const std::string &dir : ディレクトリ
////////////////////////////////////////////////////////////////

void Option::setGraphDir(const std::string &dir)
{
	graphDir = dir;
	flagModifiedGraphDir = true;
}

////////////////////////////////////////////////////////////////
// BGM・ディレクトリを設定
// const std::string &dir : ディレクトリ
////////////////////////////////////////////////////////////////

void Option::setMusicDir(const std::string &dir)
{
	musicDir = dir;
	flagModifiedMusicDir = true;
}

////////////////////////////////////////////////////////////////
// フラグの配列の値を文字列に変換して取得
// OptionIdx idx : 配列のインデックス
// return : フラグ
////////////////////////////////////////////////////////////////

std::string Option::getFlagString(OptionIdx idx)
{
	if (idx < 0)
		return "false";
	if (idx >= OPTION_IDX_MAX)
		return "false";

	if (this->flag[idx])
		return "true";
	else
		return "false";

	return "false";
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
// キーの配列の値を文字列に変換して取得
// OptionIdx idx : 配列のインデックス
// return : キー
////////////////////////////////////////////////////////////////

std::string Option::getKeyString(OptionIdx idx)
{
	if (idx < 0)
		return " ";
	if (idx >= OPTION_IDX_MAX)
		return " ";

	return convertKeyToString(this->key[idx]);
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

std::string Option::getFile(OptionIdx idx)
{
	if (idx < 0)
		return "";
	if (idx >= OPTION_IDX_MAX)
		return "";

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
	if ((key == '\n') || (key == '\r'))
		return captionEnter;

	int n = tolower(key) - 'a';
	if (n < 0)
		return "";
	if (n >= CAPTION_MAX)
		return "";

	return caption[n];
}

////////////////////////////////////////////////////////////////
// グラフィック・ディレクトリのパスを返す
// return : パス
////////////////////////////////////////////////////////////////

std::string Option::getGraphDir()
{
	return graphDir;
}

////////////////////////////////////////////////////////////////
// BGM・ディレクトリのパスを返す
// return : パス
////////////////////////////////////////////////////////////////

std::string Option::getMusicDir()
{
	return musicDir;
}

////////////////////////////////////////////////////////////////
// キャプションの修整フラグを取得
// char key : キャプションのキー
// return : キャプションの修整フラグ
////////////////////////////////////////////////////////////////

bool Option::getFlagModifiedCaption(char key)
{
	if (key == ' ')
		return flagModifiedCaptionSpace;
	if ((key == '\n') || (key == '\r'))
		return flagModifiedCaptionEnter;

	int n = tolower(key) - 'a';
	if (n < 0)
		return false;
	if (n >= CAPTION_MAX)
		return false;

	return flagModifiedCaption[n];
}

////////////////////////////////////////////////////////////////
// キーを文字列に変換
// int key : キー
// return : 変換後の文字列
////////////////////////////////////////////////////////////////

std::string Option::convertKeyToString(int key)
{
	char buf[31 + 1] = " ";
	sprintf(buf, "%c", key);

	return buf;
}

////////////////////////////////////////////////////////////////
// 文字列をクォーティング
// std::string str : 文字列
// return : クォーティング後の文字列
////////////////////////////////////////////////////////////////

std::string Option::quoteString(std::string str)
{
	std::string s = "";
	s = "\"";
	s += escapeString(str, false);
	s += "\"";

	return s;
}

////////////////////////////////////////////////////////////////
// 文字列内の特殊文字をエスケープ
// std::string str : 文字列
// return : エスケープ後の文字列
////////////////////////////////////////////////////////////////

std::string Option::escapeString(std::string str, bool flagEscapeSpace)
{
	std::string s = "";
	for (string::iterator it = str.begin(); it != str.end(); ++it) {
		int c = *it;
		switch (c) {
		case '\\':
			s += '\\';
			s += '\\';
			break;
		case '\"':
			s += '\\';
			s += '\"';
			break;
		case '\'':
			s += '\\';
			s += '\'';
			break;
		case ' ':
			if (flagEscapeSpace)
				s += '\\';
			s += ' ';
			break;
		case '\n':
			s += '\\';
			s += 'n';
			break;
		default:
			s += c;
			break;
		}
	}

	return s;
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

