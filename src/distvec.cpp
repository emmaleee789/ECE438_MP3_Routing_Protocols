#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>

using namespace std;

#define INTMAX 2147483647

#define DEBUGtopo 0
#define DEBUGgraph 0
#define DEBUGvec 0
#define DEBUGmsg 0
#define DEBUGbellman 0
#define DEBUGbellLogic 0
#define DEBUGout 0
#define DEBUGchanges 0

class State{
public:
    int id;
    int distFromStart;
    int distBackUp;
    int nodeToHopFromStart;
    vector<int> path;
    State(){
        this->id = 0;
        this->distFromStart = INTMAX;
        this->distBackUp = INTMAX;
        this->nodeToHopFromStart = 0;
        this->path = vector<int>();
    };
    State(int id, int distFromStart, int distBackUp, int nodeToHopFromStart, vector<int> path){
        this->id = id;
        this->distFromStart = distFromStart;
        this->distBackUp = distBackUp;
        this->nodeToHopFromStart = nodeToHopFromStart;
        this->path = path;
    }
};

int counter = 1;
int numVertices;
int numSize;
ofstream outfile;
FILE *fpOut;
vector<vector<int>> graph;
vector<vector<int>> msg;
vector<vector<string>> msgWords;
// vector<State> stateTo;
vector<vector<State>> stateToVec;

struct compare {
    bool operator()(State a, State b) {
        return  a.distFromStart > b.distFromStart; 
    }
};


void log_print(string log){
    printf(log.c_str());
}

void print_topo(int start, vector<State> stateTo){ //this one should be fixed: 1. not print; 2. logic issue
    /* prints topology entries */
    
    for (int destId=1; destId<=numVertices; destId++){ //
        string a, b, c;
        if (start == destId){
            a = to_string(start);
            b = to_string(start);
            c = to_string(0);
        }
        else{
            a = to_string(destId);
            b = to_string(stateTo[destId].nodeToHopFromStart);
            c = to_string(stateTo[destId].distFromStart);
        }

        string outStr = a + " " + b + " " + c + " \n";
        fwrite(outStr.c_str(), sizeof(char), strlen(outStr.c_str()), fpOut); //为什么 sizeof(outStr)/sizeof(char或者)sizeof(outStr)不行？


        #if DEBUGout

        cout<<"a: "<<a<<"; b: "<<b<<"; c: "<<c<<endl;

        #endif
    }
}

void print_msg(int start, vector<State> stateTo){
    /* prints message output lines */
    int sz = msg[start].size();
    if (sz == 0) return;
    else{
        for (int i=0; i<sz; i++){
            int destId = msg[start][i];
            int szHops = stateTo[destId].path.size();
            string outStr = "from " + to_string(start) + " to " + to_string(destId) + " cost " + to_string(szHops) + " hops ";
            for (int id : stateTo[destId].path){
                outStr = outStr + to_string(id) + " ";
            }
            outStr = outStr + "message " + msgWords[start][i] + "\n";
            fwrite(outStr.c_str(), sizeof(char), strlen(outStr.c_str()), fpOut);
        }
    }
}

void backUpDist(int start){
    for (int id=1; id<=numVertices; id++){
        stateToVec[start][id].distBackUp = stateToVec[start][id].distFromStart; // avoid more relax op
    }
}

void BellmanFord(int start){
    // vector<int> distTo(numVertices, INTMAX);
    // vector<int> startHopTo(numVertices, 0);
    // vector<State> stateTo(numSize); //every new start node, a new stateTo
    /* stateTo == stateToVec[start] */
    vector<int> path;
    stateToVec[start][start].id = start;
    stateToVec[start][start].distFromStart = 0;
    stateToVec[start][start].distBackUp = 0;
    stateToVec[start][start].nodeToHopFromStart = 0;
    stateToVec[start][start].path = vector<int>();

    for (int i=0; i<numVertices - 1; i++){ // iterates maximum length times

        backUpDist(start);

        #if DEBUGbellLogic

        if (start == 1)
            cout<<"###"<<i+1<<" iteration"<<endl<<endl<<endl;

        #endif

        for (int startId=1; startId<=numVertices; startId++){ // iterates all edges
            for (int endId=1; endId<=numVertices; endId++){
                if (graph[startId][endId] == -999) continue; // no edge

                int costToAdj = stateToVec[start][startId].distBackUp + graph[startId][endId];

                #if DEBUGbellLogic

                if ((start == 1) ){//|| (startId == 2 && start == 1)){
                    cout<<"startId: "<<startId<<endl;
                    cout<<"endId: "<<endId<<endl;
                    cout<<"costToAdj: "<<costToAdj<<endl;
                    cout<<"startId.distBackUp: "<<stateToVec[start][startId].distBackUp<<endl;
                    cout<<"graph[curState.id][endId]: "<<graph[startId][endId]<<endl;
                    // cout<<"stateToVec[start][endId].distFromStart: "<<stateToVec[start][endId].distFromStart<<endl;
                }

                #endif

                if (costToAdj < stateToVec[start][endId].distFromStart && stateToVec[start][startId].distBackUp != INTMAX){

                    stateToVec[start][endId].id = endId; // update id

                    stateToVec[start][endId].distFromStart = costToAdj; //update distTo

                    path = stateToVec[start][startId].path;
                    path.push_back(startId);
                    stateToVec[start][endId].path = path; //update pathTo

                    if (startId == start){ //update nodeToHopFromStart
                        stateToVec[start][endId].nodeToHopFromStart = endId;
                    }
                    else{
                        stateToVec[start][endId].nodeToHopFromStart = stateToVec[start][startId].nodeToHopFromStart;
                    }

                }
                //relax在 bellman 是按照顺序的，所以不需要考虑第一种、第三种 tie 的情况
                #if DEBUGbellLogic
                if ((start == 1) ){ //startId == 1 && 
                    cout<<"stateToVec[start][endId].distFromStart: "<<stateToVec[start][endId].distFromStart<<endl;
                    cout<<endl;

                }
                #endif
            }
        }
        #if DEBUGbellLogic
        if (start == 1){
            for (int id=1; id<=numVertices; id++){
                cout<<stateToVec[start][id].distFromStart<<endl; // avoid more relax op
            }
        }
        cout<<endl;
        #endif
    }

    #if DEBUGbellman
    if (start == 1){
        cout<<"##Print the "<<counter<<" time"<<endl;
        counter++;
    }
    cout<<endl<<"start id: "<<start<<endl<<endl;
    for (int j=1; j<=numVertices; j++){
        cout<<"stateTo["<<j<<"]: "<<endl;
        cout<<"id: "<<stateToVec[start][j].id<<endl;
        cout<<"distFromStart: "<<stateToVec[start][j].distFromStart<<endl;
        cout<<"nodeToHopFromStart: "<<stateToVec[start][j].nodeToHopFromStart<<endl;
        cout<<"path: ";
        for (int k=0; k<stateToVec[start][j].path.size(); k++){
            cout<<stateToVec[start][j].path[k]<<" ";
        }
        cout<<endl;

    }

    #endif

}


