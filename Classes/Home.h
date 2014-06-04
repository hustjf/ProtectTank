//
//  Home.h
//  MyGame
//
//  Created by Alex on 14-6-3.
//
//

#ifndef __MyGame__Home__
#define __MyGame__Home__
#pragma once

#include <iostream>
#include "cocos2d.h"

class Home: public cocos2d::Sprite
{
public:
	int homeHP;
	Sprite* sprite;
    virtual bool init();
	CREATE_FUNC(Home);
};

class NormalHome:public Home
{
public:
    static Home* home();
};

#endif /* defined(__MyGame__Home__) */
