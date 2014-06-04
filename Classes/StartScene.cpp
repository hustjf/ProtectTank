//
//  StartScene.cpp
//  MyGame
//
//  Created by Alex on 14-5-29.
//
//

#include "StartScene.h"
#include "DataModel.h"
#include "Tower.h"
#include <Vector>
#include <string>
#include <sstream>


Scene* StartScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = StartScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    
    auto myGameHUD = GameHUD::shareHUD();
	DataModel *m = DataModel::getModel();
	m->_gameHUDLayer = myGameHUD;
	scene->addChild(myGameHUD, 1);
    return scene;
}

bool StartScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
//    widget = dynamic_cast<Layout*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("StartUi_1/StartUi_1.json"));
//    this->addChild(widget,0);
    std::string file = "01.tmx";
	auto str = String::createWithContentsOfFile(FileUtils::getInstance()-> fullPathForFilename(file.c_str()).c_str());
	this->_tileMap =TMXTiledMap::createWithXML(str->getCString(),"");
	this->_background = _tileMap->layerNamed("bg");
	_tileMap->setPosition(Point(0, 0));
	addChild(_tileMap, -1);
    
	DataModel *m = DataModel::getModel();
	m->_gameLayer = this;
    
    // 3. add your codes below...
	this->addWayPoint();
    this->addWaves();
    this->addHome();
    this->scheduleUpdate();
    this->schedule(schedule_selector(StartScene::gameLogic), 1.0f);
    this->currentLevel = 0;
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("start.wav");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("blast.wav");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("hit.wav");
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("start.wav");
    return true;
}

void StartScene::FollowPath(Node *sender)
{
    DataModel *m = DataModel::getModel();
    Creep* creep = (Creep *)sender;
    WayPoint* curWaypoint = creep->getCurrentWaypoint();
    WayPoint* waypoint = creep->getNextWaypoint();
    if (waypoint!=nullptr) {
        Point moveVector = waypoint->getPosition()-curWaypoint->getPosition();
        float angle = ccpToAngle(moveVector);
        float cocosAngle = CC_RADIANS_TO_DEGREES(angle);
//        std::ostringstream text("angle = ");
//        text<<std::to_string(cocosAngle)<<" curPoint("<<std::to_string(curWaypoint->getPosition().x)<<","<<std::to_string(curWaypoint->getPosition().y)<<") nextPoint("<<std::to_string(waypoint->getPosition().x)<<","<<std::to_string(waypoint->getPosition().y)<<")";
//        log(text.str().c_str());
        if (creep->inverse&&(cocosAngle==0||cocosAngle==180)) {
            sender->setRotation(cocosAngle+180);
        }else
            sender->setRotation(cocosAngle);
        int moveDuration = creep->moveDuration;
        auto actionMove = MoveTo::create(moveDuration, waypoint->getPosition());
        auto actionMoveDone = CallFuncN::create(this, callfuncN_selector(StartScene::FollowPath));
        creep->stopAllActions();
        creep->runAction(Sequence::create(actionMove, actionMoveDone, NULL));
    }else{
        m->home->homeHP--;
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("blast.wav");
        this->removeChild(creep);
        if (m->home->homeHP<=0) {
            this->removeChild(creep);
            m->home->removeAllChildren();
            m->home->sprite = Sprite::create("destroy.png");
            m->home->addChild(m->home->sprite,0);
            Size visibleSize = Director::getInstance()->getVisibleSize();
            Point origin = Director::getInstance()->getVisibleOrigin();
            auto winPic = Sprite::create("over.png");
            winPic->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
            //        winPic->setScale(2);
            this->addChild(winPic,2);
            this->cleanup();
        }
//        this->removeChild(creep,true);
    }
}

void StartScene::addHome()
{
    DataModel *m = DataModel::getModel();
    Home* home = NormalHome::home();
    WayPoint* wp = *(m->waypoints.end()-1);
    home->setPosition(wp->getPosition());
    m->home = home;
    this->addChild(home,1);
}

