#include "ComponentGeneration_MiscTest.h"
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
//						import static org.junit.Assert.assertEquals;
						using namespace uk::ac::cam::ch::wwmm::opsin;
//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;
						using org::junit::Test;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=ComponentGenerationException.class) public void testRejectSingleComponentSaltComponent() throws ComponentGenerationException
						void ComponentGeneration_MiscTest::testRejectSingleComponentSaltComponent() throw(ComponentGenerationException)
						{
							//reject "hydrate"
							Element *molecule = new GroupingEl(XmlDeclarations::MOLECULE_EL);
							Element *wordRule = new GroupingEl(XmlDeclarations::WORDRULE_EL);
							Element *word = new GroupingEl(XmlDeclarations::WORD_EL);
							Element *root = new GroupingEl(XmlDeclarations::ROOT_EL);
							Element *group = new TokenEl(XmlDeclarations::GROUP_EL);
							Attribute tempVar(XmlDeclarations::TYPE_ATR, XmlDeclarations::SIMPLEGROUP_TYPE_VAL);
							group->addAttribute(&tempVar);
							Attribute tempVar2(XmlDeclarations::SUBTYPE_ATR, XmlDeclarations::SALTCOMPONENT_SUBTYPE_VAL);
							group->addAttribute(&tempVar2);
							root->addChild(group);
							word->addChild(root);
							wordRule->addChild(word);
							molecule->addChild(wordRule);
							processComponents(molecule);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testNumericallyMultipliedSaltComponent() throws ComponentGenerationException
						void ComponentGeneration_MiscTest::testNumericallyMultipliedSaltComponent() throw(ComponentGenerationException)
						{
							Element *molecule = new GroupingEl(XmlDeclarations::MOLECULE_EL);
							GroupingEl tempVar(XmlDeclarations::WORDRULE_EL);
							molecule->addChild(&tempVar);

							Element *wordRule = new GroupingEl(XmlDeclarations::WORDRULE_EL);
							Element *word = new GroupingEl(XmlDeclarations::WORD_EL);
							Element *root = new GroupingEl(XmlDeclarations::ROOT_EL);
							Element *group = new TokenEl(XmlDeclarations::GROUP_EL);
							Attribute tempVar2(XmlDeclarations::TYPE_ATR, XmlDeclarations::SIMPLEGROUP_TYPE_VAL);
							group->addAttribute(&tempVar2);
							Attribute tempVar3(XmlDeclarations::SUBTYPE_ATR, XmlDeclarations::SALTCOMPONENT_SUBTYPE_VAL);
							group->addAttribute(&tempVar3);
							group->setValue(L"2hcl");
							root->addChild(group);
							word->addChild(root);
							wordRule->addChild(word);
							molecule->addChild(wordRule);
							processComponents(molecule);
							assertEquals(2, root->getChildCount());
							Element *multiplier = root->getChild(0);
							assertEquals(XmlDeclarations::MULTIPLIER_EL, multiplier->getName());
							assertEquals(L"2", multiplier->getAttributeValue(XmlDeclarations::VALUE_ATR));
							assertEquals(L"2", multiplier->getValue());
							Element *updatedGroup = root->getChild(1);
							assertEquals(L"hcl", updatedGroup->getValue());
						}

						void ComponentGeneration_MiscTest::processComponents(Element *parse) throw(ComponentGenerationException)
						{
							ComponentGenerator tempVar(new NameToStructureConfig());
							(&tempVar)->processParse(parse);
						}
					}
				}
			}
		}
	}
}
