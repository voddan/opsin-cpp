#pragma once

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class ComponentGenerationException; } } } } } }
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class StructureBuildingException; } } } } } }

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

						using namespace uk::ac::cam::ch::wwmm::opsin;
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;


//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static org.junit.Assert.*;
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static org.mockito.Mockito.mock;

						class ComponentProcessorTest
						{

						public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=ComponentGenerationException.class) public void testSubtractiveWithNoGroupToAttachTo() throws ComponentGenerationException
							virtual void testSubtractiveWithNoGroupToAttachTo() throw(ComponentGenerationException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSubtractiveWithBiochemicalToAttachTo() throws ComponentGenerationException
							virtual void testSubtractiveWithBiochemicalToAttachTo() throw(ComponentGenerationException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSubtractiveRightMostPreferred() throws ComponentGenerationException
							virtual void testSubtractiveRightMostPreferred() throw(ComponentGenerationException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSubtractiveBiochemicalPreferredToRightMost() throws ComponentGenerationException
							virtual void testSubtractiveBiochemicalPreferredToRightMost() throw(ComponentGenerationException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSubtractiveWithMultiplierAndLocants() throws ComponentGenerationException
							virtual void testSubtractiveWithMultiplierAndLocants() throw(ComponentGenerationException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDLStereochemistryLOnAminoAcid() throws ComponentGenerationException, StructureBuildingException
							virtual void testDLStereochemistryLOnAminoAcid() throw(ComponentGenerationException, StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDLStereochemistryDOnAminoAcid() throws ComponentGenerationException, StructureBuildingException
							virtual void testDLStereochemistryDOnAminoAcid() throw(ComponentGenerationException, StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDLStereochemistryDLOnAminoAcid() throws ComponentGenerationException, StructureBuildingException
							virtual void testDLStereochemistryDLOnAminoAcid() throw(ComponentGenerationException, StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDLStereochemistryDOnAchiralAminoAcid() throws ComponentGenerationException, StructureBuildingException
							virtual void testDLStereochemistryDOnAchiralAminoAcid() throw(ComponentGenerationException, StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDLStereochemistryLOnCarbohydrate() throws ComponentGenerationException, StructureBuildingException
							virtual void testDLStereochemistryLOnCarbohydrate() throw(ComponentGenerationException, StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDLStereochemistryDOnCarbohydrate() throws ComponentGenerationException, StructureBuildingException
							virtual void testDLStereochemistryDOnCarbohydrate() throw(ComponentGenerationException, StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDLStereochemistryInvertedNaturalOnCarbohydrate1() throws ComponentGenerationException, StructureBuildingException
							virtual void testDLStereochemistryInvertedNaturalOnCarbohydrate1() throw(ComponentGenerationException, StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDLStereochemistryInvertedNaturalOnCarbohydrate2() throws ComponentGenerationException, StructureBuildingException
							virtual void testDLStereochemistryInvertedNaturalOnCarbohydrate2() throw(ComponentGenerationException, StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDStereochemistryDOnCarbohydratePrefix() throws ComponentGenerationException, StructureBuildingException
							virtual void testDStereochemistryDOnCarbohydratePrefix() throw(ComponentGenerationException, StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testLStereochemistryDOnCarbohydratePrefix() throws ComponentGenerationException, StructureBuildingException
							virtual void testLStereochemistryDOnCarbohydratePrefix() throw(ComponentGenerationException, StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDLStereochemistryDOnCarbohydratePrefix() throws ComponentGenerationException, StructureBuildingException
							virtual void testDLStereochemistryDOnCarbohydratePrefix() throw(ComponentGenerationException, StructureBuildingException);

						};

					}
				}
			}
		}
	}
}
