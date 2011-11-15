#ifndef LOG_4_CPLUS_H
#define LOG_4_CPLUS_H

#include <string>
#include "utils/log/log_module.h"
#include "utils/log/logger.h"


#ifdef _DEBUG
#define DEF_LOG_MODULE(module)  \
    struct _ ## module {      \
        _ ## module() : module_name(#module){} \
        std::string  module_name; \
    };   \
    typedef utl::details::LogModule< _##module >     log_ ## module; \

#else

#define DEF_LOG_MODULE(module)  \
	struct log_ ## module { \
		log_ ## module(std::string, std::string event){}; \
	};

#endif

#endif//LOG_4_CPLUS_H

