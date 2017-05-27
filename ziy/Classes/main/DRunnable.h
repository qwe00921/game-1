#ifndef _D_RUNNABLE_H_
#define _D_RUNNABLE_H_


#include "DList.h"
//#include "pthread.h"


class DRunnable
{
public:
	DRunnable(bool autoFree)
	{
		isAutoFree = autoFree;
	}
	virtual ~DRunnable()
	{

	}
public:
	virtual void run() = 0;
	void cleanup()
	{
		if (isAutoFree)
		{
			delete this;
		}
	}
protected:
	bool isAutoFree;
};



/*
 * 线程同步
 */
class DThreadRun
{
public:
	DThreadRun();
	~DThreadRun();
public:
	void addRunnable(DRunnable* run);
	void runList();
private:
	DList<DRunnable>* mList;
	//pthread_mutex_t mutex;
};









#endif //#ifndef _D_RUNNABLE_H_