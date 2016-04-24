#ifndef __MainGame_SCENE_H__
#define __MainGame_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Miner.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace cocostudio::timeline;

class MainGame : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(MainGame);

	virtual bool onTouchBegan(Touch* touch, Event*  event);
	void touchEnded(Touch* touch, Event*  event);
	void mouseDown(Event*  event);

private:
	int igoalCoin;

	Text* goalCoin;
	Text* curCoin;
	Text* curLevel;
	Text* timeDown;

	Button* btnPause;

	Miner* miner;
	ActionTimeline* aniTimeDowm;
};

#endif // __MainGame_SCENE_H__
