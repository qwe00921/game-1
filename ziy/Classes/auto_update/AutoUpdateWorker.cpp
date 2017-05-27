#include "AutoUpdateWorker.hpp"
#include "unzip/unzip.h"
#include "base/CCAsyncTaskPool.h"
#include "network/HttpClient.h"
#include "tools/LibSha256.h"
#include "otherfunction.h"

USING_NS_CC;
using namespace network;
using namespace std;
using namespace nlohmann;

#define BUFFER_SIZE    8192
#define MAX_FILENAME   512

static void post_json_to_url(string url, json &postData, std::function<void(bool success, json result)>callback);
static void calculate_sha256(Data &data, string &result_string);

bool AutoUpdateWorker::init() {
    _fileUtils = FileUtils::getInstance();
    _userDefault = UserDefault::getInstance();
    _scheduler = Director::getInstance()->getScheduler();
    _zipFile = _fileUtils->getWritablePath() + "1234.zip";
    _patchInfoFile = _fileUtils->getWritablePath() + "patch_info";
    _patchDir = _fileUtils->getWritablePath() + "patch_dir/";
    _testDir = _fileUtils->getWritablePath() + "test_dir/";
    _checkUpdateUrl = "https://update.trgame.cn:9000/checkupdate";
    _downloadPatchUrlBase = "https://download.trgame.cn/patchfiles/";

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	_downloadPatchUrlBase = "http://download.trgame.cn/patchfiles/";
#endif

    _patchCheckResult = nullptr;
    CCLOG("init patch_version %d", _userDefault->getIntegerForKey("patch_version"));
    return true;
}

void AutoUpdateWorker::checkUpdate() {
    tryDelete();
    json postData;
    postData["package_name"] = statGetDeviceParams(StatParamPackageName).getChar();
    postData["version"] = statGetDeviceParams(StatParamVersion).getChar();
    postData["patch_version"] = 0;
	CCLOG("code postData %s", postData.dump().c_str());
    post_json_to_url(_checkUpdateUrl, postData, [this](bool success, json result){
        cout<<"in callback2" << endl;
        cout<<result<<endl;
        int cb_result = 0;
        _patchCheckResult = result;
        if(!success || result.is_null() || !result["status"].is_number()) {
            cb_result = -1;
        } else if ( result.value("status", 0) == 101) { //101 means no patch file for this version
            cb_result = 0;
        } else if( result.value("status", 0) != 100 || //100 means found patch file for this version
           !result["patch_version"].is_number() ||
           !result["file_2"].is_string() ||
           !result["size_2"].is_number() ||
           !result["sha256_2"].is_string()) { // err
            cb_result = -1;
        } else {
            if(_userDefault->getIntegerForKey("patch_version") < (int)result["patch_version"]) {
                cb_result = result.value("size_2", -1);
            } else {
                cb_result = 0;
            }
        }
        _scheduler->performFunctionInCocosThread([this, cb_result](){
            delegator->checkUpdateCB(cb_result);
        });
//        _newPatchVersion = result["patch_version"];
//        downloadUpdate();
    });

    //fileDownloader->createDownloadDataTask(const std::string& srcUrl, const std::string& identifier = "");
}

