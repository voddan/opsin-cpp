#include "StructureBuildingMethodsTest.h"
#include "opsin-core/Element.h"
#include "opsin-core/GroupingEl.h"
#include "opsin-core/XmlDeclarations.h"
#include "opsin-core/StructureBuildingMethods.h"
#include "opsin-core/Attribute.h"
#include "opsin-core/StructureBuildingException.h"
#include "opsin-core/BuildState.h"
#include "opsin-core/NameToStructureConfig.h"
#include "opsin-core/TokenEl.h"
#include "opsin-core/Fragment.h"
#include "opsin-core/Bond.h"
#include "opsin-core/ChemEl.h"


using org::junit::Test;

//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;
//						import static org.junit.Assert.*;
//						import static org.mockito.Mockito.mock;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void bracketedPrimeNotSpecialCase()
void StructureBuildingMethodsTest::bracketedPrimeNotSpecialCase() {
    Element *word = new GroupingEl(XmlDeclarations::WORD_EL);
    Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
    word->addChild(substituent);
    assertEquals(nullptr, StructureBuildingMethods::checkForBracketedPrimedLocantSpecialCase(substituent, L"4"));
    assertEquals(nullptr, StructureBuildingMethods::checkForBracketedPrimedLocantSpecialCase(substituent, L"4'"));
    assertEquals(nullptr, StructureBuildingMethods::checkForBracketedPrimedLocantSpecialCase(substituent, L"4''"));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void bracketedPrimeSpecialCase1()
void StructureBuildingMethodsTest::bracketedPrimeSpecialCase1() {
    Element *word = new GroupingEl(XmlDeclarations::WORD_EL);
    Element *bracket = new GroupingEl(XmlDeclarations::BRACKET_EL);
    word->addChild(bracket);
    Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
    bracket->addChild(substituent);
    assertEquals(nullptr, StructureBuildingMethods::checkForBracketedPrimedLocantSpecialCase(substituent, L"4"));
    assertEquals(L"4", StructureBuildingMethods::checkForBracketedPrimedLocantSpecialCase(substituent, L"4'"));
    assertEquals(nullptr, StructureBuildingMethods::checkForBracketedPrimedLocantSpecialCase(substituent, L"4''"));
    Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::IMPLICIT_TYPE_VAL);
    bracket->addAttribute(&tempVar);
    assertEquals(nullptr, StructureBuildingMethods::checkForBracketedPrimedLocantSpecialCase(substituent, L"4"));
    assertEquals(nullptr, StructureBuildingMethods::checkForBracketedPrimedLocantSpecialCase(substituent, L"4'"));
    assertEquals(nullptr, StructureBuildingMethods::checkForBracketedPrimedLocantSpecialCase(substituent, L"4''"));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void bracketedPrimeSpecialCase2()
void StructureBuildingMethodsTest::bracketedPrimeSpecialCase2() {
    Element *word = new GroupingEl(XmlDeclarations::WORD_EL);
    Element *bracket = new GroupingEl(XmlDeclarations::BRACKET_EL);
    word->addChild(bracket);
    Element *bracket2 = new GroupingEl(XmlDeclarations::BRACKET_EL);
    bracket->addChild(bracket2);
    Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
    bracket2->addChild(substituent);
    assertEquals(nullptr, StructureBuildingMethods::checkForBracketedPrimedLocantSpecialCase(substituent, L"4"));
    assertEquals(nullptr, StructureBuildingMethods::checkForBracketedPrimedLocantSpecialCase(substituent, L"4'"));
    assertEquals(L"4", StructureBuildingMethods::checkForBracketedPrimedLocantSpecialCase(substituent, L"4''"));
    Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::IMPLICIT_TYPE_VAL);
    bracket2->addAttribute(&tempVar);
    assertEquals(nullptr, StructureBuildingMethods::checkForBracketedPrimedLocantSpecialCase(substituent, L"4"));
    assertEquals(L"4", StructureBuildingMethods::checkForBracketedPrimedLocantSpecialCase(substituent, L"4'"));
    assertEquals(nullptr, StructureBuildingMethods::checkForBracketedPrimedLocantSpecialCase(substituent, L"4''"));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void notPhosphoSubstitution() throws StructureBuildingException
void StructureBuildingMethodsTest::notPhosphoSubstitution() throw(StructureBuildingException) {
    //standard unlocanted substitution
    BuildState * state = new BuildState(mock(NameToStructureConfig::
    typeid));
    Element *word = new GroupingEl(XmlDeclarations::WORD_EL);

    Element *amino = new TokenEl(XmlDeclarations::GROUP_EL);
    Fragment *aminoFrag = state->fragManager->buildSMILES(L"-N");
    amino->setFrag(aminoFrag);
    Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
    substituent->addChild(amino);

    Element *methanol = new TokenEl(XmlDeclarations::GROUP_EL);
    methanol->setFrag(state->fragManager->buildSMILES(L"CO"));
    Element *root = new GroupingEl(XmlDeclarations::ROOT_EL);
    root->addChild(methanol);

    word->addChild(substituent);
    word->addChild(root);
    StructureBuildingMethods::resolveRootOrSubstituentUnLocanted(state, substituent);

    Set < Bond * > *interFragmentBonds = state->fragManager->getInterFragmentBonds(aminoFrag);
    assertEquals(1, interFragmentBonds->size());
    assertEquals(ChemEl::C, interFragmentBonds->begin()->next().getOtherAtom(aminoFrag->getFirstAtom()).getElement());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phosphoUnlocantedSubstitution() throws StructureBuildingException
void StructureBuildingMethodsTest::phosphoUnlocantedSubstitution() throw(StructureBuildingException) {
    BuildState * state = new BuildState(mock(NameToStructureConfig::
    typeid));
    Element *word = new GroupingEl(XmlDeclarations::WORD_EL);

    Element *phospho = new TokenEl(XmlDeclarations::GROUP_EL);
    Attribute tempVar(XmlDeclarations::SUBTYPE_ATR, XmlDeclarations::PHOSPHO_SUBTYPE_VAL);
    phospho->addAttribute(&tempVar);
    Fragment *phosphoFrag = state->fragManager->buildSMILES(L"-P(=O)O");
    phospho->setFrag(phosphoFrag);
    Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
    substituent->addChild(phospho);

    Element *methanol = new TokenEl(XmlDeclarations::GROUP_EL);
    methanol->setFrag(state->fragManager->buildSMILES(L"CO"));
    Element *root = new GroupingEl(XmlDeclarations::ROOT_EL);
    root->addChild(methanol);

    word->addChild(substituent);
    word->addChild(root);
    StructureBuildingMethods::resolveRootOrSubstituentUnLocanted(state, substituent);

    Set < Bond * > *interFragmentBonds = state->fragManager->getInterFragmentBonds(phosphoFrag);
    assertEquals(1, interFragmentBonds->size());
    assertEquals(ChemEl::O, interFragmentBonds->begin()->next().getOtherAtom(phosphoFrag->getFirstAtom()).getElement());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void phosphoLocantedSubstitution() throws StructureBuildingException
void StructureBuildingMethodsTest::phosphoLocantedSubstitution() throw(StructureBuildingException) {
    BuildState * state = new BuildState(mock(NameToStructureConfig::
    typeid));
    Element *word = new GroupingEl(XmlDeclarations::WORD_EL);

    Element *phospho = new TokenEl(XmlDeclarations::GROUP_EL);
    Attribute tempVar(XmlDeclarations::SUBTYPE_ATR, XmlDeclarations::PHOSPHO_SUBTYPE_VAL);
    phospho->addAttribute(&tempVar);
    Fragment *phosphoFrag = state->fragManager->buildSMILES(L"-P(=O)O");
    phospho->setFrag(phosphoFrag);
    Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
    Attribute tempVar2(XmlDeclarations::LOCANT_ATR, L"4");
    substituent->addAttribute(&tempVar2);
    substituent->addChild(phospho);

    Element *methanol = new TokenEl(XmlDeclarations::GROUP_EL);
    methanol->setFrag(state->fragManager->buildSMILES(L"CCCCO", methanol, L"1/2/3/4/"));
    Element *root = new GroupingEl(XmlDeclarations::ROOT_EL);
    root->addChild(methanol);

    word->addChild(substituent);
    word->addChild(root);
    StructureBuildingMethods::resolveRootOrSubstituentLocanted(state, substituent);

    Set < Bond * > *interFragmentBonds = state->fragManager->getInterFragmentBonds(phosphoFrag);
    assertEquals(1, interFragmentBonds->size());
    assertEquals(ChemEl::O, interFragmentBonds->begin()->next().getOtherAtom(phosphoFrag->getFirstAtom()).getElement());
}
