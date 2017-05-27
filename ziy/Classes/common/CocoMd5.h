#ifndef _COCOS_MD5_H 
#define _COCOS_MD5_H 
#include "Common.h"
#include "main.h"
//typedef unsigned char byte;
//typedef unsigned int uint32;

class CocosMD5 {
public:
	CocosMD5();
	void update(const std::string& str);
	std::string toString();
	void reset();
	const unsigned char* digest();
	std::string ToMD5(const std::string& str);

private:
	void final();
	void update(const unsigned char* input, size_t length);
	void transform(const unsigned char block[64]);
	void encode(const unsigned int* input, unsigned char* output, size_t length);
	void decode(const unsigned char* input, unsigned int* output, size_t length);
	std::string bytesToHexString(const unsigned char* input, size_t length);
	CocosMD5(const CocosMD5&);
	CocosMD5& operator=(const CocosMD5&);

private:
	unsigned int _state[4]; /* state (ABCD) */
	unsigned int _count[2]; /* number of bits, modulo 2^64 (low-order word first) */
	unsigned char _buffer[64]; /* input buffer */
	unsigned char _digest[16]; /* message digest */
	bool _finished; /* calculate finished ? */

	static const unsigned char PADDING[64]; /* padding for calculate */
	static const char HEX[16];
	enum { BUFFER_SIZE = 1024 };
};

#endif /*MD5_H*/