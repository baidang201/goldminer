#include "Gold.h"
#include "GameSetting.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "spine\spine-cocos2dx.h"

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
	}
	else if (type == "middlegold")
	{
		if (!initWithSpriteFrameName("gold-0-0.png"))
		{
			return false;
		}
		setScale(0.5);
		weight = 4;
	}
	else if (type == "biggold")
	{
		if (!initWithSpriteFrameName("gold-1-0.png"))
		{
			return false;
		}
		weight = 5;
	}
	else if (type == "smallstone")
	{
		if (!initWithSpriteFrameName("stone-0.png"))
		{
			return false;
		}
		setScale(0.5);
		weight = 7;
	}
	else if (type == "bigstone")
	{
		if (!initWithSpriteFrameName("stone-1.png"))
		{
			return false;
		}
		weight = 8;
	}
	else if (type == "bag")
	{
		if (!initWithSpriteFrameName("treasure-bag.png"))
		{
			return false;
		}
		weight = 1;
	}
	return true;
}

int Gold::getWeight()
{
	return weight;
}
