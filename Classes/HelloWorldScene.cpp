#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "MathEngine.h"
using std::string;

USING_NS_CC;

static const float scale = 20;
static const string NAME_GAUSS = "Гаус: ";
static const string NAME_SIMSON = "Сімсон: ";
static const string NAME_SIMPLE_TRAPESION = "Трапеція: ";
static const string NAME_TRAPESION = "Квад. Трапеція: ";

Scene* MainScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();

    initResultLabels();

    initLayerInput();
    initDrawNode();
    initTouches();



    return true;
}


void MainScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void MainScene::initDrawNode()
{
    dnode = DrawNode::create(3);
    dnode->setPosition(visibleSize / 2);
    addChild(dnode, 3);
}

void MainScene::initTouches()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [this](Touch*, Event*) -> bool {
        return true;
    };

    listener->onTouchMoved = [this](Touch* touch, Event*) -> void {
        dnode->setPosition(dnode->getPosition() + (touch->getLocation() - touch->getPreviousLocation()));
    };

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void MainScene::initLayerInput()
{
    labelIn = cocos2d::ui::TextField::create("In", "fonts/Bicubik.ttf", 32);
    labelTo = cocos2d::ui::TextField::create("To", "fonts/Bicubik.ttf", 32);
    labelStep = cocos2d::ui::TextField::create("5",   "fonts/Bicubik.ttf", 32);
    labelExpr = cocos2d::ui::TextField::create("x * x", "fonts/Bicubik.ttf", 32);
    labelExpr->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);

    labelIn->setPosition({visibleSize.width - 100, 64});
    labelTo->setPosition({visibleSize.width - 100, 32});
    labelExpr->setPosition({visibleSize.width - 40, visibleSize.height - 32});

    cocos2d::ui::Button *button = cocos2d::ui::Button::create("button.png");
    button->addClickEventListener([this](Ref*){
        calculateAndDraw();
    });
    button->setPosition({100, 100});

    layerInput = Layer::create();
    layerInput->addChild(labelIn);
    layerInput->addChild(labelTo);
    layerInput->addChild(labelStep);
    layerInput->addChild(labelExpr);
    layerInput->addChild(button);
    this->addChild(layerInput, 5);
}

void MainScene::initResultLabels()
{
    int LevelResult = 5;

    labelResultGauss = LabelTTF::create(NAME_GAUSS, "fonts/Bicubik.ttf", 18);
    labelResultTrapesion = LabelTTF::create(NAME_TRAPESION, "fonts/Bicubik.ttf", 18);
    labelResultSimpleTrapesion = LabelTTF::create(NAME_SIMPLE_TRAPESION, "fonts/Bicubik.ttf", 18);
    labelResultSimson = LabelTTF::create(NAME_SIMSON, "fonts/Bicubik.ttf", 18);

    labelResultGauss->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    labelResultSimpleTrapesion->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    labelResultSimson->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    labelResultTrapesion->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);

    float posXLabel = 40;
    float stepSubY = 32;
    float posTopY = visibleSize.height - stepSubY;

    labelResultSimson->setPosition(posXLabel, posTopY);
    labelResultGauss->setPosition(posXLabel, labelResultSimson->getPositionY() - stepSubY);
    labelResultTrapesion->setPosition(posXLabel, labelResultGauss->getPositionY() - stepSubY);
    labelResultSimpleTrapesion->setPosition(posXLabel, labelResultTrapesion->getPositionY() - stepSubY);

    for (LabelTTF* ptr : {labelResultGauss,
                          labelResultSimpleTrapesion,
                          labelResultSimson,
                          labelResultTrapesion})
    {
        addChild(ptr, LevelResult);
    }

}

float MainScene::f(float x)
{
    return MathEngine::calculate(setValueX(expr.getCString(), x));
}

float MainScene::getSimpleTrapesion()
{
    const auto &a = into.first;
    const auto &b = into.second;

    return ((f(a) + f(b)) / 2) * (b-a);
}

float MainScene::getGaussResult()
{
    const auto &a = into.first;
    const auto &b = into.second;

    float f1 = f( (a+b)/2 - (b-a)/(2.f * sqrt(3.0)) );
    float f2 = f( (a+b)/2 + (b-a)/(2.f * sqrt(3.0)) );
    return (f1 + f2) * (b-a)/2;
}

float MainScene::getSimsonResult()
{
    const auto &a = into.first;
    const auto &b = into.second;

    return (b - a)/6 * (f(a) + 4.f*f((a + b) / 2) + f(b));
}

