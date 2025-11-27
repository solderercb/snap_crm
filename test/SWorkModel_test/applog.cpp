#include "applog.h"

SAppLog* SAppLog::p_instance = nullptr;

SAppLog::SAppLog()
{
}

SAppLog *SAppLog::getInstance()
{
    if( !p_instance )
      p_instance = new SAppLog();
    return p_instance;
}
