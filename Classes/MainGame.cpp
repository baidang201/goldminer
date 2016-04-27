#include "MainGame.h"
#include "GameSetting.h"
#include "GamePause.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "GameMenu.h"

USING_NS_CC;
using namespace CocosDenshion;
using namespace cocostudio::timeline;

#define WORLDTAG 100
#define LEFTTIME 60

MainGame::~MainGame()
{
	//�Ƴ��Զ��������
	_eventDispatcher->removeCustomEventListeners("pullcomplete");
	_eventDispatcher->removeCustomEventListeners("goOnGame");
	_eventDispatcher->removeCustomEventListeners("exitLevel");
}

Scene* MainGame::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();//������������    
	PhysicsWorld* word = scene->getPhysicsWorld();
	word->setGravity(Vec2::ZERO);//���ö�ά������������Ϊ0��0
	word->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    // 'layer' is an autorelease object
    auto layer = MainGame::create();

	Size size = Director::getInstance()->getWinSize();
	PhysicsBody* body = PhysicsBody::createEdgeBox(size);//�����߽�
	//������ײ����
	body->setCategoryBitmask(10);
	body->setCollisionBitmask(10);
	body->setContactTestBitmask(10);

	Node* node = Node::create();
	node->setPosition(size/2);
	node->setPhysicsBody(body);//���������body��cocos�����node��
	node->setTag(WORLDTAG);
	scene->addChild(node);


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

	leftTime = LEFTTIME;

	bool isPlayBgMusic = UserDefault::getInstance()->getBoolForKey("isPlayBgMusic", true);
	if (isPlayBgMusic)
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/menu-bg.wav", true);
	}

	Size size = Director::getInstance()->getVisibleSize();

	icurLevel = UserDefault::getInstance()->getIntegerForKey("curLevel");
	String* strLevel = String::createWithFormat("level%d.csb", icurLevel);
	auto level = CSLoader::createNode( strLevel->getCString() );
	addChild(level);

	Vector<Node*> vectorGold= Helper::seekWidgetByName(static_cast<Layout*>( level), "panelGold")->getChildren();
	for (int i = 0; i < vectorGold.size(); i++)
	{
		auto gold = vectorGold.at(i);
		Size goldSize = gold->getContentSize();

		std::string name = gold->getName();
		if (name == "smallgold")
		{
			goldSize = goldSize*0.3;
		}
		else if (name == "middlegold")
		{
			goldSize = goldSize*0.5;
		}

		auto body = PhysicsBody::createBox(goldSize);
		body->setCategoryBitmask(10);
		body->setCollisionBitmask(10);
		body->setContactTestBitmask(10);
		gold->setPhysicsBody(body);

	}
	
	auto levelTop = CSLoader::createNode("leveltop.csb");
	addChild(levelTop);
		
	aniTimeDowm = CSLoader::createTimeline("leveltop.csb");
	aniTimeDowm->gotoFrameAndPause(0);
	levelTop->runAction(aniTimeDowm);

	//��ʼ����ǰ�ؿ���ʣ��ʱ��  Ŀ���������ǰ���. 
	icurCoin = UserDefault::getInstance()->getIntegerForKey("curCoin");
	igoalCoin = 650 + 135 * (icurLevel - 1) * (icurLevel - 1) + 410 * (icurLevel - 1);

	Image_1 = static_cast<ImageView*> (Helper::seekWidgetByName(static_cast<Layout*>(levelTop), "Image_1"));
	goalCoin = static_cast<Text*> (Helper::seekWidgetByName( static_cast<Layout*>(levelTop), "goalCoin"));
	curCoin = static_cast<Text*> (Helper::seekWidgetByName(static_cast<Layout*>(levelTop), "curCoin"));
	curLevel = static_cast<Text*> (Helper::seekWidgetByName(static_cast<Layout*>(levelTop), "curLevel"));
	timeDown = static_cast<Text*> (Helper::seekWidgetByName(static_cast<Layout*>(levelTop), "timeDown"));

	goalCoin->setText(String::createWithFormat("%d", igoalCoin)->getCString());
	curCoin->setText(String::createWithFormat("%d", icurCoin)->getCString());
	curLevel->setText(String::createWithFormat("%d", icurLevel)->getCString());
	timeDown->setText(String::createWithFormat("%d", 60)->getCString());

	btnPause = static_cast<Button*> (Helper::seekWidgetByName(static_cast<Layout*>(levelTop), "btnPause"));

	//�ƽ��
	miner = Miner::create();	
	addChild(miner, 20);
	miner->setPosition(size.width + 100, size.height - 110);

	//������Ϸ�ؿ���ʾ 
	auto starttips = CSLoader::createNode("starttips.csb");
	addChild(starttips);

	Text* levelNum = static_cast<Text*> (Helper::seekWidgetByName(static_cast<Layout*>(starttips), "levelNum"));
	levelNum->setText(String::createWithFormat("%d", icurLevel)->getCString());

	Text* goalNum = static_cast<Text*> (Helper::seekWidgetByName(static_cast<Layout*>(starttips), "goalNum"));
	goalNum->setText(String::createWithFormat("%d", igoalCoin)->getCString());
	
	Text* levelNode = static_cast<Text*> (Helper::seekWidgetByName(static_cast<Layout*>(starttips), "levelNode"));
	Text* goalNode = static_cast<Text*> (Helper::seekWidgetByName(static_cast<Layout*>(starttips), "goalNode"));
	auto goalSymbol = Helper::seekWidgetByName(static_cast<Layout*>(starttips), "goalSymbol");

	//�ȼ���ʾ�ı��볡
	auto aniLevelNode = Sequence::create(
		EaseBackInOut::create( MoveTo::create(1, Vec2(Director::getInstance()->getVisibleSize().width/ 2 - 10, 430))),
		DelayTime::create(1),
		EaseBackInOut::create(MoveTo::create(1, Vec2(Director::getInstance()->getVisibleSize().width / 2 + 20, 430))),
		FadeOut::create(0.1),
		NULL);
	levelNode->runAction(aniLevelNode);

	//�÷���ʾͼƬ�볡
	auto anigoalSymbol = Sequence::create(
		EaseBackInOut::create(MoveTo::create(1, Vec2(Director::getInstance()->getVisibleSize().width / 2 - 150, 250))),
		DelayTime::create(1),
		EaseBackInOut::create(MoveTo::create(1, Vec2(Director::getInstance()->getVisibleSize().width / 2 - 150, 80))),
		FadeOut::create(0.1),
		NULL);
	goalSymbol->runAction(anigoalSymbol);

	//�÷���ʾ�ı��볡
	auto anigoalNode = Sequence::create(
		EaseBackInOut::create(MoveTo::create(1, Vec2(Director::getInstance()->getVisibleSize().width / 2 + 10, 250))),
		DelayTime::create(1),
		EaseBackInOut::create(MoveTo::create(1, Vec2(Director::getInstance()->getVisibleSize().width / 2 + 10, 80))),
		FadeOut::create(0.1),
		CCCallFuncN::create([=](Node* node) 
		{
			starttips->removeFromParentAndCleanup(true);
			
			//�ƽ�󹤳�ʼ��
			miner->runAppear();
			miner->runAction(
				Sequence::create(
					MoveTo::create(2, Vec2(size.width / 2, size.height - 110)),
					CallFuncN::create(
						[=](Ref* pSender) 
						{
							aniTimeDowm->gotoFrameAndPlay(0, 60, false);//ʱ��ؼ��볡
							schedule(schedule_selector(MainGame::timeDownCount), 1, LEFTTIME - 1, 0);


							miner->runShakeClaw();//�������Ұڶ�

							//��ӷŹ�����Ļ�����¼�
							auto listener = EventListenerTouchOneByOne::create();
							listener->onTouchBegan = [=](Touch* touch, Event* event) 
							{
								if (!miner->isRopeChanging())//���쳤��ʱ�򣬽����쳤
								{
									miner->stopShakeActions();
									miner->runRopeThrow();									
								}

								return true;
							};

							_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

							//������ײ�¼����� �󹤽���󣬲Ž�����ײ������Ҫ��Ȼ�ٽ����볡���������У����ڱ�Եʱ�ᴥ����ײ
							auto physicListener = EventListenerPhysicsContact::create();
							physicListener->onContactBegin = [=](PhysicsContact& contact)
							{
								auto shapa = contact.getShapeA()->getBody()->getNode();
								auto shapb = contact.getShapeB()->getBody()->getNode();

								if (shapb->getTag() != WORLDTAG)//ץ�����ǽ�� ʯͷ
								{
									miner->runClawClose();
									miner->addGold(shapb->getName());
									shapb->removeFromParent();
								}

								//�����Ӷ���	
								miner->runRopePull();
								return true;
							};
							_eventDispatcher->addEventListenerWithSceneGraphPriority(physicListener, this);

							//��������ӽ������Զ���ص�
							_eventDispatcher->addCustomEventListener("pullcomplete", [=](EventCustom* evn)
							{
								String* strValue = (String*)(evn->getUserData());

								int addGold = strValue->intValue();
								icurCoin += addGold;								

								auto lbAddGold = LabelTTF::create();
								lbAddGold->setString(String::createWithFormat("%d", icurCoin)->getCString());
								lbAddGold->setFontSize(20);
								lbAddGold->setColor(ccc3(250,0,0));
								lbAddGold->setPosition( miner->rope->convertToWorldSpace( miner->clawAxis->getPosition() ));
								addChild(lbAddGold);


								auto actionSpawn = Spawn::create(
									MoveTo::create(1, Image_1->convertToWorldSpace( curCoin->getPosition())),//todo(liyh) ��������
									Sequence::create(ScaleTo::create(0.5, 3), ScaleTo::create(0.5, 0.1), nullptr),
									nullptr);

								auto seq = Sequence::create(actionSpawn,
									CallFuncN::create(
										[=](Node*)
									{
										lbAddGold->removeFromParent();
										curCoin->setString(String::createWithFormat("%d", icurCoin)->getCString());
									}
									), 
									nullptr);
								
								lbAddGold->runAction(seq);
							}
							);
						}
					),
					NULL));
		}),
		NULL);
	goalNode->runAction(anigoalNode);
	

	//��Ϸ��ͣ����
	btnPause->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::ENDED)
		{
			onExit();
			auto* gamePause = GamePause::create();

			//������һ�ذ�ť״̬
			if (icurCoin < igoalCoin)
			{
				gamePause->setNextLevelDisable();
			}

			Director::getInstance()->getRunningScene()->addChild(gamePause);
		}
	});
	
	//�ָ�
	_eventDispatcher->addCustomEventListener("goOnGame", [=](EventCustom* evn)
	{
		onEnter();
	});

	//�˳�
	_eventDispatcher->addCustomEventListener("exitLevel", [=](EventCustom* evn)
	{
		onEnter();
		exitLevel();
	});

    return true;
}

