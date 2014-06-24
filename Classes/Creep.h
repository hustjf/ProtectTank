#pragma once
#include "cocos2d.h" 
#include "WayPoint.h"  
USING_NS_CC; 

class Creep: public Sprite 
{ 
public:  

	enum class CreepType
    {
		FastGrayCreep,
		StrongGreenCreep,
		MonsterYellowCreep,
		GiantBlueCreep,
		TitanRedCreep,
    };

	CreepType creepType;
	
	/** Create the creep by their types */
	static Creep* createWithType(CreepType creepType,int wayPointNum,bool inverse);
    CCProgressTimer* lifeBar;
    Sprite* emptyLifeBar;
    bool inverse;
	int curHp;
    int maxHp;
	int moveDuration;  
	int curWaypoint;  
	int tag;   
	int wayPointNum;
    int bonusMoney;
	Sprite* sprite;
	virtual bool init(); 
	Creep* initWithCreep(Creep* copyFrom);  
	WayPoint* getNextWaypoint();   
	WayPoint* getCurrentWaypoint();   
	CREATE_FUNC(Creep);
};   

class FastGrayCreep: public Creep  
{
public:   
	static Creep* creep(int wayPointNum,bool inverse);
};   

class StrongGreenCreep: public Creep 
{
public:   
	static Creep* creep(int wayPointNum,bool inverse);
}; 

class MonsterYellowCreep: public Creep 
{
public:   
	static Creep* creep(int wayPointNum,bool inverse);
}; 

class GiantBlueCreep: public Creep 
{
public:   
	static Creep* creep(int wayPointNum,bool inverse);
}; 

class TitanRedCreep: public Creep 
{
public:   
	static Creep* creep(int wayPointNum,bool inverse);
}; 

