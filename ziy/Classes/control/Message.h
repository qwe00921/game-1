#ifndef _Message_H_
#define _Message_H_

/////////////////////////////////////////////////////////////////////
//消息
class Message
{
public:
	Message()
	{
		msgId = 0;
		type = 0;
		coiner = NULL;
	}
	Message(int msg, TRControlPtr ctrl)
	{
		msgId = msg;
		type = 0;
		coiner = ctrl;
	}
public:
    int msgId;
    int type;
    TRControlPtr coiner;   //发起者
};

typedef Message* MessagePtr;
MessagePtr CreateMessage(int msgId, TRControlPtr coiner);
void DelMessage(MessagePtr msg);


#endif