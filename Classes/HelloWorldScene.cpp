#include "HelloWorldScene.h"
#include "VisibleRect.h"
#include "TLog.h"

USING_NS_CC;

HelloWorld::HelloWorld()
:m_cb(5, 4)
{
    m_pDrawNode = NULL;
    m_scale = 1;
    m_bBegin = false;
    m_bMoveBegin = false;
    m_curPos = ccp(0, 0);
}

HelloWorld::~HelloWorld()
{
    m_pLable->release();
}

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !SuperClass::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...
    //setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, 10);
    
    m_pLable = CCLabelTTF::create("", "Arial", 24);
    m_pLable->retain();
    CCPoint pos = VisibleRect::right();
    m_pLable->setPosition(pos);
    m_pLable->setAnchorPoint(ccp(1, 1));
    addChild(m_pLable);
    
    drawBoard();
    
    return true;
}

ccColor4F getColor(CChessboard::EChessType eType)
{
    if (eType == CChessboard::e_red)
        return ccc4f(1, 0, 0, 1);
    else if (eType == CChessboard::e_blue)
        return ccc4f(0, 0, 1, 1);
    else if (eType == CChessboard::e_yellow)
        return ccc4f(1, 1, 0, 1);
    else
        return ccc4f(0.5, 0.5, 0, 1);
}
//
//void HelloWorld::initBoard()
//{
//    CChessboard::TVecChessPoint vec;
//    m_cb.GetBoard(vec);
//    
//    CTLog::Instance().Log(1, "Width: %f; Height: %f.", VisibleRect::width(), VisibleRect::height());
//    
////    CCDrawNode * pDrawNode = CCDrawNode::create();
////    addChild(pDrawNode);
//    for (CChessboard::TVecChessPoint::iterator it = vec.begin();
//         it != vec.end(); ++it) {
//        CCPoint pt;
//        float r;
//        GetDot(it->pos.x, it->pos.y, pt, r);
////        pDrawNode->drawDot(pt, r, ccc4f(0.1, 0.3, 0.6, 1));
//        
////        if (it->type != CChessboard::e_none) {
//            CCSprite * pChess = CCSprite::create("1.png");
//            CCSize size = pChess->getContentSize();
//            pChess->setScale(2*r/size.width);
//            pChess->setPosition(pt);
//            pChess->setColor(getColor(it->type));
//            addChild(pChess);
////        }
//    }
//}

void HelloWorld::drawBoard()
{
    if (m_pDrawNode) {
        removeChild(m_pDrawNode);
    }
    
    CChessboard::TVecChessPoint vec;
    m_cb.GetBoard(vec);
    
    m_pDrawNode = CCDrawNode::create();
    m_pDrawNode->setScale(m_scale);
    m_pDrawNode->setAnchorPoint(ccp(0, 0));
    m_pDrawNode->setPosition(m_curPos);
    addChild(m_pDrawNode);
    
    CChessboard::TMapPath foundPoses;
    for (CChessboard::TVecChessPoint::iterator it = vec.begin();
         it != vec.end(); ++it) {
        CCPoint pt;
        float r;
        GetDot(it->pos.x, it->pos.y, pt, r);
        
        ccColor4F c = getColor(it->type);
        if (it->pos == m_curSel.pos) {
            m_pDrawNode->drawDot(pt, r, ccc4f(0.5, 1, 0.5, 1));
            m_pDrawNode->drawDot(pt, r-2, c);
            
            m_cb.GetMovables(m_curSel.pos, foundPoses);
        } else
            m_pDrawNode->drawDot(pt, r, c);
    }
    
    for (CChessboard::TMapPath::iterator it = foundPoses.begin();
         it != foundPoses.end(); ++it) {
        SPos pos = it->first;
        CCPoint pt;
        float r;
        GetDot(pos.x, pos.y, pt, r);
        m_pDrawNode->drawDot(pt, r, ccc4f(0.5, 0.5, 0.5, 1));
    }
    
    if (m_cb.getWinner() != CChessboard::e_none) {
        m_pLable->setString("You WIN!");
    } else
        m_pLable->setString("You move.");
}

void HelloWorld::GetDot(int x, int y, CCPoint& point, float& r)
{
    float rate = 300.0f / 17 / tan(M_PI/3);
    point = ccp(20+x*rate, 20+y*300.0f / 17);
    r = rate;
}

bool HelloWorld::getChessBoardPos(cocos2d::CCPoint pt0, CChessboard::SChessPoint& chessPos)
{
    CChessboard::TVecChessPoint vec;
    m_cb.GetBoard(vec);
    for (CChessboard::TVecChessPoint::iterator it = vec.begin();
         it != vec.end(); ++it) {
        CCPoint pt;
        float r;
        GetDot(it->pos.x, it->pos.y, pt, r);
        
        if (ccpDistance(pt0, pt) <= r)
        {
            chessPos = *it;
            return true;
        }
    }
    return false;
}


