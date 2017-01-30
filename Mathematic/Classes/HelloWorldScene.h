#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class MainScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(MainScene);

private:
    void initDrawNode();
    void initTouches();
    void initLayerInput();
    void initResultLabels();

private:
    cocos2d::ui::TextField *labelExpr;
    cocos2d::ui::TextField *labelStep;
    cocos2d::ui::TextField *labelIn;
    cocos2d::ui::TextField *labelTo;
    cocos2d::LabelTTF *labelResultTrapesion;

    cocos2d::LabelTTF *labelResultSimson;
    cocos2d::LabelTTF *labelResultGauss;
    cocos2d::LabelTTF *labelResultSimpleTrapesion;
//    cocos2d::LabelTTF *labelResultTrapesion;

    cocos2d::ui::Button *fire;
    cocos2d::Layer *layerInput;

    cocos2d::Size visibleSize;

    cocos2d::DrawNode *dnode;

private:
    cocos2d::String expr;
    std::pair<float, float> into;
    float step;

public:
    inline float f(float x);
    float getSimpleTrapesion();
    float getGaussResult();
    float getSimsonResult();
    void calculateAndDraw();
    void drawing(cocos2d::DrawNode *node);
    void frawGrid(cocos2d::DrawNode *node);

private: // speciaal functions
    std::string setValueX(const std::string &strExpr, float x);
    float calculateAreaAndDrawDiscretPositions(const std::string &strExpr, const std::pair<float, float> &diapasonAB);

};

#endif // __MAIN_SCENE_H__
