#include "SMILESFragmentBuilderTest.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/SMILESFragmentBuilder.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/StructureBuildingException.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/Fragment.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/Atom.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/ChemEl.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/Bond.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/AtomParity.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/BondStereo.h"


//						import static org.junit.Assert.*;
using org::junit::Test;
using BondStereoValue = BondStereo::BondStereoValue;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testBuild() throws StructureBuildingException
void SMILESFragmentBuilderTest::testBuild() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"C");
    assertNotNull(L"Got a fragment", fragment);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSimple1() throws StructureBuildingException
void SMILESFragmentBuilderTest::testSimple1() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"CC");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(2, atomList.size());
    assertEquals(ChemEl::C, atomList[0]->getElement());
    assertEquals(ChemEl::C, atomList[1]->getElement());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSimple2() throws StructureBuildingException
void SMILESFragmentBuilderTest::testSimple2() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"O=C=O");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(3, atomList.size());
    assertEquals(ChemEl::O, atomList[0]->getElement());
    assertEquals(ChemEl::C, atomList[1]->getElement());
    assertEquals(ChemEl::O, atomList[2]->getElement());
    Set < Bond * > *bonds = fragment->getBondSet();
    assertEquals(2, bonds->size());
    for (auto bond : bonds) {
        assertEquals(2, bond->getOrder());
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSimple3() throws StructureBuildingException
void SMILESFragmentBuilderTest::testSimple3() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"C#N");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(2, atomList.size());
    Set < Bond * > *bonds = fragment->getBondSet();
    assertEquals(1, bonds->size());
    for (auto bond : bonds) {
        assertEquals(3, bond->getOrder());
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSimple4() throws StructureBuildingException
void SMILESFragmentBuilderTest::testSimple4() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"CCN(CC)CC");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(7, atomList.size());
    Atom *nitrogen = atomList[2];
    assertEquals(ChemEl::N, nitrogen->getElement());
    assertEquals(3, nitrogen->getBondCount());
    std::vector<Atom *> neighbours = nitrogen->getAtomNeighbours(); //bonds and hence neighbours come from a linked hash set so the order of the neighbours is deterministic
    assertEquals(3, neighbours.size());
    assertEquals(atomList[1], neighbours[0]);
    assertEquals(atomList[3], neighbours[1]);
    assertEquals(atomList[5], neighbours[2]);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSimple5() throws StructureBuildingException
void SMILESFragmentBuilderTest::testSimple5() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"CC(=O)O");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(4, atomList.size());
    Atom *carbon = atomList[1];
    std::vector<Atom *> neighbours = carbon->getAtomNeighbours();
    assertEquals(3, neighbours.size());
    assertEquals(atomList[0], neighbours[0]);
    assertEquals(atomList[2], neighbours[1]);
    assertEquals(atomList[3], neighbours[2]);
    assertEquals(2, carbon->getBondToAtomOrThrow(atomList[2])->getOrder());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSimple6() throws StructureBuildingException
void SMILESFragmentBuilderTest::testSimple6() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"C1CCCCC1");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(6, atomList.size());
    for (auto atom : atomList) {
        assertEquals(2, atom->getAtomNeighbours().size());
        assertEquals(false, atom->hasSpareValency());
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSimple7() throws StructureBuildingException
void SMILESFragmentBuilderTest::testSimple7() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"c1ccccc1");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(6, atomList.size());
    for (auto atom : atomList) {
        assertEquals(2, atom->getAtomNeighbours().size());
        assertEquals(true, atom->hasSpareValency());
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSimple8() throws StructureBuildingException
void SMILESFragmentBuilderTest::testSimple8() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"[I-].[Na+]");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(2, atomList.size());
    Atom *iodine = atomList[0];
    assertEquals(0, iodine->getAtomNeighbours().size());
    assertEquals(-1, iodine->getCharge());

    Atom *sodium = atomList[1];
    assertEquals(0, sodium->getAtomNeighbours().size());
    assertEquals(1, sodium->getCharge());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSimple9() throws StructureBuildingException
void SMILESFragmentBuilderTest::testSimple9() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"(C(=O)O)");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(3, atomList.size());
    Atom *carbon = atomList[0];
    assertEquals(2, carbon->getAtomNeighbours().size());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSimple10() throws StructureBuildingException
