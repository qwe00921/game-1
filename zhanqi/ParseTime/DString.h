#ifndef CLASS_DFC_LANG_String
#define CLASS_DFC_LANG_String


class DStringBuf;
class DString
{	
protected:
	DStringBuf* m_pObject;
public:		
	DString(){m_pObject=0;};
	DString(DStringBuf* p){m_pObject=0;assign(p);}
	DString(const DString& r){m_pObject=0;assign(r.m_pObject);}		
	DString(const char* pch);	
	DString(const char* pch, int subLength);
	~DString();	
public:
	const DString& operator=(const DString& r) { assign(r); return *this;};
	const DString& operator=(const char* pch);
	const DString& operator=(DStringBuf* p) { assign(p); return *this; };

	const DString& operator+=(const DString s);
	DString operator+(DString p2)const;
	DString operator+(const char* p2)const;
	
	bool operator==( const char* pch1) const;
	bool operator!=(const char* pch1) const;	
	bool operator==(  DString& s) const;
	
	char operator [](int aIndex) const;
	char& operator [](int aIndex);
	
	friend DString operator+(const char* pch1, DString str2);
	operator DStringBuf*() const { return m_pObject; };
	DStringBuf* operator->()
	{
		return (DStringBuf*)m_pObject;
	};
	char* getNativeCharPtr();
	bool isNull()
	{
		return m_pObject == 0;
	}
	bool isEmpty();
	bool IsNullOrEmpty()
	{
		return isNull() || isEmpty();
	}

protected:
	void assign(DStringBuf* p);		
};


class DStringBuf
{
protected:
	int	m_iRef;
public:	
	void RefLock();
	void RefRelease();	

protected:
	int len;
	char* m_pchData;	
protected:
	void alloc(int iSize);
	static DString cat(DStringBuf* str1, DStringBuf* str2);
	static DString cat(DStringBuf* str1, const char* pch2);
	static DString cat(const char* pch1, DStringBuf* str2);
	static DString cat(const char* pch1, const char* pch2);
	DStringBuf* clone();
public:
	DStringBuf();
 	DStringBuf(const char* pchText);
	DStringBuf(const char* pchText, int subStrLen);
	DStringBuf(DString original);
	virtual ~DStringBuf();

public:
	
	int length() { return len; };
	char charAt(int pos);
	/** Compares this StringBuf to the #anotherString string, ignoring case */
	bool equals(DString &anotherString);
	/** Compares this string to the #anotherString string. */
	bool equals(const char* anotherString);
	int compareTo(DString &anotherString);
	int compareTo(const char* anotherString);
	/** Tests if this string starts with the #prefix prefix, starting at the #toffest index. */
	bool startsWith(DString prefix, int toffset);
	/** Tests if this string starts with the #prefix prefix. */
	bool startsWith(DString prefix);
	/** Tests if this string ends with the #suffix suffix. */
	bool endsWith(DString suffix);
	/** Returns the index within this string of the first occurrence of the #ch character. */
	int indexOf(int ch);
	/** Returns the index within this string of the first occurrence of the #ch character, starting at the #fromIndex index. */
	int indexOf(int ch, int fromIndex);
	/** Returns the index within this string of the last occurrence of the #ch character. */
	int lastIndexOf(int ch);
	/** Returns the index within this string of the last occurrence of the #ch character, starting at the #fromIndex index. */
	int lastIndexOf(int ch, int fromIndex);
	/** Returns the index within this string of the first occurrence of the #str string. */
	int indexOf(DString str);
	/** Returns the index within this string of the first occurrence of the #str string, starting at the #fromIndex index. */
	int indexOf(DString str, int fromIndex);
	/** Returns the index within this string of the last occurrence of the #str string. */
	int lastIndexOf(DString str);
	/** Returns the index within this string of the last occurrence of the #str string, starting at the #fromIndex index. */
	int lastIndexOf(DString str, int fromIndex);
	/** Returns a string containing the substring from #beginIndex up to the end of this string*/
	DString substring(int beginIndex);
	/** Returns a string containing the substring from #beginIndex up to, but not including, #endIndex */
	DString substring(int beginIndex, int endIndex);
	/** Returns a new copy of this string and replaces all occurrences of #olchar character in the returned string with #newChar character. */
	DString replace(char olchar, char newChar);
	/** Creates a new copy of this string and removes white space from both ends of the returned string. */
	DString trim();
	/** Creates a new copy of this string and reverse all chars. */
	DString reverse();
	virtual DString toString();

	/** Returns a new string object containing #c character */
	static DString valueOf(char c);
	static DString valueOf(int i);

	char* getNativeCharPtr() { return m_pchData; };

	friend class DString;
	friend DString operator+(const char* pch1, DString str2);
	friend DString operator+(char ch1, DString str2);
};


char *IntToString(int i);
int StringToInt(const char* str);

#endif

