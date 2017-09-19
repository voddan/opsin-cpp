#include "CycleDetectorTest.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/SMILESFragmentBuilder.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/StructureBuildingException.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/Fragment.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/Atom.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/CycleDetector.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/Bond.h"


//						import static org.junit.Assert.*;
using org::junit::Test;
using Fragment = Fragment;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAssignCyclic1() throws StructureBuildingException
void CycleDetectorTest::testAssignCyclic1() throw(StructureBuildingException) {
    Fragment *frag = sBuilder->build(L"CCCC");
    for (auto a : frag->getAtomList()) {
        assertEquals(L"Should be acylic", false, a->getAtomIsInACycle());
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAssignCyclic2() throws StructureBuildingException
void CycleDetectorTest::testAssignCyclic2() throw(StructureBuildingException) {
    Fragment *frag = sBuilder->build(L"c1ccccc1");
    for (auto a : frag->getAtomList()) {
        assertEquals(L"Should be cylic", true, a->getAtomIsInACycle());
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAssignCyclic3() throws StructureBuildingException
void CycleDetectorTest::testAssignCyclic3() throw(StructureBuildingException) {
    Fragment *frag = sBuilder->build(L"c12.c23.c34.c45.c56.c61");
    for (auto a : frag->getAtomList()) {
        assertEquals(L"Should be cylic", true, a->getAtomIsInACycle());
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAssignCyclic4() throws StructureBuildingException
void CycleDetectorTest::testAssignCyclic4() throw(StructureBuildingException) {
    Fragment *frag = sBuilder->build(L"c1ccccc1CCc1ccccc1");
    std::vector<Atom *> atomList = frag->getAtomList();
    for (int i = 0; i < atomList.size(); i++) {
        Atom *a = atomList[i];
        if (i <= 5 || i >= 8) {
            assertEquals(L"Should be cylic", true, a->getAtomIsInACycle());
        } else {
            assertEquals(L"Should be acylic", false, a->getAtomIsInACycle());
        }
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAssignCyclic5() throws StructureBuildingException
void CycleDetectorTest::testAssignCyclic5() throw(StructureBuildingException) {
    Fragment *frag = sBuilder->build(L"CCc1ccc(O)cc1");
    std::vector<Atom *> atomList = frag->getAtomList();
    for (int i = 0; i < atomList.size(); i++) {
        Atom *a = atomList[i];
        if (i <= 1 || i == 6) {
            assertEquals(L"Should be acylic", false, a->getAtomIsInACycle());
        } else {
            assertEquals(L"Should be cylic", true, a->getAtomIsInACycle());
        }
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAssignCyclic6() throws StructureBuildingException
void CycleDetectorTest::testAssignCyclic6() throw(StructureBuildingException) {
    Fragment *frag = sBuilder->build(L"CC1CC(O1)C");
    std::vector<Atom *> atomList = frag->getAtomList();
    for (int i = 0; i < atomList.size(); i++) {
        Atom *a = atomList[i];
        if (i == 0 || i == 5) {
            assertEquals(L"Should be acylic", false, a->getAtomIsInACycle());
        } else {
            assertEquals(L"Should be cylic", true, a->getAtomIsInACycle());
        }
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testFindPathBetweenAtoms1() throws StructureBuildingException
void CycleDetectorTest::testFindPathBetweenAtoms1() throw(StructureBuildingException) {
    Fragment *frag = sBuilder->build(L"c1ccccc1");
    std::vector<Atom *> atomList = frag->getAtomList();
    std::vector<std::vector<Atom *>> paths = CycleDetector::getPathBetweenAtomsUsingBonds(atomList[0], atomList[3],
                                                                                          frag->getBondSet());
    assertEquals(2, paths.size());
    for (auto path : paths) {
        assertEquals(2, path.size());
    }
    for (auto path : paths) {
        if (atomList.find(path.get(0)) == 1) {
            assertEquals(2, atomList.find(path.get(1)));
        } else {
            assertEquals(5, atomList.find(path.get(0)));
            assertEquals(4, atomList.find(path.get(1)));
        }
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testFindPathBetweenAtoms2() throws StructureBuildingException
void CycleDetectorTest::testFindPathBetweenAtoms2() throw(StructureBuildingException) {
    Fragment *frag = sBuilder->build(L"C1CCCC2CCCCC12");
    std::vector<Atom *> atomList = frag->getAtomList();
    Set < Bond * > *bonds = std::unordered_set<Bond *>(frag->getBondSet());
    bonds->remove(atomList[4]->getBondToAtom(atomList[9]));
    std::vector<std::vector<Atom *>> paths = CycleDetector::getPathBetweenAtomsUsingBonds(atomList[4], atomList[9],
                                                                                          bonds);
    assertEquals(2, paths.size());

    std::vector<Atom *> pathLeftRing;
    std::vector<Atom *> pathRightRing;
    if (atomList.find(paths[0][0]) == 3) {
        pathLeftRing = paths[0];
        pathRightRing = paths[1];
    } else {
        pathLeftRing = paths[1];
        pathRightRing = paths[0];
    }
    assertEquals(3, atomList.find(pathLeftRing[0]));
    assertEquals(2, atomList.find(pathLeftRing[1]));
    assertEquals(1, atomList.find(pathLeftRing[2]));
    assertEquals(0, atomList.find(pathLeftRing[3]));

    assertEquals(5, atomList.find(pathRightRing[0]));
    assertEquals(6, atomList.find(pathRightRing[1]));
    assertEquals(7, atomList.find(pathRightRing[2]));
    assertEquals(8, atomList.find(pathRightRing[3]));
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testFindPathBetweenAtoms3() throws StructureBuildingException
void CycleDetectorTest::testFindPathBetweenAtoms3() throw(StructureBuildingException) {
    Fragment *frag = sBuilder->build(L"C1(C)CCCC2C(C)CCCC12");
    std::vector<Atom *> atomList = frag->getAtomList();
    Set < Bond * > *bonds = std::unordered_set<Bond *>(frag->getBondSet());
    bonds->remove(atomList[0]->getBondToAtom(atomList[1]));
    bonds->remove(atomList[6]->getBondToAtom(atomList[7]));
    bonds->remove(atomList[5]->getBondToAtom(atomList[11]));
    std::vector<std::vector<Atom *>> paths = CycleDetector::getPathBetweenAtomsUsingBonds(atomList[0], atomList[6],
                                                                                          bonds);
    assertEquals(2, paths.size());

    std::vector<Atom *> pathLeftRing;
    std::vector<Atom *> pathRightRing;
    if (atomList.find(paths[0][0]) == 2) {
        pathLeftRing = paths[0];
        pathRightRing = paths[1];
    } else {
        pathLeftRing = paths[1];
        pathRightRing = paths[0];
    }
    assertEquals(2, atomList.find(pathLeftRing[0]));
    assertEquals(3, atomList.find(pathLeftRing[1]));
    assertEquals(4, atomList.find(pathLeftRing[2]));
    assertEquals(5, atomList.find(pathLeftRing[3]));

    assertEquals(11, atomList.find(pathRightRing[0]));
    assertEquals(10, atomList.find(pathRightRing[1]));
    assertEquals(9, atomList.find(pathRightRing[2]));
    assertEquals(8, atomList.find(pathRightRing[3]));
}
