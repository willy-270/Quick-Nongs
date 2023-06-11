#include <Geode/Geode.hpp>

#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/CustomSongLayer.hpp>
#include <Geode/modify/LevelSettingsObject.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/InputNode.hpp>
#include "UI/popup.hpp"
#include <filesystem>
#include <Windows.h>
#include <winbase.h>
#include <regex>
#include <wininet.h>
#include <iostream>
#include <fstream>
#include <string>

#pragma comment(lib,"Wininet.lib")

using namespace geode::prelude;


class $modify(mCustomSongLayer, CustomSongLayer) {

	InputNode *ytLinkInput;
	InputNode *replacementIDInput;

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

		//obtains songolder path and mp3 location
		std::string mp3Location = "nongyt\\" + replacementID + ".mp3";
		std::string appdata = getenv("LOCALAPPDATA");
		std::string songFolder = appdata + "\\GeometryDash";

		//error checks
		if (isValidInput(ytLink, replacementID, songFolder)) {
			dlMp3();
		}  
	}	

public:
	bool isValidInput(std::string ytLink, std::string replacementID, std::string songFolder) {
		if (ytLink == "" && replacementID == "") {
			FLAlertLayer::create(
				"Nothing Entered",    
				"Try again idiot.",  
				"OK"        
			)->show();
			return false;
		}

		if (ytLink == "") {
			FLAlertLayer::create(
				"No URL",    
				"Please enter a URL.",  
				"OK"        
			)->show();
			return false;
		}

		if (replacementID == "") {
			FLAlertLayer::create(
				"No ID",    
				"Please enter a replacement ID.",  
				"OK"        
			)->show();
			return false;
		}

		const std::regex pattern("((http|https)://)(www.)?[a-zA-Z0-9@:%._\\+~#?&//=]{2,256}\\.[a-z]{2,6}\\b([-a-zA-Z0-9@:%._\\+~#?&//=]*)");

		if(!std::regex_match(ytLink, pattern)) {
			FLAlertLayer::create(
				"Invalid URL",    
				"Please enter a valid URL.",  
				"OK"        
			)->show();
			return false;
		}

		char url[128];
		strcat(url, "http://www.google.com");
		bool bConnect = InternetCheckConnection(url, FLAG_ICC_FORCE_CONNECTION, 0);

		if (!bConnect) {
			FLAlertLayer::create(
				"Network not found",    
				"Please check your connection.",  
				"OK"        
			)->show();
			return false;
		}

		std::string mp3File = replacementID + ".mp3";;

		//loops through every file in the gd song folder and checks if any match the reauested replacement id
		for (const auto & entry : std::filesystem::directory_iterator(songFolder)) {
			std::string fullFileName = entry.path().string();
			std::string fileName = eraseSubStr(fullFileName, songFolder + "\\");

			if (fileName == mp3File) {
				createQuickPopup(
					"ID Already In Use",        
					"Replace exising file?", 
					"No", "Yes",    
					[this](auto, bool btn2) {
						if (btn2) {
							replaceFile();
						}
					}
				);
				return false;
			}
		}
		return true;
	}
public:
	void replaceFile() {
		//deltes conflicting file in the gd song folder, then downloads the mp3
		std::string appdata = getenv("LOCALAPPDATA");
		std::string songFolder = appdata + "\\GeometryDash";
		std::string replacementID = m_fields->replacementIDInput->getString();
		std::string ytLink = m_fields->ytLinkInput->getString();
		std::string mp3File = replacementID + ".mp3";

		std::filesystem::remove(songFolder + "\\" + mp3File);
		std::filesystem::remove("nongyt\\" + mp3File + "info.json");

		dlMp3();
	}
public: 
	void dlMp3() {
		//get variables that ive already gotten like 10 seperate times but global stuff doesnt work good
		std::string appdata = getenv("LOCALAPPDATA");
		std::string songFolder = appdata + "\\GeometryDash";
		std::string replacementID = m_fields->replacementIDInput->getString();
		std::string ytLink = m_fields->ytLinkInput->getString();
		std::string mp3File = replacementID + ".mp3";
		std::string mp3Location = "nongyt\\" + mp3File;

		//download the mp3
		std::string dlMp3Cmd = "cd nongyt && yt-dlp -x --audio-format mp3 --parse-metadata \"title:%(artist)s - %(title)s\" --parse-metadata \"%(artist|)s:%(meta_artist)s\" --parse-metadata \"%(track|)s:%(meta_title)s\" --embed-metadata " + ytLink + " -o \"" + replacementID + ".%(ext)s\" --console-title --write-info-json";
		system(dlMp3Cmd.c_str());

		std::ifstream file("nongyt\\" + replacementID + ".info.json");
		std::string json((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		log::info(json);

		// Extract values from the JSON
		std::string title = extractValue(json, "title");
		log::info(title);
		
		std::string artist = extractValue(json, "artist");
		if (artist == "") {
			artist = extractValue(json, "uploader");
		}		
		log::info(artist);

		// copies mp3 to songfolder, deltes original
		std::filesystem::copy(mp3Location, songFolder);
		std::filesystem::remove(mp3Location);
		log::info("copy n remove done");

		// "done" popup
		FLAlertLayer::create(
			"Done!",    
			"Download sucsessful!\nStored <cy>\"" + title + "\"</c> by <cy>" + artist + "</c> as <cy>\"" + replacementID + ".mp3\"</c>",  
			"OK"        
		)->show();
	}
public:
	//used to narrow down to the mp3 file paths to just the file itself
	std::string eraseSubStr(std::string & mainStr, const std::string & toErase)
	{
		size_t pos = mainStr.find(toErase);
		if (pos != std::string::npos) {
			mainStr.erase(pos, toErase.length());
		}
		return mainStr;
	}
public:
	std::string extractValue(const std::string& json, const std::string& key) {
		size_t pos = json.find("\"" + key + "\":");
		if (pos == std::string::npos)
			return ""; // Key not found

		pos = json.find_first_of("\"", pos + key.length() + 3); // Move past the key and the ":"
		size_t endPos = json.find_first_of("\"", pos + 1); // Find the closing quote

		if (pos == std::string::npos || endPos == std::string::npos)
			return ""; // Malformed JSON

		return json.substr(pos + 1, endPos - pos - 1);
	}
};