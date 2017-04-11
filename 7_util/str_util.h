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

	@brief	去除s中所有字符c

	@author	wangxin
	@date	2017/4/11

	@param [in,out]	s	备操作的字符串
	@param 			c	在字符串中去掉的字符

	@return	返回去除字符的字符串引用，与传进来的string为同一个对象
	*/
	static std::string& trim(std::string &s, char c);

	/**
	 @fn	std::vector<std::string> split(std::string str, std::string pattern)

	 @brief	字符串分割函数.

	 @author	wangxin
	 @date	2017/4/11

	 @param	str	   	要被分割的字符串.
	 @param	pattern	分割用的字符串.

	 @return	返回一个分割后的字符串数组;
	 */

	static std::vector<std::string> split(std::string str, std::string pattern);

	static std::string getInfoByReadTxt(char * pathname);

	/**
	 @fn	std::string GetStr(std::string str);

	 @brief	获取一个string对象.

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

	 @brief	获得一个目录下的所有文件名

	 @author	wangxin
	 @date	2017/4/11

	 @param 			_path 	路径
	 @param 			_exd  	文件类型，为空则表示全部类型.
	 @param [in,out]	_files	返回的文件名集合，文件名包含路径.
	 */

	static void getFiles(std::string _path, std::string _exd, std::vector<std::string>& _files);

	/**
	 @fn	bool IsDirectory(const char * pDir)

	 @brief	判断一个路径是否为目录

	 @author	wangxin
	 @date	2017/4/11

	 @param	pDir	要判断的路径

	 @return	返回是不是路径
	 */

	static bool IsDirectory(const char * pDir);

	/**
	 @fn	bool DeleteDirectory(const char * DirName)

	 @brief	删除一个目录

	 @author	wangxin
	 @date	2017/4/11

	 @param	要删除的路径

	 @return	返回删除是否成功
	 */

	static bool DeleteDirectory(const char * DirName);

	/**
	 @fn	std::string getName_byPath(const char * _path)

	 @brief	根据带路径的文件名，获得文件名

	 @author	wangxin
	 @date	2017/4/11

	 @param	_path	带路径的文件名

	 @return	返回文件名
	 */

	static std::string getName_byPath(const char * _path);
#else
#endif

};

#endif
