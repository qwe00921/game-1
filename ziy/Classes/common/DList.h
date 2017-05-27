#ifndef _DLIST_H_
#define _DLIST_H_


#include "stdlib.h"

/**
 *可变指针数组，使用模版方式管理
 */
template<typename T>
class DList
{
private:
	int m_iAlloc;
	bool m_autofree;
	int m_iLength;
	T** m_pData;
	void alloc(int iSize);
	void realloc(int iNewSize);
public:
	/*
	* 构造函数
	* @param autofree true，自动释放资源；false，手动释放
	*/
	DList(bool autofree);
	/*
	* 析构函数
	*/
	~DList();
	/*
	* 尺寸
	*/
	int size() const { return m_iLength; };
	/*
	* 是否为空
	*/
	bool isEmpty() const;
	/*
	* 是否包含
	*/
	bool contains(T* elem) const;
	/*
	* 查询
	*/
	int indexOf(T* elem) const;
	/*
	* 查询
	*/
	int indexOf(T* elem, int index) const;
	T* elementAt(int index) const
	{
		return m_pData[index];
	};
	T* operator [](int index) const
	{
		return m_pData[index];
	}
	void setElementAt(T* obj, int index);
	T* removeElementAt(int index);
	void insertElementAt(T* obj, int index);
	void addElement(T* obj);
	T* removeElement(T* obj);
	void removeAllElements();
};


template<typename T>
void DList<T>::alloc(int iSize)
{
	m_pData = new T*[iSize];
	m_iAlloc = iSize;
}

template<typename T>
void DList<T>::realloc(int iNewSize)
{
	if (iNewSize <= m_iAlloc) return;
	if (iNewSize < m_iAlloc * 2) iNewSize = m_iAlloc * 2 + 1;
	T** pOld = m_pData;
	int oldAlloc = m_iAlloc;
	alloc(iNewSize);
	memcpy(m_pData, pOld, oldAlloc*sizeof(T*));
	SAFE_DELETE_ARRAY(pOld);
	memset(&m_pData[oldAlloc], 0, (m_iAlloc - oldAlloc)*sizeof(T*));
}

template<typename T>
DList<T>::DList(bool autofree)
{
	this->m_autofree = autofree;
	alloc(10);
	memset(m_pData, 0, m_iAlloc*sizeof(T*));
	m_iLength = 0;
}

template<typename T>
DList<T>::~DList()
{
	if (m_autofree)
	{
		for (int i = 0; i < m_iLength; i++)
		{
			if (m_pData[i] != NULL)
			{
				SAFE_DELETE(m_pData[i]);
			}
		}

	}
	SAFE_DELETE_ARRAY(m_pData);
	m_iAlloc = 0;
	m_iLength = 0;
}

template<typename T>
bool DList<T>::isEmpty() const
{
	return	m_iLength == 0;
}

template<typename T>
bool DList<T>::contains(T* elem) const
{
	return indexOf(elem) != -1;
}

template<typename T>
int DList<T>::indexOf(T* elem) const
{
	return indexOf(elem, 0);
}

template<typename T>
int DList<T>::indexOf(T* elem, int index) const
{
	int i;
	if (elem == NULL)
	{
		for (i = index; i < m_iLength; i++)
		if (m_pData[i] == NULL) return i;
	}
	else
	{
		for (i = index; i < m_iLength; i++)
		if (m_pData[i] == elem) return i;
	}
	return -1;
}

template<typename T>
void DList<T>::setElementAt(T* obj, int index)
{
	m_pData[index] = obj;
}

template<typename T>
T* DList<T>::removeElementAt(int index)
{
	T* ret = m_pData[index];
	if (m_autofree)
	{
		if (m_pData[index] != NULL)
		{
			SAFE_DELETE(m_pData[index]);
			ret = NULL;
		}
	}
	for (int i = index; i < m_iLength - 1; i++)
	{
		m_pData[i] = m_pData[i + 1];
	}
	m_iLength--;
	m_pData[m_iLength] = 0;
	return ret;
}

template<typename T>
void DList<T>::insertElementAt(T* obj, int index)
{
	realloc(m_iLength + 1);
	if (index<m_iLength)
	{
		for (int i = m_iLength; i>index; i--)
		{
			m_pData[i] = m_pData[i - 1];
		}
		m_pData[index] = 0;
	}
	m_pData[index] = obj;
	m_iLength++;
}

template<typename T>
void DList<T>::addElement(T* obj)
{
	realloc(m_iLength + 1);
	m_pData[m_iLength] = obj;
	m_iLength++;
}

template<typename T>
T* DList<T>::removeElement(T* obj)
{
	int i = indexOf(obj);
	if (i == -1) return NULL;
	return removeElementAt(i);
}

template<typename T>
void DList<T>::removeAllElements()
{
	for (int i = m_iLength - 1; i >= 0; i--)
	{
		if (m_autofree)
		if (m_pData[i] != NULL)
		{
			SAFE_DELETE(m_pData[i]);
		}

	}
	m_iLength = 0;
}




#endif

