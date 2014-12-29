//
//  TLogLayer.h
//  dddd
//
//  Created by 唐新明 on 12/3/14.
//
//

#ifndef TLogLayer_h
#define TLogLayer_h

#include "cocos2d.h"

class CTLogLayer: public cocos2d::CCLayer
{
public:
    virtual bool init();
    
    virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);

protected:
    bool m_bShow;
    cocos2d::CCLabelTTF * m_pLogNode;
    void ShowOrHide();
    void UpdateLog(float dt);
};

#endif
