#include "BondTest.h"
#include "opsin-core/Element.h"
#include "opsin-core/Fragment.h"
#include "opsin-core/Atom.h"
#include "opsin-core/ChemEl.h"
#include "opsin-core/Bond.h"
#include "opsin-core/BondStereo.h"


//						import static org.junit.Assert.*;
//						import static org.mockito.Mockito.mock;
using org::junit::Test;
using SMILES_BOND_DIRECTION = Bond::SMILES_BOND_DIRECTION;
using BondStereoValue = BondStereo::BondStereoValue;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testBond()
void BondTest::testBond() {
    Fragment * frag = new Fragment(mock(Element::
    typeid));
    Atom *a1 = new Atom(1, ChemEl::C, frag);
    Atom *a2 = new Atom(2, ChemEl::C, frag);
    frag->addAtom(a1);
    frag->addAtom(a2);
    Bond *bond = new Bond(a1, a2, 1);
    assertNotNull(L"Got bond", bond);
    assertEquals(L"From = 1", 1, bond->getFrom());
    assertEquals(L"To = 2", 2, bond->getTo());
    assertEquals(L"Order = 1", 1, bond->getOrder());
    assertEquals(a1, bond->getFromAtom());
    assertEquals(a2, bond->getToAtom());
    assertEquals(a2, bond->getOtherAtom(a1));
    assertEquals(a1, bond->getOtherAtom(a2));
    assertEquals(nullptr, bond->getBondStereo());
    assertEquals(nullptr, bond->getSmilesStereochemistry());
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testBondMutation()
void BondTest::testBondMutation() {
    Fragment * frag = new Fragment(mock(Element::
    typeid));
    Atom *a1 = new Atom(1, ChemEl::C, frag);
    Atom *a2 = new Atom(2, ChemEl::C, frag);
    Atom *a3 = new Atom(3, ChemEl::C, frag);
    Atom *a4 = new Atom(4, ChemEl::C, frag);
    frag->addAtom(a1);
    frag->addAtom(a2);
    frag->addAtom(a3);
    frag->addAtom(a4);
    Bond *bond = new Bond(a2, a3, 1);
    bond->setOrder(2);
    assertEquals(L"Order = 2", 2, bond->getOrder());
    BondStereo *bondStereo = new BondStereo(std::vector<Atom *>{a1, a2, a3, a4}, BondStereoValue::TRANS);
    bond->setBondStereo(bondStereo);
    assertEquals(bondStereo, bond->getBondStereo());
    bond->setSmilesStereochemistry(SMILES_BOND_DIRECTION::LSLASH);
    assertEquals(SMILES_BOND_DIRECTION::LSLASH, bond->getSmilesStereochemistry());
}
