#pragma once

#include <Geode/loader/SettingNode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/Geode.hpp>

using namespace geode::prelude;


class downloadingPopup : public geode::Popup<std::string const&> {
protected:
    bool setup(std::string const& value) override {
        auto winSize = CCDirector::sharedDirector()->getWinSize();

        // convenience function provided by Popup 
        // for adding/setting a title to the popup
        this->setTitle("");

        auto label = CCLabelBMFont::create(value.c_str(), "bigFont.fnt");
        label->setPosition(winSize / 2);
        this->addChild(label);

        return true;
    }

public:
    static downloadingPopup* create(std::string const& text) {
        auto ret = new downloadingPopup();
        if (ret && ret->init(240.f, 140.f, text)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};