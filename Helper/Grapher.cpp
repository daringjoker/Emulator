//
// Created by DaringJoker on 11/24/2020.
//

#include "Grapher.h"

void Grapher::addCedge(Word from, Word to) {
    edges.push_back({from,to});
    edges.push_back({from,static_cast<Word>(from+3)});
    edges.push_back({to-1,to});
    blockbreaks.insert(to);
    blockbreaks.insert(from+3);
}

void Grapher::addedge(Word from, Word to) {
    edges.push_back({from,to});
    remedges.push_back({from+2,from+3});
    blockbreaks.insert(to);
    blockbreaks.insert(from+3);
}

void Grapher::makeblocks(int codesize,Word baseaddr) {
    Word block_begin=baseaddr;
    for(auto& begin_address:blockbreaks)
    {
        codeblock cb;
        cb.from=block_begin;
        cb.to=begin_address;
        codeblocks.push_back(cb);
        block_begin=begin_address;
        states.push_back(notvisited);
        vector<int> adjs;
        graph.push_back(adjs);
        revgraph.push_back(adjs);
    }
    codeblock cb;
    cb.from=block_begin;
    cb.to=baseaddr+codesize;
    codeblocks.push_back(cb);
    states.push_back(notvisited);
    vector<int> adjs;
    graph.push_back(adjs);
    revgraph.push_back(adjs);
    graphs_from_edges();
    toposort_DAC(0);
}

void Grapher::addret(Word at) {
    blockbreaks.insert(at+1);
}

void Grapher::toposort_DAC(int v) {
    for(int w:graph[v])
    {
        if(states[w]==visited)
        {
            DAC_edges.push_back({v,w});
        }
        else if(states[w]==notvisited)
        {
            DAC_edges.push_back({v,w});
            states[w]=pending;
            toposort_DAC(w);
            states[w]=visited;
        }
    }
}

void Grapher::graphs_from_edges() {
    for(auto & edge : edges)
    {
        if(!removed(edge)) {
            int from = in_block(edge.from);
            int to = in_block(edge.to);
            graph[from].push_back(to);
            revgraph[to].push_back(from);
        }
    }
}

int Grapher::in_block(Word addr) {
    for (int i = 0; i < codeblocks.size(); ++i) {
        if(addr>=codeblocks[i].from && addr<codeblocks[i].to)
            return i;
    }
    return 0xfffffff;
}

bool Grapher::removed(edge &edge) {
    for(auto& redge:remedges)
    {
        if(edge.to==redge.to&&edge.from==redge.from)
            return true;
    }
    return false;
}
