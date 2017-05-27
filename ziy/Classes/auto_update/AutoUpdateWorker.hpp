#ifndef AutoUpdateWorker_hpp
#define AutoUpdateWorker_hpp

#include "cocos2d.h"
#include "network/CCDownloader.h"
#include "platform/CCFileUtils.h"
#include "tools/json.hpp"
#include "Defines.h"


class AutoUpdateWorkerDelegate
{
public:
	virtual void checkUpdateCB(int size) = 0;
	virtual void updateProgressingCB(float progress) = 0;
	virtual void updateResultCB(bool result) = 0;
};

class AutoUpdateWorker : public cocos2d::Ref
{
public:
	CREATE_FUNC(AutoUpdateWorker);
	virtual bool init();
	virtual void checkUpdate();
	virtual void downloadUpdate();
	virtual int getPatchVersion();
private:
	std::shared_ptr<cocos2d::network::Downloader> fileDownloader;
	bool decompress(const std::string &zip, const std::string &dest_dir);
	std::string basename(const std::string& path) const;
	void tryDelete();
	cocos2d::FileUtils * _fileUtils;
	cocos2d::UserDefault * _userDefault;
	cocos2d::Scheduler * _scheduler;
	std::string _zipFile;
	std::string _patchInfoFile;
	std::string _patchDir;
	std::string _testDir;
	std::string _checkUpdateUrl;
	std::string _downloadPatchUrlBase;
	nlohmann::json _patchCheckResult;
	CC_SYNTHESIZE(AutoUpdateWorkerDelegate *, delegator, Delegator);
};

#endif /* AutoUpdateWorker_hpp */
