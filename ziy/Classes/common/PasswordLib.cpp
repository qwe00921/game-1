﻿#include "PasswordLib.h"
#include "Common.h"





SecretKey::SecretKey(){
	index = 0;
	useTimes = 0;
	secretLength = 0;
	secretKey = NULL;
	secretKeyCopy = NULL;
}
SecretKey::~SecretKey(){
	SAFE_DELETE_ARRAY(secretKey);
	SAFE_DELETE_ARRAY(secretKeyCopy);
}


void SecretKey::setSecretKey(const char* secret, int length){

	SAFE_DELETE_ARRAY(secretKey);
	SAFE_DELETE_ARRAY(secretKeyCopy);

	index = 0;
	useTimes = 0;

	secretKey = new char[length];
	memcpy(secretKey, secret, length);
	secretLength = length;

	secretKeyCopy = new char[length];
	memcpy(secretKeyCopy, secret, length);
}


void SecretKey::computeSecretKey(int minLength, int maxLength, const char* data, int dataLen)
{
	SAFE_DELETE_ARRAY(secretKey);
	SAFE_DELETE_ARRAY(secretKeyCopy);

	index = 0;
	useTimes = 0;

	secretLength = minLength + dataLen % (maxLength - minLength + 1);
	secretKey = new char[secretLength];
	memset(secretKey, 0, secretLength);

	for (int i = 0; i < dataLen; i++)
	{
		int index = i % secretLength;
		secretKey[index] = (char)(secretKey[index] ^ data[i]);
	}

	secretKeyCopy = new char[secretLength];
	memcpy(secretKeyCopy, secretKey, secretLength);
}

void SecretKey::doSecret(unsigned char* data, bool needChange){

	*data = *data^ *(secretKey + index);
	index++;
	if (index == secretLength){

		index = 0;

		if (needChange)
		{
			useTimes++;
			secretChange();
		}
	}

}



void SecretKey::resetToDefault(){
	index = 0;
	useTimes = 0;
	if (0 != secretLength&&secretKey != NULL&&secretKeyCopy!=NULL)
	{
		memcpy(secretKey, secretKeyCopy, secretLength);
	}

	
}




void SecretKey::secretChange(){

	int changeIndex = useTimes%secretLength;

	char change = (useTimes * 7) & 0xff;
	secretKey[changeIndex] = (secretKey[changeIndex] + change) & 0xff;

}

int SecretKey::getSecretLength(){

	return secretLength;
}

