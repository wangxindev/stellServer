#if 1
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <curl/curl.h>
#include <fstream>

std::ofstream ofs;
size_t write_data_file3(void *buffer, size_t size, size_t nmemb, void *userp) {
#if 0
	FILE *fptr = (FILE*)userp;
	fwrite(buffer, size, nmemb, fptr);
	std::cout << size << " " << nmemb << std::endl;
	fclose(fptr);
	return 0;
#else
	strcpy(((char*)userp), (char *)buffer);
	ofs << (char*)userp;


	return nmemb*size;
#endif
}

void getJson(const char * url, const char*filename)
{
	CURL *curl;

	ofs.open(filename);
	char buf[1024 * 100] = { 0 };

	curl = curl_easy_init();

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_file3);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, buf);

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
	//curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(data));
	//curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

	curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	ofs.close();
}

void int2str(const int &int_temp, std::string &string_temp)
{
	char s[12];				//设定12位对于存储32位int值足够
	itoa(int_temp, s, 10);			//itoa函数亦可以实现，但是属于C中函数，在C++中推荐用流的方法
	string_temp.append(s);
}


int main(void)
{
#if 0
	getJson("http://dc.banggood.com/index.php?com=dcapi&t=getadList&ad_status=1&page=1", "page1.txt");
#else
	std::string url("http://dc.banggood.com/index.php?com=dcapi&t=getadList");
	std::string url2("&ad_status=");
	std::string url3("&page=");
	std::string filename("page");
	std::string fileType(".txt");
	for (int j = 1; j < 3; ++j)
	{

		for (int i = 1; i < 50; ++i)
		{
			std::string wUrl;
			wUrl.append(url);
			wUrl.append("&start_date=2017-03-18");
			wUrl.append(url2);
			int2str(j, wUrl);
			wUrl.append(url3);
			int2str(i, wUrl);

			std::string wFileName;
			wFileName.append(filename);
			int2str(j, wFileName);
			wFileName.append("_");
			int2str(i, wFileName);
			wFileName.append(fileType);

			std::cout << wUrl << std::endl;
			std::cout << wFileName << std::endl;
			char buf[4] = { 0 };
		send:		getJson(wUrl.c_str(), wFileName.c_str());

			std::ifstream ifile;
			ifile.open(wFileName.c_str());
			if (!ifile.is_open()) { std::cout << "txt open err\n"; }

			memset(buf, 0, 4);
			ifile.getline(buf, 4);
			if (strcmp(buf, "404\0") == 0)
			{
				ifile.close();
				goto send;
			}
			else
			{
				ifile.close();
				//std::cout << "ok" << std::endl;
			}

		}
	}
#endif

	return 0;
}

#else

#include "EasyCurl.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <assert.h>


typedef std::vector<std::string > strVec;
typedef std::vector<strVec > strVecVec;

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

std::string GetStr(std::string str)
{
	if (str.empty())
		return "";
	else
		return str;
}

//发送信息到对方接口
std::string postMessage(std::string strParam)
{
	//发送数据到对方接口
	string strResponse;
	string strUrl = "http://sem.banggood.com//index.php?com=api&t=newAdAuto&token=8e429c852f7e9646113aa9f270b6ad7f";
	EasyCurl client;
	client.http_post(strUrl, strParam, strResponse, NULL);
	std::cout << "======================================================\n";
	std::cout << "strResponse:\n" << strResponse << std::endl;
	return strResponse;
}


unsigned char ToHex(unsigned char x)   
{   
    return  x > 9 ? x + 55 : x + 48;   
}  
  
unsigned char FromHex(unsigned char x)   
{   
    unsigned char y;  
    if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;  
    else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;  
    else if (x >= '0' && x <= '9') y = x - '0';  
    else assert(0);  
    return y;  
}  

std::string UrlEncode(const std::string& str)  
{  
    std::string strTemp = "";  
    size_t length = str.length();  
    for (size_t i = 0; i < length; i++)  
    {  
        if (isalnum((unsigned char)str[i]) ||   
            (str[i] == '-') ||  
            (str[i] == '_') ||   
            (str[i] == '.') ||   
            (str[i] == '~'))  
            strTemp += str[i];  
        else if (str[i] == ' ')  
            strTemp += "+";  
        else  
        {  
            strTemp += '%';  
            strTemp += ToHex((unsigned char)str[i] >> 4);  
            strTemp += ToHex((unsigned char)str[i] % 16);  
        }  
    }  
    return strTemp;  
}  

