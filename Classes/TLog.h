//
//  TLog.h
//  dddd
//
//  Created by 唐新明 on 12/3/14.
//
//

#ifndef TLog_h
#define TLog_h

#include <string>
#include <vector>
#include <list>

class CTLog
{
    CTLog();
    
public:
    typedef std::vector<std::string> TVecStr;
    static CTLog & Instance();
    void Log(int level, const char * fmt, ...);
    
    void GetLogs(int levelMask, TVecStr & outStr, int iFrom, int iCount);
protected:
    typedef struct _SLog
    {
        int level;
        std::string msg;
    }SLog;
    typedef std::list<SLog> TListLog;
    TListLog m_Logs;
};

#endif
