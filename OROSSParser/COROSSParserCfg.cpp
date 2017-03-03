#ifdef _WINDOWS
#include "stdafx.h"
#endif

#include <fstream>
#include <vector>
#include <sstream>
#include <iterator>

#include "COROSSParserCfg.h"

void COROSSParserCfg::load(const std::string& cfgFile)
{
    std::ifstream config(cfgFile, std::ifstream::binary);
    configMap["errors"] = "";
    configMap["stop"] = "";
    configMap["stop_l"] = "";
    configMap["bigramms"] = "";
    configMap["trigramms"] = "";
    configMap["tetragramms"] = "";
    configMap["smap"] = "";
    configMap["lemmata"] = "";
    configMap["foreign"] = "";
    configMap["ROS_2012"] = "";
    configMap["arts"] = "";
    configMap["pararest"] = "";
    configMap["formulas"] = "";
    configMap["orthos"] = "";
    configMap["tutorial"] = "";
    configMap["output"] = "";
    configMap["output_articles"] = "";
    configMap["output_words"] = "";
    configMap["output_mistakes"] = "";
    configMap["output_abc"] = "";
    configMap["words"] = "";

    if (config.is_open()) {
        while (!config.eof()) {
            std::string str("");
            std::getline(config, str);
            str = str.substr(0, str.length() - 1);
            if (str.length() == 0)
                continue;

            std::istringstream iss(str.c_str());
            std::vector<std::string> parts;
            std::copy(std::istream_iterator<std::string, char>(iss),
                std::istream_iterator<std::string, char>(),
                std::back_inserter(parts));
            if (parts.size() < 2)
                continue;
            auto it = configMap.find(parts[0]);
            if (it != configMap.end())
                it->second = parts[1];

        }
        config.close();
    }
}

