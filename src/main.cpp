#include <Geode/Geode.hpp>

#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/CustomSongLayer.hpp>
#include <Geode/modify/LevelSettingsObject.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/InputNode.hpp>
#include "UI/popup.hpp"
#include "Geode/loader/log.hpp"

using namespace geode::prelude;


class $modify(mCustomSongLayer, CustomSongLayer) {

	InputNode ytLinkInput;
	InputNode replacementIDInput;

	bool init(LevelSettingsObject* p0) {
		if (!CustomSongLayer::init(p0))
			return false;
		
		//create menu
		auto menu = CCMenu::create();
		this->addChild(menu);

		//create main button
		auto spr = ButtonSprite::create("nong");
        auto btn = CCMenuItemSpriteExtra::create(
            spr,
            this,
            menu_selector(mCustomSongLayer::onClick)
        );
		menu->addChild(btn);
		btn->setPosition(-145.f, -90.f);

		return true;
	}

public:
    void onClick(CCObject* sender) {
		//get winsize
		auto winSize = CCDirector::get()->getWinSize();
		auto hSize = CCDirector::get()->getScreenScaleFactorH();

		//create popup + menu
		auto MyPopup = MyPopup::create("");
        MyPopup->show();
		auto menu2 = CCMenu::create();
		MyPopup->addChild(menu2);

		//create ytlink input field
		m_fields->ytLinkInput = InputNode::create(300.f, "Enter YT Link: ", "bigFont.fnt");
		MyPopup->addChild(ytLinkInput);
		ytLinkInput->setPosition(winSize.width / 2, 200.f);
		ytLinkInput->setEnabled(true);

		//create replacementid input field
		auto replacementIDInput = InputNode::create(300.f, "Enter Replacement ID: ", "bigFont.fnt");
		m_fields->replacementIDInput;
		MyPopup->addChild(replacementIDInput);
		replacementIDInput->setPosition(winSize.width / 2, 150.f);
		replacementIDInput->setEnabled(true);

		//create submit button
		auto spr2 = ButtonSprite::create("submit");
        auto btn2 = CCMenuItemSpriteExtra::create(
            spr2,
            this,
            menu_selector(mCustomSongLayer::submit)
        );
		menu2->addChild(btn2);
		btn2->setPosition(hSize / 2, -90.f);
    }

public:
    void submit(CCObject* sender) {
		std::string ytLink = m_fields->ytLinkInput.getString();
		log::info(ytLink);
		
		// auto winSize = CCDirector::get()->getWinSize(); 

		// auto label = CCLabelBMFont::create(ytLink2.c_str(), "bigFont.fnt");
		// label->setPosition(winSize.width / 2, winSize.height / 2);
		// this->addChild(label);
	

		// std::string filename = "yayyy.py";
		// std::string command = "python ";
		// command += filename;
		// system(command.c_str());
	}
};



