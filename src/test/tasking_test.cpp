

#include <gtest/gtest.h>
#include "../engine/tasking/tasking.h"
#include "../engine/tasking/atomic_counter.h"
#include <chrono>
#include <thread>
#include <stdlib.h>

namespace tna
{

constexpr uint32_t num_threads = 1;
  
/**
 * Creates one asynchronous task per thread, where each task stores its therad id in the
 * result array
 * */
TEST(TaskingTest, SimpleAsycTaskTest) {

  uint32_t* result = new uint32_t[num_threads];

  // Starts Thread Pool
  tasking_start_thread_pool(num_threads);

  // Create tasks
  atomic_counter_t syncCounter;
  atomic_counter_init(&syncCounter);
  for(uint32_t i = 0; i < num_threads; ++i) {
    task_t task;
    task.p_fp = [] (void * arg){
                int32_t* result = reinterpret_cast<int32_t*>(arg);
                *result=tasking_get_current_thread_id();
              }; 
    task.p_args = &result[i];

    tasking_execute_task_async(i, task, &syncCounter,"test", "test");
  }

  // Synchronize with the running tasks
  atomic_counter_join(&syncCounter);
  atomic_counter_release(&syncCounter);

  // Stops thread pool
  tasking_stop_thread_pool();

  // Checks the results
  for(uint32_t i = 0; i < num_threads; ++i) {
    ASSERT_TRUE(result[i] == i);
  }

  delete [] result;
}




/**
 * This test tests the use of lightweight threads tasks by soring an array
 * recursively using merge sort. First, two tasks are spawned and sent to the
 * tasking library. Each of these two tasks is in charge of soring half of the.
 * Each task recursively spawns a task to sort half of its input array part
 * until a length of 2 is achieved. When backtracking, each task merges the two
 * parts sorted by its two spawned sub-tasks and continues to backtrack, until
 * the main thread is reached, which performes the last merging to return the
 * finally sorted array
 */

struct Params{
  int32_t m_begin;
  int32_t m_end;
  int32_t* m_inputArray;
  int32_t* m_workArray;
};

void mergeArrays(int32_t* array, int32_t* workArray, int32_t begin, int32_t end) {
  int32_t splitPoint = (end - begin) / 2 + begin;
  int32_t i = begin;
  int32_t j = splitPoint;
  for (int k = begin; k < end; ++k) {
    if (i < splitPoint && (j >= end || array[i] <= array[j])) {
            workArray[k] = array[i];
            i = i + 1;
        } else {
            workArray[k] = array[j];
            j = j + 1;
        }
  }

  for (int k = begin; k < end; ++k) {
    array[k] = workArray[k];
  }
}

/**
 * @brief Recursive function that creates two tasks and each sorts one half of
 * the array. This function is executed in the lightweight threads of the
 * tasking library.
 *
 * @param inputArray The array to sort
 * @param length The length of the array to sort
 */
void mergeSort(void* args) {
  Params* params = reinterpret_cast<Params*>(args);
  if(params->m_end - params->m_begin > 2) {

    printf("THREAD ID: %d BEGIN: %d END: %d SORTS\n", tasking_get_current_thread_id(), params->m_begin, params->m_end);
    int32_t splitPoint = (params->m_end - params->m_begin) / 2 + params->m_begin;
    Params leftParams{params->m_begin, splitPoint, params->m_inputArray, params->m_workArray};
    Params rightParams{splitPoint, params->m_end, params->m_inputArray, params->m_workArray};
    atomic_counter_t counter;
    atomic_counter_init(&counter);
    task_t task_left;
    task_left.p_fp = mergeSort;
    task_left.p_args = &leftParams;
    tasking_execute_task_async(tasking_get_current_thread_id(), task_left, &counter,"","");

    task_t task_right;
    task_right.p_fp = mergeSort;
    task_right.p_args = &rightParams;
    tasking_execute_task_async(tasking_get_current_thread_id(), task_right, &counter, "", "");
    atomic_counter_join(&counter);
    atomic_counter_release(&counter);
    printf("THREAD ID: %d BEGIN: %d END: %d MERGES\n", tasking_get_current_thread_id(), params->m_begin, params->m_end);
    mergeArrays(params->m_inputArray, params->m_workArray, params->m_begin, params->m_end);
  } 
  else 
  {
    if(params->m_inputArray[params->m_begin] > params->m_inputArray[params->m_end-1]) 
    {
      params->m_workArray[params->m_begin] = params->m_inputArray[params->m_end-1];
      params->m_workArray[params->m_end-1] = params->m_inputArray[params->m_begin];
    } 
    else 
    {
      params->m_workArray[params->m_begin] = params->m_inputArray[params->m_begin];
      params->m_workArray[params->m_end-1] = params->m_inputArray[params->m_end-1];
    }
    params->m_inputArray[params->m_begin] = params->m_workArray[params->m_begin];
    params->m_inputArray[params->m_end-1] = params->m_workArray[params->m_end-1];
  }

};

/**
 * @brief Starts the sorting procedure by spawning two tasks, each sorts half of
 * the array, and then merges. This method is executed in the main thread. 
 *
 * @param inputArray The array to sort
 * @param length The length of the array to sort
 */
void mergeSortStart(int32_t* inputArray, int32_t length) {
    int32_t* workArray = new int32_t[length];
    Params leftParams{0, length/2, inputArray, workArray};
    Params rightParams{length/2, length, inputArray, workArray};
    atomic_counter_t counter;
    atomic_counter_init(&counter);

    task_t task_left;
    task_left.p_fp = mergeSort;
    task_left.p_args = &leftParams;
    tasking_execute_task_async(0, task_left, &counter, "", "");

    task_t task_right;
    task_right.p_fp = mergeSort;
    task_right.p_args = &rightParams;
    tasking_execute_task_async(1, task_right, &counter, "", "");

    atomic_counter_join(&counter);
    atomic_counter_release(&counter);
    mergeArrays(inputArray, workArray, 0, length);
    delete [] workArray;
}


/*TEST(TaskingTest, ParallelSortTest) {

  start_thread_pool(2);
  int32_t arrayLength = 4;
  int32_t* array = new int32_t[arrayLength];

  for (int i = 0; i < arrayLength; ++i) {
    array[i] = rand() % 10000;
  }

  mergeSortStart(array, arrayLength);

  for (int32_t i = 1; i < arrayLength; ++i) {
    ASSERT_TRUE(array[i] >= array[i-1]);
  }

  delete [] array;
  stop_thread_pool();

}*/

} /* tna */ 

int main(int argc, char* argv[]){
  ::testing::InitGoogleTest(&argc,argv);
  int ret = RUN_ALL_TESTS();
  return ret;
}

