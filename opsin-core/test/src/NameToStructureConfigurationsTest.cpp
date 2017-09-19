#include "NameToStructureConfigurationsTest.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/NameToStructure.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/StructureBuildingException.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/NameToStructureConfig.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/OpsinResult.h"


//						import static org.junit.Assert.*;
using org::junit::AfterClass;
using org::junit::BeforeClass;
using org::junit::Test;
using OPSIN_RESULT_STATUS = OpsinResult::OPSIN_RESULT_STATUS;
NameToStructure *NameToStructureConfigurationsTest::n2s;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @BeforeClass public static void setUp()
void NameToStructureConfigurationsTest::setUp() {
    n2s = NameToStructure::getInstance();
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @AfterClass public static void cleanUp()
void NameToStructureConfigurationsTest::cleanUp() {
    n2s = nullptr;
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAllowRadicals() throws StructureBuildingException
void NameToStructureConfigurationsTest::testAllowRadicals() throw(StructureBuildingException) {
    NameToStructureConfig *n2sConfig = NameToStructureConfig::getDefaultConfigInstance();
    n2sConfig->setAllowRadicals(false);
    OpsinResult * or = n2s->parseChemicalName(L"methyl", n2sConfig);
    assertEquals(OPSIN_RESULT_STATUS::FAILURE, or->getStatus());

    n2sConfig->setAllowRadicals(true);
    or = n2s->parseChemicalName(L"methyl", n2sConfig);
    assertEquals(OPSIN_RESULT_STATUS::SUCCESS, or->getStatus());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testOutputRadicalsAsWildCards() throws StructureBuildingException
void NameToStructureConfigurationsTest::testOutputRadicalsAsWildCards() throw(StructureBuildingException) {
    NameToStructureConfig *n2sConfig = NameToStructureConfig::getDefaultConfigInstance();
    n2sConfig->setAllowRadicals(true);
    n2sConfig->setOutputRadicalsAsWildCardAtoms(false);
    OpsinResult * or = n2s->parseChemicalName(L"methyl", n2sConfig);
    assertEquals(L"[CH3]", or->getSmiles());

    n2sConfig->setOutputRadicalsAsWildCardAtoms(true);
    or = n2s->parseChemicalName(L"methyl", n2sConfig);
    assertEquals(L"C*", or->getSmiles());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testInterpretAcidsWithoutTheWordAcid() throws StructureBuildingException
void NameToStructureConfigurationsTest::testInterpretAcidsWithoutTheWordAcid() throw(StructureBuildingException) {
    NameToStructureConfig *n2sConfig = NameToStructureConfig::getDefaultConfigInstance();
    n2sConfig->setInterpretAcidsWithoutTheWordAcid(false);
    OpsinResult * or = n2s->parseChemicalName(L"acetic", n2sConfig);
    assertEquals(OPSIN_RESULT_STATUS::FAILURE, or->getStatus());

    n2sConfig->setInterpretAcidsWithoutTheWordAcid(true);
    or = n2s->parseChemicalName(L"acetic", n2sConfig);
    assertEquals(OPSIN_RESULT_STATUS::SUCCESS, or->getStatus());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testWarnRatherThanFailOnUninterpretableStereochemistry() throws StructureBuildingException
void
NameToStructureConfigurationsTest::testWarnRatherThanFailOnUninterpretableStereochemistry() throw(StructureBuildingException) {
    NameToStructureConfig *n2sConfig = NameToStructureConfig::getDefaultConfigInstance();
    n2sConfig->setWarnRatherThanFailOnUninterpretableStereochemistry(false);
    OpsinResult * or = n2s->parseChemicalName(L"(R)-2,2'-Bis(diphenylphosphino)-1,1'-binaphthyl", n2sConfig);
    assertEquals(OPSIN_RESULT_STATUS::FAILURE, or->getStatus());

    n2sConfig->setWarnRatherThanFailOnUninterpretableStereochemistry(true);
    or = n2s->parseChemicalName(L"(R)-2,2'-Bis(diphenylphosphino)-1,1'-binaphthyl", n2sConfig);
    assertEquals(OPSIN_RESULT_STATUS::WARNING, or->getStatus());
}
