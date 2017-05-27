//
//  HelpLayer.h
//  TRGame
//
//  Created by 唐成予 on 17/05/18.
//
//

#include "HelpLayer.h"

int HelpLayerTableView::REWARD_W = 570;
int HelpLayerTableView::REWARD_H = 310;

int HelpLayer::m_type = 0;
HelpLayer::HelpLayer()
{

}

HelpLayer::~HelpLayer()
{

}

HelpLayer* HelpLayer::create(int type, string backgroud)
{
	HelpLayer* pRet = new HelpLayer();
	if(pRet&&pRet->init(type,backgroud))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}
HelpLayer* HelpLayer::create(string backgroud, Json::Value value)
{
	HelpLayer* pRet = new HelpLayer();
	if (pRet&&pRet->init(0, backgroud, value))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool HelpLayer::init(int type, string backgroud)
{
	if(!Layer::init())
	{
		return false;
	}
	m_value = NULL;
	HelpLayer::m_type = type;
	m_mbackgroud = backgroud;
	initControl();
	return true;
}
bool HelpLayer::init(int type, string backgroud, Json::Value value)
{
	
	if (!Layer::init())
	{
		return false;
	}
	m_value = value;
	HelpLayer::m_type = type;
	m_mbackgroud = backgroud;
	initControl();
	return true;
}



void HelpLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	float fScale = 0.90f;

	//遮罩
	/*LayerColor* layBlack = LayerColor::create(Color4B(0, 0, 0, 100));
	layBlack->setPosition(Vec2(0, 0));
	this->addChild(layBlack);*/

	string backgroudPath = m_mbackgroud;
	//switch (m_mbackgroud)
	//{
	//case 0:
	//{
	//	backgroudPath = "new_ui/pvp/pvp_bg5.png";
	//	break;
	//}
	//case 1:
	//{
	//	backgroudPath = "new_ui/pvp/pvp_bg3.png";
	//	break;
	//}
	//case 2:
	//{
	//	backgroudPath = "new_ui/pvp/pvp_bg4.png";
	//	break;
	//}
	//case 3:
	//{
	//	backgroudPath = "new_ui/pvp/pvp_bg5.png";
	//	break;
	//}
	//default:
	//	break;
	//}
	//背景1
	auto spBG1 = ReadSpriteName(backgroudPath.c_str());
	spBG1->setPosition(point);
	this->addChild(spBG1);

	HelpLayerTableView::REWARD_W = spBG1->getContentSize().width-40;
	HelpLayerTableView::REWARD_H = spBG1->getContentSize().height-100;

	////帮助
	if (m_value == NULL)
	{
		auto tvReward = HelpLayerTableView::create(m_mbackgroud);
		this->addChild(tvReward);
	}
	else
	{
		auto tvReward = HelpLayerTableView::create(m_mbackgroud, m_value);
		this->addChild(tvReward);
	}

	if (HelpLayer::m_type ==0)
	{
		Sprite* m_spBG = ReadSpriteName("new_ui/img_global/help_title.png");
		m_spBG->setPosition(Vec2(spBG1->getContentSize().width/2, spBG1->getContentSize().height-30+8));
		spBG1->addChild(m_spBG);
	}

	//按钮菜单
	Menu* ptrMenu = Menu::create();
	ptrMenu->setPosition(Vec2(0, 0));
	this->addChild(ptrMenu);

	//关闭
	Sprite* spCloseN = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* spCloseS = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* btnClose = MenuItemSprite::create(spCloseN, spCloseS, CC_CALLBACK_1(HelpLayer::callBackClose, this));
	btnClose->setPosition(point + Vec2(spBG1->getContentSize().width/2,spBG1->getContentSize().height/2));
	if (HelpLayer::m_type==0)
	{
		btnClose->setPosition(point + Vec2(spBG1->getContentSize().width / 2-30, spBG1->getContentSize().height / 2-30));
	}
	ptrMenu->addChild(btnClose);
}

