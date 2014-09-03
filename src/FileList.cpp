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

#include "GameMain.h"
#include "FileList.h"

////////////////////////////////////////////////////////////////
// �ե����븡���Υ��󥹥ȥ饯��
////////////////////////////////////////////////////////////////

FileList::FileList()
{
	sDirBase = "";
	sDirSub = "";
	sDirFullPath = "";
	sExt = "";
	bFlagExist = false;

#ifdef D_MFC
	bFlagUseWin32ApiFind = true;
	bFlagOpen = false;
#else // D_MFC
	dpFile = NULL;
#endif // D_MFC
}

////////////////////////////////////////////////////////////////
// �ե����븡���Υǥ��ȥ饯��
////////////////////////////////////////////////////////////////

FileList::~FileList()
{
#ifndef D_MFC
	if( dpFile != NULL ){
		closedir( dpFile );
		dpFile = NULL;
	}
#endif // D_MFC
}

////////////////////////////////////////////////////////////////
// �����Υꥻ�å�
// std::string dirSub : ��फ������Хѥ�
// std::string ext : ��ĥ�ҤΥꥹ��
// return : ���顼��̵���ä�����
////////////////////////////////////////////////////////////////

bool FileList::reset( std::string dirSub, std::string ext )
{
#ifdef D_MFC
	if( bFlagOpen ){
		if( bFlagUseWin32ApiFind )
			::FindClose( hFindFile );
		else
			finder.Close();

		bFlagOpen = false;
	}
#else // D_MFC
	if( dpFile != NULL ){
		closedir( dpFile );
		dpFile = NULL;
	}
#endif // D_MFC

	sDirBase = jointDir( getBaseDir(), "" );
	sDirSub = jointDir( dirSub, "" );
	sDirFullPath = jointDir( sDirBase, sDirSub );

	DIR *tmp = opendir( sDirFullPath.c_str() );
	if (tmp == NULL)
		makeDir( sDirFullPath.c_str() );
	else
		closedir(tmp);

	sExt = ext;
	bFlagExist = false;

#ifdef D_MFC
	std::string path = jointDir( sDirFullPath, "*.*" );
	path.replaceStd::String( "/", "\\", 0 );

	if( bFlagUseWin32ApiFind ){
		ZeroMemory( &findData, sizeof( findData ) );
		hFindFile = ::FindFirstFile( path, &findData );
		if( hFindFile == INVALID_HANDLE_VALUE )
			return false;

		findData.dwFileAttributes = 0
				| FILE_ATTRIBUTE_ARCHIVE
				| FILE_ATTRIBUTE_COMPRESSED
				| FILE_ATTRIBUTE_READONLY
				;
	} else {
		if( !finder.FindFile( path ) )
			return false;
	}

	bFlagOpen = true;
#else // D_MFC
	dpFile = opendir( sDirFullPath.c_str() );
	if( dpFile == NULL )
		return false;
#endif // D_MFC

	return true;
}

////////////////////////////////////////////////////////////////
// ���Υե�����򸡺�
// ���Ĥ���ʤ��ä���ǥ��쥯�ȥ�򤵤��Τܤ�
// return : ���Ĥ��ä��ե���������Хѥ�
////////////////////////////////////////////////////////////////

std::string FileList::next()
{
	static std::string path = "";

	for( long i = 0; i < LOOP_MAX_100; i++ ){
		path = nextNode();

		if( path.length() > 0 )
			return path;
		if( bFlagExist ){
			path = "";
			return path;
		}

		// �ǥ��쥯�ȥ�򺬤˸����ä��̤�

		for( long j = 0; j < LOOP_MAX_100; j++ ){
			string::size_type pos1 = sDirSub.find( "/" );
			if( pos1 == string::npos ){
				path = "";
				return path;
			}

			string::size_type pos2 = sDirSub.rfind( "/" );
			if( pos2 == string::npos ){
				path = "";
				return path;
			}
			sDirSub = sDirSub.substr( 0, pos2 );

			pos2 = sDirSub.rfind( "/" );
			if( pos2 == string::npos ){
				path = "";
				return path;
			}
			sDirSub = sDirSub.substr( 0, pos2 );
			if( reset( sDirSub, sExt ) )
				break;
		}
	}

	// ���顼

	path = "";
	return path;
}

