#include "Projectile.h"   
Projectile* Projectile::projectile()
{     
	Projectile* projectile = (Projectile*)Sprite::create("Projectile.png"); 
	if (projectile != NULL)    
	{         
		return projectile; 
	} 
	else
	{  
		return NULL;  
	}
}