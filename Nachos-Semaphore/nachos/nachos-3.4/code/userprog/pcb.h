// ---------------------------------------------
// Thanh vien nhom:
// 21120518 - Dang An Nguyen
// 21120312 - Phan Nguyen Phuong
// 21120498 - Do Hoang Long
// 21120355 - Nguyen Anh Tu
// 21120511 - Le Nguyen
// ---------------------------------------------

#ifndef PCB_H
#define PCB_H

#include "thread.h"
#include "synch.h"

// Process Control Block
class PCB
{
public:
	int parentID; // ID của tiến trình cha

	PCB();
	PCB(int id); // contructor

	~PCB(); // deconstructor

	// nạp chương trình có tên lưu trong biến filename và processID là pid
	int Exec(char *filename, int pid); // Tạo 1 thread mới có tên là filename và process là pid

	int GetID(); // Trả về ProcessID của tiến trình gọi thực hiện

	int GetNumWait(); // Trả về số lượng tiến trình chờ

	void JoinWait(); // 1. Tiến trình cha đợi tiến trình con kết thúc

	void ExitWait(); // 4. Tiến trình con kết thúc

	void JoinRelease(); // 2. Báo cho tiến trình cha thực thi tiếp

	void ExitRelease(); // 3. Cho phép tiến trình con kết thúc

	void IncNumWait(); // Tăng số tiến trình chờ

	void DecNumWait(); // Giảm số tiến trình chờ

	void SetExitCode(int ec); // Đặt exitcode của tiến trình

	int GetExitCode(); // Trả về exitcode

	void SetFileName(char *fn); // Đặt tên của tiến trình

	char *GetFileName(); // Trả về tên của tiến trình

private:
	Semaphore *joinsem; // semaphore cho quá trình join
	Semaphore *exitsem; // semaphore cho quá trình exit
	Semaphore *multex;	// semaphore cho quá trình truy xuất đọc quyền
	int exitcode;
	int numwait; // số tiến trình đã join

	char FileName[50];
	Thread *m_thread;
	bool boolBG;
};

#endif // PCB_H
