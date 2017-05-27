//
//  codec.h
//  trgameEncoder
//
//  Created by shenliming on 15/2/10.
//  Copyright (c) 2015年 shenliming. All rights reserved.
//

#ifndef __trgameEncoder__codec__
#define __trgameEncoder__codec__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SAFE_DELETE(pObj)  if(pObj!=NULL){delete pObj; (pObj)=NULL;}
#define SAFE_DELETE_ARRAY(pArray)  if(pArray!=NULL){delete []pArray; pArray=NULL;}


/**
 * SecretKey 密钥类
 */
class SecretKey
{
public:
    
    /**
     * 构造函数
     * 初始化 index，useTimes，secretLength,secretKey,secretKeyCopy;
     */
    SecretKey();
    
    /**
     * 析构函数
     * 释放 secretKey secretKeyCopy
     */
    ~SecretKey();
    
    /**
     * 设置密钥
     * @param secret 密钥
     * @param length 长度
     */
    void setSecretKey(char* secret, int length);
    
    /**
     * 单字节加密（异或）操作
     * @param [in] [out] data 需要加密的字节地址
     * @param needChange 是否需要密钥变换
     */
    void doSecret(unsigned char* data, bool needChange);
    
    /**返回密钥长度*/
    int getSecretLength();
    
    /**重置密钥*/
    void resetToDefault();
    
    
    
private:
    
    /**密钥*/
    char* secretKey;
    /**密钥长度*/
    char secretLength;
    /**初始密钥*/
    char* secretKeyCopy;
    /**使用次数*/
    int useTimes;
    /**下标*/
    int index;
    
    /**密钥变换*/
    void secretChange();
    
};

/**
 * SecretFile 文件加、解密类
 */
class SecretFile
{
    
public:
    
    /**
     * 构造函数
     * 初始化 codeSecret,fileSecret,versionCode
     */
    SecretFile();
    
    /**
     * 析构函数
     * 释放 codeSecret,fileSecret
     */
    ~SecretFile();
    
    /**
     * 设置代码密钥
     * @param versionCode 版本号
     * @return true版本号存在，false版本号不存在
     */
    bool setCodeSecret(int versionCode);
    
    /**
     * 解密文件
     * @param file 需要解密的内容
     * @param length 内容长度
     * @param [out] afterLength 解密后内容的长度
     * @return 返回解密后的内容，需要在外部释放,如果失败返回NULL
     */
    char* decryptData(char* file, int length,int* afterLength);
    
    /**
     * 加密文件
     * @param file 需要加密的内容
     * @param length 内容长度
     * @param [out] afterLength 加密后内容的长度
     * @return 返回加密后的内容，需要在外部释放
     */
    char* encryptData(char* data, int length, int* afterLength);
    
    char* encryptDataPartial(char* fileData, int fileLength, int encodeLength, int* afterLength);

    void setCodeSecretKey(char* key);

private:
    
    /**代码密钥*/
    SecretKey* codeSecret;
    /**文件密钥*/
    SecretKey* fileSecret;
    /**版本号*/
    int versionCode;
    
    /**
     * 字符数组转int（小端字节序）
     * @param value 需要转换的字符数组
     * @return 返回转换后的int
     */
    unsigned int charsToInt(unsigned char* ch);//unsigned char算数移位，char 逻辑移位
    
    /**
     * int转字符数组（小端字节序）
     * @param i 需要转换的整数
     * @param [out] value 转换后的字符数组
     */
    void intToChars(unsigned int i, unsigned char* value);
    
    /**
     * 随机产生文件密钥
     * @param len 文件密钥长度
     * @param [out] out 文件密钥地址
     */
    void randomFileSecret(int len,char* out);

    char * genSecretKeyByFile(char* fileData, int fileLength, unsigned char * keyLength);

    
};



#endif /* defined(__trgameEncoder__codec__) */
