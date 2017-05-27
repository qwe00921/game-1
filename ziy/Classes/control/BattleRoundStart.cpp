#include "Common.h"
#include "main.h"
#include "BattleRoundStart.h"

void BattleRoundStart::onPaint(DGraphics* g)
{
	int x = screen_size_x / 2 - imgRelation->width / 2;
	int y = screen_size_y / 2 - imgRelation->height / 2;
	if (relation == 0)
	{
		y -= 52;
	}
	g->drawImage(imgRelation, x, y);

	//第%d回合
	if (relation == 0 && frames > SHOW_BONUS_FRAME)
	{
		int x = screen_size_x / 2 - imgRound->width / 2;
		int y = screen_size_y / 2 + 12 - imgRound->height / 2;
		g->drawImage(imgRound, x, y);
		int img_num = imgNum.size();
		x += 102 - img_num * 17;
		for (int i = 0; i<img_num; i++)
		{
			g->drawImage(imgNum[i], x+i*34, y+1);
		}
	}

	//天气图标
	if(relation == 0 && frames > 2 * SHOW_BONUS_FRAME)
	{
		int x = screen_size_x / 2 - imgAir->width / 2;
		int y = screen_size_y / 2 + 90 - imgAir->height / 2;
		g->drawImage(imgAir, x, y);
	}

	if(frames > 4 * SHOW_BONUS_FRAME)
	{
		MessagePtr msg = CreateMessage(msgBaseId+MSG_BONUS_RELEASED,this);
		owner->onMessage(msg);
		DelMessage(msg);
		if (relation == 0)
		{
			history->updateGuide();
		}
	}
}

BattleRoundStart::BattleRoundStart(int relation,int msgBaseId)
{
    this->msgBaseId = msgBaseId;
	this->relation = relation;
	imgAir = NULL;

	if (relation==0)
		imgRelation = CreateGlobalImageFile("new_ui/battle/warround_txt1.png");
	else
		imgRelation = CreateGlobalImageFile("new_ui/battle/warround_txt2.png");

	imgRound = CreateGlobalImageFile("new_ui/battle/warround_txt3.png");

	if (relation == 0)
	{
		int iRound = history->GetCurRound();
		short numArr[33];
		int arr_num = 0;
		do
		{
			numArr[arr_num++] = iRound % 10;
			iRound = iRound / 10;
		} while (iRound > 0);

		int img_num = 0;
		__String* img_path = NULL;
		for (int i = arr_num - 1; i >= 0; i--)
		{
			img_path = __String::createWithFormat("new_ui/img_number/num2_%d.png", numArr[i]);
			imgNum[img_num++] = CreateGlobalImageFile(img_path->getCString());
		}

		__String* strAir = __String::createWithFormat("new_ui/battle/weather%d.png", history->m_iCurAir);
		imgAir = CreateGlobalImageFile(strAir->getCString());
	}
}

BattleRoundStart::~BattleRoundStart()
{
	SAFE_DELETE_IMAGE(imgRelation);
	SAFE_DELETE_IMAGE(imgRound);
	SAFE_DELETE_IMAGE(imgAir);
	for (map<int, TRImagePtr>::iterator it = imgNum.begin(); it != imgNum.end(); it++)
	{
		SAFE_DELETE_IMAGE(it->second);
	}
}
