#include "Wave.h" 
USING_NS_CC;   
bool Wave::init()
{    
	if (!Node::init())   
	{         
		return false;  
	}       
	return true;
}   

Wave* Wave::initWithCreep(Creep::CreepType creeptype, double SpawnRate, int TotalCreeps)
{     
	this->creeptype = creeptype;  
	this->spawnRate = SpawnRate;  
	this->totalCreeps = TotalCreeps;
    this->time = 0;
    this->lastTimeTargetAdded = 0;
	return this;
}