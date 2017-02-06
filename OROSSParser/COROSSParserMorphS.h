#ifndef __OROSSPARSERMORPH_H_
#define __OROSSPARSERMORPH_H_

#include "resource.h"       // main symbols

#include <vector>
#include <map>
#include <string>

class COROSSParserMorph {

    std::map<std::wstring, size_t> foreignMap;
    std::map<std::wstring, std::wstring> lemmataMap;

public:
    COROSSParserMorph();
    ~COROSSParserMorph();
public:
    void Load();
    void Terminate();
    bool Check(const std::wstring& word);
    bool Search(const std::wstring& word);
    std::wstring GetLemma(const std::wstring& word);
    bool IsLemma(const std::wstring& word);
};

#endif // __OROSSPARSERMORPH_H_
