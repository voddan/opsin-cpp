#pragma once

#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/IDManager.h"
#include <vector>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class SMILESFragmentBuilder; } } } } } }
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

//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static org.junit.Assert.*;




						//Cycle detection is performed as part of fragment creation so we can just check the output of fragment creation
						class CycleDetectorTest
						{
						private:
							SMILESFragmentBuilder *sBuilder = new SMILESFragmentBuilder(new IDManager());

						public:
							virtual ~CycleDetectorTest()
							{
								delete sBuilder;
							}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAssignCyclic1() throws StructureBuildingException
							virtual void testAssignCyclic1() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAssignCyclic2() throws StructureBuildingException
							virtual void testAssignCyclic2() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAssignCyclic3() throws StructureBuildingException
							virtual void testAssignCyclic3() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAssignCyclic4() throws StructureBuildingException
							virtual void testAssignCyclic4() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAssignCyclic5() throws StructureBuildingException
							virtual void testAssignCyclic5() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAssignCyclic6() throws StructureBuildingException
							virtual void testAssignCyclic6() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testFindPathBetweenAtoms1() throws StructureBuildingException
							virtual void testFindPathBetweenAtoms1() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testFindPathBetweenAtoms2() throws StructureBuildingException
							virtual void testFindPathBetweenAtoms2() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testFindPathBetweenAtoms3() throws StructureBuildingException
							virtual void testFindPathBetweenAtoms3() throw(StructureBuildingException);
						};


					}
				}
			}
		}
	}
}
