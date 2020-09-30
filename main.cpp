#include <iostream>
#include "Assembler/Assembler.h"
using namespace std;
int main() {
    Assembler a;
    a.Assemble_file("did.asm");
    for(auto b:a.label2addr)
    cout<<b.first<<" : "<<hex<<b.second<<endl;
    cout<<"++++++++++++++++++++++++++++++++++++++++++++++"<<endl;\
    int c=0;
    for(auto f:a.assembled)
    {
        c++;
        cout<<hex<<(int)f<<" "<<((c%10==0)?"\n":"  ");

    }
    for(auto e:a.parser.errorlist)
    {
        cerr<<e.msg<<endl<<e.line_no<<" : "<<e.line<<endl;
    }
}
