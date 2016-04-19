#include "GameSetting.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* GameSetting::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameSetting::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameSetting::init()
{    
    //////////////////////////////
    // 1. super init first
    if ( !LayerColor::initWithColor(Color4B(0,0,0, 100)) )
    {
        return false;
    }

	size = Director::getInstance()->getVisibleSize();

    menuSetting = CSLoader::createNode("menuSetting.csb");
    addChild(menuSetting);

	menuSetting->setAnchorPoint(Vec2(0.5, 0.5));
	menuSetting->setPosition(Vec2(-200, size.y/2));
	auto show = MoveTo::create(0.5, size/2);
	menuSetting->runAction(EaseBackInOut::create( show) );

	
	btnClose = static_cast<Button*>(Helper::seekWidgetByName(static_cast<Layout*>( menuSetting), "btnClose"));
	btnClose->addTouchEventListener(CC_CALLBACK_2(GameSetting::close, this));
	
	btnEffect = static_cast<Button*>(Helper::seekWidgetByName(static_cast<Layout*>(menuSetting), "btnEffect"));
	btnEffect->addTouchEventListener(CC_CALLBACK_2(GameSetting::toggleEffect, this));

	btnBgMusic = static_cast<Button*>(Helper::seekWidgetByName(static_cast<Layout*>(menuSetting), "btnBgMusic"));
	btnBgMusic->addTouchEventListener(CC_CALLBACK_2(GameSetting::toggleBgMusic, this));

    return true;
}

void GameSetting::close(Ref * pSender, Widget::TouchEventType type)
{
	if (Widget::TouchEventType::ENDED == type)
	{
		auto leave = MoveTo::create(0.5, Vec2(size.x + 300, size.y/2));
		auto seq = Sequence::create(
			leave, 
			CallFuncN::create(CC_CALLBACK_1(GameSetting::removeSetting, this)),
			NULL);
		menuSetting->runAction(EaseBackInOut::create( seq ));
	}
}

void GameSetting::toggleEffect(Ref * pSender, Widget::TouchEventType type)
{
	if (Widget::TouchEventType::ENDED == type)
	{

	}
}

void GameSetting::toggleBgMusic(Ref * pSender, Widget::TouchEventType type)
{
	if (Widget::TouchEventType::ENDED == type)
	{

	}
}

void GameSetting::removeSetting(Ref * pSender)
{
	removeFromParentAndCleanup(true);
}
