#ifndef __GameMenu_SCENE_H__
#define __GameMenu_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;

class GameMenu : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(GameMenu);

	void showMenu(Ref* pSender, Widget::TouchEventType type);
	void showSetting(Ref* pSender, Widget::TouchEventType type);

private:
	bool settingIsShow;

	Button* btnSetting;
};

#endif // __GameMenu_SCENE_H__
