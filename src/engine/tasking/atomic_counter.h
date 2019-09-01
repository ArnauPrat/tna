

#ifndef _TNA_TASKING_ATOMIC_COUNTER_H_
#define _TNA_TASKING_ATOMIC_COUNTER_H_

#include "../types.h"

namespace std
{
template<typename T>
struct atomic;
}

namespace tna
{

struct atomic_counter_t 
{
  std::atomic<int32_t>* p_impl = nullptr;
};

/**
 * \brief Initializes the atomic counter
 *
 * \param counter
 */
void
atomic_counter_init(atomic_counter_t* counter);

/**
 * \brief Releases the atomic counter
 *
 * \param counter
 */
void
atomic_counter_release(atomic_counter_t* counter);

/**
 * @brief Sets the value of the counter
 *
 * @param value
 */
void 
atomic_counter_set(atomic_counter_t* counter, 
                   int32_t value);

/**
 * @brief Fetches and Increments the value of the counter
 *
 * @return 
 */
int32_t 
atomic_counter_fetch_increment(atomic_counter_t* counter);

/**
 * @brief Fetches and Decrements the value of the counter
 */
int32_t 
atomic_counter_fetch_decrement(atomic_counter_t* counter);

/**
 * @brief Blocks until the counter is set to zero
 */
void 
atomic_counter_join(atomic_counter_t* counter);

} /* tna */ 

#endif /* ifndef _SMILE_TASKING_SYNC_COUNTER_H_ */
