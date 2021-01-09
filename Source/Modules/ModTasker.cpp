#include "ModTasker.h"
#include <thread>
#include <vector>
#include <atomic>
#include <functional>
#include <mutex>
#include <queue>

#include "Utilities/Logger.h"


namespace tasker {

uint concurrent_threads = 0;
std::atomic<bool> stop_pool = false;

std::mutex coordination_mtx;
std::mutex status_mtx;
std::condition_variable condition;

std::vector<std::thread> m_threads;
//True when thread is executing a task
std::vector <bool> m_threads_status;
std::queue<std::function<void()>> m_tasks_queue;


// "Private" functions
void ProcessTask(uint thread_id, std::atomic<bool>& stop) {
    while (true) {
        {
		std::lock_guard<std::mutex> lk(status_mtx);
		m_threads_status[thread_id] = false;
		}


        std::function<void()> m_task;   

        {
        std::unique_lock<std::mutex> lock(coordination_mtx);
        condition.wait(lock, [&] { return !m_tasks_queue.empty() || stop; });

        if (stop)
            break;

        {
        std::lock_guard<std::mutex> lk(status_mtx);
        m_threads_status[thread_id] = true;
        }

        m_task = m_tasks_queue.front();
        m_tasks_queue.pop();
        }

        m_task();

    }

}

// "Public" functions
char Init() {
    concurrent_threads = std::thread::hardware_concurrency();

    m_threads.reserve(concurrent_threads - 1);
    m_threads_status.reserve(concurrent_threads - 1);
    for (uint i = 0; i < concurrent_threads - 1; ++i){
        m_threads.push_back(std::thread(&ProcessTask, i, std::ref(stop_pool)));
        m_threads_status.push_back(false);
    }

    stop_pool = false;

    return true;
}

char CleanUp() {
    CONSOLE_LOG("Shutting down thread pool");
    {
    std::lock_guard<std::mutex> lk(coordination_mtx);
    stop_pool = true;
    }

    condition.notify_all(); //Wake up all threads
    for (std::thread& current_thread : m_threads)
		current_thread.join();

    m_threads.clear();
    m_threads_status.clear();

    return true;
}

void WaitForTasks() {
    bool is_processing;

    do {
        coordination_mtx.lock();
        if (m_tasks_queue.empty()) {
            coordination_mtx.unlock();
            do {
                is_processing = false;
                std::lock_guard<std::mutex> lk(status_mtx);
                for (int i = 0; i < m_threads_status.size() && !is_processing; ++i)
                    is_processing = m_threads_status[i];

                //mTODO: Maybe wait here to not bang our heads while the threads process
            } while (is_processing);
        }
        else {
            is_processing = true;
            std::function<void()> m_task;
			m_task = m_tasks_queue.front();
			m_tasks_queue.pop();
			coordination_mtx.unlock();
			m_task();
        }
    } while (is_processing);
}

void AddTask(std::function<void()> task) {
    {
    std::lock_guard<std::mutex> lk(coordination_mtx);
    m_tasks_queue.push(task);
    }

    condition.notify_one();
}

} //namespace tasker