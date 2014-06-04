#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "CCScene.h"
#include "SimpleAudioEngine.h"
#include <cstdlib>
#include "cocostudio/CocoStudio.h"
#include "StartScene.h"
using namespace cocos2d::ui;
USING_NS_CC;
USING_NS_CC_EXT;

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    Layout* widget;
    ImageView* selectTank;
    Button* startBtn;
    Button* highScoreBtn;
    
    void startEvent(Widget* target, ui::TouchEventType type);
    
    void highScoreEvent(Widget* target, ui::TouchEventType type);
    
};

#endif // __HELLOWORLD_SCENE_H__