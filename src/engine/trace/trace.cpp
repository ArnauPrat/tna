

#include "trace.h"
#include "../tasking/mutex.h" 

#include <string.h>
#include <time.h>

namespace  tna
{

static uint32_t m_num_queues = 0;

/**
 * \brief Trace event arrays for each of the threads
 */
static trace_event_t**  p_trace_event_arrays = nullptr;

/**
 * \brief The trace event arrays count
 */
static uint32_t*        m_trace_event_count = nullptr;

/**
 * \brief The trace event arrays capacity
 */
static uint32_t*        m_trace_event_capacity = nullptr;

/**
 * \brief Mutex for event arrays
 */
static mutex_t          m_trace_event_mutex;

static bool             m_trace_record_enabled = false;

void
trace_init(uint32_t num_queues)
{
  m_num_queues = num_queues;
  p_trace_event_arrays = new trace_event_t*[m_num_queues];
  m_trace_event_count  = new uint32_t[m_num_queues];
  m_trace_event_capacity = new uint32_t[m_num_queues];

  for(uint32_t i = 0; i < m_num_queues; ++i) 
  {
    m_trace_event_count[i] = 0;
    m_trace_event_capacity[i] = 1024;
    p_trace_event_arrays[i] = new trace_event_t[1024];
  }

  mutex_init(&m_trace_event_mutex);
}

void
trace_release()
{
  for (uint32_t i = 0; i < m_num_queues; ++i) 
  {
    delete [] p_trace_event_arrays[i];
  }

  delete [] p_trace_event_arrays;
  delete [] m_trace_event_capacity;
  delete [] m_trace_event_count;
  mutex_release(&m_trace_event_mutex);
}

void
trace_record(uint32_t queue_id, 
             trace_event_type_t event_type,
             const char* name, 
             const char* info)
{

  mutex_lock(&m_trace_event_mutex);
  if(m_trace_record_enabled)
  {
    timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    trace_event_t trace_event = {0};
    trace_event.m_time_ns = time.tv_sec * 1000 * 1000 * 1000 + time.tv_nsec;
    trace_event.m_event_type = event_type;
    if(info != nullptr)
    {
      strncpy(trace_event.m_info, info, _TNA_TRACE_MAX_INFO_STRING_LENGTH-1);
      trace_event.m_info[_TNA_TRACE_MAX_INFO_STRING_LENGTH-1] = '\0';
    }
    else
    {
      trace_event.m_info[0] = '\0';
    }

    if(info != nullptr)
    {
      strncpy(trace_event.m_name, name, _TNA_TRACE_MAX_NAME_STRING_LENGTH-1);
      trace_event.m_name[_TNA_TRACE_MAX_NAME_STRING_LENGTH-1] = '\0';
    }
    else
    {
      trace_event.m_name[0] = '\0';
    }

    if(m_trace_event_count[queue_id] >= m_trace_event_capacity[queue_id])
    {
      uint32_t new_capacity = m_trace_event_capacity[queue_id]*2;
      trace_event_t* new_buffer = new trace_event_t[new_capacity];
      memcpy(new_buffer, p_trace_event_arrays[queue_id], sizeof(trace_event_t)*m_trace_event_count[queue_id]);
      m_trace_event_capacity[queue_id] = new_capacity;
      delete [] p_trace_event_arrays[queue_id];
      p_trace_event_arrays[queue_id] = new_buffer;
    }

    (p_trace_event_arrays[queue_id])[m_trace_event_count[queue_id]] = trace_event;
    m_trace_event_count[queue_id]++;
  }
  mutex_unlock(&m_trace_event_mutex);
}

trace_event_t*
trace_get_trace_event_array(uint32_t queue_id, uint32_t* count)
{
  TNA_PERMA_ASSERT(!m_trace_record_enabled && "Trace arrays cannot be accessed while recording traces");

  *count = m_trace_event_count[queue_id];
  return p_trace_event_arrays[queue_id];
}

void
trace_flush()
{
  mutex_lock(&m_trace_event_mutex);
  for(uint32_t i = 0; i < m_num_queues; ++i)
  {
    m_trace_event_count[i] = 0;
  }
  mutex_unlock(&m_trace_event_mutex);
}

void
trace_record_enable()
{
  mutex_lock(&m_trace_event_mutex);
  m_trace_record_enabled = true;
  mutex_unlock(&m_trace_event_mutex);
}

void
trace_record_disable()
{
  mutex_lock(&m_trace_event_mutex);
  m_trace_record_enabled = false;
  mutex_unlock(&m_trace_event_mutex);
}

} /*  tna */ 