void HelpLayer::callBackClose(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
HelpLayerCell::HelpLayerCell()
{

}

HelpLayerCell::~HelpLayerCell()
{

}

HelpLayerCell* HelpLayerCell::create()
{
	HelpLayerCell* pRet = new HelpLayerCell();
	if (pRet&&pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}
HelpLayerCell* HelpLayerCell::create(Json::Value m_value)
{
	HelpLayerCell* pRet = new HelpLayerCell();
	if (pRet&&pRet->init(m_value))
	{
		
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool HelpLayerCell::init()
{
	if (!TableViewCell::init())
	{
		return false;
	}
	
	initControl();
	return true;
}
bool HelpLayerCell::init(Json::Value m_ivalue)
{
	
	if (!TableViewCell::init())
	{
		return false;
	}
	m_value = m_ivalue;
	initControl();
	return true;
}

void HelpLayerCell::UpDateCell(int iIndex)
{

}

void HelpLayerCell::initControl()
{
	int iPointX = 5;
	int iPointY = 5;
	Layer* ptrLayer = Layer::create();

	char* sData = ChessReadFile("json/dic_help.json");
	if (sData == NULL)
	{
		CCAssert(true, "未找到文件,或文件加载错误");
		return;
	}
	Json::Reader reader;
	Json::Value line;
	if (!reader.parse(sData, line, false))return;
	int size = line.size();
	
	if (HelpLayer::m_type == 0)
	{
		for (int i=0;i<m_value["infoAry"].size();i++)
		{
			__String* m_string;
			if (m_value["infoAry"][i]["isSuccess"].isInt())
			{
				if (m_value["infoAry"][i]["isSuccess"].asInt() == 1)
				{
					m_string = __String::createWithFormat(LL("qianghua10"), m_value["infoAry"][i]["times"].asInt(), LL("qianghua10chenggong"));
				}
				else if (m_value["infoAry"][i]["isSuccess"].asInt() == 0)
				{
					m_string = __String::createWithFormat(LL("qianghua10"), m_value["infoAry"][i]["times"].asInt(), LL("qianghua10shibai"));
				}
			}
			else
			{
				if (m_value["infoAry"][i]["error"].isString())
				{
					m_string = __String::createWithFormat(LL("qiangtua10tingzhi"), m_value["infoAry"][i]["error"].asCString());
				}
			}
			
			Label* labRank = LabelSystemFont(m_string->_string, 16);
			labRank->setAnchorPoint(Vec2(0.0f, 1.0f));
			labRank->setPosition(Vec2(iPointX+60, -iPointY));
			labRank->setDimensions(HelpLayerTableView::REWARD_W - 10, 0);
			labRank->enableOutline(Color4B(158, 116, 48, 255), 1);
			//labRank->setColor(Color3B(102,0,0));
			if (m_value["infoAry"][i]["isSuccess"].asInt() == 1)
			{
				labRank->setColor(Color3B::YELLOW);
			}
			else if (m_value["infoAry"][i]["isSuccess"].asInt() == 0)
			{
				
			}
			else
			{
				
			}
			ptrLayer->addChild(labRank);
			iPointY += labRank->getContentSize().height + 5;

			Sprite* gold_bg = ReadSpriteName("new_ui/img_global/help_bg_dian.png");
			gold_bg->setPosition(Vec2(-gold_bg->getContentSize().width/2, gold_bg->getContentSize().height/2));
			labRank->addChild(gold_bg);
		}
	}
	else 
	{
		for (int i = 0; i < size; i++)
		{
			if (line[i]["id"].asInt() == HelpLayer::m_type)
			{
				Label* labRank = LabelSystemFont(line[i]["text"].asString(), 16);
				labRank->setAnchorPoint(Vec2(0.0f, 1.0f));
				labRank->setPosition(Vec2(iPointX, -iPointY));
				labRank->setDimensions(HelpLayerTableView::REWARD_W - 10, 0);
				labRank->enableOutline(Color4B(158, 116, 48, 255), 1);
				ptrLayer->addChild(labRank);
				iPointY += labRank->getContentSize().height + 5;
			}
		}
	}
	

	////规则标题
	//Label* labRank = LabelSystemFont(LL("pvprewardtitle"), 16);
	//labRank->setAnchorPoint(Vec2(0.0f, 1.0f));
	//labRank->setPosition(Vec2(iPointX, -iPointY));
	//labRank->enableOutline(Color4B(158, 116, 48, 255), 1);
	//ptrLayer->addChild(labRank);
	//iPointY += labRank->getContentSize().height + 5;

	////规则明细
	//for (UInt i = 0; ; i++)
	//{
	//	string strInfo = StringUtils::format("pvprewardinfo%d", i + 1);
	//	char* chInfo = LL(strInfo.c_str(), false, true);
	//	if (chInfo)
	//	{
	//		Label* labInfo = LabelSystemFont(chInfo, 16);
	//		labInfo->setAnchorPoint(Vec2(0.0f, 1.0f));
	//		labInfo->setPosition(Vec2(iPointX, -iPointY));
	//		labInfo->setDimensions(REWARD_W - 10, 0);
	//		labInfo->enableOutline(Color4B(158, 116, 48, 255), 1);
	//		ptrLayer->addChild(labInfo);
	//		iPointY += labInfo->getContentSize().height + 5;
	//	}
	//	else break;
	//}
	iPointY += 10;
	m_iHeight = iPointY;
	ptrLayer->setPosition(Vec2(0, iPointY));
	this->addChild(ptrLayer);

	ChessFree(sData);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
HelpLayerTableView::HelpLayerTableView()
{

}

HelpLayerTableView::~HelpLayerTableView()
{

}

HelpLayerTableView* HelpLayerTableView::create(string backgroud)
{
	HelpLayerTableView* pRet = new HelpLayerTableView();
	if (pRet&&pRet->init(backgroud))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}
HelpLayerTableView* HelpLayerTableView::create(string backgroud, Json::Value value)
{
	HelpLayerTableView* pRet = new HelpLayerTableView();
	if (pRet&&pRet->init(backgroud, value))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}
bool HelpLayerTableView::init(string backgroud, Json::Value value)
{
	
	if (!Layer::init())
	{
		return false;
	}
	m_value = value;
	m_backgroud = backgroud;
	HelpLayerCell* ptrCell = HelpLayerCell::create(value);
	m_iCellHeight = ptrCell->getCellHeight();

	//创建一个talbleview 将datasource设置为当前的类的对象 tableview的显示区域大小为 570 * 310
	tableView = TableView::create(this, Size(HelpLayerTableView::REWARD_W, HelpLayerTableView::REWARD_H));
	//设置tableview为水平方向  ScrollView::Direction::VERTICAL 为垂直,ScrollView::Direction::HORIZONTAL 为水平
	tableView->setDirection(ScrollView::Direction::VERTICAL);
	//设置位置
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	tableView->setPosition(Vec2(size.width / 2 - REWARD_W / 2, size.height / 2 - REWARD_H / 2 - 15));
	//设置代理对象
	tableView->setDelegate(this);
	// 填充顺序
	tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	//添加tableview到当前layer
	this->addChild(tableView);
	return true;
}
bool HelpLayerTableView::init(string backgroud)
{
	if (!Layer::init())
	{
		return false;
	}
	m_value = NULL;
	m_backgroud = backgroud;
	HelpLayerCell* ptrCell = HelpLayerCell::create();
	m_iCellHeight = ptrCell->getCellHeight();

	//创建一个talbleview 将datasource设置为当前的类的对象 tableview的显示区域大小为 570 * 310
	tableView = TableView::create(this, Size(HelpLayerTableView::REWARD_W, HelpLayerTableView::REWARD_H));
	//设置tableview为水平方向  ScrollView::Direction::VERTICAL 为垂直,ScrollView::Direction::HORIZONTAL 为水平
	tableView->setDirection(ScrollView::Direction::VERTICAL);
	//设置位置
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	tableView->setPosition(Vec2(size.width / 2 - REWARD_W / 2, size.height / 2 - REWARD_H / 2 - 15));
	//设置代理对象
	tableView->setDelegate(this);
	// 填充顺序
	tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	//添加tableview到当前layer
	this->addChild(tableView);

	return true;
}

void HelpLayerTableView::tableCellTouched(TableView* table, TableViewCell* cell)
{

}

Size HelpLayerTableView::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
	return Size(REWARD_W, m_iCellHeight);
}

TableViewCell* HelpLayerTableView::tableCellAtIndex(TableView *table, ssize_t idx)
{
	TableViewCell *cell = table->dequeueCell();
	if (!cell)
	{
		if (m_value==NULL)
		{
			cell = HelpLayerCell::create();
		}
		else
		{
			cell = HelpLayerCell::create(m_value);
		}
	}
	else
	{
		HelpLayerCell* c = (HelpLayerCell*)cell;
		c->UpDateCell(idx);
	}
	return cell;
}

ssize_t HelpLayerTableView::numberOfCellsInTableView(TableView *table)
{
	return 1;
}


AbstructHelpBtn * AbstructHelpBtn::create(int type, string image, string backgroud)
{

	AbstructHelpBtn* pRet = new AbstructHelpBtn();
	if (pRet&&pRet->init(type,image, backgroud))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool AbstructHelpBtn::init(int type, string image, string backgroud)
{
	if (!Node::init())
	{
		return false;
	}

	m_type = type;
	m_image = image;
	m_backgroud = backgroud;
	initContrulr();

	return true;
}
void AbstructHelpBtn::initContrulr()
{
	menu = Menu::create();
	this->addChild(menu);

	string normalImage = m_image, selectedImage= m_image, disableImage= m_image;
	auto spriteN = ReadSpriteName(normalImage.c_str());
	Sprite* spModelS = ReadSpriteName(selectedImage.c_str());
	auto btn = MenuItemSprite::create(spriteN, spModelS, CC_CALLBACK_1(AbstructHelpBtn::clikeBtn, this));

	menu->addChild(btn);


	//switch (m_image)
	//{
	//case 0:
	//{
	//	normalImage = "new_ui/icon_ui/icon10_2.png";
	//	selectedImage= "new_ui/icon_ui/icon10_2.png";
	//	disableImage = "new_ui/icon_ui/icon10_2.png";
	//	break;
	//}
	//case 1:
	//{
	//	
	//	break;
	//}
	//case 2:
	//{
	//	
	//	break;
	//}
	//case 3:
	//{
	//	
	//	break;
	//}
	//default:
	//	break;
	//}

	
	
	
}

void AbstructHelpBtn::clikeBtn(Ref * ref)
{
	ChessApplication::getInstance()->AddFunctionLayer(HelpLayer::create(m_type, m_backgroud));
}
