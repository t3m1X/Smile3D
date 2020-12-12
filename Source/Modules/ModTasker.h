#ifndef SMILE3D_MODULES_MODTASKER_H
#define SMILE3D_MODULES_MODTASKER_H

#include "../globals.hpp"
#include <functional>

namespace tasker {

enum class TaskType {
    kAsyncTask = 0,
    kUpdateTask,
    kFrameTask
};

char Init();
char CleanUp();

template<typename F, typename... Args>
void AddTask(TaskType t, std::function<void()> task);

uint GetConcurrentThreads();
void WaitForTasks();

} //namespace tasker


#endif //SMILE3D_MODULES_MODTASKER_H
