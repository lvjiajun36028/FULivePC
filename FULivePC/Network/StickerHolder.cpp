﻿#include "StickerHolder.h"

#include <stdio.h>
#include <string.h>
#include <thread>
#include <algorithm>
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "fu_tool.h"
#include "fu_tool_mac.h"
#include <fstream>
#include "zlib.h"
#include "zip.h"

//#define CURL_DEBUG 1
#define CURL_WAIT_TIMEOUT_MSECS 60000 //60s
#define CURL_MULIT_MAX_NUM 5

static size_t recive_data_fun(void* buffer, size_t size, size_t nmemb, void* stream) {
	//return fwrite(ptr, size, nmemb, (FILE*)stream);
	(static_cast<std::string*>(stream))->append((char*)buffer, size * nmemb);
	return (size * nmemb);
}

static size_t read_head_fun(void* ptr, size_t size, size_t nmemb, void* stream) {
	char head[2048] = { 0 };
	memcpy(head, ptr, size * nmemb + 1);
	printf(" %s \n", head);
	return size * nmemb;
}

#define CATEGORY_STICKER "sticker"
#define CATEGORY_AVATAR "avatar"
#define CATEGORY_ANIMOJI "Animoji"

bool BundleRes::IsAvatar()
{
	return mCategory == CATEGORY_AVATAR;
}

StikcerHolder::StikcerHolder()
{
	curl_global_init(CURL_GLOBAL_ALL);
	//Login();

	//DownList();

	RequestTags();

	RequestTools();
}

StikcerHolder::~StikcerHolder()
{
	curl_easy_cleanup(mCurl);
}


#define MAIN_URL  string("https://items.faceunity.com:4006/api") //string("http://192.168.0.122:8089/api") 

#define GET_QUEST_TAG_URL  (MAIN_URL + "/guest/tags?platform=pc")

#define POST_QUEST_TOOLS_URL  (MAIN_URL + "/guest/tools")

#define POST_QUEST_DOWNLOAD (MAIN_URL + "/guest/download")

#define POST_LOGIN_URL (MAIN_URL + "/user/login")

#define POST_DOWNLOADLIST_URL (MAIN_URL + "/download/list")

#define FILENAME "curltest.log"

void StikcerHolder::RequestTags()
{
	std::string strResp;
	if (Get(GET_QUEST_TAG_URL, "", "", strResp))
	{
		rapidjson::Document doc;
		doc.Parse(strResp.c_str());
		const rapidjson::Value& dataObj = doc["data"];
		for (int i = 0; i < dataObj.Size(); i++)
		{
			mTags.push_back(dataObj[i].GetString());
		}
	}
}

std::string StikcerHolder::RequestBundleUrl(std::string strId)
{
	std::string strUrl;
	std::string strBundleResp;

	if (Get(POST_QUEST_DOWNLOAD, "?id=" + strId + "&platform=pc", "", strBundleResp))
	{
		rapidjson::Document bundleDoc;
		bundleDoc.Parse(strBundleResp.c_str());
		if (bundleDoc.HasMember("data"))
		{
			if (bundleDoc["data"].HasMember("url"))
			{
				strUrl = bundleDoc["data"]["url"].GetString();
			}
		}
	}

	return strUrl;
}