void MainGame::timeDownCount(float dt)
{
	leftTime--;
	timeDown->setString(String::createWithFormat("%d", leftTime)->getCString());

	if (leftTime <= 0)
	{
		//��Ϸ����
		gameResult();
	}
}

void MainGame::exitLevel()
{
	Size size = Director::getInstance()->getVisibleSize();

	miner->dropGold();
	aniTimeDowm->gotoFrameAndPlay(0, -30,  false);
	miner->runDisApper();

	miner->runAction(
		Sequence::create(
			MoveTo::create(2, Vec2(size.width + 100, size.height - 110)),
			CallFuncN::create(
			[=](Ref* pSender)
			{
				//�˳���ʾ
				auto exitLevelTip = CSLoader::createNode("gameresult.csb");
				addChild(exitLevelTip);

				auto gameExit = static_cast<Text*>(Helper::seekWidgetByName(static_cast<Widget*>(exitLevelTip), "gameExit"));


				auto seq = Sequence::create(
					EaseBackInOut::create(MoveTo::create(0.5, size / 2)),
					DelayTime::create(1),
					EaseBackInOut::create(MoveTo::create(0.5, Vec2(size.width + 300, size.height / 2))),
					CallFuncN::create(
					[=](Node*)
					{
						Director::getInstance()->replaceScene(GameMenu::createScene());
					}),
					nullptr);

				gameExit->runAction(seq);
			}),
			nullptr)
	);
 
	
}

