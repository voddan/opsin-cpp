#pragma once

#include <string>
#include <vector>
#include "stringbuilder.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class FragmentManager; } } } } } }
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class NameToStructure; } } } } } }
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class CipOrderingException; } } } } } }
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class StructureBuildingException; } } } } } }
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class Atom; } } } } } }

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
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static org.mockito.Mockito.mock;




						class StereochemistryTest
						{

						private:
							FragmentManager *fm;

						public:
							virtual ~StereochemistryTest()
							{
								delete fm;
							}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void setup()
							virtual void setup();

						private:
							static NameToStructure *n2s;

						public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @BeforeClass public static void intialSetup()
							static void intialSetup();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @AfterClass public static void cleanUp()
							static void cleanUp();

							/*
							 * Tests for finding stereo centres
							 */
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void findStereoCentresBromoChloroFluoroMethane()
							virtual void findStereoCentresBromoChloroFluoroMethane();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void findStereoCentresNacetylleucine() throws CipOrderingException
							virtual void findStereoCentresNacetylleucine() throw(CipOrderingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void findStereoCentresBut2ene()
							virtual void findStereoCentresBut2ene();

							/*
							 * Tests for applying stereochemistry
							 */

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void applyStereochemistryLocantedZ() throws StructureBuildingException
							virtual void applyStereochemistryLocantedZ() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void applyStereochemistryLocantedE() throws StructureBuildingException
							virtual void applyStereochemistryLocantedE() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void applyStereochemistryUnlocantedZ() throws StructureBuildingException
							virtual void applyStereochemistryUnlocantedZ() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void applyStereochemistryUnlocantedE() throws StructureBuildingException
							virtual void applyStereochemistryUnlocantedE() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void applyStereochemistryCis() throws StructureBuildingException
							virtual void applyStereochemistryCis() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void applyStereochemistryTrans() throws StructureBuildingException
							virtual void applyStereochemistryTrans() throw(StructureBuildingException);


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void applyStereochemistryLocantedRS() throws StructureBuildingException
							virtual void applyStereochemistryLocantedRS() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCIPpriority1() throws StructureBuildingException
							virtual void testCIPpriority1() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCIPpriority2() throws StructureBuildingException
							virtual void testCIPpriority2() throw(StructureBuildingException);


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCIPpriority3() throws StructureBuildingException
							virtual void testCIPpriority3() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCIPpriority4() throws StructureBuildingException
							virtual void testCIPpriority4() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCIPpriority5() throws StructureBuildingException
							virtual void testCIPpriority5() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCIPpriority6() throws StructureBuildingException
							virtual void testCIPpriority6() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCIPpriority7() throws StructureBuildingException
							virtual void testCIPpriority7() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCIPpriority8() throws StructureBuildingException
							virtual void testCIPpriority8() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCIPpriority9() throws StructureBuildingException
							virtual void testCIPpriority9() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCIPpriority10() throws StructureBuildingException
							virtual void testCIPpriority10() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCIPpriority11() throws StructureBuildingException
							virtual void testCIPpriority11() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCIPpriority12() throws StructureBuildingException
							virtual void testCIPpriority12() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCIPpriority13() throws StructureBuildingException
							virtual void testCIPpriority13() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCIPpriority14() throws StructureBuildingException
							virtual void testCIPpriority14() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCIPpriority15() throws StructureBuildingException
							virtual void testCIPpriority15() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=CipOrderingException.class) public void testCipUnassignable() throws StructureBuildingException
							virtual void testCipUnassignable() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAtomParityEquivalence1()
							virtual void testAtomParityEquivalence1();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAtomParityEquivalence2()
							virtual void testAtomParityEquivalence2();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCisTransUnambiguous() throws StructureBuildingException
							virtual void testCisTransUnambiguous() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCisTransAmbiguous() throws StructureBuildingException
							virtual void testCisTransAmbiguous() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testChiralAtomWhichBecomesAchiral() throws StructureBuildingException
							virtual void testChiralAtomWhichBecomesAchiral() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testChiralBondWhichBecomesAchiral() throws StructureBuildingException
							virtual void testChiralBondWhichBecomesAchiral() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testIsTetrahedral() throws StructureBuildingException
							virtual void testIsTetrahedral() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAchiralDueToResonance() throws StructureBuildingException
							virtual void testAchiralDueToResonance() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAchiralDueToTautomerism() throws StructureBuildingException
							virtual void testAchiralDueToTautomerism() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testFindPseudoAsymmetricCarbon1() throws StructureBuildingException
							virtual void testFindPseudoAsymmetricCarbon1() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testFindPseudoAsymmetricCarbon2() throws StructureBuildingException
							virtual void testFindPseudoAsymmetricCarbon2() throw(StructureBuildingException);

						private:
							std::wstring atomRefsToIdStr(std::vector<Atom*> &atomRefs4);
						};

					}
				}
			}
		}
	}
}
