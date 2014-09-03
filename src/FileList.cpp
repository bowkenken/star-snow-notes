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

#include "GameMain.h"
#include "FileList.h"

////////////////////////////////////////////////////////////////
// ファイル検索のコンストラクタ
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
// ファイル検索のデストラクタ
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
// 検索のリセット
// std::string dirSub : 基準からの相対パス
// std::string ext : 拡張子のリスト
// return : エラーが無かったか？
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
// 次のファイルを検索
// 見つからなかったらディレクトリをさかのぼる
// return : 見つかったファイルの絶対パス
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

		// ディレクトリを根に向かって遡る

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

	// エラー

	path = "";
	return path;
}

////////////////////////////////////////////////////////////////
// 次のファイルを検索
// return : 見つかったファイルの絶対パス
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
// 基準ディレクトリを返す
// return : 基準ディレクトリ
////////////////////////////////////////////////////////////////

std::string FileList::getBaseDir()
{
	static std::string dir = "/";
	dir = jointDir( getHomeDir(), STR_DIR_BASE );
	dir = jointDir( dir, sDirBaseSelGraph );

	return dir;
}

////////////////////////////////////////////////////////////////
// 基準ディレクトリを設定する (画像)
// std::string dir : 基準ディレクトリ
// return : 基準ディレクトリ
////////////////////////////////////////////////////////////////

std::string FileList::setStrDirSelGraph( std::string dir )
{
	sDirBaseSelGraph = jointDir( "graph/", dir );

	return sDirBaseSelGraph;
}

////////////////////////////////////////////////////////////////
// 基準ディレクトリを設定する (BGM)
// std::string dir : 基準ディレクトリ
// return : 基準ディレクトリ
////////////////////////////////////////////////////////////////

std::string FileList::setStrDirSelMusic( std::string dir )
{
	sDirBaseSelGraph = jointDir( "music/", dir );

	return sDirBaseSelGraph;
}

////////////////////////////////////////////////////////////////
// 基準ディレクトリを設定する (SE)
// std::string dir : 基準ディレクトリ
// return : 基準ディレクトリ
////////////////////////////////////////////////////////////////

std::string FileList::setStrDirSelSound( std::string dir )
{
	sDirBaseSelGraph = jointDir( "sound/", dir );

	return sDirBaseSelGraph;
}

////////////////////////////////////////////////////////////////
// ディレクトリどうしを "/" を補間して連結する
// std::string dir1 : ディレクトリ名
// std::string dir2 : ディレクトリ名
// return : 補間後のディレクトリ名
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
// パス名からファイル名を切り出す
// std::string path : パス名
// return : ファイル名
////////////////////////////////////////////////////////////////

std::string FileList::getFileName( std::string path )
{
	string::size_type pos = path.rfind( "/" );
	static std::string name = path.substr( 0, pos );

	return name;
}

////////////////////////////////////////////////////////////////
// ファイル名の拡張子部分を返す
// std::string name : ファイル名
// return : 拡張子
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
// ホーム・ディレクトリを探す
// return : ホーム・ディレクトリ
////////////////////////////////////////////////////////////////

std::string FileList::getHomeDir()
{
	static std::string homeRes = "";
	const char *home;

	home = getenv( STR_ENV_HOME );

	/* 無ければカレントディレクトリ */
	if( (home == NULL) || (home[0] == '\0') )
		home = STR_DIR_NAME_CUR;

	homeRes = home;
	return homeRes;
}

////////////////////////////////////////////////////////////////
// ディレクトリを掘る
// char *s : ディレクトリ名
// return : エラーが無かったか?
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
// ディレクトリを 1 つ掘る
// char *s : ディレクトリ名
// return : エラーが無かったか?
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

