#include "ActionLayer.h"
#include "Defines.h"


bool ActionLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	mAddArmNum = 0;
	mReduceArmNum = 0;
	return true;
}
void ActionLayer::LoadAnimationFile()
{
	for (int i = 0; i < trData->animation_info_num; i++)
	{
		__String* arm_mame = __String::createWithFormat("skill_effect/%s.ExportJson", trData->animation_info[i].name);
		ArmatureDataManager::getInstance()->addArmatureFileInfo(getRespath(arm_mame->getCString(), 0, 1));
	}
}
void ActionLayer::RunAction(int skill_anim_id, int m_skill_type, HistoryActorPtr ptrCurActor, HistoryActorPtr ptrOppoActor, const string& ptrArmName, bool bIsOwn)
{
	if (ptrCurActor && ptrCurActor->m_pArmSkill) //技能动画播放中
	{
		return;
	}

	__String* armFileName = NULL;
	__String* armNameCs = NULL;
	__String* armPlayName = NULL;
	bool bFileX = false, bFileY = false;
	int iZorder = 1;
	int iRotate = 0;

	if (ptrArmName != "")
	{
		armFileName = __String::createWithFormat("skill_effect/%s.ExportJson", ptrArmName.c_str());
		armNameCs = __String::create(ptrArmName);
		armPlayName = armNameCs;
		iZorder = 0;
	}
	else
	{
		armFileName = __String::createWithFormat("skill_effect/skill_%d.ExportJson", skill_anim_id);
		armNameCs = __String::createWithFormat("skill_%d", skill_anim_id);
		armPlayName = getAnimationPlayId(ptrCurActor, ptrOppoActor, skill_anim_id, m_skill_type, bFileX, bFileY, iRotate);
	}

	ArmatureDataManager::getInstance()->addArmatureFileInfo(getRespath(armFileName->getCString(), 0, 1));

	Armature* ptrArmature = Armature::create(armNameCs->getCString());
	if (ptrCurActor)
	{
		ptrCurActor->m_pArmSkill = ptrArmature;
	}
	if (!bIsOwn)
		ptrArmature->setPosition(Vec2(ptrOppoActor->xx() - history->scr_x + 24, screen_size_y - (ptrOppoActor->yy() - history->scr_y + 24)));
	else
		ptrArmature->setPosition(Vec2(ptrCurActor->xx() - history->scr_x + 24, screen_size_y - (ptrCurActor->yy() - history->scr_y + 24)));
	ptrArmature->getAnimation()->play(armPlayName->getCString());
	ptrArmature->setTag(mAddArmNum);
	ptrArmature->getAnimation()->setMovementEventCallFunc([=](Armature* armature, MovementEventType type, const std::string& id)
	{
		if (type == COMPLETE)
		{
			int iArmTag = armature->getTag();
			ArmElem[iArmTag]->setISEND(1);
			armature->removeFromParent();
			mReduceArmNum++;
			if(mReduceArmNum >= mAddArmNum)
			{
				mReduceArmNum = 0;
				mAddArmNum = 0;
				history->m_iRunAction = 0;
			}
			if(ptrCurActor)
			{
				ptrCurActor->m_pArmSkill = nullptr;
			}
		}
	});

	if (iRotate)
		ptrArmature->setRotation(iRotate * 45);

	if (bFileX)
		ptrArmature->setScaleX(-1);
	else if (bFileY)
		ptrArmature->setScaleY(-1);

	history->m_iRunAction = 1;
	this->addChild(ptrArmature, iZorder);

	if (ArmElem.count(mAddArmNum))
	{
		ArmElem[mAddArmNum]->setISEND(0);
		if (!bIsOwn)
			ArmElem[mAddArmNum]->setACTOR(ptrOppoActor);
		else
			ArmElem[mAddArmNum]->setACTOR(ptrCurActor);
		ArmElem[mAddArmNum]->setARM(ptrArmature);
	}
	else
	{
		ActionElem* ptrElem = new ActionElem();
		ptrElem->setISEND(0);
		if (!bIsOwn)
			ptrElem->setACTOR(ptrOppoActor);
		else
			ptrElem->setACTOR(ptrCurActor);
		ptrElem->setARM(ptrArmature);
		ArmElem[mAddArmNum] = ptrElem;
	}
	mAddArmNum++;
}

