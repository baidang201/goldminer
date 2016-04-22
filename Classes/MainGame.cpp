#include "MainGame.h"
#include "GameSetting.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;
using namespace cocostudio::timeline;

Scene* MainGame::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainGame::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainGame::init()
{    
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	bool isPlayBgMusic = UserDefault::getInstance()->getBoolForKey("isPlayBgMusic", true);
	if (isPlayBgMusic)
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/menu-bg.wav", true);
	}

	Size size = Director::getInstance()->getVisibleSize();

	auto level = CSLoader::createNode("level1.csb");
	addChild(level);

	Vector<Node*> vectorGold= Helper::seekWidgetByName(static_cast<Layout*>( level), "panelGold")->getChildren();

	auto levelTop = CSLoader::createNode("leveltop.csb");
	addChild(levelTop);
		
	aniTimeDowm = CSLoader::createTimeline("leveltop.csb");
	aniTimeDowm->gotoFrameAndPause(0);
	levelTop->runAction(aniTimeDowm);

	//初始化当前关卡，剩余时间  目标分数，当前金币.       
	int icurLevel = UserDefault::getInstance()->getIntegerForKey("curLevel");
	int icurCoin = UserDefault::getInstance()->getIntegerForKey("curCoin");
	igoalCoin = 650 + 135 * (icurLevel - 1) * (icurLevel - 1) + 410 * (icurLevel - 1);

	goalCoin = static_cast<Text*> (Helper::seekWidgetByName( static_cast<Layout*>(levelTop), "goalCoin"));
	curCoin = static_cast<Text*> (Helper::seekWidgetByName(static_cast<Layout*>(levelTop), "curCoin"));
	curLevel = static_cast<Text*> (Helper::seekWidgetByName(static_cast<Layout*>(levelTop), "curLevel"));
	timeDown = static_cast<Text*> (Helper::seekWidgetByName(static_cast<Layout*>(levelTop), "timeDown"));

	goalCoin->setText(String::createWithFormat("%d", igoalCoin)->getCString());
	curCoin->setText(String::createWithFormat("%d", icurCoin)->getCString());
	curLevel->setText(String::createWithFormat("%d", icurLevel)->getCString());
	timeDown->setText(String::createWithFormat("%d", 60)->getCString());


	btnPause = static_cast<Button*> (Helper::seekWidgetByName(static_cast<Layout*>(levelTop), "timeDown"));

	//黄金矿工
	miner = Miner::create();
	addChild(miner);
	miner->setPosition(size.width + 100, size.height - 110);

	//增加游戏关卡提示 
	auto starttips = CSLoader::createNode("starttips.csb");
	addChild(starttips);

	Text* levelNum = static_cast<Text*> (Helper::seekWidgetByName(static_cast<Layout*>(starttips), "levelNum"));
	levelNum->setText(String::createWithFormat("%d", icurLevel)->getCString());

	Text* goalNum = static_cast<Text*> (Helper::seekWidgetByName(static_cast<Layout*>(starttips), "goalNum"));
	goalNum->setText(String::createWithFormat("%d", igoalCoin)->getCString());
	
	Text* levelNode = static_cast<Text*> (Helper::seekWidgetByName(static_cast<Layout*>(starttips), "levelNode"));
	Text* goalNode = static_cast<Text*> (Helper::seekWidgetByName(static_cast<Layout*>(starttips), "goalNode"));
	auto goalSymbol = Helper::seekWidgetByName(static_cast<Layout*>(starttips), "goalSymbol");

	auto aniLevelNode = Sequence::create(
		EaseBackInOut::create( MoveTo::create(1, Vec2(Director::getInstance()->getVisibleSize().width/ 2 - 10, 430))),
		DelayTime::create(1),
		EaseBackInOut::create(MoveTo::create(1, Vec2(Director::getInstance()->getVisibleSize().width / 2 + 20, 430))),
		FadeOut::create(0.1),
		NULL);
	levelNode->runAction(aniLevelNode);

	auto anigoalSymbol = Sequence::create(
		EaseBackInOut::create(MoveTo::create(1, Vec2(Director::getInstance()->getVisibleSize().width / 2 - 150, 250))),
		DelayTime::create(1),
		EaseBackInOut::create(MoveTo::create(1, Vec2(Director::getInstance()->getVisibleSize().width / 2 - 150, 80))),
		FadeOut::create(0.1),
		NULL);
	goalSymbol->runAction(anigoalSymbol);

	auto anigoalNode = Sequence::create(
		EaseBackInOut::create(MoveTo::create(1, Vec2(Director::getInstance()->getVisibleSize().width / 2 + 10, 250))),
		DelayTime::create(1),
		EaseBackInOut::create(MoveTo::create(1, Vec2(Director::getInstance()->getVisibleSize().width / 2 + 10, 80))),
		FadeOut::create(0.1),
		CCCallFuncN::create([=](Node* node) 
		{
			starttips->removeFromParentAndCleanup(true);
			
			//黄金矿工初始化
			miner->runAppear();
			miner->runAction(
				Sequence::create(
					MoveTo::create(2, Vec2(size.width / 2, size.height - 110)),
					CallFuncN::create(
						[=](Ref* pSender) 
						{
							aniTimeDowm->gotoFrameAndPlay(0, 60, false);
						}
					),
					NULL));
		}),
		NULL);
	goalNode->runAction(anigoalNode);





    return true;
}

