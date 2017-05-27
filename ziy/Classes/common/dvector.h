#ifndef _DVECTOR_H_
#define _DVECTOR_H_

#include "stdlib.h"


/**可变对象数组*/
template<typename T>
class DVector
{
private:
	int m_iAlloc;
	int m_iLength;
	T* m_pData;
	void alloc(int iSize)
	{
		m_pData = new T[iSize];
		m_iAlloc = iSize;
	}
	void realloc(int iNewSize)
	{
		if (iNewSize <= m_iAlloc) return;
		if (iNewSize < m_iAlloc * 2) iNewSize = m_iAlloc * 2 + 1;
		T* pOld = m_pData;
		int oldAlloc = m_iAlloc;
		alloc(iNewSize);
		for (int i = 0; i < m_iLength; i++)
		{
			m_pData[i] = pOld[i];
		}
		SAFE_DELETE_ARRAY(pOld);
	}
public:
	/*
	* 构造函数
	*/
	DVector()
	{
		alloc(10);
		m_iLength = 0;
	}
	/*
	* 析构函数
	*/
	~DVector()
	{
		SAFE_DELETE_ARRAY(m_pData);
		m_iAlloc = 0;
		m_iLength = 0;
	}
	/*
	* 尺寸
	*/
	int size() const
	{
		return m_iLength;
	}
	/*
	* 是否为空
	*/
	bool isEmpty() const
	{
		return	m_iLength == 0;
	}
	/*
	* 是否包含
	*/
	bool contains(const T& elem) const
	{
		return indexOf(elem) != -1;
	}
	/*
	* 查询
	*/
	int indexOf(const T& elem) const
	{
		return indexOf(elem, 0);
	}
	/*
	* 查询
	*/
	int indexOf(const T& elem, int index) const
	{
		for (int i = index; i < m_iLength; i++)
		{
			if (m_pData[i] == elem)
			{
				return i;
			}
		}
		return -1;
	}
	T& elementAt(int index) const
	{
		return m_pData[index];
	}
	T& operator [](int index) const
	{
		return m_pData[index];
	}
	void setElementAt(const T& obj, int index)
	{
		m_pData[index] = obj;
	}
	T removeElementAt(int index)
	{
		T ret = m_pData[index];
		for (int i = index; i < m_iLength - 1; i++)
		{
			m_pData[i] = m_pData[i + 1];
		}
		m_iLength--;
		return ret;
	}
	void insertElementAt(const T& obj, int index)
	{
		realloc(m_iLength + 1);
		if (index<m_iLength)
		{
			for (int i = m_iLength; i>index; i--)
			{
				m_pData[i] = m_pData[i - 1];
			}
		}
		m_pData[index] = obj;
		m_iLength++;
	}
	void addElement(const T& obj)
	{
		realloc(m_iLength + 1);
		m_pData[m_iLength] = obj;
		m_iLength++;
	}
	void removeElement(const T& obj)
	{
		int i = indexOf(obj);
		if (i != -1)
		{
			removeElementAt(i);
		}
	}
	void removeAllElements()
	{
		m_iLength = 0;
	}
	T popup()
	{
		T& result = m_pData[m_iLength - 1];
		if (m_iLength > 0)
		{
			m_iLength--;
		}
		return result;
	}
};


typedef DVector<int> DIntVector;
typedef DVector<short> DShortVector;
typedef DVector<char> DCharVector;




#endif

