#include "Tower.h" 
#include "DataModel.h"  
Tower* Tower::createWithType(TowerType towerType)
{
    Tower *tower;
    switch (towerType) {
        case TowerType::MachineGunTower:
            tower = MachineGunTower::tower();
            break;
        case TowerType::MissileTower:
            tower = MissileTower::tower();
            break;
        case TowerType::LaserTower:
            tower = LaserTower::tower();
            break;
        case TowerType::SlowTower:
            tower = SlowTower::tower();
            break;
        default:
            break;
    }
    return tower;
}

Creep* Tower::getClosestTarget() 
{    
	Creep *closestCreep = NULL;   
	double maxDistant = 99999; 
	// It should be bigger than the range.  
	DataModel *m = DataModel::getModel();  
	for(Vector<Creep*>::iterator i = m->targets.begin(); i < m->targets.end(); ++i)  
	{ 
		Sprite *target = *i;
		Creep *creep = (Creep *)target;    
		double curDistance = ccpDistance(this->getPosition(), creep->getPosition());   
		if (curDistance < maxDistant)
		{         
			closestCreep = creep;
			maxDistant = curDistance; 
		}    
	}    
	if (maxDistant < this->range) 
		return closestCreep;  
	return NULL; 
}

Tower* MachineGunTower::tower()
{
	Tower* tower = Tower::create();
	tower->sprite = Sprite::create("p1tankR.png");
	tower->setScale(0.5);
	tower->addChild(tower->sprite, 0);
	tower->range = 200;
    tower->price = 200;
	tower->schedule(schedule_selector(MachineGunTower::towerLogic), 0.2);
	return tower;
}

bool MachineGunTower::init() 
{     
	if (!Sprite::init())
	{        
		return false; 
	}
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("fire.wav");
	return true;
}     

void MachineGunTower::towerLogic(float dt) 
{    
	this->target = this->getClosestTarget();     
	if(this->target != NULL) 
	{    
		Point shootVector = this->target->getPosition() - this->getPosition(); 
		float shootAngle = ccpToAngle(shootVector);   
		float cocosAngle = CC_RADIANS_TO_DEGREES(-1 * shootAngle);   
		float rotateSpeed = 0.5 / M_PI;    
		float rotateDuration = fabs(shootAngle * rotateSpeed);  
		this->runAction(Sequence::create(RotateTo::create(rotateDuration, cocosAngle), NULL));  
		finishFiring();
	} 
}


void MachineGunTower::finishFiring() 
{    
	DataModel *m = DataModel::getModel();    
	if (this->target != NULL && this->target->curHp > 0 && this->target->curHp < 100)    
	{        
		this->nextProjectile = Projectile::createWithType(Projectile::BulletType::MachineGunBullet);
		this->nextProjectile->setPosition(this->getPosition());  
		this->getParent()->addChild(this->nextProjectile, 1);
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("fire.wav");
		m->projectiles.pushBack(this->nextProjectile);   
		float delta = 1.0f;        
		Point shootVector = -(this->target->getPosition() - this->getPosition());   
		Point normalizedShootVector = ccpNormalize(shootVector);  
		Point overshotVector = normalizedShootVector * 320;    
		Point offscreenPoint = (this->getPosition() - overshotVector);     
		this->nextProjectile->runAction(Sequence::create(MoveTo::create(delta, offscreenPoint),
			CallFuncN::create(this, callfuncN_selector(MachineGunTower::creepMoveFinished)), NULL));
		this->nextProjectile->setTag(2);
		this->nextProjectile = NULL;    
	} 
}  


void MachineGunTower::creepMoveFinished(Node* sender)  
{     
	DataModel * m = DataModel::getModel();   
	Sprite *sprite = (Sprite *)sender;  
	this->getParent()->removeChild(sprite,true);    
	m->projectiles.eraseObject((Projectile*)sprite);
}

Tower* MissileTower::tower()
{
	Tower* tower = Tower::create();
	tower->sprite = Sprite::create("p2tankR.png");
	tower->setScale(0.5);
	tower->addChild(tower->sprite, 0);
	tower->range = 200;
    tower->price = 300;
	tower->schedule(schedule_selector(MissileTower::towerLogic), 0.2);
	return tower;
}

bool MissileTower::init()
{
	if (!Sprite::init())
	{
		return false;
	}
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("fire.wav");
	return true;
}

void MissileTower::towerLogic(float dt)
{
	this->target = this->getClosestTarget();
	if(this->target != NULL)
	{
		Point shootVector = this->target->getPosition() - this->getPosition();
		float shootAngle = ccpToAngle(shootVector);
		float cocosAngle = CC_RADIANS_TO_DEGREES(-1 * shootAngle);
		float rotateSpeed = 0.5 / M_PI;
		float rotateDuration = fabs(shootAngle * rotateSpeed);
		this->runAction(Sequence::create(RotateTo::create(rotateDuration, cocosAngle), NULL));
		finishFiring();
	}
}


void MissileTower::finishFiring()
{
	DataModel *m = DataModel::getModel();
	if (this->target != NULL && this->target->curHp > 0 && this->target->curHp < 100)
	{
		this->nextProjectile = Projectile::createWithType(Projectile::BulletType::MissileBullet);
		this->nextProjectile->setPosition(this->getPosition());
		this->getParent()->addChild(this->nextProjectile, 1);
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("fire.wav");
		m->projectiles.pushBack(this->nextProjectile);
		float delta = 1.0f;
		Point shootVector = -(this->target->getPosition() - this->getPosition());
		Point normalizedShootVector = ccpNormalize(shootVector);
		Point overshotVector = normalizedShootVector * 320;
		Point offscreenPoint = (this->getPosition() - overshotVector);
		this->nextProjectile->runAction(Sequence::create(MoveTo::create(delta, offscreenPoint),
                                                         CallFuncN::create(this, callfuncN_selector(MissileTower::creepMoveFinished)), NULL));
		this->nextProjectile->setTag(2);
		this->nextProjectile = NULL;
	}
}


