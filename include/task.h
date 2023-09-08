#include <pthread.h>

/***********************************************
Types
***********************************************/
enum class TaskId {
    VIDEO_CAPTURE,
    VIDEO_PROC,
#ifdef DIAGNOSTICS_ENABLED
    DIAGNOSTICS,
#endif
    COUNT
};
enum class TaskPriority {
    VIDEO_CAPTURE = 1,
    VIDEO_PROC,
#ifdef DIAGNOSTICS_ENABLED
    DIAGNOSTICS,
#endif
};
typedef void* (*TaskFunction)(void*);

/***********************************************
Classes
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
Variables
***********************************************/
extern Task tasks[static_cast<int>(TaskId::COUNT)];

/***********************************************
Functions
***********************************************/
void task_init();
void task_shutdown();
