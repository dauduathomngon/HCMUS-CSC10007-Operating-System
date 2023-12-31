// ---------------------------------------------
// Thanh vien nhom:
// 21120518 - Dang An Nguyen
// 21120312 - Phan Nguyen Phuong
// 21120498 - Do Hoang Long
// 21120355 - Nguyen Anh Tu
// 21120511 - Le Nguyen
// ---------------------------------------------

#ifndef STABLE_H
#define STABLE_H
#include "synch.h"
#include "bitmap.h"

#define MAX_SEMAPHORE 10

#endif // STABLE_H

class SemList
{
public:
	SemList(char *na, int i)
	{
		strcpy(this->name, na);
		sem = new Semaphore(this->name, i);
	}

	~SemList()
	{
		if (sem)
			delete sem;
	}

	void wait()
	{
		sem->P();
	}

	void signal()
	{
		sem->V();
	}

	char *GetName()
	{
		return this->name;
	}

private:
	char name[100];
	Semaphore *sem;
};

class STable
{
public:
	STable();

	~STable();

	int Create(char *name, int init);

	int Wait(char *name);

	int Signal(char *name);

	int FindFreeSlot();

private:
	BitMap *bm;
	SemList *semList[MAX_SEMAPHORE];
};
