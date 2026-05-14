#include "configManager.h"

SectionInfo::SectionInfo() {}

SectionInfo::~SectionInfo() {
	_section_map.clear();
}

SectionInfo::SectionInfo(const SectionInfo& sectionInfo) {
	_section_map = sectionInfo._section_map;
}

SectionInfo& SectionInfo::operator=(const SectionInfo& sectionInfo) {
	if (this == &sectionInfo) {
		return *this;
	}
	_section_map = sectionInfo._section_map;
	return *this;
}

std::string SectionInfo::operator[](const std::string& key) {
	if (_section_map.find(key) == _section_map.end()) {
		return "";
	}
	return _section_map[key];
}

ConfigManager::ConfigManager() {
	// 获取当前工作目录
	std::filesystem::path cur_path = std::filesystem::current_path();
	std::filesystem::path config_path = cur_path / "config.ini";
	std::cout << "Config path: " << config_path << std::endl;
	// 读取ini文件
	boost::property_tree::ptree pt;
	boost::property_tree::read_ini(config_path.string(), pt);

	for (const auto& section_pair : pt) {
		const std::string& section_name = section_pair.first;
		const boost::property_tree::ptree& section_tree = section_pair.second;

		std::map<std::string, std::string> section_config;
		for (const auto& key_value_pair : section_tree) {
			const std::string& key = key_value_pair.first;
			const std::string& value = key_value_pair.second.get_value<std::string>();
			section_config[key] = value;
		}
		
		SectionInfo sectionInfo;
		sectionInfo._section_map = section_config;
		_config_map[section_name] = sectionInfo;
	}

	for (const auto& section_entry : _config_map) {
		const std::string& section_name = section_entry.first;
		SectionInfo section_config = section_entry.second;
		std::cout << "[" << section_name << "]" << std::endl;
		for (const auto& key_value_pair : section_config._section_map) {
			std::cout << key_value_pair.first << "=" << key_value_pair.second << std::endl;
		}
	}
}

ConfigManager::~ConfigManager() {
	_config_map.clear();
}

ConfigManager::ConfigManager(const ConfigManager& config) {
	_config_map = config._config_map;
}

ConfigManager& ConfigManager::operator=(const ConfigManager& config) {
	if (this == &config) {
		return *this;
	}
	_config_map = config._config_map;
	return *this;
}

SectionInfo& ConfigManager::operator[](const std::string& key) {
	if (_config_map.find(key) == _config_map.end()) {
		SectionInfo();
	}
	return _config_map[key];
}
