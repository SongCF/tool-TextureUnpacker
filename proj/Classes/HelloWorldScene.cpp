#include "HelloWorldScene.h"
#include "PlistTool.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
	Size winSize = Director::getInstance()->getWinSize();

	//1. bg
	Sprite* bgSprite = Sprite::create("bg.png");
	this->addChild(bgSprite);
	bgSprite->setPosition(winSize/2);

	this->scheduleOnce(schedule_selector(HelloWorld::timeToDo), 0.1f);
   
    return true;
}

void HelloWorld::timeToDo(float dt)
{
	PlistTool *tool = new PlistTool();
	tool->unpackTextureByPlist("Enemy.plist");
}