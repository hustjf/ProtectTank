#include "Creep.h" 
#include "WayPoint.h"
#include "DataModel.h" 
USING_NS_CC;  
bool Creep::init() 
{     
	if (!Sprite::init())   
	{         
		return false;  
	}   
	return true; 
}  

Creep* Creep::initWithCreep(Creep* copyFrom)
{    
	return NULL; 
}   

Creep* FastGrayCreep::creep(int wayPointNum,bool inverse)
{    
	auto creep = Creep::create();
    if (inverse)
        creep->sprite = Sprite::create("enemy1L.png");
    else
        creep->sprite = Sprite::create("enemy1R.png");
    creep->inverse = inverse;
	creep->setScale(0.4);  
	creep->addChild(creep->sprite, 0); 
	creep->curHp = 15;
    creep->maxHp = 15;
	creep->moveDuration = 3;   
	creep->curWaypoint = 0;
    creep->bonusMoney = 50;
	creep->wayPointNum = wayPointNum;
	return creep;
}  

Creep* StrongGreenCreep::creep(int wayPointNum,bool inverse)
{     
	auto creep = Creep::create();   
	if (inverse)
        creep->sprite = Sprite::create("enemy2L.png");
    else
        creep->sprite = Sprite::create("enemy2R.png");
    creep->inverse = inverse;
	creep->setScale(0.4);    
	creep->addChild(creep->sprite, 0);  
	creep->curHp = 30;
    creep->maxHp = 30;
	creep->moveDuration = 5;    
	creep->curWaypoint = 0;
    creep->bonusMoney = 80;
	creep->wayPointNum = wayPointNum;
	return creep;
}

Creep* MonsterYellowCreep::creep(int wayPointNum,bool inverse)
{     
	auto creep = Creep::create();   
	if (inverse)
        creep->sprite = Sprite::create("enemy3L.png");
    else
        creep->sprite = Sprite::create("enemy3R.png");
    creep->inverse = inverse;
	creep->setScale(0.4);    
	creep->addChild(creep->sprite, 0);  
	creep->curHp = 45;
    creep->maxHp = 45;
	creep->moveDuration = 8;    
	creep->curWaypoint = 0;
    creep->bonusMoney = 120;
	creep->wayPointNum = wayPointNum;
	return creep;
}

//Creep* GiantBlueCreep::creep(int wayPointNum)
//{     
//	auto creep = Creep::create();   
//	creep->sprite = Sprite::create("enemy4D.png");  
//	creep->setScale(0.4);    
//	creep->addChild(creep->sprite, 0);  
//	creep->curHp = 30;   
//	creep->moveDuration = 8;    
//	creep->curWaypoint = 0;  
//	creep->wayPointNum = wayPointNum;
//	return creep;
//}
//
//Creep* TitanRedCreep::creep(int wayPointNum)
//{     
//	auto creep = Creep::create();   
//	creep->sprite = Sprite::create("enemy5D.png");  
//	creep->setScale(0.4);    
//	creep->addChild(creep->sprite, 0);  
//	creep->curHp = 30;   
//	creep->moveDuration = 8;    
//	creep->curWaypoint = 0;  
//	creep->wayPointNum = wayPointNum;
//	return creep;
//}

WayPoint* Creep::getCurrentWaypoint()
{     
	DataModel* m = DataModel::getModel();   
	WayPoint* waypoint = (WayPoint *)m->waypoints.at(this->curWaypoint);  
	return waypoint; 
}   

WayPoint* Creep::getNextWaypoint() 
{    
	DataModel* m = DataModel::getModel(); 
	if (this->curWaypoint != m->waypoints.size()-2)
	{        
		this->curWaypoint++;   
	} 
	else  
	{     
//		this->curWaypoint = 0;
        return nullptr;
	}
	CCLOG("%d",this->curWaypoint); 
	// For testing.       
	WayPoint *waypoint = (WayPoint *)m->waypoints.at(curWaypoint);   
	return waypoint; 
}

Creep* Creep::createWithType(CreepType creepType,int wayPointNum,bool inverse){
	
	Creep* creep;

	//Create the creep by the creep type
	switch (creepType) {
	case CreepType::FastGrayCreep:

		creep = FastGrayCreep::creep(wayPointNum,inverse);
			break;
	case CreepType::StrongGreenCreep:
			
		creep = StrongGreenCreep::creep(wayPointNum,inverse);
			break;
	case CreepType::MonsterYellowCreep:
			
		creep = MonsterYellowCreep::creep(wayPointNum,inverse);
			break;
//	case CreepType::GiantBlueCreep:
//			
//		creep = GiantBlueCreep::creep(wayPointNum);
//			break;
//			
//	case CreepType::TitanRedCreep:
//			
//		creep = TitanRedCreep::creep(wayPointNum);
//			break;
			
	default:
			break;
	}

	return creep;
}