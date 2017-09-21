#include "StereochemistryTest.h"
#include "opsin-core/FragmentManager.h"
#include "opsin-core/IDManager.h"
#include "opsin-core/SMILESFragmentBuilder.h"
#include "opsin-core/NameToStructure.h"
#include "opsin-core/Fragment.h"
#include "opsin-core/StereoAnalyser.h"
#include "opsin-core/Atom.h"
#include "opsin-core/ChemEl.h"
#include "opsin-core/CipOrderingException.h"
#include "opsin-core/Bond.h"
#include "opsin-core/StructureBuildingException.h"
#include "opsin-core/BondStereo.h"
#include "opsin-core/CipSequenceRules.h"
#include "opsin-core/StereochemistryHandler.h"


//						import static org.junit.Assert.*;
//						import static org.mockito.Mockito.mock;
using org::junit::AfterClass;
using org::junit::Before;
using org::junit::BeforeClass;
using org::junit::Test;
using BondStereoValue = BondStereo::BondStereoValue;
using StereoBond = StereoAnalyser::StereoBond;
using StereoCentre = StereoAnalyser::StereoCentre;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void setup()
void StereochemistryTest::setup() {
    IDManager *idManager = new IDManager();
    SMILESFragmentBuilder tempVar(idManager);
    fm = new FragmentManager(&tempVar, idManager);
}

