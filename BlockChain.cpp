#include <string>
#include <fstream>
#include <iostream>
#include "Transaction.h"
#include "BlockChain.h"
using std::string;
using std::ifstream;
using std::ofstream;
using std::endl;
using std::getline;
using std::cout;

int BlockChainGetSize(const BlockChain &blockChain) {
    int counter = 0;
    Block *current = blockChain.head;
    while (current != nullptr) {
        counter++;
        current = current->next;
    }
    return counter;
}


int BlockChainPersonalBalance(const BlockChain &blockChain, const string &name) {
    int wallet;
    const Block* cur = blockChain.head;
    while (cur != nullptr) {
        if (cur->transaction.receiver == name) {
            wallet = wallet + (int) cur->transaction.value;
        }
        if (cur->transaction.sender == name) {
            wallet = wallet - (int) cur->transaction.value;
        }
        cur = cur->next;
    }
    return wallet;
}

void BlockChainAppendTransaction(
        BlockChain &blockChain,
        unsigned int value,
        const string &sender,
        const string &receiver,
        const string &timestamp
) {
    Transaction transaction = {value,sender,receiver};
    BlockChainAppendTransaction(blockChain,transaction,timestamp);
}


void BlockChainAppendTransaction(
        BlockChain &blockChain,
        const Transaction &transaction,
        const string &timestamp
) {
    Block *add = new Block();
    add->transaction = transaction;
    add->timestamp = timestamp;
    add->next = blockChain.head;
    blockChain.head = add;
}



BlockChain BlockChainLoad(std::ifstream& file) {
    BlockChain chain;
    Block* head = nullptr;
    Block* tail = nullptr;

    string sender, receiver, timestamp;
    unsigned int value;

    while (file >> sender >> receiver >> value >> timestamp) {
        Block* newBlock = new Block;
        newBlock->transaction.sender = sender;
        newBlock->transaction.receiver = receiver;
        newBlock->transaction.value = value;
        newBlock->timestamp = timestamp;
        newBlock->next = nullptr;

        if (!head) {
            head = tail = newBlock;
        } else {
            tail->next = newBlock;
            tail = newBlock;
        }
    }

    chain.head = head;
    return chain;
}


void BlockChainDump(const BlockChain& blockChain,ofstream& file) {
    int transaction_num = 0;
    Block* to_show = blockChain.head;

    file << "BlockChain Info:" << endl;

    while (to_show != nullptr) {
        transaction_num++;
        file << transaction_num << "." << endl;
        TransactionDumpInfo( to_show->transaction, file);
        file << "Transaction timestamp: " << to_show->timestamp << endl;
        to_show = to_show->next;
    }
}


void BlockChainDumpHashed(const BlockChain &blockChain, ofstream &file) {
    string hash;
    Block* cur = blockChain.head;
    while (cur->next != nullptr) {
        hash = TransactionHashedMessage(cur->transaction);
        file << hash << endl;
        cur = cur->next;
    }
    hash = TransactionHashedMessage(cur->transaction);
    file << hash ;

}


bool BlockChainVerifyFile(const BlockChain &blockChain, std::ifstream &file) {
    Block* current = blockChain.head;
    while (current != nullptr && !file.eof()) {
        string s1;
        file >> s1;
        if (s1 != TransactionHashedMessage(current->transaction)) {
            return false;
        }
        current = current->next;
    }
    if( current == nullptr && file .eof()){
        return true;
    }
    return false;
}

void BlockChainCompress(BlockChain& blockChain) {
    Block* current = blockChain.head;

    while (current && current->next) {
        Block* runner = current->next;
        unsigned int totalValue = current->transaction.value;
        string lastTimestamp = current->timestamp;
        while (runner &&
               runner->transaction.sender == current->transaction.sender &&
               runner->transaction.receiver == current->transaction.receiver) {
            totalValue += runner->transaction.value;
            lastTimestamp = current->timestamp;

            Block* toDelete = runner;
            runner = runner->next;
            delete toDelete;
        }

        current->transaction.value = totalValue;
        current->timestamp = lastTimestamp;
        current->next = runner;

        current = runner;
    }
}

void BlockChainTransform(BlockChain &blockChain, updateFunction function) {
    Block *current = blockChain.head;
    while (current != nullptr) {
        current->transaction.value = function(current->transaction.value);
        current = current->next;
    }
}
