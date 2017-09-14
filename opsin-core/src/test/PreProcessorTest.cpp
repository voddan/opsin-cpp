#include "PreProcessorTest.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/PreProcessingException.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/PreProcessor.h"


//						import static org.junit.Assert.*;
using org::junit::Test;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=PreProcessingException.class) public void testPreProcessBlankThrows() throws PreProcessingException
void PreProcessorTest::testPreProcessBlankThrows() throw(PreProcessingException) {
    PreProcessor::preProcess(L"");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testPreProcessConvertsDollarA() throws PreProcessingException
void PreProcessorTest::testPreProcessConvertsDollarA() throw(PreProcessingException) {
    assertEquals(L"Convert dollar-a", L"alpha-bromo", PreProcessor::preProcess(L"$a-bromo"));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testPreProcessConvertsDollarB() throws PreProcessingException
void PreProcessorTest::testPreProcessConvertsDollarB() throw(PreProcessingException) {
    assertEquals(L"Convert dollar-b", L"beta-bromo", PreProcessor::preProcess(L"$b-bromo"));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testPreProcessConvertsDollarG() throws PreProcessingException
void PreProcessorTest::testPreProcessConvertsDollarG() throw(PreProcessingException) {
    assertEquals(L"Convert dollar-g", L"gamma-bromo", PreProcessor::preProcess(L"$g-bromo"));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testPreProcessConvertsDollarD() throws PreProcessingException
void PreProcessorTest::testPreProcessConvertsDollarD() throw(PreProcessingException) {
    assertEquals(L"Convert dollar-d", L"delta-bromo", PreProcessor::preProcess(L"$d-bromo"));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testPreProcessConvertsDollarE() throws PreProcessingException
void PreProcessorTest::testPreProcessConvertsDollarE() throw(PreProcessingException) {
    assertEquals(L"Convert dollar-e", L"epsilon-bromo", PreProcessor::preProcess(L"$e-bromo"));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testPreProcessConvertsDollarL() throws PreProcessingException
void PreProcessorTest::testPreProcessConvertsDollarL() throw(PreProcessingException) {
    assertEquals(L"Convert dollar-l", L"lambda-bromo", PreProcessor::preProcess(L"$l-bromo"));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testPreProcessConvertsGreekLetterToWord() throws PreProcessingException
void PreProcessorTest::testPreProcessConvertsGreekLetterToWord() throw(PreProcessingException) {
    assertEquals(L"Convert greek to word", L"alpha-bromo", PreProcessor::preProcess(L"\u03b1-bromo"));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testPreProcessConvertsSulphToSulf() throws PreProcessingException
void PreProcessorTest::testPreProcessConvertsSulphToSulf() throw(PreProcessingException) {
    assertEquals(L"Converts 'sulph' to 'sulph'", L"sulfur dioxide", PreProcessor::preProcess(L"sulphur dioxide"));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRemovalOfDotsFromGreekWords1() throws PreProcessingException
void PreProcessorTest::testRemovalOfDotsFromGreekWords1() throw(PreProcessingException) {
    assertEquals(L"Converts '.alpha.' to 'alpha'", L"alpha-methyl-toluene",
                 PreProcessor::preProcess(L".alpha.-methyl-toluene"));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRemovalOfDotsFromGreekWords2() throws PreProcessingException
void PreProcessorTest::testRemovalOfDotsFromGreekWords2() throw(PreProcessingException) {
    assertEquals(L"alphabetaeta", PreProcessor::preProcess(L".alpha..beta..eta."));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testHtmlGreeks() throws PreProcessingException
void PreProcessorTest::testHtmlGreeks() throw(PreProcessingException) {
    assertEquals(L"alpha-methyl-toluene", PreProcessor::preProcess(L"&alpha;-methyl-toluene"));
    assertEquals(L"beta-methyl-styrene", PreProcessor::preProcess(L"&BETA;-methyl-styrene"));
}
