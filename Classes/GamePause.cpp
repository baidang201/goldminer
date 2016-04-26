#include "GamePause.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

using namespace cocostudio::timeline;
using namespace CocosDenshion;

Scene* GamePause::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GamePause::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GamePause::init()
{    
    //////////////////////////////
    // 1. super init first
    if ( !LayerColor::initWithColor(Color4B(0,0,0, 100)) )
    {
        return false;
    }

	size = Director::getInstance()->getVisibleSize();

    menuSetting = CSLoader::createNode("gamepause.csb");
    addChild(menuSetting);

	menuSetting->setAnchorPoint(Vec2(0.5, 0.5));
	menuSetting->setPosition(Vec2(-200, size.y/2));
	auto show = MoveTo::create(0.5, size/2);
	menuSetting->runAction(EaseBackInOut::create( show) );

	
	btnClose = static_cast<Button*>(Helper::seekWidgetByName(static_cast<Layout*>( menuSetting), "btnClose"));
	btnClose->addTouchEventListener(CC_CALLBACK_2(GamePause::close, this));
	
	btnEffect = static_cast<Button*>(Helper::seekWidgetByName(static_cast<Layout*>(menuSetting), "btnEffect"));
	btnEffect->addTouchEventListener(CC_CALLBACK_2(GamePause::toggleEffect, this));

	btnBgMusic = static_cast<Button*>(Helper::seekWidgetByName(static_cast<Layout*>(menuSetting), "btnBgMusic"));
	btnBgMusic->addTouchEventListener(CC_CALLBACK_2(GamePause::toggleBgMusic, this));

	btnNextLevel = static_cast<Button*>(Helper::seekWidgetByName(static_cast<Layout*>(menuSetting), "btnNextLevel"));
	btnNextLevel->addTouchEventListener(CC_CALLBACK_2(GamePause::nextLevel, this));

	btnExit = static_cast<Button*>(Helper::seekWidgetByName(static_cast<Layout*>(menuSetting), "btnExit"));
	btnExit->addTouchEventListener(CC_CALLBACK_2(GamePause::exit, this));

    return true;
}

void GamePause::close(Ref * pSender, Widget::TouchEventType type)
{
	if (Widget::TouchEventType::ENDED == type)
	{
		auto leave = MoveTo::create(0.5, Vec2(size.x + 300, size.y/2));
		auto seq = Sequence::create(
			leave, 
			CallFuncN::create(CC_CALLBACK_1(GamePause::removePause, this)),
			NULL);
		menuSetting->runAction(EaseBackInOut::create( seq ));
	}
}

void GamePause::nextLevel(Ref * pSender, Widget::TouchEventType type)
{
	if (Widget::TouchEventType::ENDED == type)
	{
	}
}

void GamePause::exit(Ref * pSender, Widget::TouchEventType type)
{
	if (Widget::TouchEventType::ENDED == type)
	{
		auto leave = MoveTo::create(0.5, Vec2(size.x + 300, size.y / 2));
		auto seq = Sequence::create(
			leave,
			CallFuncN::create([=](Node* node)
		{
			_eventDispatcher->dispatchCustomEvent("exitLevel");
			removeFromParentAndCleanup(true);
		}
			),
			NULL);
		menuSetting->runAction(EaseBackInOut::create(seq));

		
	}
}

void GamePause::toggleEffect(Ref * pSender, Widget::TouchEventType type)
{
	if (Widget::TouchEventType::ENDED == type)
	{
		bool isPlayEffect = UserDefault::getInstance()->getBoolForKey("isPlayEffect", true);
		if (!isPlayEffect)
		{
			//play
			btnEffect->loadTextureNormal("sound-on-btn-0.png", Widget::TextureResType::PLIST);
			btnEffect->loadTexturePressed("sound-on-btn-0.png", Widget::TextureResType::PLIST);
		}
		else
		{
			//pause
			SimpleAudioEngine::getInstance()->stopAllEffects();

			btnEffect->loadTextureNormal("sound-off-btn-0.png", Widget::TextureResType::PLIST);
			btnEffect->loadTexturePressed("sound-off-btn-0.png", Widget::TextureResType::PLIST);
		}
		isPlayEffect = !isPlayEffect;
		UserDefault::getInstance()->setBoolForKey("isPlayEffect", isPlayEffect);
	}
}

void GamePause::toggleBgMusic(Ref * pSender, Widget::TouchEventType type)
{
	if (Widget::TouchEventType::ENDED == type)
	{
		bool isPlayBgMusic = UserDefault::getInstance()->getBoolForKey("isPlayBgMusic", true);
		if (!isPlayBgMusic)
		{
			SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/menu-bg.wav", true);
			btnBgMusic->loadTextureNormal("music-on-btn-0.png", Widget::TextureResType::PLIST);
			btnBgMusic->loadTexturePressed("music-on-btn-0.png", Widget::TextureResType::PLIST);
		}
		else
		{
			SimpleAudioEngine::getInstance()->stopBackgroundMusic();
			btnBgMusic->loadTextureNormal("music-off-btn-0.png", Widget::TextureResType::PLIST);
			btnBgMusic->loadTexturePressed("music-off-btn-0.png", Widget::TextureResType::PLIST);
		}
		isPlayBgMusic = !isPlayBgMusic;
		UserDefault::getInstance()->setBoolForKey("isPlayBgMusic", isPlayBgMusic);
	}
}

void GamePause::removePause(Ref * pSender)
{
	removeFromParentAndCleanup(true);
	_eventDispatcher->dispatchCustomEvent("goOnGame");
}
