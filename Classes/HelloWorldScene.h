#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "TLogLayer.h"
#include "CChessboard.h"


class HelloWorld : public CTLogLayer
{
    typedef CTLogLayer SuperClass;
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
    
public:
    HelloWorld();
    ~HelloWorld();
    //virtual void draw();
    virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    void doTouchesBegin(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    void doTouches(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    void doTouchesEnd(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);

protected:
    CChessboard m_cb;
    void initBoard();
    
    cocos2d::CCDrawNode * m_pDrawNode;
    cocos2d::CCLabelTTF * m_pLable;
    void drawBoard();
    bool getChessBoardPos(cocos2d::CCPoint pt, CChessboard::SChessPoint& chessPos);
    CChessboard::SChessPoint m_curSel;

    void GetDot(int x, int y, cocos2d::CCPoint& point, float& r);
    
    float m_distance, m_deltax, m_deltay, m_scale;
    cocos2d::CCPoint m_curPos;
    bool m_bBegin;
    
    void doMoveBegin(cocos2d::CCSet *pTouches);
    void doMove(cocos2d::CCSet *pTouches);
    void doMoveEnd();
    cocos2d::CCPoint m_moveBeginPoint;
    bool m_bMoveBegin;
};

#endif // __HELLOWORLD_SCENE_H__
