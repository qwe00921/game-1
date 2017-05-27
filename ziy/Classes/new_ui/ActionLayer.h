#ifndef __TRGAME_ACTION_LAYER__
#define  __TRGAME_ACTION_LAYER__
#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "Common.h"
#include "main.h"
#include "editor-support/cocostudio/CCArmature.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace cocostudio;

class ActionElem
{
	CC_SYNTHESIZE(int, mIsEnd, ISEND);
	CC_SYNTHESIZE(HistoryActorPtr,mActor,ACTOR);
	CC_SYNTHESIZE(Armature*, mArm, ARM);
};


class ActionLayer :public Layer
{
public:
	CREATE_FUNC(ActionLayer);
	bool init();
	ActionLayer();
	~ActionLayer();

	void RunAction(int skill_anim_id, int m_skill_type, HistoryActorPtr ptrCurActor,HistoryActorPtr ptrOppoActor, const string& ptrArmName = NULL, bool bIsOwn = false);
	void ScriptRunAction(int skill_anim_id,int m_skill_type,int iPointX, int iPointY);
	void ScriptRunBossHideAction(int skill_anim_id, int m_skill_type, int iActorId,int* iPtrBossHide = NULL);
	void SetActionPosition();
	void stopSkillAction(HistoryActor* ptrActor);
	__String* getAnimationPlayId(HistoryActorPtr ptrCurActor, HistoryActorPtr ptrOppoActor, int skill_anim_id, int m_skill_type, bool &bFlipX, bool &bFlipY, int &iRotate);
	int frame;
	void LoadAnimationFile();
	map<int, ActionElem*> ArmElem;
	int mAddArmNum;
	int mReduceArmNum;

	void callback();

private:
	int* m_iPtrBossHide;
};
#endif //(__TRGAME_ACTION_LAYER__)