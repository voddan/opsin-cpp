#include "FragmentManagerTest.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/FragmentManager.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/IDManager.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/SMILESFragmentBuilder.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/StructureBuildingException.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/Fragment.h"
#include "../../../../../../../../main/java/uk/ac/cam/ch/wwmm/opsin/FragmentTools.h"

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
						using org::junit::Before;
						using org::junit::Test;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void setUp() throws java.io.IOException
						void FragmentManagerTest::setUp() throw(IOException)
						{
							IDManager *idManager = new IDManager();
							SMILESFragmentBuilder tempVar(idManager);
							fragManager = new FragmentManager(&tempVar, idManager);
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testGetUnifiedFrags() throws StructureBuildingException
						void FragmentManagerTest::testGetUnifiedFrags() throw(StructureBuildingException)
						{
							Fragment *frag1 = fragManager->buildSMILES(L"CC");
							Fragment *frag2 = fragManager->buildSMILES(L"CNC");

							fragManager->createBond(frag1->getFirstAtom(), frag2->getFirstAtom(), 1);
							Fragment *frag = fragManager->getUnifiedFragment();
							assertEquals(L"Frag has five atoms", 5, frag->getAtomCount());
							assertEquals(L"Frag has four bonds", 4, frag->getBondSet()->size());
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRelabelFusedRingSystem() throws StructureBuildingException
						void FragmentManagerTest::testRelabelFusedRingSystem() throw(StructureBuildingException)
						{
							Fragment *naphthalene = fragManager->buildSMILES(L"C1=CC=CC2=CC=CC=C12");
							FragmentTools::relabelLocantsAsFusedRingSystem(naphthalene->getAtomList());
							assertEquals(L"Locant 1 = atom 1", 1, naphthalene->getIDFromLocant(L"1"));
							assertEquals(L"Locant 4a = atom 5", 5, naphthalene->getIDFromLocant(L"4a"));
							assertEquals(L"Locant 8 = atom 9", 9, naphthalene->getIDFromLocant(L"8"));
							assertEquals(L"Locant 8a = atom 10", 10, naphthalene->getIDFromLocant(L"8a"));
							assertEquals(L"No locant 9", 0, naphthalene->getIDFromLocant(L"9"));
						}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCloneFragment() throws StructureBuildingException
						void FragmentManagerTest::testCloneFragment() throw(StructureBuildingException)
						{
							Fragment *urea = fragManager->buildSMILES(L"NC(=O)N");
							FragmentTools::assignElementLocants(urea, std::vector<Fragment*>());
							assertNotNull(urea->getAtomByLocant(L"N"));
							assertNotNull(urea->getAtomByLocant(L"N'"));
							assertNull(urea->getAtomByLocant(L"N''"));
							assertNull(urea->getAtomByLocant(L"N'''"));

							Fragment *primedCopy = fragManager->copyAndRelabelFragment(urea, 1);
							assertEquals(4, primedCopy->getAtomCount());
							assertNull(primedCopy->getAtomByLocant(L"N"));
							assertNull(primedCopy->getAtomByLocant(L"N'"));
							assertNotNull(primedCopy->getAtomByLocant(L"N''"));
							assertNotNull(primedCopy->getAtomByLocant(L"N'''"));
						}
					}
				}
			}
		}
	}
}
