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

	/// @brief		HTTP POST����
	/// @param[in]	strUrl �������,�����Url��ַ,��:https://www.baidu.com
	/// @param[in]	strParam �������,ʹ�ø�ʽ"name=kandy&pwd=1234"
	/// @param[out]	strResponse �������,���ص�����
	/// @param[in]	pCaPath �������,ΪCA֤���·��.�������ΪNULL,����֤��������֤�����Ч��.
	///	@remark		�����Ƿ�Post�ɹ�
	///	@return		CURLE_OK,�ɹ�!����ʧ��
	int http_post(const string & strUrl, const string & strParam, string & strResponse, const char * pCaPath = NULL);

	/// @brief		HTTPS GET����
	/// @param[in]	strUrl �������,�����Url��ַ,��:https://www.baidu.com
	/// @param[out]	strResponse �������,���ص�����
	/// @param[in]	pCaPath �������,ΪCA֤���·��.�������ΪNULL,����֤��������֤�����Ч��.
	///	@remark		�����Ƿ�Post�ɹ�
	///	@return		CURLE_OK,�ɹ�!����ʧ��
	int http_get(const string & strUrl, string & strResponse, const char * pCaPath = NULL);

	/// @brief		�ļ�����
	/// @param[in]	url : Ҫ�����ļ���url��ַ
	/// @param[in]	outfilename : �����ļ�ָ�����ļ���
	///	@remark		
	///	@return		����0����ɹ�
	int download_file(const string & strUrl, const string & strFile);

	/// @brief		���ȱ��洦��
	/// @param[in]	func : ������ַ
	///	@remark		
	///	@return		void
	void set_progress_function(ProgressFunction func);

	/// @brief		���ȱ��洦��
	/// @param[in]	pCallback : ����Ķ���
	///	@remark		ʹ�õ���̳���IProgressCallback
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