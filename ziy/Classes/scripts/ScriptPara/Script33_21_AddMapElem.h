#ifndef _ScriptAddMapElem_H_
#define _ScriptAddMapElem_H_


class ScriptAddMapElem  : public Script
{
public:
	 ScriptAddMapElem() {
		cmd = CMD_ADD_MAP_ELEM;
	}
	 ScriptAddMapElem(int elem_id,int x,int y) {
		cmd = CMD_ADD_MAP_ELEM;
		this->elem_id = (short) elem_id;
		this->x = x;
		this->y = y;
		this->delay1 = 0;
		this->delay2 = 0;
	}
	 short unkown1;

	 int x;

	 short unkown2;

	 int y;

	 short unkown3;

	 short elem_id;

	 short unkown4;

	 short delay1;
	 
	 short unkown5;

	 short delay2;
	 int HandleScript();
	 int initPara(char*SceneData,int index);
};
#endif
