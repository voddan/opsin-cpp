#include "SMILESWriterTest.h"
#include "opsin-core/FragmentManager.h"
#include "opsin-core/IDManager.h"
#include "opsin-core/SMILESFragmentBuilder.h"
#include "opsin-core/StructureBuildingException.h"
#include "opsin-core/Fragment.h"
#include "opsin-core/SMILESWriter.h"
#include "opsin-core/StringTools.h"
#include "opsin-core/Atom.h"
#include "opsin-core/BondStereo.h"
#include "opsin-core/XmlDeclarations.h"


//						import static org.junit.Assert.*;
using org::junit::Before;
using org::junit::Test;
using BondStereoValue = BondStereo::BondStereoValue;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void setup()
void SMILESWriterTest::setup() {
    IDManager *idManager = new IDManager();
    SMILESFragmentBuilder tempVar(idManager);
    fm = new FragmentManager(&tempVar, idManager);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRoundTrip1() throws StructureBuildingException
void SMILESWriterTest::testRoundTrip1() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"C");
    fm->makeHydrogensExplicit();
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    assertEquals(L"C", smiles);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRoundTrip2() throws StructureBuildingException
void SMILESWriterTest::testRoundTrip2() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"C#N");
    fm->makeHydrogensExplicit();
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    assertEquals(L"C#N", smiles);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRoundTrip3() throws StructureBuildingException
void SMILESWriterTest::testRoundTrip3() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(StringTools::multiplyString(L"C", 200));
    fm->makeHydrogensExplicit();
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    assertEquals(StringTools::multiplyString(L"C", 200), smiles);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRoundTrip4() throws StructureBuildingException
void SMILESWriterTest::testRoundTrip4() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"O=C=O");
    fm->makeHydrogensExplicit();
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    assertEquals(L"O=C=O", smiles);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRoundTrip5() throws StructureBuildingException
void SMILESWriterTest::testRoundTrip5() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"CCN(CC)CC");
    fm->makeHydrogensExplicit();
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    assertEquals(L"CCN(CC)CC", smiles);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRoundTrip6() throws StructureBuildingException
void SMILESWriterTest::testRoundTrip6() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"CC(=O)O");
    fm->makeHydrogensExplicit();
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    assertEquals(L"CC(=O)O", smiles);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRoundTrip7() throws StructureBuildingException
void SMILESWriterTest::testRoundTrip7() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"C1CCCCC1");
    fm->makeHydrogensExplicit();
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    assertEquals(L"C1CCCCC1", smiles);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRoundTrip8() throws StructureBuildingException
void SMILESWriterTest::testRoundTrip8() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"C1=CC=CC=C1");
    fm->makeHydrogensExplicit();
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    assertEquals(L"C1=CC=CC=C1", smiles);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRoundTrip9() throws StructureBuildingException
void SMILESWriterTest::testRoundTrip9() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"NC(Cl)(Br)C(=O)O");
    fm->makeHydrogensExplicit();
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    assertEquals(L"NC(Cl)(Br)C(=O)O", smiles);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRoundTrip10() throws StructureBuildingException
void SMILESWriterTest::testRoundTrip10() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"[NH4+].[Cl-].F.[He-2]");
    fm->makeHydrogensExplicit();
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    assertEquals(L"[NH4+].[Cl-].F.[He-2]", smiles);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRoundTrip11() throws StructureBuildingException
void SMILESWriterTest::testRoundTrip11() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"[NH4+].[Cl-].F.[He-2]");
    std::vector<Atom *> atomList = f->getAtomList();
    std::reverse(atomList.begin(), atomList.end());
    f->reorderAtomCollection(atomList);
    fm->makeHydrogensExplicit();
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    assertEquals(L"[He-2].F.[Cl-].[NH4+]", smiles);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRoundTrip12() throws StructureBuildingException
void SMILESWriterTest::testRoundTrip12() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"CCO.N=O.C#N");
    fm->makeHydrogensExplicit();
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    assertEquals(L"CCO.N=O.C#N", smiles);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testOrganic1() throws StructureBuildingException
void SMILESWriterTest::testOrganic1() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"[S]");
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    assertEquals(L"[S]", smiles);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testOrganic2() throws StructureBuildingException
void SMILESWriterTest::testOrganic2() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"[S][H]");
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    assertEquals(L"[SH]", smiles);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testOrganic3() throws StructureBuildingException
void SMILESWriterTest::testOrganic3() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"[S]([H])[H]");
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    assertEquals(L"S", smiles);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testOrganic4() throws StructureBuildingException
void SMILESWriterTest::testOrganic4() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"[S]([H])([H])[H]");
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    assertEquals(L"[SH3]", smiles);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testOrganic5() throws StructureBuildingException
void SMILESWriterTest::testOrganic5() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"[S]([H])([H])([H])[H]");
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    assertEquals(L"[SH4]", smiles);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testOrganic6() throws StructureBuildingException
void SMILESWriterTest::testOrganic6() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"S(F)(F)(F)F");
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    assertEquals(L"S(F)(F)(F)F", smiles);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testOrganic7() throws StructureBuildingException
void SMILESWriterTest::testOrganic7() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"S([H])(F)(F)(F)(F)F");
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    assertEquals(L"S(F)(F)(F)(F)F", smiles);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testOrganic8() throws StructureBuildingException
void SMILESWriterTest::testOrganic8() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"S([H])([H])(F)(F)(F)F");
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    assertEquals(L"[SH2](F)(F)(F)F", smiles);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testOrganic9() throws StructureBuildingException
void SMILESWriterTest::testOrganic9() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"S(F)(F)(F)(F)(F)(F)F");
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    assertEquals(L"S(F)(F)(F)(F)(F)(F)F", smiles);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testOrganic10() throws StructureBuildingException
void SMILESWriterTest::testOrganic10() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"[I]([H])([H])[H]");
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    assertEquals(L"[IH3]", smiles);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCharged1() throws StructureBuildingException
void SMILESWriterTest::testCharged1() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"[CH3+]");
    fm->makeHydrogensExplicit();
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    assertEquals(L"[CH3+]", smiles);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCharged2() throws StructureBuildingException
void SMILESWriterTest::testCharged2() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"[Mg+2]");
    fm->makeHydrogensExplicit();
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    assertEquals(L"[Mg+2]", smiles);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCharged3() throws StructureBuildingException
void SMILESWriterTest::testCharged3() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"[BH4-]");
    fm->makeHydrogensExplicit();
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    assertEquals(L"[BH4-]", smiles);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCharged4() throws StructureBuildingException
void SMILESWriterTest::testCharged4() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"[O-2]");
    fm->makeHydrogensExplicit();
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    assertEquals(L"[O-2]", smiles);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testIsotope() throws StructureBuildingException
void SMILESWriterTest::testIsotope() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"[15NH3]");
    fm->makeHydrogensExplicit();
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    assertEquals(L"[15NH3]", smiles);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRGroup1() throws StructureBuildingException
void SMILESWriterTest::testRGroup1() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"[R]CC[R]");
    fm->makeHydrogensExplicit();
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    assertEquals(L"*CC*", smiles);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRGroup2() throws StructureBuildingException
void SMILESWriterTest::testRGroup2() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"[H][R]");
    fm->makeHydrogensExplicit();
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    assertEquals(L"[H]*", smiles);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRingOpeningsGreaterThan10() throws StructureBuildingException
void SMILESWriterTest::testRingOpeningsGreaterThan10() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(
            L"C12=C3C4=C5C6=C1C7=C8C9=C1C%10=C%11C(=C29)C3=C2C3=C4C4=C5C5=C9C6=C7C6=C7C8=C1C1=C8C%10=C%10C%11=C2C2=C3C3=C4C4=C5C5=C%11C%12=C(C6=C95)C7=C1C1=C%12C5=C%11C4=C3C3=C5C(=C81)C%10=C23");
    fm->makeHydrogensExplicit();
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    assertEquals(
            L"C12=C3C4=C5C6=C1C1=C7C8=C9C%10=C%11C(=C28)C3=C3C2=C4C4=C5C5=C8C6=C1C1=C6C7=C9C9=C7C%10=C%10C%11=C3C3=C2C2=C4C4=C5C5=C%11C%12=C(C1=C85)C6=C9C9=C%12C%12=C%11C4=C2C2=C%12C(=C79)C%10=C32",
            smiles);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testHydrogenNotBondedToAnyNonHydrogen1() throws StructureBuildingException
