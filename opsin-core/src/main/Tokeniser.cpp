#include "Tokeniser.h"
#include "ParseRules.h"
#include "ParsingException.h"
#include "TokenizationResult.h"
#include "WordTools.h"
#include "ParseRulesResults.h"
#include "ParseTokens.h"
#include "ReverseParseRules.h"
#include "Parse.h"
#include "ParseWord.h"
#include "OpsinTools.h"
#include "WordType.h"


Tokeniser::Tokeniser(ParseRules *parseRules) : parseRules(parseRules) {
}

ParseRules *Tokeniser::getParseRules() {
    return parseRules;
}

TokenizationResult *
Tokeniser::tokenize(const std::wstring &name, bool allowRemovalOfWhiteSpace) throw(ParsingException) {
    TokenizationResult *result = allowRemovalOfWhiteSpace ? new TokenizationResult(
            WordTools::removeWhiteSpaceIfBracketsAreUnbalanced(name)) : new TokenizationResult(name);
    TokenizationResult *resultFromBeforeWhitespaceRemoval = nullptr;

    while (!result->isSuccessfullyTokenized()) {
        ParseRulesResults *results = parseRules->getParses(result->getUnparsedName());
        std::vector<ParseTokens *> parseTokens = results->getParseTokensList();
        result->setWorkingName(results->getUninterpretableName());

        std::wstring parsedName = result->getUnparsedName().substr(0, result->getUnparsedName().length() -
                                                                      result->getWorkingName().length());

        if (isWordParsable(parseTokens, result)) {
            parseWord(result, parseTokens, parsedName, false);
            resultFromBeforeWhitespaceRemoval = nullptr;
        } else {
            if (resultFromBeforeWhitespaceRemoval == nullptr) {
                resultFromBeforeWhitespaceRemoval = new TokenizationResult(name);
                resultFromBeforeWhitespaceRemoval->setErrorFields(result->getUnparsedName(), result->getWorkingName(),
                                                                  results->getUnparseableName());
            }

            if (!fixWord(result, parsedName, allowRemovalOfWhiteSpace)) {
                result->setErrorFields(resultFromBeforeWhitespaceRemoval->getUnparsedName(),
                                       resultFromBeforeWhitespaceRemoval->getUninterpretableName(),
                                       resultFromBeforeWhitespaceRemoval->getUnparsableName());
                break;
            }
        }
    }

    return result;
}

TokenizationResult *Tokeniser::tokenizeRightToLeft(ReverseParseRules *reverseParseRules, const std::wstring &name,
                                                   bool allowRemovalOfWhiteSpace) throw(ParsingException) {
    TokenizationResult *result = new TokenizationResult(name);
    //removeWhiteSpaceIfBracketsAreUnbalanced is not currently employed as the input to this function from the parser will often be what the LR tokenizer couldn't handle, which may not have matching brackets
    TokenizationResult *resultFromBeforeWhitespaceRemoval = nullptr;

    while (!result->isSuccessfullyTokenized()) {
        ParseRulesResults *results = reverseParseRules->getParses(result->getUnparsedName());
        std::vector<ParseTokens *> parseTokens = results->getParseTokensList();
        result->setWorkingName(results->getUninterpretableName());

        std::wstring parsedName = result->getUnparsedName().substr(result->getWorkingName().length());

        if (isWordParsableInReverse(parseTokens, result)) {
            parseWord(result, parseTokens, parsedName, true);
            resultFromBeforeWhitespaceRemoval = nullptr;
        } else {
            if (resultFromBeforeWhitespaceRemoval == nullptr) {
                resultFromBeforeWhitespaceRemoval = new TokenizationResult(name);
                resultFromBeforeWhitespaceRemoval->setErrorFields(result->getUnparsedName(), result->getWorkingName(),
                                                                  results->getUnparseableName());
            }
            if (!fixWordInReverse(result, parsedName, allowRemovalOfWhiteSpace)) {
                result->setErrorFields(resultFromBeforeWhitespaceRemoval->getUnparsedName(),
                                       resultFromBeforeWhitespaceRemoval->getUninterpretableName(),
                                       resultFromBeforeWhitespaceRemoval->getUnparsableName());
                break;
            }
        }
    }

    std::reverse(result->getParse()->getWords().begin(), result->getParse()->getWords().end());
    return result;
}

bool Tokeniser::isWordParsableInReverse(std::vector<ParseTokens *> &parseTokens, TokenizationResult *result) {
    return parseTokens.size() > 0 &&
           (result->isFullyInterpretable() || result->getWorkingName()[result->getWorkingName().length() - 1] == L' ' ||
            result->getWorkingName()[result->getWorkingName().length() - 1] == L'-');
}

bool Tokeniser::isWordParsable(std::vector<ParseTokens *> &parseTokens, TokenizationResult *result) {
    return parseTokens.size() > 0 && (result->isFullyInterpretable() || result->getWorkingName()[0] == L' ' ||
                                      result->getWorkingName()[0] == L'-');
}

