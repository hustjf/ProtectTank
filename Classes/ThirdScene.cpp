//
//  ThirdScene.cpp
//  MyGame
//
//  Created by Alex on 14-6-19.
//
//

#include "ThirdScene.h"
#include "DataModel.h"
template<typename T>
std::string to_string(const T& t) {
    std::ostringstream os;
	os << t;
	return os.str();
}

Scene* ThirdScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    
    return scene;
}

bool ThirdScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    DataModel *m = DataModel::getModel();
    auto myGameHUD = GameHUD::shareHUD();
	m->_gameHUDLayer = myGameHUD;
	this->addChild(myGameHUD, 1);
    std::string file = "03.tmx";
	auto str = String::createWithContentsOfFile(FileUtils::getInstance()-> fullPathForFilename(file.c_str()).c_str());
	this->_tileMap =TMXTiledMap::createWithXML(str->getCString(),"");
	this->_background = _tileMap->layerNamed("bg");
    //    _tileMap->setScaleX(1.2);
	_tileMap->setPosition(Point(0, 0));
	addChild(_tileMap, -1);
    
	m->_gameLayer = this;
    m->currentMoney = 500;
    // 3. add your codes below...
    widget = dynamic_cast<Layout*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("PlayUi_1.json"));
    widget->setPosition(Point(0,0));
    this->addChild(widget,0);
    //    for (auto& child : widget->getChildren())
    //    {
    //        if (child)
    //        {
    //            Widget* widgetChild = dynamic_cast<Widget*>(child);
    //            if (widgetChild)
    //            {
    //                log(widgetChild->getName());
    //            }
    //        }
    //    }
	this->addWayPoint();
    this->addWaves();
    this->addHome();
    this->scheduleUpdate();
    this->schedule(schedule_selector(ThirdScene::gameLogic), 2.0f);
    this->currentLevel = 0;
    money = dynamic_cast<Text*>(widget->getChildByName("money"));
    life = dynamic_cast<Text*>(widget->getChildByName("life"));
    currentWave = dynamic_cast<Text*>(widget->getChildByName("currentWave"));
    totalWave = dynamic_cast<Text*>(widget->getChildByName("totalWave"));
    money->setText(to_string(m->currentMoney));
    life->setText(to_string(m->home->homeHP));
    currentWave->setText(to_string(currentLevel+1));
    totalWave->setText(to_string(m->waves.size()));
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("start.wav");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("blast.wav");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("hit.wav");
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("start.wav");
    return true;
}

