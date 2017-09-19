#include "FragmentTest.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/Fragment.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/FragmentManager.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/IDManager.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/SMILESFragmentBuilder.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/StructureBuildingException.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/Atom.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/ChemEl.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/Bond.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/FragmentTools.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/XmlDeclarations.h"


using org::junit::Before;
using org::junit::Test;
//						import static org.junit.Assert.*;
using namespace uk::ac::cam::ch::wwmm::opsin;
//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void setUp()
void FragmentTest::setUp() {
    IDManager *idManager = new IDManager();
    SMILESFragmentBuilder tempVar(idManager);
    fm = new FragmentManager(&tempVar, idManager);
    try {
        frag = fm->buildSMILES(L"");
    }
    catch (const StructureBuildingException &e) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException(e);
        throw std::exception();
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testFragment()
void FragmentTest::testFragment() {
    assertNotNull(L"Has atom list", frag->getAtomList());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAddAtom()
void FragmentTest::testAddAtom() {
    assertEquals(L"Has no atoms", 0, frag->getAtomCount());
    Atom tempVar(1, ChemEl::C, frag);
    frag->addAtom(&tempVar);
    assertEquals(L"Now has one atom", 1, frag->getAtomCount());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAddBond()
void FragmentTest::testAddBond() {
    Atom tempVar(1, ChemEl::C, frag);
    frag->addAtom(&tempVar);
    Atom tempVar2(2, ChemEl::C, frag);
    frag->addAtom(&tempVar2);
    assertEquals(L"Has no bonds", 0, frag->getBondSet()->size());
    fm->createBond(frag->getAtomByID(1), frag->getAtomByID(2), 1);
    assertEquals(L"Now has one bond", 1, frag->getBondSet()->size());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testImportFrag() throws StructureBuildingException
void FragmentTest::testImportFrag() throw(StructureBuildingException) {
    Fragment *frag1 = fm->buildSMILES(L"CC");
    Fragment *frag2 = fm->buildSMILES(L"CC");
    assertEquals(L"Fragment has two atoms", 2, frag1->getAtomCount());
    assertEquals(L"Fragment has one bond", 1, frag1->getBondSet()->size());
    fm->incorporateFragment(frag2, frag1);
    assertEquals(L"Fragment now has four atoms", 4, frag1->getAtomCount());
    assertEquals(L"Fragment now has two bonds", 2, frag1->getBondSet()->size());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testImportFragWithIntraFragBonds1() throws StructureBuildingException
void FragmentTest::testImportFragWithIntraFragBonds1() throw(StructureBuildingException) {
    Fragment *frag1 = fm->buildSMILES(L"C");
    Fragment *frag2 = fm->buildSMILES(L"C");
    fm->createBond(frag1->getFirstAtom(), frag2->getFirstAtom(), 1);
    assertEquals(0, frag1->getBondSet()->size());
    assertEquals(0, frag2->getBondSet()->size());
    assertEquals(1, fm->getInterFragmentBonds(frag1)->size());
    assertEquals(1, fm->getInterFragmentBonds(frag2)->size());
    fm->incorporateFragment(frag2, frag1);
    assertEquals(1, frag1->getBondSet()->size());
    assertEquals(0, frag2->getBondSet()->size());
    assertEquals(0, fm->getInterFragmentBonds(frag1)->size());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testImportFragWithIntraFragBonds2() throws StructureBuildingException
void FragmentTest::testImportFragWithIntraFragBonds2() throw(StructureBuildingException) {
    Fragment *frag1 = fm->buildSMILES(L"C");
    Fragment *frag2 = fm->buildSMILES(L"C");
    Fragment *frag3 = fm->buildSMILES(L"C");
    fm->createBond(frag2->getFirstAtom(), frag3->getFirstAtom(), 1);
    assertEquals(0, frag1->getBondSet()->size());
    assertEquals(0, frag2->getBondSet()->size());
    assertEquals(0, frag3->getBondSet()->size());
    assertEquals(0, fm->getInterFragmentBonds(frag1)->size());
    assertEquals(1, fm->getInterFragmentBonds(frag2)->size());
    assertEquals(1, fm->getInterFragmentBonds(frag3)->size());
    fm->incorporateFragment(frag2, frag1);
    assertEquals(0, frag1->getBondSet()->size());
    assertEquals(0, frag2->getBondSet()->size());
    assertEquals(0, frag3->getBondSet()->size());
    assertEquals(1, fm->getInterFragmentBonds(frag1)->size());
    assertEquals(1, fm->getInterFragmentBonds(frag3)->size());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testGetIDFromLocant()
void FragmentTest::testGetIDFromLocant() {
    Atom *atom = new Atom(10, ChemEl::C, frag);
    atom->addLocant(L"a");
    frag->addAtom(atom);
    atom = new Atom(20, ChemEl::C, frag);
    atom->addLocant(L"silly");
    frag->addAtom(atom);
    assertEquals(L"Locant a has ID 10", 10, frag->getIDFromLocant(L"a"));
    assertEquals(L"Locant silly has ID 20", 20, frag->getIDFromLocant(L"silly"));
    assertEquals(L"Locant 42 is not present", 0, frag->getIDFromLocant(L"42"));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testGetAtomByLocant()
void FragmentTest::testGetAtomByLocant() {
    Atom *atom1 = new Atom(10, ChemEl::C, frag);
    atom1->addLocant(L"a");
    frag->addAtom(atom1);
    Atom *atom2 = new Atom(20, ChemEl::C, frag);
    atom2->addLocant(L"silly");
    frag->addAtom(atom2);
    assertEquals(L"Locant a gets atom1", atom1, frag->getAtomByLocant(L"a"));
    assertEquals(L"Locant silly gets atom2", atom2, frag->getAtomByLocant(L"silly"));
    assertNull(L"Locant 42 is not present", frag->getAtomByLocant(L"42"));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testGetAtomByID()
void FragmentTest::testGetAtomByID() {
    Atom *atom1 = new Atom(10, ChemEl::C, frag);
    frag->addAtom(atom1);
    Atom *atom2 = new Atom(20, ChemEl::C, frag);
    frag->addAtom(atom2);
    assertEquals(L"ID 10 gets atom1", atom1, frag->getAtomByID(10));
    assertEquals(L"ID 20 gets atom2", atom2, frag->getAtomByID(20));
    assertNull(L"ID 42 is not present", frag->getAtomByID(42));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testFindBond()
void FragmentTest::testFindBond() {
    Atom tempVar(1, ChemEl::C, frag);
    frag->addAtom(&tempVar);
    Atom tempVar2(2, ChemEl::C, frag);
    frag->addAtom(&tempVar2);
    Atom tempVar3(3, ChemEl::N, frag);
    frag->addAtom(&tempVar3);
    Atom tempVar4(4, ChemEl::O, frag);
    frag->addAtom(&tempVar4);
    fm->createBond(frag->getAtomByID(2), frag->getAtomByID(4), 2);
    fm->createBond(frag->getAtomByID(1), frag->getAtomByID(2), 1);
    fm->createBond(frag->getAtomByID(1), frag->getAtomByID(3), 3);
    Bond *b = frag->findBond(2, 4);
    assertNotNull(L"Found a bond", b);
    assertEquals(L"..a double bond", 2, b->getOrder());
    b = frag->findBond(3, 1);
    assertNotNull(L"Found a different bond", b);
    assertEquals(L"..a triple bond", 3, b->getOrder());
    b = frag->findBond(2, 3);
    assertNull(L"Don't find non-existent bonds", b);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testGetChainLength()
void FragmentTest::testGetChainLength() {
    assertEquals(L"No chain", 0, frag->getChainLength());
    Atom *a1 = new Atom(1, ChemEl::C, frag);
    a1->addLocant(L"1");
    frag->addAtom(a1);
    assertEquals(L"Methane", 1, frag->getChainLength());
    Atom *a2 = new Atom(2, ChemEl::C, frag);
    a2->addLocant(L"2");
    frag->addAtom(a2);
    fm->createBond(frag->getAtomByID(1), frag->getAtomByID(2), 1);
    assertEquals(L"ethane", 2, frag->getChainLength());
    Atom *a3 = new Atom(3, ChemEl::C, frag);
    a3->addLocant(L"3");
    frag->addAtom(a3);
    fm->createBond(frag->getAtomByID(2), frag->getAtomByID(3), 1);
    assertEquals(L"propane", 3, frag->getChainLength());
    Atom *a4 = new Atom(4, ChemEl::C, frag);
    frag->addAtom(a4);
    a4->addLocant(L"4");
    fm->createBond(frag->getAtomByID(2), frag->getAtomByID(4), 1);
    assertEquals(L"isobutane", 3, frag->getChainLength());
    fm->removeBond(a2->getBondToAtom(a4));
    fm->createBond(a3, a4, 1);
    assertEquals(L"butane", 4, frag->getChainLength());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRelabelSuffixLocants() throws StructureBuildingException
void FragmentTest::testRelabelSuffixLocants() throw(StructureBuildingException) {
    frag = fm->buildSMILES(L"C(N)N");
    assertEquals(L"Can't find locant N in frag", 0, frag->getIDFromLocant(L"N"));
    assertEquals(L"Can't find locant N' in frag", 0, frag->getIDFromLocant(L"N'"));
    FragmentTools::assignElementLocants(frag, std::vector<Fragment *>());
    int idN = frag->getIDFromLocant(L"N");
    int idNprime = frag->getIDFromLocant(L"N'");
    if ((idN == 2 && idNprime == 3) || idN == 3 && idNprime == 2) {
    } else {
        fail(L"Locants misassigned");
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testLabelCarbamimidamido() throws StructureBuildingException
void FragmentTest::testLabelCarbamimidamido() throw(StructureBuildingException) {
    frag = fm->buildSMILES(L"C(N)(=N)N-", XmlDeclarations::NONCARBOXYLICACID_TYPE_VAL,
                           XmlDeclarations::NONE_LABELS_VAL);
    FragmentTools::assignElementLocants(frag, std::vector<Fragment *>());
    assertEquals(L"Can find locant N in frag: ID = 4", 4, frag->getIDFromLocant(L"N"));
    assertEquals(L"Can find locant N' in frag: ID = 2", 2, frag->getIDFromLocant(L"N'"));
    assertEquals(L"Can find locant N'' in frag: ID = 3", 3, frag->getIDFromLocant(L"N''"));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testLabelHydrazonoHydrazide() throws StructureBuildingException
void FragmentTest::testLabelHydrazonoHydrazide() throw(StructureBuildingException) {
    frag = fm->buildSMILES(L"C(=NN)NN", XmlDeclarations::NONCARBOXYLICACID_TYPE_VAL, XmlDeclarations::NONE_LABELS_VAL);
    FragmentTools::assignElementLocants(frag, std::vector<Fragment *>());
    assertEquals(L"Can find locant N in frag: ID = 4", 4, frag->getIDFromLocant(L"N"));
    assertEquals(L"Can find locant N' in frag: ID = 5", 5, frag->getIDFromLocant(L"N'"));
    assertEquals(L"Can find locant N'' in frag: ID = 2", 2, frag->getIDFromLocant(L"N''"));
    assertEquals(L"Can find locant N''' in frag: ID = 3", 3, frag->getIDFromLocant(L"N'''"));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testLabelCarbonimidoyl() throws StructureBuildingException
void FragmentTest::testLabelCarbonimidoyl() throw(StructureBuildingException) {
    frag = fm->buildSMILES(L"C(=N)", XmlDeclarations::ACIDSTEM_TYPE_VAL, XmlDeclarations::NONE_LABELS_VAL);
    frag->addOutAtom(frag->getFirstAtom(), 1, true);
    frag->addOutAtom(frag->getFirstAtom(), 1, true);
    FragmentTools::assignElementLocants(frag, std::vector<Fragment *>());
    assertEquals(L"Can find locant N in frag: ID = 2", 2, frag->getIDFromLocant(L"N"));
    assertEquals(L"Can find locant N in frag: ID = 1", 1, frag->getIDFromLocant(L"C"));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testLabelHydrazonicAmide() throws StructureBuildingException
void FragmentTest::testLabelHydrazonicAmide() throw(StructureBuildingException) {
    frag = fm->buildSMILES(L"C", XmlDeclarations::ACIDSTEM_TYPE_VAL, XmlDeclarations::NONE_LABELS_VAL);
    Fragment *suffixfrag = fm->buildSMILES(L"[R](N)=NN", XmlDeclarations::SUFFIX_TYPE_VAL,
                                           XmlDeclarations::NONE_LABELS_VAL);
    std::vector<Fragment *> suffixes;
    suffixes.push_back(suffixfrag);
    FragmentTools::assignElementLocants(frag, suffixes);
    fm->incorporateFragment(suffixfrag, frag);
    assertEquals(L"Can find locant N in frag: ID = 3", 3, frag->getIDFromLocant(L"N"));
    assertEquals(L"Can find locant N'' in frag: ID = 5", 5, frag->getIDFromLocant(L"N''"));

    //DEVIATION From systematic behaviour
    assertEquals(L"Can find locant N' in frag: ID = 5", 5, frag->getIDFromLocant(L"N'"));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testLabelHydrazonate() throws StructureBuildingException
void FragmentTest::testLabelHydrazonate() throw(StructureBuildingException) {
    frag = fm->buildSMILES(L"C", XmlDeclarations::ACIDSTEM_TYPE_VAL, XmlDeclarations::NONE_LABELS_VAL);
    Fragment *suffixfrag = fm->buildSMILES(L"[R]([O-])=NN", XmlDeclarations::SUFFIX_TYPE_VAL,
                                           XmlDeclarations::NONE_LABELS_VAL);
    std::vector<Fragment *> suffixes;
    suffixes.push_back(suffixfrag);
    FragmentTools::assignElementLocants(frag, suffixes);
    fm->incorporateFragment(suffixfrag, frag);
    assertEquals(L"Can find locant N' in frag: ID = 5", 5, frag->getIDFromLocant(L"N'"));

    //DEVIATION From systematic behaviour
    assertEquals(L"Can find locant N in frag: ID = 5", 5, frag->getIDFromLocant(L"N"));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testLabelHexanDiamide() throws StructureBuildingException
void FragmentTest::testLabelHexanDiamide() throw(StructureBuildingException) {
    frag = fm->buildSMILES(L"CCCCCC", XmlDeclarations::CHAIN_TYPE_VAL, L"1/2/3/4/5/6");
    Fragment *suffixfrag1 = fm->buildSMILES(L"[R]N", XmlDeclarations::SUFFIX_TYPE_VAL,
                                            XmlDeclarations::NONE_LABELS_VAL);
    Fragment *suffixfrag2 = fm->buildSMILES(L"[R]N", XmlDeclarations::SUFFIX_TYPE_VAL,
                                            XmlDeclarations::NONE_LABELS_VAL);
    std::vector<Fragment *> suffixes;
    suffixes.push_back(suffixfrag1);
    suffixes.push_back(suffixfrag2);
    FragmentTools::assignElementLocants(frag, suffixes);
    fm->incorporateFragment(suffixfrag1, frag);
    fm->incorporateFragment(suffixfrag2, frag);
    assertEquals(L"Can find locant N in frag: ID = 8", 8, frag->getIDFromLocant(L"N"));
    assertEquals(L"Can find locant N' in frag: ID = 10", 10, frag->getIDFromLocant(L"N'"));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testLabelDiimidooxalicDiamide() throws StructureBuildingException
void FragmentTest::testLabelDiimidooxalicDiamide() throw(StructureBuildingException) {
    frag = fm->buildSMILES(L"CC", XmlDeclarations::ACIDSTEM_TYPE_VAL, L"1/2");
    Fragment *suffixfrag1 = fm->buildSMILES(L"[R](N)=N", XmlDeclarations::SUFFIX_TYPE_VAL,
                                            XmlDeclarations::NONE_LABELS_VAL);
    Fragment *suffixfrag2 = fm->buildSMILES(L"[R](N)=N", XmlDeclarations::SUFFIX_TYPE_VAL,
                                            XmlDeclarations::NONE_LABELS_VAL);
    std::vector<Fragment *> suffixes;
    suffixes.push_back(suffixfrag1);
    suffixes.push_back(suffixfrag2);
    FragmentTools::assignElementLocants(frag, suffixes);
    fm->incorporateFragment(suffixfrag1, frag);
    fm->incorporateFragment(suffixfrag2, frag);
    assertEquals(L"Can find locant N in frag: ID = 4", 4, frag->getIDFromLocant(L"N"));
    assertEquals(L"Can find locant N' in frag: ID = 7", 7, frag->getIDFromLocant(L"N'"));
    assertEquals(L"Can find locant N'' in frag: ID = 5", 5, frag->getIDFromLocant(L"N''"));
    assertEquals(L"Can find locant N''' in frag: ID = 8", 8, frag->getIDFromLocant(L"N'''"));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testLabelHydrazinecarbohydrazide() throws StructureBuildingException
void FragmentTest::testLabelHydrazinecarbohydrazide() throw(StructureBuildingException) {
    frag = fm->buildSMILES(L"NN", XmlDeclarations::SIMPLEGROUP_TYPE_VAL, L"1/2");
    Fragment *suffix = fm->buildSMILES(L"[R]C(=O)NN", XmlDeclarations::SUFFIX_TYPE_VAL, L"/X///");
    std::vector<Fragment *> suffixes;
    suffixes.push_back(suffix);
    FragmentTools::assignElementLocants(frag, suffixes);
    fm->incorporateFragment(suffix, frag);
    assertEquals(L"Can find locant N in frag: ID = 6", 6, frag->getIDFromLocant(L"N"));
    assertEquals(L"Can find locant N' in frag: ID = 7", 7, frag->getIDFromLocant(L"N'"));
    assertEquals(L"Can't find locant N'' in frag", 0, frag->getIDFromLocant(L"N''"));
    assertEquals(L"Can't find locant C in frag", 0, frag->getIDFromLocant(L"C"));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testLabelCarbonicDihydrazide() throws StructureBuildingException
void FragmentTest::testLabelCarbonicDihydrazide() throw(StructureBuildingException) {
    frag = fm->buildSMILES(L"C(=O)(NN)NN", XmlDeclarations::NONCARBOXYLICACID_TYPE_VAL,
                           XmlDeclarations::NONE_LABELS_VAL);
    FragmentTools::assignElementLocants(frag, std::vector<Fragment *>());
    int idN = frag->getIDFromLocant(L"N");
    int idNprime = frag->getIDFromLocant(L"N'");
    int idNprime2 = frag->getIDFromLocant(L"N''");
    int idNprime3 = frag->getIDFromLocant(L"N'''");
    if ((idN == 3 && idNprime == 4 && idNprime2 == 5 && idNprime3 == 6) ||
        (idN == 5 && idNprime == 6 && idNprime2 == 3 && idNprime3 == 4)) {
    } else {
        fail(L"Locants misassigned");
    }
    assertEquals(L"Can't find locant C in frag", 0, frag->getIDFromLocant(L"C"));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testLabelSulfonoThioate() throws StructureBuildingException
void FragmentTest::testLabelSulfonoThioate() throw(StructureBuildingException) {
    frag = fm->buildSMILES(L"C");
    Fragment *suffix = fm->buildSMILES(L"[R]S(=O)(=O)S", XmlDeclarations::SUFFIX_TYPE_VAL, L"/X///");
    std::vector<Fragment *> suffixes;
    suffixes.push_back(suffix);
    FragmentTools::assignElementLocants(frag, suffixes);
    fm->incorporateFragment(suffix, frag);
    assertEquals(L"Can find locant S in frag: ID = 6", 6, frag->getIDFromLocant(L"S"));
    assertEquals(L"Can't find locant S' in frag", 0, frag->getIDFromLocant(L"S'"));
    int idO = frag->getIDFromLocant(L"O");
    int idOprime = frag->getIDFromLocant(L"O'");
    if ((idO == 4 && idOprime == 5) || (idO == 5 && idOprime == 4)) {
    } else {
        fail(L"Locants misassigned");
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testLabelAcetoanilide() throws StructureBuildingException
void FragmentTest::testLabelAcetoanilide() throw(StructureBuildingException) {
    frag = fm->buildSMILES(L"CC");
    Fragment *suffix = fm->buildSMILES(L"[*](=O)Nc1ccccc1", XmlDeclarations::SUFFIX_TYPE_VAL, L"///1'/2'/3'/4'/5'/6'");
    std::vector<Fragment *> suffixes;
    suffixes.push_back(suffix);
    FragmentTools::assignElementLocants(frag, suffixes);
    fm->incorporateFragment(suffix, frag);
    assertEquals(L"Can find locant N in frag: ID = 5", 5, frag->getIDFromLocant(L"N"));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testLabelPyridine() throws StructureBuildingException
void FragmentTest::testLabelPyridine() throw(StructureBuildingException) {
    frag = fm->buildSMILES(L"n1ccccc1", XmlDeclarations::RING_TYPE_VAL, L"1/2/3/4/5/6");
    FragmentTools::assignElementLocants(frag, std::vector<Fragment *>());
    assertEquals(L"Can find locant N in frag: ID = 1", 1, frag->getIDFromLocant(L"N"));
    assertEquals(L"Can't find locant C in frag", 0, frag->getIDFromLocant(L"C"));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testLabelPiperazine() throws StructureBuildingException
void FragmentTest::testLabelPiperazine() throw(StructureBuildingException) {
    frag = fm->buildSMILES(L"N1CCNCC1", XmlDeclarations::RING_TYPE_VAL, L"1/2/3/4/5/6");
    FragmentTools::assignElementLocants(frag, std::vector<Fragment *>());
    int idN = frag->getIDFromLocant(L"N");
    int idNprime = frag->getIDFromLocant(L"N'");
    if ((idN == 1 && idNprime == 4) || (idN == 4 && idNprime == 1)) {
    } else {
        fail(L"Locants misassigned");
    }
    assertEquals(L"Can't find locant C in frag", 0, frag->getIDFromLocant(L"C"));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testLabelCarboximidohydrazide() throws StructureBuildingException
void FragmentTest::testLabelCarboximidohydrazide() throw(StructureBuildingException) {
    frag = fm->buildSMILES(L"c1ccccc1");
    Fragment *suffix = fm->buildSMILES(L"[R]C(=N)NN", XmlDeclarations::SUFFIX_TYPE_VAL, L"/X//1'/2'");
    std::vector<Fragment *> suffixes;
    suffixes.push_back(suffix);
    FragmentTools::assignElementLocants(frag, suffixes);
    fm->incorporateFragment(suffix, frag);
    assertEquals(L"Can find locant N in frag: ID = 10", 10, frag->getIDFromLocant(L"N"));
    assertEquals(L"Can find locant N' in frag: ID = 11", 11, frag->getIDFromLocant(L"N'"));
    assertEquals(L"Can find locant N'' in frag: ID = 9", 9, frag->getIDFromLocant(L"N''"));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testIndicatedHydrogen() throws StructureBuildingException
void FragmentTest::testIndicatedHydrogen() throw(StructureBuildingException) {
    Fragment *pyrrole = fm->buildSMILES(L"[nH]1cccc1");
    assertEquals(L"Pyrrole has 1 indicated hydrogen", 1, pyrrole->getIndicatedHydrogen().size());
    assertEquals(L"..and the indicated hydrogen is on the nitrogen", pyrrole->getFirstAtom(),
                 pyrrole->getIndicatedHydrogen()[0]);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSpareValenciesOnAromaticAtoms() throws StructureBuildingException
void FragmentTest::testSpareValenciesOnAromaticAtoms() throw(StructureBuildingException) {
    Fragment *naphthalene = fm->buildSMILES(L"c1cccc2ccccc12");
    for (auto a : naphthalene->getAtomList()) {
        assertEquals(L"All atoms have sv", true, a->hasSpareValency());
    }
    for (auto b : naphthalene->getBondSet()) {
        assertEquals(L"All bonds are of order 1", 1, b->getOrder());
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testConvertSpareValenciesToDoubleBonds() throws StructureBuildingException
void FragmentTest::testConvertSpareValenciesToDoubleBonds() throw(StructureBuildingException) {
    Fragment *dhp = fm->buildSMILES(L"c1cCccC1");
    FragmentTools::convertSpareValenciesToDoubleBonds(dhp);
    for (auto a : dhp->getAtomList()) {
        assertEquals(L"All atoms have no sv", false, a->hasSpareValency());
    }
    Fragment *funnydiene = fm->buildSMILES(L"C(=C)C=C");
    FragmentTools::convertSpareValenciesToDoubleBonds(funnydiene);
    for (auto a : funnydiene->getAtomList()) {
        assertEquals(L"All atoms have no sv", false, a->hasSpareValency());
    }
    Fragment *naphthalene = fm->buildSMILES(L"c1cccc2ccccc12");
    FragmentTools::convertSpareValenciesToDoubleBonds(naphthalene);
    for (auto a : naphthalene->getAtomList()) {
        assertEquals(L"All atoms have no sv", false, a->hasSpareValency());
    }
    Fragment *pentalene = fm->buildSMILES(L"c12c(ccc1)ccc2");
    for (auto a : pentalene->getAtomList()) {
        assertEquals(L"All atoms have sv", true, a->hasSpareValency());
    }
    FragmentTools::convertSpareValenciesToDoubleBonds(pentalene);
    for (auto a : pentalene->getAtomList()) {
        assertEquals(L"All atoms have no sv", false, a->hasSpareValency());
    }

}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testGetAtomNeighbours() throws StructureBuildingException
void FragmentTest::testGetAtomNeighbours() throw(StructureBuildingException) {
    Fragment *naphthalene = fm->buildSMILES(L"C1=CC=CC2=CC=CC=C12");
    assertEquals(L"Atom 1 has two neighbours", 2,
                 naphthalene->getIntraFragmentAtomNeighbours(naphthalene->getAtomByID(1)).size());
    assertEquals(L"Atom 5 has three neighbours", 3,
                 naphthalene->getIntraFragmentAtomNeighbours(naphthalene->getAtomByID(5)).size());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testIsCharacteristicAtomSuffix() throws StructureBuildingException
void FragmentTest::testIsCharacteristicAtomSuffix() throw(StructureBuildingException) {
    Fragment *parent = fm->buildSMILES(L"CC");
    Fragment *suffix = fm->buildSMILES(L"N", XmlDeclarations::SUFFIX_TYPE_VAL, XmlDeclarations::NONE_LABELS_VAL);
    fm->incorporateFragment(suffix, suffix->getFirstAtom(), parent, parent->getFirstAtom(), 1);
    std::vector<Atom *> parentAtoms = parent->getAtomList();
    assertFalse(FragmentTools::isCharacteristicAtom(parentAtoms[0]));
    assertFalse(FragmentTools::isCharacteristicAtom(parentAtoms[1]));
    assertTrue(FragmentTools::isCharacteristicAtom(parentAtoms[2]));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testIsCharacteristicAtomAldehyde() throws StructureBuildingException
void FragmentTest::testIsCharacteristicAtomAldehyde() throw(StructureBuildingException) {
    Fragment *parent = fm->buildSMILES(L"CC");
    Fragment *suffix = fm->buildSMILES(L"O", XmlDeclarations::SUFFIX_TYPE_VAL, XmlDeclarations::NONE_LABELS_VAL);
    fm->incorporateFragment(suffix, suffix->getFirstAtom(), parent, parent->getFirstAtom(), 2);
    std::vector<Atom *> parentAtoms = parent->getAtomList();
    parentAtoms[1]->setProperty(Atom::ISALDEHYDE, true);
    assertFalse(FragmentTools::isCharacteristicAtom(parentAtoms[0]));
    assertTrue(FragmentTools::isCharacteristicAtom(parentAtoms[1]));
    assertTrue(FragmentTools::isCharacteristicAtom(parentAtoms[2]));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testIsCharacteristicAtomFunctionalAtom() throws StructureBuildingException
void FragmentTest::testIsCharacteristicAtomFunctionalAtom() throw(StructureBuildingException) {
    Fragment *parent = fm->buildSMILES(L"CC(=O)[O-]");
    std::vector<Atom *> parentAtoms = parent->getAtomList();
    parent->addFunctionalAtom(parentAtoms[3]);
    for (int i = 0; i < parentAtoms.size() - 1; i++) {
        assertFalse(FragmentTools::isCharacteristicAtom(parentAtoms[i]));
    }
    assertTrue(FragmentTools::isCharacteristicAtom(parentAtoms[parentAtoms.size() - 1]));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testIsCharacteristicAtomHydroxy() throws StructureBuildingException
void FragmentTest::testIsCharacteristicAtomHydroxy() throw(StructureBuildingException) {
    std::vector<Atom *> phenolAtoms = fm->buildSMILES(L"Oc1ccccc1")->getAtomList();
    assertTrue(FragmentTools::isCharacteristicAtom(phenolAtoms[0]));
    for (int i = 1; i < phenolAtoms.size(); i++) {
        assertFalse(FragmentTools::isCharacteristicAtom(phenolAtoms[i]));
    }
}