void HelloWorld::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    CChessboard::SChessPoint chessPoint;
    bool bFound = false;
    CCSetIterator iter = pTouches->begin();
    for (; iter != pTouches->end(); iter++)
    {
        CCTouch* pTouch = (CCTouch*)(*iter);
        CCPoint location = m_pDrawNode->convertTouchToNodeSpace(pTouch);
        CTLog::Instance().Log(1, "point at: (%f, %f)", location.x, location.y);
        if (getChessBoardPos(location, chessPoint)) {
            bFound = true;
            break;
        }
    }
    TVecPos outPath;
    m_cb.DoMove(m_curSel.pos, chessPoint.pos, outPath);
    m_curSel = chessPoint;
    
    if (bFound)
        drawBoard();
    else
        SuperClass::ccTouchesBegan(pTouches, pEvent);
}

void HelloWorld::doTouchesBegin(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    if(pTouches->count() >= 2){
        m_bBegin = true;
        CCSetIterator iter = pTouches->begin();
        CCPoint mPoint1 = convertTouchToNodeSpace((CCTouch*)(*iter));
        //mPoint1 = CCDirector::sharedDirector()->convertToGL(mPoint1);
        iter ++;
        CCPoint mPoint2 = convertTouchToNodeSpace((CCTouch*)(*iter));
        //mPoint2 = CCDirector::sharedDirector()->convertToGL(mPoint2);
        
        m_distance = sqrt((mPoint1.x - mPoint2.x) * (mPoint1.x - mPoint2.x) + (mPoint1.y - mPoint2.y) * (mPoint1.y - mPoint2.y));
        
        m_deltax = (mPoint1.x + mPoint2.x)/2 - m_pDrawNode->getPositionX();
        m_deltay = (mPoint1.y + mPoint2.y)/2 - m_pDrawNode->getPositionY();
    }
}

void HelloWorld::doTouchesEnd(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    m_bBegin = false;
}

void HelloWorld::doTouches(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    if(pTouches->count() >= 2){
        CCSetIterator iter = pTouches->begin();
        CCPoint mPoint1 = convertTouchToNodeSpace((CCTouch*)(*iter));
        //mPoint1 = CCDirector::sharedDirector()->convertToGL(mPoint1);
        iter ++;
        CCPoint mPoint2 = convertTouchToNodeSpace((CCTouch*)(*iter));
        //mPoint2 = CCDirector::sharedDirector()->convertToGL(mPoint2);
        
        float mdistance = sqrt((mPoint1.x - mPoint2.x) * (mPoint1.x - mPoint2.x) + (mPoint1.y - mPoint2.y) * (mPoint1.y - mPoint2.y));
        if (mdistance < 0.001)
            return;
        float scale = mdistance / m_distance;
        m_scale = scale * m_scale;
        
        m_distance = mdistance;
        
        //m_pDrawNode->setAnchorPoint(ccp((mPoint1.x + mPoint2.x)/2, (mPoint1.y + mPoint2.y)/2));
        m_pDrawNode->setScale(m_scale);
        
        float x = (mPoint1.x + mPoint2.x)/2 - m_deltax*scale;
        float y = (mPoint1.y + mPoint2.y)/2 - m_deltay*scale;
        m_curPos = ccp(x, y);
        m_pDrawNode->setPosition(m_curPos);
        m_deltax = m_deltax*scale;
        m_deltay = m_deltay*scale;
    }
}

void HelloWorld::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    //CTLog::Instance().Log(1, "Move: %d", pTouches->count());
    if (pTouches->count() >= 2) {
        if (!m_bBegin)
            doTouchesBegin(pTouches, pEvent);
        else
            doTouches(pTouches, pEvent);
    } else
    {
//        if (m_bBegin) {
//            doTouchesEnd(pTouches, pEvent);
//        }
//        
//        if (!m_bMoveBegin) {
//            doMoveBegin(pTouches);
//        }
//        else
//            doMove(pTouches);
    }
}

void HelloWorld::doMoveBegin(cocos2d::CCSet *pTouches)
{
    m_bMoveBegin = true;
    CCSetIterator iter = pTouches->begin();
    m_moveBeginPoint = convertTouchToNodeSpace((CCTouch*)(*iter));
}
void HelloWorld::doMove(cocos2d::CCSet *pTouches)
{
    CCSetIterator iter = pTouches->begin();
    CCPoint curPoint = convertTouchToNodeSpace((CCTouch*)(*iter));
    
    m_curPos.x = m_curPos.x + curPoint.x - m_moveBeginPoint.x;
    m_curPos.y = m_curPos.y + curPoint.y - m_moveBeginPoint.y;
    m_moveBeginPoint = curPoint;
    m_pDrawNode->setPosition(m_curPos);
}
void HelloWorld::doMoveEnd()
{
    m_bMoveBegin = false;
}


void HelloWorld::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
    doTouchesEnd(pTouches, pEvent);
    doMoveEnd();
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}
