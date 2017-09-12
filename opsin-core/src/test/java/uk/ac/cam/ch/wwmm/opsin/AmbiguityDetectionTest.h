#pragma once

#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class NameToStructure; } } } } } }

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
//						import static org.junit.Assert.assertEquals;



						class AmbiguityDetectionTest
						{

						private:
							static NameToStructure *n2s;

						public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @BeforeClass public static void setUp()
							static void setUp();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @AfterClass public static void cleanUp()
							static void cleanUp();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testNamesThatShouldBeDetectedAsAmbiguous() throws java.io.IOException
							virtual void testNamesThatShouldBeDetectedAsAmbiguous() throw(IOException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testUnAmbiguousCounterExamples() throws java.io.IOException
							virtual void testUnAmbiguousCounterExamples() throw(IOException);

						private:
							void checkNames(const std::wstring &file, bool isAmbiguous) throw(IOException);

						};

					}
				}
			}
		}
	}
}