char* SecretKey::getSecretKey()
{
	return secretKey;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////

SecretFile::SecretFile(){
	codeSecret = new SecretKey();
	fileSecret = new SecretKey();
	versionCode = 0;
}
SecretFile::~SecretFile(){
	SAFE_DELETE(codeSecret);
	SAFE_DELETE(fileSecret);
}





bool SecretFile::setCodeSecret(int vCode){

	versionCode = vCode;
	switch (vCode)
	{
	case 1:
		codeSecret->setSecretKey("gYjhBMzjNtSaVYDv", 16);
		return true;
	default:
		return false;
	}
}

char* SecretFile::decryptData(char* file, int length, int* afterLength){

	if (file == NULL || length<38 || afterLength==NULL)
	{
		return NULL;
	}

	codeSecret->resetToDefault();
	fileSecret->resetToDefault();

	//核对校验码
	unsigned char checkCode = *(file + 12);
	unsigned char temp = *file;
	for (int i = 1; i < length;i++)
	{
		if (12!=i)
		{
			temp ^= *(file + i);
		}
	}
	if (temp != checkCode)
	{
		//校验失败
		return NULL;
	}

	int index = 0;
	//识别码
	if (strncmp(file, "BGT1", 4)!=0)
	{
		//识别码验证失败
		return NULL;
	}
	index += 4;

	//版本号
	int vCode = charsToInt((unsigned char*)file + index);
	if (!setCodeSecret(vCode))
	{
		return NULL;
	}
	index += 4;


	//文件长度
	unsigned int fileLength = charsToInt((unsigned char*)file + 8);
	index += 4;

	//校验码
	index++;

	//文件密钥长度
	unsigned char fileSecretLength = *(file + index);
	int codeSecretLength = codeSecret->getSecretLength();
	codeSecret->resetToDefault();
	for (int i = 0; i < codeSecretLength; i++)
	{
		codeSecret->doSecret(&fileSecretLength, false);
	}

	if (length<18 + fileSecretLength)
	{
		return NULL;
	}

	index++;

	//文件密钥
	codeSecret->resetToDefault();
	unsigned char fSecret[33] = {0};
	memcpy(fSecret, file + index, fileSecretLength);
	for (int i = 0; i < fileSecretLength; i++)
	{
		*(fSecret + i) ^= fileSecretLength;
		codeSecret->doSecret(fSecret + i, false);
	}
	fileSecret->setSecretKey((char*)fSecret, fileSecretLength);
	index += fileSecretLength;

	//包体长度
	codeSecret->resetToDefault();
	fileSecret->resetToDefault();
	unsigned char bodyLen[4] = { 0 };
	memcpy(bodyLen, file + index, 4);
	for (int i = 0; i < 4; i++)
	{
		codeSecret->doSecret(bodyLen + i, false);
		fileSecret->doSecret(bodyLen + i, false);
	}

	int bodyLength = charsToInt(bodyLen);

	if (bodyLength<0)
	{
		return NULL;
	}

	*afterLength = bodyLength;
	index += 4;

	//加密长度
	codeSecret->resetToDefault();
	fileSecret->resetToDefault();
	unsigned char secretLen[4] = { 0 };
	memcpy(secretLen, file + index, 4);
	for (int i = 0; i < 4; i++)
	{
		codeSecret->doSecret(secretLen + i, false);
		fileSecret->doSecret(secretLen + i, false);
	}

	int secretLength = charsToInt(secretLen);

	if (secretLength < 0)
	{
		return NULL;
	}
	else if (secretLength > bodyLength)
	{
		secretLength = bodyLength;
	}
	index += 4;

	if (22 + fileSecretLength + bodyLength != fileLength)
	{
		return NULL;
	}

	//包体
	unsigned char* body = new unsigned char[bodyLength];
	memcpy(body, file + index, bodyLength);
	codeSecret->resetToDefault();
	fileSecret->resetToDefault();

	for (int i = 0; i < secretLength; i++)
	{
		codeSecret->doSecret(body + i, true);
		fileSecret->doSecret(body + i, true);
	}

	return (char*)body;


}



char* SecretFile::encryptData(char* data, int length, int* afterLength){

	if (data == NULL || length < 0 || afterLength == NULL)
	{
		return NULL;
	}

	codeSecret->resetToDefault();
	fileSecret->resetToDefault();

	fileSecret->computeSecretKey(16, 32, data, length);
	int fileLength = 22 + fileSecret->getSecretLength() + length;
	*afterLength = fileLength;
	unsigned char* encodeData = new unsigned char[fileLength];
	int index = 0;
	//识别码
	encodeData[0] = 'B';
	encodeData[1] = 'G';
	encodeData[2] = 'T';
	encodeData[3] = '1';
	index += 4;

	//版本号
	int vCode = versionCode;
	intToChars(vCode, encodeData + index);
	index += 4;

	//文件长度
	intToChars(fileLength, encodeData + index);
	index += 4;

	int checkCodeIndex = index;
	index++;

	//文件密钥长度
	unsigned char  unKnowFileSecretLength = fileSecret->getSecretLength();
	int codeSecretLength = codeSecret->getSecretLength();
	codeSecret->resetToDefault();
	for (int i = 0; i < codeSecretLength; i++)
	{
		codeSecret->doSecret(&unKnowFileSecretLength, false);
	}
	encodeData[index] = unKnowFileSecretLength;
	index++;

	//文件密钥内容
	memcpy(encodeData + index, fileSecret->getSecretKey(), fileSecret->getSecretLength());
	codeSecret->resetToDefault();
	for (int i = 0; i < fileSecret->getSecretLength(); i++)
	{
		encodeData[index + i] ^= fileSecret->getSecretLength();
		codeSecret->doSecret(encodeData + index + i, false);
	}
	index += fileSecret->getSecretLength();

	//包体长度
	codeSecret->resetToDefault();
	fileSecret->resetToDefault();

	*(encodeData + index) = length & 0xff;
	fileSecret->doSecret(encodeData + index, false);
	codeSecret->doSecret(encodeData + index, false);

	*(encodeData + index+1) = length >> 8 & 0xff;
	fileSecret->doSecret(encodeData + index + 1, false);
	codeSecret->doSecret(encodeData + index + 1, false);

	*(encodeData + index + 2) = length >> 16 & 0xff;
	fileSecret->doSecret(encodeData + index + 2, false);
	codeSecret->doSecret(encodeData + index + 2, false);

	*(encodeData + index + 3) = length >> 24 & 0xff;
	fileSecret->doSecret(encodeData + index + 3, false);
	codeSecret->doSecret(encodeData + index + 3, false);
	index += 4;

	//加密长度
	codeSecret->resetToDefault();
	fileSecret->resetToDefault();

	//全部加密，加密长度=包体长度
	*(encodeData + index) = length & 0xff;
	fileSecret->doSecret(encodeData + index, false);
	codeSecret->doSecret(encodeData + index, false);

	*(encodeData + index + 1) = length >> 8 & 0xff;
	fileSecret->doSecret(encodeData + index + 1, false);
	codeSecret->doSecret(encodeData + index + 1, false);

	*(encodeData + index + 2) = length >> 16 & 0xff;
	fileSecret->doSecret(encodeData + index + 2, false);
	codeSecret->doSecret(encodeData + index + 2, false);

	*(encodeData + index + 3) = length >> 24 & 0xff;
	fileSecret->doSecret(encodeData + index + 3, false);
	codeSecret->doSecret(encodeData + index + 3, false);
	index += 4;

	//包体
	memcpy(encodeData + index,data,length);
	codeSecret->resetToDefault();
	fileSecret->resetToDefault();
	for (int i = 0; i < length; i++)
	{
		fileSecret->doSecret(encodeData + index + i, true);
		codeSecret->doSecret(encodeData + index + i, true);
	}

	//校验码
	encodeData[checkCodeIndex] = encodeData[0];
	for (int i = 1; i < fileLength; i++)
	{
		if (checkCodeIndex != i)
		{
			encodeData[checkCodeIndex] ^= encodeData[i];
		}

	}

	return (char*)encodeData;
}


unsigned int SecretFile::charsToInt(unsigned char* ch){

	return (ch[3] << 24) | (ch[2] << 16) | (ch[1] << 8) | ch[0];
}

void SecretFile::intToChars(unsigned int i, unsigned char* value){

	value[0] = i & 0xff;
	value[1] = i >> 8 & 0xff;
	value[2] = i >> 16 & 0xff;
	value[3] = i >> 24 & 0xff;
}