void SMILESFragmentBuilderTest::testSimple10() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"C-C-O");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(3, atomList.size());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSimple11() throws StructureBuildingException
void SMILESFragmentBuilderTest::testSimple11() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"NC(Cl)(Br)C(=O)O");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(7, atomList.size());
    assertEquals(ChemEl::Cl, atomList[2]->getElement());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=StructureBuildingException.class) public void unterminatedRingOpening() throws StructureBuildingException
void SMILESFragmentBuilderTest::unterminatedRingOpening() throw(StructureBuildingException) {
    sBuilder->build(L"C1CC");
    fail(L"Should throw exception for bad smiles");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void doublePositiveCharge1() throws StructureBuildingException
void SMILESFragmentBuilderTest::doublePositiveCharge1() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"[C++]");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(1, atomList.size());
    assertEquals(2, atomList[0]->getCharge());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void doublePositiveCharge2() throws StructureBuildingException
void SMILESFragmentBuilderTest::doublePositiveCharge2() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"[C+2]");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(1, atomList.size());
    assertEquals(2, atomList[0]->getCharge());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void doubleNegativeCharge1() throws StructureBuildingException
void SMILESFragmentBuilderTest::doubleNegativeCharge1() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"[O--]");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(1, atomList.size());
    assertEquals(-2, atomList[0]->getCharge());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void doubleNegativeCharge2() throws StructureBuildingException
void SMILESFragmentBuilderTest::doubleNegativeCharge2() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"[O-2]");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(1, atomList.size());
    assertEquals(-2, atomList[0]->getCharge());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void noIsotopeSpecified() throws StructureBuildingException
void SMILESFragmentBuilderTest::noIsotopeSpecified() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"[NH3]");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(1, atomList.size());
    assertEquals(nullptr, atomList[0]->getIsotope());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void isotopeSpecified() throws StructureBuildingException
void SMILESFragmentBuilderTest::isotopeSpecified() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"[15NH3]");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(1, atomList.size());
    assertNotNull(L"Isotope should not be null", atomList[0]->getIsotope());
    int isotope = atomList[0]->getIsotope();
    assertEquals(15, isotope);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=StructureBuildingException.class) public void badlyFormedSMILE1() throws StructureBuildingException
void SMILESFragmentBuilderTest::badlyFormedSMILE1() throw(StructureBuildingException) {
    sBuilder->build(L"H5");
    fail(L"Should throw exception for bad smiles");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=StructureBuildingException.class) public void badlyFormedSMILE2() throws StructureBuildingException
void SMILESFragmentBuilderTest::badlyFormedSMILE2() throw(StructureBuildingException) {
    sBuilder->build(L"CH4");
    fail(L"Should throw exception for bad smiles");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=StructureBuildingException.class) public void badlyFormedSMILE3() throws StructureBuildingException
void SMILESFragmentBuilderTest::badlyFormedSMILE3() throw(StructureBuildingException) {
    sBuilder->build(L"13C");
    fail(L"Should throw exception for bad smiles");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=StructureBuildingException.class) public void badlyFormedSMILE4() throws StructureBuildingException
void SMILESFragmentBuilderTest::badlyFormedSMILE4() throw(StructureBuildingException) {
    sBuilder->build(L"C=#C");
    fail(L"Should throw exception for bad smiles: is it a double or triple bond?");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=StructureBuildingException.class) public void badlyFormedSMILE5() throws StructureBuildingException
void SMILESFragmentBuilderTest::badlyFormedSMILE5() throw(StructureBuildingException) {
    sBuilder->build(L"C#=C");
    fail(L"Should throw exception for bad smiles: is it a double or triple bond?");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=StructureBuildingException.class) public void badlyFormedSMILE6() throws StructureBuildingException
void SMILESFragmentBuilderTest::badlyFormedSMILE6() throw(StructureBuildingException) {
    sBuilder->build(L"F//C=C/F");
    fail(L"Should throw exception for bad smiles: bond configuration specified twice");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=StructureBuildingException.class) public void badlyFormedSMILE7() throws StructureBuildingException
void SMILESFragmentBuilderTest::badlyFormedSMILE7() throw(StructureBuildingException) {
    sBuilder->build(L"F/C=C/\\F");
    fail(L"Should throw exception for bad smiles: bond configuration specified twice");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=StructureBuildingException.class) public void badlyFormedSMILE8() throws StructureBuildingException
