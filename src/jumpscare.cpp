#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

// timings
constexpr float PUNCH_IN = 0.15f;
constexpr float HOLD_TIME = 1.8f;
constexpr float ZOOM_OUT = 0.4f;
constexpr float OVERSHOOT = 1.1f;

class $modify(MyMenuLayer, MenuLayer) {
    struct Fields {
        Ref<CCSprite> jumpscare = nullptr;
        bool busy = false; // in case someone spam it
    };

    bool init() {
        if (!MenuLayer::init()) return false;

        auto icon = CCSprite::create("cat-icon.png"_spr);
        if (!icon) return false;
        icon->setScale(0.5f);

        auto button = CCMenuItemSpriteExtra::create(
            icon, this, menu_selector(MyMenuLayer::onJumpscareButton));
        button->setID("jumpscare-button"_spr);

        auto bottomMenu = this->getChildByID("bottom-menu");
        if (!bottomMenu) return false;

        bottomMenu->addChild(button);
        bottomMenu->updateLayout();

        return true;
    }

    void onJumpscareButton(CCObject*) {
        if (m_fields->busy) return;
        m_fields->busy = true;

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        auto sprite = CCSprite::create("cat.png"_spr);
        if (!sprite) {
            m_fields->busy = false;
            return;
        }

        auto size = sprite->getContentSize();
        float coverScale = std::max(winSize.width / size.width, winSize.height / size.height);

        sprite->setPosition(winSize / 2);
        sprite->setScale(coverScale * OVERSHOOT);
        sprite->setOpacity(0);
        this->addChild(sprite, 999999);

        m_fields->jumpscare = sprite;

        auto punchIn = CCSpawn::create(
            CCEaseBackOut::create(CCScaleTo::create(PUNCH_IN, coverScale)),
            CCFadeIn::create(PUNCH_IN * 0.5f),
            nullptr);

        auto zoomOut = CCSpawn::create(
            CCScaleTo::create(ZOOM_OUT, coverScale * OVERSHOOT),
            CCFadeOut::create(ZOOM_OUT),
            nullptr);

        sprite->runAction(CCSequence::create(
            punchIn,
            CCDelayTime::create(HOLD_TIME),
            zoomOut,
            CallFuncExt::create([this]() {
                if (m_fields->jumpscare) {
                    m_fields->jumpscare->removeFromParent();
                    m_fields->jumpscare = nullptr;
                }
                m_fields->busy = false;
            }),
            nullptr
        ));

        shakeScreen();
    }

private:
    void shakeScreen() {
        auto origin = this->getPosition();

        // asymmetric on purpose
        this->runAction(CCSequence::create(
            CCMoveBy::create(0.03f, { 10.f, -2.f }),
            CCMoveBy::create(0.025f, { -18.f, 4.f }),
            CCMoveBy::create(0.03f, { 14.f, -6.f }),
            CCMoveBy::create(0.02f, { -9.f, 3.f }),
            CCMoveBy::create(0.025f, { 5.f, -1.f }),
            CCMoveTo::create(0.03f, origin),
            nullptr
        ));
    }
};
