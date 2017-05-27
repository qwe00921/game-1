#ifndef _ScriptSetAirType_H_
#define _ScriptSetAirType_H_
class ScriptSetAirType : public Script
{
public:
	 ScriptSetAirType() {
		cmd = CMD_SET_AIR_TYPE;
	}
	 short unkown1;
	 short air_id;
	int HandleScript();
	 int initPara(char*SceneData,int index);
};
#endif