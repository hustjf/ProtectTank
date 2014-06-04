#pragma once 
#include "cocos2d.h" 
#include "Creep.h"
USING_NS_CC; 

class Wave: public Node 
{ 
public:  
	Point position;
	int totalCreeps; 
	double spawnRate;
	Creep::CreepType creeptype;
    double time;
    double lastTimeTargetAdded;
	virtual bool init();
    /**
     *@parameter SpawnRate 重生速率
     */
	Wave* initWithCreep(Creep::CreepType creeptype, double SpawnRate,int TotalCreeps);
	CREATE_FUNC(Wave);
};