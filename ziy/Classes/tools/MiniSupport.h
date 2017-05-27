//
//  MiniSupport.h
//  TRGame
//
//  Created by Long on 17/3/7.
//
//

#ifndef MiniSupport_h
#define MiniSupport_h
#include "cocos2d.h"
#include "network/CCDownloader.h"
#include "Image.h"
USING_NS_CC;
/************************************************************************/
/* 暂时支持png,jpg
*/
/************************************************************************/
class MiniSupport 
{
public:
	//ReadSpriteName接口使用的图片支持更新下载
	static Sprite* makeRemoteSprite(const char* imgFile, Sprite* sp);

	//ChessReadFile接口使用的TRImage支持更新下载
	static void checkMiniTRImagePtr(const char *filename, int is_writeable, int is_globe);

	//检查大图是否需要更新下载
	static void checkMiniTRPacketImage(const char *filename, int is_writeable, int is_globe);


	//文件后缀是否支持
	static bool isMiniSupportExt(const char* fileName);

	//这个文件是否正在下载
	static bool isDownloading(const char* fileName, int is_writeable = 0, int is_globe = 1);

	//放进下载完毕实时刷新通知列表
	static void pushPtr(const char* key, TRImagePtr imagePtr);

	//从下载完毕实时刷新通知列表中移除
	static void popPtr(const char* key);

	//刷新TRImagePtr
	static void doRefreshTRImagePtr(const char* key);



	MiniSupport() { assert(false && "do not alloc me"); };
	//初始化一次
	static void initWithPath(const std::string& gamePath, const std::string& writeAblePath);
	//worker 清理
	static void Destroy();
};

class MiniImgDownloadWorker
{
public:
	virtual bool init();
	void downloadFile(const char* file, int is_writeable = 0, int is_globe = 1);
	const char* getDownloadUrl(const char* file, int is_writeable = 0, int is_globe = 1);
	//这个文件是否正在下载
	bool isDownloading(const char* file, int is_writeable = 0, int is_globe = 1);
private:
	FileUtils * fileUtils;
	std::string miniPatchDir;
	std::string downloadMiniPatchUrlBase;
	std::shared_ptr<network::Downloader> fileDownloader;
};

class TrImageRefreshWorker
{
	typedef std::map<std::string, TRImagePtr> TRImagePtrMap;
	typedef TRImagePtrMap::iterator TRPMapI;
public:
	void pushPtr(const char* key, TRImagePtr imagePtr);
	void popPtr(const char* key);
	//刷新TRImagePtr
	void doRefreshTRImagePtr(const char* key);
	virtual bool init();
private:
	TRImagePtrMap imagePtrMap;

};
#endif /* MiniSupport_h */
