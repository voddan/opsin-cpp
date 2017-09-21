#include "SSSRTest.h"
#include "opsin-core/NameToStructure.h"
#include "opsin-core/Fragment.h"
#include "opsin-core/Ring.h"
#include "opsin-core/SSSRFinder.h"


//						import static org.junit.Assert.*;
using org::junit::Test;
using Fragment = Fragment;
using NameToStructure = NameToStructure;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testFindSSSR() throws Exception
void SSSRTest::testFindSSSR() throw(std::exception) {
    NameToStructure *n2s = NameToStructure::getInstance();
    Fragment *f = n2s->parseChemicalName(L"violanthrene")->getStructure();
    std::vector<Ring *> rings = SSSRFinder::getSetOfSmallestRings(f);
    assertEquals(9, rings.size());

    f = n2s->parseChemicalName(L"aceanthrene")->getStructure();
    rings = SSSRFinder::getSetOfSmallestRings(f);
    assertEquals(4, rings.size());
}
