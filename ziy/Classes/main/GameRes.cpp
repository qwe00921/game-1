#include "common.h"
#include "main.h"
#include "StatConfig.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

int mp3_playing = 0;
int video_playing = 0;

enum 
{
	EVENT_NO_EVENT = -1,
	EVENT_PLAY_MP3 = 0,
	EVENT_STOP_MP3,
	EVENT_PLAY_VIDEO,
	EVENT_STOP_VIDEO,
	EVENT_PLAY_SOUND,
	EVENT_PLAY_E_SOUND,
	EVENT_PLAY_M_SOUND
};

int sound_event[100];
int sound_index[100];
int sound_repeat[100];
int sound_free_index = 0;
int sound_now_index = 0;
int fire_seted = 0;
int total_msg_num = 0;

int music_event[100];
int music_index[100];
int music_repeat[100];
int music_free_index = 0;
int music_now_index = 0;

int has_repeat = 0;
void playSound_Repeat(int sound_id,int repeat)
{	
	if (history->setting_flag[1] == 0)
	{
		return;
	}
	if(sound_id == SOUND_SET_FIRE&&fire_seted)
		return;
	else if(sound_id == SOUND_SET_FIRE)
		fire_seted = 1;
	else
		fire_seted = 0;
	if (sound_id == 4 || (sound_id >= 200 && sound_id<600))
	{
		SimpleAudioEngine::getInstance()->stopAllEffects();
		has_repeat = 0;
	}
	if(sound_id<100)
	{
		ChessPlaySound(sound_id);
	}
	else if(sound_id<200)
	{
		ChessPlayMSound(sound_id - 100);

	}
	else if (sound_id<600)
	{
		has_repeat = repeat == 255 ? 0 : 1;
		ChessPlayESound(sound_id - 200, has_repeat);
	}
	else
	{
		ChessPlayQSound(sound_id - 600);
	}
	total_msg_num ++;
}
void playSound(int sound_id)
{
	if (history->setting_flag[1] == 0)
	{
		return;
	}
	playSound_Repeat(sound_id,255);
}

void ChessPlayMp3(int index, bool bIsScript /*= false*/)
{
	history->bg_music_index = index;
	if(history->setting_flag[0])
	{
		SimpleAudioEngine::getInstance()->stopAllEffects();
		SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);

		char filename[100];
		string strName = bIsScript ? "Musics/bg%d.mp3" : "Musics/ui_%d.mp3";
		sprintf(filename, strName.c_str(), index);

		char* respath = getRespath(filename);
		ChessFile::saveResourceGen(respath);
		SimpleAudioEngine::getInstance()->playBackgroundMusic(respath, true);
	}
}

void ChessPlayNetSound(int index,const char* ptrName)
{
	if (history->setting_flag[1] == 0)
	{
		return;
	}
	char filename[100];
	if (ptrName == NULL)
		sprintf(filename, "Musics/ui_%d.mp3", index);
	else
		sprintf(filename, "Musics/%s_%d.mp3", ptrName,index);
	char* respath = getRespath(filename);
	ChessFile::saveResourceGen(respath);
	SimpleAudioEngine::getInstance()->playEffect(respath, false);
}

void ChessStopMp3()
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
}

int ChessPlaySound(int index)
{
	if (history->setting_flag[1] == 0)
	{
		return 0;
	}
// 	sndPlaySound (NULL, 0);
 	char filename[100];
// 	int flag = SND_ASYNC;
	sprintf(filename, "%s%d%s", index < 10 ? "Sounds/Se0" : "Sounds/Se", index, ".mp3");
//	ChessFree( ChessReadFile(filename, 0,0));
// 	return sndPlaySound(filename,flag);
	char* respath = getRespath(filename);
	ChessFile::saveResourceGen(respath);
	if (4 == index){//战斗开始背景音乐比较长用playBackgroundMusic播放 锁屏自动停止
        //SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.5);
        SimpleAudioEngine::getInstance()->stopAllEffects();
        SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		SimpleAudioEngine::getInstance()->playBackgroundMusic(respath, false);
		return 1;
	}
	else{
		return (int)SimpleAudioEngine::getInstance()->playEffect(respath, false);
	}

	
}
int ChessPlayQSound(int index)
{
	if (history->setting_flag[1] == 0)
	{
		return 0;
	}
	// 	sndPlaySound (NULL, 0);
	char filename[100];
	// 	int flag = SND_ASYNC;
	sprintf(filename, "%s%d%s", index < 10 ? "Sounds/Se_q_0" : "Sounds/Se_q_", index, ".mp3");
	//	ChessFree( ChessReadFile(filename, 0,0));
	// 	return sndPlaySound(filename,flag);
	char* respath = getRespath(filename);
	ChessFile::saveResourceGen(respath);
	return (int)SimpleAudioEngine::getInstance()->playEffect(respath, false);
}
int ChessPlayMSound(int index)
{
	if (history->setting_flag[1] == 0)
	{
		return 0;
	}
// 	sndPlaySound (NULL, 0);
 	char filename[100];
// 	int flag = SND_ASYNC;
	sprintf(filename, "%s%d%s", index < 10 ? "Sounds/Se_m_0" : "Sounds/Se_m_", index, ".mp3");
// 	return sndPlaySound(filename,flag);
//	ChessFree(ChessReadFile(filename, 0, 0));
	char* respath = getRespath(filename);
	ChessFile::saveResourceGen(respath);
	return (int)SimpleAudioEngine::getInstance()->playEffect(respath, false);
}
int ChessPlayESound(int index,int loop)
{
	if (history->setting_flag[1] == 0)
	{
		return 0;
	}
// 	sndPlaySound (NULL, 0);
 	char filename[100];
 //	int flag = SND_ASYNC;
 //	if(loop)
 //		flag |= SND_LOOP;
	sprintf(filename, "%s%d%s", index < 10 ? "Sounds/Se_e_0" : "Sounds/Se_e_", index, ".mp3");
//	ChessFree( ChessReadFile(filename, 0,0));
 //	return sndPlaySound(filename,flag);
	char* respath = getRespath(filename);
	ChessFile::saveResourceGen(respath);
	//return (int)SimpleAudioEngine::sharedEngine()->playEffect(respath, loop == 0 ? false : true);
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	SimpleAudioEngine::getInstance()->playBackgroundMusic(respath, loop == 0 ? false : true);//长音效用playBackgroundMusic播放 锁屏自动停止
	return 1;
}