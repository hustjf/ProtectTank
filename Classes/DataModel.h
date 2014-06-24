#pragma once
#include "cocos2d.h"
#include "CCVector.h" 
#include "WayPoint.h"   
#include "Creep.h"   
#include "Tower.h"
#include "Projectile.h"
#include "StartScene.h"
#include "Home.h"

USING_NS_CC;       
class DataModel      
{     
public:     
	StartScene* _gameLayer;
	static DataModel* getModel(); 
	Vector<WayPoint*> waypoints;     
	Vector<Creep*> targets;       
	Vector<Wave*> waves;         
	Vector<Tower*> towers;   
	Vector<Projectile*> projectiles;
    std::unordered_multimap<float, float> buildedPoints;
	GameHUD* _gameHUDLayer;
    Home* home;
    int currentMoney;
    Tower::TowerType towerType;
    void clear();

private:       
	DataModel(){};         
	static DataModel * m_pInstance;   
};