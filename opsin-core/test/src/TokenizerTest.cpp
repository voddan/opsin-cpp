#include "TokenizerTest.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/Tokeniser.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/ReverseParseRules.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/ResourceGetter.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/ResourceManager.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/ParseRules.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/ParsingException.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/TokenizationResult.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/Parse.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/ParseWord.h"


//						import static org.junit.Assert.*;
using org::junit::AfterClass;
using org::junit::BeforeClass;
using org::junit::Test;
Tokeniser *TokenizerTest::tokenizer;
ReverseParseRules *TokenizerTest::reverseParseRules;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @BeforeClass public static void setUp() throws java.io.IOException
void TokenizerTest::setUp() throw(IOException) {
    ResourceGetter *rg = new ResourceGetter(L"uk/ac/cam/ch/wwmm/opsin/resources/");
    ResourceManager *rm = new ResourceManager(rg);
    ParseRules tempVar(rm);
    tokenizer = new Tokeniser(&tempVar);
    reverseParseRules = new ReverseParseRules(rm);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @AfterClass public static void cleanUp()
void TokenizerTest::cleanUp() {
    tokenizer = nullptr;
    reverseParseRules = nullptr;
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hexane() throws ParsingException
void TokenizerTest::hexane() throw(ParsingException) {
    TokenizationResult *result = tokenizer->tokenize(L"hexane", true);
    assertEquals(true, result->isSuccessfullyTokenized());
    assertEquals(true, result->isFullyInterpretable());
    assertEquals(L"", result->getUninterpretableName());
    assertEquals(L"", result->getUnparsableName());
    assertEquals(L"", result->getUnparsedName());
    Parse *parse = result->getParse();
    assertEquals(L"One Word", 1, parse->getWords().size());
    ParseWord *w = parse->getWords()[0];
    assertEquals(L"One Parse", 1, w->getParseTokens().size());
    std::vector<std::wstring> tokens = w->getParseTokens()[0]->getTokens();
    assertEquals(L"Three tokens", 3, tokens.size());
    assertEquals(L"First token: hex", L"hex", tokens[0]);
    assertEquals(L"Second token: ane", L"ane", tokens[1]);
    assertEquals(L"Third token: end of main group", L"", tokens[2]);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hexachlorohexane() throws ParsingException
void TokenizerTest::hexachlorohexane() throw(ParsingException) {
    Parse *parse = tokenizer->tokenize(L"hexachlorohexane", true)->getParse();
    assertEquals(L"One Word", 1, parse->getWords().size());
    ParseWord *w = parse->getWords()[0];
    assertEquals(L"One Parse", 1, w->getParseTokens().size());
    std::vector<std::wstring> tokens = w->getParseTokens()[0]->getTokens();
    assertEquals(L"Seven tokens", 7, tokens.size());
    assertEquals(L"First token: hex", L"hex", tokens[0]);
    assertEquals(L"Second token: a", L"a", tokens[1]);
    assertEquals(L"Third token: chloro", L"chloro", tokens[2]);
    assertEquals(L"Fourth token: end of main substituent", L"", tokens[3]);
    assertEquals(L"Fifth token: hex", L"hex", tokens[4]);
    assertEquals(L"Sixth token: ane", L"ane", tokens[5]);
    assertEquals(L"Seventh token: end of main group", L"", tokens[6]);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void ethylChloride() throws ParsingException
void TokenizerTest::ethylChloride() throw(ParsingException) {
    Parse *parse = tokenizer->tokenize(L"ethyl chloride", true)->getParse();
    assertEquals(L"Two Words", 2, parse->getWords().size());
    ParseWord *w = parse->getWord(0);
    assertEquals(L"One Parse", 1, w->getParseTokens().size());
    std::vector<std::wstring> tokens = w->getParseTokens()[0]->getTokens();
    assertEquals(L"Three tokens", 3, tokens.size());
    assertEquals(L"First token: eth", L"eth", tokens[0]);
    assertEquals(L"Second token: yl", L"yl", tokens[1]);
    assertEquals(L"Third token: end of substituent", L"", tokens[2]);
    w = parse->getWord(1);
    assertEquals(L"One Parse", 1, w->getParseTokens().size());
    tokens = w->getParseTokens()[0]->getTokens();
    assertEquals(L"Two tokens", 2, tokens.size());
    assertEquals(L"First token: chloride", L"chloride", tokens[0]);
    assertEquals(L"Second token: end of functionalTerm", L"", tokens[1]);


    parse = tokenizer->tokenize(L"ethylchloride", true)->getParse(); //missing space
    assertEquals(L"Two Words", 2, parse->getWords().size());
    w = parse->getWord(0);
    assertEquals(L"One Parse", 1, w->getParseTokens().size());
    tokens = w->getParseTokens()[0]->getTokens();
    assertEquals(L"Three tokens", 3, tokens.size());
    assertEquals(L"First token: eth", L"eth", tokens[0]);
    assertEquals(L"Second token: yl", L"yl", tokens[1]);
    assertEquals(L"Third token: end of substituent", L"", tokens[2]);
    w = parse->getWord(1);
    assertEquals(L"One Parse", 1, w->getParseTokens().size());
    tokens = w->getParseTokens()[0]->getTokens();
    assertEquals(L"Two tokens", 2, tokens.size());
    assertEquals(L"First token: chloride", L"chloride", tokens[0]);
    assertEquals(L"Second token: end of functionalTerm", L"", tokens[1]);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hexachlorohexaneeeeeee() throws ParsingException
void TokenizerTest::hexachlorohexaneeeeeee() throw(ParsingException) {
    TokenizationResult *result = tokenizer->tokenize(L"hexachlorohexaneeeeeee", true);
    assertEquals(L"Unparsable", false, result->isSuccessfullyTokenized());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void bracketedHexachlorohexane() throws ParsingException
void TokenizerTest::bracketedHexachlorohexane() throw(ParsingException) {
    Parse *parse = tokenizer->tokenize(L"(hexachloro)hexane", true)->getParse();
    assertEquals(L"One Word", 1, parse->getWords().size());
    ParseWord *w = parse->getWords()[0];
    assertEquals(L"One Parse", 1, w->getParseTokens().size());
    std::vector<std::wstring> tokens = w->getParseTokens()[0]->getTokens();
    assertEquals(L"Nine tokens", 9, tokens.size());
    assertEquals(L"First token: {", L"(", tokens[0]);
    assertEquals(L"Second token: hex", L"hex", tokens[1]);
    assertEquals(L"Third token: a", L"a", tokens[2]);
    assertEquals(L"Fourth token: chloro", L"chloro", tokens[3]);
    assertEquals(L"Fifth token: )", L")", tokens[4]);
    assertEquals(L"Sixth token: end of main substituent", L"", tokens[5]);
    assertEquals(L"Seventh token: hex", L"hex", tokens[6]);
    assertEquals(L"Eigth token: ane", L"ane", tokens[7]);
    assertEquals(L"Ninth token: end of main group", L"", tokens[8]);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void methyl() throws ParsingException
void TokenizerTest::methyl() throw(ParsingException) {
    Parse *parse = tokenizer->tokenize(L"methyl", true)->getParse();
    assertEquals(L"One Word", 1, parse->getWords().size());
    ParseWord *w = parse->getWords()[0];
    assertEquals(L"One Parse", 1, w->getParseTokens().size());
    std::vector<std::wstring> tokens = w->getParseTokens()[0]->getTokens();
    assertEquals(L"Three tokens", 3, tokens.size());
    assertEquals(L"First token: meth", L"meth", tokens[0]);
    assertEquals(L"Second token: yl", L"yl", tokens[1]);
    assertEquals(L"Third token: end of substituent", L"", tokens[2]);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void aceticacid() throws ParsingException
void TokenizerTest::aceticacid() throw(ParsingException) {
    Parse *parse = tokenizer->tokenize(L"acetic acid", true)->getParse();
    assertEquals(L"One Word", 1, parse->getWords().size());
    ParseWord *w = parse->getWords()[0];
    assertEquals(L"One Parse", 1, w->getParseTokens().size());
    std::vector<std::wstring> tokens = w->getParseTokens()[0]->getTokens();
    assertEquals(L"Three tokens", 3, tokens.size());
    assertEquals(L"First token: acet", L"acet", tokens[0]);
    assertEquals(L"Second token: ic acid", L"ic acid", tokens[1]);
    assertEquals(L"Third token: end of main group", L"", tokens[2]);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void acceptableInterWordBreaks() throws ParsingException
void TokenizerTest::acceptableInterWordBreaks() throw(ParsingException) {
    assertEquals(true, tokenizer->tokenize(L"methane ethane", false)->isSuccessfullyTokenized());
    assertEquals(true, tokenizer->tokenize(L"methane-ethane", false)->isSuccessfullyTokenized());
    assertEquals(true, tokenizer->tokenize(L"methane - ethane", false)->isSuccessfullyTokenized());
    assertEquals(false, tokenizer->tokenize(L"methane -ethane", false)->isSuccessfullyTokenized());
    assertEquals(false, tokenizer->tokenize(L"methane - ", false)->isSuccessfullyTokenized());

    assertEquals(true, tokenizer->tokenizeRightToLeft(reverseParseRules, L"methane ethane",
                                                      false)->isSuccessfullyTokenized());
    assertEquals(true, tokenizer->tokenizeRightToLeft(reverseParseRules, L"methane-ethane",
                                                      false)->isSuccessfullyTokenized());
    assertEquals(true, tokenizer->tokenizeRightToLeft(reverseParseRules, L"methane - ethane",
                                                      false)->isSuccessfullyTokenized());
    assertEquals(false, tokenizer->tokenizeRightToLeft(reverseParseRules, L"methane -ethane",
                                                       false)->isSuccessfullyTokenized());
    assertEquals(false,
                 tokenizer->tokenizeRightToLeft(reverseParseRules, L"methane - ", false)->isSuccessfullyTokenized());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void compoundWithValidUse() throws ParsingException
void TokenizerTest::compoundWithValidUse() throw(ParsingException) {
    TokenizationResult *result = tokenizer->tokenize(L"benzene compound with toluene", true);
    assertEquals(true, result->isSuccessfullyTokenized());
    TokenizationResult *result2 = tokenizer->tokenize(L"benzene and toluene", true);
    assertEquals(true, result2->isSuccessfullyTokenized());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void compoundWithInvalidUse1() throws ParsingException
void TokenizerTest::compoundWithInvalidUse1() throw(ParsingException) {
    TokenizationResult *result = tokenizer->tokenize(L"ethyl and toluene", true);
    assertEquals(false, result->isSuccessfullyTokenized());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void compoundWithInvalidUse2() throws ParsingException
void TokenizerTest::compoundWithInvalidUse2() throw(ParsingException) {
    TokenizationResult *result = tokenizer->tokenize(L"and benzene", true);
    assertEquals(false, result->isSuccessfullyTokenized());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void CCCP() throws ParsingException
void TokenizerTest::CCCP() throw(ParsingException) {
    TokenizationResult *result = tokenizer->tokenize(L"Carbonyl cyanide m-chlorophenyl oxime", true);
    assertEquals(true, result->isSuccessfullyTokenized());
    assertEquals(true, result->isFullyInterpretable());
    assertEquals(L"", result->getUninterpretableName());
    assertEquals(L"", result->getUnparsableName());
    assertEquals(L"", result->getUnparsedName());
    Parse *parse = result->getParse();
    assertEquals(L"Four Words", 4, parse->getWords().size());
    ParseWord *w = parse->getWords()[0];
    assertEquals(L"One Parse", 1, w->getParseTokens().size());
    std::vector<std::wstring> tokens = w->getParseTokens()[0]->getTokens();
    assertEquals(L"Three tokens", 3, tokens.size());
    assertEquals(L"First token: carbon", L"carbon", tokens[0]);
    assertEquals(L"Second token: yl", L"yl", tokens[1]);
    assertEquals(L"Third token: end of  substituent", L"", tokens[2]);

    w = parse->getWords()[1];
    assertEquals(L"One Parse", 1, w->getParseTokens().size());
    tokens = w->getParseTokens()[0]->getTokens();
    assertEquals(L"Two tokens", 2, tokens.size());
    assertEquals(L"First token: cyanide", L"cyanide", tokens[0]);
    assertEquals(L"Second token: end of functionalTerm", L"", tokens[1]);

    w = parse->getWords()[2];
    assertEquals(L"One Parse", 1, w->getParseTokens().size());
    tokens = w->getParseTokens()[0]->getTokens();
    assertEquals(L"Five tokens", 5, tokens.size());
    assertEquals(L"First token: m-", L"m-", tokens[0]);
    assertEquals(L"Second token: chloro", L"chloro", tokens[1]);
    assertEquals(L"Third token: end of  substituent", L"", tokens[2]);
    assertEquals(L"Fourth token: phenyl", L"phenyl", tokens[3]);
    assertEquals(L"Fifth token: end of  substituent", L"", tokens[4]);

    w = parse->getWords()[3];
    assertEquals(L"One Parse", 1, w->getParseTokens().size());
    tokens = w->getParseTokens()[0]->getTokens();
    assertEquals(L"Two tokens", 2, tokens.size());
    assertEquals(L"First token: oxime", L"oxime", tokens[0]);
    assertEquals(L"Second token: end of functionalTerm", L"", tokens[1]);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void CCCP_RL() throws ParsingException
void TokenizerTest::CCCP_RL() throw(ParsingException) {
    TokenizationResult *result = tokenizer->tokenizeRightToLeft(reverseParseRules,
                                                                L"Carbonyl cyanide m-chlorophenyl oxime", true);
    assertEquals(true, result->isSuccessfullyTokenized());
    assertEquals(true, result->isFullyInterpretable());
    assertEquals(L"", result->getUninterpretableName());
    assertEquals(L"", result->getUnparsableName());
    assertEquals(L"", result->getUnparsedName());
    Parse *parse = result->getParse();
    assertEquals(L"Four Words", 4, parse->getWords().size());
    ParseWord *w = parse->getWords()[0];
    assertEquals(L"One Parse", 1, w->getParseTokens().size());
    std::vector<std::wstring> tokens = w->getParseTokens()[0]->getTokens();
    assertEquals(L"Three tokens", 3, tokens.size());
    assertEquals(L"First token: carbon", L"carbon", tokens[0]);
    assertEquals(L"Second token: yl", L"yl", tokens[1]);
    assertEquals(L"Third token: end of  substituent", L"", tokens[2]);

    w = parse->getWords()[1];
    assertEquals(L"One Parse", 1, w->getParseTokens().size());
    tokens = w->getParseTokens()[0]->getTokens();
    assertEquals(L"Two tokens", 2, tokens.size());
    assertEquals(L"First token: cyanide", L"cyanide", tokens[0]);
    assertEquals(L"Second token: end of functionalTerm", L"", tokens[1]);

    w = parse->getWords()[2];
    assertEquals(L"One Parse", 1, w->getParseTokens().size());
    tokens = w->getParseTokens()[0]->getTokens();
    assertEquals(L"Five tokens", 5, tokens.size());
    assertEquals(L"First token: m-", L"m-", tokens[0]);
    assertEquals(L"Second token: chloro", L"chloro", tokens[1]);
    assertEquals(L"Third token: end of  substituent", L"", tokens[2]);
    assertEquals(L"Fourth token: phenyl", L"phenyl", tokens[3]);
    assertEquals(L"Fifth token: end of  substituent", L"", tokens[4]);

    w = parse->getWords()[3];
    assertEquals(L"One Parse", 1, w->getParseTokens().size());
    tokens = w->getParseTokens()[0]->getTokens();
    assertEquals(L"Two tokens", 2, tokens.size());
    assertEquals(L"First token: oxime", L"oxime", tokens[0]);
    assertEquals(L"Second token: end of functionalTerm", L"", tokens[1]);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void partiallyInterpretatableLR() throws ParsingException
void TokenizerTest::partiallyInterpretatableLR() throw(ParsingException) {
    TokenizationResult *result = tokenizer->tokenize(L"ethyl-2H-foo|ene", true);
    assertEquals(false, result->isSuccessfullyTokenized());
    assertEquals(false, result->isFullyInterpretable());
    assertEquals(L"2H-foo|ene", result->getUninterpretableName());
    assertEquals(L"foo|ene", result->getUnparsableName());
    assertEquals(L"ethyl-2H-foo|ene", result->getUnparsedName());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void partiallyInterpretatableRL1() throws ParsingException
void TokenizerTest::partiallyInterpretatableRL1() throw(ParsingException) {
    TokenizationResult *result = tokenizer->tokenizeRightToLeft(reverseParseRules, L"ethyl-2H-foo|ene", true);
    assertEquals(false, result->isSuccessfullyTokenized());
    assertEquals(false, result->isFullyInterpretable());
    assertEquals(L"ethyl-2H-foo|ene", result->getUninterpretableName());
    assertEquals(L"ethyl-2H-foo|", result->getUnparsableName());
    assertEquals(L"ethyl-2H-foo|ene", result->getUnparsedName());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void partiallyInterpretatableRL2() throws ParsingException
void TokenizerTest::partiallyInterpretatableRL2() throw(ParsingException) {
    TokenizationResult *result = tokenizer->tokenizeRightToLeft(reverseParseRules, L"fooylpyridine oxide", true);
    assertEquals(false, result->isSuccessfullyTokenized());
    assertEquals(false, result->isFullyInterpretable());
    assertEquals(L"fooyl", result->getUninterpretableName());
    assertEquals(L"f", result->getUnparsableName()); //o as in the end of thio then oyl
    assertEquals(L"fooylpyridine", result->getUnparsedName());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void tokenizeDoesNotTokenizeUnTokenizableName() throws ParsingException
void TokenizerTest::tokenizeDoesNotTokenizeUnTokenizableName() throw(ParsingException) {
    TokenizationResult *result = tokenizer->tokenize(L"ethyl acet|foo toluene", true);
    assertEquals(false, result->isSuccessfullyTokenized());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void tokenizePreservesSpacesInUninterpretableNameLR1() throws ParsingException
void TokenizerTest::tokenizePreservesSpacesInUninterpretableNameLR1() throw(ParsingException) {
    TokenizationResult *result = tokenizer->tokenize(L"ethyl acet|foo toluene", true);
    assertEquals(L"acet|foo toluene", result->getUninterpretableName());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void tokenizePreservesSpacesInUnparsableNameLR1() throws ParsingException
void TokenizerTest::tokenizePreservesSpacesInUnparsableNameLR1() throw(ParsingException) {
    TokenizationResult *result = tokenizer->tokenize(L"ethyl acet|foo toluene", true);
    assertEquals(L"|foo toluene", result->getUnparsableName());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void tokenizePreservesSpacesInUnparsedNameLR1() throws ParsingException
void TokenizerTest::tokenizePreservesSpacesInUnparsedNameLR1() throw(ParsingException) {
    TokenizationResult *result = tokenizer->tokenize(L"ethyl acet|foo toluene", true);
    assertEquals(L"acet|foo toluene", result->getUnparsedName());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void tokenizePreservesSpacesInUninterpretableNameLR2() throws ParsingException
void TokenizerTest::tokenizePreservesSpacesInUninterpretableNameLR2() throw(ParsingException) {
    TokenizationResult *result = tokenizer->tokenize(L"eth yl acet|foo toluene", true);
    assertEquals(L"acet|foo toluene", result->getUninterpretableName());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void tokenizePreservesSpacesInUnparsableNameLR2() throws ParsingException
void TokenizerTest::tokenizePreservesSpacesInUnparsableNameLR2() throw(ParsingException) {
    TokenizationResult *result = tokenizer->tokenize(L"eth yl acet|foo toluene", true);
    assertEquals(L"|foo toluene", result->getUnparsableName());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void tokenizePreservesSpacesInUnparsedNameLR2() throws ParsingException
void TokenizerTest::tokenizePreservesSpacesInUnparsedNameLR2() throw(ParsingException) {
    TokenizationResult *result = tokenizer->tokenize(L"eth yl acet|foo toluene", true);
    assertEquals(L"acet|foo toluene", result->getUnparsedName());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void tokenizePreservesSpacesInUninterpretableNameRL1() throws ParsingException
void TokenizerTest::tokenizePreservesSpacesInUninterpretableNameRL1() throw(ParsingException) {
    TokenizationResult *result = tokenizer->tokenizeRightToLeft(reverseParseRules, L"ethyl foo|yl toluene", true);
    assertEquals(L"ethyl foo|yl", result->getUninterpretableName());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void tokenizePreservesSpacesInUnparsableNameRL1() throws ParsingException
void TokenizerTest::tokenizePreservesSpacesInUnparsableNameRL1() throw(ParsingException) {
    TokenizationResult *result = tokenizer->tokenizeRightToLeft(reverseParseRules, L"ethyl foo|yl toluene", true);
    assertEquals(L"ethyl foo|", result->getUnparsableName());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void tokenizePreservesSpacesInUnparsedNameRL1() throws ParsingException
void TokenizerTest::tokenizePreservesSpacesInUnparsedNameRL1() throw(ParsingException) {
    TokenizationResult *result = tokenizer->tokenizeRightToLeft(reverseParseRules, L"ethyl foo|yl toluene", true);
    assertEquals(L"ethyl foo|yl", result->getUnparsedName());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void tokenizePreservesSpacesInUninterpretableNameRL2() throws ParsingException
void TokenizerTest::tokenizePreservesSpacesInUninterpretableNameRL2() throw(ParsingException) {
    TokenizationResult *result = tokenizer->tokenizeRightToLeft(reverseParseRules, L"ethyl foo|yl tolu ene", true);
    assertEquals(L"ethyl foo|yl", result->getUninterpretableName());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void tokenizePreservesSpacesInUnparsableNameRL2() throws ParsingException
void TokenizerTest::tokenizePreservesSpacesInUnparsableNameRL2() throw(ParsingException) {
    TokenizationResult *result = tokenizer->tokenizeRightToLeft(reverseParseRules, L"ethyl foo|yl tolu ene", true);
    assertEquals(L"ethyl foo|", result->getUnparsableName());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void tokenizePreservesSpacesInUnparsedNameRL2() throws ParsingException
void TokenizerTest::tokenizePreservesSpacesInUnparsedNameRL2() throw(ParsingException) {
    TokenizationResult *result = tokenizer->tokenizeRightToLeft(reverseParseRules, L"ethyl foo|yl tolu ene", true);
    assertEquals(L"ethyl foo|yl", result->getUnparsedName());
}
