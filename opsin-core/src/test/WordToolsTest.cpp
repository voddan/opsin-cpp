#include "WordToolsTest.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/ParsingException.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/OpsinTools.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/ParseTokens.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/ParseWord.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/WordTools.h"


//						import static org.junit.Assert.*;
using org::junit::Test;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testNormalCase() throws ParsingException
void WordToolsTest::testNormalCase() throw(ParsingException) {
    ParseTokens *pTokens = new ParseTokens(Arrays::asList(L"fooane", L""),
                                           Arrays::asList(L'a', OpsinTools::END_OF_MAINGROUP));
    std::vector<ParseWord *> parseWords = WordTools::splitIntoParseWords(Arrays::asList(pTokens), L"fooane");
    assertEquals(1, parseWords.size());
    assertEquals(L"fooane", parseWords[0]->getWord());
    assertEquals(1, parseWords[0]->getParseTokens()->size());
    assertEquals(pTokens, parseWords[0]->getParseTokens()[0]);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testNormalCase2() throws ParsingException
void WordToolsTest::testNormalCase2() throw(ParsingException) {
    ParseTokens *pTokens = new ParseTokens(Arrays::asList(L"fooyl", L"", L"fooane", L""),
                                           Arrays::asList(L'a', OpsinTools::END_OF_SUBSTITUENT, L'a',
                                                          OpsinTools::END_OF_MAINGROUP));
    std::vector<ParseWord *> parseWords = WordTools::splitIntoParseWords(Arrays::asList(pTokens), L"fooylfooane");
    assertEquals(1, parseWords.size());
    assertEquals(L"fooylfooane", parseWords[0]->getWord());
    assertEquals(1, parseWords[0]->getParseTokens()->size());
    assertEquals(pTokens, parseWords[0]->getParseTokens()[0]);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testNormalCase3() throws ParsingException
void WordToolsTest::testNormalCase3() throw(ParsingException) {
    ParseTokens *pTokens = new ParseTokens(Arrays::asList(L"functionalfoo", L""),
                                           Arrays::asList(L'a', OpsinTools::END_OF_FUNCTIONALTERM));
    std::vector<ParseWord *> parseWords = WordTools::splitIntoParseWords(Arrays::asList(pTokens), L"functionalfoo");
    assertEquals(1, parseWords.size());
    assertEquals(L"functionalfoo", parseWords[0]->getWord());
    assertEquals(1, parseWords[0]->getParseTokens()->size());
    assertEquals(pTokens, parseWords[0]->getParseTokens()[0]);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testNormalCase4() throws ParsingException
void WordToolsTest::testNormalCase4() throw(ParsingException) {
    ParseTokens *pTokens1 = new ParseTokens(Arrays::asList(L"fooane", L""),
                                            Arrays::asList(L'a', OpsinTools::END_OF_MAINGROUP));
    ParseTokens *pTokens2 = new ParseTokens(Arrays::asList(L"fooane", L""),
                                            Arrays::asList(L'b', OpsinTools::END_OF_MAINGROUP));
    std::vector<ParseWord *> parseWords = WordTools::splitIntoParseWords(Arrays::asList(pTokens1, pTokens2), L"fooane");
    assertEquals(1, parseWords.size());
    assertEquals(L"fooane", parseWords[0]->getWord());
    assertEquals(2, parseWords[0]->getParseTokens()->size());
    assertEquals(pTokens1, parseWords[0]->getParseTokens()[0]);
    assertEquals(pTokens2, parseWords[0]->getParseTokens()[1]);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testStartingFunctionalTerm1() throws ParsingException
void WordToolsTest::testStartingFunctionalTerm1() throw(ParsingException) {
    ParseTokens *pTokens = new ParseTokens(Arrays::asList(L"poly", L"", L"foo", L""),
                                           Arrays::asList(L'a', OpsinTools::END_OF_FUNCTIONALTERM, L'a',
                                                          OpsinTools::END_OF_SUBSTITUENT));
    std::vector<ParseWord *> parseWords = WordTools::splitIntoParseWords(Arrays::asList(pTokens), L"polyfoo");
    assertEquals(2, parseWords.size());
    assertEquals(L"poly", parseWords[0]->getWord());
    assertEquals(1, parseWords[0]->getParseTokens()->size());
    ParseTokens *pTokensFunc = new ParseTokens(Arrays::asList(L"poly", L""),
                                               Arrays::asList(L'a', OpsinTools::END_OF_FUNCTIONALTERM));
    assertEquals(pTokensFunc, parseWords[0]->getParseTokens()[0]);
    assertEquals(L"foo", parseWords[1]->getWord());
    assertEquals(1, parseWords[1]->getParseTokens()->size());
    ParseTokens *pTokensGroup = new ParseTokens(Arrays::asList(L"foo", L""),
                                                Arrays::asList(L'a', OpsinTools::END_OF_SUBSTITUENT));
    assertEquals(pTokensGroup, parseWords[1]->getParseTokens()[0]);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testStartingFunctionalTerm2() throws ParsingException
void WordToolsTest::testStartingFunctionalTerm2() throw(ParsingException) {
    ParseTokens *pTokens = new ParseTokens(Arrays::asList(L"poly", L"", L"foo", L""),
                                           Arrays::asList(L'a', OpsinTools::END_OF_FUNCTIONALTERM, L'a',
                                                          OpsinTools::END_OF_MAINGROUP));
    std::vector<ParseWord *> parseWords = WordTools::splitIntoParseWords(Arrays::asList(pTokens), L"polyfoo");
    assertEquals(2, parseWords.size());
    assertEquals(L"poly", parseWords[0]->getWord());
    assertEquals(1, parseWords[0]->getParseTokens()->size());
    ParseTokens *pTokensFunc = new ParseTokens(Arrays::asList(L"poly", L""),
                                               Arrays::asList(L'a', OpsinTools::END_OF_FUNCTIONALTERM));
    assertEquals(pTokensFunc, parseWords[0]->getParseTokens()[0]);
    assertEquals(L"foo", parseWords[1]->getWord());
    assertEquals(1, parseWords[1]->getParseTokens()->size());
    ParseTokens *pTokensGroup = new ParseTokens(Arrays::asList(L"foo", L""),
                                                Arrays::asList(L'a', OpsinTools::END_OF_MAINGROUP));
    assertEquals(pTokensGroup, parseWords[1]->getParseTokens()[0]);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testTerminalFunctionalTerm() throws ParsingException
void WordToolsTest::testTerminalFunctionalTerm() throw(ParsingException) {
    ParseTokens *pTokens = new ParseTokens(Arrays::asList(L"fooyl", L"", L"functionalfoo", L""),
                                           Arrays::asList(L'a', OpsinTools::END_OF_SUBSTITUENT, L'a',
                                                          OpsinTools::END_OF_FUNCTIONALTERM));
    std::vector<ParseWord *> parseWords = WordTools::splitIntoParseWords(Arrays::asList(pTokens),
                                                                         L"fooylfunctionalfoo");
    assertEquals(2, parseWords.size());
    assertEquals(L"fooyl", parseWords[0]->getWord());
    assertEquals(1, parseWords[0]->getParseTokens()->size());
    ParseTokens *pTokensSub = new ParseTokens(Arrays::asList(L"fooyl", L""),
                                              Arrays::asList(L'a', OpsinTools::END_OF_SUBSTITUENT));
    assertEquals(pTokensSub, parseWords[0]->getParseTokens()[0]);
    assertEquals(L"functionalfoo", parseWords[1]->getWord());
    assertEquals(1, parseWords[1]->getParseTokens()->size());
    ParseTokens *pTokensFunc = new ParseTokens(Arrays::asList(L"functionalfoo", L""),
                                               Arrays::asList(L'a', OpsinTools::END_OF_FUNCTIONALTERM));
    assertEquals(pTokensFunc, parseWords[1]->getParseTokens()[0]);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testMultipleParsesTerminalFunctionalTerm() throws ParsingException
void WordToolsTest::testMultipleParsesTerminalFunctionalTerm() throw(ParsingException) {
    ParseTokens *pTokens1 = new ParseTokens(Arrays::asList(L"fooyl", L"", L"functionalfoo", L""),
                                            Arrays::asList(L'a', OpsinTools::END_OF_SUBSTITUENT, L'a',
                                                           OpsinTools::END_OF_FUNCTIONALTERM));
    ParseTokens *pTokens2 = new ParseTokens(Arrays::asList(L"fooyl", L"", L"functionalfoo", L""),
                                            Arrays::asList(L'b', OpsinTools::END_OF_SUBSTITUENT, L'a',
                                                           OpsinTools::END_OF_FUNCTIONALTERM));
    std::vector<ParseWord *> parseWords = WordTools::splitIntoParseWords(Arrays::asList(pTokens1, pTokens2),
                                                                         L"fooylfunctionalfoo");
    assertEquals(2, parseWords.size());
    assertEquals(L"fooyl", parseWords[0]->getWord());
    assertEquals(2, parseWords[0]->getParseTokens()->size());
    ParseTokens *pTokensSub1 = new ParseTokens(Arrays::asList(L"fooyl", L""),
                                               Arrays::asList(L'a', OpsinTools::END_OF_SUBSTITUENT));
    assertEquals(pTokensSub1, parseWords[0]->getParseTokens()[0]);
    ParseTokens *pTokensSub2 = new ParseTokens(Arrays::asList(L"fooyl", L""),
                                               Arrays::asList(L'b', OpsinTools::END_OF_SUBSTITUENT));
    assertEquals(pTokensSub2, parseWords[0]->getParseTokens()[1]);
    assertEquals(L"functionalfoo", parseWords[1]->getWord());
    assertEquals(1, parseWords[1]->getParseTokens()->size());
    ParseTokens *pTokensFunc = new ParseTokens(Arrays::asList(L"functionalfoo", L""),
                                               Arrays::asList(L'a', OpsinTools::END_OF_FUNCTIONALTERM));
    assertEquals(pTokensFunc, parseWords[1]->getParseTokens()[0]);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testMultipleParsesAmbiguousWordTokenisationTerminalFunctionalTerm() throws ParsingException
void WordToolsTest::testMultipleParsesAmbiguousWordTokenisationTerminalFunctionalTerm() throw(ParsingException) {
    ParseTokens *pTokens1 = new ParseTokens(Arrays::asList(L"fooyl", L"", L"functionalfoo", L""),
                                            Arrays::asList(L'a', OpsinTools::END_OF_SUBSTITUENT, L'a',
                                                           OpsinTools::END_OF_FUNCTIONALTERM));
    ParseTokens *pTokens2 = new ParseTokens(Arrays::asList(L"fooylfunc", L"", L"tionalfoo", L""),
                                            Arrays::asList(L'b', OpsinTools::END_OF_SUBSTITUENT, L'a',
                                                           OpsinTools::END_OF_FUNCTIONALTERM));
    std::vector<ParseWord *> parseWords = WordTools::splitIntoParseWords(Arrays::asList(pTokens1, pTokens2),
                                                                         L"fooylfunctionalfoo");
    assertEquals(2, parseWords.size());
    assertEquals(L"fooyl", parseWords[0]->getWord());
    assertEquals(1, parseWords[0]->getParseTokens()->size());
    ParseTokens *pTokensSub = new ParseTokens(Arrays::asList(L"fooyl", L""),
                                              Arrays::asList(L'a', OpsinTools::END_OF_SUBSTITUENT));
    assertEquals(pTokensSub, parseWords[0]->getParseTokens()[0]);
    assertEquals(L"functionalfoo", parseWords[1]->getWord());
    assertEquals(1, parseWords[1]->getParseTokens()->size());
    ParseTokens *pTokensFunc = new ParseTokens(Arrays::asList(L"functionalfoo", L""),
                                               Arrays::asList(L'a', OpsinTools::END_OF_FUNCTIONALTERM));
    assertEquals(pTokensFunc, parseWords[1]->getParseTokens()[0]);
}