void MainGame::gameResult()
{
	Size size = Director::getInstance()->getVisibleSize();

	miner->dropGold();
	aniTimeDowm->gotoFrameAndPlay(0, -30, false);
	miner->runDisApper();

	miner->runAction(
		Sequence::create(
			MoveTo::create(2, Vec2(size.width + 100, size.height - 110)),
			CallFuncN::create(
				[=](Ref* pSender)
			{
				//�˳���ʾ
				auto exitLevelTip = CSLoader::createNode("gameresult.csb");
				addChild(exitLevelTip);

				auto gameFail = static_cast<Text*>(Helper::seekWidgetByName(static_cast<Widget*>(exitLevelTip), "gameFail"));
				auto gameSuccess = static_cast<Text*>(Helper::seekWidgetByName(static_cast<Widget*>(exitLevelTip), "gameSuccess"));
				auto nodeCoin = Helper::seekWidgetByName(static_cast<Widget*>(exitLevelTip), "nodeCoin");
				auto gainCoin = static_cast<Text*>(Helper::seekWidgetByName(static_cast<Widget*>(exitLevelTip), "gainCoin"));

				if (icurCoin < igoalCoin)//����ʧ��
				{
					auto seq = Sequence::create(
						EaseBackInOut::create(MoveTo::create(0.5, size / 2)),
						DelayTime::create(1),
						EaseBackInOut::create(MoveTo::create(0.5, Vec2(size.width + 300, size.height / 2))),
						CallFuncN::create(
							[=](Node*)
					{
						//��������
						UserDefault::getInstance()->setIntegerForKey("curLevel", 1);
						UserDefault::getInstance()->setIntegerForKey("curCoin", 0);

						Director::getInstance()->replaceScene(GameMenu::createScene());
					}),
						nullptr);

					gameFail->runAction(seq);
				}
				else//���سɹ�
				{
					int igainCoin = UserDefault::getInstance()->getIntegerForKey("curCoin");//��ʷ��ȡ�Ľ������
					gainCoin->setString(String::createWithFormat("%d", igainCoin)->getCString());

					//��������
					UserDefault::getInstance()->setIntegerForKey("curLevel", icurLevel + 1);
					UserDefault::getInstance()->setIntegerForKey("curCoin", 0);
					UserDefault::getInstance()->setIntegerForKey("gainCoin", igainCoin + icurCoin);

					auto seqSuccessTips = Sequence::create(
						EaseBackInOut::create(MoveTo::create(0.5, size / 2)),
						DelayTime::create(1),
						EaseBackInOut::create(MoveTo::create(0.5, Vec2(size.width + 300, size.height / 2))),
						CallFuncN::create(
							[=](Node*)
					{
						//��ת����һ��
						Director::getInstance()->replaceScene(MainGame::createScene());
					}),
						nullptr);
					gameSuccess->runAction(seqSuccessTips);


					//�ؿ���ȡ���
					auto seqNodeCoin = Sequence::create(
						EaseBackInOut::create(MoveTo::create(0.5, Vec2(size.width / 2, size.height / 2 - 100))),
						DelayTime::create(1),
						EaseBackInOut::create(MoveTo::create(0.5, Vec2(size.width + 300, size.height / 2 - 100))),
						nullptr);
					nodeCoin->runAction(seqNodeCoin);

				}
			}),
			nullptr)
		);
}