void SMILESFragmentBuilderTest::badlyFormedSMILE8() throw(StructureBuildingException) {
    sBuilder->build(L"F[C@@](Cl)Br");
    fail(L"Should throw exception for invalid atom parity, not enough atoms in atom parity");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void ringClosureHandling1() throws StructureBuildingException
void SMILESFragmentBuilderTest::ringClosureHandling1() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"C=1CN1");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(3, atomList.size());
    assertEquals(2, atomList[0]->getBondToAtomOrThrow(atomList[2]).getOrder());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void ringClosureHandling2() throws StructureBuildingException
void SMILESFragmentBuilderTest::ringClosureHandling2() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"C1CN=1");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(3, atomList.size());
    assertEquals(2, atomList[0]->getBondToAtomOrThrow(atomList[2]).getOrder());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=StructureBuildingException.class) public void ringClosureHandling3() throws StructureBuildingException
void SMILESFragmentBuilderTest::ringClosureHandling3() throw(StructureBuildingException) {
    sBuilder->build(L"C#1CN=1");
    fail(L"Should throw exception for bad smiles");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void ringClosureHandling4() throws StructureBuildingException
void SMILESFragmentBuilderTest::ringClosureHandling4() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"C=1CN=1");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(3, atomList.size());
    assertEquals(2, atomList[0]->getBondToAtomOrThrow(atomList[2]).getOrder());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void ringSupportGreaterThan10() throws StructureBuildingException
void SMILESFragmentBuilderTest::ringSupportGreaterThan10() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"C%10CC%10");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(3, atomList.size());
    assertEquals(2, atomList[0]->getAtomNeighbours()->size());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hydrogenHandling1() throws StructureBuildingException
void SMILESFragmentBuilderTest::hydrogenHandling1() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"[OH3+]");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(1, atomList.size());
    assertEquals(1, atomList[0]->getCharge());
    assertEquals(1, atomList[0]->getProtonsExplicitlyAddedOrRemoved());
    assertEquals(3, atomList[0]->determineValency(true));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hydrogenHandling2() throws StructureBuildingException
void SMILESFragmentBuilderTest::hydrogenHandling2() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"[CH3][CH2][OH]");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(3, atomList.size());
    assertEquals(4, atomList[0]->determineValency(true));
    assertEquals(0, atomList[0]->getProtonsExplicitlyAddedOrRemoved());
    assertEquals(4, atomList[1]->determineValency(true));
    assertEquals(0, atomList[1]->getProtonsExplicitlyAddedOrRemoved());
    assertEquals(2, atomList[2]->determineValency(true));
    assertEquals(0, atomList[2]->getProtonsExplicitlyAddedOrRemoved());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hydrogenHandling3() throws StructureBuildingException
void SMILESFragmentBuilderTest::hydrogenHandling3() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"[SH2]");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(1, atomList.size());
    assertEquals(2, atomList[0]->determineValency(true));
    assertEquals(0, atomList[0]->getProtonsExplicitlyAddedOrRemoved());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hydrogenHandling4() throws StructureBuildingException
void SMILESFragmentBuilderTest::hydrogenHandling4() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"[SH4]");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(1, atomList.size());
    int minimumVal = atomList[0]->getMinimumValency();
    assertEquals(4, minimumVal);
    assertEquals(4, atomList[0]->determineValency(true));
    assertEquals(0, atomList[0]->getProtonsExplicitlyAddedOrRemoved());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hydrogenHandling5() throws StructureBuildingException
void SMILESFragmentBuilderTest::hydrogenHandling5() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"[SH6]");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(1, atomList.size());
    int minimumVal = atomList[0]->getMinimumValency();
    assertEquals(6, minimumVal);
    assertEquals(6, atomList[0]->determineValency(true));
    assertEquals(0, atomList[0]->getProtonsExplicitlyAddedOrRemoved());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hydrogenHandling6() throws StructureBuildingException
void SMILESFragmentBuilderTest::hydrogenHandling6() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"[SH3]");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(1, atomList.size());
    int minimumVal = atomList[0]->getMinimumValency();
    assertEquals(3, minimumVal);
    assertEquals(3, atomList[0]->determineValency(true));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hydrogenHandling7() throws StructureBuildingException
void SMILESFragmentBuilderTest::hydrogenHandling7() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"[SH3+]");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(1, atomList.size());
    assertEquals(1, atomList[0]->getCharge());
    assertEquals(1, atomList[0]->getProtonsExplicitlyAddedOrRemoved());
    assertEquals(3, atomList[0]->determineValency(true));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hydrogenHandling8() throws StructureBuildingException
