#include "ComponentGeneration_StereochemistryTest.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/ComponentGenerationException.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/Element.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/GroupingEl.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/XmlDeclarations.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/TokenEl.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/Attribute.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/ComponentGenerator.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/NameToStructureConfig.h"

namespace uk
{
	namespace ac
	{
		namespace cam
		{
			namespace ch
			{
				namespace wwmm
				{
					namespace opsin
					{
//						import static org.junit.Assert.*;
						using namespace uk::ac::cam::ch::wwmm::opsin;
//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;
						using org::junit::Test;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testUnlocantedS() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testUnlocantedS() throw(ComponentGenerationException)
						{
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"(S)");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::STEREOCHEMISTRYBRACKET_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							substituent->addChild(stereochem);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(1, children.size());
							Element *newStereochemistryEl = children[0];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl->getName());
							assertEquals(nullptr, newStereochemistryEl->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"S", newStereochemistryEl->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::R_OR_S_TYPE_VAL, newStereochemistryEl->getAttributeValue(XmlDeclarations::TYPE_ATR));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testMultipleUnLocanted() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testMultipleUnLocanted() throw(ComponentGenerationException)
						{
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"(R,R)");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::STEREOCHEMISTRYBRACKET_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							substituent->addChild(stereochem);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(2, children.size());
							Element *newStereochemistryEl1 = children[0];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl1->getName());
							assertEquals(nullptr, newStereochemistryEl1->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"R", newStereochemistryEl1->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::R_OR_S_TYPE_VAL, newStereochemistryEl1->getAttributeValue(XmlDeclarations::TYPE_ATR));

							Element *newStereochemistryEl2 = children[1];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl2->getName());
							assertEquals(nullptr, newStereochemistryEl2->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"R", newStereochemistryEl2->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::R_OR_S_TYPE_VAL, newStereochemistryEl2->getAttributeValue(XmlDeclarations::TYPE_ATR));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testLocantedR() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testLocantedR() throw(ComponentGenerationException)
						{
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"(1R)");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::STEREOCHEMISTRYBRACKET_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							substituent->addChild(stereochem);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(1, children.size());
							Element *newStereochemistryEl = children[0];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl->getName());
							assertEquals(L"1", newStereochemistryEl->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"R", newStereochemistryEl->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::R_OR_S_TYPE_VAL, newStereochemistryEl->getAttributeValue(XmlDeclarations::TYPE_ATR));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testMultipleRorSLocanted() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testMultipleRorSLocanted() throw(ComponentGenerationException)
						{
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"(alphaR,3S,7'S)");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::STEREOCHEMISTRYBRACKET_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							substituent->addChild(stereochem);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(3, children.size());
							Element *newStereochemistryEl1 = children[0];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl1->getName());
							assertEquals(L"alpha", newStereochemistryEl1->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"R", newStereochemistryEl1->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::R_OR_S_TYPE_VAL, newStereochemistryEl1->getAttributeValue(XmlDeclarations::TYPE_ATR));

