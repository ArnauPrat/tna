

#include <gtest/gtest.h>
#include "../engine/types.h"
#include "../engine/data/queue.h"

namespace tna
{

using uint32_queue = queue_t<uint32_t>;
  
/**
 * Creates one asynchronous task per thread, where each task stores its therad id in the
 * result array
 * */
TEST(QueueTest, SimpleTest) {

  constexpr uint32_t QUEUE_INITIAL_CAPACITY = 1024;

  uint32_queue queue; 
  queue_init(&queue, QUEUE_INITIAL_CAPACITY);

  for (uint32_t i = 0; i < QUEUE_INITIAL_CAPACITY; ++i) 
  {
    queue_push(&queue, i);
  }

  for (uint32_t i = 0; i < QUEUE_INITIAL_CAPACITY/2; ++i) 
  {
    uint32_t next;
    bool res = queue_pop(&queue, &next);
    ASSERT_TRUE(res);
    ASSERT_EQ(next, i);
  }

  for (uint32_t i = QUEUE_INITIAL_CAPACITY; i < QUEUE_INITIAL_CAPACITY*2; ++i) 
  {
    queue_push(&queue, i);
  }

  for (uint32_t i = QUEUE_INITIAL_CAPACITY/2; i < QUEUE_INITIAL_CAPACITY*2; ++i) 
  {
    uint32_t next;
    bool res = queue_pop(&queue, &next);
    ASSERT_TRUE(res);
    ASSERT_EQ(next, i);
  }

  uint32_t next;
  bool res = queue_pop(&queue, &next);
  ASSERT_FALSE(res);

  queue_release(&queue);
}


} /* tna */ 

int main(int argc, char* argv[]){
  ::testing::InitGoogleTest(&argc,argv);
  int ret = RUN_ALL_TESTS();
  return ret;
}

