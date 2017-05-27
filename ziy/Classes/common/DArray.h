#ifndef _D_ARRAY_H_
#define _D_ARRAY_H_


template<typename T>
class DArray
{
private:
	int m_iLength;
	T* m_pData;
public:
	DArray()
	{
		m_pData = NULL;
		m_iLength = 0;
	}
	DArray(int len)
	{
		if (len < 0)
		{
			len = 0;
		}
		m_pData = new T[len];
		m_iLength = len;
	}
	~DArray()
	{
		if (m_pData != NULL)
		{
			delete[] m_pData;
			m_pData = NULL;
		}
		m_iLength = 0;
	}
public:
	int size() const
	{
		return m_iLength;
	}
	void resize(int newLen)
	{
		if (newLen < 0)
		{
			newLen = 0;
		}
		T* newData = new T[newLen];
		if (m_pData != NULL)
		{
			delete[] m_pData;
			m_pData = NULL;
		}
		m_pData = newData;
		m_iLength = newLen;
	}
	void memset(const T& v)
	{
		for (int i = 0; i < m_iLength; i++)
		{
			m_pData[i] = v;
		}
	}
	bool contains(const T& v) const
	{		
		return contains(v, m_iLength);
	}
	bool contains(const T& v, int findLen) const
	{
		int newFindLen = findLen < m_iLength ? findLen : m_iLength;
		for (int i = 0; i < newFindLen; i++)
		{
			if (m_pData[i] == v)
			{
				return true;
			}
		}
		return false;
	}
public:
	DArray(const DArray& da)
	{
		m_iLength = da.m_iLength;
		m_pData = new T[m_iLength];
		for (int i = 0; i < m_iLength; i++)
		{
			m_pData[i] = da.m_pData[i];
		}
	}
	T& operator [](int index)
	{
		return m_pData[index];
	}
	operator T*() const
	{
		return m_pData;
	}
public:
	T* getAddress() const
	{
		return m_pData;
	}
	void copyFrom(const T* src, int len)
	{
		int copyLen = len<m_iLength ? len : m_iLength;
		for (int i = 0; i < copyLen; i++)
		{
			m_pData[i] = src[i];
		}
	}
	void copyTo(T* dst, int len)
	{
		int copyLen = len<m_iLength ? len : m_iLength;
		for (int i = 0; i < copyLen; i++)
		{
			dst[i] = m_pData[i];
		}
	}
private:
	//禁止拷贝
	DArray& operator=(const DArray& da);
};




typedef DArray<int> DIntArray;
typedef DArray<short> DShortArray;
typedef DArray<char> DCharArray;


#endif //_D_ARRAY_H_

