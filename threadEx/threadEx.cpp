#include<thread>
#include <iostream>
#include <chrono>
#include <mutex>
#include <atomic>

std::atomic<int> atom(0);
std::mutex mutex;
int x = 0;

void foo() {
	//do stuff

	std::cout << "I am the first thread\n" << std::endl;
}

void Do_Stuff() {
	//do stuff

	mutex.lock();
	std::cout << "I am a Mutex thing\n" << std::endl;
	// unlock mutex
	mutex.unlock();
}

void barMutex(int x) {
	//do stuff

	std::lock_guard<std::mutex> lock(mutex); // use lock_guard to automatically lock and unlock the mutex
	std::cout << "I am the second thread, here is the value of x: " << x << std::endl;
}

void bar(int x) {
	//do stuff

	std::cout << "I am the second thread here the value of x :" << x << "\n" << std::endl;
}

void IamLate() {
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	std::cout << "Sorry I am late!\n" << std::endl;
}

void Mutexway() {
	std::thread first(foo);
	std::thread second(bar, x);
	std::thread third(IamLate);
	std::thread fourth(Do_Stuff);

	std::cout << "this is the Mutex way to solve race condition, foo and bar now execute concurrently...\n";

	// synchronize threads
	third.join();
	first.join();
	second.join();
	fourth.join();

	std::cout << "foo and bar completed.\n";
}

void Atomicway() {
	std::thread first(foo);
	std::thread second(bar, atom.load());
	std::thread third(IamLate);
	//std::thread fourth(Do_Stuff);
	std::cout << "this is the atomic way to solve race condition,, foo and bar now execute concurrently...\n";
	// synchronize threads
	third.join();
	first.join();
	second.join();
	//fourth.join();
	std::cout << "foo and bar completed.\n" << std::endl;
}

int main()
{
	bool restart = true;

	while (restart) {
		Mutexway();
		Atomicway();

		std::cout << "Do you want to run the program again? (y/n) ";
		char response;
		std::cin >> response;
		restart = (response == 'y');
	}

	return 0;
}