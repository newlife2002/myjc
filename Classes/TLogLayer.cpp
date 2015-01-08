//
//  TLogLayer.cpp
//  dddd
//
//  Created by 唐新明 on 12/3/14.
//
//

#include "TLogLayer.h"
#include "TLog.h"
#include "VisibleRect.h"

USING_NS_CC;
using namespace std;

bool CTLogLayer::init()
{
    setTouchEnabled(true);
    m_pLogNode = CCLabelTTF::create("", "Arial", 12, CCSizeZero, kCCTextAlignmentLeft);
    m_pLogNode->setAnchorPoint(CCPoint(0, 0));
    m_pLogNode->retain();
    m_bShow = false;
    CCPoint pt = VisibleRect::leftBottom();
    m_pLogNode->setPosition(pt);
    m_pLogNode->setZOrder(100);
    return true;
}

void CTLogLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    ShowOrHide();
}

void CTLogLayer::ShowOrHide()
{
    static int i=0;
    i++;
    CCDirector::sharedDirector()->setDisplayStats(m_bShow);
    if (!m_bShow) {
        schedule(schedule_selector(CTLogLayer::UpdateLog), 1.0f);
        addChild(m_pLogNode);
        m_bShow = true;
    } else {
        this->unschedule(schedule_selector(CTLogLayer::UpdateLog));
        removeChild(m_pLogNode);
        m_bShow = false;
    }
}

void CTLogLayer::UpdateLog(float dt)
{
    static string lastStr;
    string str;
    CTLog::TVecStr vecLogs;
    CTLog::Instance().GetLogs(0, vecLogs, 0, -1);
    for (CTLog::TVecStr::iterator it = vecLogs.begin();
         it != vecLogs.end(); ++it) {
        str.append(*it);
        str.append("\r\n");
    }
    if (lastStr != str) {
        lastStr = str;
        m_pLogNode->setString(str.c_str());
    }
}

