#ifndef _ScriptRUNACTION_H_
#define _ScriptRUNACTION_H_


class ScriptRunAction  : public Script
{
public:
	ScriptRunAction(HistoryActorPtr ptrCurActor, HistoryActorPtr ptrOppoActor, int iskill_anim_id, int i_skill_type, int iState, int iFrame, int iSound = 0, bool bIsOwn = false) {
		cmd = CMD_MAX;
		m_curactor = ptrCurActor;
		m_oppoactor = ptrOppoActor;
		skill_anim_id = iskill_anim_id;
		m_skill_type = i_skill_type;
		m_state = iState;
		m_frame = iFrame;
		m_bIsOwn = bIsOwn;
		mAnimName = "";
		m_sound = iSound;
	}
	ScriptRunAction(HistoryActorPtr ptrCurActor, HistoryActorPtr ptrOppoActor, const char* AnimName, int iState, int iFrame, int iSound = 0, bool bIsOwn = false) {
		cmd = CMD_MAX;
		m_curactor = ptrCurActor;
		m_oppoactor = ptrOppoActor;
		skill_anim_id = 0;
		m_skill_type = 0;
		m_state = iState;
		m_frame = iFrame;
		m_bIsOwn = bIsOwn;
		mAnimName = AnimName;
		m_sound = iSound;
	}

	int m_skill_type;
	int skill_anim_id;
	HistoryActorPtr m_curactor;
	HistoryActorPtr m_oppoactor;
	int m_state;
	int m_frame;
	bool m_bIsOwn;
	std::string mAnimName;
	int m_sound;
	int HandleScript();
	int initPara(char*SceneData,int index);
};
#endif