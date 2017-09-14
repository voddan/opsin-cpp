#include "ComponentProcessorTest.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/ComponentGenerationException.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/Element.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/GroupingEl.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/XmlDeclarations.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/TokenEl.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/Attribute.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/ComponentProcessor.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/StructureBuildingException.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/BuildState.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/NameToStructureConfig.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/Fragment.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/SuffixApplier.h"


using namespace uk::ac::cam::ch::wwmm::opsin;
//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;
using org::junit::Test;
//						import static org.junit.Assert.*;
//						import static org.mockito.Mockito.mock;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=ComponentGenerationException.class) public void testSubtractiveWithNoGroupToAttachTo() throws ComponentGenerationException
void ComponentProcessorTest::testSubtractiveWithNoGroupToAttachTo() throw(ComponentGenerationException) {
    Element *word = new GroupingEl(XmlDeclarations::WORD_EL);
    Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
    word->addChild(substituent);
    Element *substractivePrefix = new TokenEl(XmlDeclarations::SUBTRACTIVEPREFIX_EL);
    Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::DEOXY_TYPE_VAL);
    substractivePrefix->addAttribute(&tempVar);
    substituent->addChild(substractivePrefix);
    ComponentProcessor::removeAndMoveToAppropriateGroupIfSubtractivePrefix(substituent);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSubtractiveWithBiochemicalToAttachTo() throws ComponentGenerationException
void ComponentProcessorTest::testSubtractiveWithBiochemicalToAttachTo() throw(ComponentGenerationException) {
    Element *word = new GroupingEl(XmlDeclarations::WORD_EL);
    Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
    Element *substractivePrefix = new TokenEl(XmlDeclarations::SUBTRACTIVEPREFIX_EL);
    Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::DEOXY_TYPE_VAL);
    substractivePrefix->addAttribute(&tempVar);
    substituent->addChild(substractivePrefix);
    word->addChild(substituent);
    Element *root = new GroupingEl(XmlDeclarations::ROOT_EL);
    word->addChild(root);
    Element *group = new TokenEl(XmlDeclarations::GROUP_EL);
    Attribute tempVar2(XmlDeclarations::SUBTYPE_ATR, XmlDeclarations::BIOCHEMICAL_SUBTYPE_VAL);
    group->addAttribute(&tempVar2);
    root->addChild(group);

    ComponentProcessor::removeAndMoveToAppropriateGroupIfSubtractivePrefix(substituent);
    assertEquals(L"Substractive prefix should of been detached", nullptr, substituent->getParent());
    assertEquals(2, root->getChildCount());
    assertEquals(substractivePrefix, root->getChildElements()[0]);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSubtractiveRightMostPreferred() throws ComponentGenerationException
void ComponentProcessorTest::testSubtractiveRightMostPreferred() throw(ComponentGenerationException) {
    Element *word = new GroupingEl(XmlDeclarations::WORD_EL);
    Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
    Element *substractivePrefix = new TokenEl(XmlDeclarations::SUBTRACTIVEPREFIX_EL);
    Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::DEOXY_TYPE_VAL);
    substractivePrefix->addAttribute(&tempVar);
    substituent->addChild(substractivePrefix);
    word->addChild(substituent);
    Element *substituent2 = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
    Element *group1 = new TokenEl(XmlDeclarations::GROUP_EL);
    Attribute tempVar2(XmlDeclarations::TYPE_ATR, XmlDeclarations::SIMPLEGROUP_SUBTYPE_VAL);
    group1->addAttribute(&tempVar2);
    Attribute tempVar3(XmlDeclarations::SUBTYPE_ATR, XmlDeclarations::SIMPLEGROUP_SUBTYPE_VAL);
    group1->addAttribute(&tempVar3);
    substituent2->addChild(group1);
    word->addChild(substituent2);
    Element *root = new GroupingEl(XmlDeclarations::ROOT_EL);
    word->addChild(root);
    Element *group2 = new TokenEl(XmlDeclarations::GROUP_EL);
    Attribute tempVar4(XmlDeclarations::TYPE_ATR, XmlDeclarations::SIMPLEGROUP_SUBTYPE_VAL);
    group2->addAttribute(&tempVar4);
    Attribute tempVar5(XmlDeclarations::SUBTYPE_ATR, XmlDeclarations::BIOCHEMICAL_SUBTYPE_VAL);
    group2->addAttribute(&tempVar5);
    root->addChild(group2);

    ComponentProcessor::removeAndMoveToAppropriateGroupIfSubtractivePrefix(substituent);
    assertEquals(L"Substractive prefix should of been detached", nullptr, substituent->getParent());
    assertEquals(2, root->getChildCount());
    assertEquals(substractivePrefix, root->getChildElements()[0]);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSubtractiveBiochemicalPreferredToRightMost() throws ComponentGenerationException
