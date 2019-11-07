

#ifndef _TNA_TRACE_H_
#define _TNA_TRACE_H_

#include "../common.h"

#define _TNA_TRACE_MAX_INFO_STRING_LENGTH 1024
#define _TNA_TRACE_MAX_NAME_STRING_LENGTH 128

namespace  tna
{

enum class trace_event_type_t
{
  E_UNKNOWN = 0,
  E_TASK_START,
  E_TASK_RESUME,
  E_TASK_YIELD,
  E_TASK_STOP,
  E_NEW_FRAME,
};

struct trace_event_t
{
  uint64_t                  m_time_ns;
  trace_event_type_t        m_event_type;
  char                      m_name[_TNA_TRACE_MAX_NAME_STRING_LENGTH];
  char                      m_info[_TNA_TRACE_MAX_INFO_STRING_LENGTH];
};


void
trace_init(uint32_t num_queues);

void
trace_release();

/**
 * \brief Gets the event queue for the specified queue
 *
 * \param queueid The queue id to get
 * \param count Pointer to variable to store the count in the array
 *
 * \return Returns the pointer to the queue
 */
trace_event_t*
trace_get_trace_event_array(uint32_t queueid, 
                            uint32_t* count);

/**
 * \brief Records a new frame timing event
 */
void
trace_record(uint32_t queue_id, 
             trace_event_type_t event_type,
             const char* name, 
             const char* info);

/**
 * \brief Flushes all the queues
 */
void
trace_flush();

/**
 * \brief Enables recording of traces
 */
void
trace_record_enable();

/**
 * \brief Disables recording of traces
 */
void
trace_record_disable();


} /*  tna */ 

#endif