void StikcerHolder::RequestTools()
{
	int addone = 0;
	for (auto it : mTags)
	{
		rapidjson::StringBuffer buf;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buf);
		writer.StartObject();
		writer.Key("platform");
		writer.String("pc");
		writer.Key("tag");
		writer.String(it.c_str());
		writer.EndObject();

		std::string strResp;
		Post(POST_QUEST_TOOLS_URL, "", buf.GetString(), strResp);


		//printf("out : %s \r\n", strResp.data());

		rapidjson::Document doc;
		doc.Parse(strResp.c_str());
		const rapidjson::Value& dataObj = doc["data"];

		std::vector<std::shared_ptr<BundleRes>> vecBundleRes;
		const rapidjson::Value& docsObj = dataObj["docs"];
		for (int i = 0; i < docsObj.Size(); i++)
		{
			const rapidjson::Value& docObj = docsObj[i];
			if (!docObj.HasMember("tool"))
				continue;
			auto& tool = docObj["tool"];
			if (!tool.HasMember("_id") || !tool.HasMember("bundle")) //!tool.HasMember("icon")
			{
				continue;
			}

			std::string iconName = !tool.HasMember("icon") ? "" : FuTool::convert2local8bit(tool["icon"]["name"].GetString());

			std::vector<std::string> names;
			names.emplace_back(FuTool::convert2local8bit(tool["bundle"]["name"].GetString()));
			auto bundleRes = std::make_shared<BundleRes>(tool["_id"].GetString(),
				names[0], names,
				iconName);
			bundleRes->mIconUrl = !tool.HasMember("icon") ? "" : tool["icon"]["url"].GetString();

			if (tool.HasMember("adapter"))
			{
				int32_t nFlag = 0;
				std::string strFlag = tool["adapter"].GetString();
				if (strFlag.find('1') != strFlag.npos)
				{
					nFlag |= eStickerFlagAdapter::ADAPTER_FLAG_SINGLE_PEOPLE;
				}
				if (strFlag.find('2') != strFlag.npos)
				{
					nFlag |= eStickerFlagAdapter::ADAPTER_FLAG_MAKEUP_FLIP;
				}
				if (strFlag.find('3') != strFlag.npos)
				{
					nFlag |= eStickerFlagAdapter::ADAPTER_FLAG_IS_NEED_CLICK;
				}
				if (strFlag.find('4') != strFlag.npos)
				{
					nFlag |= eStickerFlagAdapter::ADAPTER_FLAG_IS_NEED_3DFLIP;
				}
				bundleRes->mAdapterFlag = nFlag;
			}
			bundleRes->mCategory = CATEGORY_STICKER;
			if (tool.HasMember("category")) {
				std::string category = tool["category"].GetString();
				if (category == CATEGORY_AVATAR)
					bundleRes->mCategory = category;
				else if (category == CATEGORY_ANIMOJI)
					bundleRes->mCategory = category;
			}
#ifdef __APPLE__
			if(bundleRes->mIconName != "小熊icon.png")
#endif
			vecBundleRes.emplace_back(bundleRes);
		}

		mTagBundleList.emplace_back(vecBundleRes);
	}

	for (auto bundleList : mTagBundleList)
	{
		for (auto it : bundleList)
		{
			DownLoadFile(it->mIconUrl, it->mIconDir);
		}
	}
}



bool StikcerHolder::Post(const std::string& strHost, const std::string& strMethod, const std::string& strArgument, std::string& strResponse)
{

	bool bStatus = true;
	CURLcode curlRet = CURLE_OK;
	do
	{
		std::string url = strHost + strMethod;
		for (int i = 0; i < 3; i++)
		{
			if (mCurl == nullptr)
			{
				mCurl = curl_easy_init();
			}

			struct curl_slist* headers = NULL;
			headers = curl_slist_append(headers, "Content-Type:application/json;charset=UTF-8");
			curl_easy_setopt(mCurl, CURLOPT_SSL_VERIFYPEER, false);
			curl_easy_setopt(mCurl, CURLOPT_HTTPHEADER, headers);

			curl_easy_setopt(mCurl, CURLOPT_TIMEOUT, 3000);
			curl_easy_setopt(mCurl, CURLOPT_SSL_VERIFYPEER, false);
			curl_easy_setopt(mCurl, CURLOPT_SSL_VERIFYHOST, true);
			curl_easy_setopt(mCurl, CURLOPT_URL, url.c_str());
			curl_easy_setopt(mCurl, CURLOPT_POST, true);
			curl_easy_setopt(mCurl, CURLOPT_POSTFIELDS, strArgument.c_str());
			curl_easy_setopt(mCurl, CURLOPT_POSTFIELDSIZE, strArgument.size());
			curl_easy_setopt(mCurl, CURLOPT_WRITEFUNCTION, recive_data_fun);
			curl_easy_setopt(mCurl, CURLOPT_WRITEDATA, &strResponse);
			curl_easy_setopt(mCurl, CURLOPT_BUFFERSIZE, 20480);
			//curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, &on_debug);
			curlRet = curl_easy_perform(mCurl);

			curl_easy_reset(mCurl);
			//curl_easy_cleanup(curl);
			if (curlRet == CURLE_OK)
				break;
			std::this_thread::sleep_for(std::chrono::milliseconds(30));
		}
	} while (0);
	if (curlRet != CURLE_OK)
	{
		return false;
	}
	return bStatus;
}

