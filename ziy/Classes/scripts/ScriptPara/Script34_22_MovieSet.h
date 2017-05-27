#ifndef _ScriptMovieSet_H_
#define _ScriptMovieSet_H_
class ScriptMovieSet : public Script
{
public:
	 ScriptMovieSet() {
		cmd = CMD_MOVIE_SET;
	}
	 short unkown1;
	 short movie_id;
	int HandleScript();
	 int initPara(char*SceneData,int index);
};
#endif