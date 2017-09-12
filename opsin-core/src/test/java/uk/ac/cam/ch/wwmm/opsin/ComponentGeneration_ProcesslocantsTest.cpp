#include "ComponentGeneration_ProcesslocantsTest.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/Element.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/GroupingEl.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/XmlDeclarations.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/TokenEl.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/ComponentGenerationException.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/ComponentGenerator.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/OpsinTools.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/Attribute.h"

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
						using org::junit::Before;
						using org::junit::Test;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void setUpSubstituent()
						void ComponentGeneration_ProcesslocantsTest::setUpSubstituent()
						{
							substituent = new GroupingEl(XmlDeclarations::SUBSTITUENT_EL);
							locant = new TokenEl(XmlDeclarations::LOCANT_EL);
							substituent->addChild(locant);
							TokenEl tempVar(XmlDeclarations::GROUP_EL);
							substituent->addChild(&tempVar); //a dummy element to give the locant a potential purpose
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCardinalNumber() throws ComponentGenerationException
						void ComponentGeneration_ProcesslocantsTest::testCardinalNumber() throw(ComponentGenerationException)
						{
							locant->setValue(L"1");
							ComponentGenerator::processLocants(substituent);
							assertEquals(L"1", locant->getValue());
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCardinalNumberWithHyphen() throws ComponentGenerationException
						void ComponentGeneration_ProcesslocantsTest::testCardinalNumberWithHyphen() throw(ComponentGenerationException)
						{
							locant->setValue(L"1-");
							ComponentGenerator::processLocants(substituent);
							assertEquals(L"1", locant->getValue());
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testElementSymbol() throws ComponentGenerationException
						void ComponentGeneration_ProcesslocantsTest::testElementSymbol() throw(ComponentGenerationException)
						{
							locant->setValue(L"N-");
							ComponentGenerator::processLocants(substituent);
							assertEquals(L"N", locant->getValue());
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAminoAcidStyleLocant() throws ComponentGenerationException
						void ComponentGeneration_ProcesslocantsTest::testAminoAcidStyleLocant() throw(ComponentGenerationException)
						{
							locant->setValue(L"N1-");
							ComponentGenerator::processLocants(substituent);
							assertEquals(L"N1", locant->getValue());
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCompoundLocant() throws ComponentGenerationException
						void ComponentGeneration_ProcesslocantsTest::testCompoundLocant() throw(ComponentGenerationException)
						{
							locant->setValue(L"1(10)-");
							ComponentGenerator::processLocants(substituent);
							assertEquals(L"1(10)", locant->getValue());
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testGreek() throws ComponentGenerationException
						void ComponentGeneration_ProcesslocantsTest::testGreek() throw(ComponentGenerationException)
						{
							locant->setValue(L"alpha");
							ComponentGenerator::processLocants(substituent);
							assertEquals(L"alpha", locant->getValue());
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testNotlowercase1() throws ComponentGenerationException
						void ComponentGeneration_ProcesslocantsTest::testNotlowercase1() throw(ComponentGenerationException)
						{
							locant->setValue(L"AlPhA-");
							ComponentGenerator::processLocants(substituent);
							assertEquals(L"alpha", locant->getValue());
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testNotlowercase2() throws ComponentGenerationException
						void ComponentGeneration_ProcesslocantsTest::testNotlowercase2() throw(ComponentGenerationException)
						{
							locant->setValue(L"NAlPhA-");
							ComponentGenerator::processLocants(substituent);
							assertEquals(L"Nalpha", locant->getValue());
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testIUPAC2004() throws ComponentGenerationException
						void ComponentGeneration_ProcesslocantsTest::testIUPAC2004() throw(ComponentGenerationException)
						{
							locant->setValue(L"2-N-");
							ComponentGenerator::processLocants(substituent);
							assertEquals(L"N2", locant->getValue());
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSuperscript1() throws ComponentGenerationException
						void ComponentGeneration_ProcesslocantsTest::testSuperscript1() throw(ComponentGenerationException)
						{
							locant->setValue(L"N^(2)");
							ComponentGenerator::processLocants(substituent);
							assertEquals(L"N2", locant->getValue());
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSuperscript2() throws ComponentGenerationException
						void ComponentGeneration_ProcesslocantsTest::testSuperscript2() throw(ComponentGenerationException)
						{
							locant->setValue(L"N^2");
							ComponentGenerator::processLocants(substituent);
							assertEquals(L"N2", locant->getValue());
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSuperscript3() throws ComponentGenerationException
						void ComponentGeneration_ProcesslocantsTest::testSuperscript3() throw(ComponentGenerationException)
						{
							locant->setValue(L"N(2)");
							ComponentGenerator::processLocants(substituent);
							assertEquals(L"N2", locant->getValue());
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSuperscript4() throws ComponentGenerationException
						void ComponentGeneration_ProcesslocantsTest::testSuperscript4() throw(ComponentGenerationException)
						{
							locant->setValue(L"N~12~");
							ComponentGenerator::processLocants(substituent);
							assertEquals(L"N12", locant->getValue());
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSuperscript5() throws ComponentGenerationException
						void ComponentGeneration_ProcesslocantsTest::testSuperscript5() throw(ComponentGenerationException)
						{
							locant->setValue(L"N(alpha)");
							ComponentGenerator::processLocants(substituent);
							assertEquals(L"Nalpha", locant->getValue());
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSuperscript6() throws ComponentGenerationException
						void ComponentGeneration_ProcesslocantsTest::testSuperscript6() throw(ComponentGenerationException)
						{
							locant->setValue(L"N^alpha");
							ComponentGenerator::processLocants(substituent);
							assertEquals(L"Nalpha", locant->getValue());
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSuperscript7() throws ComponentGenerationException
						void ComponentGeneration_ProcesslocantsTest::testSuperscript7() throw(ComponentGenerationException)
						{
							locant->setValue(L"N*12*");
							ComponentGenerator::processLocants(substituent);
							assertEquals(L"N12", locant->getValue());
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAddedHydrogen() throws ComponentGenerationException
						void ComponentGeneration_ProcesslocantsTest::testAddedHydrogen() throw(ComponentGenerationException)
						{
							locant->setValue(L"3(5'H)");
							ComponentGenerator::processLocants(substituent);
							assertEquals(L"3", locant->getValue());
							assertEquals(XmlDeclarations::ADDEDHYDROGENLOCANT_TYPE_VAL, locant->getAttributeValue(XmlDeclarations::TYPE_ATR));
							Element *addedHydrogen = OpsinTools::getPreviousSibling(locant);
							assertNotNull(addedHydrogen);
							assertEquals(XmlDeclarations::ADDEDHYDROGEN_EL, addedHydrogen->getName());
							assertEquals(L"5'", addedHydrogen->getAttributeValue(XmlDeclarations::LOCANT_ATR));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAddedHydrogen2() throws ComponentGenerationException
						void ComponentGeneration_ProcesslocantsTest::testAddedHydrogen2() throw(ComponentGenerationException)
						{
							locant->setValue(L"1,2(2H,7H)");
							ComponentGenerator::processLocants(substituent);
							assertEquals(L"1,2", locant->getValue());
							assertEquals(XmlDeclarations::ADDEDHYDROGENLOCANT_TYPE_VAL, locant->getAttributeValue(XmlDeclarations::TYPE_ATR));
							Element *addedHydrogen1 = OpsinTools::getPreviousSibling(locant);
							assertNotNull(addedHydrogen1);
							assertEquals(XmlDeclarations::ADDEDHYDROGEN_EL, addedHydrogen1->getName());
							assertEquals(L"7", addedHydrogen1->getAttributeValue(XmlDeclarations::LOCANT_ATR));
							Element *addedHydrogen2 = OpsinTools::getPreviousSibling(addedHydrogen1);
							assertNotNull(addedHydrogen2);
							assertEquals(XmlDeclarations::ADDEDHYDROGEN_EL, addedHydrogen2->getName());
							assertEquals(L"2", addedHydrogen2->getAttributeValue(XmlDeclarations::LOCANT_ATR));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testStereochemistryInLocant1() throws ComponentGenerationException
						void ComponentGeneration_ProcesslocantsTest::testStereochemistryInLocant1() throw(ComponentGenerationException)
						{
							locant->setValue(L"5(R)");
							ComponentGenerator::processLocants(substituent);
							assertEquals(L"5", locant->getValue());
							Element *stereochemistry = OpsinTools::getPreviousSibling(locant);
							assertNotNull(stereochemistry);
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, stereochemistry->getName());
							assertEquals(XmlDeclarations::STEREOCHEMISTRYBRACKET_TYPE_VAL, stereochemistry->getAttributeValue(XmlDeclarations::TYPE_ATR));
							assertEquals(L"(5R)", stereochemistry->getValue()); //will be handled by process stereochemistry function
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testStereochemistryInLocant2() throws ComponentGenerationException
						void ComponentGeneration_ProcesslocantsTest::testStereochemistryInLocant2() throw(ComponentGenerationException)
						{
							locant->setValue(L"5-(S)");
							ComponentGenerator::processLocants(substituent);
							assertEquals(L"5", locant->getValue());
							Element *stereochemistry = OpsinTools::getPreviousSibling(locant);
							assertNotNull(stereochemistry);
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, stereochemistry->getName());
							assertEquals(XmlDeclarations::STEREOCHEMISTRYBRACKET_TYPE_VAL, stereochemistry->getAttributeValue(XmlDeclarations::TYPE_ATR));
							assertEquals(L"(5S)", stereochemistry->getValue()); //will be handled by process stereochemistry function
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testStereochemistryInLocant3() throws ComponentGenerationException
						void ComponentGeneration_ProcesslocantsTest::testStereochemistryInLocant3() throw(ComponentGenerationException)
						{
							locant->setValue(L"N(3)-(S)");
							ComponentGenerator::processLocants(substituent);
							assertEquals(L"N3", locant->getValue());
							Element *stereochemistry = OpsinTools::getPreviousSibling(locant);
							assertNotNull(stereochemistry);
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, stereochemistry->getName());
							assertEquals(XmlDeclarations::STEREOCHEMISTRYBRACKET_TYPE_VAL, stereochemistry->getAttributeValue(XmlDeclarations::TYPE_ATR));
							assertEquals(L"(N3S)", stereochemistry->getValue()); //will be handled by process stereochemistry function
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testStereochemistryInLocant4() throws ComponentGenerationException
						void ComponentGeneration_ProcesslocantsTest::testStereochemistryInLocant4() throw(ComponentGenerationException)
						{
							locant->setValue(L"5(RS)");
							ComponentGenerator::processLocants(substituent);
							assertEquals(L"5", locant->getValue());
							Element *stereochemistry = OpsinTools::getPreviousSibling(locant);
							assertNotNull(stereochemistry);
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, stereochemistry->getName());
							assertEquals(XmlDeclarations::STEREOCHEMISTRYBRACKET_TYPE_VAL, stereochemistry->getAttributeValue(XmlDeclarations::TYPE_ATR));
							assertEquals(L"(5RS)", stereochemistry->getValue()); //will be handled by process stereochemistry function
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testStereochemistryInLocant5() throws ComponentGenerationException
						void ComponentGeneration_ProcesslocantsTest::testStereochemistryInLocant5() throw(ComponentGenerationException)
						{
							locant->setValue(L"5(R,S)");
							ComponentGenerator::processLocants(substituent);
							assertEquals(L"5", locant->getValue());
							Element *stereochemistry = OpsinTools::getPreviousSibling(locant);
							assertNotNull(stereochemistry);
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, stereochemistry->getName());
							assertEquals(XmlDeclarations::STEREOCHEMISTRYBRACKET_TYPE_VAL, stereochemistry->getAttributeValue(XmlDeclarations::TYPE_ATR));
							assertEquals(L"(5RS)", stereochemistry->getValue()); //will be handled by process stereochemistry function
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testStereochemistryInLocant6() throws ComponentGenerationException
						void ComponentGeneration_ProcesslocantsTest::testStereochemistryInLocant6() throw(ComponentGenerationException)
						{
							locant->setValue(L"5(R/S)");
							ComponentGenerator::processLocants(substituent);
							assertEquals(L"5", locant->getValue());
							Element *stereochemistry = OpsinTools::getPreviousSibling(locant);
							assertNotNull(stereochemistry);
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, stereochemistry->getName());
							assertEquals(XmlDeclarations::STEREOCHEMISTRYBRACKET_TYPE_VAL, stereochemistry->getAttributeValue(XmlDeclarations::TYPE_ATR));
							assertEquals(L"(5RS)", stereochemistry->getValue()); //will be handled by process stereochemistry function
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testMultipleCardinals() throws ComponentGenerationException
						void ComponentGeneration_ProcesslocantsTest::testMultipleCardinals() throw(ComponentGenerationException)
						{
							locant->setValue(L"2,3-");
							ComponentGenerator::processLocants(substituent);
							assertEquals(L"2,3", locant->getValue());
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testMultipleTypesTogether() throws ComponentGenerationException
						void ComponentGeneration_ProcesslocantsTest::testMultipleTypesTogether() throw(ComponentGenerationException)
						{
							locant->setValue(L"2,N5,GaMMa,3-N,N^3,N(2),N~10~,4(5H),3-N(S),1(6)-");
							ComponentGenerator::processLocants(substituent);
							assertEquals(L"2,N5,gamma,N3,N3,N2,N10,4,N3,1(6)", locant->getValue());
							assertEquals(XmlDeclarations::ADDEDHYDROGENLOCANT_TYPE_VAL, locant->getAttributeValue(XmlDeclarations::TYPE_ATR));
							Element *stereochemistry = OpsinTools::getPreviousSibling(locant);
							assertNotNull(stereochemistry);
							assertEquals(XmlDeclarations::STEREOCHEMISTRY_EL, stereochemistry->getName());
							assertEquals(XmlDeclarations::STEREOCHEMISTRYBRACKET_TYPE_VAL, stereochemistry->getAttributeValue(XmlDeclarations::TYPE_ATR));
							assertEquals(L"(N3S)", stereochemistry->getValue());
							Element *addedHydrogen = OpsinTools::getPreviousSibling(stereochemistry);
							assertNotNull(addedHydrogen);
							assertEquals(XmlDeclarations::ADDEDHYDROGEN_EL, addedHydrogen->getName());
							assertEquals(L"5", addedHydrogen->getAttributeValue(XmlDeclarations::LOCANT_ATR));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCarbohydrateStyleLocants() throws ComponentGenerationException
						void ComponentGeneration_ProcesslocantsTest::testCarbohydrateStyleLocants() throw(ComponentGenerationException)
						{
							//2,4,6-tri-O
							locant->setValue(L"O");
							Element *multiplier = new TokenEl(XmlDeclarations::MULTIPLIER_EL);
							Attribute tempVar(XmlDeclarations::VALUE_ATR, L"3");
							multiplier->addAttribute(&tempVar);
							OpsinTools::insertBefore(locant, multiplier);
							Element *numericLocant = new TokenEl(XmlDeclarations::LOCANT_EL);
							numericLocant->setValue(L"2,4,6");
							OpsinTools::insertBefore(multiplier, numericLocant);
							ComponentGenerator::processLocants(substituent);
							assertEquals(L"O2,O4,O6", numericLocant->getValue());
							Element *group = OpsinTools::getNextSibling(multiplier);
							assertNotNull(group);
							assertEquals(group->getName(), XmlDeclarations::GROUP_EL);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCarbohydrateStyleLocantsNoNumericComponent() throws ComponentGenerationException
						void ComponentGeneration_ProcesslocantsTest::testCarbohydrateStyleLocantsNoNumericComponent() throw(ComponentGenerationException)
						{
							//tri-O
							locant->setValue(L"O");
							Element *multiplier = new TokenEl(XmlDeclarations::MULTIPLIER_EL);
							Attribute tempVar(XmlDeclarations::VALUE_ATR, L"3");
							multiplier->addAttribute(&tempVar);
							OpsinTools::insertBefore(locant, multiplier);
							ComponentGenerator::processLocants(substituent);
							Element *elBeforeMultiplier = OpsinTools::getPreviousSibling(multiplier);
							assertNotNull(L"A locant should not be in front of the multiplier", elBeforeMultiplier);
							assertEquals(XmlDeclarations::LOCANT_EL, elBeforeMultiplier->getName());
							assertEquals(L"O,O',O''", elBeforeMultiplier->getValue());
							Element *group = OpsinTools::getNextSibling(multiplier);
							assertNotNull(group);
							assertEquals(group->getName(), XmlDeclarations::GROUP_EL);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCarbohydrateStyleLocantsCounterExample() throws ComponentGenerationException
						void ComponentGeneration_ProcesslocantsTest::testCarbohydrateStyleLocantsCounterExample() throw(ComponentGenerationException)
						{
							//2,4,6-tri-2 (this is not a carbohydrate style locant)
							locant->setValue(L"2");
							Element *multiplier = new TokenEl(XmlDeclarations::MULTIPLIER_EL);
							Attribute tempVar(XmlDeclarations::VALUE_ATR, L"3");
							multiplier->addAttribute(&tempVar);
							OpsinTools::insertBefore(locant, multiplier);
							Element *numericLocant = new TokenEl(XmlDeclarations::LOCANT_EL);
							numericLocant->setValue(L"2,4,6");
							OpsinTools::insertBefore(multiplier, numericLocant);
							ComponentGenerator::processLocants(substituent);
							assertEquals(L"2,4,6", numericLocant->getValue());
							Element *unmodifiedLocant = OpsinTools::getNextSibling(multiplier);
							assertNotNull(unmodifiedLocant);
							assertEquals(unmodifiedLocant->getName(), XmlDeclarations::LOCANT_EL);
							assertEquals(L"2", unmodifiedLocant->getValue());
						}
					}
				}
			}
		}
	}
}
