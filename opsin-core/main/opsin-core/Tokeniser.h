#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class ParseRules;
class ParsingException;
class TokenizationResult;
class ReverseParseRules;
class ParseTokens;
class Parse;
class ParseWord;


/// <summary>
/// Uses OPSIN's DFA based grammar to break a name into tokens with associated meanings ("annotations").
/// @author dl387
///
/// </summary>
class Tokeniser {
private:
    ParseRules *const parseRules;
    Pattern *const matchCasCollectiveIndex = Pattern::compile(
            L"([\\[\\(\\{]([1-9][0-9]?[cC][iI][, ]?)+[\\]\\)\\}])+|[1-9][0-9]?[cC][iI]", Pattern::CASE_INSENSITIVE);
    Pattern *const matchCompoundWithPhrase = Pattern::compile(L"(compd\\. with|compound with|and) ",
                                                              Pattern::CASE_INSENSITIVE);

public:
    virtual ~Tokeniser() {
        delete parseRules;
        delete matchCasCollectiveIndex;
        delete matchCompoundWithPhrase;
    }

    Tokeniser(ParseRules *parseRules);

    virtual ParseRules *getParseRules();

    /// <summary>
    /// Master method for tokenizing chemical names into words and within words into tokens </summary>
    /// <param name="name"> The chemical name. </param>
    /// <param name="allowRemovalOfWhiteSpace">
    /// @return </param>
    /// <exception cref="ParsingException">  </exception>
    virtual TokenizationResult *
    tokenize(const std::wstring &name, bool allowRemovalOfWhiteSpace) throw(ParsingException);

    /// <summary>
    /// Master method for tokenizing chemical names into words and within words into tokens
    /// This is performed in a right to left manner </summary>
    /// <param name="reverseParseRules"> </param>
    /// <param name="name"> The chemical name. </param>
    /// <param name="allowRemovalOfWhiteSpace">
    /// @return </param>
    /// <exception cref="ParsingException">  </exception>
    virtual TokenizationResult *tokenizeRightToLeft(ReverseParseRules *reverseParseRules, const std::wstring &name,
                                                    bool allowRemovalOfWhiteSpace) throw(ParsingException);


private:
    bool isWordParsableInReverse(std::vector<ParseTokens *> &parseTokens, TokenizationResult *result);

    bool isWordParsable(std::vector<ParseTokens *> &parseTokens, TokenizationResult *result);

    void parseWord(TokenizationResult *result, std::vector<ParseTokens *> &parseTokens, const std::wstring &parsedName,
                   bool reverse);

    void
    addParseWords(std::vector<ParseTokens *> &parseTokens, const std::wstring &parsedName, Parse *parse, bool reverse);

    bool fixWord(TokenizationResult *result, const std::wstring &parsedName,
                 bool allowRemovalOfWhiteSpace) throw(ParsingException);

    bool lastParsedWordWasFullOrFunctionalTerm(TokenizationResult *result) throw(ParsingException);

    bool fixWordInReverse(TokenizationResult *result, const std::wstring &parsedName, bool allowRemovalOfWhiteSpace);

    /// <summary>
    /// Fixes cases like for example "benzene sulfonamide" -->"benzenesulfonamide" </summary>
    /// <param name="parsedWords"> </param>
    /// <param name="result">
    /// @return </param>
    /// <exception cref="ParsingException"> </exception>
    bool reverseSpaceRemoval(std::vector<ParseWord *> &parsedWords, TokenizationResult *result) throw(ParsingException);
};

