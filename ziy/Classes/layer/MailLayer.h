//
//  MailLayer.h
//  TRGame
//
//  Created by Jimmy on 16/06/07.
//
//

#ifndef _MAILLAYER_H_
#define _MAILLAYER_H_

#include "Defines.h"

class Net_HomePage;
enum 
{
	MAIL_MSG = 1,
	MAIL_GOODS
};

class MailElem
{
	CC_SYNTHESIZE(string, m_strId, MailId);//邮件ID
	CC_SYNTHESIZE(int, m_iType, Type);//邮件类型
	CC_SYNTHESIZE(int, m_iIsRead, IsRead);//是否已读
	CC_SYNTHESIZE(string, m_strTitle, Title);//邮件标题
	CC_SYNTHESIZE(string, m_strTime, Time);//邮件时间
	CC_SYNTHESIZE(string, m_strText, Content);//邮件内容
	CC_SYNTHESIZE(string, m_strSender, Sender);//发件人
public:
	map<int, int> m_mGoodsMap;
};

class MailCell : public TableViewCell
{
public:
	CREATE_FUNC(MailCell);
	bool init();
	void updateCell(MailElem* ptrMail);
private:
	void initControl();
public:
	Sprite* m_spBGN;
private:
	Sprite* m_spIcon;
	Label* m_labTitle;
	Label* m_labType;
	Label* m_labTime;
};

class MailTableView : public BTLayer, TableViewDataSource, TableViewDelegate
{
public:
	static MailTableView* create(map<int, MailElem*> mMailMap, EventDelegate* ptrDelegate);
	virtual bool init(map<int, MailElem*> mMailMap, EventDelegate* ptrDelegate);
	MailTableView();
	~MailTableView();
	/* 必须实现的函数 */
	//    // 当滑动tableview时触发该方法 参数为当前的tableview对象
	virtual void scrollViewDidScroll(ScrollView* view);
	//    // 当tableview被放大或缩小时触发该方法  参数为当前tableview对象
	virtual void scrollViewDidZoom(ScrollView* view);
	// 当cell被点击时调用该方法 参数为当前的tableview对象与被点击的cell对象
	virtual void tableCellTouched(TableView* table, TableViewCell* cell);
	virtual void tableCellHighlight(TableView* table, TableViewCell* cell);
	virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell);
	// 设置tableview的Cell大小
	virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
	// 获取编号为idx的cell
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	// 设置tableview中cell的个数
	virtual ssize_t numberOfCellsInTableView(TableView *table);
	void updateMailShow(map<int, MailElem*> mMailMap);
private:
	void showMailInfo();
public:
	TableView* m_pTableView;
private:
	map<int, MailElem*> m_mMailMap;
	EventDelegate* m_pDelegate;
	int m_iClickIndex;
};

class MailLayer : public BTLayer
{
public:
	MailLayer();
	~MailLayer();
	static MailLayer* create(EventDelegate* ptrDelegate);
	virtual bool init(EventDelegate* ptrDelegate);
	virtual void updateShow(int iIndex);
private:
	void initControl();
	void updateMailMap();

	void callBackClose(Ref *sender);

	void mailHttpRequest();
	void onHttpRequestCompleted(Node *sender, void *resultdata);
	bool onMailSucceed(Json::Value valRes);
private:
	CC_SYNTHESIZE( Net_HomePage *,m_homeClass, m_homeClass);
	EventDelegate* m_ptrDel;
	Menu* m_pBtnMenu;
	MailTableView* m_tvMail;
	map<int, MailElem*> m_mMailMap;
};

#endif //_MAILLAYER_H_