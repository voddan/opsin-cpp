#include "ParseWord.h"
#include "ParseTokens.h"


ParseWord *ParseWord::deepCopy() {
    return new ParseWord(word, parseTokens);
}

ParseWord::ParseWord(const std::wstring &word, std::vector<ParseTokens *> &parseTokens) : parseTokens(
        this->parseTokens.clear()) {
    this->word = word;
    if (parseTokens.empty()) {
    } else {
        this->parseTokens = std::vector<ParseTokens *>(parseTokens);
    }
}

std::wstring ParseWord::getWord() {
    return word;
}

std::vector<ParseTokens *> ParseWord::getParseTokens() {
    return parseTokens;
}

std::wstring ParseWord::toString() {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
    return std::wstring(L"[") + word + std::wstring(L", ") + parseTokens.toString() + std::wstring(L"]");
}
