#ifndef _TRANSLIT_MAP_H
#define _TRANSLIT_MAP_H

#include <string>
#include <map>

class COROSSTranslit
{
    std::map<wchar_t, std::wstring> symMap;
    std::wstring* result;
public:
    COROSSTranslit();
    COROSSTranslit(const COROSSTranslit& translit);
    ~COROSSTranslit() {};
    wchar_t operator ()(wchar_t c)
    {
        auto sit = symMap.find(c);
        if (sit != symMap.end())
            result->append(sit->second);
        return c;
    }

    void initMap(void);
    void resetResult(std::wstring* res);
    std::wstring* getResult(void) const;
};

#endif //_TRANSLIT_MAP_H
