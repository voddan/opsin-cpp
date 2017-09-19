#include "CASTools.h"
#include "ParseRules.h"
#include "ParsingException.h"
#include "ParseRulesResults.h"
#include "ParseTokens.h"
#include "ParseWord.h"
#include "WordTools.h"
#include "OpsinTools.h"
#include "WordType.h"
#include "StringTools.h"
#include <boost/algorithm/string/regex.hpp>


std::wstring CASTools::uninvertCASName(const std::wstring &name, ParseRules *parseRules) throw(ParsingException) {
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
    std::wstring & splitted();
    boost::split_regex(splitted, name, matchCommaSpace);

    std::vector<std::wstring> nameComponents(splitted);
    std::vector<std::wstring> substituents;
    std::vector<std::wstring> seperateWordSubstituents;
    std::vector<std::wstring> functionalTerms;

    std::wstring parent = nameComponents[0];
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
    std::vector<std::wstring> parentNameParts = parent.split(L" ");
    if (parentNameParts.size() != 1) {
        if (matchCasCollectiveIndex->matcher(parentNameParts[parentNameParts.size() -
                                                             1]).matches()) { //CAS collective index description should be ignored
            StringBuilder *parentSB = new StringBuilder();
            for (int i = 0; i < parentNameParts.size() - 1; i++) {
                parentSB->append(parentNameParts[i]);
            }
            parent = parentSB->toString();
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
            parentNameParts = parent.split(L" ");
        }
        for (int i = 1; i < parentNameParts.size(); i++) {
            if (!matchAcid->matcher(parentNameParts[i]).matches()) {
                ParseRulesResults *results = parseRules->getParses(parentNameParts[i]);
                std::vector<ParseTokens *> parseTokens = results->getParseTokensList();
                if (parseTokens.empty()) {
                    throw ParsingException(L"Invalid CAS name. Parent compound was followed by an unexpected term");
                }
            }
        }
    }
    bool addedBracket = false;
    bool esterEncountered = false;
    for (int i = 1; i < nameComponents.size(); i++) {
        std::wstring nameComponent = nameComponents[i];
        Matcher *m = matchCompoundWithPhrase->matcher(nameComponent);
        bool compoundWithcomponent = false;
        if (m->lookingAt()) {
            nameComponent = nameComponent.substr(m->group()->length());
            compoundWithcomponent = true;
        }
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
        std::vector<std::wstring> components = nameComponents[i].split(L" ");
        for (int c = 0, componentLen = components.size(); c < componentLen; c++) {
            std::wstring component = components[c];
            if (compoundWithcomponent) {
                functionalTerms.push_back(component);
                continue;
            }
            if (boost::ends_with(component, L"-")) {
                boost::optional<wchar_t> missingCloseBracket = missingCloseBracketCharIfApplicable(component);
                if (missingCloseBracket) {
                    if (addedBracket) {
                        throw ParsingException(L"Close bracket appears to be missing");
                    }
                    parent += missingCloseBracket;
                    addedBracket = true;
                }
                substituents.push_back(component);
            } else {
                ParseRulesResults *results = parseRules->getParses(component);
                std::vector<ParseTokens *> parseTokens = results->getParseTokensList();
                if (parseTokens.size() > 0) {
                    std::vector<ParseWord *> parseWords = WordTools::splitIntoParseWords(parseTokens, component);

                    std::vector<ParseTokens *> firstParseWordTokens = parseWords[0]->getParseTokens();
                    WordType firstWordType = OpsinTools::determineWordType(firstParseWordTokens[0]->getAnnotations());
                    for (int j = 1; j < firstParseWordTokens.size(); j++) {
                        if (!firstWordType::equals(
                                OpsinTools::determineWordType(firstParseWordTokens[j]->getAnnotations()))) {
                            throw ParsingException(component + std::wstring(
                                    L"can be interpreted in multiple ways. For the sake of precision OPSIN has decided not to process this as a CAS name"));
                        }
                    }

                    if (parseWords.size() == 1) {
                        switch (firstWordType) {
                            case functionalTerm:
                                if (component.equalsIgnoreCase(L"ester")) {
                                    if (seperateWordSubstituents.empty()) {
                                        throw ParsingException(
                                                L"ester encountered but no substituents were specified in potential CAS name!");
                                    }
                                    if (esterEncountered) {
                                        throw ParsingException(
                                                L"ester formation was mentioned more than once in CAS name!");
                                    }
                                    parent = uninvertEster(parent);
                                    esterEncountered = true;
                                } else {
                                    functionalTerms.push_back(component);
                                }
                                break;
                            case substituent:
                                seperateWordSubstituents.push_back(component);
                                break;
                            case full:
                                if (StringTools::endsWithCaseInsensitive(component, L"ate") ||
                                    StringTools::endsWithCaseInsensitive(component, L"ite") ||
                                    StringTools::endsWithCaseInsensitive(component, L"ium") ||
                                    StringTools::endsWithCaseInsensitive(component, L"hydrofluoride") ||
                                    StringTools::endsWithCaseInsensitive(component, L"hydrochloride") ||
                                    StringTools::endsWithCaseInsensitive(component, L"hydrobromide") ||
                                    StringTools::endsWithCaseInsensitive(component,
                                                                         L"hydroiodide")) { //e.g. Piperazinium, 1,1-dimethyl-, 2,2,2-trifluoroacetate hydrochloride
                                    functionalTerms.push_back(component);
                                } else if (StringTools::endsWithCaseInsensitive(component, L"ic") &&
                                           c + 1 < componentLen && components[c + 1].equalsIgnoreCase(L"acid")) {
                                    functionalTerms.push_back(component);
                                    functionalTerms.push_back(components[++c]);
                                } else {
                                    throw ParsingException(std::wstring(L"Unable to interpret: ") + component +
                                                           std::wstring(
                                                                   L" (as part of a CAS index name)- A full word was encountered where a substituent or functionalTerm was expected"));
                                }
                                break;
                            default:
                                throw ParsingException(L"Unrecognised CAS index name form");
                        }
                    } else if (parseWords.size() == 2 && firstWordType::equals(WordType::substituent)) {
                        //could be something like O-methyloxime which is parsed as [O-methyl] [oxime]
                        std::vector<ParseTokens *> secondParseWordTokens = parseWords[1]->getParseTokens();
                        WordType secondWordType = OpsinTools::determineWordType(
                                secondParseWordTokens[0]->getAnnotations());
                        for (int j = 1; j < secondParseWordTokens.size(); j++) {
                            if (!secondWordType::equals(
                                    OpsinTools::determineWordType(secondParseWordTokens[j]->getAnnotations()))) {
                                throw ParsingException(component + std::wstring(
                                        L"can be interpreted in multiple ways. For the sake of precision OPSIN has decided not to process this as a CAS name"));
                            }
                        }
                        if (secondWordType::equals(WordType::functionalTerm) &&
                            matchFunctionalTermAllowingSubstituentPrefix->matcher(parseWords[1]->getWord()).matches()) {
                            functionalTerms.push_back(component);
                        } else {
                            throw ParsingException(L"Unrecognised CAS index name form, could have a missing space?");
                        }
                    } else {
                        throw ParsingException(L"Unrecognised CAS index name form");
                    }
                } else {
                    if (!matchCasCollectiveIndex->matcher(
                            component).matches()) { //CAS collective index description should be ignored
                        throw ParsingException(std::wstring(L"Unable to interpret: ") + component +
                                               std::wstring(L" (as part of a CAS index name)"));
                    }
                }
            }
        }
    }
    StringBuilder *casName = new StringBuilder();
    for (auto prefixFunctionalTerm : seperateWordSubstituents) {
        casName->append(prefixFunctionalTerm);
        casName->append(L" ");
    }
    for (int i = substituents.size() - 1; i >= 0; i--) {
        //stereochemistry term comes after substituent term. In older CAS names (9CI) this stereochemistry term can apply to the substituent term. Hence append in reverse order
        casName->append(substituents[i]);
    }
    casName->append(parent);
    for (auto functionalTerm : functionalTerms) {
        casName->append(L" ");
        casName->append(functionalTerm);
    }
    return casName->toString();
}

