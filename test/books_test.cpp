#include "books.hpp"
#include "gtest/gtest.h"
#include "countdownlatch.hpp"
#include <vector>
#include <thread>
#include <future>
#include <sstream>
#include <set>
#include "runsinthreads.hpp"

using namespace std;

TEST(BooksTest, initial)
{
    Books books;
    auto title = "Media Center";
    auto id = books.add(title);
    EXPECT_EQ(books.title(id), title);
}

TEST(BooksTest, concurrent)
{
    Books books;
    int threads = 10;
    auto latch = new clatch::countdownlatch(1);

    atomic_bool running {false};
    atomic_int overlaps {0};
    vector<future<int>> futures(threads);
    for (int i = 0; i < threads; i++) {
        ostringstream str;
        str << "Book " << i;
        auto title = str.str();
        packaged_task<int()> task([=, &books, &running, &overlaps](){
                latch->await();
                if (running.load()) {
                    overlaps++;
                }
                running.store(true);
                auto id = books.add(title);
                running.store(false);
                return id;
                });
        futures[i] = task.get_future();
        std::thread(std::move(task)).detach();
    }
    latch->count_down();
    set<int> ids;
    for (auto &f : futures) {
        ids.insert(f.get());
    }
    EXPECT_EQ(ids.size(), threads);
    EXPECT_GT(overlaps.load(), 0);
}

TEST(BooksTest, concurrent_runsinthreads)
{
    atomic_int overlaps {0};
	RunsInThreads<atomic_int> rit(overlaps, 1000);
    Books books;
	EXPECT_TRUE(rit.matches([&books](atomic_int& t){
        	ostringstream str;
        	str << "Book " << t.fetch_add(1);
        	auto title = str.str();
    		auto id = books.add(title);
    		return books.title(id) == title;
			}));
}

