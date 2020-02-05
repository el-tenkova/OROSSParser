#ifndef __OROSSPARSERMORPH_H_
#define __OROSSPARSERMORPH_H_

#ifdef _WINDOWS
    #include "resource.h"       // main symbols
#endif

#include <vector>
#include <map>
#include <string>
#include <locale>

#ifndef _ORFO
class COROSSParserMorph {

    std::map<std::wstring, size_t> foreignMap;
    std::map<std::wstring, std::wstring> lemmataMap;

public:
    COROSSParserMorph();
    ~COROSSParserMorph();
public:
    void Load();
    void Load(const std::string& foreign, const std::string& lemmata, const std::locale& russian);
    void Terminate();
    bool Check(const std::wstring& word);
    bool Search(const std::wstring& word);
    std::wstring GetLemma(const std::wstring& word);
    bool IsLemma(const std::wstring& word);
    std::wstring GetPlurNom() {};
};
#endif // _ORFO
#endif // __OROSSPARSERMORPH_H_
