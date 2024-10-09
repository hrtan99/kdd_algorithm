#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// FP-Tree Node structure
struct FPTreeNode {
    string item;
    int count;
    FPTreeNode* parent;
    map<string, FPTreeNode*> children;

    // Constructor for FP-Tree Node
    FPTreeNode(string item, FPTreeNode* parent = nullptr) : item(item), count(1), parent(parent) {}
};

// Function to update the FP-Tree
void updateTree(FPTreeNode* root, const vector<string>& transaction, map<string, FPTreeNode*>& headerTable) {
    FPTreeNode* currentNode = root;
    for (const string& item : transaction) {
        if (currentNode->children.find(item) != currentNode->children.end()) {
            currentNode->children[item]->count++;
        } else {
            FPTreeNode* newNode = new FPTreeNode(item, currentNode);
            currentNode->children[item] = newNode;

            if (headerTable.find(item) == headerTable.end()) {
                headerTable[item] = newNode;
            } else {
                FPTreeNode* node = headerTable[item];
                while (node->children.find(item) != node->children.end()) {
                    node = node->children[item];
                }
                node->children[item] = newNode;
            }
        }
        currentNode = currentNode->children[item];
    }
}

// Function to sort items in a transaction by their frequency in the header table
vector<string> sortByHeaderTable(const vector<string>& transaction, const map<string, int>& frequencyTable) {
    vector<pair<string, int>> itemFreqs;
    for (const string& item : transaction) {
        if (frequencyTable.find(item) != frequencyTable.end()) {
            itemFreqs.push_back(make_pair(item, frequencyTable.at(item)));
        }
    }

    sort(itemFreqs.begin(), itemFreqs.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
        return a.second > b.second;
    });

    vector<string> sortedItems;
    for (const auto& pair : itemFreqs) {
        sortedItems.push_back(pair.first);
    }
    return sortedItems;
}

// Function to get conditional pattern base for a given item
vector<vector<string>> getConditionalPatternBase(FPTreeNode* node) {
    vector<vector<string>> patternBase;
    while (node) {
        vector<string> prefixPath;
        FPTreeNode* currentNode = node->parent;
        while (currentNode && currentNode->item != "null") {
            prefixPath.push_back(currentNode->item);
            currentNode = currentNode->parent;
        }
        if (!prefixPath.empty()) {
            patternBase.push_back(prefixPath);
        }
        // Move to the next node in the linked list of nodes with the same item
        node = node->children.empty() ? nullptr : node->children.begin()->second;
    }
    return patternBase;
}

// Function to mine FP-tree recursively
void mineFPTree(FPTreeNode* root, map<string, int>& frequencyTable, map<string, FPTreeNode*>& headerTable, int minSupport, const vector<string>& suffix) {
    for (auto it = headerTable.rbegin(); it != headerTable.rend(); ++it) {
        string item = it->first;
        FPTreeNode* node = it->second;

        vector<string> newSuffix = suffix;
        newSuffix.push_back(item);

        // Output the frequent itemset
        cout << "Frequent Itemset: {";
        for (size_t i = 0; i < newSuffix.size(); ++i) {
            if (i > 0) cout << ", ";
            cout << newSuffix[i];
        }
        cout << "}, Support: " << node->count << endl;

        // Get conditional pattern base for the current item
        vector<vector<string>> conditionalPatternBase = getConditionalPatternBase(node);

        if (conditionalPatternBase.empty()) continue;

        // Build conditional FP-tree from the conditional pattern base
        map<string, int> conditionalFreqTable;
        for (const auto& pattern : conditionalPatternBase) {
            for (const string& item : pattern) {
                conditionalFreqTable[item]++;
            }
        }

        // Remove infrequent items
        for (auto it = conditionalFreqTable.begin(); it != conditionalFreqTable.end();) {
            if (it->second < minSupport) {
                it = conditionalFreqTable.erase(it);
            } else {
                ++it;
            }
        }

        if (conditionalFreqTable.empty()) continue;

        // Build the conditional FP-tree
        FPTreeNode* conditionalRoot = new FPTreeNode("null");
        map<string, FPTreeNode*> conditionalHeaderTable;
        for (const auto& pattern : conditionalPatternBase) {
            vector<string> sortedPattern = sortByHeaderTable(pattern, conditionalFreqTable);
            updateTree(conditionalRoot, sortedPattern, conditionalHeaderTable);
        }

        // Recursive mining on the conditional FP-tree
        mineFPTree(conditionalRoot, conditionalFreqTable, conditionalHeaderTable, minSupport, newSuffix);
    }
}

int main() {
    // Example data: each vector represents a transaction
    // vector<vector<string>> transactions = {
    //     {"bread", "milk"},
    //     {"bread", "diaper", "beer", "egg"},
    //     {"milk", "diaper", "beer", "cola"},
    //     {"bread", "milk", "diaper", "beer"},
    //     {"bread", "milk", "diaper", "cola"}
    // };
    vector<vector<string>> transactions = {
        {"a", "b", "c", "d", "e", "f", "g", "h"},
        {"a", "f", "g"},
        {"b", "d", "e", "f", "j"},
        {"a", "b", "d", "i", "k"},
        {"a", "b", "e", "g"}
    };

    int minSupport = 2;

    // Step 1: Calculate frequency of each item
    map<string, int> frequencyTable;
    for (const auto& transaction : transactions) {
        for (const string& item : transaction) {
            frequencyTable[item]++;
        }
    }

    // Step 2: Remove items that do not meet minSupport
    for (auto it = frequencyTable.begin(); it != frequencyTable.end();) {
        if (it->second < minSupport) {
            it = frequencyTable.erase(it);
        } else {
            ++it;
        }
    }

    // Step 3: Build FP-Tree
    FPTreeNode* root = new FPTreeNode("null");
    map<string, FPTreeNode*> headerTable;
    for (auto& transaction : transactions) {
        vector<string> sortedTransaction = sortByHeaderTable(transaction, frequencyTable);
        updateTree(root, sortedTransaction, headerTable);
    }

    // Step 4: Mine frequent patterns from the FP-Tree
    vector<string> suffix;
    mineFPTree(root, frequencyTable, headerTable, minSupport, suffix);

    return 0;
}