							Element *newStereochemistryEl2 = children[1];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl2->getName());
							assertEquals(L"3", newStereochemistryEl2->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"S", newStereochemistryEl2->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::R_OR_S_TYPE_VAL, newStereochemistryEl2->getAttributeValue(XmlDeclarations::TYPE_ATR));

							Element *newStereochemistryEl3 = children[2];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl3->getName());
							assertEquals(L"7'", newStereochemistryEl3->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"S", newStereochemistryEl3->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::R_OR_S_TYPE_VAL, newStereochemistryEl3->getAttributeValue(XmlDeclarations::TYPE_ATR));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testUnLocantedE() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testUnLocantedE() throw(ComponentGenerationException)
						{
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"(E)");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::STEREOCHEMISTRYBRACKET_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							substituent->addChild(stereochem);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(1, children.size());
							Element *newStereochemistryEl = children[0];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl->getName());
							assertEquals(nullptr, newStereochemistryEl->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"E", newStereochemistryEl->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::E_OR_Z_TYPE_VAL, newStereochemistryEl->getAttributeValue(XmlDeclarations::TYPE_ATR));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testLocantedZ() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testLocantedZ() throw(ComponentGenerationException)
						{
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"(5Z)");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::STEREOCHEMISTRYBRACKET_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							substituent->addChild(stereochem);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(1, children.size());
							Element *newStereochemistryEl = children[0];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl->getName());
							assertEquals(L"5", newStereochemistryEl->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"Z", newStereochemistryEl->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::E_OR_Z_TYPE_VAL, newStereochemistryEl->getAttributeValue(XmlDeclarations::TYPE_ATR));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testMultipleRorSorEorZ() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testMultipleRorSorEorZ() throw(ComponentGenerationException)
						{
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"(NZ,2E,R)");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::STEREOCHEMISTRYBRACKET_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							substituent->addChild(stereochem);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(3, children.size());
							Element *newStereochemistryEl1 = children[0];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl1->getName());
							assertEquals(L"N", newStereochemistryEl1->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"Z", newStereochemistryEl1->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::E_OR_Z_TYPE_VAL, newStereochemistryEl1->getAttributeValue(XmlDeclarations::TYPE_ATR));

							Element *newStereochemistryEl2 = children[1];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl2->getName());
							assertEquals(L"2", newStereochemistryEl2->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"E", newStereochemistryEl2->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::E_OR_Z_TYPE_VAL, newStereochemistryEl2->getAttributeValue(XmlDeclarations::TYPE_ATR));

							Element *newStereochemistryEl3 = children[2];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl3->getName());
							assertEquals(nullptr, newStereochemistryEl3->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"R", newStereochemistryEl3->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::R_OR_S_TYPE_VAL, newStereochemistryEl3->getAttributeValue(XmlDeclarations::TYPE_ATR));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDashInsteadOfComma() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testDashInsteadOfComma() throw(ComponentGenerationException)
						{
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"(NZ,2E-R)");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::STEREOCHEMISTRYBRACKET_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							substituent->addChild(stereochem);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(3, children.size());
							Element *newStereochemistryEl1 = children[0];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl1->getName());
							assertEquals(L"N", newStereochemistryEl1->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"Z", newStereochemistryEl1->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::E_OR_Z_TYPE_VAL, newStereochemistryEl1->getAttributeValue(XmlDeclarations::TYPE_ATR));

							Element *newStereochemistryEl2 = children[1];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl2->getName());
							assertEquals(L"2", newStereochemistryEl2->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"E", newStereochemistryEl2->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::E_OR_Z_TYPE_VAL, newStereochemistryEl2->getAttributeValue(XmlDeclarations::TYPE_ATR));

							Element *newStereochemistryEl3 = children[2];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl3->getName());
							assertEquals(nullptr, newStereochemistryEl3->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"R", newStereochemistryEl3->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::R_OR_S_TYPE_VAL, newStereochemistryEl3->getAttributeValue(XmlDeclarations::TYPE_ATR));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testBracketedLocantedCisTrans() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testBracketedLocantedCisTrans() throw(ComponentGenerationException)
						{
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"(3cis,5trans)");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::STEREOCHEMISTRYBRACKET_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							substituent->addChild(stereochem);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(2, children.size());
							Element *newStereochemistryEl1 = children[0];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl1->getName());
							assertEquals(L"3", newStereochemistryEl1->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"cis", newStereochemistryEl1->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::CISORTRANS_TYPE_VAL, newStereochemistryEl1->getAttributeValue(XmlDeclarations::TYPE_ATR));

							Element *newStereochemistryEl2 = children[1];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl2->getName());
							assertEquals(L"5", newStereochemistryEl2->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"trans", newStereochemistryEl2->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::CISORTRANS_TYPE_VAL, newStereochemistryEl2->getAttributeValue(XmlDeclarations::TYPE_ATR));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testBracketedUnlocantedCisTrans() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testBracketedUnlocantedCisTrans() throw(ComponentGenerationException)
						{
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"(5S-trans)");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::STEREOCHEMISTRYBRACKET_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							substituent->addChild(stereochem);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(2, children.size());
							Element *newStereochemistryEl1 = children[0];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl1->getName());
							assertEquals(L"5", newStereochemistryEl1->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"S", newStereochemistryEl1->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::R_OR_S_TYPE_VAL, newStereochemistryEl1->getAttributeValue(XmlDeclarations::TYPE_ATR));

							Element *newStereochemistryEl2 = children[1];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl2->getName());
							assertEquals(nullptr, newStereochemistryEl2->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"trans", newStereochemistryEl2->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::CISORTRANS_TYPE_VAL, newStereochemistryEl2->getAttributeValue(XmlDeclarations::TYPE_ATR));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testBracketedExo() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testBracketedExo() throw(ComponentGenerationException)
						{
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"(exo)");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::STEREOCHEMISTRYBRACKET_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							substituent->addChild(stereochem);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(1, children.size());
							Element *newStereochemistryEl = children[0];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl->getName());
							assertEquals(nullptr, newStereochemistryEl->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"exo", newStereochemistryEl->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::ENDO_EXO_SYN_ANTI_TYPE_VAL, newStereochemistryEl->getAttributeValue(XmlDeclarations::TYPE_ATR));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testBracketedEndo() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testBracketedEndo() throw(ComponentGenerationException)
						{
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"(3-endo,5S)");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::STEREOCHEMISTRYBRACKET_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							substituent->addChild(stereochem);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(2, children.size());
							Element *newStereochemistryEl1 = children[0];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl1->getName());
							assertEquals(L"3", newStereochemistryEl1->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"endo", newStereochemistryEl1->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::ENDO_EXO_SYN_ANTI_TYPE_VAL, newStereochemistryEl1->getAttributeValue(XmlDeclarations::TYPE_ATR));

							Element *newStereochemistryEl2 = children[1];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl2->getName());
							assertEquals(L"5", newStereochemistryEl2->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"S", newStereochemistryEl2->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::R_OR_S_TYPE_VAL, newStereochemistryEl2->getAttributeValue(XmlDeclarations::TYPE_ATR));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testLocantedCisTrans() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testLocantedCisTrans() throw(ComponentGenerationException)
						{
							//XML for 3-cis,5-trans:
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *locant = new TokenEl(XmlDeclarations::LOCANT_EL, L"3");
							substituent->addChild(locant);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"cis");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::CISORTRANS_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							Attribute tempVar2(XmlDeclarations::VALUE_ATR, L"cis");
							stereochem->addAttribute(&tempVar2);
							substituent->addChild(stereochem);
							locant = new TokenEl(XmlDeclarations::LOCANT_EL, L"5");
							substituent->addChild(locant);
							stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"trans");
							Attribute tempVar3(XmlDeclarations::TYPE_ATR, XmlDeclarations::CISORTRANS_TYPE_VAL);
							stereochem->addAttribute(&tempVar3);
							Attribute tempVar4(XmlDeclarations::VALUE_ATR, L"trans");
							stereochem->addAttribute(&tempVar4);
							substituent->addChild(stereochem);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(2, children.size());
							Element *modifiedStereochemistryEl1 = children[0];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, modifiedStereochemistryEl1->getName());
							assertEquals(L"3", modifiedStereochemistryEl1->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"cis", modifiedStereochemistryEl1->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::CISORTRANS_TYPE_VAL, modifiedStereochemistryEl1->getAttributeValue(XmlDeclarations::TYPE_ATR));

							Element *modifiedStereochemistryEl2 = children[1];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, modifiedStereochemistryEl2->getName());
							assertEquals(L"5", modifiedStereochemistryEl2->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"trans", modifiedStereochemistryEl2->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::CISORTRANS_TYPE_VAL, modifiedStereochemistryEl2->getAttributeValue(XmlDeclarations::TYPE_ATR));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testLocantedExoOn() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testLocantedExoOn() throw(ComponentGenerationException)
						{
							//XML for 3-exobicyclo[2.2.2]oct:
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *locant = new TokenEl(XmlDeclarations::LOCANT_EL, L"3");
							substituent->addChild(locant);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"exo");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::ENDO_EXO_SYN_ANTI_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							Attribute tempVar2(XmlDeclarations::VALUE_ATR, L"exo");
							stereochem->addAttribute(&tempVar2);
							substituent->addChild(stereochem);
							Element *multiplier = new TokenEl(XmlDeclarations::MULTIPLIER_EL);
							Attribute tempVar3(XmlDeclarations::TYPE_ATR, XmlDeclarations::VONBAEYER_TYPE_VAL);
							multiplier->addAttribute(&tempVar3);
							substituent->addChild(multiplier);
							Element *vonBaeyer = new TokenEl(XmlDeclarations::VONBAEYER_EL);
							substituent->addChild(vonBaeyer);
							Element *group = new TokenEl(XmlDeclarations::GROUP_EL);
							Attribute tempVar4(XmlDeclarations::TYPE_ATR, XmlDeclarations::CHAIN_TYPE_VAL);
							group->addAttribute(&tempVar4);
							Attribute tempVar5(XmlDeclarations::SUBTYPE_ATR, XmlDeclarations::ALKANESTEM_SUBTYPE_VAL);
							group->addAttribute(&tempVar5);
							substituent->addChild(group);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(4, children.size());
							Element *modifiedStereochemistryEl = children[0];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, modifiedStereochemistryEl->getName());
							assertEquals(L"3", modifiedStereochemistryEl->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"exo", modifiedStereochemistryEl->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::ENDO_EXO_SYN_ANTI_TYPE_VAL, modifiedStereochemistryEl->getAttributeValue(XmlDeclarations::TYPE_ATR));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testLocantedExo() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testLocantedExo() throw(ComponentGenerationException)
						{
							//XML for 3-exoamino
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *locant = new TokenEl(XmlDeclarations::LOCANT_EL, L"3");
							substituent->addChild(locant);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"exo");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::ENDO_EXO_SYN_ANTI_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							Attribute tempVar2(XmlDeclarations::VALUE_ATR, L"exo");
							stereochem->addAttribute(&tempVar2);
							substituent->addChild(stereochem);
							Element *group = new TokenEl(XmlDeclarations::GROUP_EL);
							Attribute tempVar3(XmlDeclarations::TYPE_ATR, XmlDeclarations::SUBSTITUENT_EL);
							group->addAttribute(&tempVar3);
							Attribute tempVar4(XmlDeclarations::SUBTYPE_ATR, XmlDeclarations::SIMPLESUBSTITUENT_SUBTYPE_VAL);
							group->addAttribute(&tempVar4);
							substituent->addChild(group);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(3, children.size());
							assertEquals(XmlDeclarations::LOCANT_EL, children[0]->getName());
							Element *modifiedStereochemistryEl = children[1];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, modifiedStereochemistryEl->getName());
							assertEquals(L"3", modifiedStereochemistryEl->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"exo", modifiedStereochemistryEl->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::ENDO_EXO_SYN_ANTI_TYPE_VAL, modifiedStereochemistryEl->getAttributeValue(XmlDeclarations::TYPE_ATR));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAnti() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testAnti() throw(ComponentGenerationException)
						{
							//XML for anti:
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"anti");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::ENDO_EXO_SYN_ANTI_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							Attribute tempVar2(XmlDeclarations::VALUE_ATR, L"anti");
							stereochem->addAttribute(&tempVar2);
							substituent->addChild(stereochem);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(1, children.size());
							Element *unmodifiedStereochemistryEl = children[0];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, unmodifiedStereochemistryEl->getName());
							assertEquals(L"anti", unmodifiedStereochemistryEl->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::ENDO_EXO_SYN_ANTI_TYPE_VAL, unmodifiedStereochemistryEl->getAttributeValue(XmlDeclarations::TYPE_ATR));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCis() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testCis() throw(ComponentGenerationException)
						{
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"cis");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::CISORTRANS_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							Attribute tempVar2(XmlDeclarations::VALUE_ATR, L"cis");
							stereochem->addAttribute(&tempVar2);
							substituent->addChild(stereochem);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(1, children.size());
							Element *modifiedStereochemistryEl1 = children[0];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, modifiedStereochemistryEl1->getName());
							assertEquals(nullptr, modifiedStereochemistryEl1->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"cis", modifiedStereochemistryEl1->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::CISORTRANS_TYPE_VAL, modifiedStereochemistryEl1->getAttributeValue(XmlDeclarations::TYPE_ATR));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAxial1() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testAxial1() throw(ComponentGenerationException)
						{
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"(M)");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::STEREOCHEMISTRYBRACKET_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							substituent->addChild(stereochem);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(1, children.size());
							Element *newStereochemistryEl = children[0];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl->getName());
							assertEquals(nullptr, newStereochemistryEl->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"M", newStereochemistryEl->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::AXIAL_TYPE_VAL, newStereochemistryEl->getAttributeValue(XmlDeclarations::TYPE_ATR));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAxial2() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testAxial2() throw(ComponentGenerationException)
						{
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"(Ra)");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::STEREOCHEMISTRYBRACKET_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							substituent->addChild(stereochem);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(1, children.size());
							Element *newStereochemistryEl = children[0];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl->getName());
							assertEquals(nullptr, newStereochemistryEl->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"Ra", newStereochemistryEl->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::AXIAL_TYPE_VAL, newStereochemistryEl->getAttributeValue(XmlDeclarations::TYPE_ATR));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testZUnbracketted() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testZUnbracketted() throw(ComponentGenerationException)
						{ //note that IUPAC mandates brackets
							//XML for Z,Z:
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"Z");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::E_OR_Z_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							substituent->addChild(stereochem);
							stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"Z");
							Attribute tempVar2(XmlDeclarations::TYPE_ATR, XmlDeclarations::E_OR_Z_TYPE_VAL);
							stereochem->addAttribute(&tempVar2);
							substituent->addChild(stereochem);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(2, children.size());
							Element *modifiedStereochemistryEl1 = children[0];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, modifiedStereochemistryEl1->getName());
							assertEquals(nullptr, modifiedStereochemistryEl1->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"Z", modifiedStereochemistryEl1->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::E_OR_Z_TYPE_VAL, modifiedStereochemistryEl1->getAttributeValue(XmlDeclarations::TYPE_ATR));

							Element *modifiedStereochemistryEl2 = children[1];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, modifiedStereochemistryEl2->getName());
							assertEquals(nullptr, modifiedStereochemistryEl2->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"Z", modifiedStereochemistryEl2->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::E_OR_Z_TYPE_VAL, modifiedStereochemistryEl2->getAttributeValue(XmlDeclarations::TYPE_ATR));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testEandZUnbrackettedLocanted() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testEandZUnbrackettedLocanted() throw(ComponentGenerationException)
						{ //note that IUPAC mandates brackets
							//XML for 2E,4Z:
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *locant = new TokenEl(XmlDeclarations::LOCANT_EL, L"2");
							substituent->addChild(locant);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"E");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::E_OR_Z_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							substituent->addChild(stereochem);
							locant = new TokenEl(XmlDeclarations::LOCANT_EL, L"4");
							substituent->addChild(locant);
							stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"Z");
							Attribute tempVar2(XmlDeclarations::TYPE_ATR, XmlDeclarations::E_OR_Z_TYPE_VAL);
							stereochem->addAttribute(&tempVar2);
							substituent->addChild(stereochem);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(2, children.size());
							Element *modifiedStereochemistryEl1 = children[0];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, modifiedStereochemistryEl1->getName());
							assertEquals(L"2", modifiedStereochemistryEl1->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"E", modifiedStereochemistryEl1->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::E_OR_Z_TYPE_VAL, modifiedStereochemistryEl1->getAttributeValue(XmlDeclarations::TYPE_ATR));

							Element *modifiedStereochemistryEl2 = children[1];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, modifiedStereochemistryEl2->getName());
							assertEquals(L"4", modifiedStereochemistryEl2->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"Z", modifiedStereochemistryEl2->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::E_OR_Z_TYPE_VAL, modifiedStereochemistryEl2->getAttributeValue(XmlDeclarations::TYPE_ATR));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testEandZUnbrackettedBeforeEne() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testEandZUnbrackettedBeforeEne() throw(ComponentGenerationException)
						{ //not allowed in IUPAC names
							//XML for 2E,4Z-diene:
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *locant = new TokenEl(XmlDeclarations::LOCANT_EL, L"2");
							substituent->addChild(locant);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"E");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::E_OR_Z_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							substituent->addChild(stereochem);
							locant = new TokenEl(XmlDeclarations::LOCANT_EL, L"4");
							substituent->addChild(locant);
							stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"Z");
							Attribute tempVar2(XmlDeclarations::TYPE_ATR, XmlDeclarations::E_OR_Z_TYPE_VAL);
							stereochem->addAttribute(&tempVar2);
							substituent->addChild(stereochem);
							Element *multiplier = new TokenEl(XmlDeclarations::MULTIPLIER_EL, L"di");
							Attribute tempVar3(XmlDeclarations::VALUE_ATR, L"2");
							multiplier->addAttribute(&tempVar3);
							substituent->addChild(multiplier);
							Element *unsaturator = new TokenEl(XmlDeclarations::UNSATURATOR_EL, L"ene");
							Attribute tempVar4(XmlDeclarations::VALUE_ATR, L"2");
							unsaturator->addAttribute(&tempVar4);
							substituent->addChild(unsaturator);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(5, children.size());
							Element *modifiedStereochemistryEl1 = children[0];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, modifiedStereochemistryEl1->getName());
							assertEquals(L"2", modifiedStereochemistryEl1->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"E", modifiedStereochemistryEl1->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::E_OR_Z_TYPE_VAL, modifiedStereochemistryEl1->getAttributeValue(XmlDeclarations::TYPE_ATR));

							Element *modifiedStereochemistryEl2 = children[1];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, modifiedStereochemistryEl2->getName());
							assertEquals(L"4", modifiedStereochemistryEl2->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"Z", modifiedStereochemistryEl2->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::E_OR_Z_TYPE_VAL, modifiedStereochemistryEl2->getAttributeValue(XmlDeclarations::TYPE_ATR));

							Element *newLocant = children[2];
							assertEquals(XmlDeclarations::LOCANT_EL, newLocant->getName());
							assertEquals(L"2,4", newLocant->getValue());
							assertEquals(XmlDeclarations::MULTIPLIER_EL,children[3]->getName());
							assertEquals(XmlDeclarations::UNSATURATOR_EL, children[4]->getName());
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testEandZUnbrackettedBeforeYlidene() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testEandZUnbrackettedBeforeYlidene() throw(ComponentGenerationException)
						{ //not allowed in IUPAC names
							//XML for 2Z-ylidene:
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *locant = new TokenEl(XmlDeclarations::LOCANT_EL, L"2");
							substituent->addChild(locant);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"Z");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::E_OR_Z_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							substituent->addChild(stereochem);
							Element *suffix = new TokenEl(XmlDeclarations::SUFFIX_EL, L"ylidene");
							Attribute tempVar2(XmlDeclarations::VALUE_ATR, L"ylidene");
							suffix->addAttribute(&tempVar2);
							substituent->addChild(suffix);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(3, children.size());
							Element *modifiedStereochemistryEl1 = children[0];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, modifiedStereochemistryEl1->getName());
							assertEquals(L"2", modifiedStereochemistryEl1->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"Z", modifiedStereochemistryEl1->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::E_OR_Z_TYPE_VAL, modifiedStereochemistryEl1->getAttributeValue(XmlDeclarations::TYPE_ATR));

							Element *newLocant = children[1];
							assertEquals(XmlDeclarations::LOCANT_EL, newLocant->getName());
							assertEquals(L"2", newLocant->getValue());
							assertEquals(XmlDeclarations::SUFFIX_EL,children[2]->getName());
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testBrackettedAlphaBeta() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testBrackettedAlphaBeta() throw(ComponentGenerationException)
						{
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"(1a,2b,3bEtA,4alpha,5xi)");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::STEREOCHEMISTRYBRACKET_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							substituent->addChild(stereochem);
							Element *naturalProduct = new TokenEl(XmlDeclarations::GROUP_EL);
							Attribute tempVar2(XmlDeclarations::SUBTYPE_ATR, XmlDeclarations::BIOCHEMICAL_SUBTYPE_VAL);
							naturalProduct->addAttribute(&tempVar2);
							Attribute tempVar3(XmlDeclarations::ALPHABETACLOCKWISEATOMORDERING_ATR, L"");
							naturalProduct->addAttribute(&tempVar3);
							substituent->addChild(naturalProduct);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(6, children.size());
							Element *newStereochemistryEl = children[0];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl->getName());
							assertEquals(L"1", newStereochemistryEl->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"alpha", newStereochemistryEl->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::ALPHA_OR_BETA_TYPE_VAL, newStereochemistryEl->getAttributeValue(XmlDeclarations::TYPE_ATR));

							newStereochemistryEl = children[1];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl->getName());
							assertEquals(L"2", newStereochemistryEl->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"beta", newStereochemistryEl->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::ALPHA_OR_BETA_TYPE_VAL, newStereochemistryEl->getAttributeValue(XmlDeclarations::TYPE_ATR));

							newStereochemistryEl = children[2];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl->getName());
							assertEquals(L"3", newStereochemistryEl->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"beta", newStereochemistryEl->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::ALPHA_OR_BETA_TYPE_VAL, newStereochemistryEl->getAttributeValue(XmlDeclarations::TYPE_ATR));

							newStereochemistryEl = children[3];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl->getName());
							assertEquals(L"4", newStereochemistryEl->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"alpha", newStereochemistryEl->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::ALPHA_OR_BETA_TYPE_VAL, newStereochemistryEl->getAttributeValue(XmlDeclarations::TYPE_ATR));

							newStereochemistryEl = children[4];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl->getName());
							assertEquals(L"5", newStereochemistryEl->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"xi", newStereochemistryEl->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::ALPHA_OR_BETA_TYPE_VAL, newStereochemistryEl->getAttributeValue(XmlDeclarations::TYPE_ATR));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAlphaBeta() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testAlphaBeta() throw(ComponentGenerationException)
						{
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"3beta,5alpha");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::ALPHA_OR_BETA_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							substituent->addChild(stereochem);
							Element *naturalProduct = new TokenEl(XmlDeclarations::GROUP_EL);
							Attribute tempVar2(XmlDeclarations::SUBTYPE_ATR, XmlDeclarations::BIOCHEMICAL_SUBTYPE_VAL);
							naturalProduct->addAttribute(&tempVar2);
							Attribute tempVar3(XmlDeclarations::ALPHABETACLOCKWISEATOMORDERING_ATR, L"");
							naturalProduct->addAttribute(&tempVar3);
							substituent->addChild(naturalProduct);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(3, children.size());
							Element *newStereochemistryEl = children[0];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl->getName());
							assertEquals(L"3", newStereochemistryEl->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"beta", newStereochemistryEl->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::ALPHA_OR_BETA_TYPE_VAL, newStereochemistryEl->getAttributeValue(XmlDeclarations::TYPE_ATR));

							newStereochemistryEl = children[1];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl->getName());
							assertEquals(L"5", newStereochemistryEl->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"alpha", newStereochemistryEl->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::ALPHA_OR_BETA_TYPE_VAL, newStereochemistryEl->getAttributeValue(XmlDeclarations::TYPE_ATR));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAlphaBetaNotDirectlyPrecedingANaturalProduct1() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testAlphaBetaNotDirectlyPrecedingANaturalProduct1() throw(ComponentGenerationException)
						{
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"3beta,5alpha");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::ALPHA_OR_BETA_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							substituent->addChild(stereochem);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(3, children.size());
							Element *newStereochemistryEl = children[0];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl->getName());
							assertEquals(L"3", newStereochemistryEl->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"beta", newStereochemistryEl->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::ALPHA_OR_BETA_TYPE_VAL, newStereochemistryEl->getAttributeValue(XmlDeclarations::TYPE_ATR));

							newStereochemistryEl = children[1];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl->getName());
							assertEquals(L"5", newStereochemistryEl->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"alpha", newStereochemistryEl->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::ALPHA_OR_BETA_TYPE_VAL, newStereochemistryEl->getAttributeValue(XmlDeclarations::TYPE_ATR));

							Element *newLocantEl = children[2];
							assertEquals(XmlDeclarations::LOCANT_EL, newLocantEl->getName());
							assertEquals(L"3,5", newLocantEl->getValue());
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAlphaBetaNotDirectlyPrecedingANaturalProduct2() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testAlphaBetaNotDirectlyPrecedingANaturalProduct2() throw(ComponentGenerationException)
						{
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"(3beta,5alpha)");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::STEREOCHEMISTRYBRACKET_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							substituent->addChild(stereochem);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(2, children.size());
							Element *newStereochemistryEl = children[0];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl->getName());
							assertEquals(L"3", newStereochemistryEl->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"beta", newStereochemistryEl->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::ALPHA_OR_BETA_TYPE_VAL, newStereochemistryEl->getAttributeValue(XmlDeclarations::TYPE_ATR));

							newStereochemistryEl = children[1];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl->getName());
							assertEquals(L"5", newStereochemistryEl->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"alpha", newStereochemistryEl->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::ALPHA_OR_BETA_TYPE_VAL, newStereochemistryEl->getAttributeValue(XmlDeclarations::TYPE_ATR));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAlphaBetaNotDirectlyPrecedingANaturalProduct3() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testAlphaBetaNotDirectlyPrecedingANaturalProduct3() throw(ComponentGenerationException)
						{
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *naturalProduct = new TokenEl(XmlDeclarations::GROUP_EL);
							Attribute tempVar(XmlDeclarations::SUBTYPE_ATR, XmlDeclarations::BIOCHEMICAL_SUBTYPE_VAL);
							naturalProduct->addAttribute(&tempVar);
							Attribute tempVar2(XmlDeclarations::ALPHABETACLOCKWISEATOMORDERING_ATR, L"");
							naturalProduct->addAttribute(&tempVar2);
							substituent->addChild(naturalProduct);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"3beta,5alpha");
							Attribute tempVar3(XmlDeclarations::TYPE_ATR, XmlDeclarations::ALPHA_OR_BETA_TYPE_VAL);
							stereochem->addAttribute(&tempVar3);
							substituent->addChild(stereochem);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(4, children.size());
							Element *newStereochemistryEl = children[1];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl->getName());
							assertEquals(L"3", newStereochemistryEl->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"beta", newStereochemistryEl->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::ALPHA_OR_BETA_TYPE_VAL, newStereochemistryEl->getAttributeValue(XmlDeclarations::TYPE_ATR));

							newStereochemistryEl = children[2];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl->getName());
							assertEquals(L"5", newStereochemistryEl->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"alpha", newStereochemistryEl->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::ALPHA_OR_BETA_TYPE_VAL, newStereochemistryEl->getAttributeValue(XmlDeclarations::TYPE_ATR));

							Element *newLocantEl = children[3];
							assertEquals(XmlDeclarations::LOCANT_EL, newLocantEl->getName());
							assertEquals(L"3,5", newLocantEl->getValue());
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAlphaBetaStereoMixedWithNormalLocants() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testAlphaBetaStereoMixedWithNormalLocants() throw(ComponentGenerationException)
						{
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"3beta,4,10,12alpha");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::ALPHA_OR_BETA_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							substituent->addChild(stereochem);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(3, children.size());
							Element *newStereochemistryEl = children[0];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl->getName());
							assertEquals(L"3", newStereochemistryEl->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"beta", newStereochemistryEl->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::ALPHA_OR_BETA_TYPE_VAL, newStereochemistryEl->getAttributeValue(XmlDeclarations::TYPE_ATR));

							newStereochemistryEl = children[1];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl->getName());
							assertEquals(L"12", newStereochemistryEl->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"alpha", newStereochemistryEl->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::ALPHA_OR_BETA_TYPE_VAL, newStereochemistryEl->getAttributeValue(XmlDeclarations::TYPE_ATR));

							Element *newLocantEl = children[2];
							assertEquals(XmlDeclarations::LOCANT_EL, newLocantEl->getName());
							assertEquals(L"3,4,10,12", newLocantEl->getValue());
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRelativeStereoChemistry1() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testRelativeStereoChemistry1() throw(ComponentGenerationException)
						{
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"rel-(1R,3S,4S,7R)");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::STEREOCHEMISTRYBRACKET_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							substituent->addChild(stereochem);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(4, children.size());
							Element *newStereochemistryEl1 = children[0];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl1->getName());
							assertEquals(L"1", newStereochemistryEl1->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"R", newStereochemistryEl1->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::R_OR_S_TYPE_VAL, newStereochemistryEl1->getAttributeValue(XmlDeclarations::TYPE_ATR));

							Element *newStereochemistryEl2 = children[1];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl2->getName());
							assertEquals(L"3", newStereochemistryEl2->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"S", newStereochemistryEl2->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::R_OR_S_TYPE_VAL, newStereochemistryEl2->getAttributeValue(XmlDeclarations::TYPE_ATR));

							Element *newStereochemistryEl3 = children[2];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl3->getName());
							assertEquals(L"4", newStereochemistryEl3->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"S", newStereochemistryEl3->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::R_OR_S_TYPE_VAL, newStereochemistryEl3->getAttributeValue(XmlDeclarations::TYPE_ATR));

							Element *newStereochemistryEl4 = children[3];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl4->getName());
							assertEquals(L"7", newStereochemistryEl4->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"R", newStereochemistryEl4->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::R_OR_S_TYPE_VAL, newStereochemistryEl4->getAttributeValue(XmlDeclarations::TYPE_ATR));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRelativeStereoChemistry2() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testRelativeStereoChemistry2() throw(ComponentGenerationException)
						{
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"(1R*,3S*,4S*,7R*)");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::STEREOCHEMISTRYBRACKET_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							substituent->addChild(stereochem);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(4, children.size());
							Element *newStereochemistryEl1 = children[0];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl1->getName());
							assertEquals(L"1", newStereochemistryEl1->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"R", newStereochemistryEl1->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::R_OR_S_TYPE_VAL, newStereochemistryEl1->getAttributeValue(XmlDeclarations::TYPE_ATR));

							Element *newStereochemistryEl2 = children[1];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl2->getName());
							assertEquals(L"3", newStereochemistryEl2->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"S", newStereochemistryEl2->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::R_OR_S_TYPE_VAL, newStereochemistryEl2->getAttributeValue(XmlDeclarations::TYPE_ATR));

							Element *newStereochemistryEl3 = children[2];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl3->getName());
							assertEquals(L"4", newStereochemistryEl3->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"S", newStereochemistryEl3->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::R_OR_S_TYPE_VAL, newStereochemistryEl3->getAttributeValue(XmlDeclarations::TYPE_ATR));

							Element *newStereochemistryEl4 = children[3];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl4->getName());
							assertEquals(L"7", newStereochemistryEl4->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"R", newStereochemistryEl4->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::R_OR_S_TYPE_VAL, newStereochemistryEl4->getAttributeValue(XmlDeclarations::TYPE_ATR));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRelativeStereoChemistry3() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testRelativeStereoChemistry3() throw(ComponentGenerationException)
						{
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"rel-");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::STEREOCHEMISTRYBRACKET_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							substituent->addChild(stereochem);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(0, children.size());
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRelativeCisTrans() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testRelativeCisTrans() throw(ComponentGenerationException)
						{
							//c-4-
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"c-4-");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::RELATIVECISTRANS_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							substituent->addChild(stereochem);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(2, children.size());
							Element *modifiedStereochemistryEl1 = children[0];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, modifiedStereochemistryEl1->getName());
							assertEquals(nullptr, modifiedStereochemistryEl1->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"c-4-", modifiedStereochemistryEl1->getValue());
							assertEquals(XmlDeclarations::RELATIVECISTRANS_TYPE_VAL, modifiedStereochemistryEl1->getAttributeValue(XmlDeclarations::TYPE_ATR));
							Element *locant = children[1];
							assertEquals(XmlDeclarations::LOCANT_EL, locant->getName());
							assertEquals(L"4", locant->getValue());
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRacemate1() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testRacemate1() throw(ComponentGenerationException)
						{
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"rac-(2R)");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::STEREOCHEMISTRYBRACKET_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							substituent->addChild(stereochem);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(1, children.size());
							Element *newStereochemistryEl = children[0];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl->getName());
							assertEquals(L"2", newStereochemistryEl->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"RS", newStereochemistryEl->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::R_OR_S_TYPE_VAL, newStereochemistryEl->getAttributeValue(XmlDeclarations::TYPE_ATR));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRacemate2() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testRacemate2() throw(ComponentGenerationException)
						{
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"(RS)");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::STEREOCHEMISTRYBRACKET_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							substituent->addChild(stereochem);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(1, children.size());
							Element *newStereochemistryEl = children[0];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl->getName());
							assertEquals(nullptr, newStereochemistryEl->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"RS", newStereochemistryEl->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::R_OR_S_TYPE_VAL, newStereochemistryEl->getAttributeValue(XmlDeclarations::TYPE_ATR));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRacemate2_ci() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testRacemate2_ci() throw(ComponentGenerationException)
						{
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"(rs)");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::STEREOCHEMISTRYBRACKET_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							substituent->addChild(stereochem);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(1, children.size());
							Element *newStereochemistryEl = children[0];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl->getName());
							assertEquals(nullptr, newStereochemistryEl->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"RS", newStereochemistryEl->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::R_OR_S_TYPE_VAL, newStereochemistryEl->getAttributeValue(XmlDeclarations::TYPE_ATR));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRacemate3() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testRacemate3() throw(ComponentGenerationException)
						{
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"(SR)");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::STEREOCHEMISTRYBRACKET_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							substituent->addChild(stereochem);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(1, children.size());
							Element *newStereochemistryEl = children[0];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl->getName());
							assertEquals(nullptr, newStereochemistryEl->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"SR", newStereochemistryEl->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::R_OR_S_TYPE_VAL, newStereochemistryEl->getAttributeValue(XmlDeclarations::TYPE_ATR));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRacemate4() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testRacemate4() throw(ComponentGenerationException)
						{
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"rac-(2R,4S)");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::STEREOCHEMISTRYBRACKET_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							substituent->addChild(stereochem);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(2, children.size());
							Element *newStereochemistryEl1 = children[0];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl1->getName());
							assertEquals(L"2", newStereochemistryEl1->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"RS", newStereochemistryEl1->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::R_OR_S_TYPE_VAL, newStereochemistryEl1->getAttributeValue(XmlDeclarations::TYPE_ATR));

							Element *newStereochemistryEl2 = children[1];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl2->getName());
							assertEquals(L"4", newStereochemistryEl2->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"SR", newStereochemistryEl2->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::R_OR_S_TYPE_VAL, newStereochemistryEl2->getAttributeValue(XmlDeclarations::TYPE_ATR));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRacemate5() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testRacemate5() throw(ComponentGenerationException)
						{
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"(2RS,4SR)");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::STEREOCHEMISTRYBRACKET_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							substituent->addChild(stereochem);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(2, children.size());
							Element *newStereochemistryEl1 = children[0];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl1->getName());
							assertEquals(L"2", newStereochemistryEl1->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"RS", newStereochemistryEl1->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::R_OR_S_TYPE_VAL, newStereochemistryEl1->getAttributeValue(XmlDeclarations::TYPE_ATR));

							Element *newStereochemistryEl2 = children[1];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl2->getName());
							assertEquals(L"4", newStereochemistryEl2->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"SR", newStereochemistryEl2->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::R_OR_S_TYPE_VAL, newStereochemistryEl2->getAttributeValue(XmlDeclarations::TYPE_ATR));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRacemate6() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testRacemate6() throw(ComponentGenerationException)
						{
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"rac-");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::STEREOCHEMISTRYBRACKET_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							substituent->addChild(stereochem);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(0, children.size());
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRacemate7() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testRacemate7() throw(ComponentGenerationException)
						{
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"racem-");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::STEREOCHEMISTRYBRACKET_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							substituent->addChild(stereochem);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(0, children.size());
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRacemate8() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testRacemate8() throw(ComponentGenerationException)
						{
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"racemic-");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::STEREOCHEMISTRYBRACKET_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							substituent->addChild(stereochem);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(0, children.size());
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRacemate9() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testRacemate9() throw(ComponentGenerationException)
						{
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"(R/S)-");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::STEREOCHEMISTRYBRACKET_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							substituent->addChild(stereochem);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(1, children.size());
							Element *newStereochemistryEl = children[0];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, newStereochemistryEl->getName());
							assertEquals(nullptr, newStereochemistryEl->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"RS", newStereochemistryEl->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::R_OR_S_TYPE_VAL, newStereochemistryEl->getAttributeValue(XmlDeclarations::TYPE_ATR));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRacemate10() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testRacemate10() throw(ComponentGenerationException)
						{
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"(RAC)");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::STEREOCHEMISTRYBRACKET_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							substituent->addChild(stereochem);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(0, children.size());
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRacemateEz1() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testRacemateEz1() throw(ComponentGenerationException)
						{
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"(EZ)");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::STEREOCHEMISTRYBRACKET_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							substituent->addChild(stereochem);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							Element *modifiedStereochemistryEl1 = children[0];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, modifiedStereochemistryEl1->getName());
							assertEquals(L"EZ", modifiedStereochemistryEl1->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::E_OR_Z_TYPE_VAL, modifiedStereochemistryEl1->getAttributeValue(XmlDeclarations::TYPE_ATR));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRacemateEz2() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testRacemateEz2() throw(ComponentGenerationException)
						{
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"(2EZ)");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::STEREOCHEMISTRYBRACKET_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							substituent->addChild(stereochem);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(1, children.size());
							Element *modifiedStereochemistryEl1 = children[0];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, modifiedStereochemistryEl1->getName());
							assertEquals(L"2", modifiedStereochemistryEl1->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"EZ", modifiedStereochemistryEl1->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::E_OR_Z_TYPE_VAL, modifiedStereochemistryEl1->getAttributeValue(XmlDeclarations::TYPE_ATR));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRacemateEz3_unbracketted() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testRacemateEz3_unbracketted() throw(ComponentGenerationException)
						{
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *locant = new TokenEl(XmlDeclarations::LOCANT_EL, L"2");
							substituent->addChild(locant);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"ez");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::E_OR_Z_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							substituent->addChild(stereochem);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							assertEquals(1, children.size());
							Element *modifiedStereochemistryEl1 = children[0];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, modifiedStereochemistryEl1->getName());
							assertEquals(L"2", modifiedStereochemistryEl1->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							assertEquals(L"EZ", modifiedStereochemistryEl1->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::E_OR_Z_TYPE_VAL, modifiedStereochemistryEl1->getAttributeValue(XmlDeclarations::TYPE_ATR));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRacemateEz4_unbracketted() throws ComponentGenerationException
						void ComponentGeneration_StereochemistryTest::testRacemateEz4_unbracketted() throw(ComponentGenerationException)
						{
							Element *substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							Element *stereochem = new TokenEl(XmlDeclarations::STEREOCHEMISTRY_EL, L"EZ");
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::E_OR_Z_TYPE_VAL);
							stereochem->addAttribute(&tempVar);
							substituent->addChild(stereochem);
							processStereochemistry(substituent);

							std::vector<Element*> children = substituent->getChildElements();
							Element *modifiedStereochemistryEl1 = children[0];
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, modifiedStereochemistryEl1->getName());
							assertEquals(L"EZ", modifiedStereochemistryEl1->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(XmlDeclarations::E_OR_Z_TYPE_VAL, modifiedStereochemistryEl1->getAttributeValue(XmlDeclarations::TYPE_ATR));
						}

						void ComponentGeneration_StereochemistryTest::processStereochemistry(Element *subOrRoot) throw(ComponentGenerationException)
						{
							ComponentGenerator tempVar(new NameToStructureConfig());
							(&tempVar)->processStereochemistry(subOrRoot);
						}
					}
				}
			}
		}
	}
}
