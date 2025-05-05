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
    const BlockChain *current = &blockChain;
    while (current != nullptr) {
        counter++;
        current = current->next;
    }
    return counter;
}


int BlockChainPersonalBalance(const BlockChain &blockChain, const string &name) {
    int wallet;
    const BlockChain *cur = &blockChain;
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
    BlockChain *add = new BlockChain();
    Transaction transaction = {value, sender, receiver};
    add->transaction = transaction;
    add->timestamp = timestamp;
    add->next = &blockChain;
}


void BlockChainAppendTransaction(
        BlockChain &blockChain,
        const Transaction &transaction,
        const string &timestamp
) {
    BlockChain *add = new BlockChain();
    add->transaction = transaction;
    add->timestamp = timestamp;
    add->next = &blockChain;
}



BlockChain BlockChainLoad(std::ifstream& file) {
    BlockChain* head = nullptr; // Start with an empty chain

    string sender, receiver, timestamp;
    unsigned int value;

    while (file >> sender >> receiver >> value >> timestamp) {
      
        BlockChain* newBlock = new BlockChain;
        newBlock->transaction.sender = sender;
        newBlock->transaction.receiver = receiver;
        newBlock->transaction.value = value;
        newBlock->timestamp = timestamp;

      
        newBlock->next = head;
        head = newBlock;
    }

  
    return *head;
}

void BlockChainDump(const BlockChain& blockChain, ofstream& file) {
    int transaction_num = 0;
    const BlockChain* to_show = &blockChain;

    file << "BlockChain Info:" << endl;

    while (to_show != nullptr) {
        transaction_num++;
        file << transaction_num << "." << endl;
        file << "Sender Name: " << to_show->transaction.sender << endl;
        file << "Receiver Name: " << to_show->transaction.receiver << endl;
        file << "Transaction Value: " << to_show->transaction.value << endl;
        file << "Transaction timestamp: " << to_show->timestamp << endl;

        to_show = to_show->next;
    }
}


bool BlockChainVerifyFile(const BlockChain &blockChain, std::ifstream &file) {
    const BlockChain *current = &blockChain;
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

void BlockChainCompress(BlockChain& blockChain){
    BlockChain* current = &blockChain;

    while (current != nullptr) {
        string sender = current->transaction.sender;
        string receiver = current->transaction.receiver;
        unsigned int total_value = current->transaction.value;
        string latest_timestamp = current->timestamp;

        BlockChain* prev = current;
        BlockChain* runner = current->next;


        while (runner != nullptr &&
               runner->transaction.sender == sender &&
               runner->transaction.receiver == receiver) {
            total_value += runner->transaction.value;
            latest_timestamp = runner->timestamp;

            // We will delete later
            prev = runner;
            runner = runner->next;
        }


        if (prev != current) {
            current->transaction.value = total_value;
            current->timestamp = latest_timestamp;


            BlockChain* to_delete = current->next;
            while (to_delete != runner) {
                BlockChain* next = to_delete->next;
                delete to_delete;
                to_delete = next;
            }

            current->next = runner;
        }

        current = current->next;
    }
}

void BlockChainTransform(BlockChain &blockChain, updateFunction function) {
    BlockChain *current = &blockChain;
    while (current != nullptr) {
        current->transaction.value = function(current->transaction.value);
        current = current->next;
    }
}
