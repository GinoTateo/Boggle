#include "Dictionary.h"

Dictionary::Dictionary() {      // Constructor
    root = createNode();
    numWords = 0;
    root->isWord = false;
}

Dictionary::~Dictionary() {

}

Dictionary::Dictionary(const Dictionary &otherDict) {

}

Dictionary::Dictionary(string filename) {
    root = createNode();                    // New Node
    numWords = 0;                           // Book Keeping
    root->isWord = false;                   //
    LoadDictionaryFile(filename);           // Call to Load with filename
}

Dictionary &Dictionary::operator=(const Dictionary &otherDict) {

    MakeEmpty();        // Make Empty

    copyHelper(root , otherDict.root,"");       // Call the copier
    numWords = otherDict.numWords;                                    // Book keeping

    return *this;
}

void Dictionary::LoadDictionaryFile(string filename) {

    // Load & Open filename.
    ifstream file;
    file.open(filename);

    // Error handling if file dont open
    if (file.fail()) throw filename+ " failed to open";

    // Else Getline and AddWord
    else {
        string line;
        while ( getline (file,line) )
        {
            AddWord(line);
        }
        file.close();
    }
}

void Dictionary::SaveDictionaryFile(string filename) {

    // Load file up
    ofstream save(filename);

    // Error handling if it dont open
    if (save.fail()) { throw filename+ " failed to open";}

    // Call helper function; Recursive function
        SaveDictionaryHelper(root,"", save);
}

void Dictionary::AddWord(string word) {
    insertRecursively(root, word, 0);
}

// Function will insert the string in a trie recursively
void Dictionary::insertRecursively(Dictionary::Node *thisTree, string str, int i)
{
    if(i < str.length())
    {
        int index = str[i] - 'a';
        if(index>26||index<0){ throw DictionaryError("Invalid character"); }
        if(thisTree->trie[index] == NULL )
        {
            // Create a new node
            thisTree->trie[index] = createNode();
        }

        // Recursive call for insertion of string
        insertRecursively(thisTree->trie[index], str, i + 1);
    }
    else
    {
        // Make the endOfWord true which represents
        // the end of string
        thisTree->isWord = true;
        numWords++;
    }
}

void Dictionary::MakeEmpty() {
    destroyHelper(root); // Calls recursive helper starting with root
    numWords = 0;                // Set number of words to 0
}

bool Dictionary::IsWord(string word) {
    Node* temp = root;          // Temp pointer at root

    for(int i=0; word[i]!='\0'; i++){                    // Loop if the char is not NULL
        int position = word[i] - 'a';
        if (temp->trie[position] == NULL)                // If trie points to somthing
            return false;
        temp = temp->trie[position];                     // Keep moving
    }
    if (temp != NULL && temp->isWord == 1)                  // IsWord true or false?
        return true;
    return false;
}

bool Dictionary::IsPrefix(string word) {

    Node *temp = root;                              // Create temp node and point it to root

    for (int i = 0; word[i] != '\0'; i++) {         // Loop if the char is not NULL
        int position = word[i] - 'a';               // Positioner
        if (temp->trie[position] == NULL)           // If temp doesnt point
            return false;
        temp = temp->trie[position];
    }
    if (temp != NULL )
        return true;
    return false;
}

int Dictionary::WordCount() { // returns numwords; numwords increases with addword
    return numWords;
}

void Dictionary::copyOther(const Dictionary &otherDict) {   // Nothing
   // numWords = otherDict.numWords;
}


void Dictionary::destroyHelper(Dictionary::Node *thisTree) {

    for (int i = 0; i < 26; i++) {                                  // 26 Chars
        if (thisTree->trie[i] != NULL) {                            // If it points
            for (int x = 0; x < 26; x++) {                          // 26 Chars
                if (thisTree->trie[i]->trie[x] != NULL) {           // If it points
                    destroyHelper(thisTree->trie[i]);       // Recursion Recall

                }
                thisTree->trie[i]->trie[x] = NULL;      // Set to NULL just in case.
                delete thisTree->trie[i]->trie[x];      // Deletes the trie pointing to the trie were looking at.
            }
        }
    }
}

void Dictionary::copyHelper(Dictionary::Node *&thisTree, Dictionary::Node *otherTree, string currPrefix) {
    if (otherTree == NULL) {    // If the tree we are copying from is NULL
        return;
    }
    if (otherTree->isWord){                                         // Is there a word here?
        insertRecursively(thisTree, currPrefix, 0);             // Yes? Then add it to the other list via recursion function
    }
    for (int i = 0; i < NUM_CHARS; i++){                                // 26 chars to loop through

        if (otherTree->trie[i]) {
            currPrefix += 'a' + i;
            copyHelper(thisTree,otherTree->trie[i],currPrefix);
            currPrefix.pop_back();
        }

    }

}

void Dictionary::SaveDictionaryHelper(Dictionary::Node *curr, string currPrefix, ofstream &outFile) {

    if (curr == NULL) {
        return;
    }
    if (curr->isWord){
        outFile << currPrefix << endl;
    }

    for (int i = 0; i < NUM_CHARS; i++){

            if (curr->trie[i]) {
                currPrefix += 'a' + i;
                SaveDictionaryHelper(curr->trie[i], currPrefix, outFile);
                currPrefix.pop_back();
            }
    }

}
