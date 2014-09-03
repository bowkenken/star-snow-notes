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

#ifndef FILE_LIST_H
#define FILE_LIST_H	1

#include "GameMain.h"

#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif // HAVE_STDLIB_H

#ifdef HAVE_STDIO_H
# include <stdio.h>
#endif // HAVE_STDIO_H

#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif // HAVE_UNISTD_H

#ifdef HAVE_DIRENT_H
# include <dirent.h>
#endif // HAVE_DIRENT_H

#ifdef HAVE_SYS_STAT_H
# include <sys/stat.h>
#endif // HAVE_SYS_STAT_H

using namespace std;

////////////////////////////////////////////////////////////////
// ���ꤵ�줿��ĥ�ҤΥե�����򸡺�����
// ���Ĥ���ʤ��ä��顢�ǥ��쥯�ȥ곬�ؤ���ǥ��쥯�ȥ�
// �ޤǡ������Τܤ�
////////////////////////////////////////////////////////////////

#define STR_ENV_HOME	"HOME"
#define STR_SLASH	"/"
#define STR_DIR_NAME_CUR	"."
#define STR_DIR_NAME_GAME	".star-snow-notes"
#define STR_DIR_BASE	".star-snow-notes/"
#define STR_DIR_BASE_GRAPH	STR_DIR_BASE "graph/"
#define FILE_NAME_MAX_LEN	4096

static std::string sDirBaseSelGraph = "default/";

class FileList {
public:

private:
	// ���ǥ��쥯�ȥ�
	std::string sDirBase;
	// ���ǥ��쥯�ȥ꤫������Хѥ�
	std::string sDirSub;
	// �ե�ѥ�
	std::string sDirFullPath;

	// ��ĥ�ҤΥꥹ�ȡ����ڡ����Ƕ��ڤ�
	std::string sExt;

	// ���ĤǤ�ե����뤬���Ĥ��ä�����
	// ���Ĥ��ä���ǥ��쥯�ȥ�Ϥ����Τܤ�ʤ�
	bool bFlagExist;

#ifdef D_MFC
	// �ե����롦������

	bool bFlagUseWin32ApiFind;

	WIN32_FIND_DATA findData;
	HANDLE hFindFile;

	CFileFind finder;

	bool bFlagOpen;
#else // D_MFC
	// ��������Υǥ��쥯�ȥ�
	DIR *dpFile;
#endif // D_MFC

public:
	FileList();
	~FileList();

	bool reset( std::string dir, std::string ext );
	std::string next();

	std::string getBaseDir();

	static std::string setStrDirSelGraph( std::string dir );
	static std::string setStrDirSelMusic( std::string dir );
	static std::string setStrDirSelSound( std::string dir );

	static std::string jointDir(
		std::string dir1, std::string dir2 );
	static std::string getFileName( std::string path );
	static std::string getExt( std::string name );
	static std::string getHomeDir();

	static bool makeDir(const char *path);
	static bool makeDirNode(const char *path);

private:
	std::string nextNode();
};

#endif /* FILE_LIST_H */
