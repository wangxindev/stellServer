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
	char buf[1024*100] = { 0 };
	
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
send:		getJson(wUrl.c_str(),wFileName.c_str());

			std::ifstream ifile;
			ifile.open(wFileName.c_str());
			if (!ifile.is_open()) { std::cout << "txt open err\n";}

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