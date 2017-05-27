#include "Common.h"
#include "main.h"
#include "Message.h"
///////////////////////////////////////////////////////////////
//消息
MessagePtr CreateMessage(int msgId, TRControlPtr coiner)
{
	MessagePtr msg = new Message(msgId, coiner);
	return msg;
}
void DelMessage(MessagePtr msg)
{
	SAFE_DELETE(msg);
}