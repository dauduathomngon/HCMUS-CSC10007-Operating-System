// ---------------------------------------------
// Thanh vien nhom:
// 21120518 - Dang An Nguyen
// 21120312 - Phan Nguyen Phuong
// 21120498 - Do Hoang Long
// 21120355 - Nguyen Anh Tu
// 21120511 - Le Nguyen
// ---------------------------------------------

#include "syscall.h"

int main()
{
	int result;

	PrintString("\nTao semaphore name = lenguyen, semval = 1");
	result = CreateSemaphore("lenguyen", 1);

	if (result == 0)
	{
		PrintString("\nDa tao thanh cong semaphore");
	}

	result = Exec("./test/help");
	if (result != 0)
	{
		PrintString("\nChay chuong trinh thanh cong");
	}

	PrintChar('\n');
	Halt();
}