void AutoUpdateWorker::downloadUpdate()
{
	fileDownloader = std::shared_ptr<network::Downloader>(new network::Downloader);
	//    fileDownloader->setConnectionTimeout(DEFAULT_CONNECTION_TIMEOUT);
	fileDownloader->onTaskError = [this](const DownloadTask& task,
		int errorCode,
		int errorCodeInternal,
		const std::string& errorStr)
	{
		CCLOG("Download error %d %d %s", errorCode, errorCodeInternal, errorStr.c_str());
		delegator->updateResultCB(false);
	};

	fileDownloader->onFileTaskSuccess = [this](const DownloadTask& task)
	{
		CCLOG("Download complete %d", _fileUtils->getFileSize(_fileUtils->getWritablePath() + "1234.zip"));
		auto fileData = _fileUtils->getDataFromFile(_fileUtils->getWritablePath() + "1234.zip");
		string fileDataSha256;
		calculate_sha256(fileData, fileDataSha256);
		cout << fileDataSha256 << endl;
		if(fileDataSha256 != _patchCheckResult["sha256_2"])
		{
			_scheduler->performFunctionInCocosThread([this]()
			{
				delegator->updateResultCB(false);
			});
			return;
		}

		std::function<void(void*)> mainThread = [this](void* param)
		{

		};
		AsyncTaskPool::getInstance()->enqueue(AsyncTaskPool::TaskType::TASK_OTHER, mainThread, nullptr, [this]()
		{
			bool result = false;
			_fileUtils->removeDirectory(_testDir);
			if(!decompress(_zipFile, _testDir))
				goto done;
			_fileUtils->removeDirectory(_patchDir);
			if(!_fileUtils->renameFile(_testDir, _patchDir))
				goto done;
			if(!_fileUtils->removeFile(_zipFile))
				goto done;
			_userDefault->setIntegerForKey("patch_version", _patchCheckResult["patch_version"]);
			_userDefault->flush();
			result = true;
		done:
			CCLOG("uncompress done %d! %s", result, _userDefault->getXMLFilePath().c_str());
			_scheduler->performFunctionInCocosThread([this, result]
			{
				delegator->updateResultCB(result);
			});

		});

	};

	fileDownloader->onTaskProgress = [this](const DownloadTask& task,
		int64_t bytesReceived,
		int64_t totalBytesReceived,
		int64_t totalBytesExpected)
	{
		CCLOG("Downloading %lld %lld %lld", bytesReceived, totalBytesReceived, totalBytesExpected);
		float progress = totalBytesExpected ? (float)totalBytesReceived / (float)totalBytesExpected : 0;
		_scheduler->performFunctionInCocosThread([this, progress]
		{
			delegator->updateProgressingCB(progress);
		});

	};

	auto url = _downloadPatchUrlBase + _patchCheckResult.value("file_2", "");
	fileDownloader->createDownloadFileTask(url, _fileUtils->getWritablePath() + "1234.zip");
}

int AutoUpdateWorker::getPatchVersion()
{
	return _userDefault->getIntegerForKey("patch_version");
}

void AutoUpdateWorker::tryDelete()
{
	auto package_version = Application::getInstance()->getVersion();
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	package_version = statGetDeviceParams(StatParamVersion).getChar();
#endif
	auto last_package_version = _userDefault->getStringForKey("last_package_version");
	if(last_package_version != package_version)
	{
		_fileUtils->removeDirectory(_patchDir);
		_userDefault->setStringForKey("last_package_version", package_version);
		_userDefault->setIntegerForKey("patch_version", 0);
		_userDefault->flush();
	}
}

std::string AutoUpdateWorker::basename(const std::string& path) const
{
	size_t found = path.find_last_of("/\\");

	if(std::string::npos != found)
	{
		return path.substr(0, found);
	}
	else
	{
		return path;
	}
}

