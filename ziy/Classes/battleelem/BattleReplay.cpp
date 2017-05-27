//
//  BattleReplay.cpp
//  TRGame
//
//  Created by Jimmy on 17/03/02.
//
//

#include "Common.h"
#include "main.h"
#include "BattleReplay.h"
#include "otherfunction.h"

#define FILE_BATTLE_REPKAY "replays/"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ReplayActor::ReplayActor()
{
	m_iActorId = 0;
	m_iBaseHP = 0;
	m_iAddHP = 0;
	m_iBaseMP = 0;
	m_iAddMP = 0;
	m_iLevel = 0;

	m_iInitBaseHP = 0;
	m_iInitAddHP = 0;
	m_iInitBaseMP = 0;
	m_iInitAddMP = 0;
	m_iInitLv = 0;
}

ReplayActor::~ReplayActor()
{

}

void ReplayActor::initActor(HistoryActor* ptrActor)
{
	m_iActorId = ptrActor->actor_id;
	m_iBaseHP = ptrActor->GetBaseHp();
	m_iAddHP = ptrActor->GetGoodsAddHp();
	m_iBaseMP = ptrActor->GetBaseMp();
	m_iAddMP = ptrActor->GetGoodsAddMp();
	m_iLevel = ptrActor->GetActorLv();
}

void ReplayActor::setActor()
{
	HistoryActor* ptrActor = &history->actors[m_iActorId];
	m_iInitBaseHP = ptrActor->GetBaseHp();
	m_iInitAddHP = ptrActor->GetGoodsAddHp();
	m_iInitBaseMP = ptrActor->GetBaseMp();
	m_iInitAddMP = ptrActor->GetGoodsAddMp();
	m_iInitLv = ptrActor->GetActorLv();

	ptrActor->SetBaseHp(m_iBaseHP);
	ptrActor->SetGoodsAddHp(m_iAddHP);
	ptrActor->SetBaseMp(m_iBaseMP);
	ptrActor->SetGoodsAddMp(m_iAddMP);
	ptrActor->SetActorLv(m_iLevel);
	ptrActor->SetCurHp(ptrActor->getTotalAbility(TOTAL_HP));
	ptrActor->SetCurMp(ptrActor->getTotalAbility(TOTAL_MP));
}

void ReplayActor::resetActor()
{
	HistoryActor* ptrActor = &history->actors[m_iActorId];
	ptrActor->SetBaseHp(m_iInitBaseHP);
	ptrActor->SetGoodsAddHp(m_iInitAddHP);
	ptrActor->SetBaseMp(m_iInitBaseMP);
	ptrActor->SetGoodsAddMp(m_iInitAddMP);
	ptrActor->SetActorLv(m_iInitLv);
	ptrActor->SetCurHp(ptrActor->getTotalAbility(TOTAL_HP));
	ptrActor->SetCurMp(ptrActor->getTotalAbility(TOTAL_MP));
}

int ReplayActor::getActorId()
{
	return m_iActorId;
}

void ReplayActor::readActor(DMemoryReader* reader)
{
	reader->tryReadInt(m_iActorId);
	reader->tryReadInt(m_iBaseHP);
	reader->tryReadInt(m_iAddHP);
	reader->tryReadInt(m_iBaseMP);
	reader->tryReadInt(m_iAddMP);
	reader->tryReadInt(m_iLevel);
	setActor();
}

void ReplayActor::saveActor(DMemoryWriter* writer)
{
	writer->writeInt(m_iActorId);
	writer->writeInt(m_iBaseHP);
	writer->writeInt(m_iAddHP);
	writer->writeInt(m_iBaseMP);
	writer->writeInt(m_iAddMP);
	writer->writeInt(m_iLevel);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ReplayRound::ReplayRound()
{
	m_iCurAir = 0;
}

ReplayRound::~ReplayRound()
{

}

void ReplayRound::readRound(DMemoryReader* reader)
{
	reader->tryReadInt(m_iCurAir);

}

void ReplayRound::saveRound(DMemoryWriter* writer)
{
	writer->writeInt(m_iCurAir);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BattleReplay* BattleReplay::m_pInstance = nullptr;

BattleReplay* BattleReplay::getInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new BattleReplay();
	}
	return m_pInstance;
}

BattleReplay::BattleReplay()
{
	m_iStatus = REPLAY_STATUS_NONE;

	m_iScriptIndex = 0;
	m_iAIIndex = 0;
	m_iGainIndex = 0;
}

string BattleReplay::getFileName()
{
	return StringUtils::format("%sreplay%d.bin", FILE_BATTLE_REPKAY, m_iScriptIndex);
}

BattleReplay::~BattleReplay()
{

}