void Tokeniser::parseWord(TokenizationResult *result, std::vector<ParseTokens *> &parseTokens,
                          const std::wstring &parsedName, bool reverse) {
    //If something like ethylchloride is encountered this should be split back to ethyl chloride and there will be 2 ParseWords returned
    //In cases of properly formed names there will be only one ParseWord
    //If there are two parses one of which assumes a missing space and one of which does not the former is discarded
    addParseWords(parseTokens, parsedName, result->getParse(), reverse);

    if (result->isFullyInterpretable()) {
        result->setUnparsedName(result->getWorkingName());
    } else {
        std::wstring remainingName = result->getWorkingName();
        if (reverse) {
            if (remainingName.length() > 3 && boost::ends_with(remainingName, L" - ")) {
                remainingName = remainingName.substr(0, remainingName.length() - 3);
            } else {
                remainingName = remainingName.substr(0, remainingName.length() - 1);
            }
        } else {
            if (remainingName.length() > 3 &&
                boost::starts_with(remainingName, L" - ")) { //this is a way of of indicating a mixture
                remainingName = remainingName.substr(3);
            } else {
                remainingName = remainingName.substr(1);
            }
        }
        result->setUnparsedName(remainingName);
    }
}

void Tokeniser::addParseWords(std::vector<ParseTokens *> &parseTokens, const std::wstring &parsedName, Parse *parse,
                              bool reverse) {
    std::vector<ParseWord *> parseWords = WordTools::splitIntoParseWords(parseTokens, parsedName);

    if (reverse) {
        std::reverse(parseWords.begin(), parseWords.end()); //make this set of words back to front as well
    }

    for (auto parseWord : parseWords) {
        parse->addWord(parseWord);
    }
}

bool Tokeniser::fixWord(TokenizationResult *result, const std::wstring &parsedName,
                        bool allowRemovalOfWhiteSpace) throw(ParsingException) {
    Matcher *m = matchCompoundWithPhrase->matcher(result->getWorkingName());
    if (m->lookingAt() && lastParsedWordWasFullOrFunctionalTerm(result)) {
        result->setUnparsedName(parsedName + result->getWorkingName().substr(m->group()->length()));
    } else if (matchCasCollectiveIndex->matcher(result->getWorkingName()).matches()) {
        result->setUnparsedName(parsedName);
    } else {
        if (allowRemovalOfWhiteSpace) {
            //TODO add a warning message if this code is invoked. A name invoking this is unambiguously BAD
            std::vector<ParseWord *> parsedWords = result->getParse()->getWords();
            if (!reverseSpaceRemoval(parsedWords, result)) {
                //Try and remove a space from the right and try again
                int indexOfSpace = (int) result->getWorkingName().find(L' ');
                if (indexOfSpace != -1) {
                    result->setUnparsedName(parsedName + result->getWorkingName().substr(0, indexOfSpace) +
                                            result->getWorkingName().substr(indexOfSpace + 1));
                } else {
                    return false;
                }
            }
        } else {
            return false;
        }
    }
    return true;
}

bool Tokeniser::lastParsedWordWasFullOrFunctionalTerm(TokenizationResult *result) throw(ParsingException) {
    std::vector<ParseWord *> parseWords = result->getParse()->getWords();
    if (parseWords.size() > 0) {
        std::vector<ParseTokens *> parseTokensList = parseWords[parseWords.size() - 1]->getParseTokens();
        for (auto parseTokens : parseTokensList) {
            WordType type = OpsinTools::determineWordType(parseTokens->getAnnotations());
            if (type::equals(WordType::full) || type::equals(WordType::functionalTerm)) {
                return true;
            }
        }
    }
    return false;
}

bool
Tokeniser::fixWordInReverse(TokenizationResult *result, const std::wstring &parsedName, bool allowRemovalOfWhiteSpace) {
    if (allowRemovalOfWhiteSpace) {
        //Try and remove a space and try again
        //TODO add a warning message if this code is invoked. A name invoking this is unambiguously BAD
        int indexOfSpace = (int) result->getWorkingName().rfind(L' ');
        if (indexOfSpace != -1) {
            result->setUnparsedName(result->getWorkingName().substr(0, indexOfSpace) +
                                    result->getWorkingName().substr(indexOfSpace + 1) + parsedName);
        } else {
            return false;
        }
    } else {
        return false;
    }
    return true;
}

bool Tokeniser::reverseSpaceRemoval(std::vector<ParseWord *> &parsedWords,
                                    TokenizationResult *result) throw(ParsingException) {
    bool successful = false;

    if (!parsedWords.empty()) { //first see whether the space before the unparseable word is erroneous
        ParseWord *pw = parsedWords[parsedWords.size() - 1];
        ParseRulesResults *backResults = parseRules->getParses(pw->getWord() + result->getUnparsedName());
        std::vector<ParseTokens *> backParseTokens = backResults->getParseTokensList();
        std::wstring backUninterpretableName = backResults->getUninterpretableName();
        std::wstring backParsedName = pw->getWord() + result->getUnparsedName().substr(0,
                                                                                       result->getUnparsedName().length() -
                                                                                       backUninterpretableName.length());
        if (backParsedName.length() > pw->getWord().length() && backParseTokens.size() > 0 &&
            (backUninterpretableName == L"" || backUninterpretableName[0] == L' ' ||
             backUninterpretableName[0] == L'-')) { //a word was interpretable
            result->getParse()->removeWord(pw);
            std::vector<ParseWord *> parseWords = WordTools::splitIntoParseWords(backParseTokens, backParsedName);
            for (auto parseWord : parseWords) {
                result->getParse()->addWord(parseWord);
            }
            if (backUninterpretableName != L"") {
                result->setUnparsedName(
                        backUninterpretableName.substr(1)); //remove white space at start of uninterpretableName
            } else {
                result->setUnparsedName(backUninterpretableName);
            }
            successful = true;
        }
    }

    return successful;
}
