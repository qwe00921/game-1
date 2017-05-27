//
//  PVPActorLayer.h
//  TRGame
//
//  Created by Jimmy on 17/03/23.
//
//

#ifndef _PVPActorLayer_H_
#define  _PVPActorLayer_H_

#include "Defines.h"
#include "PVPHomeLayer.h"

class PVPActorLayer : public BTLayer
{
public:
	PVPActorLayer();
	~PVPActorLayer();
	static PVPActorLayer* create(PVPActorData* ptrActor);
	virtual bool init(PVPActorData* ptrActor);
private:
	void initControl();
	void callBackFight(Ref *sender);
	void callBackClose(Ref *sender);
private:
	PVPActorData* m_ptrActor;
};

#endif // _PVPActorLayer_H_