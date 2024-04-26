#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

std::mutex mtx;
std::condition_variable cv;
bool ready = false;

void worker_thread()
{
    std::unique_lock<std::mutex> lck(mtx);
    cv.wait(lck, []
            { return ready; });
    std::cout << "Hilo de trabajo: Data procesada." << std::endl;
}

void main_thread()
{
    std::thread worker(worker_thread);

    {
        std::cout << "Hilo principal: Procesando la data." << std::endl;
        for (int i = 0; i < 10; i++)
        {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::lock_guard<std::mutex> lck(mtx);
            std::cout << "Data " << i << std::endl;
        }
        ready = true;
    }

    cv.notify_one();
    worker.join();
}

int main()
{
    main_thread();
    return 0;
}