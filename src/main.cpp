#include <Geode/Geode.hpp>

#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/CustomSongLayer.hpp>
#include <Geode/modify/LevelSettingsObject.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/InputNode.hpp>
#include "UI/popup.hpp"
#include "UI/downloadingPopup.hpp"
#include <filesystem>
#include <Windows.h>
#include <fileapi.h>

#include <stdlib.h>
#include <stdio.h>
#include <shlobj_core.h>
#include <direct.h>
#include <regex>
#include <iostream>
#include <string>

using namespace geode::prelude;


class $modify(mCustomSongLayer, CustomSongLayer) {

	InputNode *ytLinkInput;
	InputNode *replacementIDInput;

	CCSize winSize = CCDirector::get()->getWinSize();
	float hSize = CCDirector::get()->getScreenScaleFactorH();

	MyPopup *MyPopup;

	bool init(LevelSettingsObject* p0) {
		if (!CustomSongLayer::init(p0))
			return false;
		
		//create menu
		auto menu = CCMenu::create();

		//create main button
		auto spr = ButtonSprite::create("nong");
        auto btn = CCMenuItemSpriteExtra::create(
            spr,
            this,
            menu_selector(mCustomSongLayer::onClick)
        );
		menu->addChild(btn);
		m_mainLayer->addChild(menu);
		btn->setPosition(-195.f, -70.f);

		return true;
	}

public:
    void onClick(CCObject* sender) {
		std::string filter = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890-_:/=?.";

		//get winsize
		auto winSize = CCDirector::get()->getWinSize();
		auto hSize = CCDirector::get()->getScreenScaleFactorH();

		//create popup
		m_fields->MyPopup = MyPopup::create("");
        m_fields->MyPopup->show();

		//create menu
		auto menu2 = CCMenu::create();
		m_fields->MyPopup->m_mainLayer->addChild(menu2);
	

		//create ytlink input field
		m_fields->ytLinkInput = InputNode::create(300.f, "Enter URL:", "bigFont.fnt", filter, 150);
		m_fields->ytLinkInput->setPosition(0, 30);
		m_fields->ytLinkInput->setEnabled(true);
		menu2->addChild(m_fields->ytLinkInput);

		//create replacementid input field
		m_fields->replacementIDInput = InputNode::create(300.f, "Enter Replacement ID:", "bigFont.fnt", "1234567890", 10);
		m_fields->replacementIDInput->setPosition(0, -20);
		m_fields->replacementIDInput->setEnabled(true);
		menu2->addChild(m_fields->replacementIDInput);

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
		//obtains link and replacement id from the input feilds
		std::string ytLink = m_fields->ytLinkInput->getString();
		std::string replacementID = m_fields->replacementIDInput->getString();
		log::info("link is: " + ytLink);
		log::info("replacement id is: " + replacementID);

		//obtains songolder path and mp3 location
		std::string mp3Location = "nongyt\\" + replacementID + ".mp3";
		std::string appdata = getenv("LOCALAPPDATA");
		std::string songFolder = appdata + "\\GeometryDash";
		log::info("song path is: " + songFolder);

		//error checks
		if (!isValidInput(ytLink, replacementID, songFolder)) {
			return;
		} 

		//downloads mp3
		std::string dlmp3 = "cd nongyt && yt-dlp -x --audio-format mp3 --parse-metadata \"title:%(artist)s - %(title)s\" --parse-metadata \"%(artist|)s:%(meta_artist)s\" --parse-metadata \"%(track|)s:%(meta_title)s\" --embed-metadata " + ytLink + " -o \"" + replacementID + ".%(ext)s\"";
		system(dlmp3.c_str());
		// WinExec(dlmp3.c_str(), SW_HIDE);

		//"done" popup
		auto downloadingPopup = downloadingPopup::create("Done!"); //td, add title in the done popup
		downloadingPopup->show();

		//copies mp3 to songfolder, deltes original
		std::filesystem::copy(mp3Location, songFolder);
		std::filesystem::remove(mp3Location);
	}

public:
	bool isValidInput(std::string ytLink, std::string replacementID, std::string songFolder) {
		const std::regex pattern("((http|https)://)(www.)?[a-zA-Z0-9@:%._\\+~#?&//=]{2,256}\\.[a-z]{2,6}\\b([-a-zA-Z0-9@:%._\\+~#?&//=]*)");
		if (ytLink == "" && replacementID == "") {
			auto downloadingPopup = downloadingPopup::create("enter smth\n  idiot");
        	downloadingPopup->show();
			return false;
		}
		if (ytLink == "") {
			auto downloadingPopup = downloadingPopup::create("enter\n URL");
			downloadingPopup->show();
			return false;
		}
		if (replacementID == "") {
			auto downloadingPopup = downloadingPopup::create("enter\n  ID");
			downloadingPopup->show();
			return false;
		}
		if(!std::regex_match(ytLink, pattern)) {
			auto downloadingPopup = downloadingPopup::create("Invalid\n  URL");
        	downloadingPopup->show();
			return false;
		}
		std::string mp3File = replacementID + ".mp3";

		for (const auto & entry : std::filesystem::directory_iterator(songFolder)) {
			std::string fullFileName = entry.path().string();
			std::string fileName = eraseSubStr(fullFileName, songFolder + "\\");

			if (fileName == mp3File) {
				auto downloadingPopup = downloadingPopup::create("ID already\n   in use");
				downloadingPopup->show();
				return false;
			}
		}
		return true;
	}
public:
	std::string eraseSubStr(std::string & mainStr, const std::string & toErase)
	{
		// Search for the substring in string
		size_t pos = mainStr.find(toErase);
		if (pos != std::string::npos)
		{
			// If found then erase it from string
			mainStr.erase(pos, toErase.length());
		}
		return mainStr;
	}
};