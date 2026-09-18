#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(TheWormPlayLayer, PlayLayer) {
    struct Fields {
        std::vector<Ref<CCSprite>> cats;
        CCPoint lastPos = ccp(0, 0);
        bool hasLast = false;
    };

    void postUpdate(float dt) {
        PlayLayer::postUpdate(dt);

        auto player = m_player1;
        if (!player || !player->getParent()) return;

        bool enabled = Mod::get()->getSettingValue<bool>("the-worm");

        player->setOpacity(enabled ? 0 : 255);

        if (!enabled) {
            m_fields->hasLast = false;
            return;
        }

        auto pos = player->getPosition();

        if (!m_fields->hasLast) {
            m_fields->lastPos = pos;
            m_fields->hasLast = true;
            return;
        }

        float trailStep = 6.5f;
        if (ccpDistance(pos, m_fields->lastPos) < trailStep) return;

        m_fields->lastPos = pos;
        spawnCat(player->getParent(), pos, player->getRotation());
    }

    void spawnCat(CCNode* parent, CCPoint pos, float rot) {
        size_t maxCats = 420;

        if (m_fields->cats.size() >= maxCats) {
            if (auto oldest = m_fields->cats.front()) {
                oldest->removeFromParent();
            }
            m_fields->cats.erase(m_fields->cats.begin());
        }

        auto cat = CCSprite::create("cat-icon.png"_spr);
        if (!cat) return;

        cat->setPosition(pos);
        cat->setScale(0.25f);
        cat->setOpacity(220);
        cat->setRotation(rot);
        parent->addChild(cat, 100);

        cat->runAction(CCSequence::create(
            CCDelayTime::create(1.4f),
            CCFadeOut::create(0.45f),
            CCRemoveSelf::create(),
            nullptr));

        m_fields->cats.push_back(cat);
    }

    void cleanupCats() {
        for (auto& cat : m_fields->cats) {
            if (cat) cat->removeFromParent();
        }
        m_fields->cats.clear();
    }

    void resetLevel() {
        cleanupCats();
        PlayLayer::resetLevel();
    }
};
