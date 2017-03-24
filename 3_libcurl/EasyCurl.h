#ifndef __BASE_EASY_CURL_H__
#define __BASE_EASY_CURL_H__
//#include <Windows.h>
#include <string>
#include <functional>
using std::string;
//
class IProgressCallback
{
public:
	virtual bool OnProgressCallback(int nValue) = 0;
};

//
class EasyCurl
{
public:
	EasyCurl(void);
	~EasyCurl(void);
	typedef std::tr1::function<void(int)>  ProgressFunction;

public:

	/// @brief		HTTP POST请求
	/// @param[in]	strUrl 输入参数,请求的Url地址,如:https://www.baidu.com
	/// @param[in]	strParam 输入参数,使用格式"name=kandy&pwd=1234"
	/// @param[out]	strResponse 输出参数,返回的内容
	/// @param[in]	pCaPath 输入参数,为CA证书的路径.如果输入为NULL,则不验证服务器端证书的有效性.
	///	@remark		返回是否Post成功
	///	@return		CURLE_OK,成功!其余失败
	int http_post(const string & strUrl, const string & strParam, string & strResponse, const char * pCaPath = NULL);

	/// @brief		HTTPS GET请求
	/// @param[in]	strUrl 输入参数,请求的Url地址,如:https://www.baidu.com
	/// @param[out]	strResponse 输出参数,返回的内容
	/// @param[in]	pCaPath 输入参数,为CA证书的路径.如果输入为NULL,则不验证服务器端证书的有效性.
	///	@remark		返回是否Post成功
	///	@return		CURLE_OK,成功!其余失败
	int http_get(const string & strUrl, string & strResponse, const char * pCaPath = NULL);

	/// @brief		文件下载
	/// @param[in]	url : 要下载文件的url地址
	/// @param[in]	outfilename : 下载文件指定的文件名
	///	@remark		
	///	@return		返回0代表成功
	int download_file(const string & strUrl, const string & strFile);

	/// @brief		进度报告处理
	/// @param[in]	func : 函数地址
	///	@remark		
	///	@return		void
	void set_progress_function(ProgressFunction func);

	/// @brief		进度报告处理
	/// @param[in]	pCallback : 传入的对象
	///	@remark		使用的类继承于IProgressCallback
	///	@return		void
	void set_progress_callback(IProgressCallback *pCallback);
	//	
public:
	void SetDebug(bool bDebug);

protected:
	static int progress_callback(void *pParam, double dltotal, double dlnow, double ultotal, double ulnow);

private:
	bool m_bDebug;
	ProgressFunction	m_updateProgress;
	IProgressCallback	*m_pHttpCallback;
};

#endif//__BASE_EASY_CURL_H__