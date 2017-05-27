//
//  WebViewLayer.h
//  TRGame
//
//  Created by Jimmy on 16/09/08.
//
//

#ifndef _WebViewLayer_H_
#define _WebViewLayer_H_

#include "Defines.h"

class WebViewLayer : public BTLayer
{
public:
	static WebViewLayer* create(string strUrl);
	virtual bool init(string strUrl);
	WebViewLayer();
	~WebViewLayer();
private:
	void initControl();

	void callBackClose(Ref *sender);
private:
	string m_strUrl;
};

#endif //_WebViewLayer_H_