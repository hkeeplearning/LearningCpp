# 生产者消费者模式

生产者消费者模式：有一个生产者生产产品，这些产品提供给若干个消费者去消费，为了使生产者和消费者能够并发执行，在两者之间设置一个具有多个缓冲区的缓冲池，生产者将它生产的产品放到缓冲区中，消费者可以从缓冲区中取走产品进行消费，显然生产者和消费者之间需要保持同步。

多线程之间的同步可以使用`mutex`，`semaphore`和`critical_section`。多进程之间同步可以使用`mutex`和`semaphore`。

## 多线程（进程）下`mutex`，`semaphore`的区别

| `semaphore` | 为了使一个信号从一个任务传到另一个任务。                     |
| ----------- | ------------------------------------------------------------ |
| `mutex`     | 使受保护的共享资源在每次任务中都是按照取得与释放的顺序进行。 |

`semaphore`:

```
/* Task 1 - Producer */
semPost(sem_power_btn); // Send the signal

/* Task 2 - Consumer */
semPend(sem_power_btn); // Wait for signal
```

Is the number of free identical toilet keys. Example, say we have four toilets with identical locks and keys. The semaphore count - the count of keys - is set to 4 at beginning (all four toilets are free), then the count value is decremented as people are coming in. If all toilets are full, ie. there are no free keys left, the semaphore count is 0. Now, when eq. one person leaves the toilet, semaphore is increased to 1 (one free key), and given to the next person in the queue.

信号量是一个自由的官方厕所钥匙，我们有四个厕所，他们的锁和钥匙是一样的。
信号量开始设置为4，表示4个厕所是自由滴，如果一个人进去了，数量就-1.
如果厕所满了，钥匙数目就为0，信号量数目这时也是0.如果一个人离开厕所，信号量+1，队列中的下一个人可以用啦！



`semaphore`是允许多个线程进入，访问互斥资源。除了多元信号量之外，还存在一种二元信号量。即只存在是与否，0与1两种状态。



`mutex`:

```
/* Task 1 */
mutexWait(mutex_mens_room);
// Safely use shared resource
mutexRelease(mutex_mens_room);

/* Task 2 */
mutexWait(mutex_mens_room);
// Safely use shared resource
mutexRelease(mutex_mens_room);
```

Is a key to a toilet. One person can have the key - occupy the toilet - at the time.
When finished, the person gives (frees) the key to the next person in the queue.

`mutex`是厕所钥匙，一次只能一人那着这把钥匙去厕所。结束了，这个人把钥匙给队列中的下一个人。



它和二元信号量不同的是，占有和释放必须是同一个线程。比如互斥量M被线程A占有，那么释放的时候肯定也是A线程释放的。二元信号量则不必如此，一个二元信号量的占有和释放可以是不同线程。mutex是可以用于进程也可以用于线程的同步机制。



`critical_section`:

耗费资源少，用于多线程之间的同步问题。

## 模式

### 多线程

1. 一个生产者一个消费者一个缓冲区

```C++
#include <windows.h>
#include <thread>
#include <iostream>

using std::cout;
using std::endl;

DWORD WINAPI ProducerThread(LPVOID);
DWORD WINAPI ConsumerThread(LPVOID);

const int kProductNum = 10;
int g_buffer = 0;
CRITICAL_SECTION g_criticalSection;
HANDLE g_hEventBufEmpty;
HANDLE g_hEventBufFull;

int main()
{
	::InitializeCriticalSection(&g_criticalSection);
	g_hEventBufEmpty = ::CreateEvent(NULL, FALSE, TRUE, NULL);
	g_hEventBufFull = ::CreateEvent(NULL, FALSE, FALSE, NULL);

	const int kThreadNum = 2;
	HANDLE threadHandle[kThreadNum];
	threadHandle[0] = CreateThread(NULL, 0, ProducerThread, NULL, 0, NULL);
	threadHandle[1] = CreateThread(NULL, 0, ConsumerThread, NULL, 0, NULL);
	::WaitForMultipleObjects(kThreadNum, threadHandle, TRUE, INFINITE);

	::DeleteCriticalSection(&g_criticalSection);
	::CloseHandle(threadHandle[0]);
	::CloseHandle(threadHandle[1]);
	::CloseHandle(g_hEventBufFull);
	::CloseHandle(g_hEventBufEmpty);

	return 0;
}

DWORD WINAPI ProducerThread(LPVOID p)
{
	for (int i = 0; i <= kProductNum; ++i)
	{
		::WaitForSingleObject(g_hEventBufEmpty, INFINITE);
		EnterCriticalSection(&g_criticalSection);
		g_buffer = i;
		cout << "生产者将数据 " << g_buffer << " 放入缓冲区！" << endl;
		LeaveCriticalSection(&g_criticalSection);
		::SetEvent(g_hEventBufFull);
	}
	return 0;
}
DWORD WINAPI ConsumerThread(LPVOID p)
{
	for (int i = 0; i <= kProductNum; ++i)
	{
		::WaitForSingleObject(g_hEventBufFull, INFINITE);
		EnterCriticalSection(&g_criticalSection);
		cout << "\t\t\t\t消费者将数据 " << g_buffer << " 从缓冲区取出！" << endl;
		LeaveCriticalSection(&g_criticalSection);
		::SetEvent(g_hEventBufEmpty);
	}
	return 0;
}
```

