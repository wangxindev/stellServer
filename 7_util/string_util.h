#pragma once
#ifndef __STRING_UTIL_H__
#define __STRING_UTIL_H__
#include <string>
#include <vector>
#ifdef WIN32
#include <io.h>
#include"windows.h"
#else
#endif


namespace string_util {
	/**
 @fn	std::string& trim(std::string &s, char c)

 @brief	去除s中所有字符c

 @author	wangxin
 @date	2017/4/11

 @param [in,out]	s	备操作的字符串
 @param 			c	在字符串中去掉的字符

 @return	返回去除字符的字符串引用，与传进来的string为同一个对象
 */
	std::string& trim(std::string &s, char c)
	{
		int index = 0;
		if (!s.empty())
		{
			while ((index = s.find(c, index)) != std::string::npos)
			{
				s.erase(index, 1);
			}
		}
		return s;
	}

	/**
	 @fn	std::vector<std::string> split(std::string str, std::string pattern)

	 @brief	字符串分割函数.

	 @author	wangxin
	 @date	2017/4/11

	 @param	str	   	要被分割的字符串.
	 @param	pattern	分割用的字符串.

	 @return	返回一个分割后的字符串数组;
	 */

	std::vector<std::string> split(std::string str, std::string pattern)
	{
		std::string::size_type pos;
		std::vector<std::string> result;
		str += pattern;//扩展字符串以方便操作
		int size = str.size();

		for (int i = 0; i < size; i++)
		{
			pos = str.find(pattern, i);
			if (pos < size)
			{
				std::string s = str.substr(i, pos - i);
				result.push_back(s);
				i = pos + pattern.size() - 1;
			}
		}
		return result;
	}

#ifdef WIN32

	/**
	 @fn	void getFiles(std::string _path, std::string _exd, std::vector<std::string>& _files)
	
	 @brief	获得一个目录下的所有文件名
	
	 @author	wangxin
	 @date	2017/4/11
	
	 @param 			_path 	路径
	 @param 			_exd  	文件类型，为空则表示全部类型.
	 @param [in,out]	_files	返回的文件名集合，文件名包含路径.
	 */

	void getFiles(std::string _path, std::string _exd, std::vector<std::string>& _files)
	{
		//文件句柄
		long   hFile = 0;
		//文件信息
		struct _finddata_t fileinfo;
		std::string pathName, exdName;

		if (0 != strcmp(_exd.c_str(), ""))
		{
			exdName = "\\*." + _exd;
		}
		else
		{
			exdName = "\\*";
		}

		if ((hFile = _findfirst(pathName.assign(_path).append(exdName).c_str(), &fileinfo)) != -1)
		{
			do
			{
				//如果是文件夹中仍有文件夹,迭代之
				//如果不是,加入列表
				if ((fileinfo.attrib &  _A_SUBDIR))
				{
					if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
						getFiles(pathName.assign(_path).append("\\").append(fileinfo.name), _exd, _files);
				}
				else
				{
					if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
						_files.push_back(pathName.assign(_path).append("\\").append(fileinfo.name));
				}
			} while (_findnext(hFile, &fileinfo) == 0);
			_findclose(hFile);
		}
	}

	/**
	 @fn	bool IsDirectory(const char * pDir)
	
	 @brief	判断一个路径是否为目录
	
	 @author	wangxin
	 @date	2017/4/11
	
	 @param	pDir	要判断的路径
	
	 @return	返回是不是路径
	 */

	bool IsDirectory(const char * pDir)
	{
		char szCurPath[500];
		ZeroMemory(szCurPath, 500);
		sprintf_s(szCurPath, 500, "%s//*", pDir);
		WIN32_FIND_DATAA FindFileData;
		ZeroMemory(&FindFileData, sizeof(WIN32_FIND_DATAA));

		HANDLE hFile = FindFirstFileA(szCurPath, &FindFileData); /**< find first file by given path. */

		if (hFile == INVALID_HANDLE_VALUE)
		{
			FindClose(hFile);
			return false; /** 如果不能找到第一个文件，那么没有目录 */
		}
		else
		{
			FindClose(hFile);
			return true;
		}

	}

	/**
	 @fn	bool DeleteDirectory(const char * DirName)
	
	 @brief	删除一个目录
	
	 @author	wangxin
	 @date	2017/4/11
	
	 @param	要删除的路径
	
	 @return	返回删除是否成功
	 */

	bool DeleteDirectory(const char * DirName)
	{
		//    CFileFind tempFind;        //声明一个CFileFind类变量，以用来搜索
		char szCurPath[MAX_PATH];        //用于定义搜索格式
		_snprintf(szCurPath, MAX_PATH, "%s//*.*", DirName);    //匹配格式为*.*,即该目录下的所有文件
		WIN32_FIND_DATAA FindFileData;
		ZeroMemory(&FindFileData, sizeof(WIN32_FIND_DATAA));
		HANDLE hFile = FindFirstFileA(szCurPath, &FindFileData);
		bool IsFinded = true;
		while (IsFinded)
		{
			IsFinded = FindNextFileA(hFile, &FindFileData);    //递归搜索其他的文件
			if (strcmp(FindFileData.cFileName, ".") && strcmp(FindFileData.cFileName, "..")) //如果不是"." ".."目录
			{
				std::string strFileName = "";
				strFileName = strFileName + DirName + "//" + FindFileData.cFileName;
				std::string strTemp;
				strTemp = strFileName;
				if (IsDirectory(strFileName.c_str())) //如果是目录，则递归地调用
				{
					DeleteDirectory(strTemp.c_str());
				}
				else
				{
					DeleteFileA(strTemp.c_str());
				}
			}
		}
		FindClose(hFile);

		BOOL bRet = RemoveDirectoryA(DirName);
		if (bRet == 0) //删除目录
		{
			return false;
		}
		return true;
	}

	/**
	 @fn	std::string getName_byPath(const char * _path)
	
	 @brief	根据带路径的文件名，获得文件名
	
	 @author	wangxin
	 @date	2017/4/11
	
	 @param	_path	带路径的文件名
	
	 @return	返回文件名
	 */

	std::string getName_byPath(const char * _path)
	{
		std::string filename;
		char szDrive[_MAX_DRIVE];   //磁盘名
		char szDir[_MAX_DIR];       //路径名
		char szFname[_MAX_FNAME];   //文件名
		char szExt[_MAX_EXT];       //后缀名
		_splitpath(_path, szDrive, szDir, szFname, szExt); //分解路径
		filename.append(szFname);
		return filename;
	}
#endif

}

#endif