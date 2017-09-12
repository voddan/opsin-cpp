#include "ParserTest.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/Parser.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/NameToStructureConfig.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/ParsingException.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/Element.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/XmlDeclarations.h"

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
//						import static org.junit.Assert.*;
						using org::junit::AfterClass;
						using org::junit::BeforeClass;
						using org::junit::Test;
Parser *ParserTest::parser;
NameToStructureConfig *ParserTest::config;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @BeforeClass public static void setUp() throws java.io.IOException
						void ParserTest::setUp() throw(IOException)
						{
							parser = new Parser();
							config = NameToStructureConfig::getDefaultConfigInstance();
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @AfterClass public static void cleanUp()
						void ParserTest::cleanUp()
						{
							parser = nullptr;
							config = nullptr;
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=ParsingException.class) public void testParseThrowsWhenNameIsUninterpretable() throws ParsingException
						void ParserTest::testParseThrowsWhenNameIsUninterpretable() throw(ParsingException)
						{
							parser->parse(config, L"chunky bacon");
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testParseUninvertsCASNomenclature() throws ParsingException
						void ParserTest::testParseUninvertsCASNomenclature() throw(ParsingException)
						{
							std::vector<Element*> parse = parser->parse(config, L"Piperidine, 1-(1-oxopropyl)-");

							assertFalse(parse.empty());
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testParseReturnsOneWordRuleForEachMixtureComponent() throws ParsingException
						void ParserTest::testParseReturnsOneWordRuleForEachMixtureComponent() throw(ParsingException)
						{
							std::vector<Element*> parse = parser->parse(config, L"benzene; ethane");

							assertEquals(2, parse[0]->getChildElements(XmlDeclarations::WORDRULE_EL)->size());
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=ParsingException.class) public void testParseThrowsWhenNameIsSubstituentOnly() throws ParsingException
						void ParserTest::testParseThrowsWhenNameIsSubstituentOnly() throw(ParsingException)
						{
							parser->parse(config, L"chloro");
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=ParsingException.class) public void testNoParseForOneComponentSalt() throws ParsingException
						void ParserTest::testNoParseForOneComponentSalt() throw(ParsingException)
						{
							parser->parse(config, L"pyridine salt");
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testConvertStringToComponentRatios1() throws ParsingException
						void ParserTest::testConvertStringToComponentRatios1() throw(ParsingException)
						{
							std::wstring ratio = L"(1:2)";
							std::vector<boost::optional<int>> componentRatios = Parser::processStoichiometryIndication(ratio);
							assertEquals(2, componentRatios.size());
							for (int i = 0; i < componentRatios.size(); i++)
							{
								if (i == 0)
								{
									assertEquals(1,static_cast<int>(componentRatios[i]));
								}
								if (i == 1)
								{
									assertEquals(2,static_cast<int>(componentRatios[i]));
								}
							}
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testConvertStringToComponentRatios2() throws ParsingException
						void ParserTest::testConvertStringToComponentRatios2() throw(ParsingException)
						{
							std::wstring ratio = L"[1/1/2]";
							std::vector<boost::optional<int>> componentRatios = Parser::processStoichiometryIndication(ratio);
							assertEquals(3, componentRatios.size());
							for (int i = 0; i < componentRatios.size(); i++)
							{
								if (i == 0)
								{
									assertEquals(1,static_cast<int>(componentRatios[i]));
								}
								if (i == 1)
								{
									assertEquals(1,static_cast<int>(componentRatios[i]));
								}
								if (i == 2)
								{
									assertEquals(2,static_cast<int>(componentRatios[i]));
								}
							}
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testConvertStringToComponentRatios3() throws ParsingException
						void ParserTest::testConvertStringToComponentRatios3() throw(ParsingException)
						{
							std::wstring ratio = L"(1:2:?)";
							std::vector<boost::optional<int>> componentRatios = Parser::processStoichiometryIndication(ratio);
							assertEquals(3, componentRatios.size());
							for (int i = 0; i < componentRatios.size(); i++)
							{
								if (i == 0)
								{
									assertEquals(1,static_cast<int>(componentRatios[i]));
								}
								if (i == 1)
								{
									assertEquals(2,static_cast<int>(componentRatios[i]));
								}
								if (i == 2)
								{
									assertEquals(1,static_cast<int>(componentRatios[i]));
								}
							}
						}
					}
				}
			}
		}
	}
}
