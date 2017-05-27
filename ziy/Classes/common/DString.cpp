#include "Common.h" 
#include <stdarg.h>

DStringBuf::DStringBuf() 
{
	m_iRef= 0; 
	len=0;
	m_pchData=NULL;
};

DStringBuf::DStringBuf(const char* pchText)
{
	m_iRef= 0;
	len=0;
	m_pchData=NULL;
	len=strlen(pchText);
	alloc(len+1);
	memcpy(m_pchData, pchText, (len+1)*sizeof(char));
};

DStringBuf::DStringBuf(const char* pchText, int subStrLen)
{
	m_iRef = 0;
	len = 0;
	m_pchData = NULL;

	alloc(subStrLen + 1);
	memcpy(m_pchData, pchText, subStrLen*sizeof(char));
	len = strlen(m_pchData);
}

DStringBuf::DStringBuf(DString original)
{	
	m_iRef= 0;
	m_pchData=NULL;
	len=original->length();
	alloc(len+1);
	memcpy(m_pchData, original->getChar(), (len+1)*sizeof(char));
}


DStringBuf::~DStringBuf()
{
	if(m_pchData!=NULL)
	{
		delete []m_pchData; 
		m_pchData=NULL;
	}
}

DString DStringBuf::cat(DStringBuf* str1, DStringBuf* str2)
{
	DStringBuf* ret= new DStringBuf();
	ret->len= str1->len+str2->len;
	ret->alloc(ret->len+1);
	memcpy(ret->m_pchData, str1->m_pchData, str1->len*sizeof(char));
	memcpy(ret->m_pchData+str1->len, str2->m_pchData,
		(str2->len+1)*sizeof(char));
	return DString(ret);
};
DString DStringBuf::cat(DStringBuf* str1, const char* pch2)
{
	DStringBuf* ret= new DStringBuf();
	int len1;
	int len2= strlen(pch2);
	if(str1 == NULL)
		len1 = 0;
	else
		len1= str1->len;
	ret->len = len1+len2;
	ret->alloc(ret->len+1);
	if(str1!=NULL)
		memcpy(ret->m_pchData, str1->m_pchData, str1->len*sizeof(char));
	memcpy(ret->m_pchData+len1, pch2, (len2+1)*sizeof(char));
	return DString(ret);
};
DString DStringBuf::cat(const char* pch1, DStringBuf* str2)
{
	DStringBuf* ret= new DStringBuf();
	int len1= strlen(pch1);
	ret->len= len1+str2->len;
	ret->alloc(ret->len+1);
	memcpy(ret->m_pchData, pch1, len1*sizeof(char));
	memcpy(ret->m_pchData+len1, str2->m_pchData, (str2->len+1)*sizeof(char));
	return DString(ret);
};
DString  DStringBuf::cat(const char* pch1, const char* pch2)
{
	DStringBuf* ret= new DStringBuf();
	int len1= strlen(pch1);
	int len2= strlen(pch2);
	ret->len= len1+len2;
	ret->alloc(ret->len+1);
	memcpy(ret->m_pchData, pch1, len1*sizeof(char));
	memcpy(ret->m_pchData+len1, pch2, (len2+1)*sizeof(char));
	return DString(ret);
}

DString DStringBuf::substring(int beginIndex, int endIndex)
{
	DStringBuf* ret= new DStringBuf();
	ret->len= endIndex-beginIndex;
	ret->alloc(ret->len+1);
	memcpy(ret->m_pchData, m_pchData+beginIndex, ret->len*sizeof(char));
	ret->m_pchData[ret->len]= 0;
	return ret;
}
DString DStringBuf::substring(int beginIndex)
{
	return substring(beginIndex, len);
}
int DStringBuf::indexOf(int ch)
{
	return indexOf(ch, 0);
}
int DStringBuf::indexOf(int ch, int fromIndex)
{
	char* ret= strchr(m_pchData+fromIndex, ch);
	return (ret==0)? -1 : ret-m_pchData;
}
int DStringBuf::indexOf(DString str)
{
	return indexOf(str, 0);
}
int DStringBuf::indexOf(DString str, int fromIndex)
{
	char* ret= strstr(m_pchData+fromIndex, str->getChar());
	return (ret==0)? -1 : ret-m_pchData;
}
int DStringBuf::lastIndexOf(int ch)
{
	char* ret= strchr(m_pchData, ch);
	return (ret==0)? -1 : ret-m_pchData;
}

int DStringBuf::lastIndexOf(int ch, int fromIndex)
{
	if(fromIndex>=len) fromIndex= len-1;
	for(;fromIndex>=0; fromIndex--)
	{
		if(m_pchData[fromIndex]==ch) return fromIndex;
	}
	return -1;
}
int DStringBuf::lastIndexOf(DString str)
{
	return lastIndexOf(str, len-1);
}

int DStringBuf::lastIndexOf(DString str, int fromIndex)
{
	if(fromIndex+str->len>len) fromIndex= len-str->len;
	for(;fromIndex>=0; fromIndex--)
	{
		if(startsWith(str, fromIndex)) return fromIndex;
	};
	return -1;
}

char DStringBuf::charAt(int pos)
{
	return m_pchData[pos];
}

int DStringBuf::compareTo(DString & anotherString)
{
	return strcmp(m_pchData, anotherString->m_pchData);
}
int DStringBuf::compareTo(const char* anotherString)
{
	return strcmp(m_pchData, anotherString);
}

bool DStringBuf::startsWith(DString prefix)
{
	if(len<prefix->len) return false;
	return memcmp(m_pchData, prefix->m_pchData, (prefix->len)*sizeof(char))==0;
}
bool DStringBuf::startsWith(DString prefix, int toffset)
{
	if(toffset<0) return false;
	if(len<prefix->len+toffset) return false;
	return memcmp(m_pchData+toffset, prefix->m_pchData, (prefix->len)*sizeof(char))==0;
}

