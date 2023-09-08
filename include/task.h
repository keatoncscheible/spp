#include <pthread.h>

/***********************************************
Types
***********************************************/
enum class TaskId { VIDEO_CAPTURE, VIDEO_PROC, DIAGNOSTICS, COUNT };
enum class TaskPriority { VIDEO_CAPTURE = 1, VIDEO_PROC, DIAGNOSTICS };
typedef void* (*TaskFunction)(void*);

/***********************************************
Classses
***********************************************/
class Task {
   public:
    Task(TaskId id, TaskPriority priority, TaskFunction function);

    void start();
    void join();

   private:
    TaskId id_;
    TaskPriority priority_;
    TaskFunction function_;
    pthread_t thread_;
    pthread_attr_t attr_;
    struct sched_param param_;
};

/***********************************************
Function Prototypes
***********************************************/
void task_init();
