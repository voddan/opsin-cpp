#include "OpsinRadixTrie.h"


OpsinRadixTrie::OpsinRadixTrie() : rootNode(new OpsinTrieNode(L"", false)) {
}

void OpsinRadixTrie::addToken(const std::wstring &token) {
    int tokenLength = token.length();
    std::wstring remaingStr = token;
    OpsinTrieNode *currentNode = rootNode;
    for (int i = 0; i < tokenLength;) {
        int charsMatched = currentNode->getNumberOfMatchingCharacters(remaingStr, 0);
        remaingStr = remaingStr.substr(charsMatched);
        i += charsMatched;
        currentNode = currentNode->add(remaingStr, charsMatched);
    }
    currentNode->setIsEndPoint(true);
}

std::vector<int> OpsinRadixTrie::findMatches(const std::wstring &chemicalName, int posInName) {
    int untokenisedChemicalNameLength = chemicalName.length();
    std::vector<int> indexes;
    if (rootNode->isEndPoint()) {
        indexes = std::vector<int>();
        indexes.push_back(posInName);
    }
    OpsinTrieNode *node = rootNode;
    for (int i = posInName; i < untokenisedChemicalNameLength; i++) {
        node = node->getChild(chemicalName[i]);
        if (node == nullptr) {
            break;
        }
        int nodeLength = node->getValue().length();
        if (nodeLength > 1) {
            int charsMatched = node->getNumberOfMatchingCharacters(chemicalName, i);
            if (charsMatched != nodeLength) {
                break;
            }
            i += (charsMatched - 1);
        }
        if (node->isEndPoint()) {
            if (indexes.empty()) {
                indexes = std::vector<int>();
            }
            indexes.push_back(i + 1);
        }
    }
    return indexes;
}

std::vector<int> OpsinRadixTrie::findMatchesReadingStringRightToLeft(const std::wstring &chemicalName, int posInName) {
    std::vector<int> indexes;
    if (rootNode->isEndPoint()) {
        indexes = std::vector<int>();
        indexes.push_back(posInName);
    }
    OpsinTrieNode *node = rootNode;
    for (int i = posInName - 1; i >= 0; i--) {
        node = node->getChild(chemicalName[i]);
        if (node == nullptr) {
            break;
        }
        int nodeLength = node->getValue().length();
        if (nodeLength > 1) {
            int charsMatched = node->getNumberOfMatchingCharactersInReverse(chemicalName, i);
            if (charsMatched != nodeLength) {
                break;
            }
            i -= (charsMatched - 1);
        }
        if (node->isEndPoint()) {
            if (indexes.empty()) {
                indexes = std::vector<int>();
            }
            indexes.push_back(i);
        }
    }
    return indexes;
}

OpsinTrieNode::OpsinTrieNode(const std::wstring &key, bool isEndPoint) {
    this->isEndPoint_Renamed = isEndPoint;
    this->key = key;
}

std::wstring OpsinTrieNode::getValue() {
    return key;
}

bool OpsinTrieNode::isEndPoint() {
    return isEndPoint_Renamed;
}

void OpsinTrieNode::setIsEndPoint(bool isEndPoint) {
    this->isEndPoint_Renamed = isEndPoint;
}

void OpsinTrieNode::setChildren(std::vector<OpsinTrieNode *> &children) {
    this->children = children;
}

OpsinTrieNode *OpsinTrieNode::add(const std::wstring &remaingStr, int charsMatched) {
    if (charsMatched < key.length()) { //need to split this Trie node
        OpsinTrieNode *newNode = new OpsinTrieNode(key.substr(charsMatched), isEndPoint_Renamed);
        newNode->setChildren(children);
        children = std::vector<OpsinTrieNode *>(128);
        children[key[charsMatched]] = newNode;
        key = key.substr(0, charsMatched);
        isEndPoint_Renamed = false;
    }
    if (remaingStr.length() != 0) {
        int charValue = static_cast<int>(remaingStr[0]);
        if (children[charValue] == nullptr) {
            children[charValue] = new OpsinTrieNode(remaingStr, false);
        }
        return children[charValue];
    }
    return this;
}

int OpsinTrieNode::getNumberOfMatchingCharacters(const std::wstring &chemicalName, int posInName) {
    int maxLength = std::min(key.length(), chemicalName.length() - posInName);
    for (int i = 0; i < maxLength; i++) {
        if (key[i] != chemicalName[posInName + i]) {
            return i;
        }
    }
    return maxLength;
}

int OpsinTrieNode::getNumberOfMatchingCharactersInReverse(const std::wstring &chemicalName, int posInName) {
    int maxLength = std::min(key.length(), posInName + 1);
    for (int i = 0; i < maxLength; i++) {
        if (key[i] != chemicalName[posInName - i]) {
            return i;
        }
    }
    return maxLength;
}

OpsinTrieNode *OpsinTrieNode::getChild(wchar_t c) {
    return children[static_cast<int>(c)];
}
