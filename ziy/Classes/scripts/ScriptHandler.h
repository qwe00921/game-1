#ifndef _ScriptHandler_H_
#define _ScriptHandler_H_

class ScriptHandler {
public:
	ScriptHandler();
	~ScriptHandler();
public:
	bool RunScript();
	bool RunRScript(RedeScreenPtr scr);	
	void LoadRFile(int index);
	void LoadRFileName(const char* file_name);
	void LoadSFile(int index);
	void LoadSFileName(const char* file_name);
	void SetScriptList(int index);
	void GenerateScriptListFromAI(const BattleAI& ai);
	void GenerateScriptListActorState(int relation,HistoryPtr history);	
	void ClearScriptList();	
	void SkipToLastOrder();  //跳转到当前指令列的最后一条指令，用于0d指令解析
	void SkipScript(int index);
	bool GetDispatchInfo(string SFileNameS); //获取战场预览我军坐标信息
	void addScript(Script* ptrScript);
	void removeScript(Script* ptrScript);
private:
	void TestFalse();
	void DecodeScenes(char* scene_data, bool bNormal = true); //bNormal为正常加载，bNormal = false为只获取战场预览信息
	void ChangeActorState(HistoryActorPtr curActor);
	void GenerateScriptGood(ActionGainPtr gain, HistoryActorPtr curActor);
	void GenerateScriptAttack(ActionGainPtr gain, HistoryActorPtr curActor);
	void GenerateScriptSkill(ActionGainPtr gain, HistoryActorPtr curActor);
	void GenerateScriptState(ActionGainPtr gain, HistoryActorPtr curActor);
	void GenerateScriptUseGood(ActionGainPtr gain);
	void GetDoubleSeid(HistoryActorPtr curActor);
public:
	int list_index;
	bool script_skipto_begin;
	DList<Script>* pre_battle_list;
	int m_iBGIndex; //战场预览S场景图片索引
private:
	int script_index;
	ScriptPtr cur_script;
	DList<Script>* cur_list;
	DVector<int>* cur_index_list;
	bool is_script_skipto;//区别剧本指令和自己添加的动作指令
private:
	DList<DList<Script> >* script_lists;
	DList<DVector<int> >* script_index_lists;
private:
	/*
	* 禁止对象拷贝，声明私有方法并且不实现
	*/
	ScriptHandler(const ScriptHandler&);
	ScriptHandler& operator=(const ScriptHandler&);
};

#endif