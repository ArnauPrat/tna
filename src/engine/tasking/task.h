

#ifndef _TNA_TASKING_TASK_H_
#define _TNA_TASKING_TASK_H_ value


namespace tna
{
  
typedef void(*task_function_t)(void *arg);  

struct task_t {
  /**
   * @brief Pointer to the function that executed the task
   */
  task_function_t  p_fp = nullptr;

  /**
   * @brief Pointer to the argument data that will be passed to the task
   * function
   */
  void*         p_args = nullptr;
};

} /* tna */ 

#endif /* ifndef _TASKING_TASK_H_ */
