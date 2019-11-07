

#define _TNA_TRACES_MAX_INFO_STRING_LENGTH 1024
#define _TNA_TRACES_MAX_NAME_STRING_LENGTH 128

enum class trace_timing_event_type_t
{
  E_START = 0,
  E_RESUME,
  E_YIELD,
  E_STOP,
  E_NEW_FRAME,
  E_UNKNOWN
};

struct task_timing_event_t
{
  uint64_t                  m_time_ns = 0;
  task_timing_event_type_t  m_event_type = task_timing_event_type_t::E_UNKNOWN;
  char                      m_name[_TNA_TASKING_MAX_NAME_STRING_LENGTH] = {'\0'};
  char                      m_info[_TNA_TASKING_MAX_INFO_STRING_LENGTH] = {'\0'};
};

using task_timing_event_queue_t = queue_t<task_timing_event_t>;
