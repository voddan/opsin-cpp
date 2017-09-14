#include "PreProcessor.h"
#include "PreProcessingException.h"
#include "StringTools.h"


const std::unordered_map<std::wstring, std::wstring> PreProcessor::DOTENCLOSED_TO_DESIRED = std::unordered_map<std::wstring, std::wstring>();
const std::unordered_map<std::wstring, std::wstring> PreProcessor::XMLENTITY_TO_DESIRED = std::unordered_map<std::wstring, std::wstring>();

PreProcessor::StaticConstructor::StaticConstructor() {
    DOTENCLOSED_TO_DESIRED[L"a"] = L"alpha";
    DOTENCLOSED_TO_DESIRED[L"b"] = L"beta";
    DOTENCLOSED_TO_DESIRED[L"g"] = L"gamma";
    DOTENCLOSED_TO_DESIRED[L"d"] = L"delta";
    DOTENCLOSED_TO_DESIRED[L"e"] = L"epsilon";
    DOTENCLOSED_TO_DESIRED[L"l"] = L"lambda";
    DOTENCLOSED_TO_DESIRED[L"x"] = L"xi";
    DOTENCLOSED_TO_DESIRED[L"alpha"] = L"alpha";
    DOTENCLOSED_TO_DESIRED[L"beta"] = L"beta";
    DOTENCLOSED_TO_DESIRED[L"gamma"] = L"gamma";
    DOTENCLOSED_TO_DESIRED[L"delta"] = L"delta";
    DOTENCLOSED_TO_DESIRED[L"epsilon"] = L"epsilon";
    DOTENCLOSED_TO_DESIRED[L"zeta"] = L"zeta";
    DOTENCLOSED_TO_DESIRED[L"eta"] = L"eta";
    DOTENCLOSED_TO_DESIRED[L"lambda"] = L"lambda";
    DOTENCLOSED_TO_DESIRED[L"xi"] = L"xi";
    DOTENCLOSED_TO_DESIRED[L"omega"] = L"omega";
    DOTENCLOSED_TO_DESIRED[L"fwdarw"] = L"->";

    XMLENTITY_TO_DESIRED[L"alpha"] = L"alpha";
    XMLENTITY_TO_DESIRED[L"beta"] = L"beta";
    XMLENTITY_TO_DESIRED[L"gamma"] = L"gamma";
    XMLENTITY_TO_DESIRED[L"delta"] = L"delta";
    XMLENTITY_TO_DESIRED[L"epsilon"] = L"epsilon";
    XMLENTITY_TO_DESIRED[L"zeta"] = L"zeta";
    XMLENTITY_TO_DESIRED[L"eta"] = L"eta";
    XMLENTITY_TO_DESIRED[L"lambda"] = L"lambda";
    XMLENTITY_TO_DESIRED[L"xi"] = L"xi";
    XMLENTITY_TO_DESIRED[L"omega"] = L"omega";
}

PreProcessor::StaticConstructor PreProcessor::staticConstructor;

std::wstring PreProcessor::preProcess(const std::wstring &chemicalName) throw(PreProcessingException) {
    chemicalName = boost::trim_copy(chemicalName); //remove leading and trailing whitespace
    if (chemicalName.length() == 0) {
        throw PreProcessingException(L"Input chemical name was blank!");
    }

    chemicalName = performMultiCharacterReplacements(chemicalName);
    chemicalName = StringTools::convertNonAsciiAndNormaliseRepresentation(chemicalName);
    return chemicalName;
}

std::wstring PreProcessor::performMultiCharacterReplacements(const std::wstring &chemicalName) {
    StringBuilder *sb = new StringBuilder(chemicalName.length());
    for (int i = 0, nameLength = chemicalName.length(); i < nameLength; i++) {
        wchar_t ch = chemicalName[i];
        switch (ch) {
            case L'$':
                if (i + 1 < nameLength) {
                    wchar_t letter = chemicalName[i + 1];
                    std::wstring replacement = getReplacementForDollarGreek(letter);
                    if (replacement != L"") {
                        sb->append(replacement);
                        i++;
                        break;
                    }
                }
                sb->append(ch);
                break;
            case L'.': {
                //e.g. .alpha.
                std::wstring dotEnclosedString = getLowerCasedDotEnclosedString(chemicalName, i);
                std::wstring dotEnclosedReplacement = DOTENCLOSED_TO_DESIRED[dotEnclosedString];
                if (dotEnclosedReplacement != L"") {
                    sb->append(dotEnclosedReplacement);
                    i = i + dotEnclosedString.length() + 1;
                    break;
                }
                sb->append(ch);
                break;
            }
            case L'&': {
                //e.g. &alpha;
                std::wstring xmlEntityString = getLowerCasedXmlEntityString(chemicalName, i);
                std::wstring xmlEntityReplacement = XMLENTITY_TO_DESIRED[xmlEntityString];
                if (xmlEntityReplacement != L"") {
                    sb->append(xmlEntityReplacement);
                    i = i + xmlEntityReplacement.length() + 1;
                    break;
                }
                sb->append(ch);
                break;
            }
            case L's':
            case L'S': //correct British spelling to the IUPAC spelling
                if (chemicalName.regionMatches(true, i + 1, L"ulph", 0, 4)) {
                    sb->append(L"sulf");
                    i = i + 4;
                    break;
                }
                sb->append(ch);
                break;
            default:
                sb->append(ch);
        }
    }
    return sb->toString();
}

std::wstring PreProcessor::getLowerCasedDotEnclosedString(const std::wstring &chemicalName, int indexOfFirstDot) {
    int end = -1;
    int limit = std::min(indexOfFirstDot + 9, chemicalName.length());
    for (int j = indexOfFirstDot + 1; j < limit; j++) {
        if (chemicalName[j] == L'.') {
            end = j;
            break;
        }
    }
    if (end > 0) {
        return chemicalName.substr(indexOfFirstDot + 1, end - (indexOfFirstDot + 1))->toLowerCase(Locale::ROOT);
    }
    return L"";
}

std::wstring PreProcessor::getLowerCasedXmlEntityString(const std::wstring &chemicalName, int indexOfAmpersand) {
    int end = -1;
    int limit = std::min(indexOfAmpersand + 9, chemicalName.length());
    for (int j = indexOfAmpersand + 1; j < limit; j++) {
        if (chemicalName[j] == L';') {
            end = j;
            break;
        }
    }
    if (end > 0) {
        return chemicalName.substr(indexOfAmpersand + 1, end - (indexOfAmpersand + 1))->toLowerCase(Locale::ROOT);
    }
    return L"";
}

std::wstring PreProcessor::getReplacementForDollarGreek(wchar_t ch) {
    switch (ch) {
        case L'a' :
            return L"alpha";
        case L'b' :
            return L"beta";
        case L'g' :
            return L"gamma";
        case L'd' :
            return L"delta";
        case L'e' :
            return L"epsilon";
        case L'l' :
            return L"lambda";
        default:
            return L"";
    }
}
