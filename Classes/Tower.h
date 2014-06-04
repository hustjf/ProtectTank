#pragma once 
#include "cocos2d.h"
#include "Creep.h"  
#include "Projectile.h"   

class Tower: public Sprite 
{
public:  
	int range; 
	Sprite* sprite;
	Creep* target;   
	Sprite * selSpriteRange;
	Creep* getClosestTarget(); 
	Projectile* nextProjectile;
	CREATE_FUNC(Tower); 
};   

class MachineGunTower : public Tower  
{
public: 
	static Tower* tower(); 
	bool virtual init();  
	void towerLogic(float dt);
	void finishFiring(); 
	void creepMoveFinished(Node* sender);
};
