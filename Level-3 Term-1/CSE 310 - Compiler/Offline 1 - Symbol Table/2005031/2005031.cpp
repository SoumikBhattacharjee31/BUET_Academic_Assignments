#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

class SymbolInfo {
    // class variables
    string name;
    string type;
    SymbolInfo* nextSymbolInfo;

    public:
    // constructor
    SymbolInfo (string name, string type, SymbolInfo* nextSymbolInfo = NULL) {
        this->name = name;
        this->type = type;
        this->nextSymbolInfo = nextSymbolInfo;
    }

    // destructor
    ~SymbolInfo () {
        if (nextSymbolInfo != NULL)
            delete nextSymbolInfo;
    }

    // getters
    string getName () {
        return name;
    }
    string getType () {
        return type;
    }
    SymbolInfo* getNextSymbolInfo () {
        return nextSymbolInfo;
    }

    // setters
    void setName (string name) {
        this->name = name;
    }
    void setType (string type) {
        this->type = type;
    }
    void setNextSymbolInfo (SymbolInfo* nextSymbolInfo) {
        this->nextSymbolInfo = nextSymbolInfo;
    }

};

class ScopeTable {
    // class variables
    int totalBuckets;
    int childCount;
    SymbolInfo** symbolMap;
    ScopeTable* parentScope;
    string id;

    public:
    // constructor
    ScopeTable (int totalBuckets, ScopeTable* parentScope = NULL) {

        // initialize
        this->totalBuckets = totalBuckets;
        this->childCount = 0;
        this->parentScope = parentScope;
        this->symbolMap = new SymbolInfo* [totalBuckets];

        // assign null to all buckets
        for (int i=0;i<totalBuckets;i++)
            this->symbolMap[i] = NULL;
        
        // initialize id and increment childcount
        if (parentScope != NULL){
            parentScope->childCount += 1;
            this->id = parentScope->id + "." + to_string(parentScope->childCount);
        }
        else
            id = "1";
        
        // output
        cout<<"\tScopeTable# "<<id<<" created"<<endl;
    }

    // destructor
    ~ScopeTable () {
        for (int i=0;i<totalBuckets;i++)
            if(symbolMap[i]!=NULL) {
                delete symbolMap[i];
                symbolMap[i] = NULL;
            }
        delete [] symbolMap;
        cout<<"\tScopeTable# "<<id<<" deleted"<<endl;
    }

    // getters
    SymbolInfo** getSymbolMap () {
        return symbolMap;
    }
    ScopeTable* getParentScope () {
        return parentScope;
    }
    string getId () {
        return id;
    }

    // hash functions
    unsigned long long sdbmHash(string str){
        unsigned long long hash = 0;
        for (int c : str)
            hash = c + (hash << 6) + (hash << 16) - hash;
        return hash;
    }
    int hashFunction (string name) {
        unsigned long long hash = sdbmHash(name) % totalBuckets;
        if (hash < 0)   
            hash += totalBuckets;
        return hash;
    }

    // memeber functions
    bool insertInfo (string name, string type) {

        // initialize
        int index = hashFunction(name);
        int i=1;
        SymbolInfo* prev = NULL;

        // search if already exists
        for (SymbolInfo* x=symbolMap[index]; x!=NULL; prev = x, x=x->getNextSymbolInfo(), i++)
            if (x->getName() == name){
                cout<<"\t\'"<<name<<"\' already exists in the current ScopeTable# "<<id<<endl;
                return false;
            }
        
        // create new symbolInfo
        SymbolInfo* symbolInfo = new SymbolInfo (name, type);

        // if bucket is empty
        if (prev == NULL)
            symbolMap[index] = symbolInfo;
        else
            prev->setNextSymbolInfo(symbolInfo);

        // finalizing function
        cout<<"\tInserted  at position <"<<index+1<<", "<<i<<"> of ScopeTable# "<<id<<endl;
        return true;
    }

    SymbolInfo* lookupInfo (string name) {

        // initialize
        int index = hashFunction(name);
        int i=1;

        // search string
        for (SymbolInfo* x=symbolMap[index]; x!=NULL; x=x->getNextSymbolInfo(), i++)
            if (x->getName() == name){
                cout<<"\t\'"<<name<<"\' found at position <"<<index+1<<", "<<i<<"> of ScopeTable# "<<id<<endl;
                return x;
            }
        return NULL;
    }

    bool deleteInfo (string name) {

        // initialize
        int index = hashFunction(name);
        SymbolInfo* symbolInfo;
        SymbolInfo* prev = NULL;
        bool found = false;
        int i = 1;

        // search
        for (symbolInfo=symbolMap[index]; symbolInfo!=NULL; prev = symbolInfo, symbolInfo=symbolInfo->getNextSymbolInfo(), i++)
            if (symbolInfo->getName() == name){
                found = true;
                break;
            }
        
        // not found
        if (!found){
            cout<<"\tNot found in the current ScopeTable# "<<id<<endl;
            return false;
        }

        // if first index of chain or not
        if (symbolMap[index] == symbolInfo)
            symbolMap[index] = symbolInfo->getNextSymbolInfo();
        else
            prev->setNextSymbolInfo(symbolInfo->getNextSymbolInfo());

        // finalize
        symbolInfo->setNextSymbolInfo(NULL);
        delete symbolInfo;
        cout<<"\tDeleted \'"<<name<<"\' from position <"<<index+1<<", "<<i<<"> of ScopeTable# "<<id<<endl;
        return true;
    }

