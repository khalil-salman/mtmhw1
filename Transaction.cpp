#pragma once

#include <string>
#include <fstream>
#include <Utilities.h>
#include "Transaction.h"
using std::endl;
using std::ofstream;

void TransactionDumpInfo(const Transaction& transaction, ofstream& file){
    if (file.is_open())
        file << "Sender Name: " << transaction.sender << endl;
    file << "Receiver Name: " << transaction.receiver << endl;
    file << "Transaction Value: " << transaction.value << endl;
}

string TransactionHashedMessage(const Transaction& transaction){
    return hash(transaction.value,transaction.sender,transaction.receiver);
}
bool TransactionVerifyHashedMessage(
        const Transaction& transaction,
        string hashedMessage
){
    return  TransactionHashedMessage(transaction) == hashedMessage;
}