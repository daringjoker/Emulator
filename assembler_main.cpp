#include <iostream>
#include "Assembler/Assembler.h"
using namespace std;
int main(int argc,char ** argv) {
    Assembler a;
    cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
    cout<<"|                            8085 Assembler                         |"<<endl;
    cout<<"| V(0.1)sept 2020                                   -BY PRS Company |"<<endl;
    cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;

    if(argc<2)
    {
        printf("Usage %s filename",argv[0]);
        return 0;
    }
    a.Assemble_file(string(argv[1]));
    if(a.parser.errorlist.size()==0) {
        //cout << "\n\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "                       Labels Map of Executable                      \n" << endl;
        for (auto b:a.label2addr)
            printf("\t%4x : %s\n", b.second, b.first.c_str());
        cout << "\n\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "                       Memory Map of Executable                      " << endl << endl;
        int c = 0;
        printf("\t%4x : ", c + a.baseaddr);
        for (auto f:a.assembled) {
            c++;
            printf("%02x", f);
            if (c % 16 == 0) {
                if (a.assembled.size() > c)
                    printf("\n\t%4x : ", c + a.baseaddr);
            }
            else
                printf(" ");
        }
    }
    else {
        //cout << "\n\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "                         List of all Errors                         " << endl << endl;
        for (auto e:a.parser.errorlist) {
            if (e.line != string(""))
                cerr << "Error On line " << e.line_no << " : " << e.line << endl << "\t" << e.msg << endl<<endl;
            else
                cerr << "Error On line " << e.line_no << " : " << e.msg << endl<<endl;
        }
        cout << "\n\n              Please solve above errors and retry                  " << endl;

    }
    cout << "\n\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "+++++++++++++++++++        Happy Learning          ++++++++++++++++++" << endl;
    cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
}
