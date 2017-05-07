#ifndef _GRAMMA_TREE_H_
#define _GRAMMA_TREE_H_

#ifdef _WINDOWS
    #include "stdafx.h"
    #include <assert.h>
    #include <direct.h>
#endif
#include <locale>
#include <string>

#include <map>
#include <vector>
#include <fstream>
#include <codecvt>
#include <regex>
#include <sstream>

struct node
{
    std::map<std::wstring, size_t> values;
    node* child;
    node* next;
    node() :
        child(0),
        next(0){};
};

class COROSSGrammaTree
{
    std::map<std::wstring, node*> grMap;
    size_t depth;
public:
    COROSSGrammaTree() :
        depth(0) {};
    ~COROSSGrammaTree() {};
public:
    void load(const std::string path, const std::map<std::wstring, size_t>& stopLabelDic, const std::locale& russian);

public:
    bool find(const std::vector<std::wstring>& words);
    size_t get_depth() { return depth; }
private:
    node* add(node* root, std::vector<std::wstring> vct);
    void print(node* root, std::wofstream& gramms);
    bool findgramm(const node* root, const std::vector<std::wstring>& words);
};

#endif //_GRAMMA_TREE_H_
