#include "Parser.h"
#include "Tokeniser.h"
#include "WordRules.h"
#include "ResourceManager.h"
#include "ParseRules.h"
#include "ResourceGetter.h"
#include "Element.h"
#include "NameToStructureConfig.h"
#include "ParsingException.h"
#include "Parse.h"
#include "CASTools.h"
#include "TokenizationResult.h"
#include "GroupingEl.h"
#include "XmlDeclarations.h"
#include "Attribute.h"
#include "ParseTokens.h"
#include "OpsinTools.h"
#include "WordType.h"
#include "WordTools.h"
#include "ReverseParseRules.h"
#include "ParseWord.h"


using org::apache::log4j::Logger;

//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;

Parser::Parser() throw(IOException) : tokeniser(new Tokeniser(parseRules)), wordRules(new WordRules(resources)),
                                      resourceManager(new ResourceManager(resources)),
                                      parseRules(new ParseRules(this->resourceManager)) {
    ResourceGetter *resources = new ResourceGetter(L"uk/ac/cam/ch/wwmm/opsin/resources/");
}

Parser::Parser(WordRules *wordRules, Tokeniser *tokeniser, ResourceManager *resourceManager) : tokeniser(tokeniser),
                                                                                               resourceManager(
                                                                                                       resourceManager),
                                                                                               parseRules(
                                                                                                       tokeniser->getParseRules()) {
    this->wordRules = wordRules;
}

std::vector<Element *>
Parser::parse(NameToStructureConfig *n2sConfig, const std::wstring &name) throw(ParsingException) {
    std::vector<boost::optional<int>> componentRatios;
    if (boost::ends_with(name, L")") || boost::ends_with(name, L"]") || boost::ends_with(name, L"}")) {
        Matcher *m = matchStoichiometryIndication->matcher(name);
        if (m->find()) {
            componentRatios = processStoichiometryIndication(m->group());
            name = m->replaceAll(L"");
        }
    }
    Parse *parse = nullptr;
    if (name.find(L", ") != std::wstring::npos) {
        try {
            TokenizationResult *tokenizationResult = tokeniser->tokenize(CASTools::uninvertCASName(name, parseRules),
                                                                         false);
            if (tokenizationResult->isSuccessfullyTokenized()) {
                parse = tokenizationResult->getParse();
            }
        }
        catch (const ParsingException &ignored) {
        }
    } else if (name.find(L"; ") !=
               std::wstring::npos) { //a mixture, spaces are sufficient for OPSIN to treat as a mixture. These spaces for obvious reasons must not be removed
        TokenizationResult *tokenizationResult = tokeniser->tokenize(
                matchSemiColonSpace->matcher(name).replaceAll(L" "), false);
        if (tokenizationResult->isSuccessfullyTokenized()) {
            parse = tokenizationResult->getParse();
        }
    }
    bool allowSpaceRemoval;
    if (parse == nullptr) {
        allowSpaceRemoval = true;
        TokenizationResult *tokenizationResult = tokeniser->tokenize(name, true);
        if (tokenizationResult->isSuccessfullyTokenized()) {
            parse = tokenizationResult->getParse();
        } else {
            if (n2sConfig->isDetailedFailureAnalysis()) {
                generateExactParseFailureReason(tokenizationResult, name);
            } else {
                throw ParsingException(
                        name + std::wstring(L" is unparsable due to the following being uninterpretable: ") +
                        tokenizationResult->getUninterpretableName() +
                        std::wstring(L" The following was not parseable: ") + tokenizationResult->getUnparsableName());
            }
        }
    } else {
        allowSpaceRemoval = false;
    }

    std::vector<Parse *> parses = generateParseCombinations(parse);
    if (parses.empty()) {
        throw ParsingException(std::wstring(L"No parses could be found for ") + name);
    }

    std::vector<Element *> results;
    ParsingException *preciseException = nullptr;
    for (auto pp : parses) {
        Element *moleculeEl = new GroupingEl(XmlDeclarations::MOLECULE_EL);
        Attribute tempVar(XmlDeclarations::NAME_ATR, name);
        moleculeEl->addAttribute(&tempVar);
        for (auto pw : pp->getWords()) {
            Element *word = new GroupingEl(XmlDeclarations::WORD_EL);
            moleculeEl->addChild(word);
            if (pw->getParseTokens().size() != 1) {
                throw ParsingException(
                        L"OPSIN bug: parseWord should have exactly 1 annotations after creating additional parses step");
            }
            ParseTokens *tokensForWord = pw->getParseTokens().get(0);
            WordType wordType = OpsinTools::determineWordType(tokensForWord->getAnnotations());
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
            Attribute tempVar2(XmlDeclarations::TYPE_ATR, wordType::toString());
            word->addAttribute(&tempVar2);
            if (pw->getWord().startsWith(L"-")) { //we want -functionalterm to be the same as functionalterm
                Attribute tempVar3(XmlDeclarations::VALUE_ATR, pw->getWord().substr(1));
                word->addAttribute(&tempVar3);
            } else {
                Attribute tempVar4(XmlDeclarations::VALUE_ATR, pw->getWord());
                word->addAttribute(&tempVar4);
            }
            writeWordXML(word, tokensForWord->getTokens(),
                         WordTools::chunkAnnotations(tokensForWord->getAnnotations()));
        }
        /* All words are placed into a wordRule.
         * Often multiple words in the same wordRule.
         * WordRules can be nested within each other e.g. in Carbonyl cyanide m-chlorophenyl hydrazone ->
         * <wr><wr>Carbonyl cyanide</wr> m-chlorophenyl hydrazone </wr>
         */
        try {
            wordRules->groupWordsIntoWordRules(moleculeEl, n2sConfig, allowSpaceRemoval, componentRatios);
        }
        catch (const ParsingException &e) {
            if (LOG->isDebugEnabled()) {
                LOG->debug(e->what(), e);
            }
            // Using that parse no word rules matched
            continue;
        }
        try {
            if (componentRatios.size() > 0) {
                applyStoichiometryIndicationToWordRules(moleculeEl, componentRatios);
            }
            if (moleculeEl->getAttributeValue(XmlDeclarations::ISSALT_ATR) != L"" &&
                moleculeEl->getChildElements(XmlDeclarations::WORDRULE_EL).size() < 2) {
                throw ParsingException(name + std::wstring(
                        L" is apparently a salt, but the name only contained one component. The name could be describing a class of compounds"));
            }
            results.push_back(moleculeEl);
        }
        catch (const ParsingException &e) {
            preciseException = e;
        }

    }
    if (results.empty()) {
        if (preciseException != nullptr) {
            throw preciseException;
        }
        throw ParsingException(name + std::wstring(
                L" could be parsed but OPSIN was unsure of the meaning of the words. This error will occur, by default, if a name is just a substituent"));
    }

    return results;
}

