#pragma once
#ifndef __STR_UTIL_H__
#define __STR_UTIL_H__
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#ifdef WIN32
#include <io.h>
#include"windows.h"
#include <assert.h>
#else
#endif

class str_util
{
public:

	/**
	@fn	std::string& trim(std::string &s, char c)

	@brief	ȥ��s�������ַ�c

	@author	wangxin
	@date	2017/4/11

	@param [in,out]	s	���������ַ���
	@param 			c	���ַ�����ȥ�����ַ�

	@return	����ȥ���ַ����ַ������ã��봫������stringΪͬһ������
	*/
	static std::string& trim(std::string &s, char c);

	/**
	 @fn	std::vector<std::string> split(std::string str, std::string pattern)

	 @brief	�ַ����ָ��.

	 @author	wangxin
	 @date	2017/4/11

	 @param	str	   	Ҫ���ָ���ַ���.
	 @param	pattern	�ָ��õ��ַ���.

	 @return	����һ���ָ����ַ�������;
	 */

	static std::vector<std::string> split(std::string str, std::string pattern);

	static std::string getInfoByReadTxt(char * pathname);

	/**
	 @fn	std::string GetStr(std::string str);

	 @brief	��ȡһ��string����.

	 @author	wangxin
	 @date	2017/4/11

	 @param	str	The string.

	 @return	The string.
	 */

	static std::string GetStr(std::string str);

	static unsigned char ToHex(unsigned char x);

	static unsigned char FromHex(unsigned char x);

	static std::string UrlEncode(const std::string& str);

	static std::string UrlDecode(const std::string& str);

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

	static void getFiles(std::string _path, std::string _exd, std::vector<std::string>& _files);

	/**
	 @fn	bool IsDirectory(const char * pDir)

	 @brief	�ж�һ��·���Ƿ�ΪĿ¼

	 @author	wangxin
	 @date	2017/4/11

	 @param	pDir	Ҫ�жϵ�·��

	 @return	�����ǲ���·��
	 */

	static bool IsDirectory(const char * pDir);

	/**
	 @fn	bool DeleteDirectory(const char * DirName)

	 @brief	ɾ��һ��Ŀ¼

	 @author	wangxin
	 @date	2017/4/11

	 @param	Ҫɾ����·��

	 @return	����ɾ���Ƿ�ɹ�
	 */

	static bool DeleteDirectory(const char * DirName);

	/**
	 @fn	std::string getName_byPath(const char * _path)

	 @brief	���ݴ�·�����ļ���������ļ���

	 @author	wangxin
	 @date	2017/4/11

	 @param	_path	��·�����ļ���

	 @return	�����ļ���
	 */

	static std::string getName_byPath(const char * _path);
#else
#endif

};

#endif
