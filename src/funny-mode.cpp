#include <Geode/Geode.hpp>
#include <Geode/modify/CCSprite.hpp>

using namespace geode::prelude;

class $modify(EverythingsACat, CCSprite) {
    static inline Ref<CCTexture2D> catTexture = nullptr;
    static inline bool enabled = false;

    static void refresh() {
        enabled = Mod::get()->getSettingValue<bool>("funny-mode");
        if (enabled && !catTexture) {
            catTexture = CCTextureCache::sharedTextureCache()->addImage("cat-icon.png"_spr, false);
        }
    }

    void setTexture(CCTexture2D* texture) {
        if (!enabled) {
            CCSprite::setTexture(texture);
            return;
        }
        if (!catTexture) refresh(); // just in case this runs before mod load finishes

        CCSprite::setTexture(catTexture ? catTexture.data() : texture);
    }
};

$on_mod(Loaded) {
    EverythingsACat::refresh();
    listenForSettingChanges<bool>("funny-mode", [](bool) {
        EverythingsACat::refresh();
    });
}