NameToStructure *StereochemistryTest::n2s;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @BeforeClass public static void intialSetup()
void StereochemistryTest::intialSetup() {
    n2s = NameToStructure::getInstance();
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @AfterClass public static void cleanUp()
void StereochemistryTest::cleanUp() {
    n2s = nullptr;
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void findStereoCentresBromoChloroFluoroMethane()
void StereochemistryTest::findStereoCentresBromoChloroFluoroMethane() {
    Fragment *f = n2s->parseChemicalName(L"bromochlorofluoromethane")->getStructure();
    StereoAnalyser *stereoAnalyser = new StereoAnalyser(f);
    assertEquals(1, stereoAnalyser->findStereoCentres().size());
    assertEquals(0, stereoAnalyser->findStereoBonds().size());
    StereoCentre *sc = stereoAnalyser->findStereoCentres()[0];
    assertNotNull(sc->getStereoAtom());
    Atom *stereoAtom = sc->getStereoAtom();
    assertEquals(ChemEl::C, stereoAtom->getElement());
    assertEquals(4, stereoAtom->getID());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void findStereoCentresNacetylleucine() throws CipOrderingException
void StereochemistryTest::findStereoCentresNacetylleucine() throw(CipOrderingException) {
    Fragment *f = n2s->parseChemicalName(L"N-acetylleucine")->getStructure();
    StereoAnalyser *stereoAnalyser = new StereoAnalyser(f);
    assertEquals(1, stereoAnalyser->findStereoCentres().size());
    assertEquals(0, stereoAnalyser->findStereoBonds().size());
    StereoCentre *sc = stereoAnalyser->findStereoCentres()[0];
    assertNotNull(sc->getStereoAtom());
    Atom *stereoAtom = sc->getStereoAtom();
    assertEquals(ChemEl::C, stereoAtom->getElement());
    std::vector<Atom *> neighbours = sc->getCipOrderedAtoms();
    for (int i = 0; i < neighbours.size(); i++) {
        Atom *a = neighbours[i];
        if (i == 0) {
            assertEquals(ChemEl::H, a->getElement());
        } else if (i == 1) {
            assertEquals(ChemEl::C, a->getElement());
        } else if (i == 2) {
            assertEquals(ChemEl::C, a->getElement());
        } else if (i == 3) {
            assertEquals(ChemEl::N, a->getElement());
        }
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void findStereoCentresBut2ene()
void StereochemistryTest::findStereoCentresBut2ene() {
    Fragment *f = n2s->parseChemicalName(L"but-2-ene")->getStructure();
    StereoAnalyser *stereoAnalyser = new StereoAnalyser(f);
    assertEquals(0, stereoAnalyser->findStereoCentres().size());
    assertEquals(1, stereoAnalyser->findStereoBonds().size());
    StereoBond *sb = stereoAnalyser->findStereoBonds()[0];
    Bond *stereoBond = sb->getBond();
    assertNotNull(stereoBond);
    Atom *stereoAtom1 = stereoBond->getFromAtom();
    Atom *stereoAtom2 = stereoBond->getToAtom();
    assertNotNull(stereoAtom1);
    assertNotNull(stereoAtom2);
    assertNotSame(stereoAtom1, stereoAtom2);
    if (stereoAtom1->getID() == 2) {
        assertEquals(3, stereoAtom2->getID());
    } else {
        assertEquals(2, stereoAtom2->getID());
        assertEquals(3, stereoAtom1->getID());
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void applyStereochemistryLocantedZ() throws StructureBuildingException
void StereochemistryTest::applyStereochemistryLocantedZ() throw(StructureBuildingException) {
    Fragment *f = n2s->parseChemicalName(L"(2Z)-but-2-ene")->getStructure();
    Atom *atom2 = f->getAtomByLocant(L"2");
    Atom *atom3 = f->getAtomByLocant(L"3");
    assertNotNull(atom2);
    assertNotNull(atom3);
    Bond *chiralBond = atom2->getBondToAtom(atom3);
    assertNotNull(chiralBond);
    BondStereo *bondStereo = chiralBond->getBondStereo();
    assertNotNull(bondStereo);
    assertEquals(L"1 2 3 4", atomRefsToIdStr(bondStereo->getAtomRefs4()));
    assertEquals(BondStereoValue::CIS, bondStereo->getBondStereoValue());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void applyStereochemistryLocantedE() throws StructureBuildingException
void StereochemistryTest::applyStereochemistryLocantedE() throw(StructureBuildingException) {
    Fragment *f = n2s->parseChemicalName(L"(2E)-but-2-ene")->getStructure();
    Atom *atom2 = f->getAtomByLocant(L"2");
    Atom *atom3 = f->getAtomByLocant(L"3");
    assertNotNull(atom2);
    assertNotNull(atom3);
    Bond *chiralBond = atom2->getBondToAtom(atom3);
    assertNotNull(chiralBond);
    BondStereo *bondStereo = chiralBond->getBondStereo();
    assertNotNull(bondStereo);
    assertEquals(L"1 2 3 4", atomRefsToIdStr(bondStereo->getAtomRefs4()));
    assertEquals(BondStereoValue::TRANS, bondStereo->getBondStereoValue());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void applyStereochemistryUnlocantedZ() throws StructureBuildingException
void StereochemistryTest::applyStereochemistryUnlocantedZ() throw(StructureBuildingException) {
    Fragment *f = n2s->parseChemicalName(L"(Z)-but-2-ene")->getStructure();
    Atom *atom2 = f->getAtomByLocant(L"2");
    Atom *atom3 = f->getAtomByLocant(L"3");
    assertNotNull(atom2);
    assertNotNull(atom3);
    Bond *chiralBond = atom2->getBondToAtom(atom3);
    assertNotNull(chiralBond);
    BondStereo *bondStereo = chiralBond->getBondStereo();
    assertNotNull(bondStereo);
    assertEquals(L"1 2 3 4", atomRefsToIdStr(bondStereo->getAtomRefs4()));
    assertEquals(BondStereoValue::CIS, bondStereo->getBondStereoValue());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void applyStereochemistryUnlocantedE() throws StructureBuildingException
void StereochemistryTest::applyStereochemistryUnlocantedE() throw(StructureBuildingException) {
    Fragment *f = n2s->parseChemicalName(L"(E)-but-2-ene")->getStructure();
    Atom *atom2 = f->getAtomByLocant(L"2");
    Atom *atom3 = f->getAtomByLocant(L"3");
    assertNotNull(atom2);
    assertNotNull(atom3);
    Bond *chiralBond = atom2->getBondToAtom(atom3);
    assertNotNull(chiralBond);
    BondStereo *bondStereo = chiralBond->getBondStereo();
    assertNotNull(bondStereo);
    assertEquals(L"1 2 3 4", atomRefsToIdStr(bondStereo->getAtomRefs4()));
    assertEquals(BondStereoValue::TRANS, bondStereo->getBondStereoValue());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void applyStereochemistryCis() throws StructureBuildingException
void StereochemistryTest::applyStereochemistryCis() throw(StructureBuildingException) {
    Fragment *f = n2s->parseChemicalName(L"cis-but-2-ene")->getStructure();
    Atom *atom2 = f->getAtomByLocant(L"2");
    Atom *atom3 = f->getAtomByLocant(L"3");
    assertNotNull(atom2);
    assertNotNull(atom3);
    Bond *chiralBond = atom2->getBondToAtom(atom3);
    assertNotNull(chiralBond);
    BondStereo *bondStereo = chiralBond->getBondStereo();
    assertNotNull(bondStereo);
    assertEquals(L"1 2 3 4", atomRefsToIdStr(bondStereo->getAtomRefs4()));
    assertEquals(BondStereoValue::CIS, bondStereo->getBondStereoValue());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void applyStereochemistryTrans() throws StructureBuildingException
void StereochemistryTest::applyStereochemistryTrans() throw(StructureBuildingException) {
    Fragment *f = n2s->parseChemicalName(L"trans-but-2-ene")->getStructure();
    Atom *atom2 = f->getAtomByLocant(L"2");
    Atom *atom3 = f->getAtomByLocant(L"3");
    assertNotNull(atom2);
    assertNotNull(atom3);
    Bond *chiralBond = atom2->getBondToAtom(atom3);
    assertNotNull(chiralBond);
    BondStereo *bondStereo = chiralBond->getBondStereo();
    assertNotNull(bondStereo);
    assertEquals(L"1 2 3 4", atomRefsToIdStr(bondStereo->getAtomRefs4()));
    assertEquals(BondStereoValue::TRANS, bondStereo->getBondStereoValue());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void applyStereochemistryLocantedRS() throws StructureBuildingException
void StereochemistryTest::applyStereochemistryLocantedRS() throw(StructureBuildingException) {
    Fragment *f = n2s->parseChemicalName(L"(1S,2R)-2-(methylamino)-1-phenylpropan-1-ol")->getStructure();
    std::vector<Atom *> atomList = f->getAtomList();
    std::vector<Atom *> stereoAtoms;
    for (auto atom : atomList) {
        if (atom->getAtomParity() != nullptr) {
            stereoAtoms.push_back(atom);
        }
    }
    assertEquals(2, stereoAtoms.size());
    StereoAnalyser *stereoAnalyser = new StereoAnalyser(f);
    std::vector<StereoCentre *> stereoCentres = stereoAnalyser->findStereoCentres();
    assertEquals(2, stereoCentres.size());
    if (stereoCentres[0]->getStereoAtom().equals(stereoAtoms[0])) {
        assertEquals(stereoCentres[1]->getStereoAtom(), stereoAtoms[1]);
    } else {
        assertEquals(stereoCentres[0]->getStereoAtom(), stereoAtoms[1]);
        assertEquals(stereoCentres[1]->getStereoAtom(), stereoAtoms[0]);
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCIPpriority1() throws StructureBuildingException
void StereochemistryTest::testCIPpriority1() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"C(Br)(F)([H])Cl");
    CipSequenceRules tempVar(f->getFirstAtom());
    std::vector<Atom *> cipOrdered = (&tempVar)->getNeighbouringAtomsInCipOrder();
    for (int i = 0; i < cipOrdered.size(); i++) {
        Atom *a = cipOrdered[i];
        if (i == 0) {
            assertEquals(ChemEl::H, a->getElement());
        } else if (i == 1) {
            assertEquals(ChemEl::F, a->getElement());
        } else if (i == 2) {
            assertEquals(ChemEl::Cl, a->getElement());
        } else if (i == 3) {
            assertEquals(ChemEl::Br, a->getElement());
        }
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCIPpriority2() throws StructureBuildingException
void StereochemistryTest::testCIPpriority2() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"C([H])(C1CC1)(C1CCC1)O");
    CipSequenceRules tempVar(f->getFirstAtom());
    std::vector<Atom *> cipOrdered = (&tempVar)->getNeighbouringAtomsInCipOrder();
    for (int i = 0; i < cipOrdered.size(); i++) {
        Atom *a = cipOrdered[i];
        if (i == 0) {
            assertEquals(ChemEl::H, a->getElement());
        } else if (i == 1) {
            assertEquals(3, a->getID());
        } else if (i == 2) {
            assertEquals(6, a->getID());
        } else if (i == 3) {
            assertEquals(ChemEl::O, a->getElement());
        }
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCIPpriority3() throws StructureBuildingException
void StereochemistryTest::testCIPpriority3() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"[C](N)(C1=CC(O)=CC=C1)([H])C2=CC=C(O)C=C2");
    CipSequenceRules tempVar(f->getFirstAtom());
    std::vector<Atom *> cipOrdered = (&tempVar)->getNeighbouringAtomsInCipOrder();
    for (int i = 0; i < cipOrdered.size(); i++) {
        Atom *a = cipOrdered[i];
        if (i == 0) {
            assertEquals(ChemEl::H, a->getElement());
        } else if (i == 1) {
            assertEquals(11, a->getID());
        } else if (i == 2) {
            assertEquals(3, a->getID());
        } else if (i == 3) {
            assertEquals(ChemEl::N, a->getElement());
        }
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCIPpriority4() throws StructureBuildingException
void StereochemistryTest::testCIPpriority4() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"[C](N)(C1CC(O)CCC1)([H])C2CCC(O)CC2");
    CipSequenceRules tempVar(f->getFirstAtom());
    std::vector<Atom *> cipOrdered = (&tempVar)->getNeighbouringAtomsInCipOrder();
    for (int i = 0; i < cipOrdered.size(); i++) {
        Atom *a = cipOrdered[i];
        if (i == 0) {
            assertEquals(ChemEl::H, a->getElement());
        } else if (i == 1) {
            assertEquals(11, a->getID());
        } else if (i == 2) {
            assertEquals(3, a->getID());
        } else if (i == 3) {
            assertEquals(ChemEl::N, a->getElement());
        }
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCIPpriority5() throws StructureBuildingException
void StereochemistryTest::testCIPpriority5() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"C1([H])(C(=O)O[H])C([H])([H])SC([H])([H])N([H])1");
    CipSequenceRules tempVar(f->getFirstAtom());
    std::vector<Atom *> cipOrdered = (&tempVar)->getNeighbouringAtomsInCipOrder();
    for (int i = 0; i < cipOrdered.size(); i++) {
        Atom *a = cipOrdered[i];
        if (i == 0) {
            assertEquals(ChemEl::H, a->getElement());
        } else if (i == 1) {
            assertEquals(3, a->getID());
        } else if (i == 2) {
            assertEquals(7, a->getID());
        } else if (i == 3) {
            assertEquals(ChemEl::N, a->getElement());
        }
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCIPpriority6() throws StructureBuildingException
void StereochemistryTest::testCIPpriority6() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"C1([H])(O)C([H])(C([H])([H])[H])OC([H])([H])C([H])([H])C1([H])(O[H])");
    CipSequenceRules tempVar(f->getFirstAtom());
    std::vector<Atom *> cipOrdered = (&tempVar)->getNeighbouringAtomsInCipOrder();
    for (int i = 0; i < cipOrdered.size(); i++) {
        Atom *a = cipOrdered[i];
        if (i == 0) {
            assertEquals(ChemEl::H, a->getElement());
        } else if (i == 1) {
            assertEquals(17, a->getID());
        } else if (i == 2) {
            assertEquals(4, a->getID());
        } else if (i == 3) {
            assertEquals(ChemEl::O, a->getElement());
        }
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCIPpriority7() throws StructureBuildingException
void StereochemistryTest::testCIPpriority7() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(
            L"[H]OC2([H])(C([H])([H])C([H])([H])C3([H])(C4([H])(C([H])([H])C([H])([H])C1=C([H])C([H])([H])C([H])([H])C([H])([H])C1([H])C4([H])(C([H])([H])C([H])([H])C23(C([H])([H])[H])))))");
    CipSequenceRules tempVar(f->getAtomList()[34]);
    std::vector<Atom *> cipOrdered = (&tempVar)->getNeighbouringAtomsInCipOrder();
    for (int i = 0; i < cipOrdered.size(); i++) {
        Atom *a = cipOrdered[i];
        if (i == 0) {
            assertEquals(ChemEl::H, a->getElement());
        } else if (i == 1) {
            assertEquals(37, a->getID());
        } else if (i == 2) {
            assertEquals(13, a->getID());
        } else if (i == 3) {
            assertEquals(33, a->getID());
        }
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCIPpriority8() throws StructureBuildingException
void StereochemistryTest::testCIPpriority8() throw(StructureBuildingException) {
    Fragment *f = n2s->parseChemicalName(
            L"(6aR)-6-phenyl-6,6a-dihydroisoindolo[2,1-a]quinazoline-5,11-dione")->getStructure();
    CipSequenceRules tempVar(f->getAtomByLocant(L"6a"));
    std::vector<Atom *> cipOrdered = (&tempVar)->getNeighbouringAtomsInCipOrder();
    for (int i = 0; i < cipOrdered.size(); i++) {
        Atom *a = cipOrdered[i];
        if (i == 0) {
            assertEquals(ChemEl::H, a->getElement());
        } else if (i == 1) {
            assertEquals(ChemEl::C, a->getElement());
        } else if (i == 2) {
            assertEquals(L"6", a->getFirstLocant());
        } else if (i == 3) {
            assertEquals(L"12", a->getFirstLocant());
        }
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCIPpriority9() throws StructureBuildingException
void StereochemistryTest::testCIPpriority9() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"C1(C=C)CC1C2=CC=CC=C2");
    fm->makeHydrogensExplicit();
    CipSequenceRules tempVar(f->getFirstAtom());
    std::vector<Atom *> cipOrdered = (&tempVar)->getNeighbouringAtomsInCipOrder();
    for (int i = 0; i < cipOrdered.size(); i++) {
        Atom *a = cipOrdered[i];
        if (i == 0) {
            assertEquals(ChemEl::H, a->getElement());
        } else if (i == 1) {
            assertEquals(4, a->getID());
        } else if (i == 2) {
            assertEquals(2, a->getID());
        } else if (i == 3) {
            assertEquals(5, a->getID());
        }
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCIPpriority10() throws StructureBuildingException
void StereochemistryTest::testCIPpriority10() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(
            L"C(O[H])([H])(C1([H])C([H])(F)C([H])(Cl)C([H])([H])C([H])(I)C1([H])([H]))C1([H])C([H])(F)C([H])(Br)C([H])([H])C([H])(Cl)C1([H])([H])");
    fm->makeHydrogensExplicit();
    CipSequenceRules tempVar(f->getFirstAtom());
    std::vector<Atom *> cipOrdered = (&tempVar)->getNeighbouringAtomsInCipOrder();
    for (int i = 0; i < cipOrdered.size(); i++) {
        Atom *a = cipOrdered[i];
        if (i == 0) {
            assertEquals(ChemEl::H, a->getElement());
        } else if (i == 1) {
            assertEquals(5, a->getID());
        } else if (i == 2) {
            assertEquals(22, a->getID());
        } else if (i == 3) {
            assertEquals(ChemEl::O, a->getElement());
        }
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCIPpriority11() throws StructureBuildingException
void StereochemistryTest::testCIPpriority11() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"C17C=CC23C45OC6C19.O74.O2C3.C5.C6(C)C.C9");
    fm->makeHydrogensExplicit();
    //stereocentres at 1,4,5,7,8
    std::vector<Atom *> atomList = f->getAtomList();
    CipSequenceRules tempVar(atomList[0]);
    std::vector<Atom *> cipOrdered = (&tempVar)->getNeighbouringAtomsInCipOrder();
    for (int i = 0; i < cipOrdered.size(); i++) {
        Atom *a = cipOrdered[i];
        if (i == 0) {
            assertEquals(ChemEl::H, a->getElement());
        } else if (i == 1) {
            assertEquals(2, a->getID());
        } else if (i == 2) {
            assertEquals(8, a->getID());
        } else if (i == 3) {
            assertEquals(ChemEl::O, a->getElement());
        }
    }
    CipSequenceRules tempVar2(atomList[3]);
    cipOrdered = (&tempVar2)->getNeighbouringAtomsInCipOrder();
    for (int i = 0; i < cipOrdered.size(); i++) {
        Atom *a = cipOrdered[i];
        if (i == 0) {
            assertEquals(3, a->getID());
        } else if (i == 1) {
            assertEquals(11, a->getID());
        } else if (i == 2) {
            assertEquals(5, a->getID());
        } else if (i == 3) {
            assertEquals(ChemEl::O, a->getElement());
        }
    }
    CipSequenceRules tempVar3(atomList[4]);
    cipOrdered = (&tempVar3)->getNeighbouringAtomsInCipOrder();
    for (int i = 0; i < cipOrdered.size(); i++) {
        Atom *a = cipOrdered[i];
        if (i == 0) {
            assertEquals(12, a->getID());
        } else if (i == 1) {
            assertEquals(4, a->getID());
        } else if (i == 2) {
            assertEquals(6, a->getID());
        } else if (i == 3) {
            assertEquals(9, a->getID());
        }
    }
    CipSequenceRules tempVar4(atomList[6]);
    cipOrdered = (&tempVar4)->getNeighbouringAtomsInCipOrder();
    for (int i = 0; i < cipOrdered.size(); i++) {
        Atom *a = cipOrdered[i];
        if (i == 0) {
            assertEquals(ChemEl::H, a->getElement());
        } else if (i == 1) {
            assertEquals(13, a->getID());
        } else if (i == 2) {
            assertEquals(8, a->getID());
        } else if (i == 3) {
            assertEquals(ChemEl::O, a->getElement());
        }
    }
    CipSequenceRules tempVar5(atomList[7]);
    cipOrdered = (&tempVar5)->getNeighbouringAtomsInCipOrder();
    for (int i = 0; i < cipOrdered.size(); i++) {
        Atom *a = cipOrdered[i];
        if (i == 0) {
            assertEquals(ChemEl::H, a->getElement());
        } else if (i == 1) {
            assertEquals(16, a->getID());
        } else if (i == 2) {
            assertEquals(7, a->getID());
        } else if (i == 3) {
            assertEquals(1, a->getID());
        }
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCIPpriority12() throws StructureBuildingException
void StereochemistryTest::testCIPpriority12() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"C1(C)(CCC(=O)N1)CCC(=O)NC(C)C");
    fm->makeHydrogensExplicit();
    CipSequenceRules tempVar(f->getFirstAtom());
    std::vector<Atom *> cipOrdered = (&tempVar)->getNeighbouringAtomsInCipOrder();
    for (int i = 0; i < cipOrdered.size(); i++) {
        Atom *a = cipOrdered[i];
        if (i == 0) {
            assertEquals(2, a->getID());
        } else if (i == 1) {
            assertEquals(3, a->getID());
        } else if (i == 2) {
            assertEquals(8, a->getID());
        } else if (i == 3) {
            assertEquals(ChemEl::N, a->getElement());
        }
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCIPpriority13() throws StructureBuildingException
void StereochemistryTest::testCIPpriority13() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"C(O)(C#CC)C1=CC=CC=C1");
    fm->makeHydrogensExplicit();
    CipSequenceRules tempVar(f->getFirstAtom());
    std::vector<Atom *> cipOrdered = (&tempVar)->getNeighbouringAtomsInCipOrder();
    for (int i = 0; i < cipOrdered.size(); i++) {
        Atom *a = cipOrdered[i];
        if (i == 0) {
            assertEquals(ChemEl::H, a->getElement());
        } else if (i == 1) {
            assertEquals(6, a->getID());
        } else if (i == 2) {
            assertEquals(3, a->getID());
        } else if (i == 3) {
            assertEquals(2, a->getID());
        }
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCIPpriority14() throws StructureBuildingException
void StereochemistryTest::testCIPpriority14() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"C(Cl)([2H])([3H])[H]");
    CipSequenceRules tempVar(f->getFirstAtom());
    std::vector<Atom *> cipOrdered = (&tempVar)->getNeighbouringAtomsInCipOrder();
    for (int i = 0; i < cipOrdered.size(); i++) {
        Atom *a = cipOrdered[i];
        if (i == 0) {
            assertEquals(5, a->getID());
        } else if (i == 1) {
            assertEquals(3, a->getID());
        } else if (i == 2) {
            assertEquals(4, a->getID());
        } else if (i == 3) {
            assertEquals(2, a->getID());
        }
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCIPpriority15() throws StructureBuildingException
void StereochemistryTest::testCIPpriority15() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"C([H])(O)(C(C(F)CCl)CCBr)C(C(F)CF)CCI");
    fm->makeHydrogensExplicit();
    CipSequenceRules tempVar(f->getFirstAtom());
    std::vector<Atom *> cipOrdered = (&tempVar)->getNeighbouringAtomsInCipOrder();
    assertEquals(4, cipOrdered.size());
    assertEquals(2, cipOrdered[0]->getID());
    assertEquals(12, cipOrdered[1]->getID());
    assertEquals(4, cipOrdered[2]->getID());
    assertEquals(3, cipOrdered[3]->getID());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected=CipOrderingException.class) public void testCipUnassignable() throws StructureBuildingException
void StereochemistryTest::testCipUnassignable() throw(StructureBuildingException) {
    //two sides of ring are identical
    Fragment *f = fm->buildSMILES(L"NC1(O)CCC(CCC2CCCCC2)CC1");
    CipSequenceRules tempVar(f->getAtomList()[1]);
    (&tempVar)->getNeighbouringAtomsInCipOrder();
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAtomParityEquivalence1()
void StereochemistryTest::testAtomParityEquivalence1() {
    Atom * a1 = new Atom(1, ChemEl::C, mock(Fragment::
    typeid));
    Atom * a2 = new Atom(2, ChemEl::C, mock(Fragment::
    typeid));
    Atom * a3 = new Atom(3, ChemEl::C, mock(Fragment::
    typeid));
    Atom * a4 = new Atom(4, ChemEl::C, mock(Fragment::
    typeid));
    std::vector<Atom *> atomRefs1 = {a1, a2, a3, a4};
    std::vector<Atom *> atomRefs2 = {a3, a4, a1, a2};
    //2 swaps (4 by bubble sort)
    assertEquals(true, StereochemistryHandler::checkEquivalencyOfAtomsRefs4AndParity(atomRefs1, 1, atomRefs2, 1));
    assertEquals(false, StereochemistryHandler::checkEquivalencyOfAtomsRefs4AndParity(atomRefs1, 1, atomRefs2, -1));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAtomParityEquivalence2()
void StereochemistryTest::testAtomParityEquivalence2() {
    Atom * a1 = new Atom(1, ChemEl::C, mock(Fragment::
    typeid));
    Atom * a2 = new Atom(2, ChemEl::C, mock(Fragment::
    typeid));
    Atom * a3 = new Atom(3, ChemEl::C, mock(Fragment::
    typeid));
    Atom * a4 = new Atom(4, ChemEl::C, mock(Fragment::
    typeid));
    std::vector<Atom *> atomRefs1 = {a1, a2, a3, a4};
    std::vector<Atom *> atomRefs2 = {a2, a4, a1, a3};
    //3 swaps
    assertEquals(false, StereochemistryHandler::checkEquivalencyOfAtomsRefs4AndParity(atomRefs1, 1, atomRefs2, 1));
    assertEquals(true, StereochemistryHandler::checkEquivalencyOfAtomsRefs4AndParity(atomRefs1, 1, atomRefs2, -1));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCisTransUnambiguous() throws StructureBuildingException
void StereochemistryTest::testCisTransUnambiguous() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"[H]C([H])([H])C([H])=C([H])C([H])([H])[H]");
    assertEquals(true, StereochemistryHandler::cisTransUnambiguousOnBond(f->findBond(5, 7)));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCisTransAmbiguous() throws StructureBuildingException
void StereochemistryTest::testCisTransAmbiguous() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"[H]C([H])([H])C(Cl)=C([H])C([H])([H])[H]");
    assertEquals(false, StereochemistryHandler::cisTransUnambiguousOnBond(f->findBond(5, 7)));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testChiralAtomWhichBecomesAchiral() throws StructureBuildingException
void StereochemistryTest::testChiralAtomWhichBecomesAchiral() throw(StructureBuildingException) {
    Fragment *f = n2s->parseChemicalName(L"alpha-amino-alanine")->getStructure();
    StereoAnalyser *stereoAnalyser = new StereoAnalyser(f);
    assertEquals(0, stereoAnalyser->findStereoCentres().size());
    assertEquals(0, stereoAnalyser->findStereoBonds().size());
    Atom *formerChiralCentre = f->getAtomByLocantOrThrow(L"alpha");
    assertNull(L"This atom is no longer a chiral centre and hence should not have an associated atom parity",
               formerChiralCentre->getAtomParity());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testChiralBondWhichBecomesAchiral() throws StructureBuildingException
void StereochemistryTest::testChiralBondWhichBecomesAchiral() throw(StructureBuildingException) {
    Fragment *f = n2s->parseChemicalName(L"3-methylcrotonic acid")->getStructure();
    StereoAnalyser *stereoAnalyser = new StereoAnalyser(f);
    assertEquals(0, stereoAnalyser->findStereoCentres().size());
    assertEquals(0, stereoAnalyser->findStereoBonds().size());
    Bond *formerChiralBond = f->getAtomByLocantOrThrow(L"2")->getBondToAtomOrThrow(f->getAtomByLocantOrThrow(L"3"));
    assertNull(L"This Bond is no longer a chiral centre and hence should not have an associated bond stereo",
               formerChiralBond->getBondStereo());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testIsTetrahedral() throws StructureBuildingException
void StereochemistryTest::testIsTetrahedral() throw(StructureBuildingException) {
    assertEquals(true,
                 StereoAnalyser::isKnownPotentiallyStereogenic(fm->buildSMILES(L"C(N)(O)(Cl)Br")->getFirstAtom()));
    assertEquals(true,
                 StereoAnalyser::isKnownPotentiallyStereogenic(fm->buildSMILES(L"[Si](N)(O)(Cl)Br")->getFirstAtom()));
    assertEquals(true,
                 StereoAnalyser::isKnownPotentiallyStereogenic(fm->buildSMILES(L"[Ge](N)(O)(Cl)Br")->getFirstAtom()));
    assertEquals(true,
                 StereoAnalyser::isKnownPotentiallyStereogenic(fm->buildSMILES(L"[N+](N)(O)(Cl)Br")->getFirstAtom()));
    assertEquals(true,
                 StereoAnalyser::isKnownPotentiallyStereogenic(fm->buildSMILES(L"[P+](N)(O)(Cl)Br")->getFirstAtom()));
    assertEquals(true,
                 StereoAnalyser::isKnownPotentiallyStereogenic(fm->buildSMILES(L"[As+](N)(O)(Cl)Br")->getFirstAtom()));
    assertEquals(true,
                 StereoAnalyser::isKnownPotentiallyStereogenic(fm->buildSMILES(L"[B-](N)(O)(Cl)Br")->getFirstAtom()));
    assertEquals(true,
                 StereoAnalyser::isKnownPotentiallyStereogenic(fm->buildSMILES(L"[Sn](N)(O)(Cl)Br")->getFirstAtom()));
    assertEquals(true,
                 StereoAnalyser::isKnownPotentiallyStereogenic(fm->buildSMILES(L"[N](=N)(O)(Cl)Br")->getFirstAtom()));
    assertEquals(true,
                 StereoAnalyser::isKnownPotentiallyStereogenic(fm->buildSMILES(L"[P](=N)(O)(Cl)Br")->getFirstAtom()));
    assertEquals(true,
                 StereoAnalyser::isKnownPotentiallyStereogenic(fm->buildSMILES(L"[S](=N)(=O)(Cl)Br")->getFirstAtom()));
    assertEquals(true,
                 StereoAnalyser::isKnownPotentiallyStereogenic(fm->buildSMILES(L"[S+](=N)(O)(Cl)Br")->getFirstAtom()));
    assertEquals(true,
                 StereoAnalyser::isKnownPotentiallyStereogenic(fm->buildSMILES(L"[S](=O)(Cl)Br")->getFirstAtom()));
    assertEquals(true,
                 StereoAnalyser::isKnownPotentiallyStereogenic(fm->buildSMILES(L"[S+](O)(Cl)Br")->getFirstAtom()));
    assertEquals(true, StereoAnalyser::isKnownPotentiallyStereogenic(fm->buildSMILES(L"N1(C)(OS1)")->getFirstAtom()));
    assertEquals(true,
                 StereoAnalyser::isKnownPotentiallyStereogenic(fm->buildSMILES(L"[Se](=N)(=O)(Cl)Br")->getFirstAtom()));
    assertEquals(true,
                 StereoAnalyser::isKnownPotentiallyStereogenic(fm->buildSMILES(L"[Se+](=N)(O)(Cl)Br")->getFirstAtom()));
    assertEquals(true,
                 StereoAnalyser::isKnownPotentiallyStereogenic(fm->buildSMILES(L"[Se](=O)(Cl)Br")->getFirstAtom()));
    assertEquals(true,
                 StereoAnalyser::isKnownPotentiallyStereogenic(fm->buildSMILES(L"[Se+](O)(Cl)Br")->getFirstAtom()));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAchiralDueToResonance() throws StructureBuildingException
void StereochemistryTest::testAchiralDueToResonance() throw(StructureBuildingException) {
    assertEquals(true, StereoAnalyser::isAchiralDueToResonanceOrTautomerism(
            fm->buildSMILES(L"[S](=N)(=O)([O-])Br")->getFirstAtom()));
    assertEquals(true, StereoAnalyser::isAchiralDueToResonanceOrTautomerism(
            fm->buildSMILES(L"[S](=O)([O-])Br")->getFirstAtom()));
    assertEquals(false, StereoAnalyser::isAchiralDueToResonanceOrTautomerism(
            fm->buildSMILES(L"[S](=S)([O-])Br")->getFirstAtom()));
    assertEquals(false, StereoAnalyser::isAchiralDueToResonanceOrTautomerism(
            fm->buildSMILES(L"C(N)([O-])(Cl)Br")->getFirstAtom()));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAchiralDueToTautomerism() throws StructureBuildingException
void StereochemistryTest::testAchiralDueToTautomerism() throw(StructureBuildingException) {
    assertEquals(true, StereoAnalyser::isAchiralDueToResonanceOrTautomerism(
            fm->buildSMILES(L"[S](=N)(=O)([OH])Br")->getFirstAtom()));
    assertEquals(true, StereoAnalyser::isAchiralDueToResonanceOrTautomerism(
            fm->buildSMILES(L"[S](=O)([OH])Br")->getFirstAtom()));
    assertEquals(false, StereoAnalyser::isAchiralDueToResonanceOrTautomerism(
            fm->buildSMILES(L"[S](=S)([OH])Br")->getFirstAtom()));
    assertEquals(false, StereoAnalyser::isAchiralDueToResonanceOrTautomerism(
            fm->buildSMILES(L"C(N)([OH])(Cl)Br")->getFirstAtom()));
    assertEquals(true, StereoAnalyser::isAchiralDueToResonanceOrTautomerism(
            fm->buildSMILES(L"N([H])(CC)(C)")->getFirstAtom()));
    assertEquals(false,
                 StereoAnalyser::isAchiralDueToResonanceOrTautomerism(fm->buildSMILES(L"N1(C)(OS1)")->getFirstAtom()));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testFindPseudoAsymmetricCarbon1() throws StructureBuildingException
void StereochemistryTest::testFindPseudoAsymmetricCarbon1() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"OCC(O)C(O)C(O)CO");
    fm->makeHydrogensExplicit();
    StereoAnalyser *stereoAnalyser = new StereoAnalyser(f);
    std::vector<StereoCentre *> stereoCentres = stereoAnalyser->findStereoCentres();
    assertEquals(3, stereoCentres.size());
    for (int i = 0; i < stereoCentres.size(); i++) {
        StereoCentre *stereocentre = stereoCentres[i];
        if (i < 2) {
            assertEquals(true, stereocentre->isTrueStereoCentre());
        } else {
            assertEquals(false, stereocentre->isTrueStereoCentre());
            assertEquals(5, stereocentre->getStereoAtom()->getID());
        }
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testFindPseudoAsymmetricCarbon2() throws StructureBuildingException
void StereochemistryTest::testFindPseudoAsymmetricCarbon2() throw(StructureBuildingException) {
    Fragment *f = fm->buildSMILES(L"OCC(O)C(C(Cl)(Br)C)(C(Cl)(Br)C)C(O)CO");
    fm->makeHydrogensExplicit();
    StereoAnalyser *stereoAnalyser = new StereoAnalyser(f);
    std::vector<StereoCentre *> stereoCentres = stereoAnalyser->findStereoCentres();
    assertEquals(5, stereoCentres.size());
    for (int i = 0; i < stereoCentres.size(); i++) {
        StereoCentre *stereocentre = stereoCentres[i];
        if (i < 4) {
            assertEquals(true, stereocentre->isTrueStereoCentre());
        } else {
            assertEquals(false, stereocentre->isTrueStereoCentre());
            assertEquals(5, stereocentre->getStereoAtom()->getID());
        }
    }
}

std::wstring StereochemistryTest::atomRefsToIdStr(std::vector<Atom *> &atomRefs4) {
    StringBuilder *sb = new StringBuilder();
    for (int i = 0; i < atomRefs4.size(); i++) {
        sb->append(atomRefs4[i]->getID());
        if (i + 1 < atomRefs4.size()) {
            sb->append(L' ');
        }
    }
    return sb->toString();
}
