#ifndef _ScriptActorChangeImage_H_
#define _ScriptActorChangeImage_H_


//R剧本形象改变
class ScriptActorChangeImage : public Script
{
public:
	 ScriptActorChangeImage() {
		cmd = CMD_ACTOR_CHANGE_IMAGE;
	}
	 short unkown1;
	 short actor_id;
	 short unkown2;
	 short img_id;
	int HandleScript();
	int initPara(char*SceneData,int index);
};
#endif