#include "Projectile.h"  
bool Projectile::init()
{
	if (!Sprite::init())
	{
		return false;
	}
	return true;
}

Projectile* Projectile::createWithType(BulletType bulletType)
{     
	Projectile* projectile = nullptr;
	switch (bulletType) {
        case BulletType::MachineGunBullet:
            projectile = MachineGunBullet::bullet();
            break;
        case BulletType::MissileBullet:
            projectile = MissileBullet::bullet();
            break;
        case BulletType::LaserBullet:
            projectile = LaserBullet::bullet();
            break;
        case BulletType::SlowBullet:
            projectile = SlowBullet::bullet();
            break;
        default:
            break;
    }
    return projectile;
}

Projectile* MachineGunBullet::bullet()
{
    Projectile* projectTile = Projectile::create();
    projectTile->sprite = Sprite::create("bullet1.png");
    projectTile->addChild(projectTile->sprite,0);
    projectTile->power = 1;
    return projectTile;
}

Projectile* MissileBullet::bullet()
{
    Projectile* projectTile = Projectile::create();
    projectTile->sprite = Sprite::create("bullet2.png");
    projectTile->addChild(projectTile->sprite,0);
    projectTile->power = 2;
    return projectTile;
}

Projectile* LaserBullet::bullet()
{
    Projectile* projectTile = Projectile::create();
    projectTile->sprite = Sprite::create("bullet3.png");
    projectTile->addChild(projectTile->sprite,0);
    projectTile->power = 3;
    return projectTile;
}

Projectile* SlowBullet::bullet()
{
    Projectile* projectTile = Projectile::create();
    projectTile->sprite = Sprite::create("bullet4.png");
    projectTile->addChild(projectTile->sprite,0);
    projectTile->power = 4;
    return projectTile;
}