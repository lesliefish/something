/** @file   packaged_task_test.cpp
 *  @brief  多线程处理耗时任务Test
 *  @note
 *  @author lesliefish
 *  @date   2019/02/28
 */

#include <iostream>
#include <vector>
#include <map>
#include <future>
#include <iostream>
#include <thread>
#include <deque>

using namespace std;

class Task
{
public:
    Task(int n)
        :m_id(n)
        , m_result(0)
    {}

    int getId() { return m_id; }
    int getResult() { return m_result; }
    // 任务执行
    void exec()
    {
        // 假设每个任务需要100毫秒
        this_thread::sleep_for(std::chrono::milliseconds(100));
        cout << m_id << " ";
        // 测试假设将n*n作为每个任务的执行结果
        m_result = m_id * m_id;
    }

private:
    int m_id;   // task id
    int m_result; // 任务执行结果
};


class TaskManager
{
public:
    TaskManager(const vector<Task>& tasks)
        : m_tasks(tasks) {}

    // 重载()
    std::map<int, int> operator()()
    {
        std::map<int, int> results{};
        for (auto& iter : m_tasks)
        {
            iter.exec();
            results[iter.getId()] = iter.getResult();
        }

        return results;
    };

private:
    vector<Task> m_tasks{};
};


int main(int argc, char* argv[])
{
    // 初步猜测推荐线程个数是4个
    static const unsigned int threadNumberGuess = 4;
    // 假设有iAllTaskNumbers个任务 
    static const unsigned int iAllTaskNumbers = 50;
    // 总任务列表
    std::vector<Task> taskList{};
    for (int i = 0; i < iAllTaskNumbers; ++i)
    {
        taskList.emplace_back(Task(i));
    }

    {
        // 单线程执行测试
        auto start = std::chrono::system_clock::now();
        cout << "single thread start" << endl;

        TaskManager taskManager(taskList);  // 共iAllTaskNumbers个任务
        auto result = taskManager();        // 单线程执行任务

        cout << endl;
        auto end = std::chrono::system_clock::now();
        cout << "single thread finish" << endl;
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        cout << "totaltime used : " << double(duration.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den << " seconds." << endl;
    }

    cout << endl << endl;

    {
        // 多线程执行测试

        // 推荐线程个数hw
        unsigned int hw = std::thread::hardware_concurrency();
        // 实际开辟线程个数
        unsigned int realThreadNumbers = (hw != 0) ? hw : 4;
        cout << "thread numbers : " << realThreadNumbers << endl;

        // 任务管理对象作为函数对象
        std::vector<TaskManager> taskManagers{};
        for (unsigned int i = 0; i < realThreadNumbers; ++i)
        {
            // 根据线程个数将任务均分
            int begin = (i * taskList.size()) / realThreadNumbers;
            int end = (i + 1)*taskList.size() / realThreadNumbers;

            std::vector<Task> partTaskList{};
            for (auto j = begin; j < end; j++)
            {
                partTaskList.emplace_back(taskList[j]);
            }
            taskManagers.emplace_back(partTaskList);
        }

        // 任务打包
        std::deque<std::packaged_task<std::map<int, int>()>> packagedTasks;
        for (unsigned int i = 0; i < realThreadNumbers; ++i)
        {
            std::packaged_task<map<int, int>()> task(taskManagers[i]);
            packagedTasks.emplace_back(move(task));
        }

        // 获取任务执行的返回值列表
        vector< std::future<std::map<int, int>>> runFutures{};
        for (unsigned int i = 0; i < realThreadNumbers; ++i)
        {
            runFutures.push_back(packagedTasks[i].get_future());
        }

        {
            auto start = std::chrono::system_clock::now();
            cout << "mutil thread start" << endl;

            // 每个任务在独立的线程中执行
            while (!packagedTasks.empty())
            {
                std::packaged_task<std::map<int, int>()> myTask = std::move(packagedTasks.front());
                packagedTasks.pop_front();
                std::thread t(std::move(myTask));
                t.detach();
            }

            // 获取执行结果,等待任务执行结束
            deque<map<int, int>> res{};
            for (unsigned int i = 0; i < realThreadNumbers; ++i)
            {
                res.emplace_back(runFutures[i].get());
            }

            cout << endl;
            auto end = std::chrono::system_clock::now();
            cout << "mutil thread finish" << endl;
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            cout << "totaltime used : " << double(duration.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den << " seconds." << endl;
        }
    }

    return 0;
}
