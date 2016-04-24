#include "Miner.h"
#include "GameSetting.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "spine\spine-cocos2dx.h"


USING_NS_CC;
using namespace spine;

Scene* Miner::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Miner::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Miner::init()
{    
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	
	_isRopeChanging = false;
	ropeHeight = 20;
	_gold = nullptr;

	Size size = Director::getInstance()->getVisibleSize();

	miner = SkeletonAnimation::createWithFile("iphonehd/miner.json", "iphonehd/miner.atlas", 0.6);
	addChild(miner);
	
	//添加钩子
	auto claw = CSLoader::createNode("claw.csb");
	claw->setPosition(Vec2(0, -35));
	addChild(claw, 2);
		
	rope = static_cast<ImageView*>( Helper::seekWidgetByName(static_cast<Layout*>(claw), "rope") );
	clawAxis = static_cast<ImageView*>(Helper::seekWidgetByName(static_cast<Layout*>(claw), "clawAxis"));

	PhysicsBody* body = PhysicsBody::createCircle(6);
	body->setCategoryBitmask(10);
	body->setCollisionBitmask(10);
	body->setContactTestBitmask(10);
	clawAxis->setPhysicsBody(body);

	clawLeft = static_cast<ImageView*>(Helper::seekWidgetByName(static_cast<Widget*>(clawAxis), "clawLeft"));
	clawRight = static_cast<ImageView*>(Helper::seekWidgetByName(static_cast<Widget*>(clawAxis), "clawRight"));

    return true;
}

void Miner::runAppear()
{
	miner->addAnimation(0, "walk", true, 0);
	miner->addAnimation(0, "wait", false, 2);
}

void Miner::runShakeClaw()
{
	rope->setRotation(0);
	auto seq = Sequence::create(
		RotateBy::create(1.2, -80),
		RotateBy::create(2.4, 160),
		RotateBy::create(1.2, -80),
		NULL
	);
	rope->runAction(RepeatForever::create(seq));
}

void Miner::stopShakeActions()
{
	rope->stopAllActions();
}

bool Miner::isRopeChanging()
{
	return _isRopeChanging;
}

void Miner::runRopeThrow()
{
	_isRopeChanging = true;
	unschedule(schedule_selector(Miner::reduceRopeHeight));
	schedule(schedule_selector(Miner::addRopeHeight), 0.025);
	//miner->addAnimation(0, "miner-throw", false, 0); //暂时未实现骨骼动画
}

void Miner::runRopePull()
{
	unschedule(schedule_selector(Miner::addRopeHeight));
	_isRopeChanging = true;
	clawAxis->setPhysicsBody(NULL);
	schedule(schedule_selector(Miner::reduceRopeHeight), 0.025);
	
	if (_gold && _gold->getWeight()>=6)
	{
		//miner->addAnimation(0, "miner-pull-heavy", true, 0); //暂时未实现骨骼动画   根据重量加载动画
	}
	else if (_gold )
	{
		//miner->addAnimation(0, "miner-pull-light", true, 0); //暂时未实现骨骼动画   根据重量加载动画
	}
}

void Miner::runClawClose()
{
	clawLeft->runAction( RotateBy::create(0.3, -15));
	clawRight->runAction(RotateBy::create(0.3, 15));
}

void Miner::runClawOpen()
{
	clawLeft->runAction(RotateBy::create(0.03, 15));
	clawRight->runAction(RotateBy::create(0.03, -15));
}

void Miner::addGold(std::string type)
{
	_gold = Gold::create(type);
	clawAxis->addChild(_gold);

	if (type == "smallGold")
	{
		_gold->setPosition(Vec2(6, -15));
	}
	else if (type == "middleGold")
	{
		_gold->setPosition(Vec2(6, -25));
	}
	else if (type == "bigGold")
	{
		_gold->setPosition(Vec2(6, -46));
	}
	else
	{
		_gold->setAnchorPoint(Vec2(0.5, 1));
		_gold->setPosition(Vec2(6,0));
	}
}

void Miner::addRopeHeight(float delate)
{
	ropeHeight += 10;
	rope->setSize(Size(3, ropeHeight));

	PhysicsBody* body = PhysicsBody::createCircle(6);
	body->setCategoryBitmask(10);
	body->setCollisionBitmask(10);
	body->setContactTestBitmask(10);
	clawAxis->setPhysicsBody(body);
}

void Miner::reduceRopeHeight(float delate)
{
	if (_gold)
	{
		ropeHeight -=( 10 - _gold->getWeight());
	}
	else
	{
		ropeHeight -= 10;
	}

	if (ropeHeight <= 20)
	{
		unschedule(schedule_selector(Miner::reduceRopeHeight));
		ropeHeight = 20;
		rope->setSize(Size(3, ropeHeight));
		_isRopeChanging = false;
		
		if (_gold)
		{
			_gold->removeFromParent();
			_gold = nullptr;
		}
		miner->addAnimation(0, "miner-pull-wait", false, 0);  
		runClawOpen();
		runShakeClaw();
	}


	rope->setSize(Size(3, ropeHeight));
}

