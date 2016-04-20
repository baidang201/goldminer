#include "GameMenu.h"
#include "GameSetting.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "MainGame.h"

USING_NS_CC;
using namespace CocosDenshion;

using namespace cocostudio::timeline;

Scene* GameMenu::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameMenu::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameMenu::init()
{    
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	bool isPlayEffect = UserDefault::getInstance()->getBoolForKey("isPlayEffect", true);
	bool isPlayBgMusic = UserDefault::getInstance()->getBoolForKey("isPlayBgMusic", true);
	
	if (isPlayBgMusic)
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/menu-bg.wav", true);
	}

	settingIsShow = false;

    auto sysMenu = CSLoader::createNode("menu.csb");
    addChild(sysMenu);

	auto actTimelineIn = CSLoader::createTimeline("menu.csb");
	actTimelineIn->gotoFrameAndPlay(0, 60, false);
	sysMenu->runAction(actTimelineIn);

	auto actTimelineLoop = CSLoader::createTimeline("menu.csb");
	actTimelineLoop->gotoFrameAndPlay(65, 205, true);
	sysMenu->runAction(actTimelineLoop);

	
	//添加设置动画
	auto btnOption = static_cast<Button*>( Helper::seekWidgetByName(static_cast<Layout*>(sysMenu), "btnOption"));
	btnOption->addTouchEventListener(CC_CALLBACK_2(GameMenu::showMenu, this));

	btnSetting = static_cast<Button*>(Helper::seekWidgetByName(static_cast<Layout*>(sysMenu), "btnSetting"));
	btnSetting->addTouchEventListener(CC_CALLBACK_2(GameMenu::showSetting, this));

	auto gameStart = static_cast<Button*>(Helper::seekWidgetByName(static_cast<Layout*>(sysMenu), "gameStart"));
	gameStart->addTouchEventListener(
		[=](Ref* pSender, Widget::TouchEventType type)
	{
		if (Widget::TouchEventType::ENDED == type)
		{

			auto mainGame = MainGame::createScene();
			Director::getInstance()->replaceScene(mainGame);
		}
	}
	);

    return true;
}

void GameMenu::showMenu(Ref * pSender, Widget::TouchEventType type)
{
	if (Widget::TouchEventType::ENDED == type)
	{
		if (settingIsShow)//隐藏设置
		{
			auto moveDown = MoveTo::create(0.2, Vec2(60, 60));
			btnSetting->runAction(EaseBackInOut::create( moveDown));
		}
		else//显示设置
		{
			auto moveUp = MoveTo::create(0.2, Vec2(60, 140));
			btnSetting->runAction(EaseBackInOut::create(moveUp));
		}
		settingIsShow = !settingIsShow;
	}
}

void GameMenu::showSetting(Ref * pSender, Widget::TouchEventType type)
{
	if (Widget::TouchEventType::ENDED == type)
	{
		auto menuSetting = GameSetting::create();
		this->addChild(menuSetting);
	}
}
