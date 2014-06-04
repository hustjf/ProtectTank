//
//  Home.cpp
//  MyGame
//
//  Created by Alex on 14-6-3.
//
//

#include "Home.h"

bool Home::init()
{
	if (!Sprite::init())
	{
		return false;
	}
	return true;
}

Home* NormalHome::home()
{
    Home* home = Home::create();
    home->sprite = Sprite::create("symbol.png");
    home->sprite->setScale(0.7);
    home->addChild(home->sprite,0);
    home->homeHP = 10;
    return home;
}