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
// ����
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

	void usage(FILE *fp);
	void version(FILE *fp);

private:
	void checkArg(int argc, char **argv, int optind);
	void checkKeyValue(const std::string &key, const std::string &value);

	bool checkFlag(const char *optarg);
	double checkNum(const char *optarg);
	char checkChar(const char *optarg);

};

}

#endif // OPTION_H
