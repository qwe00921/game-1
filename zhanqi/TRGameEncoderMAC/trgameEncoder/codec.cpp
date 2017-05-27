//
//  codec.cpp
//  trgameEncoder
//
//  Created by shenliming on 15/2/10.
//  Copyright (c) 2015年 shenliming. All rights reserved.
//

#include <iostream>
#include "codec.h"


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


void SecretKey::setSecretKey(char* secret, int length){
    
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
            codeSecret->setSecretKey("gYjhBMzjNtSaVYDV", 16);
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
    
    unsigned char randomFileSecretLength = rand()%17+16;
    char rFileSecret[33];
    randomFileSecret(randomFileSecretLength, rFileSecret);
    fileSecret->setSecretKey(rFileSecret, randomFileSecretLength);
    int fileLength = 22 + randomFileSecretLength + length;
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
    unsigned char  unKnowFileSecretLength = randomFileSecretLength;
    int codeSecretLength = codeSecret->getSecretLength();
    codeSecret->resetToDefault();
    for (int i = 0; i < codeSecretLength; i++)
    {
        codeSecret->doSecret(&unKnowFileSecretLength, false);
    }
    encodeData[index] = unKnowFileSecretLength;
    index++;
    
    //文件密钥内容
    memcpy(encodeData + index, rFileSecret, randomFileSecretLength);
    codeSecret->resetToDefault();
    for (int i = 0; i < randomFileSecretLength; i++)
    {
        *(encodeData + index+i) ^= randomFileSecretLength;
        codeSecret->doSecret(encodeData + index + i, false);
    }
    index += randomFileSecretLength;
    
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

char * SecretFile::genSecretKeyByFile(char* fileData, int fileLength, unsigned char * keyLength){
    *keyLength = fileLength % 17 + 16;
    char * key = new char[*keyLength];
    memset(key, 0, *keyLength);
    int i = 0, j = 0;
    while(j < fileLength) {
        key[i] = key[i] ^ fileData[j];
        j++;
        i++;
        if ( i == *keyLength )
            i = 0;
    }
    
    return key;
}

void SecretFile::setCodeSecretKey(char* keyString) {
    codeSecret->setSecretKey(keyString, strlen(keyString));
}

char* SecretFile::encryptDataPartial(char* fileData, int fileLength, int encodeLength, int* afterLength){
    
    if (fileData == NULL || fileLength < 0 || encodeLength < 0 || afterLength == NULL)
    {
        std::cout << "ERROR, encryptDataPartial got wrong parameters!" << std::endl;
        return NULL;
    }
    
    int realEncodeLength = 0;
    if (encodeLength < fileLength) {
        realEncodeLength = encodeLength;
    } else {
        realEncodeLength = fileLength;
    }
    
    codeSecret->resetToDefault();
    
    unsigned char fileKeyLength = 0;
    char *fileKey = genSecretKeyByFile(fileData, fileLength, &fileKeyLength);

//    {
//        printf("FileKey is:\n");
//        for(int i =0; i < fileKeyLength; i++){
//            printf("%02x ", ((unsigned char *)fileKey)[i]);
//        }
//        printf("\n");
//    }

    fileSecret->setSecretKey(fileKey, fileKeyLength);
    fileSecret->resetToDefault();
    
    int newFileLength = 22 + fileKeyLength + fileLength;
    *afterLength = newFileLength;
    unsigned char* encodeData = new unsigned char[newFileLength];
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
    intToChars(newFileLength, encodeData + index);
    index += 4;
    
    int checkCodeIndex = index;
    index++;
    
    //文件密钥长度
    unsigned char  unKnowFileSecretLength = fileKeyLength;
    int codeSecretLength = codeSecret->getSecretLength();
    codeSecret->resetToDefault();
    for (int i = 0; i < codeSecretLength; i++)
    {
        codeSecret->doSecret(&unKnowFileSecretLength, false);
    }
    encodeData[index] = unKnowFileSecretLength;
    index++;
    
    //文件密钥内容
    memcpy(encodeData + index, fileKey, fileKeyLength);
    codeSecret->resetToDefault();
    for (int i = 0; i < fileKeyLength; i++)
    {
        *(encodeData + index+i) ^= fileKeyLength;
        codeSecret->doSecret(encodeData + index + i, false);
    }
    index += fileKeyLength;
    
    //包体长度
    codeSecret->resetToDefault();
    fileSecret->resetToDefault();
    
    *(encodeData + index) = fileLength & 0xff;
    fileSecret->doSecret(encodeData + index, false);
    codeSecret->doSecret(encodeData + index, false);
    
    *(encodeData + index+1) = fileLength >> 8 & 0xff;
    fileSecret->doSecret(encodeData + index + 1, false);
    codeSecret->doSecret(encodeData + index + 1, false);
    
    *(encodeData + index + 2) = fileLength >> 16 & 0xff;
    fileSecret->doSecret(encodeData + index + 2, false);
    codeSecret->doSecret(encodeData + index + 2, false);
    
    *(encodeData + index + 3) = fileLength >> 24 & 0xff;
    fileSecret->doSecret(encodeData + index + 3, false);
    codeSecret->doSecret(encodeData + index + 3, false);
    index += 4;
    
    //加密长度
    codeSecret->resetToDefault();
    fileSecret->resetToDefault();
    
    //全部加密，加密长度=包体长度
    *(encodeData + index) = encodeLength & 0xff;
    fileSecret->doSecret(encodeData + index, false);
    codeSecret->doSecret(encodeData + index, false);
    
    *(encodeData + index + 1) = encodeLength >> 8 & 0xff;
    fileSecret->doSecret(encodeData + index + 1, false);
    codeSecret->doSecret(encodeData + index + 1, false);
    
    *(encodeData + index + 2) = encodeLength >> 16 & 0xff;
    fileSecret->doSecret(encodeData + index + 2, false);
    codeSecret->doSecret(encodeData + index + 2, false);
    
    *(encodeData + index + 3) = encodeLength >> 24 & 0xff;
    fileSecret->doSecret(encodeData + index + 3, false);
    codeSecret->doSecret(encodeData + index + 3, false);
    index += 4;
    
    //包体加密部分
    memcpy(encodeData + index, fileData, realEncodeLength);
    codeSecret->resetToDefault();
    fileSecret->resetToDefault();
    for (int i = 0; i < realEncodeLength; i++)
    {
        fileSecret->doSecret(encodeData + index + i, true);
        codeSecret->doSecret(encodeData + index + i, true);
    }
    if (realEncodeLength < fileLength)
        memcpy(encodeData + index + realEncodeLength, fileData + realEncodeLength, fileLength - realEncodeLength);
    
    //校验码
    encodeData[checkCodeIndex] = encodeData[0];
    for (int i = 1; i < newFileLength; i++)
    {
        if (checkCodeIndex != i)
        {
            encodeData[checkCodeIndex] ^= encodeData[i];
        }
        
    }
    
    free(fileKey);
    
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

void SecretFile::randomFileSecret(int len,char* out){
    
    if (out == NULL || len<16 || len>32)
    {
        return;
    }
    
    for (int i = 0; i < len; i++)
    {
        out[i] = rand() % 256;
    }
    
    
}