void ThirdScene::addTarget()
{
	DataModel *m = DataModel::getModel();
	Wave *wave = this->getCurrentWave();
    if(wave==nullptr)
        this->cleanup();
	if (wave->totalCreeps <= 0)
	{
        if (m->targets.size()==0)
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
	
    
	target = Creep::createWithType(wave->creeptype,m->waypoints.size(),false);
	WayPoint *waypoint = target->getCurrentWaypoint();
	target->setPosition(waypoint->getPosition());
	waypoint = target->getNextWaypoint();
    auto lifeBar = CCProgressTimer::create(CCSprite::create("loginbg.png"));
    lifeBar->setType(kCCProgressTimerTypeBar);
    lifeBar->setPercentage(100.0f);
    lifeBar->setBarChangeRate(ccp(1, 0));//水平方向
    lifeBar->setMidpoint(ccp(0,0));//设置起始点为左下方
    lifeBar->setPosition(target->sprite->getPosition().x, target->sprite->getPosition().y+60);
    lifeBar->setScaleY(0.6);
    lifeBar->setScaleX(0.3);
    target->addChild(lifeBar,1);
    target->lifeBar = lifeBar;
    auto emptyLifeBar = Sprite::create("login_dbg.png");
    emptyLifeBar->setScaleX(0.3);
    emptyLifeBar->setScaleY(0.3);
    emptyLifeBar->setPosition(lifeBar->getPosition());
    target->addChild(emptyLifeBar,0);
    target->emptyLifeBar = emptyLifeBar;
    this->addChild(target, 1);
	int moveDuration = target->moveDuration;
	auto actionMove = CCMoveTo::create(moveDuration, waypoint->getPosition());
    auto lifeBarActionMove = MoveTo::create(moveDuration, ccp(waypoint->getPosition().x,waypoint->getPosition().y+25));
	auto actionMoveDone = CallFuncN::create(this, callfuncN_selector(StartScene::FollowPath));
	target->runAction(CCSequence::create(actionMove, actionMoveDone, NULL));
    //    lifeBar->runAction(CCSequence::create(lifeBarActionMove, NULL));
	target->tag = 1;
	m->targets.pushBack(target);
}

void ThirdScene::gameLogic(float dt)
{
	
	DataModel *m = DataModel::getModel();
	Wave *wave = this->getCurrentWave();
    if(wave!=nullptr){
        double lastTimeTargetAdded = wave->lastTimeTargetAdded;
        double now = wave->time;
        if (lastTimeTargetAdded == 0 || now - lastTimeTargetAdded >= wave->spawnRate)
        {
            addTarget();
            wave->lastTimeTargetAdded = now;
            //          wave->time += dt;
        }
	}
}

void ThirdScene::FollowPath(Node *sender)
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
        if (cocosAngle==0) {
            if (creep->inverse){
                cocosAngle += 180;
                creep->lifeBar->setPosition(creep->sprite->getPosition().x, creep->sprite->getPosition().y-60);
            }else
                creep->lifeBar->setPosition(creep->sprite->getPosition().x, creep->sprite->getPosition().y+60);
            creep->emptyLifeBar->setPosition(creep->lifeBar->getPosition());
        }else if(cocosAngle==180){
            if (creep->inverse) {
                cocosAngle += 180;
                creep->lifeBar->setPosition(creep->sprite->getPosition().x, creep->sprite->getPosition().y+60);
            }else
                creep->lifeBar->setPosition(creep->sprite->getPosition().x, creep->sprite->getPosition().y-60);
            creep->emptyLifeBar->setPosition(creep->lifeBar->getPosition());
        }else{
            if (!creep->inverse)
                cocosAngle += 180;
        }
        sender->setRotation(cocosAngle);
        int moveDuration = creep->moveDuration;
        auto actionMove = MoveTo::create(moveDuration, waypoint->getPosition());
        auto actionMoveDone = CallFuncN::create(this, callfuncN_selector(StartScene::FollowPath));
        creep->stopAllActions();
        //        creep->lifeBar->stopAllActions();
        creep->runAction(Sequence::create(actionMove, actionMoveDone, NULL));
        //        creep->lifeBar->runAction(Sequence::create(lifeBarActionMove, NULL));
    }else{
        m->home->homeHP--;
        life->setText(to_string(m->home->homeHP));
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("blast.wav");
        m->targets.eraseObject(creep);
        this->removeChild(creep);
        CCParticleSystem* particleSystem = CCParticleExplosion::create();
        particleSystem->setTexture(CCTextureCache::sharedTextureCache()->addImage("bullet2.png"));
        particleSystem->setPosition(creep->getPosition());
        particleSystem->setScale(0.15);
        particleSystem->setLife(1);
        this->addChild(particleSystem);
        if (m->home->homeHP<=0) {
            m->home->removeAllChildren();
            m->home->sprite = Sprite::create("destroy.png");
            m->home->sprite->setScale(0.7);
            m->home->addChild(m->home->sprite,0);
            Size visibleSize = Director::getInstance()->getVisibleSize();
            Point origin = Director::getInstance()->getVisibleOrigin();
            auto losePic = Sprite::create("over.png");
            losePic->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
            this->addChild(losePic,3);
            widget = dynamic_cast<Layout*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("OverUi_1/LoseUi_1.json"));
            widget->setPosition(Point(visibleSize.width/2 + origin.x -widget->getContentSize().width/2, visibleSize.height/2 + origin.y - widget->getContentSize().height/2));
            this->addChild(widget,2);
            auto restartButton = dynamic_cast<Button*>(widget->getChildByName("RestartBTN"));
            auto mainMenuButton = dynamic_cast<Button*>(widget->getChildByName("MainMenuBTN"));
            restartButton->addTouchEventListener(this, toucheventselector(ThirdScene::restartEvent));
            mainMenuButton->addTouchEventListener(this, toucheventselector(ThirdScene::mainMenuEvent));
            this->cleanup();
        }else{
            std::string home = to_string(m->home->homeHP) + ".png";
            m->home->removeAllChildren();
            m->home->sprite = Sprite::create(home);
            m->home->sprite->setScale(0.7);
            m->home->addChild(m->home->sprite,0);
        }
    }
}

