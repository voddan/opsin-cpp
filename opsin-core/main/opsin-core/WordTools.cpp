#include "WordTools.h"
#include "ParseTokens.h"
#include "ParseWord.h"
#include "OpsinTools.h"
#include "StringTools.h"
#include "ParsingException.h"



//						import static uk.ac.cam.ch.wwmm.opsin.OpsinTools.*;

std::vector<ParseWord *>
WordTools::splitIntoParseWords(std::vector<ParseTokens *> &parseTokensList, const std::wstring &chemicalName) {
    std::vector<ParseTokens *> wellFormedParseTokens; //these are all in the same word as would be expected
    std::vector<std::vector<ParseTokens *>> splitParseTokensForEachParseTokens;
    /*
     * Each ParseTokens is split into the number of words it describes
     * e.g. ethylchloride has one interpretation so splitParseTokensList will have one entry
     * This entry will be formed of TWO parseTokens, one for the ethyl and one for the chloride
     */
    int leastWordsInOmmittedSpaceParse = std::numeric_limits<int>::max(); //we want the least number of words i.e. less omitted spaces
    int longestFunctionalTermEncountered = 0; //we want the longest functional term
    for (auto parseTokens : parseTokensList) {
        std::vector<wchar_t> annotations = parseTokens->getAnnotations();
        std::vector<std::vector<wchar_t>> chunkedAnnotations = chunkAnnotations(
                annotations); //chunked into mainGroup/substituent/functionalTerm
        if (containsOmittedSpace(chunkedAnnotations)) {
            std::vector<ParseTokens *> omittedWordParseTokens;
            std::vector<std::wstring> tokens = parseTokens->getTokens();
            std::vector<wchar_t> newAnnotations;
            std::vector<std::wstring> newTokens;
            int currentFunctionalTermLength = 0;
            int annotPos = 0;
            for (auto annotationList : chunkedAnnotations) {
                boost::optional<wchar_t> finalAnnotationInList = annotationList.get(annotationList.size() - 1);
                if (finalAnnotationInList.equals(OpsinTools::END_OF_FUNCTIONALTERM) && newAnnotations.size() > 0) {
                    //create a new parseTokens for the substituent/maingroup preceding the functional term
                    //not necessary if the functional term is the first thing to be read e.g. in the case of poly
                    ParseTokens tempVar(newTokens, newAnnotations);
                    omittedWordParseTokens.push_back(&tempVar);
                    newAnnotations = std::vector<wchar_t>();
                    newTokens = std::vector<std::wstring>();
                }
                for (* : :optional < wchar_t > annotation : annotationList)
                {
                    newAnnotations.push_back(annotation);
                    newTokens.push_back(tokens[annotPos++]);
                }
                if (finalAnnotationInList.equals(OpsinTools::END_OF_FUNCTIONALTERM) ||
                    finalAnnotationInList.equals(OpsinTools::END_OF_MAINGROUP) || annotPos == tokens.size()) {
                    ParseTokens tempVar2(newTokens, newAnnotations);
                    omittedWordParseTokens.push_back(&tempVar2);
                    if (finalAnnotationInList.equals(OpsinTools::END_OF_FUNCTIONALTERM)) {
                        currentFunctionalTermLength = StringTools::stringListToString(newTokens, L"").length();
                    }
                    newAnnotations = std::vector<wchar_t>();
                    newTokens = std::vector<std::wstring>();
                }
            }
            if (omittedWordParseTokens.size() <= leastWordsInOmmittedSpaceParse) {
                if (omittedWordParseTokens.size() < leastWordsInOmmittedSpaceParse) {
                    splitParseTokensForEachParseTokens.clear();
                    leastWordsInOmmittedSpaceParse = omittedWordParseTokens.size();
                    longestFunctionalTermEncountered = 0;
                }
                if (currentFunctionalTermLength >= longestFunctionalTermEncountered) {
                    if (currentFunctionalTermLength > longestFunctionalTermEncountered) {
                        splitParseTokensForEachParseTokens.clear();
                        longestFunctionalTermEncountered = currentFunctionalTermLength;
                    }
                    splitParseTokensForEachParseTokens.push_back(omittedWordParseTokens);
                }
            }
        } else {
            wellFormedParseTokens.push_back(parseTokens);
        }
    }
    std::vector<ParseWord *> parseWords;
    if (!wellFormedParseTokens.empty()) {
        ParseWord tempVar3(chemicalName, wellFormedParseTokens);
        parseWords.push_back(&tempVar3);
    } else {
        for (int i = 0; i < leastWordsInOmmittedSpaceParse; i++) {
            std::vector<ParseTokens *> parseTokensForWord;
            for (auto parseTokens : splitParseTokensForEachParseTokens) {
                if (!std::find(parseTokensForWord.begin(), parseTokensForWord.end(), parseTokens.get(i)) !=
                    parseTokensForWord.end()))
                { //if only one word is ambiguous there is no need for the unambiguous word to have multiple identical interpretation
                    parseTokensForWord.push_back(parseTokens.get(i));
                }
            }
            ParseWord tempVar4(StringTools::stringListToString(parseTokensForWord[0]->getTokens(), L""),
                               parseTokensForWord);
            parseWords.push_back(&tempVar4);
        }
    }
    return parseWords;
}

bool WordTools::containsOmittedSpace(std::vector<std::vector<wchar_t>> &chunkedAnnotations) {
    if (chunkedAnnotations.size() > 1) { //there are multiple subsitutents/maingroup/functionalterms
        for (auto annotationList : chunkedAnnotations) {
            if (annotationList.contains(OpsinTools::END_OF_FUNCTIONALTERM)) {
                return true;
            }
        }
    }
    return false;
}

std::vector<std::vector<wchar_t>> WordTools::chunkAnnotations(std::vector<wchar_t> &annots) {
    std::vector<std::vector<wchar_t>> chunkList;
    std::vector<wchar_t> currentTerm;
    for (* : :optional < wchar_t > annot : annots)
    {
        currentTerm.push_back(annot);
        wchar_t ch = annot;
        if (ch == OpsinTools::END_OF_SUBSTITUENT || ch == OpsinTools::END_OF_MAINGROUP ||
            ch == OpsinTools::END_OF_FUNCTIONALTERM) {
            chunkList.push_back(currentTerm);
            currentTerm = std::vector<wchar_t>();
        }
    }
    return chunkList;
}

std::wstring WordTools::removeWhiteSpaceIfBracketsAreUnbalanced(const std::wstring &name) throw(ParsingException) {
    int bracketLevel = 0;
    int stringLength = name.length();
    for (int i = 0; i < stringLength; i++) {
        wchar_t c = name[i];
        if (c == L'(' || c == L'[' || c == L'{') {
            bracketLevel++;
        } else if (c == L')' || c == L']' || c == L'}') {
            bracketLevel--;
        } else if (c == L' ' && bracketLevel > 0) { //brackets unbalanced and a space has been encountered!
            name = name.substr(0, i) + name.substr(i + 1);
            stringLength = name.length();
            i--;
        }
    }
    if (bracketLevel > 0) {
        throw ParsingException(std::wstring(L"Unmatched opening bracket found in :") + name);
    } else if (bracketLevel < 0) {
        throw ParsingException(std::wstring(L"Unmatched closing bracket found in :") + name);
    }
    return name;
}
