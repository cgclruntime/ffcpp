#ifndef UTILS_LOG_LOG_MODULE_H
#define UTILS_LOG_LOG_MODULE_H

#include "utils/log/logger.h"

namespace utl{
namespace details{


template <typename MTy_>
class LogModule
{
private:
    LogModule(){};
public:
    LogModule(std::string strLogger, std::string strEvent)
    : m_strModuleName()
    , m_strLogger(strLogger)
    , m_strLogEvent(strEvent){
        MTy_ t;
        m_strModuleName = t.module_name;
    };

    virtual ~LogModule()
    {
        utl::Logger<0> *p = utl::Logger<0>::getInstance();
        if(p && p->isModuleLogged(m_strModuleName))
        {
#ifdef LOG_4_CPLUS
        	LOG_INFO(m_strModuleName+"     "+m_strLogger, m_strLogEvent);
#elif _DEBUG
            std::cout<<m_strLogger<<"  "<<m_strLogEvent<<std::endl;
#endif
        }
    };

private:
    std::string         m_strModuleName;
    std::string         m_strLogger;
    std::string         m_strLogEvent;
};

}//end namespace details
}//end namespace utl

#endif