int DStringBuf::hashCode()
{
	int hash=0;
		for(int i=0; i<len; i++)
		{
			hash= 31*hash+(int)(UUCHAR)m_pchData[i];
		};
	return hash;
}


bool DStringBuf::equals( DString& anotherString)
{
	return anotherString!=NULL && compareTo(anotherString)==0;
}
bool DStringBuf::equals(const char* anotherString)
{
	return strcmp(m_pchData, anotherString)==0;
}

bool DStringBuf::endsWith(DString suffix)
{
	if(len<suffix->len) return false;
	return memcmp(m_pchData+(len-suffix->len),
		suffix->m_pchData, suffix->len*sizeof(char))==0;
}

DString DStringBuf::trim()
{
	int start, end;
	
	if(len==0) return this;
	for(start= 0; m_pchData[start]==' ' && start<len; start++);
	for(end= len-1; end>=start && m_pchData[end]==' '; end--);
	if(start==0 && end==len-1) return this;
	return substring(start, end+1);
}

DStringBuf* DStringBuf::clone()
{
	DStringBuf* ret= new DStringBuf();
	ret->len= len;
	ret->alloc(len+1);
	memcpy(ret->m_pchData, m_pchData, (len+1)*sizeof(char));
	return ret;
}
DString DStringBuf::replace(char olchar, char newChar)
{
	if(len==0) return this;
	DStringBuf* ret = clone();
	for (int i = 0; i < len; i++)
	{
		if (ret->m_pchData[i] == olchar)
		{
			ret->m_pchData[i] = newChar;
		}
	}

	return ret;
}

DString DStringBuf::toString() { return this; };


DString DStringBuf::valueOf(char c)
{
	char buf[2];
	buf[0]= c;
	buf[1]= 0;
	return new DStringBuf(buf);
}

DString DStringBuf::valueOf(int i) {
	return new DStringBuf(IntToString(i));
}


 DString::DString(const char* pch) 
 {
	 m_pObject=0;
	assign((pch==0) ? NULL :new DStringBuf(pch));
 }

DString::DString(const char* pch, int subLength)
{
	m_pObject = 0;
	assign((pch == 0) ? NULL : new DStringBuf(pch, subLength));
}
 
DString::~DString()
{
	 if(m_pObject!=0) m_pObject->RefRelease();

}

DString DString::format(const char* fmt, ...)
{
	va_list args;
	char* buf = new char[2048];
	memset(buf, 0, 2048);

	va_start(args, fmt);
	//_vsnprintf(buf, 2047, fmt, args);
	vsnprintf(buf, 2047, fmt, args);
	va_end(args);

	DString ret = buf;
	delete[] buf;
	return ret;
}

char* DString::getChar()
{
	return m_pObject->getChar(); 
}

bool DString::isEmpty()
{
	return m_pObject->length() == 0;
}

void DString::assign(DStringBuf* p)
{
	if(p!=0) p->RefLock();
	if(m_pObject!=0) m_pObject->RefRelease();
	m_pObject= p;
}

 const DString& DString::operator=(const char* pch)
{ 
	assign((pch==0) ? NULL : new DStringBuf(pch)); return *this; 
}

 DString DString::operator+(DString str2)const
{
	return DStringBuf::cat((DStringBuf*)m_pObject, (DStringBuf*)str2.m_pObject);
}
 DString DString::operator+(const char* pch2)const
{
	return DStringBuf::cat((DStringBuf*)m_pObject, pch2);
}

 DString operator+(const char* pch1, DString str2)
{
	return DStringBuf::cat(pch1, (DStringBuf*)str2.m_pObject);
}

 const DString& DString::operator+=(const DString s)
{
	assign(DStringBuf::cat((DStringBuf*)m_pObject, (DStringBuf*)s.m_pObject));
	return *this;
}


bool DString::operator==(const char* pch1 ) const
{
	
	if (m_pObject==NULL&&pch1!=NULL)
		return false;
	if (m_pObject!=NULL&&pch1==NULL)
		return false;
	if (m_pObject==NULL&&pch1==NULL)
		return true;
	
	return 0==((DStringBuf*)m_pObject)->compareTo(pch1);
	
}
bool DString::operator!=(const char* pch1) const
{
	return !((*this)==pch1);	
}


bool DString::operator==( DString& s) const
{
	return 0==((DStringBuf*)m_pObject)->compareTo(s);
}

char DString::operator [](int aIndex) const
{
	if ((aIndex<0)||(aIndex>((DStringBuf*)m_pObject)->len))
		return 0;
	return ((DStringBuf*)m_pObject)->m_pchData[aIndex];
}

char& DString::operator [](int aIndex)
{
	if ((aIndex<0)||(aIndex>((DStringBuf*)m_pObject)->len))
	{
		
	}
	return (char&)((DStringBuf*)m_pObject)->m_pchData[aIndex];
}

void DStringBuf::RefLock()
{
	m_iRef++;
}

void DStringBuf::RefRelease()
{
	m_iRef--;
	if(m_iRef==0)
	{
		delete this;
	}
}
void DStringBuf::alloc(int iSize)
{
	iSize= (iSize/4+(((iSize%4)==0)?0:1))*4;
	m_pchData= new char[iSize];
	memset(m_pchData, 0, iSize);
}

DString DStringBuf::reverse()
{
	DStringBuf* ret = new DStringBuf();
	ret->len = len;
	ret->alloc(ret->len + 1);
	for(int i = 0; i < len; i++)
	{
		ret->m_pchData[i] = m_pchData[len - i - 1];
	}
	ret->m_pchData[ret->len] = 0;
	return ret;
}
