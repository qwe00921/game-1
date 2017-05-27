#ifndef _ScriptBGSet_H_
#define _ScriptBGSet_H_


class ScriptBGSet : public Script
{
public:
	 ScriptBGSet() {
		cmd = CMD_BG_SET;
	}
	
	 short unkown1; // 45图片类型:0外景 1三国地图 2内景 3战场
	
	 short bg_type;
	
	 short unkown2; // 0c外景
	
	 short waijing_id;

	 short unkown3; // 1A三国地图

	 short sanguoditu_id;

	 short unkown4; // 1C内景

	 short neijing_id;

	 short unkown5; // 15战场

	 short zhanchang_id;

	 short waijing_id_replace;
	 short neijing_id_replace;
	 short zhanchang_id_replace;

	 int HandleRScript(RedeScreenPtr scr);
	 int HandleScript();
	 int initPara(char*SceneData,int index);
};
#endif