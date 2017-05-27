#include "Common.h"
#include "DRunnable.h"
#include "cocos2d.h"





DThreadRun::DThreadRun()
{
	mList = new DList<DRunnable>(false);
//	pthread_mutex_init(&mutex, NULL);
}

DThreadRun::~DThreadRun()
{
	SAFE_DELETE(mList);
//	pthread_mutex_destroy(&mutex);
}

void DThreadRun::addRunnable(DRunnable* run)
{
//	pthread_mutex_lock(&mutex);

	mList->addElement(run);

//	pthread_mutex_unlock(&mutex);
}

void DThreadRun::runList()
{
//	pthread_mutex_lock(&mutex);

	while (mList->size() > 0)
	{
		DRunnable* run = mList->removeElementAt(0);
		run->run();
		run->cleanup();
	}

//	pthread_mutex_unlock(&mutex);
}




