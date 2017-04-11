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

 @brief	ȥ��s�������ַ�c

 @author	wangxin
 @date	2017/4/11

 @param [in,out]	s	���������ַ���
 @param 			c	���ַ�����ȥ�����ַ�

 @return	����ȥ���ַ����ַ������ã��봫������stringΪͬһ������
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

	 @brief	�ַ����ָ��.

	 @author	wangxin
	 @date	2017/4/11

	 @param	str	   	Ҫ���ָ���ַ���.
	 @param	pattern	�ָ��õ��ַ���.

	 @return	����һ���ָ����ַ�������;
	 */

	std::vector<std::string> split(std::string str, std::string pattern)
	{
		std::string::size_type pos;
		std::vector<std::string> result;
		str += pattern;//��չ�ַ����Է������
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
	
	 @brief	���һ��Ŀ¼�µ������ļ���
	
	 @author	wangxin
	 @date	2017/4/11
	
	 @param 			_path 	·��
	 @param 			_exd  	�ļ����ͣ�Ϊ�����ʾȫ������.
	 @param [in,out]	_files	���ص��ļ������ϣ��ļ�������·��.
	 */

	void getFiles(std::string _path, std::string _exd, std::vector<std::string>& _files)
	{
		//�ļ����
		long   hFile = 0;
		//�ļ���Ϣ
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
				//������ļ����������ļ���,����֮
				//�������,�����б�
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
	
	 @brief	�ж�һ��·���Ƿ�ΪĿ¼
	
	 @author	wangxin
	 @date	2017/4/11
	
	 @param	pDir	Ҫ�жϵ�·��
	
	 @return	�����ǲ���·��
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
			return false; /** ��������ҵ���һ���ļ�����ôû��Ŀ¼ */
		}
		else
		{
			FindClose(hFile);
			return true;
		}

	}

	/**
	 @fn	bool DeleteDirectory(const char * DirName)
	
	 @brief	ɾ��һ��Ŀ¼
	
	 @author	wangxin
	 @date	2017/4/11
	
	 @param	Ҫɾ����·��
	
	 @return	����ɾ���Ƿ�ɹ�
	 */

	bool DeleteDirectory(const char * DirName)
	{
		//    CFileFind tempFind;        //����һ��CFileFind�����������������
		char szCurPath[MAX_PATH];        //���ڶ���������ʽ
		_snprintf(szCurPath, MAX_PATH, "%s//*.*", DirName);    //ƥ���ʽΪ*.*,����Ŀ¼�µ������ļ�
		WIN32_FIND_DATAA FindFileData;
		ZeroMemory(&FindFileData, sizeof(WIN32_FIND_DATAA));
		HANDLE hFile = FindFirstFileA(szCurPath, &FindFileData);
		bool IsFinded = true;
		while (IsFinded)
		{
			IsFinded = FindNextFileA(hFile, &FindFileData);    //�ݹ������������ļ�
			if (strcmp(FindFileData.cFileName, ".") && strcmp(FindFileData.cFileName, "..")) //�������"." ".."Ŀ¼
			{
				std::string strFileName = "";
				strFileName = strFileName + DirName + "//" + FindFileData.cFileName;
				std::string strTemp;
				strTemp = strFileName;
				if (IsDirectory(strFileName.c_str())) //�����Ŀ¼����ݹ�ص���
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
		if (bRet == 0) //ɾ��Ŀ¼
		{
			return false;
		}
		return true;
	}

	/**
	 @fn	std::string getName_byPath(const char * _path)
	
	 @brief	���ݴ�·�����ļ���������ļ���
	
	 @author	wangxin
	 @date	2017/4/11
	
	 @param	_path	��·�����ļ���
	
	 @return	�����ļ���
	 */

	std::string getName_byPath(const char * _path)
	{
		std::string filename;
		char szDrive[_MAX_DRIVE];   //������
		char szDir[_MAX_DIR];       //·����
		char szFname[_MAX_FNAME];   //�ļ���
		char szExt[_MAX_EXT];       //��׺��
		_splitpath(_path, szDrive, szDir, szFname, szExt); //�ֽ�·��
		filename.append(szFname);
		return filename;
	}
#endif

}

#endif