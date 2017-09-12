#pragma once

#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/IDManager.h"
#include <string>
#include <vector>
#include "stringbuilder.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class SMILESFragmentBuilder; } } } } } }
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




						class SMILESFragmentBuilderTest
						{

						private:
							SMILESFragmentBuilder *sBuilder = new SMILESFragmentBuilder(new IDManager());

						public:
							virtual ~SMILESFragmentBuilderTest()
							{
								delete sBuilder;
							}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testBuild() throws StructureBuildingException
							virtual void testBuild() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSimple1() throws StructureBuildingException
							virtual void testSimple1() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSimple2() throws StructureBuildingException
							virtual void testSimple2() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSimple3() throws StructureBuildingException
							virtual void testSimple3() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSimple4() throws StructureBuildingException
							virtual void testSimple4() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSimple5() throws StructureBuildingException
							virtual void testSimple5() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSimple6() throws StructureBuildingException
							virtual void testSimple6() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSimple7() throws StructureBuildingException
							virtual void testSimple7() throw(StructureBuildingException);


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSimple8() throws StructureBuildingException
							virtual void testSimple8() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSimple9() throws StructureBuildingException
							virtual void testSimple9() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSimple10() throws StructureBuildingException
							virtual void testSimple10() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSimple11() throws StructureBuildingException
							virtual void testSimple11() throw(StructureBuildingException);


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=StructureBuildingException.class) public void unterminatedRingOpening() throws StructureBuildingException
							virtual void unterminatedRingOpening() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void doublePositiveCharge1() throws StructureBuildingException
							virtual void doublePositiveCharge1() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void doublePositiveCharge2() throws StructureBuildingException
							virtual void doublePositiveCharge2() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void doubleNegativeCharge1() throws StructureBuildingException
							virtual void doubleNegativeCharge1() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void doubleNegativeCharge2() throws StructureBuildingException
							virtual void doubleNegativeCharge2() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void noIsotopeSpecified() throws StructureBuildingException
							virtual void noIsotopeSpecified() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void isotopeSpecified() throws StructureBuildingException
							virtual void isotopeSpecified() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=StructureBuildingException.class) public void badlyFormedSMILE1() throws StructureBuildingException
							virtual void badlyFormedSMILE1() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=StructureBuildingException.class) public void badlyFormedSMILE2() throws StructureBuildingException
							virtual void badlyFormedSMILE2() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=StructureBuildingException.class) public void badlyFormedSMILE3() throws StructureBuildingException
							virtual void badlyFormedSMILE3() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=StructureBuildingException.class) public void badlyFormedSMILE4() throws StructureBuildingException
							virtual void badlyFormedSMILE4() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=StructureBuildingException.class) public void badlyFormedSMILE5() throws StructureBuildingException
							virtual void badlyFormedSMILE5() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=StructureBuildingException.class) public void badlyFormedSMILE6() throws StructureBuildingException
							virtual void badlyFormedSMILE6() throw(StructureBuildingException);


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=StructureBuildingException.class) public void badlyFormedSMILE7() throws StructureBuildingException
							virtual void badlyFormedSMILE7() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=StructureBuildingException.class) public void badlyFormedSMILE8() throws StructureBuildingException
							virtual void badlyFormedSMILE8() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void ringClosureHandling1() throws StructureBuildingException
							virtual void ringClosureHandling1() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void ringClosureHandling2() throws StructureBuildingException
							virtual void ringClosureHandling2() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=StructureBuildingException.class) public void ringClosureHandling3() throws StructureBuildingException
							virtual void ringClosureHandling3() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void ringClosureHandling4() throws StructureBuildingException
							virtual void ringClosureHandling4() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void ringSupportGreaterThan10() throws StructureBuildingException
							virtual void ringSupportGreaterThan10() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hydrogenHandling1() throws StructureBuildingException
							virtual void hydrogenHandling1() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hydrogenHandling2() throws StructureBuildingException
							virtual void hydrogenHandling2() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hydrogenHandling3() throws StructureBuildingException
							virtual void hydrogenHandling3() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hydrogenHandling4() throws StructureBuildingException
							virtual void hydrogenHandling4() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hydrogenHandling5() throws StructureBuildingException
							virtual void hydrogenHandling5() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hydrogenHandling6() throws StructureBuildingException
							virtual void hydrogenHandling6() throw(StructureBuildingException);


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hydrogenHandling7() throws StructureBuildingException
							virtual void hydrogenHandling7() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hydrogenHandling8() throws StructureBuildingException
							virtual void hydrogenHandling8() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hydrogenHandling9() throws StructureBuildingException
							virtual void hydrogenHandling9() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hydrogenHandling10() throws StructureBuildingException
							virtual void hydrogenHandling10() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hydrogenHandling11() throws StructureBuildingException
							virtual void hydrogenHandling11() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hydrogenHandling12() throws StructureBuildingException
							virtual void hydrogenHandling12() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hydrogenHandling13() throws StructureBuildingException
							virtual void hydrogenHandling13() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hydrogenHandling14() throws StructureBuildingException
							virtual void hydrogenHandling14() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hydrogenHandling15() throws StructureBuildingException
							virtual void hydrogenHandling15() throw(StructureBuildingException);


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hydrogenHandling16() throws StructureBuildingException
							virtual void hydrogenHandling16() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hydrogenHandling17() throws StructureBuildingException
							virtual void hydrogenHandling17() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hydrogenHandling18() throws StructureBuildingException
							virtual void hydrogenHandling18() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hydrogenHandling19() throws StructureBuildingException
							virtual void hydrogenHandling19() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hydrogenHandling20() throws StructureBuildingException
							virtual void hydrogenHandling20() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hydrogenHandling21() throws StructureBuildingException
							virtual void hydrogenHandling21() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void indicatedHydrogen() throws StructureBuildingException
							virtual void indicatedHydrogen() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void chiralityTest1() throws StructureBuildingException
							virtual void chiralityTest1() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void chiralityTest2() throws StructureBuildingException
							virtual void chiralityTest2() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void chiralityTest3() throws StructureBuildingException
							virtual void chiralityTest3() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void chiralityTest4() throws StructureBuildingException
							virtual void chiralityTest4() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void chiralityTest5() throws StructureBuildingException
							virtual void chiralityTest5() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void chiralityTest6() throws StructureBuildingException
							virtual void chiralityTest6() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void chiralityTest7() throws StructureBuildingException
							virtual void chiralityTest7() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void chiralityTest8() throws StructureBuildingException
							virtual void chiralityTest8() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondStereo1() throws StructureBuildingException
							virtual void testDoubleBondStereo1() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondStereo2() throws StructureBuildingException
							virtual void testDoubleBondStereo2() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondStereo3() throws StructureBuildingException
							virtual void testDoubleBondStereo3() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondStereo4() throws StructureBuildingException
							virtual void testDoubleBondStereo4() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondStereo5a() throws StructureBuildingException
							virtual void testDoubleBondStereo5a() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondStereo5b() throws StructureBuildingException
							virtual void testDoubleBondStereo5b() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondStereo6() throws StructureBuildingException
							virtual void testDoubleBondStereo6() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondMultiStereo1() throws StructureBuildingException
							virtual void testDoubleBondMultiStereo1() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondMultiStereo2() throws StructureBuildingException
							virtual void testDoubleBondMultiStereo2() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondMultiStereo3() throws StructureBuildingException
							virtual void testDoubleBondMultiStereo3() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondMultiStereo4() throws StructureBuildingException
							virtual void testDoubleBondMultiStereo4() throw(StructureBuildingException);

							//From http://baoilleach.blogspot.com/2010/09/are-you-on-my-side-or-not-its-ez-part.html
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondNoela() throws StructureBuildingException
							virtual void testDoubleBondNoela() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondNoelb() throws StructureBuildingException
							virtual void testDoubleBondNoelb() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondNoelc() throws StructureBuildingException
							virtual void testDoubleBondNoelc() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondNoeld() throws StructureBuildingException
							virtual void testDoubleBondNoeld() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=StructureBuildingException.class) public void testDoubleBondNoele() throws StructureBuildingException
							virtual void testDoubleBondNoele() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=StructureBuildingException.class) public void testDoubleBondNoelf() throws StructureBuildingException
							virtual void testDoubleBondNoelf() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=StructureBuildingException.class) public void testDoubleBondNoelg() throws StructureBuildingException
							virtual void testDoubleBondNoelg() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondCornerCase1() throws StructureBuildingException
							virtual void testDoubleBondCornerCase1() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondCornerCase2() throws StructureBuildingException
							virtual void testDoubleBondCornerCase2() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=StructureBuildingException.class) public void testDoubleBondCornerCase3() throws StructureBuildingException
							virtual void testDoubleBondCornerCase3() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=StructureBuildingException.class) public void testDoubleBondCornerCase4() throws StructureBuildingException
							virtual void testDoubleBondCornerCase4() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondCornerCase5() throws StructureBuildingException
							virtual void testDoubleBondCornerCase5() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondCornerCase6() throws StructureBuildingException
							virtual void testDoubleBondCornerCase6() throw(StructureBuildingException);

						private:
							std::wstring atomRefsToIdStr(std::vector<Atom*> &atomRefs4);
						};

					}
				}
			}
		}
	}
}
