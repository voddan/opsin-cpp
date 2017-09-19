#include "BuildResults.h"
#include "OutAtom.h"
#include "FunctionalAtom.h"
#include "Fragment.h"
#include "Element.h"
#include "OpsinTools.h"
#include "XmlDeclarations.h"
#include "Atom.h"
#include "StructureBuildingException.h"


BuildResults::BuildResults(Element *wordSubOrBracket) {
    std::vector<Element *> groups = OpsinTools::getDescendantElementsWithTagName(wordSubOrBracket,
                                                                                 XmlDeclarations::GROUP_EL);
    for (auto group : groups) {
        Fragment *frag = group->getFrag();
        fragments->add(frag);
        for (int i = 0, l = frag->getOutAtomCount(); i < l; i++) {
            outAtoms.push_back(frag->getOutAtom(i));
        }
        int functionalAtomCount = frag->getFunctionalAtomCount();
        if (functionalAtomCount > 0) {
            Element *parent = group->getParent();
            if (parent->getName() == XmlDeclarations::ROOT_EL || OpsinTools::getNextGroup(group) == nullptr) {
                for (int i = 0; i < functionalAtomCount; i++) {
                    functionalAtoms.push_back(frag->getFunctionalAtom(i));
                }
            }
        }
    }
}

BuildResults::BuildResults() {
}

Set<Fragment *> *BuildResults::getFragments() {
    return Collections::unmodifiableSet(fragments);
}

int BuildResults::getFragmentCount() {
    return fragments->size();
}

OutAtom *BuildResults::getOutAtom(int i) {
    return outAtoms[i];
}

int BuildResults::getOutAtomCount() {
    return outAtoms.size();
}

OutAtom *BuildResults::removeOutAtom(int i) {
    OutAtom *outAtom = outAtoms[i];
    outAtom->getAtom()->getFrag()->removeOutAtom(outAtom);
    return outAtoms.erase(outAtoms.begin() + i);
}

void BuildResults::removeAllOutAtoms() {
    for (int i = outAtoms.size() - 1; i >= 0; i--) {
        removeOutAtom(i);
    }
}

Atom *BuildResults::getFunctionalAtom(int i) {
    return functionalAtoms[i]->getAtom();
}

FunctionalAtom *BuildResults::removeFunctionalAtom(int i) {
    FunctionalAtom *functionalAtom = functionalAtoms[i];
    functionalAtom->getAtom()->getFrag()->removeFunctionalAtom(functionalAtom);
    return functionalAtoms.erase(functionalAtoms.begin() + i);
}

int BuildResults::getFunctionalAtomCount() {
    return functionalAtoms.size();
}

OutAtom *BuildResults::getFirstOutAtom() {
    return outAtoms[0];
}

Atom *BuildResults::getAtomByIdOrThrow(int id) throw(StructureBuildingException) {
    for (auto fragment : fragments) {
        Atom *outAtom = fragment->getAtomByID(id);
        if (outAtom != nullptr) {
            return outAtom;
        }
    }
    throw StructureBuildingException(std::wstring(L"No fragment contained this id: ") + std::to_wstring(id));
}

void BuildResults::mergeBuildResults(BuildResults *otherBR) {
    outAtoms.insert(outAtoms.end(), otherBR->outAtoms.begin(), otherBR->outAtoms.end());
    functionalAtoms.insert(functionalAtoms.end(), otherBR->functionalAtoms.begin(), otherBR->functionalAtoms.end());
    fragments->addAll(otherBR->fragments);
}

int BuildResults::getCharge() {
    int totalCharge = 0;
    for (auto frag : fragments) {
        totalCharge += frag->getCharge();
    }
    return totalCharge;
}
