//
//  SecondScene.h
//  MyGame
//
//  Created by Alex on 14-6-18.
//
//

#ifndef __MyGame__SecondScene__
#define __MyGame__SecondScene__

#include <iostream>
#include "StartScene.h"

class SecondScene : public StartScene
{
public:
    static cocos2d::Scene* createScene();
    bool init();
	CREATE_FUNC(SecondScene);
    void addTarget();
    void gameLogic(float dt);
    void FollowPath(Node *sender);
    virtual void update(float dt);
    void restartEvent(Widget* target, ui::TouchEventType type);
    void continueEvent(Widget* target, ui::TouchEventType type);
};

#endif /* defined(__MyGame__SecondScene__) */
