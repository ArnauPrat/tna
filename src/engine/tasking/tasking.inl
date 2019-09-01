

namespace tna
{
  
template<typename Func, typename T>
void task_func(void * param ) {
}

template<typename Func, typename T>
void executeTaskAsync(uint32_t queueId, 
                     Func&& f, 
                     T* param,
                     SyncCounter* counter) noexcept {
  Task task;
  task.p_fp;
  task.p_args = (void*)(param);
}

} /*  tna */ 
