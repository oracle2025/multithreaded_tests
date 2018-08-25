#pragma once

#include <functional>
#include <thread>
#include <future>
#include <vector>
#include <numeric>
#include "countdownlatch.hpp"

using namespace std;
template <typename T>
class RunsInThreads
{
	public:
		using function_type = std::function<bool(T&)>;
		RunsInThreads(T &_t, int _number_of_threads) :
			number_of_threads(_number_of_threads),
			t(_t)
		{
		}

		bool matches(function_type f)
		{
    		clatch::countdownlatch latch(1);
    		atomic_bool running {false};
    		atomic_int overlaps {0};
    		vector<future<bool>> futures(number_of_threads);
    		for (int i = 0; i < number_of_threads; i++) {
    			packaged_task<bool()> task([&](){
    					latch.await();
                		if (running.load()) {
                    		overlaps++;
                		}
                		running.store(true);
    					auto result = f(t);
    					running.store(false);
    					return result;
    					});
        		futures[i] = task.get_future();
        		std::thread(std::move(task)).detach();
    		}
    		latch.count_down();
			return accumulate(futures.begin(), futures.end(), true,
					[](bool res, future<bool> &b) {
						return res && b.get();
					})
				&& (overlaps.load() > 0);
            /*for (auto &i: futures) {
    			if (!i.get()) {
    				return false;
    			}
            }*/
			return true;
		}
	private:
		int number_of_threads;
		T &t;

};