void StartScene::addWaves()
{
	DataModel *m = DataModel::getModel();
	Wave* wave = NULL;
//	m->waypoints.size();
	wave = Wave::create()->initWithCreep(Creep::CreepType::FastGrayCreep, 0.2, 6);
	m->waves.pushBack(wave);
	wave = NULL;
	wave = Wave::create()->initWithCreep(Creep::CreepType::StrongGreenCreep, 0.4, 8);
	m->waves.pushBack(wave);
	wave = NULL;
//	wave = Wave::create()->initWithCreep(Creep::CreepType::MonsterYellowCreep, 0.6, 5);
//	m->waves.pushBack(wave);
//	wave = NULL;
//	wave = Wave::create()->initWithCreep(Creep::CreepType::GiantBlueCreep, 1.0, 2);
//	m->waves.pushBack(wave);
//	wave = NULL;
//	wave = Wave::create()->initWithCreep(Creep::CreepType::TitanRedCreep, 1.4, 1);
//	m->waves.pushBack(wave);
//	wave = NULL;
}

void StartScene::addWayPoint()
{
	DataModel *m = DataModel::getModel();
	auto objects = this->_tileMap->objectGroupNamed("Objects");
	WayPoint *wp = NULL;
	std::string stringWithFormat = "Waypoint";
	int wayPointCounter = 0;
	ValueMap wayPoint;
	wayPoint = objects->objectNamed(stringWithFormat + std::to_string(wayPointCounter));
	while (wayPoint.begin() != wayPoint.end())
	{
		int x = wayPoint.at("x").asInt();
		int y = wayPoint.at("y").asInt();
		wp = WayPoint::create();
		wp->setPosition(ccp(x, y));
		m->waypoints.pushBack(wp);
		wayPointCounter++;
		wayPoint = objects->objectNamed(stringWithFormat + std::to_string(wayPointCounter));
	}
	wp =NULL;
}

void StartScene::addTarget()
{
	DataModel *m = DataModel::getModel();
	Wave *wave = this->getCurrentWave();
    if(wave==nullptr)
        this->cleanup();
	if (wave->totalCreeps <= 0)
	{
		getNextWave();
		return;
	}
	wave->totalCreeps--;
	Creep *target = NULL;
	
	/*
     int random = rand()%3;
     if (random == 0)
     {
     target = FastGrayCreep::creep(m->waypoints.size());
     }
     else  if(random == 1)
     {
     target = StrongGreenCreep::creep(m->waypoints.size());
     }
     else
     {
     target = MonsterYellowCreep::creep(m->waypoints.size());
     }
     */
	
    
	target = Creep::createWithType(wave->creeptype,m->waypoints.size(),true);
	WayPoint *waypoint = target->getCurrentWaypoint();
	target->setPosition(waypoint->getPosition());
	waypoint = target->getNextWaypoint();
	this->addChild(target, 1);
	int moveDuration = target->moveDuration;
	auto actionMove = CCMoveTo::create(moveDuration, waypoint->getPosition());
	auto actionMoveDone = CallFuncN::create(this, callfuncN_selector(StartScene::FollowPath));
	target->runAction(CCSequence::create(actionMove, actionMoveDone, NULL));
	target->tag = 1;  
	m->targets.pushBack(target);    
}

void StartScene::gameLogic(float dt)
{
	
	DataModel *m = DataModel::getModel();
	Wave *wave = this->getCurrentWave();
    if(wave!=nullptr){
        double lastTimeTargetAdded = wave->lastTimeTargetAdded;
        double now = wave->time;
        if (lastTimeTargetAdded == 0 || now - lastTimeTargetAdded >= wave->spawnRate)
        {
            this->addTarget();
            wave->lastTimeTargetAdded = now;
//          wave->time += dt;
        }
	}
}

Wave* StartScene::getCurrentWave()
{
    if (this->currentLevel!=-1) {
	DataModel *m = DataModel::getModel();
	Wave *wave = (Wave *)m->waves.at(this->currentLevel);
	return wave;
    }else{
        return nullptr;
    }
}

Wave* StartScene::getNextWave()
{
	DataModel *m = DataModel::getModel();
	this->currentLevel++;
	if (this->currentLevel >= m->waves.size())
	{
		this->currentLevel = -1;
        return nullptr;
	}
	Wave *wave = (Wave *)m->waves.at(this->currentLevel);
	return wave;
}

Point StartScene::tileCoordForPosition(Point position)
{
	int x = position.x / this->_tileMap->getTileSize().width;
	int y = ((this->_tileMap->getMapSize().height * this->_tileMap->getTileSize().height) - position.y) / this->_tileMap->getTileSize().height;
	return ccp(x, y);
}

