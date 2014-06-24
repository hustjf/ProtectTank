//
//  ThirdScene.h
//  MyGame
//
//  Created by Alex on 14-6-19.
//
//

#ifndef __MyGame__ThirdScene__
#define __MyGame__ThirdScene__

#include <iostream>
#include "StartScene.h"

class ThirdScene : public StartScene
{
public:
    static cocos2d::Scene* createScene();
    bool init();
	CREATE_FUNC(ThirdScene);
    void addTarget();
    void gameLogic(float dt);
    void FollowPath(Node *sender);
    virtual void update(float dt);
    void restartEvent(Widget* target, ui::TouchEventType type);
    void continueEvent(Widget* target, ui::TouchEventType type);
};

#endif /* defined(__MyGame__ThirdScene__) */