void ComponentProcessorTest::testSubtractiveBiochemicalPreferredToRightMost() throw(ComponentGenerationException) {
    Element *word = new GroupingEl(XmlDeclarations::WORD_EL);
    Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
    Element *substractivePrefix = new TokenEl(XmlDeclarations::SUBTRACTIVEPREFIX_EL);
    Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::DEOXY_TYPE_VAL);
    substractivePrefix->addAttribute(&tempVar);
    substituent->addChild(substractivePrefix);
    word->addChild(substituent);
    Element *substituent2 = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
    Element *group1 = new TokenEl(XmlDeclarations::GROUP_EL);
    Attribute tempVar2(XmlDeclarations::SUBTYPE_ATR, XmlDeclarations::BIOCHEMICAL_SUBTYPE_VAL);
    group1->addAttribute(&tempVar2);
    substituent2->addChild(group1);
    word->addChild(substituent2);
    Element *root = new GroupingEl(XmlDeclarations::ROOT_EL);
    word->addChild(root);
    Element *group2 = new TokenEl(XmlDeclarations::GROUP_EL);
    Attribute tempVar3(XmlDeclarations::SUBTYPE_ATR, XmlDeclarations::SIMPLEGROUP_SUBTYPE_VAL);
    group2->addAttribute(&tempVar3);
    root->addChild(group2);

    ComponentProcessor::removeAndMoveToAppropriateGroupIfSubtractivePrefix(substituent);
    assertEquals(L"Substractive prefix should of been detached", nullptr, substituent->getParent());
    assertEquals(1, root->getChildCount());
    assertEquals(2, substituent2->getChildCount());
    assertEquals(substractivePrefix, substituent2->getChildElements()[0]);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSubtractiveWithMultiplierAndLocants() throws ComponentGenerationException
void ComponentProcessorTest::testSubtractiveWithMultiplierAndLocants() throw(ComponentGenerationException) {
    Element *word = new GroupingEl(XmlDeclarations::WORD_EL);
    Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
    Element *locant = new TokenEl(XmlDeclarations::LOCANT_EL);
    substituent->addChild(locant);
    Element *multiplier = new TokenEl(XmlDeclarations::MULTIPLIER_EL);
    substituent->addChild(multiplier);
    Element *substractivePrefix = new TokenEl(XmlDeclarations::SUBTRACTIVEPREFIX_EL);
    Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::DEOXY_TYPE_VAL);
    substractivePrefix->addAttribute(&tempVar);
    substituent->addChild(substractivePrefix);
    word->addChild(substituent);
    Element *root = new GroupingEl(XmlDeclarations::ROOT_EL);
    word->addChild(root);
    Element *group = new TokenEl(XmlDeclarations::GROUP_EL);
    Attribute tempVar2(XmlDeclarations::SUBTYPE_ATR, XmlDeclarations::BIOCHEMICAL_SUBTYPE_VAL);
    group->addAttribute(&tempVar2);
    root->addChild(group);

    ComponentProcessor::removeAndMoveToAppropriateGroupIfSubtractivePrefix(substituent);
    assertEquals(L"Substractive prefix should of been detached", nullptr, substituent->getParent());
    assertEquals(4, root->getChildCount());
    assertEquals(locant, root->getChildElements()[0]);
    assertEquals(multiplier, root->getChildElements()[1]);
    assertEquals(substractivePrefix, root->getChildElements()[2]);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDLStereochemistryLOnAminoAcid() throws ComponentGenerationException, StructureBuildingException
