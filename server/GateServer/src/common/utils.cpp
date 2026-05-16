#include "common/utils.h"

// 0-15数字 转 16进制字符
unsigned char toHex(unsigned char x)
{
	// todo: 可以加一个断言
	return  x > 9 ? x + 55 : x + 48;
}

// 16进制 转为 0-15数字
unsigned char fromHex(unsigned char x)
{
	unsigned char y = 0;
	if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
	else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
	else if (x >= '0' && x <= '9') y = x - '0';
	else assert(0);
	return y;
}

std::string Uri::uriEncode(std::string_view uri) {
	std::string strTemp = "";
	size_t length = uri.length();
	for (size_t i = 0; i < length; i++)
	{
		//判断是否仅有数字和字母构成
		if (isalnum((unsigned char)uri[i]) ||
			(uri[i] == '-') ||
			(uri[i] == '_') ||
			(uri[i] == '.') ||
			(uri[i] == '~'))
			strTemp += uri[i];
		else if (uri[i] == ' ') //为空字符
			strTemp += "+";
		else
		{
			//其他字符需要提前加%并且高四位和低四位分别转为16进制
			strTemp += '%';
			strTemp += toHex((unsigned char)uri[i] >> 4);
			strTemp += toHex((unsigned char)uri[i] & 0x0F);
		}
	}
	return strTemp;
}

std::string Uri::uriDecode(std::string_view uri) {
	std::string strTemp = "";
	size_t length = uri.length();
	for (size_t i = 0; i < length; i++)
	{
		//还原+为空
		if (uri[i] == '+') strTemp += ' ';
		//遇到%将后面的两个字符从16进制转为char再拼接
		else if (uri[i] == '%')
		{
			assert(i + 2 < length);
			unsigned char high = fromHex((unsigned char)uri[++i]);
			unsigned char low = fromHex((unsigned char)uri[++i]);
			strTemp += high * 16 + low;
		}
		else strTemp += uri[i];
	}
	return strTemp;
}