#pragma once

#include <Geode/loader/SettingNode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/Geode.hpp>

using namespace geode::prelude;


class MyPopup : public geode::Popup<std::string const&> {
protected:
    bool setup(std::string const& value) override {
        auto winSize = CCDirector::sharedDirector()->getWinSize();

        // convenience function provided by Popup 
        // for adding/setting a title to the popup
        this->setTitle("Import Nong via YouTube link");

        auto label = CCLabelBMFont::create(value.c_str(), "bigFont.fnt");
        label->setPosition(winSize / 2);
        this->addChild(label);

        return true;
    }

public:
    static MyPopup* create(std::string const& text) {
        auto ret = new MyPopup();
        if (ret && ret->init(480.f, 280.f, text)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};