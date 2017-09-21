#include "HeteroAtomReplacementTest.h"
#include "opsin-core/FragmentManager.h"
#include "opsin-core/Atom.h"
#include "opsin-core/IDManager.h"
#include "opsin-core/SMILESFragmentBuilder.h"
#include "opsin-core/ChemEl.h"
#include "opsin-core/Fragment.h"
#include "opsin-core/StructureBuildingException.h"
#include "opsin-core/StructureBuildingMethods.h"


//						import static org.junit.Assert.*;
//						import static org.mockito.Mockito.mock;
using org::junit::Before;
using org::junit::Test;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void setUp()
void HeteroAtomReplacementTest::setUp() {
    IDManager *idManager = new IDManager();
    SMILESFragmentBuilder tempVar(idManager);
    fragManager = new FragmentManager(&tempVar, idManager);
    a = new Atom(0, ChemEl::C, mock(Fragment::
    typeid));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void thia() throws StructureBuildingException
void HeteroAtomReplacementTest::thia() throw(StructureBuildingException) {
    fragManager->replaceAtomWithSmiles(a, L"S");
    assertEquals(0, a->getCharge());
    assertEquals(0, a->getProtonsExplicitlyAddedOrRemoved());
    assertEquals(2, StructureBuildingMethods::calculateSubstitutableHydrogenAtoms(a));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void thionia() throws StructureBuildingException
void HeteroAtomReplacementTest::thionia() throw(StructureBuildingException) {
    fragManager->replaceAtomWithSmiles(a, L"[SH3+]");
    assertEquals(1, a->getCharge());
    assertEquals(1, a->getProtonsExplicitlyAddedOrRemoved());
    assertEquals(3, StructureBuildingMethods::calculateSubstitutableHydrogenAtoms(a));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void sulfanylia() throws StructureBuildingException
void HeteroAtomReplacementTest::sulfanylia() throw(StructureBuildingException) {
    fragManager->replaceAtomWithSmiles(a, L"[SH+]");
    assertEquals(1, a->getCharge());
    assertEquals(-1, a->getProtonsExplicitlyAddedOrRemoved());
    assertEquals(1, StructureBuildingMethods::calculateSubstitutableHydrogenAtoms(a));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void sulfanida() throws StructureBuildingException
void HeteroAtomReplacementTest::sulfanida() throw(StructureBuildingException) {
    fragManager->replaceAtomWithSmiles(a, L"[SH-]");
    assertEquals(-1, a->getCharge());
    assertEquals(-1, a->getProtonsExplicitlyAddedOrRemoved());
    assertEquals(1, StructureBuildingMethods::calculateSubstitutableHydrogenAtoms(a));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void sulfanuida() throws StructureBuildingException
void HeteroAtomReplacementTest::sulfanuida() throw(StructureBuildingException) {
    fragManager->replaceAtomWithSmiles(a, L"[SH3-]");
    assertEquals(-1, a->getCharge());
    assertEquals(1, a->getProtonsExplicitlyAddedOrRemoved());
    assertEquals(3, StructureBuildingMethods::calculateSubstitutableHydrogenAtoms(a));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void replaceNeutralWithCharged() throws StructureBuildingException
void HeteroAtomReplacementTest::replaceNeutralWithCharged() throw(StructureBuildingException) {
    Atom * a = new Atom(0, ChemEl::C, mock(Fragment::
    typeid));
    fragManager->replaceAtomWithSmiles(a, L"[NH4+]");
    assertEquals(1, a->getCharge());
    assertEquals(1, a->getProtonsExplicitlyAddedOrRemoved());
    assertEquals(4, StructureBuildingMethods::calculateSubstitutableHydrogenAtoms(a));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void replaceChargedWithEquallyCharged() throws StructureBuildingException
void HeteroAtomReplacementTest::replaceChargedWithEquallyCharged() throw(StructureBuildingException) {
    Atom * a = new Atom(0, ChemEl::C, mock(Fragment::
    typeid));
    a->addChargeAndProtons(1, -1);
    fragManager->replaceAtomWithSmiles(a, L"[NH4+]");
    assertEquals(1, a->getCharge());
    assertEquals(1, a->getProtonsExplicitlyAddedOrRemoved());
    assertEquals(4, StructureBuildingMethods::calculateSubstitutableHydrogenAtoms(a));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=StructureBuildingException.class) public void replaceChargedWithUnEquallyCharged() throws StructureBuildingException
void HeteroAtomReplacementTest::replaceChargedWithUnEquallyCharged() throw(StructureBuildingException) {
    Atom * a = new Atom(0, ChemEl::C, mock(Fragment::
    typeid));
    a->addChargeAndProtons(1, -1);
    fragManager->replaceAtomWithSmiles(a, L"[NH2-]");
}
