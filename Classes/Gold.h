#ifndef __Gold_SCENE_H__
#define __Gold_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "spine/spine-cocos2dx.h"

using namespace cocos2d::ui;
using namespace spine;

class Gold : public cocos2d::Sprite
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static Gold* create(std::string type);

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init(std::string type);

	int getWeight();

private:
	int weight;
};

#endif // __Gold_SCENE_H__