int main(int argc, char** argv) {
    //printf("Number of arguments: %d", argc);
    if (argc != 4) {
        printf("Usage: ./linkstate topofile messagefile changesfile\n");
        return -1;
    }

    fpOut = fopen("output.txt", "w");
    // outfile.open("output.txt", ios::out);//ios::binary

    ifstream topoFile, messageFile, changesFile;
    topoFile.open(argv[1], ios::in);
    messageFile.open(argv[2], ios::in);
    changesFile.open(argv[3], ios::in);


    if (!topoFile.is_open() || !messageFile.is_open() || !changesFile.is_open()){
        log_print("Failed to open topofile or messageFile or changesFile");
    }

    
    /* calculates the number of vertices */
    string buf;
    numVertices = 0;
    while(getline(topoFile, buf)){
        if (atoi(&buf[0]) > numVertices) numVertices = atoi(&buf[0]);
        if (atoi(&buf[2]) > numVertices) numVertices = atoi(&buf[2]);
    }
    topoFile.close();

    /* deals with topology */
    int idx = 0;
    vector<int> tmp;

    numSize = numVertices + 1;
    graph.resize(numSize, vector<int>(numSize, -999)); 
    // stateTo.resize(numSize);
    stateToVec.resize(numSize, vector<State>(numSize));
    vector<vector<State>> tmpStateToVec1(numSize, vector<State>(numSize));
    vector<vector<State>> tmpStateToVec2(numSize, vector<State>(numSize));

    topoFile.open(argv[1], ios::in);
    while(getline(topoFile, buf)){
        graph[atoi(&buf[0])][atoi(&buf[2])] = atoi(&buf[4]);
        graph[atoi(&buf[2])][atoi(&buf[0])] = atoi(&buf[4]);

    }

    topoFile.close();

    #if DEBUGgraph

    cout<<"graph: "<<endl;
    for (int i=0; i<numSize; i++){
        for (int j=0; j<numSize; j++)
            cout<<graph[i][j]<<" ";
        cout<<endl;
    }

    #endif

    /* deals with messages */
    msg.resize(numSize, vector<int>());
    msgWords.resize(numSize, vector<string>());
    idx = 0;
    vector<string> temp;
    while(getline(messageFile, buf)){
        if (atoi(&buf[0]) != idx){
            idx++;
            while (atoi(&buf[0]) > idx){
                msg.push_back(tmp);
                msgWords.push_back(temp);
                idx++;
            }
        }
        msg[idx].push_back(atoi(&buf[2]));
        string str = buf;
        int sz = str.size();
        str = str.substr(4, sz-4); 
        msgWords[idx].push_back(str);

    }
    messageFile.close();


    /* calls Bellman Ford to generate routing table for each node & output.txt */
    for (int i=1; i<=numVertices; i++){
        BellmanFord(i); //now "stateTo" would be refreshed, since a new start occurs
        print_topo(i, stateToVec[i]);
    }
    for (int i=1; i<=numVertices; i++){
        if (!msg[i].empty()){
            print_msg(i, stateToVec[i]);
        }
    }
    stateToVec = tmpStateToVec1;

    /* second part: deals with changesfile */
    while(getline(changesFile, buf)){
        if ((int)atoi(&buf[4]) == -999){
            graph[atoi(&buf[0])][atoi(&buf[2])] = -999;
            graph[atoi(&buf[2])][atoi(&buf[0])] = -999;
        }
        else{
            graph[atoi(&buf[0])][atoi(&buf[2])] = atoi(&buf[4]);
            graph[atoi(&buf[2])][atoi(&buf[0])] = atoi(&buf[4]);
        }
        for (int i=1; i<=numVertices; i++){
            BellmanFord(i); //now "stateTo" would be refreshed, since a new start occurs
            print_topo(i, stateToVec[i]);
        }
        for (int i=1; i<=numVertices; i++){
            if (!msg[i].empty()){
                print_msg(i, stateToVec[i]);
            }
        }
        stateToVec = tmpStateToVec2;

        #if DEBUGchanges

        cout<<"buf: "<<buf<<endl;
        cout<<"changed graph: "<<endl;
        for (int i=0; i<numSize; i++){
            for (int j=0; j<numSize; j++)
                cout<<graph[i][j]<<" ";
            cout<<endl;
        }

        #endif

    }
    changesFile.close();

    fclose(fpOut);


    return 0;
}

