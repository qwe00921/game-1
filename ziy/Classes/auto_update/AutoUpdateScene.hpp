#ifndef AutoUpdateScene_hpp
#define AutoUpdateScene_hpp

#include "Defines.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "AutoUpdateWorker.hpp"

class AutoUpdateScene : public BTLayer, public AutoUpdateWorkerDelegate
{
public:
	// implement the "static create()" method manually
	CREATE_FUNC(AutoUpdateScene);
	virtual bool init(void) override;
protected:
    AutoUpdateWorker * worker;
	virtual void onEnter() override;
	virtual void onExit() override;
    virtual void update(float delta) override;
	virtual void callBackConfirm();

    virtual void checkUpdateCB(int size) override;
    virtual void updateProgressingCB(float progress) override;
    virtual void updateResultCB(bool result) override;
    double loadingBarChangeDeltaTime;
    double lastLoadingBarChangeTime;
    float loadingPercent; //destination value
    float currentLoadingPercent;
    float oldLoadingPercent;
public:
    virtual float getLodingPercent(void) const { return loadingPercent; }
    virtual void setLodingPercent(float var);
private:
	float getTotalSize(float fSize);
	void updatePrecent();
private:
	Sprite* m_spBarBG;
	Sprite* m_spLoading;
	Label* m_labPercent;
	float m_fTotalSize;
};

#endif /* AutoUpdateScene_hpp */
