#include "HelloWorldScene.h"
using namespace cocostudio;

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

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
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
//    auto closeItem = MenuItemImage::create(
//                                           "CloseNormal.png",
//                                           "CloseSelected.png",
//                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
//    
//	closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
//                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
//    auto menu = Menu::create(closeItem, NULL);
//    menu->setPosition(Point::ZERO);
//    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
//    auto label = LabelTTF::create("Hello World", "Arial", 24);
//    
//    // position the label on the center of the screen
//    label->setPosition(Point(origin.x + visibleSize.width/2,
//                            origin.y + visibleSize.height - label->getContentSize().height));
//
//    // add the label as a child to this layer
//    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
//    auto backGround = Sprite::create("tankResources/start.jpg");
//
//    // position the sprite on the center of the screen
//    backGround->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
//
//    // add the sprite as a child to this layer
//    this->addChild(backGround, 0);
//    
//    auto selectTank = Sprite::create("tankResources/selecttank.png");
//    selectTank->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
//    this->addChild(selectTank,1);
    widget = dynamic_cast<Layout*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("StartUi_1/StartUi_1.json"));
    this->addChild(widget,0);
    
    startBtn = dynamic_cast<Button*>(widget->getChildByName("startGameBtn"));
    startBtn->addTouchEventListener(this, toucheventselector(HelloWorld::startEvent));

    achievementBtn = dynamic_cast<Button*>(widget->getChildByName("achievementBtn"));
    achievementBtn->addTouchEventListener(this, toucheventselector(HelloWorld::achievementEvent));
    
    selectTank = dynamic_cast<ImageView*>(widget->getChildByName("selectTank"));
    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::startEvent(Widget* target, TouchEventType type)
{
    if(type==TouchEventType::TOUCH_EVENT_ENDED){
        selectTank->setPosition(Point(startBtn->getPositionX()-166, startBtn->getPositionY()));
        
		auto replaceScene = TransitionFadeBL::create(2, StartScene::createScene());
        
		Director::getInstance()->replaceScene(replaceScene);
	}
}

void HelloWorld::achievementEvent(Widget* target, TouchEventType type)
{
    if(type==TouchEventType::TOUCH_EVENT_ENDED){
        selectTank->setPosition(Point(achievementBtn->getPositionX()-166, achievementBtn->getPositionY()));
	}
}
