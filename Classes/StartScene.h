//
//  StartScene.h
//  MyGame
//
//  Created by Alex on 14-5-29.
//
//

#ifndef __MyGame__StartScene__
#define __MyGame__StartScene__

#include <iostream>
#include "HelloWorldScene.h"
#include "Creep.h"
#include "WayPoint.h"
#include "Wave.h"
#include "GameHud.h"
#include "Tower.h"

using namespace cocos2d::ui;
USING_NS_CC;
USING_NS_CC_EXT;

class StartScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    bool init();
	CREATE_FUNC(StartScene);
    Layout* widget;
    int currentLevel;
	void addWayPoint();
	void addWaves();
    void addHome();
	void FollowPath(Node *sender);
	void gameLogic(float dt);
	void addTarget();
	virtual void update(float dt);
	Wave* getCurrentWave();
	Wave* getNextWave();
	void addTower(Point pos, Tower::TowerType towerType);
	Point tileCoordForPosition(Point position);
	bool canBuildOnTilePosition(Point pos);
	Point boundLayerPos(Point newPos);
	Point position;
	GameHUD* gameHUD;
    Vector<Creep*> targetsToDelete;

	cocos2d::TMXTiledMap* _tileMap;
	cocos2d::TMXLayer* _background;
    Text* money;
    Text* life;
    Text* currentWave;
    Text* totalWave;
    
    void restartEvent(Widget* target, ui::TouchEventType type);
    void continueEvent(Widget* target, ui::TouchEventType type);
    void mainMenuEvent(Widget* target, ui::TouchEventType type);
};



#endif /* defined(__MyGame__StartScene__) */
