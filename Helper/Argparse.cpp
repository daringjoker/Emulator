//
// Created by DaringJoker on 11/19/2020.
//

#include "Argparse.h"

#include <utility>
#include <regex>

Argparse::Argparse(string slogan) {
    this->slogan = slogan;
}

void Argparse::setHeader(string header) {
    Header = std::move(header);
}

void Argparse::setFooter(string footer) {
    Footer = std::move(footer);
}


void Argparse::ParseArgument(int argcount, char **argvector) {
    \
    this->argc = argcount;
    this->argv = argvector;
    if (argc == 1) printUsage();
    for (int index = 1; index < argcount; ++index) {
        string token = string(argvector[index]);
        if (token == string("-h") || token == string("--help"))printHelp();
        if (token[0] == '-' && token[1] == '-') {
            int position = hasTokenWithLongName(std::regex_replace(token, std::regex("--"), std::string("")));
            if (position > 0) {
                position = position - 1;
                kwargs[position].found = true;
                if (kwargs[position].required) no_of_unfullfilled_requirements--;
                if (kwargs[position].expectedType != Repetition && kwargs[position].expectedType != Boolean)
                    kwargs[position].Value = string(argvector[++index]);
                else if (kwargs[position].expectedType == Repetition)
                    kwargs[position].reps++;
            } else errors.push_back(string("No such argument found ") + token);
        } else if (token[0] == '-') {
            int position = hasTokenWithShortName(std::regex_replace(token, std::regex("-"), std::string("")));
            if (position > 0) {
                position = position - 1;
                kwargs[position].found = true;
                if (kwargs[position].required) no_of_unfullfilled_requirements--;
                if (kwargs[position].expectedType != Repetition && kwargs[position].expectedType != Boolean)
                    kwargs[position].Value = string(argvector[++index]);
                else if (kwargs[position].expectedType == Repetition)
                    kwargs[position].reps++;
            } else errors.push_back(string("No such argument found ") + token);
        } else if (pargs.size() > positionalcounter) {
            pargs[positionalcounter].Value = string(argvector[index]);
            pargs[positionalcounter].found = true;
            if (pargs[positionalcounter].required) no_of_unfullfilled_requirements--;
            positionalcounter++;
        } else errors.push_back(string("unknown token found ") + token);
    }
    if (no_of_unfullfilled_requirements > 0)
        registerRequirementErrors();
    if (!errors.empty()) {
        spewErrors();
    }

}

void Argparse::printHelp() {
    if (!Header.empty()) cout << Header << endl;
    cout << argv[0] << " :\t" << slogan << endl;\
    char options[1024];
    if (!pargs.empty()) {
        cout << "\nPositional Arguments:" << endl;
        for (auto &arg: pargs) {
            if (arg.required) sprintf(options, "%s / %s", arg.shortname.c_str(), arg.longname.c_str());
            else sprintf(options, "[%s / %s]", arg.shortname.c_str(), arg.longname.c_str());
            printf("\t%-30s%s\n", options, arg.Help.c_str());
        }
    }
    if (!kwargs.empty()) {
        cout << "\nKeyword Arguments:" << endl;
        for (auto &arg: kwargs) {
            if (arg.required) sprintf(options, "-%s / --%s", arg.shortname.c_str(), arg.longname.c_str());
            else sprintf(options, "[-%s / --%s]", arg.shortname.c_str(), arg.longname.c_str());
            printf("\t%-30s%s\n", options, arg.Help.c_str());
        }
        printf("\t%-30s%s\n", "[-h / --help]", "Display this Help and Exit.");
    }
    if (!Footer.empty())cout << Footer << endl;
    exit(0);
}

void Argparse::printUsage() {
    if (!Header.empty())cout << Header << endl;
    cout << argv[0] << " :\t" << slogan << endl;
    cout << "\n\n";
    cout << "Usage: " << argv[0] << " ";
    for (auto &arg: pargs)cout << arg.shortname << "/" << arg.longname << " ";
    for (auto &arg: kwargs)cout << "[-" << arg.shortname << "/--" << arg.longname << "] ";
    cout << "[ -h/--help ]";
    cout << endl;
    if (!Footer.empty()) cout << Footer << endl;
    exit(0);
}

void Argparse::AddArgument(string Shortname, string Longname, dTypes Expectedtype, string Helpstring, int required,
                           bool positional) {
    argument arg;
    arg.longname = std::move(Longname);
    arg.shortname = std::move(Shortname);
    arg.expectedType = Expectedtype;
    arg.required = required;
    if (required)no_of_unfullfilled_requirements++;
    arg.Help = std::move(Helpstring);
    arg.positional = positional;
    if (arg.positional)
        pargs.push_back(arg);
    else kwargs.push_back(arg);
}

int Argparse::hasTokenWithShortName(string token) {
    int count = 0;
    for (const auto &key : kwargs) {
        count++;
        if (key.shortname == token)
            return count;
    }
    return 0;
}

int Argparse::hasTokenWithLongName(string token) {
    int count = 0;
    for (const auto &key : kwargs) {
        count++;
        if (key.longname == token)
            return count;
    }
    return 0;
}

int Argparse::hasPositionalWithName(string token) {
    int count = 0;
    for (const auto &key : pargs) {
        count++;
        if (key.longname == token || key.shortname == token)
            return count;
    }
    return 0;
}

void Argparse::registerRequirementErrors() {
    for (auto &arg:kwargs) {
        if (arg.required && !arg.found) errors.push_back(
                    string("Argument -") + arg.shortname + string("/--") + arg.longname +
                    string(" is Required but not supplied"));
    }
    for (auto &arg:pargs) {
        if (arg.required && !arg.found) errors.push_back(
                    string("Argument ") + arg.shortname + string("/ ") + arg.longname +
                    string(" is Required but not supplied"));
    }
}

Argparse::argument Argparse::getArg(string token) {
    if (token[0] == '-' && token[1] == '-') {
        int position = hasTokenWithLongName(std::regex_replace(token, std::regex("--"), std::string("")));
        if (position > 0) {
            return kwargs[position - 1];
        } else
            return {};
    } else if (token[0] == '-') {
        int position = hasTokenWithShortName(std::regex_replace(token, std::regex("-"), std::string("")));
        if (position > 0) {
            return kwargs[position - 1];
        } else
            return {};
    } else {
        int position = hasPositionalWithName(token);
        if (position != 0) {
            return pargs[position - 1];
        } else return {};
    }
}

void Argparse::spewErrors() {
    if (!Header.empty()) cout << Header << endl;
    cout << argv[0] << " :\t" << slogan << flush << endl;
    for (auto &error : errors)
        cerr << error << flush << endl;
    if (!Footer.empty())cout << Footer << flush << endl;
    exit(0);
}