void SMILESFragmentBuilderTest::hydrogenHandling8() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"[SH+]");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(1, atomList.size());
    assertEquals(1, atomList[0]->getCharge());
    assertEquals(-1, atomList[0]->getProtonsExplicitlyAddedOrRemoved());
    assertEquals(1, atomList[0]->determineValency(true));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hydrogenHandling9() throws StructureBuildingException
void SMILESFragmentBuilderTest::hydrogenHandling9() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"[SH3-]");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(1, atomList.size());
    assertEquals(-1, atomList[0]->getCharge());
    assertEquals(1, atomList[0]->getProtonsExplicitlyAddedOrRemoved());
    assertEquals(3, atomList[0]->determineValency(true));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hydrogenHandling10() throws StructureBuildingException
void SMILESFragmentBuilderTest::hydrogenHandling10() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"[SH-]");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(1, atomList.size());
    assertEquals(-1, atomList[0]->getCharge());
    assertEquals(-1, atomList[0]->getProtonsExplicitlyAddedOrRemoved());
    assertEquals(1, atomList[0]->determineValency(true));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hydrogenHandling11() throws StructureBuildingException
void SMILESFragmentBuilderTest::hydrogenHandling11() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"[SH5+]");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(1, atomList.size());
    int lambdaConvent = atomList[0]->getLambdaConventionValency();
    assertEquals(4, lambdaConvent);
    assertEquals(1, atomList[0]->getCharge());
    assertEquals(1, atomList[0]->getProtonsExplicitlyAddedOrRemoved());
    assertEquals(5, atomList[0]->determineValency(true));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hydrogenHandling12() throws StructureBuildingException
void SMILESFragmentBuilderTest::hydrogenHandling12() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"[Li+]");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(1, atomList.size());
    assertEquals(1, atomList[0]->getCharge());
    assertEquals(0, atomList[0]->getProtonsExplicitlyAddedOrRemoved());
    assertEquals(0, atomList[0]->determineValency(true));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hydrogenHandling13() throws StructureBuildingException
void SMILESFragmentBuilderTest::hydrogenHandling13() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"[NaH]");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(2, atomList.size());
    assertEquals(0, atomList[0]->getProtonsExplicitlyAddedOrRemoved());
    assertEquals(0, atomList[0]->getCharge());

    assertEquals(0, atomList[1]->getProtonsExplicitlyAddedOrRemoved());
    assertEquals(0, atomList[1]->getCharge());
    assertEquals(ChemEl::H, atomList[1]->getElement());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hydrogenHandling14() throws StructureBuildingException
void SMILESFragmentBuilderTest::hydrogenHandling14() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"-[SiH3]");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(1, atomList.size());
    assertEquals(4, atomList[0]->determineValency(true));
    assertEquals(0, atomList[0]->getProtonsExplicitlyAddedOrRemoved());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hydrogenHandling15() throws StructureBuildingException
void SMILESFragmentBuilderTest::hydrogenHandling15() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"=[SiH2]");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(1, atomList.size());
    assertEquals(4, atomList[0]->determineValency(true));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hydrogenHandling16() throws StructureBuildingException
void SMILESFragmentBuilderTest::hydrogenHandling16() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"#[SiH]");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(1, atomList.size());
    assertEquals(4, atomList[0]->determineValency(true));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hydrogenHandling17() throws StructureBuildingException
void SMILESFragmentBuilderTest::hydrogenHandling17() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"[SiH3]-");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(1, atomList.size());
    assertEquals(4, atomList[0]->determineValency(true));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hydrogenHandling18() throws StructureBuildingException
void SMILESFragmentBuilderTest::hydrogenHandling18() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"[SiH2]=");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(1, atomList.size());
    assertEquals(4, atomList[0]->determineValency(true));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hydrogenHandling19() throws StructureBuildingException
void SMILESFragmentBuilderTest::hydrogenHandling19() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"[SiH]#");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(1, atomList.size());
    assertEquals(4, atomList[0]->determineValency(true));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hydrogenHandling20() throws StructureBuildingException
void SMILESFragmentBuilderTest::hydrogenHandling20() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"=[Si]=");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(1, atomList.size());
    assertEquals(4, atomList[0]->determineValency(true));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hydrogenHandling21() throws StructureBuildingException
