#ifndef __GameSetting_SCENE_H__
#define __GameSetting_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

class GameSetting : public cocos2d::LayerColor
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(GameSetting);

	void close(Ref* pSender, Widget::TouchEventType type);
	void toggleEffect(Ref* pSender, Widget::TouchEventType type);
	void toggleBgMusic(Ref* pSender, Widget::TouchEventType type);

	void removeSetting(Ref* pSender);
private:

	Node* menuSetting;

	Button* btnBgMusic;
	Button* btnEffect;
	Button* btnClose;
	Button* btnNewbie;
	Button* btnAbout;


	Vec2 size;
};

#endif // __GameSetting_SCENE_H__
