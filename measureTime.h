#include <chrono>
#include <functional>
#include <iostream>

using namespace std;
using namespace std::chrono;

template <typename obj_type, typename func, typename arg_type, typename ret_type> int measureTime(const obj_type& obj,const func& f,const arg_type arg = NULL) {
	auto start = steady_clock::now();
	ret_type ret;
	if (arg != NULL){
		ret = (obj.f)(arg);
	}else{
		ret = (obj.f)();
	}
	if (ret != NULL){
		cout << ret << endl;
	}
	auto end = steady_clock::now();
	return duration_cast<seconds>(end - start).count();
}