void ThirdScene::update(float dt)
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
				creep->curHp -= projectile->power;
                creep->lifeBar->setPercentage(static_cast<float>(creep->curHp*100)/creep->maxHp);
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("hit.wav");
				if (creep->curHp <= 0)
				{
                    //					targetsToDelete.pushBack(creep);
                    m->targets.eraseObject(creep);
                    CCParticleSystem* particleSystem = CCParticleExplosion::create();
                    particleSystem->setTexture(CCTextureCache::sharedTextureCache()->addImage("bullet2.png"));
                    particleSystem->setPosition(creep->getPosition());
                    particleSystem->setScale(0.15);
                    particleSystem->setLife(1);
                    this->addChild(particleSystem);
                    //                    creep->removeAllChildren();
                    //                    creep->sprite = Sprite::create("blast7.png");
                    //                    creep->addChild(creep->sprite,0);
                    this->removeChild(creep,true);
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("blast.wav");
                    m->currentMoney += creep->bonusMoney;
                    money->setText(to_string(m->currentMoney));
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
        //        auto winPic = Sprite::create("win.png");
        //        winPic->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
        ////        winPic->setScale(2);
        //        this->addChild(winPic,2);
        widget = dynamic_cast<Layout*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("OverUi_1/WinUi_1.json"));
        widget->setPosition(Point(visibleSize.width/2 + origin.x -widget->getContentSize().width/2, visibleSize.height/2 + origin.y - widget->getContentSize().height/2));
        this->addChild(widget,2);
        auto star1 = dynamic_cast<ImageView*>(widget->getChildByName("Star_1"));
        auto star2 = dynamic_cast<ImageView*>(widget->getChildByName("Star_2"));
        auto star3 = dynamic_cast<ImageView*>(widget->getChildByName("Star_3"));
        auto restartButton = dynamic_cast<Button*>(widget->getChildByName("RestartBTN"));
        auto continueButton = dynamic_cast<Button*>(widget->getChildByName("ContinueBTN"));
        restartButton->addTouchEventListener(this, toucheventselector(ThirdScene::restartEvent));
        continueButton->addTouchEventListener(this, toucheventselector(ThirdScene::continueEvent));
        if (m->home->homeHP>=1&& m->home->homeHP<=3) {
            star2->setVisible(false);
            star3->setVisible(false);
        }else if (m->home->homeHP>=4&&m->home->homeHP<=9){
            star3->setVisible(false);
        }
        this->cleanup();
    }
    
}

void ThirdScene::restartEvent(Widget* target, TouchEventType type)
{
    if(type==TouchEventType::TOUCH_EVENT_ENDED){
        DataModel *m = DataModel::getModel();
        m->clear();
		auto replaceScene = TransitionFadeBL::create(2, createScene());
		Director::getInstance()->replaceScene(replaceScene);
	}
}

void ThirdScene::continueEvent(Widget* target, TouchEventType type)
{
    if(type==TouchEventType::TOUCH_EVENT_ENDED){
        DataModel *m = DataModel::getModel();
        m->clear();
		auto replaceScene = TransitionFadeBL::create(2, ThirdScene::createScene());
        
		Director::getInstance()->replaceScene(replaceScene);
	}
}
