#pragma once

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class Element; } } } } } }
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class ComponentGenerationException; } } } } } }

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

//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static org.junit.Assert.*;
						using namespace uk::ac::cam::ch::wwmm::opsin;
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;


						class ComponentGeneration_ProcesslocantsTest
						{

						private:
							Element *locant;
							Element *substituent;

						public:
							virtual ~ComponentGeneration_ProcesslocantsTest()
							{
								delete locant;
								delete substituent;
							}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void setUpSubstituent()
							virtual void setUpSubstituent();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCardinalNumber() throws ComponentGenerationException
							virtual void testCardinalNumber() throw(ComponentGenerationException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCardinalNumberWithHyphen() throws ComponentGenerationException
							virtual void testCardinalNumberWithHyphen() throw(ComponentGenerationException);


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testElementSymbol() throws ComponentGenerationException
							virtual void testElementSymbol() throw(ComponentGenerationException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAminoAcidStyleLocant() throws ComponentGenerationException
							virtual void testAminoAcidStyleLocant() throw(ComponentGenerationException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCompoundLocant() throws ComponentGenerationException
							virtual void testCompoundLocant() throw(ComponentGenerationException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testGreek() throws ComponentGenerationException
							virtual void testGreek() throw(ComponentGenerationException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testNotlowercase1() throws ComponentGenerationException
							virtual void testNotlowercase1() throw(ComponentGenerationException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testNotlowercase2() throws ComponentGenerationException
							virtual void testNotlowercase2() throw(ComponentGenerationException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testIUPAC2004() throws ComponentGenerationException
							virtual void testIUPAC2004() throw(ComponentGenerationException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSuperscript1() throws ComponentGenerationException
							virtual void testSuperscript1() throw(ComponentGenerationException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSuperscript2() throws ComponentGenerationException
							virtual void testSuperscript2() throw(ComponentGenerationException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSuperscript3() throws ComponentGenerationException
							virtual void testSuperscript3() throw(ComponentGenerationException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSuperscript4() throws ComponentGenerationException
							virtual void testSuperscript4() throw(ComponentGenerationException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSuperscript5() throws ComponentGenerationException
							virtual void testSuperscript5() throw(ComponentGenerationException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSuperscript6() throws ComponentGenerationException
							virtual void testSuperscript6() throw(ComponentGenerationException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSuperscript7() throws ComponentGenerationException
							virtual void testSuperscript7() throw(ComponentGenerationException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAddedHydrogen() throws ComponentGenerationException
							virtual void testAddedHydrogen() throw(ComponentGenerationException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAddedHydrogen2() throws ComponentGenerationException
							virtual void testAddedHydrogen2() throw(ComponentGenerationException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testStereochemistryInLocant1() throws ComponentGenerationException
							virtual void testStereochemistryInLocant1() throw(ComponentGenerationException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testStereochemistryInLocant2() throws ComponentGenerationException
							virtual void testStereochemistryInLocant2() throw(ComponentGenerationException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testStereochemistryInLocant3() throws ComponentGenerationException
							virtual void testStereochemistryInLocant3() throw(ComponentGenerationException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testStereochemistryInLocant4() throws ComponentGenerationException
							virtual void testStereochemistryInLocant4() throw(ComponentGenerationException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testStereochemistryInLocant5() throws ComponentGenerationException
							virtual void testStereochemistryInLocant5() throw(ComponentGenerationException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testStereochemistryInLocant6() throws ComponentGenerationException
							virtual void testStereochemistryInLocant6() throw(ComponentGenerationException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testMultipleCardinals() throws ComponentGenerationException
							virtual void testMultipleCardinals() throw(ComponentGenerationException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testMultipleTypesTogether() throws ComponentGenerationException
							virtual void testMultipleTypesTogether() throw(ComponentGenerationException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCarbohydrateStyleLocants() throws ComponentGenerationException
							virtual void testCarbohydrateStyleLocants() throw(ComponentGenerationException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCarbohydrateStyleLocantsNoNumericComponent() throws ComponentGenerationException
							virtual void testCarbohydrateStyleLocantsNoNumericComponent() throw(ComponentGenerationException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCarbohydrateStyleLocantsCounterExample() throws ComponentGenerationException
							virtual void testCarbohydrateStyleLocantsCounterExample() throw(ComponentGenerationException);
						};

					}
				}
			}
		}
	}
}