void ActionLayer::ScriptRunAction(int skill_anim_id,int m_skill_type, int iPointX, int iPointY)
{
	history->CenterPos(iPointX, iPointY);
	__String* armFileName = NULL;
	__String* armNameCs = NULL;
	__String* armPlayName = NULL;

	int iZorder = 1;

	armFileName = __String::createWithFormat("skill_effect/skill_%d.ExportJson", skill_anim_id);
	armNameCs = __String::createWithFormat("skill_%d", skill_anim_id);

	if (m_skill_type == 3)
		armPlayName = __String::createWithFormat("skill_%d_%d", skill_anim_id, 1);
	else
		armPlayName = __String::createWithFormat("skill_%d", skill_anim_id);

	ArmatureDataManager::getInstance()->addArmatureFileInfo(getRespath(armFileName->getCString(), 0, 1));

	Armature* ptrArmature = Armature::create(armNameCs->getCString());

	ptrArmature->setPosition(Vec2(iPointX*SCENE_TILE_WIDTH - history->scr_x + 24, screen_size_y - (iPointY*SCENE_TILE_WIDTH - history->scr_y + 24)));

	ptrArmature->getAnimation()->play(armPlayName->getCString());

	ptrArmature->getAnimation()->setMovementEventCallFunc([=](Armature* armature, MovementEventType type, const std::string& id)
	{
		if (type == COMPLETE)
		{
			armature->removeFromParent();
		}
	});
	this->addChild(ptrArmature, iZorder);
}

void ActionLayer::ScriptRunBossHideAction(int skill_anim_id, int m_skill_type, int iActorId,int* iPtrBossHide)
{
	__String* armFileName = NULL;
	__String* armNameCs = NULL;
	__String* armPlayName = NULL;
	m_iPtrBossHide = iPtrBossHide;

	int iZorder = 1;

	armFileName = __String::createWithFormat("skill_effect/skill_%d.ExportJson", skill_anim_id);
	armNameCs = __String::createWithFormat("skill_%d", skill_anim_id);

	if (m_skill_type == 3)
		armPlayName = __String::createWithFormat("skill_%d_%d", skill_anim_id, 1);
	else
		armPlayName = __String::createWithFormat("skill_%d", skill_anim_id);

	ArmatureDataManager::getInstance()->addArmatureFileInfo(getRespath(armFileName->getCString(), 0, 1));

	Armature* ptrArmature = Armature::create(armNameCs->getCString());

	ptrArmature->setPosition(Vec2(screen_size_x / 2, screen_size_y / 2));

	ptrArmature->getAnimation()->play(armPlayName->getCString());

	ptrArmature->getAnimation()->setMovementEventCallFunc([=](Armature* armature, MovementEventType type, const std::string& id)
	{
		if (type == COMPLETE)
		{
			Sprite* BossSp = (Sprite*)armature->getChildByTag(100);

			if (BossSp)
			{
				BossSp->setVisible(true);
				DelayTime* ptrDelayTime = DelayTime::create(2);
				CallFunc* funcall = CallFunc::create(CC_CALLBACK_0(ActionLayer::callback,this));
				Sequence* ptrSequence = Sequence::create(ptrDelayTime, funcall, NULL);
				armature->runAction(ptrSequence);
			}
			else
			{

				if (m_iPtrBossHide)
				{
					*m_iPtrBossHide = 0;
					m_iPtrBossHide = NULL;
				}
				armature->removeFromParent();
			}

		}
	});
	this->addChild(ptrArmature, iZorder);
	ptrArmature->setTag(101);

	HistoryActorPtr ptrBossActor = &history->actors[iActorId - 1];
	Sprite* bossSp = ReadSpriteNormal(__String::createWithFormat("img/Face/%d.png", ptrBossActor->face_id+1)->getCString());
	bossSp->setAnchorPoint(Vec2(0.5,0));
	bossSp->setPosition(Vec2(210,-122));
	bossSp->setScale(0.8f);
	bossSp->setFlippedX(true);
	bossSp->setTag(100);
	ptrArmature->addChild(bossSp,10);
	bossSp->setVisible(false);

	Sprite* nameBgSp = ReadSpriteName("new_ui/battle/battle_tsbox2.png");
	nameBgSp->setAnchorPoint(Vec2(0.5, 0));
	nameBgSp->setPosition(Vec2(bossSp->getContentSize().width/2,0));
	bossSp->addChild(nameBgSp);

	Label* namelab = LabelSystemFont(trData->actors[iActorId - 1].getActorName(),32);
	namelab->setPosition(Vec2(nameBgSp->getContentSize().width/2, nameBgSp->getContentSize().height/2));
	namelab->setColor(Color3B(252, 226, 162));
	namelab->enableOutline(Color4B(255,0,0,100),3);
	nameBgSp->addChild(namelab);
}