    void print () {
        cout<<"\tScopeTable# "<<id<<endl;
        for (int i=0; i<totalBuckets; i++){
            cout<<"\t"<<i+1;
            for (SymbolInfo* x=symbolMap[i]; x!=NULL; x=x->getNextSymbolInfo())
                cout<<" --> ("<<x->getName()<<","<<x->getType()<<")";
            cout<<endl;
        }
    }
};

class SymbolTable {
    // member variavle
    ScopeTable* currentTable;
    int totalBuckets;

    public:
    // constructor
    SymbolTable (int totalBuckets) {
        currentTable = new ScopeTable(totalBuckets);
        this->totalBuckets = totalBuckets;
    }

    // destructor
    ~SymbolTable () {
        ScopeTable* parentTable = currentTable->getParentScope();
        while(currentTable != NULL){
            delete currentTable;
            currentTable = parentTable;
            if (currentTable != NULL)
                parentTable = currentTable -> getParentScope();
        }
    }

    // getter
    ScopeTable* getCurrentTable(){
        return currentTable;
    }

    // setter
    void setCurrentTable (ScopeTable* currentTable) {
        this->currentTable = currentTable;
    }

    // member functions
    void enterScope () {
        ScopeTable* scopeTable = new ScopeTable(totalBuckets, currentTable);
        currentTable = scopeTable;
    }

    void exitScope () {

        // if first scope
        if (currentTable->getParentScope() == NULL){
            cout<<"\tScopeTable# "<<currentTable->getId()<<" cannot be deleted"<<endl;
            return;
        }

        // deleting scope
        ScopeTable* temp = currentTable;
        currentTable = currentTable->getParentScope();

        delete temp;
    }

    bool insert (string name, string type) {
        return currentTable->insertInfo(name, type);
    }
    
    bool remove (string name) {
        return currentTable->deleteInfo(name);
    }

    SymbolInfo* lookupInfo (string name) {

        // search symbolinfo in all scope
        for (ScopeTable* scopeTable = currentTable; scopeTable != NULL; scopeTable=scopeTable->getParentScope()) {
            SymbolInfo* symbolInfo = scopeTable->lookupInfo(name);
            if (symbolInfo)
                return symbolInfo;
        }

        // message if symbolinfo is not found
        cout<<"\t\'"<<name<<"\' not found in any of the ScopeTables"<<endl;
        return NULL;
    }

    void printCur () {
        currentTable->print();
    }

    void printAll () {
        for (ScopeTable* scopeTable = currentTable; scopeTable != NULL; scopeTable=scopeTable->getParentScope())
            scopeTable->print();
    }
};

int main(int argc, char* argv[]){

    // input filename from command line
    const char *infileName, *outfileName;
    if (argc == 3) {
        infileName = argv[1];
        outfileName = argv[2];
    }
    else {
        infileName = "input.txt";
        outfileName = "output.txt";
    }

    // input file open
    ifstream file (infileName);
    if (!file.is_open()) {
        cout<< "input file could not be opened"<<endl;
        return 1;
    }

    // output file open
    FILE* fp = freopen(outfileName,"w",stdout);
    if (fp == NULL) {
        cout<<"output file could not be opened"<<endl;
        return 1;
    }

    // variable declaration
    string line, word, func, par1, par2;
    string totalBucket;
    int bucketSize, linecount = 1;

    // getting bucket size
    getline(file,totalBucket);
    try {
    bucketSize = stoi(totalBucket);
    } catch (...) {
        // bucketSize = 10;
        cout<< "Bucket size should be an integer"<<endl;
        return 1;
    }

    // creating symbol table
    SymbolTable symbolTable(bucketSize);

    // operation loop
    while(getline(file, line)){
        cout<<"Cmd "<<linecount++<<": "<<line<<endl;

        // input operation and operands
        istringstream iss (line);
        int i=0;
        while (iss >> word){
            switch (i) {
            case 0:
                func = word;
                break;
            case 1:
                par1 = word;
                break;
            case 2:
                par2 = word;
                break;
            }
            i++;
        }

        // operations
        if (func=="I" && i==3)
            symbolTable.insert(par1, par2);

        else if (func=="L" && i==2)
            symbolTable.lookupInfo(par1);

        else if (func=="D" && i==2)
            symbolTable.remove(par1);

        else if (func=="P" && i==2) {
            if (par1=="A")
                symbolTable.printAll();
            else if (par1=="C")
                symbolTable.printCur();
            else
                cout<<"\tInvalid argument for the command "<<func<<endl;
        }

        else if (func=="S" && i==1)
            symbolTable.enterScope();

        else if (func=="E" && i==1)
            symbolTable.exitScope();

        else if (func=="Q" && i==1)
            break;

        else
            cout<<"\tWrong number of arugments for the command "<<func<<endl;
    }
    fclose (fp);
    file.close();
    return 0;
}