std::vector<boost::optional<int>>
Parser::processStoichiometryIndication(const std::wstring &ratioString) throw(ParsingException) {
    ratioString = boost::trim_copy(ratioString);
    ratioString = ratioString.substr(1, (ratioString.length() - 1) - 1);
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
    std::vector<std::wstring> ratioStrings = ratioString.split(L":");
    if (ratioStrings.size() == 1) {
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
        ratioStrings = ratioString.split(L"/");
    }
    std::vector<boost::optional<int>> componentRatios = std::vector<boost::optional<int> *>(ratioStrings.size());
    for (int i = 0; i < ratioStrings.size(); i++) {
        std::wstring currentRatio = ratioStrings[i];
        if (currentRatio.find(L"/") != std::wstring::npos) {
            throw ParsingException(L"Unexpected / in component ratio declaration");
        }
        if (currentRatio == L"?") {
            componentRatios[i] = 1;
        } else {
            componentRatios[i] = std::stoi(currentRatio);
        }
    }
    return componentRatios;
}

void Parser::generateExactParseFailureReason(TokenizationResult *tokenizationResult,
                                             const std::wstring &name) throw(ParsingException) {
    ReverseParseRules *reverseParseRules;
    try {
        reverseParseRules = new ReverseParseRules(resourceManager);
    }
    catch (const IOException &e) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("Failed to load resources for parsing names from right to left!",e);
        throw std::exception();
    }
    std::wstring uninterpretableLR = tokenizationResult->getUninterpretableName();
    std::wstring unparseableLR = tokenizationResult->getUnparsableName();
    TokenizationResult *reverseTokenizationResult = tokeniser->tokenizeRightToLeft(reverseParseRules, uninterpretableLR,
                                                                                   true);
    std::wstring uninterpretableRL = reverseTokenizationResult->getUninterpretableName();
    std::wstring unparseableRL = reverseTokenizationResult->getUnparsableName();
    int indiceToTruncateUpTo = uninterpretableLR.length() - unparseableLR.length();
    StringBuilder *message = new StringBuilder();
    message->append(name);
    if (uninterpretableRL != L"") {
        message->append(L" was uninterpretable due to the following section of the name: ");
        message->append(uninterpretableRL);
        if (indiceToTruncateUpTo <= unparseableRL.length()) {
            std::wstring uninterpretableInContext = unparseableRL.substr(indiceToTruncateUpTo);
            if (uninterpretableInContext != L"") {
                message->append(L"  The following was not understandable in the context it was used: ");
                message->append(uninterpretableInContext);
            }
        }
    } else {
        message->append(L" has no tokens unknown to OPSIN but does not conform to its grammar. ");
        message->append(L"From left to right it is unparsable due to the following being uninterpretable:");
        message->append(uninterpretableLR);
        message->append(L" The following of which was not parseable: ");
        message->append(unparseableLR);
    }
    throw ParsingException(message->toString());
}

