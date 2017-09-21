#include "ComponentGeneration_AmbiguitiesAndIrregularitiesTest.h"
#include "opsin-core/Element.h"
#include "opsin-core/GroupingEl.h"
#include "opsin-core/XmlDeclarations.h"
#include "opsin-core/TokenEl.h"
#include "opsin-core/Attribute.h"
#include "opsin-core/ComponentGenerator.h"
#include "opsin-core/ComponentGenerationException.h"


//						import static org.junit.Assert.*;

//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;
using org::junit::Test;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCorrectlyTokenisedAlkane()
void ComponentGeneration_AmbiguitiesAndIrregularitiesTest::testCorrectlyTokenisedAlkane() {
    Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
    Element *alkaneComponent1 = new TokenEl(XmlDeclarations::ALKANESTEMCOMPONENT);
    Attribute tempVar(XmlDeclarations::VALUE_ATR, L"4");
    alkaneComponent1->addAttribute(&tempVar);
    Element *alkaneComponent2 = new TokenEl(XmlDeclarations::ALKANESTEMCOMPONENT);
    Attribute tempVar2(XmlDeclarations::VALUE_ATR, L"10");
    alkaneComponent2->addAttribute(&tempVar2);
    substituent->addChild(alkaneComponent1);
    substituent->addChild(alkaneComponent2);
    try {
        ComponentGenerator::resolveAmbiguities(substituent);
    }
    catch (const ComponentGenerationException &e) {
        fail(L"alkane was well formed, exception should not be thrown");
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCorrectlyTokenisedAlkane2()
void ComponentGeneration_AmbiguitiesAndIrregularitiesTest::testCorrectlyTokenisedAlkane2() {
    Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
    Element *multiplier = new TokenEl(XmlDeclarations::MULTIPLIER_EL);
    Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::BASIC_TYPE_VAL);
    multiplier->addAttribute(&tempVar);
    Attribute tempVar2(XmlDeclarations::VALUE_ATR, L"2");
    multiplier->addAttribute(&tempVar2);
    Element *alkaneComponent = new TokenEl(XmlDeclarations::ALKANESTEMCOMPONENT);
    Attribute tempVar3(XmlDeclarations::VALUE_ATR, L"10");
    alkaneComponent->addAttribute(&tempVar3);
    substituent->addChild(multiplier);
    substituent->addChild(alkaneComponent);
    try {
        ComponentGenerator::resolveAmbiguities(substituent);
    }
    catch (const ComponentGenerationException &e) {
        fail(L"alkane was well formed, exception should not be thrown");
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCorrectlyTokenisedAlkane3()
void ComponentGeneration_AmbiguitiesAndIrregularitiesTest::testCorrectlyTokenisedAlkane3() { //unambiguously 6 hexanes
    Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
    Element *multiplier = new TokenEl(XmlDeclarations::MULTIPLIER_EL);
    Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::BASIC_TYPE_VAL);
    multiplier->addAttribute(&tempVar);
    Attribute tempVar2(XmlDeclarations::VALUE_ATR, L"6");
    multiplier->addAttribute(&tempVar2);
    Element *alkaneComponent = new TokenEl(XmlDeclarations::ALKANESTEMCOMPONENT);
    Attribute tempVar3(XmlDeclarations::VALUE_ATR, L"6");
    alkaneComponent->addAttribute(&tempVar3);
    substituent->addChild(multiplier);
    substituent->addChild(alkaneComponent);
    try {
        ComponentGenerator::resolveAmbiguities(substituent);
    }
    catch (const ComponentGenerationException &e) {
        fail(L"alkane was well formed, exception should not be thrown");
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=ComponentGenerationException.class) public void testMisTokenisedAlkane() throws ComponentGenerationException
void
ComponentGeneration_AmbiguitiesAndIrregularitiesTest::testMisTokenisedAlkane() throw(ComponentGenerationException) { //tetradec is 14 not 4 x 10
    Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
    Element *erroneousMultiplier = new TokenEl(XmlDeclarations::MULTIPLIER_EL);
    Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::BASIC_TYPE_VAL);
    erroneousMultiplier->addAttribute(&tempVar);
    Attribute tempVar2(XmlDeclarations::VALUE_ATR, L"4");
    erroneousMultiplier->addAttribute(&tempVar2);
    Element *alkaneComponent2 = new TokenEl(XmlDeclarations::ALKANESTEMCOMPONENT);
    Attribute tempVar3(XmlDeclarations::VALUE_ATR, L"10");
    alkaneComponent2->addAttribute(&tempVar3);
    substituent->addChild(erroneousMultiplier);
    substituent->addChild(alkaneComponent2);
    ComponentGenerator::resolveAmbiguities(substituent);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testLocantsIndicatingTokenizationIsCorrect()
void
ComponentGeneration_AmbiguitiesAndIrregularitiesTest::testLocantsIndicatingTokenizationIsCorrect() { //should be a group multiplier formally
    Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
    Element *locant = new TokenEl(XmlDeclarations::LOCANT_EL, L"1,2,3,4");
    substituent->addChild(locant);
    Element *multiplier = new TokenEl(XmlDeclarations::MULTIPLIER_EL);
    Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::BASIC_TYPE_VAL);
    multiplier->addAttribute(&tempVar);
    Attribute tempVar2(XmlDeclarations::VALUE_ATR, L"4");
    multiplier->addAttribute(&tempVar2);
    Element *alkaneComponent = new TokenEl(XmlDeclarations::ALKANESTEMCOMPONENT);
    Attribute tempVar3(XmlDeclarations::VALUE_ATR, L"10");
    alkaneComponent->addAttribute(&tempVar3);
    substituent->addChild(multiplier);
    substituent->addChild(alkaneComponent);
    try {
        ComponentGenerator::resolveAmbiguities(substituent);
    }
    catch (const ComponentGenerationException &e) {
        fail(L"alkane was well formed, exception should not be thrown");
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=ComponentGenerationException.class) public void testLocantsIndicatingTokenizationIsIncorrect() throws ComponentGenerationException
void
ComponentGeneration_AmbiguitiesAndIrregularitiesTest::testLocantsIndicatingTokenizationIsIncorrect() throw(ComponentGenerationException) { //tetradec is 14 not 4 x 10
    Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
    Element *locant = new TokenEl(XmlDeclarations::LOCANT_EL, L"1");
    substituent->addChild(locant);
    Element *erroneousMultiplier = new TokenEl(XmlDeclarations::MULTIPLIER_EL);
    Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::BASIC_TYPE_VAL);
    erroneousMultiplier->addAttribute(&tempVar);
    Attribute tempVar2(XmlDeclarations::VALUE_ATR, L"4");
    erroneousMultiplier->addAttribute(&tempVar2);
    Element *alkaneComponent = new TokenEl(XmlDeclarations::ALKANESTEMCOMPONENT);
    Attribute tempVar3(XmlDeclarations::VALUE_ATR, L"10");
    alkaneComponent->addAttribute(&tempVar3);
    substituent->addChild(erroneousMultiplier);
    substituent->addChild(alkaneComponent);
    ComponentGenerator::resolveAmbiguities(substituent);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=ComponentGenerationException.class) public void testTetraphenShouldBeTetra_Phen1() throws ComponentGenerationException
void
ComponentGeneration_AmbiguitiesAndIrregularitiesTest::testTetraphenShouldBeTetra_Phen1() throw(ComponentGenerationException) { //tetraphenyl
    Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
    Element *multiplier = new TokenEl(XmlDeclarations::MULTIPLIER_EL);
    Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::BASIC_TYPE_VAL);
    multiplier->addAttribute(&tempVar);
    Attribute tempVar2(XmlDeclarations::VALUE_ATR, L"4");
    multiplier->addAttribute(&tempVar2);
    Element *phen = new TokenEl(XmlDeclarations::HYDROCARBONFUSEDRINGSYSTEM_EL, L"phen");
    Element *yl = new TokenEl(XmlDeclarations::SUFFIX_EL, L"yl");
    substituent->addChild(multiplier);
    substituent->addChild(phen);
    substituent->addChild(yl);
    ComponentGenerator::resolveAmbiguities(substituent);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=ComponentGenerationException.class) public void testTetraphenShouldBeTetra_Phen2() throws ComponentGenerationException
void
ComponentGeneration_AmbiguitiesAndIrregularitiesTest::testTetraphenShouldBeTetra_Phen2() throw(ComponentGenerationException) { //tetraphenoxy
    Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
    Element *multiplier = new TokenEl(XmlDeclarations::MULTIPLIER_EL);
    Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::BASIC_TYPE_VAL);
    multiplier->addAttribute(&tempVar);
    Attribute tempVar2(XmlDeclarations::VALUE_ATR, L"4");
    multiplier->addAttribute(&tempVar2);
    Element *phen = new TokenEl(XmlDeclarations::HYDROCARBONFUSEDRINGSYSTEM_EL, L"phen");
    Element *yl = new TokenEl(XmlDeclarations::SUFFIX_EL, L"oxy");
    substituent->addChild(multiplier);
    substituent->addChild(phen);
    substituent->addChild(yl);
    ComponentGenerator::resolveAmbiguities(substituent);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testTetraphenShouldBeTetraphen1()
void ComponentGeneration_AmbiguitiesAndIrregularitiesTest::testTetraphenShouldBeTetraphen1() { //tetrapheneyl
    Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
    Element *multiplier = new TokenEl(XmlDeclarations::MULTIPLIER_EL);
    Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::BASIC_TYPE_VAL);
    multiplier->addAttribute(&tempVar);
    Attribute tempVar2(XmlDeclarations::VALUE_ATR, L"4");
    multiplier->addAttribute(&tempVar2);
    Element *phen = new TokenEl(XmlDeclarations::HYDROCARBONFUSEDRINGSYSTEM_EL, L"phen");
    Attribute tempVar3(XmlDeclarations::SUBSEQUENTUNSEMANTICTOKEN_ATR, L"e");
    phen->addAttribute(&tempVar3);
    Element *yl = new TokenEl(XmlDeclarations::SUFFIX_EL, L"yl");
    substituent->addChild(multiplier);
    substituent->addChild(phen);
    substituent->addChild(yl);
    try {
        ComponentGenerator::resolveAmbiguities(substituent);
    }
    catch (const ComponentGenerationException &e) {
        fail(L"tetraphene was the intended interpretation");
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testTetraphenShouldBeTetraphen2()
void ComponentGeneration_AmbiguitiesAndIrregularitiesTest::testTetraphenShouldBeTetraphen2() { //tetraphen2yl
    Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
    Element *multiplier = new TokenEl(XmlDeclarations::MULTIPLIER_EL);
    Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::BASIC_TYPE_VAL);
    multiplier->addAttribute(&tempVar);
    Attribute tempVar2(XmlDeclarations::VALUE_ATR, L"4");
    multiplier->addAttribute(&tempVar2);
    Element *phen = new TokenEl(XmlDeclarations::HYDROCARBONFUSEDRINGSYSTEM_EL, L"phen");
    Element *locant = new TokenEl(XmlDeclarations::LOCANT_EL, L"2");
    Element *yl = new TokenEl(XmlDeclarations::SUFFIX_EL, L"yl");
    substituent->addChild(multiplier);
    substituent->addChild(phen);
    substituent->addChild(locant);
    substituent->addChild(yl);
    try {
        ComponentGenerator::resolveAmbiguities(substituent);
    }
    catch (const ComponentGenerationException &e) {
        fail(L"tetraphen as in tetraphene was the intended interpretation");
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testTetraphenShouldBeTetraphen3()
void ComponentGeneration_AmbiguitiesAndIrregularitiesTest::testTetraphenShouldBeTetraphen3() { //2tetraphenyl
    Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
    Element *locant = new TokenEl(XmlDeclarations::LOCANT_EL, L"2");
    Element *multiplier = new TokenEl(XmlDeclarations::MULTIPLIER_EL);
    Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::BASIC_TYPE_VAL);
    multiplier->addAttribute(&tempVar);
    Attribute tempVar2(XmlDeclarations::VALUE_ATR, L"4");
    multiplier->addAttribute(&tempVar2);
    Element *phen = new TokenEl(XmlDeclarations::HYDROCARBONFUSEDRINGSYSTEM_EL, L"phen");
    Element *yl = new TokenEl(XmlDeclarations::SUFFIX_EL, L"yl");
    substituent->addChild(locant);
    substituent->addChild(multiplier);
    substituent->addChild(phen);
    substituent->addChild(yl);
    try {
        ComponentGenerator::resolveAmbiguities(substituent);
    }
    catch (const ComponentGenerationException &e) {
        fail(L"tetraphen as in tetraphene was the intended interpretation");
    }
}
