#pragma once
#include "common/const.h"

class SectionInfo
{
public:
	SectionInfo();
	~SectionInfo();
	SectionInfo(const SectionInfo&);
	SectionInfo& operator=(const SectionInfo&);
	std::string operator[](const std::string& key);
public:
	std::map<std::string, std::string> _section_map;
};

class ConfigManager
{
public:
	ConfigManager();
	~ConfigManager();
	ConfigManager(const ConfigManager&);
	ConfigManager& operator=(const ConfigManager&);
	SectionInfo& operator[](const std::string& key);
private:
	std::map<std::string, SectionInfo> _config_map;
};

