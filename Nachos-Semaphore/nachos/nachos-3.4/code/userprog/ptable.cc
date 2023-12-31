// ---------------------------------------------
// Thanh vien nhom:
// 21120518 - Dang An Nguyen
// 21120312 - Phan Nguyen Phuong
// 21120498 - Do Hoang Long
// 21120355 - Nguyen Anh Tu
// 21120511 - Le Nguyen
// ---------------------------------------------

#include "ptable.h"
#include "system.h"
#include "openfile.h"

PTable::PTable(int size)
{
	if (size < 0 || size > 10)
		return;

	psize = size;
	bm = new BitMap(size);
	bmsem = new Semaphore("bmsem", 1);

	for (int i = 0; i < MAX_PROCESS; i++)
	{
		pcb[i] = 0;
	}

	bm->Mark(0);

	pcb[0] = new PCB(0);
	pcb[0]->SetFileName("");
	pcb[0]->parentID = -1;
}

PTable::~PTable()
{
	if (bm != 0)
		delete bm;

	for (int i = 0; i < psize; i++)
	{
		if (pcb[i] != 0)
			delete pcb[i];
	}

	if (bmsem != 0)
		delete bmsem;
}

int PTable::ExecUpdate(char *name)
{

	bmsem->P();

	if (name == NULL)
	{
		printf("\nERROR: Ten khong hop le");
		bmsem->V();
		return -1;
	}

	if (strcmp(name, "") == 0 || strcmp(name, currentThread->getName()) == 0)
	{
		printf("\nERROR: Tu goi lai chinh thread");
		bmsem->V();
		return -1;
	}

	int index = this->GetFreeSlot();

	if (index < 0)
	{
		printf("\nERROR: Khong con slot trong");
		bmsem->V();
		return -1;
	}

	pcb[index] = new PCB(index);
	pcb[index]->SetFileName(name);
	pcb[index]->parentID = currentThread->processID;
	int pid = pcb[index]->Exec(name, index);
	bmsem->V();
	return pid;
}

int PTable::JoinUpdate(int pID)
{
	if (pID < 0 || pID > 9)
	{
		printf("\nERROR: Khong ton tai process co id: %d\n", pID);
		return -1;
	}

	if (pcb[pID] == NULL)
	{
		printf("\nERROR: Khong ton tai process co id: %d\n", pID);
		return -1;
	}

	// kiem tra tien trinh dang chay co la cha cua tien trinh can join hay khong
	if (currentThread->processID != pcb[pID]->parentID)
	{
		printf("\nERROR: Ko duoc phep join vao tien trinh khong phai cha cua no !!!\n");
		return -1;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////

	pcb[pID]->JoinWait(); // doi den khi tien trinh con ket thuc

	int ec = pcb[pID]->GetExitCode();

	if (ec != 0)
	{
		printf("\nERROR: Tien trinh thoat voi exitcode: %d ", ec);
		return ec;
	}

	pcb[pID]->ExitRelease(); // cho phep tien trinh con ket thuc

	return 0;
}
int PTable::ExitUpdate(int ec)
{
	// Kiem tra pID co ton tai khong
	int pID = currentThread->processID;
	if (!IsExist(pID))
	{
		printf("\nERROR: Tien trinh khong ton tai");
		return -1;
	}

	// Neu la main process thi Halt()
	if (pID == 0)
	{
		interrupt->Halt();
		return 0;
	}

	// Goi SetExitCode de dat exitcode cho tien trinh goi
	pcb[pID]->SetExitCode(ec);

	// Exit join
	pcb[pID]->JoinRelease();
	pcb[pID]->ExitWait();
	Remove(pID);

	return ec;
}

int PTable::GetFreeSlot()
{
	return bm->Find();
}

bool PTable::IsExist(int pid)
{
	return bm->Test(pid);
}

void PTable::Remove(int pid)
{
	bm->Clear(pid);
	if (pcb[pid] != 0)
		delete pcb[pid];
}

char *PTable::GetFileName(int id)
{
	return (pcb[id]->GetFileName());
}