std::vector<Parse *> Parser::generateParseCombinations(Parse *parse) throw(ParsingException) {
    int numberOfCombinations = 1;
    std::vector<ParseWord *> parseWords = parse->getWords();
    for (auto pw : parseWords) {
        int parsesForWord = pw->getParseTokens().size();
        numberOfCombinations *= parsesForWord;
        if (numberOfCombinations > 128) { //checked here to avoid integer overflow on inappropriate input
            throw ParsingException(
                    L"Too many different combinations of word interpretation are possible (>128) i.e. name contains too many terms that OPSIN finds ambiguous to interpret");
        }
    }
    if (numberOfCombinations == 1) {
        return Arrays::asList(parse);
    }
    std::vector<Parse *> parses;

    Deque < Parse * > *parseQueue = new ArrayDeque<Parse *>();
    Parse tempVar(parse->getName());
    parseQueue->add(&tempVar);
    while (!parseQueue->isEmpty()) {
        Parse *currentParse = parseQueue->removeFirst();
        int wordsInCurrentParse = currentParse->getWords().size();
        if (wordsInCurrentParse == parseWords.size()) {
            parses.push_back(currentParse);
        } else {
            ParseWord *referenceWord = parseWords[wordsInCurrentParse];
            std::vector<ParseTokens *> referenceWordParseTokens = referenceWord->getParseTokens();
            for (int i = referenceWordParseTokens.size() - 1; i >= 0; i--) {
                ParseTokens *parseTokens = referenceWordParseTokens[i];
                Parse *parseWithNextWord = i > 0 ? currentParse->deepCopy() : currentParse;
                ParseWord *newParseWord = new ParseWord(referenceWord->getWord(), Arrays::asList(parseTokens));
                parseWithNextWord->addWord(newParseWord);
                parseQueue->add(parseWithNextWord);
            }
        }
    }
    return parses;
}

void Parser::writeWordXML(Element *wordEl, std::vector<std::wstring> &tokens,
                          std::vector<std::vector<wchar_t>> &annotations) throw(ParsingException) {
    int annotNumber = 0;
    int annotPos = 0;
    Element *chunk = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
    wordEl->addChild(chunk);
    Element *lastTokenElement = nullptr;
    for (auto token : tokens) {
        if (annotPos >= annotations[annotNumber].size()) {
            annotPos = 0;
            annotNumber++;
            chunk = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
            wordEl->addChild(chunk);
            lastTokenElement = nullptr;
        }
        Element *tokenElement = resourceManager->makeTokenElement(token, annotations[annotNumber][annotPos]);
        if (tokenElement != nullptr) { //null for tokens that have ignoreWhenWritingXML set
            chunk->addChild(tokenElement);
            lastTokenElement = tokenElement;
        } else if (lastTokenElement != nullptr && token.length() > 0) {
            if (lastTokenElement->getAttribute(XmlDeclarations::SUBSEQUENTUNSEMANTICTOKEN_ATR) != nullptr) {
                lastTokenElement->getAttribute(XmlDeclarations::SUBSEQUENTUNSEMANTICTOKEN_ATR)->setValue(
                        lastTokenElement->getAttributeValue(XmlDeclarations::SUBSEQUENTUNSEMANTICTOKEN_ATR) + token);
            } else {
                Attribute tempVar(XmlDeclarations::SUBSEQUENTUNSEMANTICTOKEN_ATR, token);
                lastTokenElement->addAttribute(&tempVar);
            }
        }
        annotPos++;
    }
    WordType wordType = StringHelper::fromString<WordType>(wordEl->getAttributeValue(XmlDeclarations::TYPE_ATR));
    if (wordType == WordType::full) {
        chunk->setName(XmlDeclarations::ROOT_EL);
    } else if (wordType == WordType::functionalTerm) {
        chunk->setName(XmlDeclarations::FUNCTIONALTERM_EL);
    }
}

void Parser::applyStoichiometryIndicationToWordRules(Element *moleculeEl,
                                                     std::vector<Integer> &componentRatios) throw(ParsingException) {
    std::vector<Element *> wordRules = moleculeEl->getChildElements(XmlDeclarations::WORDRULE_EL);
    if (wordRules.size() != componentRatios.size()) {
        throw ParsingException(std::wstring(
                L"Component and stoichiometry indication indication mismatch. OPSIN believes there to be ") +
                               wordRules.size() + std::wstring(L" components but ") + componentRatios.size() +
                               std::wstring(L" ratios were given!"));
    }
    for (int i = 0; i < componentRatios.size(); i++) {
        Attribute tempVar(XmlDeclarations::STOICHIOMETRY_ATR, std::to_wstring(componentRatios[i]));
        wordRules[i]->addAttribute(&tempVar);
    }
}
