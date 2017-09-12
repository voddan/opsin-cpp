#include "SSSRTest.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/NameToStructure.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/Fragment.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/Ring.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/SSSRFinder.h"

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
						using org::junit::Test;
						using Fragment = uk::ac::cam::ch::wwmm::opsin::Fragment;
						using NameToStructure = uk::ac::cam::ch::wwmm::opsin::NameToStructure;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testFindSSSR() throws Exception
						void SSSRTest::testFindSSSR() throw(std::exception)
						{
							NameToStructure *n2s = NameToStructure::getInstance();
							Fragment *f = n2s->parseChemicalName(L"violanthrene")->getStructure();
							std::vector<Ring*> rings = SSSRFinder::getSetOfSmallestRings(f);
							assertEquals(9, rings.size());

							f = n2s->parseChemicalName(L"aceanthrene")->getStructure();
							rings = SSSRFinder::getSetOfSmallestRings(f);
							assertEquals(4, rings.size());
						}
					}
				}
			}
		}
	}
}