bool AutoUpdateWorker::decompress(const std::string &zip, const std::string &dest_dir)
{
	// Find root path for zip file
	size_t pos = zip.find_last_of("/\\");
	if(pos == std::string::npos)
	{
		CCLOG("AssetsManagerEx : no root path specified for zip file %s\n", zip.c_str());
		return false;
	}
	const std::string rootPath = dest_dir;//zip.substr(0, pos+1);

	// Open the zip file
	unzFile _zipFile = unzOpen(_fileUtils->getSuitableFOpen(zip).c_str());
	if(!_zipFile)
	{
		CCLOG("AssetsManagerEx : can not open downloaded zip file %s\n", zip.c_str());
		return false;
	}

	// Get info about the zip file
	unz_global_info global_info;
	if(unzGetGlobalInfo(_zipFile, &global_info) != UNZ_OK)
	{
		CCLOG("AssetsManagerEx : can not read file global info of %s\n", zip.c_str());
		unzClose(_zipFile);
		return false;
	}

	// Buffer to hold data read from the zip file
	char readBuffer[BUFFER_SIZE];
	// Loop to extract all files.
	uLong i;
	for(i = 0; i < global_info.number_entry; ++i)
	{
		// Get info about current file.
		unz_file_info fileInfo;
		char fileName[MAX_FILENAME];
		if(unzGetCurrentFileInfo(_zipFile,
			&fileInfo,
			fileName,
			MAX_FILENAME,
			NULL,
			0,
			NULL,
			0) != UNZ_OK)
		{
			CCLOG("AssetsManagerEx : can not read compressed file info\n");
			unzClose(_zipFile);
			return false;
		}
		const std::string fullPath = rootPath + fileName;

		// Check if this entry is a directory or a file.
		const size_t filenameLength = strlen(fileName);
		if(fileName[filenameLength - 1] == '/')
		{
			//There are not directory entry in some case.
			//So we need to create directory when decompressing file entry
			if(!_fileUtils->createDirectory(basename(fullPath)))
			{
				// Failed to create directory
				CCLOG("AssetsManagerEx : can not create directory %s\n", fullPath.c_str());
				unzClose(_zipFile);
				return false;
			}
		}
		else
		{
			// Entry is a file, so extract it.
			// Open current file.
			if(unzOpenCurrentFile(_zipFile) != UNZ_OK)
			{
				CCLOG("AssetsManagerEx : can not extract file %s\n", fileName);
				unzClose(_zipFile);
				return false;
			}

			// Create a file to store current file.
			FILE *out = fopen(_fileUtils->getSuitableFOpen(fullPath).c_str(), "wb");
			if(!out)
			{
				CCLOG("AssetsManagerEx : can not create decompress destination file %s\n", fullPath.c_str());
				unzCloseCurrentFile(_zipFile);
				unzClose(_zipFile);
				return false;
			}

			// Write current file content to destinate file.
			int error = UNZ_OK;
			do
			{
				error = unzReadCurrentFile(_zipFile, readBuffer, BUFFER_SIZE);
				if(error < 0)
				{
					CCLOG("AssetsManagerEx : can not read zip file %s, error code is %d\n", fileName, error);
					fclose(out);
					unzCloseCurrentFile(_zipFile);
					unzClose(_zipFile);
					return false;
				}

				if(error > 0)
				{
					fwrite(readBuffer, error, 1, out);
				}
			}
			while(error > 0);

			fclose(out);
		}

		unzCloseCurrentFile(_zipFile);

		// Goto next entry listed in the zip file.
		if((i + 1) < global_info.number_entry)
		{
			if(unzGoToNextFile(_zipFile) != UNZ_OK)
			{
				CCLOG("AssetsManagerEx : can not read next file for decompressing\n");
				unzClose(_zipFile);
				return false;
			}
		}
	}

	unzClose(_zipFile);
	return true;
}
static void calculate_sha256(Data &data, string &result_string)
{
	Sha256Context   sha256Context;
	SHA256_HASH     sha256Hash;
	Sha256Initialise(&sha256Context);
	Sha256Update(&sha256Context, data.getBytes(), data.getSize());
	Sha256Finalise(&sha256Context, &sha256Hash);
	result_string.clear();
	for(int i = 0; i < sizeof(sha256Hash); i++)
	{
		char tmp[3];
		snprintf(tmp, 3, "%02x", sha256Hash.bytes[i]);
		result_string.push_back(tmp[0]);
		result_string.push_back(tmp[1]);
	}
}
static void post_json_to_url(string url, json &postData, std::function<void(bool success, json result)>callback)
{
	auto postDataString = postData.dump();
	CCLOG("%s", postDataString.c_str());
	HttpRequest* request = new HttpRequest();
	request->setUrl(url);
	auto headers = vector<string>();
	headers.push_back("Content-Type:application/json");
	request->setHeaders(headers);
	request->setRequestData(postDataString.c_str(), postDataString.length());
	request->setRequestType(HttpRequest::Type::POST);
	request->setResponseCallback([callback](HttpClient* client, HttpResponse* response)
	{
		auto resultData = response->getResponseData();
		auto resultString = string(resultData->begin(), resultData->end());
		CCLOG("Callback %ld %s", response->getResponseCode(), resultString.c_str());
		callback(response->isSucceed(), response->isSucceed() ? json::parse(resultString) : json());
	});
	HttpClient::getInstance()->send(request);
	request->release();
}
