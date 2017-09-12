#pragma once

#include <string>
#include <vector>
#include <boost/optional.hpp>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class Parser; } } } } } }
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class NameToStructureConfig; } } } } } }
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class ParsingException; } } } } } }

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



						class ParserTest
						{
						private:
							static Parser *parser;
							static NameToStructureConfig *config;

						public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @BeforeClass public static void setUp() throws java.io.IOException
							static void setUp() throw(IOException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @AfterClass public static void cleanUp()
							static void cleanUp();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=ParsingException.class) public void testParseThrowsWhenNameIsUninterpretable() throws ParsingException
							virtual void testParseThrowsWhenNameIsUninterpretable() throw(ParsingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testParseUninvertsCASNomenclature() throws ParsingException
							virtual void testParseUninvertsCASNomenclature() throw(ParsingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testParseReturnsOneWordRuleForEachMixtureComponent() throws ParsingException
							virtual void testParseReturnsOneWordRuleForEachMixtureComponent() throw(ParsingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=ParsingException.class) public void testParseThrowsWhenNameIsSubstituentOnly() throws ParsingException
							virtual void testParseThrowsWhenNameIsSubstituentOnly() throw(ParsingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=ParsingException.class) public void testNoParseForOneComponentSalt() throws ParsingException
							virtual void testNoParseForOneComponentSalt() throw(ParsingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testConvertStringToComponentRatios1() throws ParsingException
							virtual void testConvertStringToComponentRatios1() throw(ParsingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testConvertStringToComponentRatios2() throws ParsingException
							virtual void testConvertStringToComponentRatios2() throw(ParsingException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testConvertStringToComponentRatios3() throws ParsingException
							virtual void testConvertStringToComponentRatios3() throw(ParsingException);
						};

					}
				}
			}
		}
	}
}
