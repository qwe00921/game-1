//
//  MiniSupport.cpp
//  TRGame
//
//  Created by Long on 17/3/7.
//
//

#include "MiniSupport.h"
#include <string.h>
#include <stdio.h>

#ifdef GAME_RES_MINI
	#include "Common.h"
#endif
const char* MINI_SUPPORT_DOWNLOAD_COMPLETE = "MINI_SUPPORT_DOWNLOAD_COMPLETE";
MiniImgDownloadWorker* s_downloaderWorker = nullptr;
TrImageRefreshWorker* s_imageRefreshWorker = nullptr;
std::string s_internalPath;
std::string s_gamePath;
typedef std::map<std::string, std::string> StringMap;
typedef StringMap::iterator SMI;
StringMap downloadingFiles;

using namespace cocos2d::network;

void downFile(const char* file, int is_writeable = 0, int is_globe = 1) {
	CCLOG("download %s", file);
	s_downloaderWorker->downloadFile(file, is_writeable, is_globe);
}

bool MiniImgDownloadWorker::init() {
	fileUtils = FileUtils::getInstance();

	miniPatchDir = fileUtils->getWritablePath() + "minipatch_dir/";

	downloadMiniPatchUrlBase = "https://longzqtx.leanapp.cn/files/";

	fileDownloader = std::shared_ptr<network::Downloader>(new network::Downloader);

	fileDownloader->onTaskError = [this](const DownloadTask& task,
		int errorCode,
		int errorCodeInternal,
		const std::string& errorStr)
	{
		CCLOG("Download error %d %d %s url %s", errorCode, errorCodeInternal, errorStr.c_str(),task.requestURL.c_str());
		SMI smi = downloadingFiles.find(task.requestURL);
		if (smi != downloadingFiles.end()){
			downloadingFiles.erase(smi);
		}
		std::string storagePath = task.storagePath;
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([&, this, storagePath] {
			fileUtils->removeFile(storagePath.c_str());
		});
	};

	fileDownloader->onFileTaskSuccess = [this](const DownloadTask& task)
	{
		SMI smi = downloadingFiles.find(task.requestURL);
		if (smi != downloadingFiles.end()) {
			downloadingFiles.erase(smi);
		}
		std::string identifier = task.identifier;
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([&, this, identifier] {
			char* buf = new char[255];
			sprintf(buf, "%s", identifier.c_str());
			EventCustom event(MINI_SUPPORT_DOWNLOAD_COMPLETE);
			event.setUserData(buf);
			Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
			CC_SAFE_DELETE_ARRAY(buf);
		});
	};


	return true;
}


bool MiniImgDownloadWorker::isDownloading(const char* file, int is_writeable, int is_globe) {
#ifdef GAME_RES_MINI
	std::string url = this->getDownloadUrl(file, is_writeable, is_globe);

	if (downloadingFiles.find(url) != downloadingFiles.end()) {
		return true;
	}
#endif
	return false;
}

const char* MiniImgDownloadWorker::getDownloadUrl(const char* file, int is_writeable, int is_globe) {
	std::string url = downloadMiniPatchUrlBase;
#ifdef GAME_RES_MINI
	std::string fileSign = getMiniResourceRelativeFile(file, is_writeable, is_globe);
	url = url + fileSign;
#endif
	return url.c_str();
}

void MiniImgDownloadWorker::downloadFile(const char* file, int is_writeable, int is_globe)
{
#ifdef GAME_RES_MINI
	std::string url = this->getDownloadUrl(file, is_writeable, is_globe);
	std::string fileSign = getMiniResourceRelativeFile(file, is_writeable, is_globe);

	if (downloadingFiles.find(url) == downloadingFiles.end()) {
		downloadingFiles.insert(std::make_pair(url, url));
		fileDownloader->createDownloadFileTask(url, miniPatchDir + fileSign, fileSign);
	}
#endif
}

void MiniSupport::initWithPath(const std::string& gamePath, const std::string& writeAblePath) {
	s_gamePath = gamePath + "/";
	s_internalPath = writeAblePath;
	//只能调用一次
	assert(s_downloaderWorker == nullptr);
	s_downloaderWorker = new MiniImgDownloadWorker();
	s_downloaderWorker->init();

	s_imageRefreshWorker = new TrImageRefreshWorker();
	s_imageRefreshWorker->init();
}