void SMILESWriterTest::testHydrogenNotBondedToAnyNonHydrogen1() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"[H-].[H+]");
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    assertEquals(L"[H-].[H+]", smiles);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testHydrogenNotBondedToAnyNonHydrogen2() throws StructureBuildingException
void SMILESWriterTest::testHydrogenNotBondedToAnyNonHydrogen2() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"[H][H]");
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    assertEquals(L"[H][H]", smiles);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testHydrogenNotBondedToAnyNonHydrogen3() throws StructureBuildingException
void SMILESWriterTest::testHydrogenNotBondedToAnyNonHydrogen3() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"[2H][H]");
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    assertEquals(L"[2H][H]", smiles);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testHydrogenNotBondedToAnyNonHydrogen4() throws StructureBuildingException
void SMILESWriterTest::testHydrogenNotBondedToAnyNonHydrogen4() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"[H]B1[H]B([H])[H]1");
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    assertEquals(L"B1[H]B[H]1", smiles);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testTetrahedralChirality1() throws StructureBuildingException
void SMILESWriterTest::testTetrahedralChirality1() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"N[C@@H](F)C");
    fm->makeHydrogensExplicit();
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    assertEquals(L"N[C@@H](F)C", smiles);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testTetrahedralChirality2() throws StructureBuildingException
void SMILESWriterTest::testTetrahedralChirality2() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"N[C@H](F)C");
    fm->makeHydrogensExplicit();
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    assertEquals(L"N[C@H](F)C", smiles);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testTetrahedralChirality3() throws StructureBuildingException
void SMILESWriterTest::testTetrahedralChirality3() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"C2.N1.F3.[C@@H]231");
    fm->makeHydrogensExplicit();
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    assertEquals(L"C[C@H](F)N", smiles);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testTetrahedralChirality4() throws StructureBuildingException
void SMILESWriterTest::testTetrahedralChirality4() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"[C@@H]231.C2.N1.F3");
    fm->makeHydrogensExplicit();
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    assertEquals(L"[C@H](C)(N)F", smiles);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testTetrahedralChirality5() throws StructureBuildingException
void SMILESWriterTest::testTetrahedralChirality5() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"[C@@H](Cl)1[C@H](C)(F).Br1");
    fm->makeHydrogensExplicit();
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    assertEquals(L"[C@H](Cl)([C@H](C)F)Br", smiles);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testTetrahedralChirality6() throws StructureBuildingException
void SMILESWriterTest::testTetrahedralChirality6() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"I[C@@](Cl)(Br)F");
    fm->makeHydrogensExplicit();
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    assertEquals(L"I[C@@](Cl)(Br)F", smiles);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testTetrahedralChirality7() throws StructureBuildingException
void SMILESWriterTest::testTetrahedralChirality7() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"C[S@](N)=O");
    fm->makeHydrogensExplicit();
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    assertEquals(L"C[S@](N)=O", smiles);
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondSupport1() throws StructureBuildingException
void SMILESWriterTest::testDoubleBondSupport1() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"C/C=C/C");
    fm->makeHydrogensExplicit();
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    if (smiles != L"C/C=C/C" && smiles != L"C\\C=C\\C") {
        fail(smiles + std::wstring(L" did not correspond to one of the expected SMILES strings"));
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondSupport2() throws StructureBuildingException
void SMILESWriterTest::testDoubleBondSupport2() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"C/C=C\\C");
    fm->makeHydrogensExplicit();
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    if (smiles != L"C/C=C\\C" && smiles != L"C\\C=C/C") {
        fail(smiles + std::wstring(L" did not correspond to one of the expected SMILES strings"));
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondSupport3() throws StructureBuildingException
void SMILESWriterTest::testDoubleBondSupport3() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"C/C=C\\C=C/C");
    fm->makeHydrogensExplicit();
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    if (smiles != L"C/C=C\\C=C/C" && smiles != L"C\\C=C/C=C\\C") {
        fail(smiles + std::wstring(L" did not correspond to one of the expected SMILES strings"));
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondSupport4() throws StructureBuildingException
void SMILESWriterTest::testDoubleBondSupport4() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"ClC(C(=O)[O-])=CC(=CC(=O)[O-])Cl");
    fm->makeHydrogensExplicit();
    f->findBond(2, 6)->setBondStereoElement(
            std::vector<Atom *>{f->getAtomByID(1), f->getAtomByID(2), f->getAtomByID(6), f->getAtomByID(7)},
            BondStereoValue::TRANS);
    f->findBond(7, 8)->setBondStereoElement(
            std::vector<Atom *>{f->getAtomByID(12), f->getAtomByID(7), f->getAtomByID(8), f->getAtomByID(9)},
            BondStereoValue::TRANS);
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    if (smiles != L"Cl\\C(\\C(=O)[O-])=C\\C(=C/C(=O)[O-])\\Cl" && smiles != L"Cl/C(/C(=O)[O-])=C/C(=C\\C(=O)[O-])/Cl") {
        fail(smiles + std::wstring(L" did not correspond to one of the expected SMILES strings"));
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondSupport5() throws StructureBuildingException
void SMILESWriterTest::testDoubleBondSupport5() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"C/C=N\\O");
    fm->makeHydrogensExplicit();
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    if (smiles != L"C/C=N\\O" && smiles != L"C\\C=N/O") {
        fail(smiles + std::wstring(L" did not correspond to one of the expected SMILES strings"));
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondSupport6() throws StructureBuildingException
void SMILESWriterTest::testDoubleBondSupport6() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"O=C(/C=C(C(O)=O)\\C=C/C(O)=O)O");
    fm->makeHydrogensExplicit();
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    if (smiles != L"O=C(/C=C(/C(O)=O)\\C=C/C(O)=O)O" && smiles != L"O=C(\\C=C(\\C(O)=O)/C=C\\C(O)=O)O") {
        fail(smiles + std::wstring(L" did not correspond to one of the expected SMILES strings"));
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondSupport7() throws StructureBuildingException
void SMILESWriterTest::testDoubleBondSupport7() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"C(=C(C=CC(=O)O)C(=O)O)C(=O)O");
    fm->makeHydrogensExplicit();
    f->findBond(1, 2)->setBondStereoElement(
            std::vector<Atom *>{f->getAtomByID(11), f->getAtomByID(1), f->getAtomByID(2), f->getAtomByID(8)},
            BondStereoValue::TRANS);
    f->findBond(3, 4)->setBondStereoElement(
            std::vector<Atom *>{f->getAtomByID(2), f->getAtomByID(3), f->getAtomByID(4), f->getAtomByID(5)},
            BondStereoValue::TRANS);
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    if (smiles != L"C(=C(/C=C/C(=O)O)\\C(=O)O)/C(=O)O" && smiles != L"C(=C(\\C=C\\C(=O)O)/C(=O)O)\\C(=O)O") {
        fail(smiles + std::wstring(L" did not correspond to one of the expected SMILES strings"));
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDoubleBondSupport8() throws StructureBuildingException
void SMILESWriterTest::testDoubleBondSupport8() throw(StructureBuildingException) {
    //hydrogen on the nitrogen must be explicit!
    Fragment *f = fm->buildSMILES(L"[H]/N=C(\\N)/O");
    fm->makeHydrogensExplicit();
    std::wstring smiles = SMILESWriter::generateSmiles(f);
    if (smiles != L"[H]/N=C(\\N)/O" && smiles != L"[H]\\N=C(/N)\\O") {
        fail(smiles + std::wstring(L" did not correspond to one of the expected SMILES strings"));
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testLabelling1() throws StructureBuildingException
void SMILESWriterTest::testLabelling1() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"CCC", L"", XmlDeclarations::NONE_LABELS_VAL);
    for (auto a : f->getAtomList()) {
        assertEquals(0, a->getLocants().size());
    }

    Fragment *f2 = fm->buildSMILES(L"CCC", L"", L"");
    for (auto a : f2->getAtomList()) {
        assertEquals(0, a->getLocants().size());
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testLabelling2() throws StructureBuildingException
void SMILESWriterTest::testLabelling2() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"CCC", L"", L"1/2,alpha,2'/");
    std::vector<Atom *> atoms = f->getAtomList();
    assertEquals(1, atoms[0]->getLocants()->size());
    assertEquals(3, atoms[1]->getLocants()->size());
    assertEquals(0, atoms[2]->getLocants()->size());

    assertEquals(L"1", atoms[0]->getLocants()[0]);
    assertEquals(L"2", atoms[1]->getLocants()[0]);
    assertEquals(L"alpha", atoms[1]->getLocants()[1]);
    assertEquals(L"2'", atoms[1]->getLocants()[2]);
}
