#pragma once

#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/IDManager.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class Fragment; } } } } } }
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
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static org.mockito.Mockito.mock;



						class AtomTest
						{

						private:
							Fragment *frag;
							SMILESFragmentBuilder *sBuilder = new SMILESFragmentBuilder(new IDManager());

						public:
							virtual ~AtomTest()
							{
								delete frag;
								delete sBuilder;
							}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void setUp()
							virtual void setUp();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAtom()
							virtual void testAtom();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAddLocantHasLocant()
							virtual void testAddLocantHasLocant();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testGetIncomingValency() throws StructureBuildingException
							virtual void testGetIncomingValency() throw(StructureBuildingException);

						};

					}
				}
			}
		}
	}
}
