#include <bits/stdc++.h>
using namespace std;

class DictionaryEntry {
public:
    string word;
    string meaning;
    vector<string> synonyms;
    vector<string> antonyms;

    DictionaryEntry(string w, string m, vector<string> syns = {}, vector<string> ants = {})
        : word(w), meaning(m), synonyms(syns), antonyms(ants) {}
};

class DictionaryTree {
private:
    struct Node {
        DictionaryEntry entry;
        Node* left;
        Node* right;

        Node(DictionaryEntry e) : entry(e), left(nullptr), right(nullptr) {}
    };

    Node* root;

    Node* insertRecursive(Node* node, DictionaryEntry entry) {
        if (node == nullptr) {
            return new Node(entry);
        }
        if (entry.word < node->entry.word) {
            node->left = insertRecursive(node->left, entry);
        } else if (entry.word > node->entry.word) {
            node->right = insertRecursive(node->right, entry);
        }
        return node;
    }




    DictionaryEntry* searchRecursive(Node* node, const string& word) const {
        if (node == nullptr || node->entry.word == word) {
            return node ? &node->entry : nullptr;
        }
        if (word < node->entry.word) {
            return searchRecursive(node->left, word);
        } else {
            return searchRecursive(node->right, word);
        }
    }

    Node* deleteRecursive(Node* node, const string& word) {
        if (node == nullptr) {
            return nullptr;
        }

        if (word < node->entry.word) {
            node->left = deleteRecursive(node->left, word);
        } else if (word > node->entry.word) {
            node->right = deleteRecursive(node->right, word);
        } else {
            // Node found
            if (node->left == nullptr) {
                Node* temp = node->right;
                delete node;
                return temp;
            } else if (node->right == nullptr) {
                Node* temp = node->left;
                delete node;
                return temp;
            }

            // Node with two children: Get the inorder successor (smallest in the right subtree)
            Node* successor = node->right;
            while (successor && successor->left) {
                successor = successor->left;
            }
            node->entry = successor->entry; // Copy the inorder successor's content to this node
            node->right = deleteRecursive(node->right, successor->entry.word); // Delete the inorder successor
        }
        return node;
    }

public:
    DictionaryTree() : root(nullptr) {}

    void insert(DictionaryEntry entry) {
        root = insertRecursive(root, entry);
    }

    DictionaryEntry* search(const string& word) const {
        return searchRecursive(root, word);
    }

    void deleteWord(const string& word) {
        root = deleteRecursive(root, word);
    }

    void insertFromFile(const string& filename) {
        ifstream file(filename);
        if (!file) {
            cerr << "Error opening file: " << filename << endl;
            return;
        }
        
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string word, meaning, synonymsStr, antonymsStr;
            vector<string> synonyms, antonyms;

            getline(ss, word, ',');
            getline(ss, meaning, ',');
            getline(ss, synonymsStr, ',');
            getline(ss, antonymsStr, ',');

            // Split the synonyms and antonyms by '|'
            stringstream synStream(synonymsStr);
            stringstream antStream(antonymsStr);
            string item;

            while (getline(synStream, item, '|')) {
                synonyms.push_back(item);
            }
            while (getline(antStream, item, '|')) {
                antonyms.push_back(item);
            }

            insert(DictionaryEntry(word, meaning, synonyms, antonyms));
        }
    }

    // Add entry manually and save to file
    void addEntryManually(const string& filename) {
        string word, meaning, synonymsStr, antonymsStr;
        vector<string> synonyms, antonyms;

        cout << "Enter word: ";
        cin.ignore();
        getline(cin, word);
        cout << "Enter meaning: ";
        getline(cin, meaning);
        cout << "Enter synonyms (separate by '|'): ";
        getline(cin, synonymsStr);
        cout << "Enter antonyms (separate by '|'): ";
        getline(cin, antonymsStr);

        // Process synonyms
        stringstream synStream(synonymsStr);
        stringstream antStream(antonymsStr);
        string item;

        while (getline(synStream, item, '|')) {
            synonyms.push_back(item);
        }
        while (getline(antStream, item, '|')) {
            antonyms.push_back(item);
        }

        DictionaryEntry newEntry(word, meaning, synonyms, antonyms);
        insert(newEntry);

        // Append the new word to the file
        ofstream file(filename, ios::app);
        if (!file) {
            cerr << "Error opening file for writing: " << filename << endl;
            return;
        }

        file << word << "," << meaning << ",";        
        for (size_t i = 0; i < synonyms.size(); ++i) {
            file << synonyms[i];
            if (i != synonyms.size() - 1) file << '|';
        }
        file << ",";        
        for (size_t i = 0; i < antonyms.size(); ++i) {
            file << antonyms[i];
            if (i != antonyms.size() - 1) file << '|';
        }
        file << endl;

        cout << "Entry added and saved to file successfully!" << endl;
    }

    void saveToFile(const string& filename) {
        ofstream file(filename);
        if (!file) {
            cerr << "Error opening file for writing: " << filename << endl;
            return;
        }
        file << "All entries have been saved (not implemented)." << endl;
    }
};

int main() {
    DictionaryTree dictionary;

    // Load words from the text file
    dictionary.insertFromFile("dictionary.txt");

    int choice;
    while (true) {
        // Display menu
        cout << "\nDictionary Menu:\n";
        cout << "1. Add a word to the dictionary\n";
        cout << "2. Search for a word in the dictionary\n";
        cout << "3. Delete a word from the dictionary\n";
        cout << "4. Exit\n";
        cout << "Enter your choice (1-4): ";
        cin >> choice;

        if (choice == 1) {
            // Add a new word manually and store in file
            dictionary.addEntryManually("dictionary.txt");
        } 
        else if (choice == 2) {
            // Search for a word
            string searchWord;
            cout << "Enter word to search: ";
            cin >> searchWord;
            DictionaryEntry* entry = dictionary.search(searchWord);
            if (entry) {
                cout << entry->word << ": " << entry->memaning << endl;
                if (!entry->synonyms.empty()) {
                    cout << "Synonyms: ";
                    for (const auto& synonym : entry->synonyms) {
                        cout << synonym << " ";
                    }
                    cout << endl;
                }
                if (!entry->antonyms.empty()) {
                    cout << "Antonyms: ";
                    for (const auto& antonym : entry->antonyms) {
                        cout << antonym << " ";
                    }
                    cout << endl;
                }
            } else {
                cout << "Word not found!" << endl;
            }
        } 
        else if (choice == 3) {
            // Delete a word
            string deleteWord;
            cout << "Enter word to delete: ";
            cin >> deleteWord;
            dictionary.deleteWord(deleteWord);
            cout << "Word deleted successfully!" << endl;
        } 
        else if (choice == 4) {
            cout << "Exiting the program..." << endl;
            break;
        } 
        else {
            cout << "Invalid choice! Please enter a valid option." << endl;
        }
    }

    return 0;
}
