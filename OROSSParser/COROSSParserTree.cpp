#include "COROSSParserTree.h"

void COROSSGrammaTree::load(const std::wstring path)
{
    std::locale loc = std::locale(std::locale("C"), new std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header>());
    // load data to search in articles
    std::wifstream bigr(path, std::wifstream::binary);

    if (bigr.is_open()) {
        bigr.imbue(loc);
        bigr.seekg(3);
        while (!bigr.eof()) {
            std::wstring str(L"");
            std::getline(bigr, str);
            str = str.substr(0, str.length() - 1);
            if (str.length() == 0)
                continue;

            std::wistringstream iss(str.c_str());
            std::vector<std::wstring> parts;
            std::copy(std::istream_iterator<std::wstring, wchar_t>(iss),
                std::istream_iterator<std::wstring, wchar_t>(),
                std::back_inserter(parts));
            if (parts.size() < 2)
                continue;
            if (depth < parts.size())
                depth = parts.size();
            auto bit = grMap.find(parts[0]);
            if (bit == grMap.end()) {
                node* root = new node();
                std::vector<std::wstring> v(parts.begin() + 1, parts.end());
                root = add(root, v);
                grMap.insert(std::pair<std::wstring, node*>(parts[0], root));
            }
            else {
                std::vector<std::wstring> v(parts.begin() + 1, parts.end());
                add(bit->second, v);
            }
        }
        bigr.close();
    }

/*    std::wofstream gramms(L"c:\\IRYA\\gramms.txt", std::wofstream::binary);
    if (gramms.is_open()) {
        gramms.imbue(loc);
        auto bit = grMap.begin();
        for (bit; bit != grMap.end(); ++bit) {
            gramms.write(L"-----------------------\n", wcslen(L"-----------------------\n"));
            gramms.write((*bit).first.c_str(), (*bit).first.length());
            print(bit->second, gramms);
        }
        gramms.close();
    }
    */
}

bool COROSSGrammaTree::find(const std::vector<std::wstring>& words)
{
    if (depth != words.size())
        return false;
    auto it = grMap.find(*(words.begin()));
    if (it == grMap.end())
        return false;
    return findgramm(it->second->child, std::vector<std::wstring>(words.begin() + 1, words.end()));
}


node* COROSSGrammaTree::add(node* root, std::vector<std::wstring> vct)
{
    if (vct.size() == 1) {
        if (root->child == 0) {
            root->child = new node();
            root->child->values.insert(std::pair<std::wstring, size_t>(*(vct.begin()), 1));
        }
        else {
            if (root->child->values.find(*(vct.begin())) == root->child->values.end()) {
                root->child->values.insert(std::pair<std::wstring, size_t>(*(vct.begin()), 1));
            }
        }
    }
    else {
        node* prev = 0;
        node* child = root->child;
        for (child; child != 0; child = child->next) {
            if (child->values.find((*vct.begin())) != child->values.end()) {
                child = add(child, std::vector<std::wstring>(vct.begin() + 1, vct.end()));
                break;
            }
            prev = child;
        }
        if (child == 0) {
            child = new node();
            child->values.insert(std::pair<std::wstring, size_t>(*(vct.begin()), 1));
            if (root->child == 0)
                root->child = child;
            if (prev != 0)
                prev->next = child;
            child = add(child, std::vector<std::wstring>(vct.begin() + 1, vct.end()));
        }
    }
    return root;
}

bool COROSSGrammaTree::findgramm(const node* root, const std::vector<std::wstring>& words)
{
    if (root == 0 || words.size() == 0)
        return false;
    for (root; root != 0; root = root->next) {
        if (root->values.find(*(words.begin())) != root->values.end()) {
            if (root->child != 0) {
                return findgramm(root->child, std::vector<std::wstring>(words.begin() + 1, words.end()));
            }
            return true;
        }
    }
    return false;
}

void COROSSGrammaTree::print(node* root, std::wofstream& gramms)
{
    auto it = root->values.begin();
    for (it; it != root->values.end(); ++it) {
        gramms.write((*it).first.c_str(), (*it).first.length());
        gramms.write(L" ", 1);
    }
    gramms.write(L"\n", 1);
    node* child = root->child;
    for (; child != 0;) {
        print(child, gramms);
        child = child->next;
    }
}
