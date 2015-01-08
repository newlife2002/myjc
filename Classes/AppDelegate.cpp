#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "TLog.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);
	
    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);
    
    CCSize designSize = CCSizeMake(480, 320);
    CCEGLView::sharedOpenGLView()->setDesignResolutionSize(designSize.width, designSize.height, kResolutionShowAll);
    CCSize sz = CCEGLView::sharedOpenGLView()->getFrameSize();
    CCRect rc = CCEGLView::sharedOpenGLView()->getViewPortRect();
    CCSize rc1 = CCEGLView::sharedOpenGLView()->getDesignResolutionSize();
    CCSize rc2 = CCEGLView::sharedOpenGLView()->getVisibleSize();
    
    CTLog::Instance().Log(1, "frame size: %f, %f; \nview: %f, %f, %f, %f\n"
                          "DesignResolutionSize: %f, %f\n"
                          "VisibleSize: %f, %f",
                          sz.width, sz.height,
                          rc.origin.x, rc.origin.y, rc.size.width, rc.size.height,
                          rc1.width, rc1.height,
                          rc2.width, rc2.height);

    // create a scene. it's an autorelease object
    CCScene *pScene = HelloWorld::scene();

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
