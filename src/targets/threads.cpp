#include <thread>
#include <chrono>
#include <iostream>

void foo(int i, int *a) {
	if (i == 0) {
		return;
	}
	*a = *a + 1;
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	foo(i - 1, a);
}

void bar(int i, int *b) {
	if (i == 0) {
		return;
	}
	*b = *b + 1;
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	bar(i - 1, b);
}

void display(int i, int *a, int *b) {
	if (i == 0) {
		return;
	}
	std::cout << *a << "," << *b << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	display(i - 1, a, b);
}

int main(int argc, char **argv) {
	int a = 0;
	int b = 0;

	std::thread t1(foo, 10, &a);
	std::thread t2(bar, 10, &b);
	std::thread t3(display, 20, &a, &b);

	t1.join();
	t2.join();
	t3.join();
	return 0;
}