bool BattleReplay::loadReplay()
{
	unsigned long ulFileSize = 0;
	char* chData = ChessReadFile(getFileName().c_str(), 1, 0, &ulFileSize);
	if (chData == nullptr)
	{
		return false;
	}

	DMemoryReader* ptrReader = new DMemoryReader(chData, (int)ulFileSize, true);
	//版本号
	ptrReader->tryReadString(m_strVersion);
	//出战队列
	int iActorSize = 0;
	ptrReader->tryReadInt(iActorSize);
	for (int i = 0; i < iActorSize; i++)
	{
		ReplayActor* ptrActor = new ReplayActor();
		ptrActor->readActor(ptrReader);
		m_mActor[i] = ptrActor;
	}
	//AI
	int iAISize = 0;
	ptrReader->tryReadInt(iAISize);
	for (int i = 0; i < iAISize; i++)
	{
		BattleAI* ptrAI = new BattleAI();
		ptrAI->readBattleAI(ptrReader);
		m_mBattleAI[i] = ptrAI;
	}
	//收益
	int iGainSize = 0;
	ptrReader->tryReadInt(iGainSize);
	for (int i = 0; i < iGainSize; i++)
	{
		ActionGain* ptrGain = new ActionGain();
		ptrGain->readGain(ptrReader);
		m_mActionGain[i] = ptrGain;
	}
	//回合数据
	int iRoundSize = 0;
	ptrReader->tryReadInt(iRoundSize);
	for (int i = 0; i < iRoundSize; i++)
	{
		int iRound = 0;
		ptrReader->tryReadInt(iRound);
		ReplayRound* ptrRound = new ReplayRound();
		ptrRound->readRound(ptrReader);
		m_mRepRound[iRound] = ptrRound;
	}
	SAFE_DELETE(ptrReader);
	return true;
}

void BattleReplay::saveReplay()
{
	DMemoryWriter* ptrWriter = new DMemoryWriter();

	//版本号
	string strVersion = statGetDeviceParams(StatParamVersion)->getChar();
	ptrWriter->writeString(strVersion);
	//出战队列
	int iActorSize = m_mActor.size();
	ptrWriter->writeInt(iActorSize);
	for (int i = 0; i < iActorSize; i++)
	{
		m_mActor[i]->saveActor(ptrWriter);
	}
	//AI
	ptrWriter->writeInt(m_iAIIndex);
	ptrWriter->writeCharArray(m_dwBattleAI->getDataAddr(), m_dwBattleAI->getDataLength());
	//收益
	ptrWriter->writeInt(m_iGainIndex);
	ptrWriter->writeCharArray(m_dwActionGain->getDataAddr(), m_dwActionGain->getDataLength());
	//回合数据
	int iRoundSize = m_mRepRound.size();
	ptrWriter->writeInt(iRoundSize);
	for (map<int, ReplayRound*>::iterator it = m_mRepRound.begin(); it != m_mRepRound.end(); it++)
	{
		ptrWriter->writeInt(it->first);
		it->second->saveRound(ptrWriter);
	}

	ChessWriteFile(getFileName().c_str(), ptrWriter->getDataAddr(), ptrWriter->getDataLength());

	SAFE_DELETE(m_dwBattleAI);
	SAFE_DELETE(m_dwActionGain);
	SAFE_DELETE(ptrWriter);
}

void BattleReplay::startSaveReplay()
{
	m_iStatus = REPLAY_STATUS_SAVE;
	m_dwBattleAI = new DMemoryWriter();
	m_dwActionGain = new DMemoryWriter();

	m_iAIIndex = 0;
	m_iGainIndex = 0;
}

void BattleReplay::saveBattleAI(BattleAI* ai)
{
	m_iAIIndex++;
	ai->saveBattleAI(m_dwBattleAI);
}

void BattleReplay::saveActionGain(ActionGain* gain)
{
	m_iGainIndex++;
	gain->saveGain(m_dwActionGain);
}

void BattleReplay::stopSaveReplay()
{
	if (m_iStatus == REPLAY_STATUS_SAVE)
	{
		saveReplay();
		m_iStatus = REPLAY_STATUS_NONE;
	}
}

void BattleReplay::saveBattleActor(map<int, int> mActorId)
{
	for (UInt i = 0; i < mActorId.size(); i++)
	{
		HistoryActor* ptrActor = &history->actors[mActorId[i]];
		ReplayActor* repActor = new ReplayActor();
		repActor->initActor(ptrActor);
		m_mActor[i] = repActor;
	}
}

std::map<int, int> BattleReplay::getBattleActor()
{
	map<int, int> mActorId;
	for (UInt i = 0; i < m_mActor.size(); i++)
	{
		mActorId[i] = m_mActor[i]->getActorId();
	}
	return mActorId;
}

void BattleReplay::startReplay()
{
	loadReplay();
	m_iAIIndex = 0;
	m_iGainIndex = 0;
}

BattleAI* BattleReplay::getBattleAI(bool move /*= true*/)
{
	if (move)
	{
		return m_mBattleAI[m_iAIIndex++];
	}
	else
	{
		return m_mBattleAI[m_iAIIndex];
	}
}

ActionGain* BattleReplay::getActionGain(bool move)
{
	if (move)
	{
		return m_mActionGain[m_iGainIndex++];
	}
	else
	{
		return m_mActionGain[m_iGainIndex];
	}
}

ReplayRound* BattleReplay::getRepRound(int iRound)
{
	if (m_mRepRound.count(iRound) <= 0)
	{
		m_mRepRound[iRound] = new ReplayRound();
	}
	return m_mRepRound[iRound];
}

void BattleReplay::saveMove()
{
	if(m_iStatus == REPLAY_STATUS_SAVE)
	{
		ActionGainPtr curGain = new ActionGain();
		curGain->moveIndex = BattleActor::move_index;
		MovePathPtr path = BattleActor::moveList->elementAt(BattleActor::move_index);
		curGain->point.x = path->point.x;
		curGain->point.y = path->point.y;
		saveActionGain(curGain);
		SAFE_DELETE(curGain);
	}
}
void BattleReplay::saveOrDeleteMove()
{
	if (m_iStatus == REPLAY_STATUS_REPLAY)
	/*	delete*/(getActionGain());
	else
		saveMove();
}