void SMILESFragmentBuilderTest::hydrogenHandling21() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"[o+]1ccccc1");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(6, atomList.size());
    assertEquals(1, atomList[0]->getProtonsExplicitlyAddedOrRemoved());
    assertEquals(true, atomList[0]->hasSpareValency());
    assertEquals(3, atomList[0]->determineValency(true));
    assertEquals(0, atomList[1]->getProtonsExplicitlyAddedOrRemoved());
    assertEquals(4, atomList[1]->determineValency(true));
    assertEquals(true, atomList[1]->hasSpareValency());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void indicatedHydrogen() throws StructureBuildingException
void SMILESFragmentBuilderTest::indicatedHydrogen() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"Nc1[nH]c(=O)c2c(n1)nc[nH]2");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(11, atomList.size());
    assertEquals(2, fragment->getIndicatedHydrogen().size());
    assertEquals(atomList[2], fragment->getIndicatedHydrogen()[0]);
    assertEquals(atomList[10], fragment->getIndicatedHydrogen()[1]);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void chiralityTest1() throws StructureBuildingException
void SMILESFragmentBuilderTest::chiralityTest1() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"N[C@@H](F)C");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(4, atomList.size());
    Atom *chiralAtom = atomList[1];
    assertEquals(3, chiralAtom->getAtomNeighbours().size());
    AtomParity *atomParity = chiralAtom->getAtomParity();
    std::vector<Atom *> atomRefs4 = atomParity->getAtomRefs4();
    assertEquals(atomList[0], atomRefs4[0]);
    assertEquals(AtomParity::hydrogen, atomRefs4[1]);
    assertEquals(atomList[2], atomRefs4[2]);
    assertEquals(atomList[3], atomRefs4[3]);
    assertEquals(1, atomParity->getParity());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void chiralityTest2() throws StructureBuildingException
void SMILESFragmentBuilderTest::chiralityTest2() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"N[C@H](F)C");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(4, atomList.size());
    Atom *chiralAtom = atomList[1];
    assertEquals(3, chiralAtom->getAtomNeighbours().size());
    AtomParity *atomParity = chiralAtom->getAtomParity();
    std::vector<Atom *> atomRefs4 = atomParity->getAtomRefs4();
    assertEquals(atomList[0], atomRefs4[0]);
    assertEquals(AtomParity::hydrogen, atomRefs4[1]);
    assertEquals(atomList[2], atomRefs4[2]);
    assertEquals(atomList[3], atomRefs4[3]);
    assertEquals(-1, atomParity->getParity());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void chiralityTest3() throws StructureBuildingException
void SMILESFragmentBuilderTest::chiralityTest3() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"C2.N1.F3.[C@@H]231");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(4, atomList.size());
    Atom *chiralAtom = atomList[3];
    assertEquals(3, chiralAtom->getAtomNeighbours().size());
    AtomParity *atomParity = chiralAtom->getAtomParity();
    std::vector<Atom *> atomRefs4 = atomParity->getAtomRefs4();
    assertEquals(AtomParity::hydrogen, atomRefs4[0]);
    assertEquals(atomList[0], atomRefs4[1]);
    assertEquals(atomList[2], atomRefs4[2]);
    assertEquals(atomList[1], atomRefs4[3]);
    assertEquals(1, atomParity->getParity());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void chiralityTest4() throws StructureBuildingException
void SMILESFragmentBuilderTest::chiralityTest4() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"[C@@H]231.C2.N1.F3");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(4, atomList.size());
    Atom *chiralAtom = atomList[0];
    assertEquals(3, chiralAtom->getAtomNeighbours().size());
    AtomParity *atomParity = chiralAtom->getAtomParity();
    std::vector<Atom *> atomRefs4 = atomParity->getAtomRefs4();
    assertEquals(AtomParity::hydrogen, atomRefs4[0]);
    assertEquals(atomList[1], atomRefs4[1]);
    assertEquals(atomList[3], atomRefs4[2]);
    assertEquals(atomList[2], atomRefs4[3]);
    assertEquals(1, atomParity->getParity());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void chiralityTest5() throws StructureBuildingException
