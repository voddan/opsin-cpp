#pragma once

#include <vector>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class FragmentManager; } } } } } }
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



						class FragmentManagerTest
						{

						public:
							FragmentManager *fragManager;

							virtual ~FragmentManagerTest()
							{
								delete fragManager;
							}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void setUp() throws java.io.IOException
							virtual void setUp() throw(IOException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testGetUnifiedFrags() throws StructureBuildingException
							virtual void testGetUnifiedFrags() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRelabelFusedRingSystem() throws StructureBuildingException
							virtual void testRelabelFusedRingSystem() throw(StructureBuildingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCloneFragment() throws StructureBuildingException
							virtual void testCloneFragment() throw(StructureBuildingException);
						};

					}
				}
			}
		}
	}
}
