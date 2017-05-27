#include "Common.h"
#include "jsons.h"
#include "TrPayInfo.h"
#include "cocos2d.h"
#include "Data.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////

TrPayInfo::TrChargeChannel::TrChargeChannel()
{
	channelId = 0;
	discount = 0;
	iosProductID = "";
}

Json::Value TrPayInfo::TrChargeChannel::toJsonValue()
{
	Json::Value ret;
	ret["channel_id"] = channelId;
	ret["discount"] = discount;
	ret["ios_productID"] = iosProductID->getChar();
	return ret;
}

TrPayInfo::TrChargeChannel* TrPayInfo::TrChargeChannel::parseJsonValue(Json::Value jsonValue)
{
	if (jsonValue["channel_id"].isInt() && jsonValue["discount"].isInt())
	{
		TrChargeChannel* ret = new TrChargeChannel();
		ret->channelId = jsonValue["channel_id"].asInt();
		ret->discount = jsonValue["discount"].asInt();
		if (&jsonValue["ios_productID"] != NULL && jsonValue["ios_productID"].isString()){
			ret->iosProductID = jsonValue["ios_productID"].asCString();
		}
		return ret;
	}
	return NULL;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////

TrPayInfo::TrChargeItem::TrChargeItem()
{
	mChannelList = new DList<TrChargeChannel>(true);

	chargeId = "";
	subject = "";
	body = "";
	price = 0;
}

TrPayInfo::TrChargeItem::~TrChargeItem()
{
	SAFE_DELETE(mChannelList);
}

TrPayInfo::TrChargeChannel* TrPayInfo::TrChargeItem::findChannelItemById(int channelId)
{
	for (int i = 0; i < mChannelList->size(); i++)
	{
		TrChargeChannel* item = mChannelList->elementAt(i);
		if (item->channelId == channelId)
		{
			return item;
		}
	}
	return NULL;
}

Json::Value TrPayInfo::TrChargeItem::toJsonValue()
{
	Json::Value ret;
	ret["charge_id"] = chargeId->getChar();
	ret["subject"] = subject->getChar();
	ret["body"] = body->getChar();
	ret["price"] = price;
	Json::Value channel;
	for (int i = 0; i < getChannelSize(); i++)
	{
		channel.append(this->getChannelItem(i)->toJsonValue());
	}
	ret["channel"] = channel;
	return ret;
}

TrPayInfo::TrChargeItem* TrPayInfo::TrChargeItem::parseJsonValue(Json::Value jsonValue)
{
	if (jsonValue["charge_id"].isString()
		&& jsonValue["subject"].isString()
		&& jsonValue["body"].isString()
		&& jsonValue["price"].isInt()
		&& jsonValue["channel"].isArray())
	{
		TrChargeItem* ret = new TrChargeItem();
		ret->chargeId = jsonValue["charge_id"].asCString();
		ret->subject = jsonValue["subject"].asCString();
		ret->body = jsonValue["body"].asCString();
		ret->price = jsonValue["price"].asInt();

		Json::Value channelValue = jsonValue["channel"];
		for (unsigned int i = 0; i < channelValue.size(); i++)
		{
			TrChargeChannel* channel = TrChargeChannel::parseJsonValue(channelValue[i]);
			if (channel == NULL)
			{
				SAFE_DELETE(ret);
				return NULL;
			}
			ret->mChannelList->addElement(channel);
		}
		return ret;
	}
	return NULL;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////

TrPayInfo::TrPayInfo()
{
	mChargeList = new DList<TrChargeItem>(true);
}

TrPayInfo::~TrPayInfo()
{
	SAFE_DELETE(mChargeList);
}

bool TrPayInfo::loadJsonValue(Json::Value jsonValue)
{
	Json::Value line = jsonValue;

	if (!line.isArray())
	{
		return false;
	}
	mChargeList->removeAllElements();

	for (unsigned int i = 0; i < line.size(); i++)
	{
		TrChargeItem* chargeItem = TrChargeItem::parseJsonValue(line[i]);
		if (chargeItem == NULL)
		{
			mChargeList->removeAllElements();
			return false;
		}
		mChargeList->addElement(chargeItem);
	}

	return true;
}

Json::Value TrPayInfo::toJsonValue()
{
	Json::Value ret;

	for (int i = 0; i < this->getChargeSize(); i++)
	{
		ret.append(this->getChargeItem(i)->toJsonValue());
	}

	return ret;
}

TrPayInfo::TrChargeItem* TrPayInfo::findChargeItem(const char* chargeId)
{
	if (chargeId == NULL)
	{
		return NULL;
	}
	for (int i = 0; i < mChargeList->size(); i++)
	{
		TrChargeItem* item = mChargeList->elementAt(i);
		if (item->chargeId->equals(chargeId))
		{
			return item;
		}
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
TrGoodCharge::TrGoodItem::TrGoodItem()
{
	id = 0;
	goodId = 0;
    goodLevel = 0;
	num = 0;
	levelStrict = 0;
	stageStrict = 0;
	chargeId = "";
	jh_id = 0;
 	isReceived = false;
	rebuy = 0;

	iosProductID = "";
	iosPriceLocal = "";
    iosPriceToDisplay = "";
	title = "";
	desc = "";
	itunesPrice = -1;
}

bool TrGoodCharge::TrGoodItem::isFree()
{
	return is_free;
}

Json::Value TrGoodCharge::TrGoodItem::toJsonValue()
{
	Json::Value ret;
	ret["id"] = id;
	ret["good_id"] = goodId + 1;
	ret["num"] = num;
	ret["level_strict"] = levelStrict;
	ret["gk_id"] = stageStrict+1;
	ret["charge_id"] = chargeId->getChar();
	ret["jh_id"] = jh_id;
	ret["rebuy"] = rebuy;
	ret["is_display"] = is_display;
	
	return ret;
}

TrGoodCharge::TrGoodItem* TrGoodCharge::TrGoodItem::parseJsonValue(Json::Value jsonValue)
{
	if (jsonValue["id"].isInt() 
		&& jsonValue["good_id"].isInt() 
		&& jsonValue["num"].isInt()
		&& jsonValue["level_strict"].isInt()
		&& jsonValue["gk_id"].isInt()
		&& jsonValue["charge_id"].isString() 
		&& jsonValue["jh_id"].isInt())
	{
		TrGoodItem* ret = new TrGoodItem();
		ret->id = jsonValue["id"].asInt();
		ret->goodId = jsonValue["good_id"].asInt()-1;
		ret->num = jsonValue["num"].asInt();
		ret->levelStrict = jsonValue["level_strict"].asInt();
		ret->stageStrict = jsonValue["gk_id"].asInt()-1;
		ret->chargeId = jsonValue["charge_id"].asCString();
		ret->jh_id = jsonValue["jh_id"].asInt();
		if (jsonValue["rebuy"].isInt()){
			ret->rebuy = jsonValue["rebuy"].asInt();
		}
		else{
			ret->rebuy = 0;
		}

        if( jsonValue["in_secret_shop"].isNull() || !jsonValue["in_secret_shop"].isInt()) {
            ret->isInSecretShop = false;
        } else {
            ret->isInSecretShop = jsonValue["in_secret_shop"].asInt() == 1 ? true : false;
        }
        if( jsonValue["level"].isNull() || !jsonValue["level"].isInt()) {
            ret->goodLevel = 0;
        } else {
            ret->goodLevel = jsonValue["level"].asInt();
        }
		ret->is_free = ret->chargeId->equals("charge0");


		if (jsonValue["is_display"].isNull() || !jsonValue["is_display"].isInt())
		{
			ret->is_display = 1;
		}
		else
		{
			ret->is_display = jsonValue["is_display"].asInt();
		}

		return ret;
	}
	return NULL;
}











//////////////////////////////////////////////////////////////////////////////////////////////////////////


TrGoodCharge::TrGoodCharge()
{
	mGoodList = new DList<TrGoodItem>(true);
}

TrGoodCharge::~TrGoodCharge()
{
	SAFE_DELETE(mGoodList);
}

bool TrGoodCharge::loadJsonValue(Json::Value jsonValue)
{
	Json::Value line = jsonValue;

	if (!line.isArray())
	{
		return false;
	}
	mGoodList->removeAllElements();

	for (unsigned int i = 0; i < line.size(); i++)
	{
		TrGoodItem* goodItem = TrGoodItem::parseJsonValue(line[i]);
		if (goodItem == NULL)
		{
			mGoodList->removeAllElements();
			return false;
		}
		mGoodList->addElement(goodItem);
	}

	return true;
}

bool TrGoodCharge::loadItunesPriceJsonValue(const char* sData)
{
	
    Json::Reader reader;
    Json::Value line;
	if (!reader.parse(sData, line, false))
	{
		trData->isLoadItunesPriceSuccess = 0;
		return false;
	}
	bool isrefresh = false;
	for(unsigned int j = 0; j < line.size(); j++)
	{
        for (int i = 0; i < mGoodList->size(); i++)
        {
            TrGoodItem* goodItem = mGoodList->elementAt(i);
            
			if (goodItem != NULL&&!goodItem->isFree())
            {
                TrPayInfo::TrChargeChannel* channel = trData->trGamePay->getPayInfo()->findChargeItem(goodItem->chargeId->getChar())->findChannelItemById(TrPayInfo::CHANNEL_IOSPAY);
                if(channel!=NULL){
                DString productId = channel->iosProductID;
                    if (productId->equals(line[j]["productIdentifier"].asCString())) {
						isrefresh = true;
                        goodItem->itunesPrice = line[j]["price"].asDouble()*100;
                        goodItem->iosPriceToDisplay = line[j]["priceToDisplay"].asCString();
                        break;
                    }
                } 
            }
        }
    }
	if (isrefresh)
	{
		trData->isLoadItunesPriceSuccess = 1; 
	}
	else
	{
		trData->isLoadItunesPriceSuccess = 0;
	}
	return true;
}

bool TrGoodCharge::isItunesPriceLoadSuccess()
{
        for (int i = 0; i < mGoodList->size(); i++)
        {
            TrGoodItem* goodItem = mGoodList->elementAt(i);
            
            if (goodItem != NULL)
            {
                if (!goodItem->isFree()&&goodItem->itunesPrice==-1) {
                     return false;
                }
                
            }
        }
    
    return true;
}

Json::Value TrGoodCharge::toJsonValue()
{
	Json::Value ret;

	for (int i = 0; i < getGoodSize(); i++)
	{
		ret.append(getGoodItem(i)->toJsonValue());
	}

	return ret;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

TrGamePay::TrGamePay()
{
	mPayInfo = new TrPayInfo();
	mGoodCharge = new TrGoodCharge();
}

TrGamePay::~TrGamePay()
{
	SAFE_DELETE(mPayInfo);
	SAFE_DELETE(mGoodCharge);
}

bool TrGamePay::loadJsonValue(Json::Value root)
{
	return mGoodCharge->loadJsonValue(root["store_data"]) && mPayInfo->loadJsonValue(root["charge_data"]);
}



Json::Value TrGamePay::loadFileJsonValue(const char* file){
	Json::Reader reader;
	Json::Value line;

	char* sData = ChessReadFile(file);
	if (sData != NULL)
	{
		if (reader.parse(sData, line, false))
		{
			ChessFree(sData);
			return line;
		}
		ChessFree(sData);
	}
	return line;

}




void TrGamePay::loadDefault()
{
	//mGoodCharge->loadJsonValue(loadFileJsonValue("json/good_charge.json"));
	mPayInfo->loadJsonValue(loadFileJsonValue("json/trpay.json"));
}

void TrGamePay::loadItunesPrice(const char* json)
{
	mGoodCharge->loadItunesPriceJsonValue(json);
}

bool TrGamePay::isItunesPriceLoadSuccess()
{
    return mGoodCharge->isItunesPriceLoadSuccess();
}

Json::Value TrGamePay::toJsonValue()
{
	Json::Value ret;
	ret["store_data"] = mGoodCharge->toJsonValue();
	ret["charge_data"] = mPayInfo->toJsonValue();
	return ret;
}

bool TrGamePay::loadConfig()
{
	char* sData = ChessReadFile(FILE_GAME_PAY, 1);
	if (sData == NULL)
	{
		return false;
	}
	Json::Reader reader;
	Json::Value root; 
	if (!reader.parse(sData, root, false))
	{
		ChessFree(sData);
		return false;
	}
	ChessFree(sData);
	return loadJsonValue(root);
}

bool TrGamePay::saveConfig()
{
	Json::FastWriter fast_writer;

	std::string out = fast_writer.write(toJsonValue());
	ChessWriteFile(FILE_GAME_PAY, (char*)out.c_str(), out.size());
	return true;
}

bool TrGamePay::checkData()
{
	DList<TrGoodCharge::TrGoodItem>* deleteList = new DList<TrGoodCharge::TrGoodItem>(false);
	for (int i = 0; i < mGoodCharge->getGoodSize(); i++)
	{
		if (mGoodCharge->getGoodItem(i)->isFree())
		{
			continue;
		}
		if (mPayInfo->findChargeItem(mGoodCharge->getGoodItem(i)->chargeId->getChar()) == NULL)
		{
			return false;
		}
	}

	for (int i = 0; i < deleteList->size(); i++)
	{
		mGoodCharge->removeGoodCharge(deleteList->elementAt(i));
	}
	SAFE_DELETE(deleteList);
	return true;
}