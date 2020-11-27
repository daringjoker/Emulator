//
// Created by DaringJoker on 11/24/2020.
//

#ifndef EMULATOR_GRAPHER_H
#define EMULATOR_GRAPHER_H


#include "../Components/constants.h"
#include <vector>
#include <stack>
#include <map>
#include <set>
#include <tuple>

using namespace std;
typedef struct codeblock
{
    Word from;
    Word to;
    Byte no_outgoing_edges;
    Byte no_incoming_edges;
    Byte row;
    Byte column;
    bool reachable=false;
}codeblock;
typedef struct edge
{
    int from;
    int to;
}edge;
class Grapher {
public:
    vector<codeblock> codeblocks;
    vector<edge> edges;
    vector<edge> remedges;
    set<Word> blockbreaks;
    enum blockstate:Byte{visited,notvisited,pending};
    vector<blockstate>states;
    vector<vector<int>> graph;
    vector<vector<int>> revgraph;
    vector<edge> DAC_edges;
    vector<edge> tree_edges;
    vector<int> reversePostorder;
public:
    void addCedge(Word from, Word to);
    void addedge(Word from,Word to);
    void addret(Word at);
    void makeblocks(int codesize,Word baseaddr=0x4000);
    void graphs_from_edges();
    int in_block(Word addr);
    void toposort_DAC(int start);

    bool removed(edge &edge);
};


#endif //EMULATOR_GRAPHER_H