Sprite* MiniSupport::makeRemoteSprite(const char* imgFile, Sprite* sp) {
#ifdef GAME_RES_MINI
	auto fileUtils = FileUtils::getInstance();
	auto fullPathFile = fileUtils->fullPathForFilename(imgFile);
	auto textureCache = Director::getInstance()->getTextureCache();
	if (sp == nullptr) {
		if (fullPathFile.find("minipatch_dir/") != std::string::npos) {
			//网络异常可能上次下载的图片不对
			fileUtils->removeFile(fullPathFile);
			textureCache->removeTextureForKey(imgFile);
			sp = CCSprite::create(imgFile);
			if (sp == nullptr) {
				assert(false);
			}
		}
		else {
			assert(false);//给的图包里没有
			return sp;
		}
	}
	std::string orignPath = sp->getTexture()->getPath().c_str();

	if (fullPathFile.find("patch_dir/") == std::string::npos) {
		if (sp) {
			auto dispatcher = Director::getInstance()->getEventDispatcher();
			auto listener = EventListenerCustom::create(MINI_SUPPORT_DOWNLOAD_COMPLETE, [=](EventCustom* event) {
				char* userData = static_cast<char*>(event->getUserData());

				std::string fileSign;
				if (fullPathFile.find(s_gamePath) != std::string::npos || orignPath.find(s_gamePath) != std::string::npos) {
					fileSign = getMiniResourceRelativeFile(imgFile, 0, 3);
				}
				else {
					fileSign = getMiniResourceRelativeFile(imgFile, 0, 1);
				}
				if (fileSign.compare(userData) == 0) {
					textureCache->reloadTexture(imgFile);
					sp->setTexture(imgFile);
					dispatcher->removeEventListenersForTarget(sp);
				}
			});
			dispatcher->addEventListenerWithSceneGraphPriority(listener, sp);

			downFile(imgFile);
		}
		
	}
	else {//已经更新过资源的不需要监听文件下载完成事件
		if (fullPathFile.compare(orignPath) != 0) {
			textureCache->reloadTexture(imgFile);
			sp->setTexture(imgFile);
		}
	}
#endif
	return sp;
}


void MiniSupport::checkMiniTRImagePtr(const char *filename, int is_writeable, int is_globe) {
	auto fileUtils = FileUtils::getInstance();

	if (!(MiniSupport::isDownloading(filename))) {
		return;
	}

	CCLOG("checkMiniTRImagePtr:%s is_writeable:%d is_globe:%d", filename, is_writeable, is_globe);

	auto fullPathFile = fileUtils->fullPathForFilename(filename);
	if (fullPathFile.find("patch_dir/") == std::string::npos) {
		if (!fileUtils->isFileExist(fullPathFile)) {//TODO 需要热更新的图片,目前暂认为没在更新目录就去下载
			downFile(filename, is_writeable,is_globe);
		}
	}
}
void MiniSupport::checkMiniTRPacketImage(const char *filename, int is_writeable, int is_globe) {
	MiniSupport::checkMiniTRImagePtr(filename, is_writeable, is_globe);
}


bool MiniSupport::isMiniSupportExt(const char* fileName) {
	auto fileUtils = FileUtils::getInstance();
	std::string ext = fileUtils->getFileExtension(fileName);
	return ext == ".png" || ext == ".jpg";
}

bool MiniSupport::isDownloading(const char* fileName, int is_writeable, int is_globe) {
	return s_downloaderWorker->isDownloading(fileName, is_writeable, is_globe);
}

void MiniSupport::Destroy(){
	Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(MINI_SUPPORT_DOWNLOAD_COMPLETE);
	CC_SAFE_DELETE(s_downloaderWorker);
	CC_SAFE_DELETE(s_imageRefreshWorker);
}

//放进下载完毕实时刷新通知列表
void MiniSupport::pushPtr(const char* key, TRImagePtr imagePtr) {
	s_imageRefreshWorker->pushPtr(key,imagePtr);
}

//从下载完毕实时刷新通知列表中移除
void MiniSupport::popPtr(const char* key) {
	s_imageRefreshWorker->popPtr(key);
}


void MiniSupport::doRefreshTRImagePtr(const char* key) {
	s_imageRefreshWorker->doRefreshTRImagePtr(key);
}

////////////////////////TrImageRefreshWorker
bool TrImageRefreshWorker::init() {
	Director::getInstance()->getEventDispatcher()->addCustomEventListener(MINI_SUPPORT_DOWNLOAD_COMPLETE, [](EventCustom *event) {
		const char* userData = static_cast<const char*>(event->getUserData());
		MiniSupport::doRefreshTRImagePtr(userData);
	});
	return true;
}

//刷新TRImagePtr
void TrImageRefreshWorker::doRefreshTRImagePtr(const char* key) {

	TRPMapI f = imagePtrMap.find(key);
	if (f != imagePtrMap.end()) {
		//f->second->Create(key,true);
		CCLOG("TODO refresh TRImagePtr %s", key);
		imagePtrMap.erase(f);
	}
}


void TrImageRefreshWorker::pushPtr(const char* key, TRImagePtr imagePtr) {
	imagePtrMap.insert(std::make_pair(key, imagePtr));
}
void TrImageRefreshWorker::popPtr(const char* key) {
	TRPMapI f = imagePtrMap.find(key);
	if (f != imagePtrMap.end()) {
		imagePtrMap.erase(f);
	}
}