void SMILESFragmentBuilderTest::chiralityTest5() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"[C@@H](Cl)1[C@H](C)(F).Br1");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(6, atomList.size());
    Atom *chiralAtom1 = atomList[0];
    assertEquals(3, chiralAtom1->getAtomNeighbours().size());
    AtomParity *atomParity = chiralAtom1->getAtomParity();
    std::vector<Atom *> atomRefs4 = atomParity->getAtomRefs4();
    assertEquals(AtomParity::hydrogen, atomRefs4[0]);
    assertEquals(atomList[1], atomRefs4[1]);
    assertEquals(atomList[5], atomRefs4[2]);
    assertEquals(atomList[2], atomRefs4[3]);
    assertEquals(1, atomParity->getParity());

    Atom *chiralAtom2 = atomList[2];
    assertEquals(3, chiralAtom2->getAtomNeighbours().size());
    atomParity = chiralAtom2->getAtomParity();
    atomRefs4 = atomParity->getAtomRefs4();
    assertEquals(atomList[0], atomRefs4[0]);
    assertEquals(AtomParity::hydrogen, atomRefs4[1]);
    assertEquals(atomList[3], atomRefs4[2]);
    assertEquals(atomList[4], atomRefs4[3]);
    assertEquals(-1, atomParity->getParity());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void chiralityTest6() throws StructureBuildingException
void SMILESFragmentBuilderTest::chiralityTest6() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"I[C@@](Cl)(Br)F");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(5, atomList.size());
    Atom *chiralAtom = atomList[1];
    assertEquals(4, chiralAtom->getAtomNeighbours().size());
    AtomParity *atomParity = chiralAtom->getAtomParity();
    std::vector<Atom *> atomRefs4 = atomParity->getAtomRefs4();
    assertEquals(atomList[0], atomRefs4[0]);
    assertEquals(atomList[2], atomRefs4[1]);
    assertEquals(atomList[3], atomRefs4[2]);
    assertEquals(atomList[4], atomRefs4[3]);
    assertEquals(1, atomParity->getParity());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void chiralityTest7() throws StructureBuildingException
void SMILESFragmentBuilderTest::chiralityTest7() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"C[S@](N)=O");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(4, atomList.size());
    Atom *chiralAtom = atomList[1];
    assertEquals(3, chiralAtom->getAtomNeighbours().size());
    AtomParity *atomParity = chiralAtom->getAtomParity();
    std::vector<Atom *> atomRefs4 = atomParity->getAtomRefs4();
    assertEquals(atomList[0], atomRefs4[0]);
    assertEquals(atomList[1], atomRefs4[1]);
    assertEquals(atomList[2], atomRefs4[2]);
    assertEquals(atomList[3], atomRefs4[3]);
    assertEquals(-1, atomParity->getParity());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void chiralityTest8() throws StructureBuildingException
void SMILESFragmentBuilderTest::chiralityTest8() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"[S@](C)(N)=O");
    std::vector<Atom *> atomList = fragment->getAtomList();
    assertEquals(4, atomList.size());
    Atom *chiralAtom = atomList[0];
    assertEquals(3, chiralAtom->getAtomNeighbours().size());
    AtomParity *atomParity = chiralAtom->getAtomParity();
    std::vector<Atom *> atomRefs4 = atomParity->getAtomRefs4();
    assertEquals(atomList[0], atomRefs4[0]);
    assertEquals(atomList[1], atomRefs4[1]);
    assertEquals(atomList[2], atomRefs4[2]);
    assertEquals(atomList[3], atomRefs4[3]);
    assertEquals(-1, atomParity->getParity());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondStereo1() throws StructureBuildingException
void SMILESFragmentBuilderTest::testDoubleBondStereo1() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"F/C=C/F");
    Bond *b = fragment->findBond(2, 3);
    assertEquals(BondStereoValue::TRANS, b->getBondStereo()->getBondStereoValue());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondStereo2() throws StructureBuildingException
void SMILESFragmentBuilderTest::testDoubleBondStereo2() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"F\\C=C/F");
    Bond *b = fragment->findBond(2, 3);
    assertEquals(BondStereoValue::CIS, b->getBondStereo()->getBondStereoValue());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondStereo3() throws StructureBuildingException
void SMILESFragmentBuilderTest::testDoubleBondStereo3() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"C(/F)=C/F");
    Bond *b = fragment->findBond(1, 3);
    assertEquals(BondStereoValue::CIS, b->getBondStereo()->getBondStereoValue());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondStereo4() throws StructureBuildingException
void SMILESFragmentBuilderTest::testDoubleBondStereo4() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"C(\\F)=C/F");
    Bond *b = fragment->findBond(1, 3);
    assertEquals(BondStereoValue::TRANS, b->getBondStereo()->getBondStereoValue());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondStereo5a() throws StructureBuildingException
