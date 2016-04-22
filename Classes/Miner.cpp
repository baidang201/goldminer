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
	
	Size size = Director::getInstance()->getVisibleSize();

	miner = SkeletonAnimation::createWithFile("iphonehd/miner.json", "iphonehd/miner.atlas", 0.6);
	addChild(miner);
	
	//Ìí¼Ó¹³×Ó
	auto claw = CSLoader::createNode("claw.csb");
	claw->setPosition(Vec2(-7, 52));
	addChild(claw, 2);

    return true;
}

void Miner::runAppear()
{
	miner->addAnimation(0, "walk", true, 0);
	miner->addAnimation(0, "wait", false, 2);
}

