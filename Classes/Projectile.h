#pragma once 
#include "cocos2d.h"  
USING_NS_CC; 
class Projectile: public Sprite 
{ 
public:
    enum class BulletType
    {
        MachineGunBullet,
        MissileBullet,
        LaserBullet,
        SlowBullet
    };
	static Projectile* createWithType(BulletType bulletType);
    int power;
    Sprite* sprite;
    virtual bool init();
    CREATE_FUNC(Projectile);
};

class MachineGunBullet: public Projectile
{
public:
    static Projectile* bullet();
};

class MissileBullet: public Projectile
{
public:
    static Projectile* bullet();
};

class LaserBullet: public Projectile
{
public:
    static Projectile* bullet();
};

class SlowBullet: public Projectile
{
public:
    static Projectile* bullet();
};