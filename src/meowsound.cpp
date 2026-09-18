#include <Geode/Geode.hpp>
#include <Geode/modify/CCMenuItem.hpp>

using namespace geode::prelude;

class $modify(MeowMenuItem, CCMenuItem) {
    void activate() {
        if (Mod::get()->getSettingValue<bool>("meow-sound")) {
            FMODAudioEngine::sharedEngine()->playEffect("meow.mp3"_spr);
        }
        CCMenuItem::activate();
    }
};
