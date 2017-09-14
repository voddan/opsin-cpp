#include "Parse.h"
#include "ParseWord.h"


Parse::Parse(const std::wstring &chemicalName) : name(chemicalName) {
}

Parse *Parse::deepCopy() {
    Parse *p = new Parse(name);
    for (auto pw : words) {
        p->words.push_back(pw->deepCopy());
    }
    return p;
}

std::wstring Parse::toString() {
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
    return std::wstring(L"[") + name + std::wstring(L", ") + words.toString() + std::wstring(L"]");
}

std::vector<ParseWord *> Parse::getWords() {
    return words;
}

bool Parse::addWord(ParseWord *word) {
    return words.push_back(word);
}

bool Parse::removeWord(ParseWord *word) {
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
    return words.remove(word);
}

ParseWord *Parse::getWord(int indice) {
    return words[indice];
}

std::wstring Parse::getName() {
    return name;
}
