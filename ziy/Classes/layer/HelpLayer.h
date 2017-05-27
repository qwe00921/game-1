//
//  HelpLayer.h
//  TRGame
//
//  Created by 唐成予 on 17/05/18.
//
//

#ifndef _HelpLayer_H_
#define _HelpLayer_H_

#include "Defines.h"

class HelpLayer : public BTLayer
{
public:
	static HelpLayer* create(int type, string backgroud);
	static HelpLayer* create(string backgroud, Json::Value value);
	virtual bool init(int type, string backgroud);
	virtual bool init(int type, string backgroud, Json::Value value);
	HelpLayer();
	~HelpLayer();
	static int m_type;
private:
	void initControl();

	void callBackClose(Ref *sender);
	
private:
	CC_SYNTHESIZE( Json::Value, m_value, mValue);
	string m_mbackgroud;
	string m_strUrl;
};

class HelpLayerTableView : public BTLayer, TableViewDataSource, TableViewDelegate
{
public:
	HelpLayerTableView();
	~HelpLayerTableView();
	static HelpLayerTableView* create(string backgroud);
	static HelpLayerTableView* create(string backgroud, Json::Value value);
	virtual bool init(string backgroud);
	virtual bool init(string backgroud, Json::Value value);
	/* 必须实现的函数 */
	// 当cell被点击时调用该方法 参数为当前的tableview对象与被点击的cell对象
	virtual void tableCellTouched(TableView* table, TableViewCell* cell);
	// 设置tableview的Cell大小
	virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
	// 获取编号为idx的cell
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	// 设置tableview中cell的个数
	virtual ssize_t numberOfCellsInTableView(TableView *table);

	string m_backgroud;
	static int REWARD_W, REWARD_H;
private:
	CC_SYNTHESIZE(Json::Value, m_value, mValue);
	TableView* tableView;
	int m_iCellHeight;
};

class HelpLayerCell : public TableViewCell
{
public:
	HelpLayerCell();
	~HelpLayerCell();
	static HelpLayerCell* create();
	static HelpLayerCell* create(Json::Value m_value);
	virtual bool init();
	virtual bool init(Json::Value m_value);
	void UpDateCell(int iIndex);
private:
	void initControl();
	CC_SYNTHESIZE(Json::Value, m_value, mValue);
	CC_SYNTHESIZE(int, m_iHeight, CellHeight);//单元高度
};

class AbstructHelpBtn : public Node
{
public:
	/*
	@type与excel表中的id对应表示要显示的字符是哪个界面的
	@image表示帮助按钮的图标样式默认为0
	@backgroud帮助界面背景
	*/
	static AbstructHelpBtn* create(int type, string image="new_ui/img_global/suit_bnt1.png", string backgroud="new_ui/hero/hero_exp_bg.png");

	virtual bool init(int type, string image, string backgroud);
	void clikeBtn(Ref* ref);
private:
	virtual void initContrulr();
	int m_type;
	string m_image;
	string m_backgroud;
	Menu* menu;
};

#endif //_WebViewLayer_H_