void ActionLayer::callback()
{
	if (m_iPtrBossHide)
	{
		*m_iPtrBossHide = 0;
		m_iPtrBossHide = NULL;
	}
	Armature* ArmatureSp = (Armature*)this->getChildByTag(101);
	ArmatureSp->removeFromParent();
}

void ActionLayer::SetActionPosition()
{
	for (int i = 0; i < mAddArmNum; i++)
	{
		if (!ArmElem[i]->getISEND())
		{
			HistoryActorPtr ptrActor = ArmElem[i]->getACTOR();
			ArmElem[i]->getARM()->setPosition(Vec2(ptrActor->xx() - history->scr_x + 24, screen_size_y - (ptrActor->yy() - history->scr_y + 24)));
		}
	}
}

void ActionLayer::stopSkillAction(HistoryActor* ptrActor)
{
	if (ptrActor->m_pArmSkill)
	{
		int iArmTag = ptrActor->m_pArmSkill->getTag();
		ArmElem[iArmTag]->setISEND(1);
		ptrActor->m_pArmSkill->removeFromParent();
		mReduceArmNum++;
		if(mReduceArmNum >= mAddArmNum)
		{
			mReduceArmNum = 0;
			mAddArmNum = 0;
			history->m_iRunAction = 0;
		}
		ptrActor->m_pArmSkill = nullptr;
	}
}

__String* ActionLayer::getAnimationPlayId(HistoryActorPtr ptrCurActor, HistoryActorPtr ptrOppoActor, int skill_anim_id, int m_skill_type, bool &bFlipX, bool &bFlipY, int &iRotate)
{
	switch (m_skill_type)
	{
	case 1:
		return __String::createWithFormat("skill_%d", skill_anim_id);
		break;
	case 2:
	{
			  int offsetX = ptrCurActor->x - ptrOppoActor->x;
			  int offsetY = ptrCurActor->y - ptrOppoActor->y;

			  if (offsetX<0&&offsetY<0)
				  iRotate = 1;
			  else if (!offsetX && offsetY < 0)
				  iRotate = 2;
			  else if (offsetX > 0 && offsetY < 0)
				  iRotate = 3;
			  else if (offsetX > 0 && !offsetY)
				  iRotate = 4;
			  else if (offsetX > 0 && offsetY>0)
				  iRotate = 5;
			  else if (!offsetX && offsetY > 0)
				  iRotate = 6;
			  else if (offsetX <0&& offsetY > 0)
				  iRotate = 7;

		return __String::createWithFormat("skill_%d", skill_anim_id);
	}
		break;
	case 3:
	{
			  int iFlip = 1;
			  if (ptrCurActor->direction == 0)
			  {
				  iFlip = 2;
			  }
			  else if (ptrCurActor->direction == 1)
			  {
				  iFlip = 1;
			  }
			  else if (ptrCurActor->direction == 2)
			  {
				  iFlip = 2;
				  bFlipY = true;
			  }
			  else if (ptrCurActor->direction == 3)
			  {
				  iFlip = 1;
				  bFlipX = true;
			  }
			  return __String::createWithFormat("skill_%d_%d", skill_anim_id, iFlip);
	}
		break;
	}
	return NULL;
}

ActionLayer::ActionLayer()
{

}

ActionLayer::~ActionLayer()
{

}