void
ComponentProcessorTest::testDLStereochemistryLOnAminoAcid() throw(ComponentGenerationException, StructureBuildingException) {
    BuildState * state = new BuildState(mock(NameToStructureConfig::
    typeid));
    Fragment *f = state->fragManager->buildSMILES(L"N[C@@H](C)C");
    Element *aminoAcidEl = new TokenEl(XmlDeclarations::GROUP_EL);
    aminoAcidEl->setFrag(f);
    int parityBefore = f->getAtomByID(2)->getAtomParity()->getParity();
    ComponentProcessor * processor = new ComponentProcessor(state, mock(SuffixApplier::
    typeid));
    assertEquals(true, processor->applyDlStereochemistryToAminoAcid(aminoAcidEl, L"l"));
    assertEquals(parityBefore, f->getAtomByID(2)->getAtomParity()->getParity());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDLStereochemistryDOnAminoAcid() throws ComponentGenerationException, StructureBuildingException
void
ComponentProcessorTest::testDLStereochemistryDOnAminoAcid() throw(ComponentGenerationException, StructureBuildingException) {
    BuildState * state = new BuildState(mock(NameToStructureConfig::
    typeid));
    Fragment *f = state->fragManager->buildSMILES(L"N[C@@H](C)C");
    Element *aminoAcidEl = new TokenEl(XmlDeclarations::GROUP_EL);
    aminoAcidEl->setFrag(f);
    int parityBefore = f->getAtomByID(2)->getAtomParity()->getParity();
    ComponentProcessor * processor = new ComponentProcessor(state, mock(SuffixApplier::
    typeid));
    assertEquals(true, processor->applyDlStereochemistryToAminoAcid(aminoAcidEl, L"d"));
    assertEquals(parityBefore, -f->getAtomByID(2)->getAtomParity()->getParity());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDLStereochemistryDLOnAminoAcid() throws ComponentGenerationException, StructureBuildingException
void
ComponentProcessorTest::testDLStereochemistryDLOnAminoAcid() throw(ComponentGenerationException, StructureBuildingException) {
    BuildState * state = new BuildState(mock(NameToStructureConfig::
    typeid));
    Fragment *f = state->fragManager->buildSMILES(L"N[C@@H](C)C");
    Element *aminoAcidEl = new TokenEl(XmlDeclarations::GROUP_EL);
    aminoAcidEl->setFrag(f);
    ComponentProcessor * processor = new ComponentProcessor(state, mock(SuffixApplier::
    typeid));
    assertEquals(true, processor->applyDlStereochemistryToAminoAcid(aminoAcidEl, L"dl"));
    assertEquals(nullptr, f->getAtomByID(2)->getAtomParity());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDLStereochemistryDOnAchiralAminoAcid() throws ComponentGenerationException, StructureBuildingException
void
ComponentProcessorTest::testDLStereochemistryDOnAchiralAminoAcid() throw(ComponentGenerationException, StructureBuildingException) {
    BuildState * state = new BuildState(mock(NameToStructureConfig::
    typeid));
    Fragment *f = state->fragManager->buildSMILES(L"NC(C)C");
    Element *aminoAcidEl = new TokenEl(XmlDeclarations::GROUP_EL);
    aminoAcidEl->setFrag(f);
    ComponentProcessor * processor = new ComponentProcessor(state, mock(SuffixApplier::
    typeid));
    assertEquals(false, processor->applyDlStereochemistryToAminoAcid(aminoAcidEl, L"d"));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDLStereochemistryLOnCarbohydrate() throws ComponentGenerationException, StructureBuildingException
void
ComponentProcessorTest::testDLStereochemistryLOnCarbohydrate() throw(ComponentGenerationException, StructureBuildingException) {
    BuildState * state = new BuildState(mock(NameToStructureConfig::
    typeid));
    Fragment *f = state->fragManager->buildSMILES(L"N[C@@H](C)C");
    Element *carbohydrateEl = new TokenEl(XmlDeclarations::GROUP_EL);
    carbohydrateEl->setFrag(f);
    int parityBefore = f->getAtomByID(2)->getAtomParity()->getParity();
    ComponentProcessor * processor = new ComponentProcessor(state, mock(SuffixApplier::
    typeid));
    processor->applyDlStereochemistryToCarbohydrate(carbohydrateEl, L"l");
    assertEquals(parityBefore, -f->getAtomByID(2)->getAtomParity()->getParity());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDLStereochemistryDOnCarbohydrate() throws ComponentGenerationException, StructureBuildingException
void
ComponentProcessorTest::testDLStereochemistryDOnCarbohydrate() throw(ComponentGenerationException, StructureBuildingException) {
    BuildState * state = new BuildState(mock(NameToStructureConfig::
    typeid));
    Fragment *f = state->fragManager->buildSMILES(L"N[C@@H](C)C");
    Element *carbohydrateEl = new TokenEl(XmlDeclarations::GROUP_EL);
    carbohydrateEl->setFrag(f);
    int parityBefore = f->getAtomByID(2)->getAtomParity()->getParity();
    ComponentProcessor * processor = new ComponentProcessor(state, mock(SuffixApplier::
    typeid));
    processor->applyDlStereochemistryToCarbohydrate(carbohydrateEl, L"d");
    assertEquals(parityBefore, f->getAtomByID(2)->getAtomParity()->getParity());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDLStereochemistryInvertedNaturalOnCarbohydrate1() throws ComponentGenerationException, StructureBuildingException
void
ComponentProcessorTest::testDLStereochemistryInvertedNaturalOnCarbohydrate1() throw(ComponentGenerationException, StructureBuildingException) {
    BuildState * state = new BuildState(mock(NameToStructureConfig::
    typeid));
    Fragment *f = state->fragManager->buildSMILES(L"N[C@@H](C)C");
    Element *carbohydrateEl = new TokenEl(XmlDeclarations::GROUP_EL);
    Attribute tempVar(XmlDeclarations::NATURALENTISOPPOSITE_ATR, L"yes");
    carbohydrateEl->addAttribute(&tempVar);
    carbohydrateEl->setFrag(f);
    int parityBefore = f->getAtomByID(2)->getAtomParity()->getParity();
    ComponentProcessor * processor = new ComponentProcessor(state, mock(SuffixApplier::
    typeid));
    processor->applyDlStereochemistryToCarbohydrate(carbohydrateEl, L"l");
    assertEquals(parityBefore, f->getAtomByID(2)->getAtomParity()->getParity());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDLStereochemistryInvertedNaturalOnCarbohydrate2() throws ComponentGenerationException, StructureBuildingException
void
ComponentProcessorTest::testDLStereochemistryInvertedNaturalOnCarbohydrate2() throw(ComponentGenerationException, StructureBuildingException) {
    BuildState * state = new BuildState(mock(NameToStructureConfig::
    typeid));
    Fragment *f = state->fragManager->buildSMILES(L"N[C@@H](C)C");
    Element *carbohydrateEl = new TokenEl(XmlDeclarations::GROUP_EL);
    Attribute tempVar(XmlDeclarations::NATURALENTISOPPOSITE_ATR, L"yes");
    carbohydrateEl->addAttribute(&tempVar);
    carbohydrateEl->setFrag(f);
    int parityBefore = f->getAtomByID(2)->getAtomParity()->getParity();
    ComponentProcessor * processor = new ComponentProcessor(state, mock(SuffixApplier::
    typeid));
    processor->applyDlStereochemistryToCarbohydrate(carbohydrateEl, L"d");
    assertEquals(parityBefore, -f->getAtomByID(2)->getAtomParity()->getParity());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDStereochemistryDOnCarbohydratePrefix() throws ComponentGenerationException, StructureBuildingException
void
ComponentProcessorTest::testDStereochemistryDOnCarbohydratePrefix() throw(ComponentGenerationException, StructureBuildingException) {
    Element *prefix = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL);
    Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::CARBOHYDRATECONFIGURATIONPREFIX_TYPE_VAL);
    prefix->addAttribute(&tempVar);
    Attribute tempVar2(XmlDeclarations::VALUE_ATR, L"l/r");
    prefix->addAttribute(&tempVar2); //D-threo
    ComponentProcessor::applyDlStereochemistryToCarbohydrateConfigurationalPrefix(prefix, L"d");
    assertEquals(L"l/r", prefix->getAttributeValue(XmlDeclarations::VALUE_ATR));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testLStereochemistryDOnCarbohydratePrefix() throws ComponentGenerationException, StructureBuildingException
void
ComponentProcessorTest::testLStereochemistryDOnCarbohydratePrefix() throw(ComponentGenerationException, StructureBuildingException) {
    Element *prefix = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL);
    Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::CARBOHYDRATECONFIGURATIONPREFIX_TYPE_VAL);
    prefix->addAttribute(&tempVar);
    Attribute tempVar2(XmlDeclarations::VALUE_ATR, L"r/l");
    prefix->addAttribute(&tempVar2);
    ComponentProcessor::applyDlStereochemistryToCarbohydrateConfigurationalPrefix(prefix, L"l");
    assertEquals(L"l/r", prefix->getAttributeValue(XmlDeclarations::VALUE_ATR));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDLStereochemistryDOnCarbohydratePrefix() throws ComponentGenerationException, StructureBuildingException
void
ComponentProcessorTest::testDLStereochemistryDOnCarbohydratePrefix() throw(ComponentGenerationException, StructureBuildingException) {
    Element *prefix = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL);
    Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::CARBOHYDRATECONFIGURATIONPREFIX_TYPE_VAL);
    prefix->addAttribute(&tempVar);
    Attribute tempVar2(XmlDeclarations::VALUE_ATR, L"l/r");
    prefix->addAttribute(&tempVar2);
    ComponentProcessor::applyDlStereochemistryToCarbohydrateConfigurationalPrefix(prefix, L"dl");
    assertEquals(L"?/?", prefix->getAttributeValue(XmlDeclarations::VALUE_ATR));
}
