//
//  TLog.cpp
//  dddd
//
//  Created by 唐新明 on 12/3/14.
//
//

#include "TLog.h"
#include "cocos2d.h"


CTLog::CTLog()
{
    
}

CTLog & CTLog::Instance()
{
    static CTLog r;
    return r;
}
void CTLog::Log(int level, const char * fmt, ...)
{
    char szBuf[1024];
    
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(szBuf, sizeof(szBuf), fmt, ap);
    va_end(ap);
    
    SLog sLog;
    sLog.level = level;
    sLog.msg = szBuf;
    
    m_Logs.push_back(sLog);
    if (m_Logs.size() > 20)
        m_Logs.pop_front();
    CCLOG("%s", szBuf);
}

void CTLog::GetLogs(int levelMask, TVecStr & outStr, int iFrom, int iCount)
{
    for (TListLog::iterator it = m_Logs.begin(); it != m_Logs.end(); ++it) {
        outStr.push_back(it->msg);
    }
}

