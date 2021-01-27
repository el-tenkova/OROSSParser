/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: el-tenkova
 *
 * Created on 1 октября 2016 г., 16:23
 */

#include <cstdlib>
#include <iostream>

#include "OROSSParser/OROSSParser/COROSSParser.h"
#include "COROSSParser.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    auto a = 1;
    std::cout << argv[0] << std::endl;
    std::cout << argc << std::endl;
    std::locale loc = std::locale(std::locale("C"), new std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header>());
    
    COROSSParser op;
//    char* cfg = new char [strlen(argv[0] + 256)];
//    memset(cfg, 0, strlen(argv[0] + 256));
//    strcat(cfg, argv[0]);
//    strcat(cfg, "/rebuilder.cfg");
    std::string cfg("");
    COROSSParser::modeName Mode = COROSSParser::Update;
    for (int i = 1; i < argc; i+=2) {
        //std::cout << argv[i] << std::endl;
        //std::cout << argv[i + 1] << std::endl;
        if (strcmp(argv[i], "-m") == 0) {
            std::cout << argv[i + 1] << std::endl;
            Mode = (COROSSParser::modeName)atoi(argv[i + 1]);
        }
        else if (strcmp(argv[i], "-c") == 0)
            cfg = argv[i + 1];
            
    }
   // COROSSParser::modeName Mode = argc > 2 ? (COROSSParser::modeName)atoi(argv[2]) : COROSSParser::Update;
    std::cout << "Mode = "<< Mode << std::endl;
    std::cout << "Cfg = " << cfg << std::endl;
    op.Init(Mode, cfg);//"/home/test/Data/rebuilder.cfg");
    if (Mode == COROSSParser::PreView)
    {
        std::cout << "PreView" << std::endl;
        op.PreViewArticle();
    }
    else
    {
        //op.ReadOROSS("/home/test/Data/oross/oross.txt");
        op.Terminate();
    }
/*    COROSSDiacritics d;
    d.load("/root/Data/lemmata.txt");
    COROSSParserMorph m;
    m.Load("/root/Data/foreign.txt", "/root/Data/lemmata.txt"); */
    std::cout << "hello my friends!"<< std::endl;
//    delete cfg;
    return 0;
}

