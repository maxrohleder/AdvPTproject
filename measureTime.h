#include <chrono>
#include <functional>

using namespace std;
using namespace std::chrono;

int measureTime(function<void()> f) {
	auto start = steady_clock::now();
	f();
	auto end = steady_clock::now();
	return duration_cast<seconds>(end - start).count();
}