std::string UrlDecode(const std::string& str)  
{  
    std::string strTemp = "";  
    size_t length = str.length();  
    for (size_t i = 0; i < length; i++)  
    {  
        if (str[i] == '+') strTemp += ' ';  
        else if (str[i] == '%')  
        {  
            assert(i + 2 < length);  
            unsigned char high = FromHex((unsigned char)str[++i]);  
            unsigned char low = FromHex((unsigned char)str[++i]);  
            strTemp += high*16 + low;  
        }  
        else strTemp += str[i];  
    }  
    return strTemp;  
}  

//将参数合成为接口需要的文本信息，并发送
int loopPost(strVecVec vec)
{
	int i = 0;
	for (auto iter : vec)
	{
		string strParam;
		{//拼接参数
			std::string param[10] = { "dc_id=", "&domain=", "&type=", "&text=", "&see_more_link=", "&image_url=", "&image_link=", "&image_desc=", "&image_headline=", "&cat_id=" };
			for (int i = 0; i < 10; ++i)
			{
				strParam.append(param[i]);
				strParam.append(UrlEncode(iter[i]));
			}
			std::cout << "======================================================\n";
			std::cout << "strParam:\n" << strParam << std::endl;
		}

#if 0
		std::string strParamTmp = UrlEncode(strParam);
		std::string strResponse = postMessage(strParamTmp);
#else
		std::string strResponse = postMessage(strParam);
#endif
		++i;
	}
	return i;
}
//将本地读取的文件转化成合适的参数供合成
strVecVec makeInfo(std::string strData)
{
	strVec dataLineVec;
	strVecVec idVec;
	if (!strData.empty())
	{
		dataLineVec = split(strData, "\n");
		do//"dc_id","domain","type","text","see_more_link","image_url","image_link","image_desc","image_headline"
		{
			if (dataLineVec.empty())
				break;
			if (dataLineVec[0] == "")
			{
				dataLineVec.erase(dataLineVec.begin());
				continue;
			}

			std::string dc_id;
			std::string domain;
			std::string type;
			std::string text;
			std::string see_more_link;
			std::string image_url;
			std::string image_link;
			std::string image_desc;
			std::string image_headline;
			std::string cat_id;

			strVec dataVec;
			dataVec = split(dataLineVec[0], ",");
			if (dataVec.empty() || dataVec.size() < 10)
			{
				dataLineVec.erase(dataLineVec.begin());
				continue;
			}

			dc_id = GetStr(dataVec[0]);
			domain = GetStr(dataVec[1]);
			type = GetStr(dataVec[2]);
			text = GetStr(dataVec[3]);
			see_more_link = GetStr(dataVec[4]);
			//image_url = dataVec[4];
			image_url.append("[{\"image_url\":[");
			image_link.append("[{\"image_link\":[");
			image_desc.append("[{\"image_desc\":[");
			image_headline.append("[{\"image_headline\":[");

			//拿到每一行

			int i = 0;
			for (auto iter : dataLineVec)
			{

				std::vector <std::string > dataVecTmp;
				dataVecTmp = split(iter, ",");
				if (dataVecTmp[0] == dc_id)
				{
					++i;
					image_url.append(GetStr(dataVecTmp[5]));
					image_url.append(",");
					image_link.append(GetStr(dataVecTmp[6]));
					image_link.append(",");
					image_desc.append(GetStr(dataVecTmp[7]));
					image_desc.append(",");
					image_headline.append(GetStr(dataVecTmp[8]));
					image_headline.append(",");

				}
			}

			image_url.append("]}]");
			image_link.append("]}]");
			image_desc.append("]}]");
			image_headline.append("]}]");
			cat_id = GetStr(dataVec[9]);

			dataLineVec.erase(dataLineVec.begin(), dataLineVec.begin() + i);
			strVec idVecTmp;
			idVecTmp.push_back(dc_id);
			idVecTmp.push_back(domain);
			idVecTmp.push_back(type);
			idVecTmp.push_back(text);
			idVecTmp.push_back(see_more_link);
			idVecTmp.push_back(image_url);
			idVecTmp.push_back(image_link);
			idVecTmp.push_back(image_desc);
			idVecTmp.push_back(image_headline);
			idVecTmp.push_back(cat_id);
			idVec.push_back(idVecTmp);

		} while (dataLineVec.size());

	}

	return idVec;
}

std::string getInfoByReadTxt(char * pathname)
{
	std::ifstream ifs;
	ifs.open(pathname);

	std::string strData;
	char buf[2] = { 0 };

	while (ifs.read(buf, 1))
	{
		strData.append(buf);
		memset(buf, 0, 2);
	}

	return strData;
}

int main(void)
{
	std::string strData = getInfoByReadTxt("send_table.txt");
	strVecVec idVec = makeInfo(strData);
	int count = loopPost(idVec);

	getchar();

	return 0;
}
#endif