////////////////////////////////////////////////////////////////
// ���Υե�����򸡺�
// return : ���Ĥ��ä��ե���������Хѥ�
////////////////////////////////////////////////////////////////

std::string FileList::nextNode()
{
	static std::string name = "";
#ifdef D_MFC
	if( !bFlagOpen )
		return name;
#else // D_MFC
	if( dpFile == NULL )
		return name;
#endif // D_MFC

	while( 1 ){
#ifdef D_MFC
		if( bFlagUseWin32ApiFind ){
			if( !::FindNextFile( hFindFile, &findData ) )
				break;

			if( findData.dwFileAttributes
					& (FILE_ATTRIBUTE_DIRECTORY) ){
				continue;
			}

			name = jointDir( sDirFullPath,
					findData.cFileName );
		} else {
			if( !finder.FindNextFile() )
				break;

			if( finder.IsDirectory() )
				continue;

			name = finder.GetFilePath();
		}
		name.replaceString( "\\", "/", 0 );
#else // D_MFC
		struct dirent *buf = readdir( dpFile );
		if( buf == NULL )
			break;

		std::string fileName = buf->d_name;
# ifdef D_DOWS
		fileName.replaceString( "\\", "/", 0 );
# endif // D_DOWS
		name = jointDir( sDirFullPath, fileName );

		struct stat statBuf;
		stat( name.c_str(), &statBuf );
		if( S_ISDIR( statBuf.st_mode ) )
			continue;
#endif // D_MFC
		std::string extList = sExt;
		std::string ext = getExt( name );
		for( long i = 0; i < LOOP_MAX_100; i++ ){
			string::size_type pos = extList.find( " " );
			if( pos == string::npos ){
				pos = extList.length();
				if( pos <= 0 )
					break;
			}

			std::string curExt = extList.substr( 0, pos );
			if( curExt == ext ){
				bFlagExist = true;
				return name;
			}

			if( pos >= extList.length() )
				break;
			extList = extList.substr( pos + 1 );
		}
	}

#ifdef D_MFC
	if( bFlagUseWin32ApiFind )
		::FindClose( hFindFile );
	else
		finder.Close();

	bFlagOpen = false;
#endif // D_MFC

	name = "";
	return name;
}

////////////////////////////////////////////////////////////////
// ���ǥ��쥯�ȥ���֤�
// return : ���ǥ��쥯�ȥ�
////////////////////////////////////////////////////////////////

std::string FileList::getBaseDir()
{
	static std::string dir = "/";
	dir = jointDir( getHomeDir(), STR_DIR_BASE );
	dir = jointDir( dir, sDirBaseSelGraph );

	return dir;
}

////////////////////////////////////////////////////////////////
// ���ǥ��쥯�ȥ�����ꤹ�� (����)
// std::string dir : ���ǥ��쥯�ȥ�
// return : ���ǥ��쥯�ȥ�
////////////////////////////////////////////////////////////////

std::string FileList::setStrDirSelGraph( std::string dir )
{
	sDirBaseSelGraph = jointDir( "graph/", dir );

	return sDirBaseSelGraph;
}

////////////////////////////////////////////////////////////////
// ���ǥ��쥯�ȥ�����ꤹ�� (BGM)
// std::string dir : ���ǥ��쥯�ȥ�
// return : ���ǥ��쥯�ȥ�
////////////////////////////////////////////////////////////////

std::string FileList::setStrDirSelMusic( std::string dir )
{
	sDirBaseSelGraph = jointDir( "music/", dir );

	return sDirBaseSelGraph;
}

////////////////////////////////////////////////////////////////
// ���ǥ��쥯�ȥ�����ꤹ�� (SE)
// std::string dir : ���ǥ��쥯�ȥ�
// return : ���ǥ��쥯�ȥ�
////////////////////////////////////////////////////////////////

