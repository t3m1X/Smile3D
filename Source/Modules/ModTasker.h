#ifndef SMILE3D_MODULES_MODTASKER_H
#define SMILE3D_MODULES_MODTASKER_H

#include "globals.hpp"
#include <functional>

namespace tasker {

/*enum class TaskType {
    kAsyncTask = 0,
    kUpdateTask,
    kFrameTask
}; mTODO:Future implementation */

char Init();
char CleanUp();

void AddTask(std::function<void()> task);

uint GetConcurrentThreads();
void WaitForTasks();

} //namespace tasker


#endif //SMILE3D_MODULES_MODTASKER_H