void SMILESFragmentBuilderTest::testDoubleBondStereo5a() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"CC1=C/F.O\\1");
    Bond *b = fragment->findBond(2, 3);
    assertEquals(BondStereoValue::CIS, b->getBondStereo()->getBondStereoValue());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondStereo5b() throws StructureBuildingException
void SMILESFragmentBuilderTest::testDoubleBondStereo5b() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"CC/1=C/F.O1");
    Bond *b = fragment->findBond(2, 3);
    assertEquals(BondStereoValue::CIS, b->getBondStereo()->getBondStereoValue());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondStereo6() throws StructureBuildingException
void SMILESFragmentBuilderTest::testDoubleBondStereo6() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"CC1=C/F.O/1");
    Bond *b = fragment->findBond(2, 3);
    assertEquals(BondStereoValue::TRANS, b->getBondStereo()->getBondStereoValue());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondMultiStereo1() throws StructureBuildingException
void SMILESFragmentBuilderTest::testDoubleBondMultiStereo1() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"F/C=C/C=C/C");
    Bond *b = fragment->findBond(2, 3);
    assertEquals(BondStereoValue::TRANS, b->getBondStereo()->getBondStereoValue());
    b = fragment->findBond(4, 5);
    assertEquals(BondStereoValue::TRANS, b->getBondStereo()->getBondStereoValue());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondMultiStereo2() throws StructureBuildingException
void SMILESFragmentBuilderTest::testDoubleBondMultiStereo2() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"F/C=C\\C=C/C");
    Bond *b = fragment->findBond(2, 3);
    assertEquals(BondStereoValue::CIS, b->getBondStereo()->getBondStereoValue());
    b = fragment->findBond(4, 5);
    assertEquals(BondStereoValue::CIS, b->getBondStereo()->getBondStereoValue());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondMultiStereo3() throws StructureBuildingException
void SMILESFragmentBuilderTest::testDoubleBondMultiStereo3() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"F/C=C\\C=C\\C");
    Bond *b = fragment->findBond(2, 3);
    assertEquals(BondStereoValue::CIS, b->getBondStereo()->getBondStereoValue());
    b = fragment->findBond(4, 5);
    assertEquals(BondStereoValue::TRANS, b->getBondStereo()->getBondStereoValue());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondMultiStereo4() throws StructureBuildingException
void SMILESFragmentBuilderTest::testDoubleBondMultiStereo4() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"F/C=C\\C=CC");
    Bond *b = fragment->findBond(2, 3);
    assertEquals(BondStereoValue::CIS, b->getBondStereo()->getBondStereoValue());
    b = fragment->findBond(4, 5);
    assertEquals(nullptr, b->getBondStereo());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondNoela() throws StructureBuildingException
void SMILESFragmentBuilderTest::testDoubleBondNoela() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"C/C=C\\1/NC1");
    Bond *b = fragment->findBond(2, 3);
    if (BondStereoValue::TRANS::equals(b->getBondStereo()->getBondStereoValue())) {
        assertEquals(L"1 2 3 4", atomRefsToIdStr(b->getBondStereo()->getAtomRefs4()));
    } else {
        assertEquals(L"1 2 3 5", atomRefsToIdStr(b->getBondStereo()->getAtomRefs4()));
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondNoelb() throws StructureBuildingException
void SMILESFragmentBuilderTest::testDoubleBondNoelb() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"C/C=C1/NC1");
    Bond *b = fragment->findBond(2, 3);
    assertEquals(BondStereoValue::TRANS, b->getBondStereo()->getBondStereoValue());
    assertEquals(L"1 2 3 4", atomRefsToIdStr(b->getBondStereo()->getAtomRefs4()));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondNoelc() throws StructureBuildingException
