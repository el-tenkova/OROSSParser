#ifndef __COROSSPARSER_CFG__
#define __COROSSPARSER_CFG__

#include <map>
#include <string>

class COROSSParserCfg
{
    std::map<std::string, std::string> configMap;
public:
    COROSSParserCfg() {};
    ~COROSSParserCfg() {};
    void load(const std::string& cfgFile);
    std::string operator[](const std::string& key) { return configMap[key]; }
};

#endif // __COROSSPARSER_CFG__
