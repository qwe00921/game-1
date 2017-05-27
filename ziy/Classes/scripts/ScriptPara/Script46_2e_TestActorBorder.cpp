#include "Common.h"
#include "Scripts.h"
bool ScriptTestActorBorder::testBorder(int id1,int id2)
{
	HistoryActorPtr actor1 = &history->actors[id1];
	HistoryActorPtr actor2 = &history->actors[id2];
	if (actor1->hide == 1 || actor2->hide == 1 || actor1->GetCurHp() <= 0 || actor2->GetCurHp()<=0)
	{
		return false;
	}
	else if(id1 ==id2)
	{
		return false;
	}	
	else if(attackAreaCareLess != 0)
	{
		if((int)abs(actor1->x-actor2->x)>1||(int)abs(actor1->y-actor2->y)>1)
		{
			return false;
		}
	}
	else
	{
		if((int)abs(actor1->x - actor2->x)>1 || (int)abs(actor1->y - actor2->y)>1 || !(actor1->inAttackArea_PK(actor2->x, actor2->y) || actor2->inAttackArea_PK(actor1->x, actor1->y)))
		{
			return false;
		}
	}
	test_result= true;
	return true;
}
bool ScriptTestActorBorder::testBorder2(int id1,int id2)
{
	int i;
	switch(id2)
	{
	case 1024:
		for(i = 0;i<history->battleOwnNum;i++)
		{
			if(testBorder(id1,history->battleOwn[i]->actor_id))
				return true;
		}
		for(i = 0;i<history->battleFriendNum;i++)
		{
			if(testBorder(id1,history->battleFriend[i]->actor_id))
				return true;
		}
		for(i = 0;i<history->battleEnemyNum;i++)
		{
			if(testBorder(id1,history->battleEnemy[i]->actor_id))
				return true;
		}
		break;
	case 1025:
		for(i = 0;i<history->battleOwnNum;i++)
		{
			if(testBorder(id1,history->battleOwn[i]->actor_id))
				return true;
		}
		for(i = 0;i<history->battleFriendNum;i++)
		{
			if(testBorder(id1,history->battleFriend[i]->actor_id))
				return true;
		}
		break;
	case 1026:
		for(i = 0;i<history->battleEnemyNum;i++)
		{
			if(testBorder(id1,history->battleEnemy[i]->actor_id))
				return true;
		}
		break;
	case 1027:
		if(testBorder(id1,history->cur_actor->actor_id))
			return true;
		break;
	}
	return false;
}
//1024 任何部队
//1025 我军或友军
//1026 敌军
//1027 我军当前部队
int ScriptTestActorBorder::HandleScript(){
	test_result= false;
	if(actor_id1<1024&&actor_id2<1024)
	{
		if(testBorder(actor_id1,actor_id2))
			return 0;
	}
	else
	{
		int i;
		if(actor_id1<1024)
		{
			if(testBorder2(actor_id1,actor_id2))
				return 0;
		}
		else if(actor_id2<1024)
		{
			if(testBorder2(actor_id2,actor_id1))
				return 0;
		}
		else
		{
			switch(actor_id1)
			{
			case 1024:
				for(i = 0;i<history->battleOwnNum;i++)
				{
					if(testBorder2(history->battleOwn[i]->actor_id,actor_id2))
						return true;
				}
				for(i = 0;i<history->battleFriendNum;i++)
				{
					if(testBorder(history->battleFriend[i]->actor_id,actor_id2))
						return true;
				}
				for(i = 0;i<history->battleEnemyNum;i++)
				{
					if(testBorder(history->battleEnemy[i]->actor_id,actor_id2))
						return true;
				}
				break;
			case 1025:
				for(i = 0;i<history->battleOwnNum;i++)
				{
					if(testBorder(history->battleOwn[i]->actor_id,actor_id2))
						return true;
				}
				for(i = 0;i<history->battleFriendNum;i++)
				{
					if(testBorder(history->battleFriend[i]->actor_id,actor_id2))
						return true;
				}
				break;
			case 1026:
				for(i = 0;i<history->battleEnemyNum;i++)
				{
					if(testBorder(history->battleEnemy[i]->actor_id,actor_id2))
						return true;
				}
				break;
			case 1027:
				if(testBorder(history->cur_actor->actor_id,actor_id2))
					return true;
				break;
			}
		}
	}
	if(!get_test_result())
	{
		return -1;
	}
	return 0;
}

int ScriptTestActorBorder::initPara(char*SceneData,int index){
	unkown1 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_id1 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown2 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_id2 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;	
	unkown3 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	attackAreaCareLess =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;	
	return index;
}
