#pragma once
#include "common/const.h"

class Uri {
public:
	static std::string uriEncode(std::string_view);
	static std::string uriDecode(std::string_view);
};

class Utils
{
public:
	static Uri uri;
};

