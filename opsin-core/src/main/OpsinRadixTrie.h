#pragma once

#include <string>
#include <vector>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class OpsinTrieNode;


/// <summary>
/// A black/white radix tree implementation.
/// A radix tree is a type of trie where common prefixes are merged together to save space
/// This implementation employs short arrays rather than maps to exploit the fact that all OPSIN tokens are ASCII.
/// @author dl387
///
/// </summary>
class OpsinRadixTrie {
public:
    OpsinTrieNode *const rootNode;

    virtual ~OpsinRadixTrie() {
        delete rootNode;
    }

    OpsinRadixTrie();

    /// <summary>
    /// Adds a string to the Trie.
    /// This string should not contain any non ASCII characters </summary>
    /// <param name="token"> </param>
    virtual void addToken(const std::wstring &token);

    /// <summary>
    /// Returns all possible runs of the input string that reached end point nodes in the trie
    /// e.g. ylidene might return 2 ("yl"), 6 ("yliden") and 7 ("ylidene")
    /// Results are given as the index of the end of the match in the chemicalName
    /// Returns null if no runs were possible </summary>
    /// <param name="chemicalName"> </param>
    /// <param name="posInName"> The point at which to start matching
    /// @return </param>
    virtual std::vector<int> findMatches(const std::wstring &chemicalName, int posInName);

    /// <summary>
    /// Same as findMatches but the trie has been populated by reversed tokens </summary>
    /// <param name="chemicalName"> </param>
    /// <param name="posInName"> The index after the first character to start matching
    /// @return </param>
    virtual std::vector<int> findMatchesReadingStringRightToLeft(const std::wstring &chemicalName, int posInName);
};

class OpsinTrieNode {

private:
//JAVA TO C++ CONVERTER NOTE: Fields cannot have the same name as methods:
    bool isEndPoint_Renamed = false;
    std::wstring key;

    std::vector<OpsinTrieNode *> children(

    128);

public:
    OpsinTrieNode(const std::wstring &key, bool isEndPoint);

    virtual std::wstring getValue();

    virtual bool isEndPoint();

    virtual void setIsEndPoint(bool isEndPoint);

private:
    void setChildren(std::vector<OpsinTrieNode *> &children);

public:
    virtual OpsinTrieNode *add(const std::wstring &remaingStr, int charsMatched);

    virtual int getNumberOfMatchingCharacters(const std::wstring &chemicalName, int posInName);

    virtual int getNumberOfMatchingCharactersInReverse(const std::wstring &chemicalName, int posInName);

    virtual OpsinTrieNode *getChild(wchar_t c);
};