boost::optional<wchar_t> CASTools::missingCloseBracketCharIfApplicable(const std::wstring &component) {
    int bracketLevel = 0;
    boost::optional<wchar_t> missingCloseBracket = nullptr;
    for (int i = 0, l = component.length(); i < l; i++) {
        wchar_t character = component[i];
        if (character == L'(' || character == L'[' || character == L'{') {
            bracketLevel++;
            if (bracketLevel == 1) {
                missingCloseBracket = character;
            }
        }
        if (character == L')' || character == L']' || character == L'}') {
            bracketLevel--;
            if (bracketLevel < 0) {
                return boost::none;
            }
        }
    }
    if (bracketLevel == 1) {
        if (missingCloseBracket == L'(') {
            return L')';
        }
        if (missingCloseBracket == L'[') {
            return L']';
        }
        if (missingCloseBracket == L'{') {
            return L'}';
        }
    }
    return boost::none;
}

std::wstring CASTools::uninvertEster(const std::wstring &parent) throw(ParsingException) {
    int len = parent.length();
    if (len == 0) {
        throw ParsingException(L"Failed to uninvert CAS ester");
    }
    wchar_t lastChar = parent[len - 1];

    std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>> result;

    if (lastChar == L')') {
        if (StringTools::endsWithCaseInsensitive(parent, L"ic acid)")) {
            result = parent.substr(0, parent.length() - 8) + std::wstring(L"ate)");
        } else if (StringTools::endsWithCaseInsensitive(parent, L"ous acid)")) {
            result = parent.substr(0, parent.length() - 9) + std::wstring(L"ite)");
        } else if (StringTools::endsWithCaseInsensitive(parent, L"ine)")) { //amino acid
            result = parent.substr(0, parent.length() - 2) + std::wstring(L"ate)");
        } else {
            throw ParsingException(L"Failed to uninvert CAS ester");
        }
    } else {
        if (StringTools::endsWithCaseInsensitive(parent, L"ic acid")) {
            result = parent.substr(0, parent.length() - 7) + std::wstring(L"ate");
        } else if (StringTools::endsWithCaseInsensitive(parent, L"ous acid")) {
            result = parent.substr(0, parent.length() - 8) + std::wstring(L"ite");
        } else if (StringTools::endsWithCaseInsensitive(parent, L"ine")) { //amino acid
            result = parent.substr(0, parent.length() - 1) + std::wstring(L"ate");
        } else {
            throw ParsingException(L"Failed to uninvert CAS ester");
        }
    }
    return parent;
}
