#include "AtomTest.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/Fragment.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/SMILESFragmentBuilder.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/Element.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/Atom.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/ChemEl.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/StructureBuildingException.h"


//						import static org.junit.Assert.*;
//						import static org.mockito.Mockito.mock;
using org::junit::Before;
using org::junit::Test;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void setUp()
void AtomTest::setUp() {
    frag = new Fragment(mock(Element::
    typeid));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAtom()
void AtomTest::testAtom() {
    Atom *atom = new Atom(10, ChemEl::C, frag);
    assertNotNull(L"Got atom", atom);
    assertEquals(L"Id = 10", 10, atom->getID());
    assertEquals(L"Element = C", ChemEl::C, atom->getElement());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAddLocantHasLocant()
void AtomTest::testAddLocantHasLocant() {
    Atom *atom = new Atom(10, ChemEl::C, frag);
    atom->addLocant(L"1");
    assertTrue(L"Atom has locant '1'", atom->hasLocant(L"1"));
    assertFalse(L"Atom has no locant 'C'", atom->hasLocant(L"C"));
    atom->addLocant(L"C");
    assertTrue(L"Atom now has locant 'C'", atom->hasLocant(L"C"));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testGetIncomingValency() throws StructureBuildingException
void AtomTest::testGetIncomingValency() throw(StructureBuildingException) {
    assertEquals(L"No bonds", 0, sBuilder->build(L"C")->getFirstAtom()->getIncomingValency());
    assertEquals(L"One bond", 1, sBuilder->build(L"CC")->getFirstAtom()->getIncomingValency());
    assertEquals(L"Two bonds", 2, sBuilder->build(L"C(C)C")->getFirstAtom()->getIncomingValency());
    assertEquals(L"Double bond", 2, sBuilder->build(L"C=O")->getFirstAtom()->getIncomingValency());
    assertEquals(L"Triple bond", 3, sBuilder->build(L"C#C")->getFirstAtom()->getIncomingValency());
    assertEquals(L"One bond", 1, sBuilder->build(L"CC=CC#N")->getFirstAtom()->getIncomingValency());
}
