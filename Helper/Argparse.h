//
// Created by DaringJoker on 11/19/2020.
//

#ifndef EMULATOR_ARGPARSE_H
#define EMULATOR_ARGPARSE_H
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

class Argparse {
public:
    enum dTypes{Integer,Float,String,Path,Boolean,Repetition};
    typedef struct argument{
        string shortname;
        string longname;
        dTypes expectedType;
        string Help;
        string Value;
        int reps=0;
        bool positional=false;
        bool found=false;
        bool required;
    } argument;
private:
    char** argv;
    int argc;
    vector<argument> kwargs;
    vector<argument> pargs;
    vector<string> errors;
    int no_of_unfullfilled_requirements=0;
    int positionalcounter=0;
    string Header;
    string slogan;
    string Footer;
    string HelpString;
    void printHelp();
    void printUsage();
    int hasTokenWithShortName(string token);
    int hasTokenWithLongName(string token);
    int hasPositionalWithName(string token);
    void spewErrors();
public:
    explicit Argparse(string slogan);
    void setHeader(string header);
    void setFooter(string footer);
    Argparse::argument getArg(string token);
    void AddArgument(string Shortname, string Longname, dTypes Expectedtype, string Helpstring, int required = false,
                     bool positional=false);

    void ParseArgument(int argcount, char ** argvector);

    void registerRequirementErrors();
};


#endif //EMULATOR_ARGPARSE_H