bool StikcerHolder::Get(const std::string& strHost, const std::string& strMethod, const std::string& strArgument, std::string& strResponse)
{
	bool bStatus = true;
	CURLcode curlRet = CURLE_OK;
	do
	{
		std::string url = strHost + strMethod;
		for (int i = 0; i < 3; i++)
		{
			if (mCurl == nullptr)
			{
				mCurl = curl_easy_init();
			}

			struct curl_slist* headers = NULL;
			headers = curl_slist_append(headers, "Content-Type:application/json;charset=UTF-8");
			curl_easy_setopt(mCurl, CURLOPT_SSL_VERIFYPEER, false);
			curl_easy_setopt(mCurl, CURLOPT_HTTPHEADER, headers);

			curl_easy_setopt(mCurl, CURLOPT_TIMEOUT, 3000);
			curl_easy_setopt(mCurl, CURLOPT_URL, url.c_str());
			curl_easy_setopt(mCurl, CURLOPT_WRITEFUNCTION, recive_data_fun);
			curl_easy_setopt(mCurl, CURLOPT_WRITEDATA, &strResponse);
			curl_easy_setopt(mCurl, CURLOPT_BUFFERSIZE, 20480);
			curlRet = curl_easy_perform(mCurl);

			curl_easy_reset(mCurl);
			if (curlRet == CURLE_OK)
				break;
			std::this_thread::sleep_for(std::chrono::milliseconds(30));
		}
	} while (0);
	if (curlRet != CURLE_OK)
	{
		return false;
	}
	return bStatus;
}

int StikcerHolder::WriteCallback(void* pBuffer, size_t nSize, size_t nMemByte, FILE* hFile)
{
	unsigned int dwWritten = 0;
	if (hFile == nullptr)
		return 0;

	dwWritten = fwrite(pBuffer, nSize, nMemByte, hFile);

	return dwWritten;
}

bool StikcerHolder::DownLoadFile(std::string strUrl, std::string strPath)
{
	if (strUrl == "")
		return false;

	CURLcode curlRet;

	if (mCurl == nullptr)
	{
		mCurl = curl_easy_init();
	}

	curl_easy_setopt(mCurl, CURLOPT_WRITEDATA, this);
	CURLcode code = curl_easy_setopt(mCurl, CURLOPT_URL, strUrl.c_str());

	remove(strPath.c_str());
	FILE* pFile = fopen(strPath.c_str(), "ab+");
	if (nullptr == pFile)
	{
		return false;
	}
	curl_easy_setopt(mCurl, CURLOPT_SSL_VERIFYPEER, false);
	curl_easy_setopt(mCurl, CURLOPT_BUFFERSIZE, 10240);
	curl_easy_setopt(mCurl, CURLOPT_NOPROGRESS, 0);
	curl_easy_setopt(mCurl, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(mCurl, CURLOPT_WRITEDATA, pFile);
	curl_easy_setopt(mCurl, CURLOPT_PROGRESSDATA, this);

	curlRet = curl_easy_perform(mCurl);
	fclose(pFile);
	return curlRet == CURLE_OK ? true : false;
}

void StikcerHolder::UnzipFile(string strPath, string dirPath, int stickerIndex, int index)
{

	struct zip* pZip;
	pZip = zip_open(strPath.c_str(), 0, &errno);
	int numitems = zip_get_num_files(pZip);

	mTagBundleList[stickerIndex][index]->mBundleDirs.clear();
	for (int i = 0; i < numitems; i++)
	{
		struct zip_stat zipStat = { 0 };
		zip_stat_init(&zipStat);
		zip_stat_index(pZip, i, 0, &zipStat);
		struct zip_file* pzipFile = zip_fopen_index(pZip, i, 0);
		char* buf = new char[zipStat.size];
		zip_fread(pzipFile, buf, zipStat.size);
		std::fstream fs;
		string file = dirPath + zipStat.name;
		fs.open(file, std::fstream::binary | std::fstream::out);
		fs.write(buf, zipStat.size);
		fs.close();
		string fileName(zipStat.name);
		if (fileName.find(".bundle") != string::npos) {
			mTagBundleList[stickerIndex][index]->mBundleDirs.emplace_back(dirPath + fileName);
		}
	}

}