std::string FileList::setStrDirSelSound( std::string dir )
{
	sDirBaseSelGraph = jointDir( "sound/", dir );

	return sDirBaseSelGraph;
}

////////////////////////////////////////////////////////////////
// �ǥ��쥯�ȥ�ɤ����� "/" ����֤���Ϣ�뤹��
// std::string dir1 : �ǥ��쥯�ȥ�̾
// std::string dir2 : �ǥ��쥯�ȥ�̾
// return : ��ָ�Υǥ��쥯�ȥ�̾
////////////////////////////////////////////////////////////////

std::string FileList::jointDir(
	std::string dir1, std::string dir2 )
{
	const std::string sSlash = "/";
	static std::string dir = "/";

	int n = dir1.length() - 1;
	if( n < 0 ){
		dir = dir2;
		return( dir );
	}

	if( dir1[n] == '/' )
		dir = dir1 + dir2;
	else
		dir = dir1 + sSlash + dir2;

	return( dir );
}

////////////////////////////////////////////////////////////////
// �ѥ�̾����ե�����̾���ڤ�Ф�
// std::string path : �ѥ�̾
// return : �ե�����̾
////////////////////////////////////////////////////////////////

std::string FileList::getFileName( std::string path )
{
	string::size_type pos = path.rfind( "/" );
	static std::string name = path.substr( 0, pos );

	return name;
}

////////////////////////////////////////////////////////////////
// �ե�����̾�γ�ĥ����ʬ���֤�
// std::string name : �ե�����̾
// return : ��ĥ��
////////////////////////////////////////////////////////////////

std::string FileList::getExt( std::string name )
{
	string::size_type pos = name.rfind( "." );
	if( pos == string::npos ){
		name = "";
		return name;
	}

	name = name.substr( pos + 1 );
	return name;
}

////////////////////////////////////////////////////////////////
// �ۡ��ࡦ�ǥ��쥯�ȥ��õ��
// return : �ۡ��ࡦ�ǥ��쥯�ȥ�
////////////////////////////////////////////////////////////////

std::string FileList::getHomeDir()
{
	static std::string homeRes = "";
	const char *home;

	home = getenv( STR_ENV_HOME );

	/* ̵����Х����ȥǥ��쥯�ȥ� */
	if( (home == NULL) || (home[0] == '\0') )
		home = STR_DIR_NAME_CUR;

	homeRes = home;
	return homeRes;
}

////////////////////////////////////////////////////////////////
// �ǥ��쥯�ȥ�򷡤�
// char *s : �ǥ��쥯�ȥ�̾
// return : ���顼��̵���ä���?
////////////////////////////////////////////////////////////////

bool FileList::makeDir(const char *path)
{
	char	cur_path[FILE_NAME_MAX_LEN + 1];
	char	*p;

	if( path == NULL )
		return false;

	strncpy( cur_path, path, FILE_NAME_MAX_LEN );

	p = cur_path;
	while( 1 ){
		p = strchr( p + 1, '/' );
		if( p == NULL )
			break;
		if( *p == '\0' )
			break;

		*p = '\0';
		makeDirNode( cur_path );
		*p = '/';
	}
	makeDirNode( cur_path );

	return true;
}

////////////////////////////////////////////////////////////////
// �ǥ��쥯�ȥ�� 1 �ķ���
// char *s : �ǥ��쥯�ȥ�̾
// return : ���顼��̵���ä���?
////////////////////////////////////////////////////////////////

bool FileList::makeDirNode(const char *path)
{
	if( path == NULL )
		return false;
#if	defined( D_DOS )
	if( _mkdir( path ) )
		return true;
	else
		return false;
#elif	defined( HAVE_MKDIR )
	if( mkdir( path, S_IFDIR | 0755 ) == 0 )
		return true;
	else
		return false;
#else
	if( mknod( path, S_IFDIR | 0755, 0 ) == 0 )
		return true;
	else
		return false;
#endif
	return false;
}

