#pragma once
#ifndef __STR_UTIL_H__
#define __STR_UTIL_H__
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#ifdef WIN32
#include <io.h>
#include"windows.h"
#include <assert.h>
#else
#endif

using std::string;
using std::vector;

class str_util
{
public:

	template <class T>

	/**
	 @fn	static string str_util::toString(T data)
	
	 @brief	��һ������ת��Ϊstring..
	
	 @author	wangxin
	 @date	2017/5/4
	
	 @param	data	The data.
	
	 @return	A string that represents this object.
	 */

	static string toString(T data)
	{
		std::stringstream sstr;
		string str;
		sstr << data;
		sstr >> str;
		return str;
	}

	/**
	@fn	std::string& trim(std::string &s, char c)

	@brief	ȥ��s�������ַ�c

	@author	wangxin
	@date	2017/4/11

	@param [in,out]	s	���������ַ���
	@param 			c	���ַ�����ȥ�����ַ�

	@return	����ȥ���ַ����ַ������ã��봫������stringΪͬһ������
	*/
	static string& trim(string &s, char c);

	/**
	 @fn	std::vector<string> split(string str, string pattern)

	 @brief	�ַ����ָ��.

	 @author	wangxin
	 @date	2017/4/11

	 @param	str	   	Ҫ���ָ���ַ���.
	 @param	pattern	�ָ��õ��ַ���.

	 @return	����һ���ָ����ַ�������;
	 */

	static vector<string> split(string str, string pattern);

	static string getInfoByReadTxt(char * pathname);

	/**
	 @fn	std::string GetStr(std::string str);

	 @brief	��ȡһ��string����.

	 @author	wangxin
	 @date	2017/4/11

	 @param	str	The string.

	 @return	The string.
	 */

	static string GetStr(string str);

	static unsigned char ToHex(unsigned char x);

	static unsigned char FromHex(unsigned char x);

	static string UrlEncode(const string& str);

	static string UrlDecode(const string& str);

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

	static void getFiles(string _path, string _exd, std::vector<string>& _files);

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

	static string getName_byPath(const char * _path);
#else
#endif

};

#endif
