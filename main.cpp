
#include <iostream>
#include <iostream>
#include <fstream>
#include <string>
#include "BlockChain.h"
#include "Transaction.h"
#include "Utilities.h"
using std :: endl ;
using std :: cerr;
using std :: cout;
void free_Chain(BlockChain &blockChain) {
    Block* cur = blockChain.head;
    while (cur != nullptr) {
        Block* toDelete = cur;
        cur = cur->next;
        delete toDelete;
    }
}
int main(int argc,char** argv) {
    if (argc != 4){
        cout << getErrorMessage() <<endl;
        return 0;
    }
    string op = argv[1];

    if(op == "format" ){
        ifstream in_file(argv[2]);
        ofstream out_file(argv[3]);
        BlockChain blockChain = BlockChainLoad(in_file);
        BlockChainDump(blockChain,out_file);
        free_Chain(blockChain);
        //delet block chain
    }
    else if(op == "hash"){
        ifstream in_file(argv[2]);
        ofstream out_file(argv[3]);
        BlockChain blockChain = BlockChainLoad(in_file);
        BlockChainDumpHashed(blockChain,out_file);
        free_Chain(blockChain);
        //delet block chain
    }
    else if(op == "compress"){
        ifstream in_file(argv[2]);
        ofstream out_file(argv[3]);
        BlockChain blockChain = BlockChainLoad(in_file);
        BlockChainCompress(blockChain);
        BlockChainDump(blockChain,out_file);
        free_Chain(blockChain);
        //you know the drill
    }
    else if(op == "verify"){
        ifstream in_file(argv[2]);
        ifstream out_file(argv[3]);
        BlockChain blockChain = BlockChainLoad(in_file);
        bool ver = BlockChainVerifyFile(blockChain, out_file);
        if(ver){
            cout << "Verification passed" << endl;
        }
        else{
            cout << "Verification failed" << endl;
        }
        free_Chain(blockChain);
    }
    else {
        cout << getErrorMessage() << endl;
    }
}