void MissileTower::creepMoveFinished(Node* sender)
{
	DataModel * m = DataModel::getModel();
	Sprite *sprite = (Sprite *)sender;
	this->getParent()->removeChild(sprite,true);
	m->projectiles.eraseObject((Projectile*)sprite);
}

Tower* LaserTower::tower()
{
	Tower* tower = Tower::create();
	tower->sprite = Sprite::create("p1tankD42.png");
	tower->setScale(0.5);
	tower->addChild(tower->sprite, 0);
	tower->range = 200;
    tower->price = 400;
	tower->schedule(schedule_selector(LaserTower::towerLogic), 0.2);
	return tower;
}

bool LaserTower::init()
{
	if (!Sprite::init())
	{
		return false;
	}
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("fire.wav");
	return true;
}

void LaserTower::towerLogic(float dt)
{
	this->target = this->getClosestTarget();
	if(this->target != NULL)
	{
		Point shootVector = this->target->getPosition() - this->getPosition();
		float shootAngle = ccpToAngle(shootVector);
		float cocosAngle = CC_RADIANS_TO_DEGREES(-1 * shootAngle);
		float rotateSpeed = 0.5 / M_PI;
		float rotateDuration = fabs(shootAngle * rotateSpeed);
		this->runAction(Sequence::create(RotateTo::create(rotateDuration, cocosAngle), NULL));
		finishFiring();
	}
}


void LaserTower::finishFiring()
{
	DataModel *m = DataModel::getModel();
	if (this->target != NULL && this->target->curHp > 0 && this->target->curHp < 100)
	{
		this->nextProjectile = Projectile::createWithType(Projectile::BulletType::LaserBullet);
		this->nextProjectile->setPosition(this->getPosition());
		this->getParent()->addChild(this->nextProjectile, 1);
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("fire.wav");
		m->projectiles.pushBack(this->nextProjectile);
		float delta = 1.0f;
		Point shootVector = -(this->target->getPosition() - this->getPosition());
		Point normalizedShootVector = ccpNormalize(shootVector);
		Point overshotVector = normalizedShootVector * 320;
		Point offscreenPoint = (this->getPosition() - overshotVector);
		this->nextProjectile->runAction(Sequence::create(MoveTo::create(delta, offscreenPoint),
                                                         CallFuncN::create(this, callfuncN_selector(LaserTower::creepMoveFinished)), NULL));
		this->nextProjectile->setTag(2);
		this->nextProjectile = NULL;
	}
}


void LaserTower::creepMoveFinished(Node* sender)
{
	DataModel * m = DataModel::getModel();
	Sprite *sprite = (Sprite *)sender;
	this->getParent()->removeChild(sprite,true);
	m->projectiles.eraseObject((Projectile*)sprite);
}

Tower* SlowTower::tower()
{
	Tower* tower = Tower::create();
	tower->sprite = Sprite::create("p1tankD32.png");
	tower->setScale(0.5);
	tower->addChild(tower->sprite, 0);
	tower->range = 200;
    tower->price = 500;
	tower->schedule(schedule_selector(SlowTower::towerLogic), 0.2);
	return tower;
}

bool SlowTower::init()
{
	if (!Sprite::init())
	{
		return false;
	}
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("fire.wav");
	return true;
}

void SlowTower::towerLogic(float dt)
{
	this->target = this->getClosestTarget();
	if(this->target != NULL)
	{
		Point shootVector = this->target->getPosition() - this->getPosition();
		float shootAngle = ccpToAngle(shootVector);
		float cocosAngle = CC_RADIANS_TO_DEGREES(-1 * shootAngle);
		float rotateSpeed = 0.5 / M_PI;
		float rotateDuration = fabs(shootAngle * rotateSpeed);
		this->runAction(Sequence::create(RotateTo::create(rotateDuration, cocosAngle), NULL));
		finishFiring();
	}
}


void SlowTower::finishFiring()
{
	DataModel *m = DataModel::getModel();
	if (this->target != NULL && this->target->curHp > 0 && this->target->curHp < 100)
	{
		this->nextProjectile = Projectile::createWithType(Projectile::BulletType::SlowBullet);
		this->nextProjectile->setPosition(this->getPosition());
        Point shootVector = -(this->target->getPosition() - this->getPosition());
        float shootAngle = ccpToAngle(shootVector);
		float cocosAngle = CC_RADIANS_TO_DEGREES(-1 * shootAngle);
        this->nextProjectile->setRotation(cocosAngle);
		this->getParent()->addChild(this->nextProjectile, 1);
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("fire.wav");
		m->projectiles.pushBack(this->nextProjectile);
		float delta = 1.0f;
		Point normalizedShootVector = ccpNormalize(shootVector);
		Point overshotVector = normalizedShootVector * 320;
		Point offscreenPoint = (this->getPosition() - overshotVector);
		this->nextProjectile->runAction(Sequence::create(MoveTo::create(delta, offscreenPoint),
                                                         CallFuncN::create(this, callfuncN_selector(SlowTower::creepMoveFinished)), NULL));
		this->nextProjectile->setTag(2);
		this->nextProjectile = NULL;
	}
}


void SlowTower::creepMoveFinished(Node* sender)
{
	DataModel * m = DataModel::getModel();
	Sprite *sprite = (Sprite *)sender;
	this->getParent()->removeChild(sprite,true);
	m->projectiles.eraseObject((Projectile*)sprite);
}