bool StartScene::canBuildOnTilePosition(Point pos)
{
	Point towerLoc = this->tileCoordForPosition(pos);
	int tileGid = this->_background->getTileGIDAt(towerLoc);
	Value props = this->_tileMap->getPropertiesForGID(tileGid);
	ValueMap map = props.asValueMap();
	int type_int;
	if (map.size() == 0)
	{
		type_int = 0;
	}
	else
	{
		type_int = map.at("buildable").asInt();
	}
	if (1 == type_int)
	{
		return true;
	}
	return false;
}

void StartScene::addTower(Point pos)
{
	DataModel *m = DataModel::getModel();
	Tower *target = NULL ;
	Point towerLoc = this->tileCoordForPosition(pos);
	int tileGid = this->_background->tileGIDAt(towerLoc);
	Value props = this->_tileMap->propertiesForGID(tileGid);
	ValueMap map = props.asValueMap();
	int type_int = map.at("buildable").asInt();
	if (1 == type_int)
	{
//        std::string targetPos = "pos.x = "+std::to_string(towerLoc.x)+" pos.y = "+std::to_string(towerLoc.y);
//        log(targetPos.c_str());
        bool buildable = true;
        for (auto point = m->buildedPoints.equal_range(towerLoc.x); point.first!=point.second; ++point.first) {
            if (towerLoc.y==point.first->second) {
                buildable = false;
                break;
            }
        }
        if (buildable) {
            m->buildedPoints.insert(std::make_pair(towerLoc.x, towerLoc.y));
            target = MachineGunTower::tower();
            target->setPosition(ccp((towerLoc.x * 32) + 16, this->_tileMap->getContentSize().height - (towerLoc.y * 32) - 16));
            this->addChild(target,1);
            target->setTag(1);
            m->towers.pushBack(target);
        }
	}
	else
	{
		log("Tile Not Buildable");
	}
}

Point StartScene::boundLayerPos(Point newPos)
{
	Size winSize = CCDirector::getInstance()->getWinSize();
	Point retval = newPos;
	retval.x = MIN(retval.x, 0);
	retval.x = MAX(retval.x, _tileMap->getContentSize().width + winSize.width);
	retval.y = MIN(0, retval.y);
	retval.y = MAX(_tileMap->getContentSize().height + winSize.height, retval.y);
	return retval;
}

void StartScene::update(float dt)
{
	DataModel *m = DataModel::getModel();
	Vector<Projectile*> projectilesToDelete;
    
	for (int i = 0; i < m->projectiles.size(); i++)
	{
		Projectile* projectile = (Projectile*)(m->projectiles.at(i));
		Rect projectileRect = Rect(projectile->getPositionX() - (projectile->getContentSize().width / 2),
                                   projectile->getPositionY() - (projectile->getContentSize().height / 2),
                                   projectile->getContentSize().width,
                                   projectile->getContentSize().height);
		for (int i = 0; i < m->targets.size(); i++)
		{
			Creep* target = (Creep*)(m->targets.at(i));
			Rect targetRect = Rect(target->getPositionX() - (target->sprite->getContentSize().width / 2),
                                   target->getPositionY() - (target->sprite->getContentSize().height / 2),
                                   target->sprite->getContentSize().width,
                                   target->sprite->getContentSize().height);
			if (projectileRect.intersectsRect(targetRect))
			{
				projectilesToDelete.pushBack(projectile);
				Creep *creep = target;
				creep->curHp -= 1;
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("hit.wav");
				if (creep->curHp <= 0)
				{
//					targetsToDelete.pushBack(creep);
                    m->targets.eraseObject(creep);
                    this->removeChild(creep,true);
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("blast.wav");
				}
//                for (int i = 0; i < targetsToDelete.size(); i++)
//                {
//                    Creep *target = (Creep*)(targetsToDelete.at(i));
//                    m->targets.eraseObject(target);
//                    this->removeChild(target, true);
//                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("blast.wav");
//                }
				break;
			}
		}
	}
    
    
	
	for (int i =0; i < projectilesToDelete.size(); i++)
	{    
		Projectile *projectile = (Projectile*)(projectilesToDelete.at(i));
		m->projectiles.eraseObject(projectile);      
		this->removeChild(projectile,true);   
	}
    
    if (this->currentLevel==-1&&m->targets.size()==0) {
        //win
        Size visibleSize = Director::getInstance()->getVisibleSize();
        Point origin = Director::getInstance()->getVisibleOrigin();
        auto winPic = Sprite::create("win.jpg");
        winPic->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
//        winPic->setScale(2);
        this->addChild(winPic,2);
        this->cleanup();
    }
    
}