void MainScene::calculateAndDraw()
{
    step = 0.2;
    expr = labelExpr->getString();
    into.first = atof(labelIn->getString().c_str());
    into.second= atof(labelTo->getString().c_str());

    if (into.first >= into.second)
        return;

    if ((into.first * into.second) < 0) {

        auto cinto = into;

        float r = 0;
        for (auto vec : {std::pair<float, float>(into.first, 0), std::pair<float, float>(0, into.second)}) {
            into = vec;
            r += MathEngine::ABS(getGaussResult());
        }
        labelResultGauss->setString(NAME_GAUSS + std::to_string(r));
        into = cinto;
    }
    else {
        labelResultGauss->setString(NAME_GAUSS + std::to_string(getGaussResult()));
        labelResultSimson->setString(NAME_SIMSON + std::to_string(getSimsonResult()));
        labelResultSimpleTrapesion->setString(NAME_SIMPLE_TRAPESION + std::to_string(getSimpleTrapesion()));
    }

    log("Simson: %f", getSimsonResult());
    log("Gauss: %f", getGaussResult());
    log("Trapesion: %f", getSimpleTrapesion());

    dnode->clear();
    dnode->setLineWidth(1);
    frawGrid(dnode);

    drawing(dnode);
    calculateAreaAndDrawDiscretPositions(expr.getCString(), into);
    log("drawing");
}

float my_floor_to_3(float value){
    //return value;
    return (float)((int)(value * 100000)) / 100000;
}

void MainScene::drawing(DrawNode *node)
{
    string str = expr.getCString();

    log("c_str: %s", str.c_str());

    auto lamGetCoord = [=](string str, float x) -> Vec2 {

        string out;
        for(int i = 0; i < str.length(); i++){
            if(str[i] == 'x'){
                out += StringUtils::toString(x);
            }
            else out += str[i];
        }

        return Vec2(x*scale, MathEngine::calculate(out)*scale);

    };

    static float i;
    i = into.first;

    auto lambdaDrawing = [=, &i]() -> void {

        static const float dt = 0.01;
        auto p1 = lamGetCoord(str, my_floor_to_3(i));
        auto p2 = lamGetCoord(str, my_floor_to_3(i + dt));

        if (p1.y == INFINITY || p2.y == INFINITY || p1.distance(p2) > 1000) {
            i += dt;
            return;
        }

        node->drawSegment(p1, p2, 1, Color4F::GREEN);
        i += dt;

    };

    while (i < into.second)
        lambdaDrawing();
}

void MainScene::frawGrid(DrawNode *node)
{
    float offsetX = ((visibleSize.width / scale) - ((int)((visibleSize.width / scale)))) * scale * scale;
    float offsetY = ((visibleSize.height/ scale) - ((int)((visibleSize.height/ scale)))) * scale * scale;

    node->drawLine(Vec2(-visibleSize.width, 0), Vec2(visibleSize.width, 0), Color4F::WHITE);
    node->drawLine(Vec2(0, -visibleSize.height),Vec2(0, visibleSize.height), Color4F::WHITE);

    /* drawing horisontal lines */
    for (float i = -visibleSize.height * scale; i  < visibleSize.height * scale; i += scale) {
        Vec2 p1(-visibleSize.width * scale, i + offsetY);
        Vec2 p2( visibleSize.width * scale, i + offsetY);
        node->drawLine(p1, p2, Color4F(0.3, 0.3, 0.3, 0.5));
    }

    /* drawing vertical lines */
    for (float i = -visibleSize.width * scale; i  < visibleSize.width * scale; i += scale) {
        Vec2 p1(i + offsetX, -visibleSize.height * scale);
        Vec2 p2(i + offsetX,  visibleSize.height * scale);
        node->drawLine(p1, p2, Color4F(0.3, 0.3, 0.3, 0.5));
    }

}

std::string MainScene::setValueX(const std::string &strExpr, float x)
{
    string outExpr;
    outExpr.reserve(strExpr.length());
    for(int i = 0; i < strExpr.length(); i++){
        if(strExpr[i] == 'x'){
            outExpr += std::to_string(x);
        }
        else outExpr += strExpr[i];
    }
    return outExpr;
}

float MainScene::calculateAreaAndDrawDiscretPositions(const std::string &strExpr, const std::pair<float, float> &diapasonAB)
{
    float h = (diapasonAB.second - diapasonAB.first) / step;

    float resultArea = 0;

    float begin = MathEngine::ABS(MathEngine::calculate(setValueX(strExpr, diapasonAB.first))  / 2);
    resultArea += begin;
    resultArea += MathEngine::ABS(MathEngine::calculate(setValueX(strExpr, diapasonAB.second)) / 2);

    std::vector<Vec2> points;
    points.push_back(Vec2(diapasonAB.first*scale, begin*scale));

    int index(0);
    for (float x = diapasonAB.first + step; x < diapasonAB.second; x += step) {
        float y = (MathEngine::calculate(setValueX(strExpr, x)));
        dnode->drawLine(Vec2(x*scale, 0), Vec2(x*scale, y*scale), Color4F::MAGENTA);
//        dnode->drawLine(points[index], points[index+1], Color4F::RED);
        resultArea += MathEngine::ABS(y);
        index++;
    }
    resultArea *= (diapasonAB.second - diapasonAB.first) / h;
    labelResultTrapesion->setString(NAME_TRAPESION + std::to_string(resultArea));
    log("result area: %f", resultArea);
    return resultArea;
}