void SMILESFragmentBuilderTest::testDoubleBondNoelc() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"C/C=C\\1/NC/1");
    Bond *b = fragment->findBond(2, 3);
    if (BondStereoValue::TRANS::equals(b->getBondStereo()->getBondStereoValue())) {
        assertEquals(L"1 2 3 4", atomRefsToIdStr(b->getBondStereo()->getAtomRefs4()));
    } else {
        assertEquals(L"1 2 3 5", atomRefsToIdStr(b->getBondStereo()->getAtomRefs4()));
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondNoeld() throws StructureBuildingException
void SMILESFragmentBuilderTest::testDoubleBondNoeld() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"C/C=C1/NC/1");
    Bond *b = fragment->findBond(2, 3);
    if (BondStereoValue::TRANS::equals(b->getBondStereo()->getBondStereoValue())) {
        assertEquals(L"1 2 3 4", atomRefsToIdStr(b->getBondStereo()->getAtomRefs4()));
    } else {
        assertEquals(L"1 2 3 5", atomRefsToIdStr(b->getBondStereo()->getAtomRefs4()));
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=StructureBuildingException.class) public void testDoubleBondNoele() throws StructureBuildingException
void SMILESFragmentBuilderTest::testDoubleBondNoele() throw(StructureBuildingException) {
    sBuilder->build(L"C/C=C\\1\\NC1");
    fail(L"Should throw exception for bad smiles: contradictory double bond configuration");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=StructureBuildingException.class) public void testDoubleBondNoelf() throws StructureBuildingException
void SMILESFragmentBuilderTest::testDoubleBondNoelf() throw(StructureBuildingException) {
    sBuilder->build(L"C/C=C\1NC\1");
    fail(L"Should throw exception for bad smiles: contradictory double bond configuration");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=StructureBuildingException.class) public void testDoubleBondNoelg() throws StructureBuildingException
void SMILESFragmentBuilderTest::testDoubleBondNoelg() throw(StructureBuildingException) {
    sBuilder->build(L"C/C=C\1/NC\1");
    fail(L"Should throw exception for bad smiles: contradictory double bond configuration");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondCornerCase1() throws StructureBuildingException
void SMILESFragmentBuilderTest::testDoubleBondCornerCase1() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"C\\1NC1=C/C");
    Bond *b = fragment->findBond(3, 4);
    assertEquals(BondStereoValue::CIS, b->getBondStereo()->getBondStereoValue());
    assertEquals(L"1 3 4 5", atomRefsToIdStr(b->getBondStereo()->getAtomRefs4()));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondCornerCase2() throws StructureBuildingException
void SMILESFragmentBuilderTest::testDoubleBondCornerCase2() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"C1NC/1=C/C");
    Bond *b = fragment->findBond(3, 4);
    assertEquals(BondStereoValue::CIS, b->getBondStereo()->getBondStereoValue());
    assertEquals(L"1 3 4 5", atomRefsToIdStr(b->getBondStereo()->getAtomRefs4()));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=StructureBuildingException.class) public void testDoubleBondCornerCase3() throws StructureBuildingException
void SMILESFragmentBuilderTest::testDoubleBondCornerCase3() throw(StructureBuildingException) {
    sBuilder->build(L"C/1=C/CCCCCC/1");
    fail(L"Should throw exception for bad smiles: contradictory double bond configuration");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=StructureBuildingException.class) public void testDoubleBondCornerCase4() throws StructureBuildingException
void SMILESFragmentBuilderTest::testDoubleBondCornerCase4() throw(StructureBuildingException) {
    sBuilder->build(L"C\\1=C/CCCCCC\\1");
    fail(L"Should throw exception for bad smiles: contradictory double bond configuration");
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondCornerCase5() throws StructureBuildingException
void SMILESFragmentBuilderTest::testDoubleBondCornerCase5() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"C\\1=C/CCCCCC/1");
    Bond *b = fragment->findBond(1, 2);
    assertEquals(BondStereoValue::TRANS, b->getBondStereo()->getBondStereoValue());
    assertEquals(L"8 1 2 3", atomRefsToIdStr(b->getBondStereo()->getAtomRefs4()));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondCornerCase6() throws StructureBuildingException
void SMILESFragmentBuilderTest::testDoubleBondCornerCase6() throw(StructureBuildingException) {
    Fragment *fragment = sBuilder->build(L"C/1=C/CCCCCC\\1");
    Bond *b = fragment->findBond(1, 2);
    assertEquals(BondStereoValue::CIS, b->getBondStereo()->getBondStereoValue());
    assertEquals(L"8 1 2 3", atomRefsToIdStr(b->getBondStereo()->getAtomRefs4()));
}

std::wstring SMILESFragmentBuilderTest::atomRefsToIdStr(std::vector<Atom *> &atomRefs4) {
    StringBuilder *sb = new StringBuilder();
    for (int i = 0; i < atomRefs4.size(); i++) {
        sb->append(atomRefs4[i]->getID());
        if (i + 1 < atomRefs4.size()) {
            sb->append(L' ');
        }
    }
    return sb->toString();
}
