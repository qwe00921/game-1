#ifndef _Script117_75_AssignSImage_H_
#define _Script117_75_AssignSImage_H_
class ScriptAssignSImage : public Script
{
public:
	ScriptAssignSImage() {
		cmd = CMD_ASSIGN_SIMAGE;
	}

	short unkown1;
	short actor_id1;
	short unkown2;
	short image_fight_id;

	int HandleScript();
	int initPara(char*SceneData, int index);
};
#endif