#include "WONFile.h"
#include <sys/stat.h>

#ifdef WIN32
#include <direct.h>
#include <io.h>
#endif

using namespace WONAPI;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
const char WONFile::PATH_SEP = '/';

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
WONFile::WONFile()
{
	SetFilePath("");
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
WONFile::WONFile(const std::string &thePath)
{
	SetFilePath(thePath);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void WONFile::Reset()
{
	mFilePath.erase();
	mFileDirectory.erase();
	mFileName.erase();
	mFileExtension.erase();

	mGotFileDirectory = false;
	mGotFileName = false;
	mGotFileExtension = false;
	mGotFileInfo = false;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void WONFile::SetFilePath(const std::string &thePath)
{
	Reset();
	mFilePath = thePath;
	for(int i=0; i<mFilePath.length(); i++)
	{
		if(mFilePath[i]=='\\')
			mFilePath[i] = '/';
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool WONFile::Remove(bool evenIfReadOnly)
{
	if(evenIfReadOnly)
	{
		if(!mGotFileInfo)
			CalcFileInfo();

		if(mReadOnly)
			SetReadOnly(false);
	}

	return remove(mFilePath.c_str())==0;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool WONFile::SetReadOnly(bool readOnly)
{
	return chmod(mFilePath.c_str(),readOnly?S_IREAD:S_IREAD|S_IWRITE)==0;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool WONFile::SetWorkingDirectory()
{
	GetFileDirectory();
	if(mFileDirectory.empty())
		return false;
	else
		return chdir(mFileDirectory.c_str())==0;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
std::string WONFile::GetRelativePath(const WONFile &theSubFile)
{
	const std::string &aPath1 = GetFileDirectory();
	const std::string &aPath2 = theSubFile.GetFilePath();
	if(aPath2.length() > aPath1.length())
	{
		int i;
		for(i=0; i<aPath1.length(); i++)
		{
			unsigned char char1 = aPath1[i];
			unsigned char char2 = aPath2[i];

#ifdef WIN32
			char1 = toupper(char1);
			char2 = toupper(char2);
#endif
			if(char1!=char2)
				break;
		}

		if(i==aPath1.length() && aPath2[i]=='/') // paths match
			return aPath2.substr(i+1);
	}

	// no match --> return absolute path
	return aPath2;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
const std::string& WONFile::GetFilePath() const
{
	return mFilePath;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void WONFile::CalcFileInfo()
{
	mGotFileInfo = true;
	struct stat fileInfo;
	if(stat(mFilePath.c_str(), &fileInfo)==0)
	{
		mExists = true;
		int aMode = 0;
#ifdef WIN32
		aMode = 2;
#else
		aMode = 0200;
#endif

		mReadOnly = (fileInfo.st_mode & aMode)?false:true;
		mSize = fileInfo.st_size;
		mModifyTime = fileInfo.st_mtime;

#if defined(macintosh) && (macintosh == 1)
	mModifyTime += 126230400; // # of seconds Mac time_t differs in stat and utime
#endif // mac
	}
	else
	{
		mSize = 0;
		mModifyTime = 0;
		mExists = false;
		mReadOnly = false;
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void WONFile::CalcFileDirectory()
{
	int aSlashPos = (int)(mFilePath.find_last_of('/'));
	if(aSlashPos!=string::npos)
		mFileDirectory = mFilePath.substr(0,aSlashPos);

	mFileName = mFilePath.substr(aSlashPos+1);
	mGotFileDirectory = true;
	mGotFileName = true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void WONFile::CalcFileExtension()
{
	GetFileName();

	int aDotPos = (int)(mFileName.find_last_of('.'));
	if(aDotPos!=string::npos)
	{
		mFileExtension = mFileName.substr(aDotPos+1);
		mFileNameWithoutExtension = mFileName.substr(0,aDotPos);
	}
	else
		mFileNameWithoutExtension  = mFileName;

	mGotFileExtension = true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
const std::string& WONFile::GetFileDirectory() const
{
	if(mGotFileDirectory)
		return mFileDirectory;

	(const_cast<WONFile*>(this))->CalcFileDirectory();
	return mFileDirectory;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
const std::string& WONFile::GetFileName() const
{
	if(!mGotFileName)
		GetFileDirectory();

	return mFileName;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
const std::string& WONFile::GetFileExtension() const
{
	if(!mGotFileExtension)
		(const_cast<WONFile*>(this))->CalcFileExtension();

	return mFileExtension;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
const std::string& WONFile::GetFileNameWithoutExtension() const
{
	GetFileExtension();
	return mFileNameWithoutExtension;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
int WONFile::GetSize() const
{
	if(!mGotFileInfo)
		(const_cast<WONFile*>(this))->CalcFileInfo();

	return mSize;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
time_t WONFile::GetModifyTime() const
{
	if(!mGotFileInfo)
		(const_cast<WONFile*>(this))->CalcFileInfo();

	return mModifyTime;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool WONFile::Exists() const
{
	if(!mGotFileInfo)
		(const_cast<WONFile*>(this))->CalcFileInfo();

	return mExists;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool WONFile::IsReadOnly() const
{
	if(!mGotFileInfo)
		(const_cast<WONFile*>(this))->CalcFileInfo();

	return mReadOnly;
}
