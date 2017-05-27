//
//  ExitGameLayer.h
//  TRGame
//
//  Created by Jimmy on 16/07/11.
//
//

#ifndef _ExitGameLayer_H_
#define _ExitGameLayer_H_

#include "Defines.h"

class ExitGameLayer : public BTLayer
{
public:
	ExitGameLayer();
	~ExitGameLayer();
	static ExitGameLayer* getInstance();
	virtual bool init();
private:
	CREATE_FUNC(ExitGameLayer);
	void initControl();

	void callBackCancel(Ref* node);
	void callBackConfirm(Ref* node);
private:
	static ExitGameLayer* m_pInstance;
};

#endif //_ExitGameLayer_H_