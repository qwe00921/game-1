//
//  PayRestoreLayer.h
//  TRGame
//
//  Created by Jimmy on 17/02/15.
//
//

#ifndef _PayRestoreLayer_H_
#define _PayRestoreLayer_H_

#include "Defines.h"

class PayRestoreLayer : public BTLayer
{
public:
	PayRestoreLayer();
	~PayRestoreLayer();
	static PayRestoreLayer* create(string strTitle, string strContent);
	virtual bool init(string strTitle, string strContent);
private:
	void initControl();
	void callBackConfirm(Ref *sender);
private:
	string m_strTitle;
	string m_strContent;



	Menu* m_menuNull;
};

#endif //_PayRestoreLayer_H_