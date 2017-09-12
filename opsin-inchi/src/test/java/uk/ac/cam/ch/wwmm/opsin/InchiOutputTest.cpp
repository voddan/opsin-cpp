#include "InchiOutputTest.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/NameToInchi.h"
#include "../../../../../../../../../../opsin-core/src/main/java/uk/ac/cam/ch/wwmm/opsin/StructureBuildingException.h"
#include "../../../../../../../../../../opsin-core/src/main/java/uk/ac/cam/ch/wwmm/opsin/IDManager.h"
#include "../../../../../../../../../../opsin-core/src/main/java/uk/ac/cam/ch/wwmm/opsin/SMILESFragmentBuilder.h"
#include "../../../../../../../../../../opsin-core/src/main/java/uk/ac/cam/ch/wwmm/opsin/Fragment.h"
#include "../../../../../../../../../../opsin-core/src/main/java/uk/ac/cam/ch/wwmm/opsin/OpsinResult.h"

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
						using OPSIN_RESULT_STATUS = uk::ac::cam::ch::wwmm::opsin::OpsinResult::OPSIN_RESULT_STATUS;
NameToInchi *InchiOutputTest::n2i;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @BeforeClass public static void setUp()
						void InchiOutputTest::setUp()
						{
							n2i = new NameToInchi();
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @AfterClass public static void cleanUp()
						void InchiOutputTest::cleanUp()
						{
							n2i = nullptr;
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testStaticToInChI() throws StructureBuildingException
						void InchiOutputTest::testStaticToInChI() throw(StructureBuildingException)
						{
							IDManager tempVar();
							SMILESFragmentBuilder *sBuilder = new SMILESFragmentBuilder(&tempVar);
							Fragment *f = sBuilder->build(L"C([H])([H])([H])C(=O)N([H])[H]");
							OpsinResult *result = new OpsinResult(f, OPSIN_RESULT_STATUS::SUCCESS, L"", L"");
							assertEquals(L"InChI=1/C2H5NO/c1-2(3)4/h1H3,(H2,3,4)/f/h3H2", NameToInchi::convertResultToInChI(result));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testStaticToStdInChI() throws StructureBuildingException
						void InchiOutputTest::testStaticToStdInChI() throw(StructureBuildingException)
						{
							IDManager tempVar();
							SMILESFragmentBuilder *sBuilder = new SMILESFragmentBuilder(&tempVar);
							Fragment *f = sBuilder->build(L"C([H])([H])([H])C(=O)N([H])[H]");
							OpsinResult *result = new OpsinResult(f, OPSIN_RESULT_STATUS::SUCCESS, L"", L"");
							assertEquals(L"InChI=1S/C2H5NO/c1-2(3)4/h1H3,(H2,3,4)", NameToInchi::convertResultToStdInChI(result));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testStaticToStdInChIKey() throws StructureBuildingException
						void InchiOutputTest::testStaticToStdInChIKey() throw(StructureBuildingException)
						{
							IDManager tempVar();
							SMILESFragmentBuilder *sBuilder = new SMILESFragmentBuilder(&tempVar);
							Fragment *f = sBuilder->build(L"C([H])([H])([H])C(=O)N([H])[H]");
							OpsinResult *result = new OpsinResult(f, OPSIN_RESULT_STATUS::SUCCESS, L"", L"");
							assertEquals(L"DLFVBJFMPXGRIB-UHFFFAOYSA-N", NameToInchi::convertResultToStdInChIKey(result));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testParseToInChI()
						void InchiOutputTest::testParseToInChI()
						{
							assertEquals(L"InChI=1/C2H5NO/c1-2(3)4/h1H3,(H2,3,4)/f/h3H2", n2i->parseToInchi(L"acetamide"));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testParseToStdInChI()
						void InchiOutputTest::testParseToStdInChI()
						{
							assertEquals(L"InChI=1S/C2H5NO/c1-2(3)4/h1H3,(H2,3,4)", n2i->parseToStdInchi(L"acetamide"));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testParseToStdInChIKey()
						void InchiOutputTest::testParseToStdInChIKey()
						{
							assertEquals(L"DLFVBJFMPXGRIB-UHFFFAOYSA-N", n2i->parseToStdInchiKey(L"acetamide"));
						}
					}
				}
			}
		}
	}
}