2. 一个生产者多个消费者多个缓冲区

```C++
#include <windows.h>
#include <thread>
#include <iostream>

using std::cout;
using std::endl;

DWORD WINAPI ProducerThread(LPVOID);
DWORD WINAPI ConsumerThread(LPVOID);

const int kProductNum = 10;
const int kBufferSize = 4;
int g_buffer[kBufferSize] = { 0 };
CRITICAL_SECTION g_criticalSection;
HANDLE g_hEventBufEmpty;
HANDLE g_hEventBufFull;
int g_i = 0, g_j = 0;

int main()
{
	::InitializeCriticalSection(&g_criticalSection);
	g_hEventBufEmpty = ::CreateSemaphore(NULL, 4, 4, NULL);
	g_hEventBufFull = ::CreateSemaphore(NULL, 0, 4, NULL);

	const int kThreadNum = 3;
	HANDLE threadHandle[kThreadNum];
	threadHandle[0] = CreateThread(NULL, 0, ProducerThread, NULL, 0, NULL);
	threadHandle[1] = CreateThread(NULL, 0, ConsumerThread, NULL, 0, NULL);
	threadHandle[2] = CreateThread(NULL, 0, ConsumerThread, NULL, 0, NULL);
	::WaitForMultipleObjects(kThreadNum, threadHandle, TRUE, INFINITE);

	::DeleteCriticalSection(&g_criticalSection);
	for (int i = 0; i <= kThreadNum; ++i)
	{
		::CloseHandle(threadHandle[i]);
	}
	::CloseHandle(g_hEventBufFull);
	::CloseHandle(g_hEventBufEmpty);

	return 0;
}

DWORD WINAPI ProducerThread(LPVOID p)
{
	for (int i = 0; i <= kProductNum; ++i)
	{
		::WaitForSingleObject(g_hEventBufEmpty, INFINITE);
		EnterCriticalSection(&g_criticalSection);
		
		g_buffer[g_i] = i;
		cout << "生产者将数据 " << "(" << g_i << " " << g_buffer[g_i] << ") 放入缓冲区！" << endl;
		g_i = (g_i + 1) % kBufferSize;
		
		::LeaveCriticalSection(&g_criticalSection);
		::ReleaseSemaphore(g_hEventBufFull, 1, NULL);
	}
	return 0;
}
DWORD WINAPI ConsumerThread(LPVOID p)
{
	for (int i = 0; i <= kProductNum; ++i)
	{
		::WaitForSingleObject(g_hEventBufFull, INFINITE);
		EnterCriticalSection(&g_criticalSection);

		cout << "\t\t\t\t消费者 " << ::GetCurrentThreadId() << ": (" << g_j << " " << g_buffer[g_j] << ") 从缓冲区取出！" << endl;
		if (g_buffer[g_j] == kProductNum)
		{
			::LeaveCriticalSection(&g_criticalSection);
			::ReleaseSemaphore(g_hEventBufFull, 1, NULL);
			break;
		}
		g_j = (g_j + 1) % kBufferSize;
		
		LeaveCriticalSection(&g_criticalSection);
		::ReleaseSemaphore(g_hEventBufEmpty, 1, NULL);
	}
	cout << "\t\t\t\t消费者 " << ::GetCurrentThreadId() << " 退出！" << endl;
	return 0;
}
```

### 多进程

- 一个常驻进程，申请共享缓冲区（共享内存），初始化信号量和互斥量
- 生产者进程，读取本地数据，写入共享缓冲区
- 消费者进程，读取共享缓冲区数据，写到本地

[实现](https://github.com/hkeeplearning/LearningCpp/tree/master/ProducerConsumer)