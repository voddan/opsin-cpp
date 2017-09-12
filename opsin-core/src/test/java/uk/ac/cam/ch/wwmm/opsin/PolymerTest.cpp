#include "PolymerTest.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/ParsingException.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/NameToStructure.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/OpsinResult.h"

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
//						import static org.junit.Assert.assertEquals;
//						import static org.junit.Assert.assertNotNull;
						using org::junit::Test;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSimplePolymer() throws ParsingException
						void PolymerTest::testSimplePolymer() throw(ParsingException)
						{
							OpsinResult *result = NameToStructure::getInstance()->parseChemicalName(L"poly(oxyethylene)");
							std::wstring smiles = result->getSmiles();
							assertNotNull(smiles);
							assertEquals(true, smiles.find(L"[*:1]") != std::wstring::npos);
							assertEquals(true, smiles.find(L"[*:2]") != std::wstring::npos);

							std::wstring cml = result->getCml();
							assertEquals(true, cml.find(L"alpha") != std::wstring::npos);
							assertEquals(true, cml.find(L"omega") != std::wstring::npos);
						}
					}
				}
			}
		}
	}
}
