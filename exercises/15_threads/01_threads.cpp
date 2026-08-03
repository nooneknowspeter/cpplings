// TODO: We sometimes encourage you to keep trying things on a given exercise
// even after you already figured it out.
//
// Threads enable concurrent execution of code.
//
// std::thread (C++11):
// - Creates a new thread of execution
// - Pass function to execute in the thread
// - join() waits for thread to complete
// - detach() allows thread to run independently
//
// Mutex (mutual exclusion):
// - std::mutex: basic mutex
// - lock() acquires, unlock() releases
// - std::lock_guard: RAII mutex wrapper (automatic unlock)
//
// https://www.learncpp.com/cpp-tutorial/multithreading/

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <mutex>
#include <thread>
#include <vector>

std::mutex mtx;
int counter = 0;

// TODO: Implement a function that increments the counter safely.
// Use a mutex to protect the shared resource.
void incrementSafely()
{
    std::_ lock(mtx);
    counter++;
}

// TODO: Implement a function that returns the current counter value.
int getCounter()
{
    std::lock_guard<std::mutex> lock(mtx);
    return counter;
}

int main(int argc, char *argv[])
{
    return Catch::Session().run(argc, argv);
}

TEST_CASE("Threads Threads01")
{
    counter = 0;

    std::vector<std::thread> threads;
    for (int i = 0; i < 10; i++)
    {
        threads.push_back(std::thread(incrementSafely));
    }

    for (auto &t : threads)
    {
        t.join();
    }

    REQUIRE(getCounter() == 10);
}
