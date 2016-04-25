#include "Gold.h"
#include "GameSetting.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "spine\spine-cocos2dx.h"
#include <stdlib.h>

USING_NS_CC;

Gold * Gold::create(std::string type)
{
	Gold* gold = new Gold();
	if (gold && gold->init(type))
	{
		gold->autorelease();
		return gold;
	}
	delete gold;
	gold = nullptr;
	return nullptr;
}

bool Gold::init(std::string type)
{
	if (type == "smallgold")
	{
		if (!initWithSpriteFrameName("gold-0-0.png"))
		{
			return false;
		}
		setScale(0.3);
		weight = 3;
		value = 50;
	}
	else if (type == "middlegold")
	{
		if (!initWithSpriteFrameName("gold-0-0.png"))
		{
			return false;
		}
		setScale(0.5);
		weight = 4;
		value = 250;
	}
	else if (type == "biggold")
	{
		if (!initWithSpriteFrameName("gold-1-0.png"))
		{
			return false;
		}
		weight = 5;
		value = 500;
	}
	else if (type == "smallstone")
	{
		if (!initWithSpriteFrameName("stone-0.png"))
		{
			return false;
		}
		setScale(0.5);
		weight = 7;
		value = 11;
	}
	else if (type == "bigstone")
	{
		if (!initWithSpriteFrameName("stone-1.png"))
		{
			return false;
		}
		weight = 8;
		value = 20;
	}
	else if (type == "bag")
	{
		if (!initWithSpriteFrameName("treasure-bag.png"))
		{
			return false;
		}
		weight = 1;
		value = rand();
	}
	return true;
}

int Gold::getWeight()
{
	return weight;
}

int Gold::getValue()
{
	return value;
}
