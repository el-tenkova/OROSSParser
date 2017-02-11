#ifndef __OROSSPARSERMORPH_H_
#define __OROSSPARSERMORPH_H_

#ifdef _WINDOWS
    #include "resource.h"       // main symbols
#endif

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
    void Load(const std::string& foreign, const std::string& lemmata);
    void Terminate();
    bool Check(const std::wstring& word);
    bool Search(const std::wstring& word);
    std::wstring GetLemma(const std::wstring& word);
    bool IsLemma(const std::wstring& word);
};

#endif // __OROSSPARSERMORPH_H_
