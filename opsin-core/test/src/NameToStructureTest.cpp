#include "NameToStructureTest.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/NameToStructure.h"


//						import static org.junit.Assert.*;
using org::junit::Test;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testNameToStructure()
void NameToStructureTest::testNameToStructure() {
    NameToStructure *nts = NameToStructure::getInstance();
    assertNotNull(L"Got a name to structure convertor", nts);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testParseToCML()
void NameToStructureTest::testParseToCML() {
    NameToStructure *nts = NameToStructure::getInstance();
    std::wstring cml = nts->parseToCML(L"ethane");
    // output is syntactically valid (schema, dictRefs)
    // labels assigned and is correct.
    // contains a molecule with same connectivity as 'frag of CML'

    assertEquals(L"Parsing 'ethane'", std::wstring(L"<cml xmlns=\"http://www.xml-cml.org/schema\" ") +
                                      std::wstring(L"convention=\"conventions:molecular\" ") +
                                      std::wstring(L"xmlns:conventions=\"http://www.xml-cml.org/convention/\" ") +
                                      std::wstring(L"xmlns:cmlDict=\"http://www.xml-cml.org/dictionary/cml/\" ") +
                                      std::wstring(L"xmlns:nameDict=\"http://www.xml-cml.org/dictionary/cml/name/\">") +
                                      std::wstring(L"<molecule id=\"m1\">") +
                                      std::wstring(L"<name dictRef=\"nameDict:unknown\">ethane</name><atomArray>") +
                                      std::wstring(
                                              L"<atom id=\"a1\" elementType=\"C\"><label value=\"1\" dictRef=\"cmlDict:locant\"/><label value=\"alpha\" dictRef=\"cmlDict:locant\"/></atom>") +
                                      std::wstring(
                                              L"<atom id=\"a2\" elementType=\"C\"><label value=\"2\" dictRef=\"cmlDict:locant\"/><label value=\"beta\" dictRef=\"cmlDict:locant\"/></atom>") +
                                      std::wstring(L"<atom id=\"a3\" elementType=\"H\"/>") +
                                      std::wstring(L"<atom id=\"a4\" elementType=\"H\"/>") +
                                      std::wstring(L"<atom id=\"a5\" elementType=\"H\"/>") +
                                      std::wstring(L"<atom id=\"a6\" elementType=\"H\"/>") +
                                      std::wstring(L"<atom id=\"a7\" elementType=\"H\"/>") +
                                      std::wstring(L"<atom id=\"a8\" elementType=\"H\"/>") +
                                      std::wstring(L"</atomArray><bondArray>") +
                                      std::wstring(L"<bond id=\"a1_a2\" atomRefs2=\"a1 a2\" order=\"S\"/>") +
                                      std::wstring(L"<bond id=\"a1_a3\" atomRefs2=\"a1 a3\" order=\"S\"/>") +
                                      std::wstring(L"<bond id=\"a1_a4\" atomRefs2=\"a1 a4\" order=\"S\"/>") +
                                      std::wstring(L"<bond id=\"a1_a5\" atomRefs2=\"a1 a5\" order=\"S\"/>") +
                                      std::wstring(L"<bond id=\"a2_a6\" atomRefs2=\"a2 a6\" order=\"S\"/>") +
                                      std::wstring(L"<bond id=\"a2_a7\" atomRefs2=\"a2 a7\" order=\"S\"/>") +
                                      std::wstring(L"<bond id=\"a2_a8\" atomRefs2=\"a2 a8\" order=\"S\"/>") +
                                      std::wstring(L"</bondArray></molecule></cml>"), cml);
    assertNull(L"Won't parse helloworld", nts->parseToCML(L"helloworld"));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testParseToSmiles()
void NameToStructureTest::testParseToSmiles() {
    NameToStructure *nts = NameToStructure::getInstance();
    std::wstring smiles = nts->parseToSmiles(L"ethane");
    assertEquals(L"CC", smiles);
}
