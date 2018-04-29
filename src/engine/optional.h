

#ifndef _TNA_OPTIONAL_H_
#define _TNA_OPTIONAL_H_ value


#include <boost/optional.hpp>

namespace tna {

template<typename T>
using optional = boost::optional<T>;
  
} /* tna */ 
#endif /* ifndef _TNA_OPTIONAL_H_ */
