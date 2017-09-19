#include "OpsinTools.h"
#include "Element.h"
#include "XmlDeclarations.h"
#include "Atom.h"
#include "ParsingException.h"



//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;
const std::wstring OpsinTools::NEWLINE = System::getProperty(L"line.separator");

Element *OpsinTools::getNextNonChargeSuffix(Element *currentEl) {
    Element *next = getNextSibling(currentEl);
    while (next != nullptr) {
        if (next->getName() == XmlDeclarations::SUFFIX_EL &&
            XmlDeclarations::CHARGE_TYPE_VAL != next->getAttributeValue(XmlDeclarations::TYPE_ATR)) {
            return next;
        }
        next = getNextSibling(next);
    }
    return nullptr;
}

std::vector<Element *> OpsinTools::combineElementLists(std::vector<Element *> &list1, std::vector<Element *> &list2) {
    std::vector<Element *> elementList(list1);
    elementList.insert(elementList.end(), list2.begin(), list2.end());
    return elementList;
}

Element *OpsinTools::getPreviousGroup(Element *current) {
    if (current->getName() == XmlDeclarations::GROUP_EL) { //can start with a group or the sub/root the group is in
        current = current->getParent();
    }
    Element *parent = current->getParent();
    if (parent == nullptr || parent->getName() == XmlDeclarations::WORDRULE_EL) {
        return nullptr;
    }
    int index = parent->find(current);
    if (index == 0) {
        return getPreviousGroup(parent); //no group found
    }
    Element *previous = parent->getChild(index - 1);
    while (previous->getChildCount() != 0) {
        previous = previous->getChild(previous->getChildCount() - 1);
    }
    std::vector<Element *> groups = previous->getParent()->getChildElements(XmlDeclarations::GROUP_EL);
    if (groups.empty()) {
        return getPreviousGroup(previous);
    } else {
        return groups[groups.size() - 1]; //return last group if multiple exist e.g. fused ring
    }
}

Element *OpsinTools::getNextGroup(Element *current) {
    if (current->getName() == XmlDeclarations::GROUP_EL) { //can start with a group or the sub/root the group is in
        current = current->getParent();
    }
    Element *parent = current->getParent();
    if (parent == nullptr || parent->getName() == XmlDeclarations::MOLECULE_EL) {
        return nullptr;
    }
    int index = parent->find(current);
    if (index == parent->getChildCount() - 1) {
        return getNextGroup(parent); //no group found
    }
    Element *next = parent->getChild(index + 1);
    while (next->getChildCount() != 0) {
        next = next->getChild(0);
    }
    std::vector<Element *> groups = next->getParent()->getChildElements(XmlDeclarations::GROUP_EL);
    if (groups.empty()) {
        return getNextGroup(next);
    } else {
        return groups[0]; //return first group if multiple exist e.g. fused ring
    }
}

Element *OpsinTools::getParentWordRule(Element *el) {
    Element *parent = el->getParent();
    while (parent != nullptr && parent->getName() != XmlDeclarations::WORDRULE_EL) {
        parent = parent->getParent();
    }
    if (parent == nullptr) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("Cannot find enclosing wordRule element");
        throw std::exception();
    } else {
        return parent;
    }
}

Atom *OpsinTools::depthFirstSearchForNonSuffixAtomWithLocant(Atom *startingAtom, const std::wstring &targetLocant) {
    Deque < Atom * > *stack = new ArrayDeque<Atom *>();
    stack->add(startingAtom);
    Set < Atom * > *atomsVisited = std::unordered_set<Atom *>();
    while (stack->size() > 0) {
        Atom *currentAtom = stack->removeLast();
        atomsVisited->add(currentAtom);
        std::vector<Atom *> neighbours = currentAtom->getAtomNeighbours();
        for (auto neighbour : neighbours) {
            if (atomsVisited->contains(neighbour)) { //already visited
                continue;
            }
            std::vector<std::wstring> locants(neighbour->getLocants());
            locants.removeAll(neighbour->getElementSymbolLocants());

            //A main group atom, would expect to only find one except in something strange like succinimide
            //The locants.size() > 0 condition allows things like terephthalate to work which have an atom between the suffixes and main atoms that has no locant
            if (locants.size() > 0 && !neighbour->getType().equals(XmlDeclarations::SUFFIX_TYPE_VAL)) {
                if (std::find(locants.begin(), locants.end(), targetLocant) != locants.end()) {
                    return neighbour;
                }
                continue;
            }
            stack->add(neighbour);
        }
    }
    return nullptr;
}

Atom *OpsinTools::depthFirstSearchForAtomWithNumericLocant(Atom *startingAtom) {
    Deque < Atom * > *stack = new ArrayDeque<Atom *>();
    stack->add(startingAtom);
    Set < Atom * > *atomsVisited = std::unordered_set<Atom *>();
    while (stack->size() > 0) {
        Atom *currentAtom = stack->removeLast();
        atomsVisited->add(currentAtom);
        std::vector<Atom *> neighbours = currentAtom->getAtomNeighbours();
        for (auto neighbour : neighbours) {
            if (atomsVisited->contains(neighbour)) { //already visited
                continue;
            }
            std::vector<std::wstring> locants = neighbour->getLocants();
            for (auto neighbourLocant : locants) {
                if (MATCH_NUMERIC_LOCANT->matcher(neighbourLocant).matches()) {
                    return neighbour;
                }
            }
            stack->add(neighbour);
        }
    }
    return nullptr;
}

WordType OpsinTools::determineWordType(std::vector<wchar_t> &annotations) throw(ParsingException) {
    wchar_t finalAnnotation = annotations[annotations.size() - 1];
    if (finalAnnotation == END_OF_MAINGROUP) {
        return WordType::full;
    } else if (finalAnnotation == END_OF_SUBSTITUENT) {
        return WordType::substituent;
    } else if (finalAnnotation == END_OF_FUNCTIONALTERM) {
        return WordType::functionalTerm;
    } else {
        throw ParsingException(L"OPSIN bug: Unable to determine word type!");
    }

}

Element *OpsinTools::getNextSibling(Element *element) {
    Element *parent = element->getParent();
    int i = parent->find(element);
    if (i + 1 >= parent->getChildCount()) {
        return nullptr;
    }
    return parent->getChild(i + 1);
}

Element *OpsinTools::getNextSibling(Element *current, const std::wstring &elName) {
    Element *next = getNextSibling(current);
    while (next != nullptr) {
        if (next->getName() == elName) {
            return next;
        }
        next = getNextSibling(next);
    }
    return nullptr;
}

Element *OpsinTools::getPreviousSibling(Element *element) {
    Element *parent = element->getParent();
    int i = parent->find(element);
    if (i == 0) {
        return nullptr;
    }
    return parent->getChild(i - 1);
}

Element *OpsinTools::getPreviousSibling(Element *current, const std::wstring &elName) {
    Element *prev = getPreviousSibling(current);
    while (prev != nullptr) {
        if (prev->getName() == elName) {
            return prev;
        }
        prev = getPreviousSibling(prev);
    }
    return nullptr;
}

void OpsinTools::insertBefore(Element *element, Element *newElement) {
    Element *parent = element->getParent();
    int i = parent->find(element);
    parent->insertChild(newElement, i);
}

void OpsinTools::insertAfter(Element *element, Element *neweElement) {
    Element *parent = element->getParent();
    int i = parent->find(element);
    parent->insertChild(neweElement, i + 1);
}

Element *OpsinTools::getNext(Element *element) {
    Element *parent = element->getParent();
    if (parent == nullptr || parent->getName() == XmlDeclarations::MOLECULE_EL) {
        return nullptr;
    }
    int index = parent->find(element);
    if (index + 1 >= parent->getChildCount()) {
        return getNext(parent); //reached end of element
    }
    Element *next = parent->getChild(index + 1);
    while (next->getChildCount() > 0) {
        next = next->getChild(0);
    }
    return next;
}

Element *OpsinTools::getPrevious(Element *element) {
    Element *parent = element->getParent();
    if (parent == nullptr || parent->getName() == XmlDeclarations::MOLECULE_EL) {
        return nullptr;
    }
    int index = parent->find(element);
    if (index == 0) {
        return getPrevious(parent); //reached beginning of element
    }
    Element *previous = parent->getChild(index - 1);
    while (previous->getChildCount() > 0) {
        previous = previous->getChild(previous->getChildCount() - 1);
    }
    return previous;
}

std::vector<Element *> OpsinTools::getNextSiblingsOfType(Element *currentElem, const std::wstring &elName) {
    std::vector<Element *> laterSiblingElementsOfType;
    Element *parent = currentElem->getParent();
    if (parent == nullptr) {
        return laterSiblingElementsOfType;
    }
    int indexOfCurrentElem = parent->find(currentElem);
    for (int i = indexOfCurrentElem + 1; i < parent->getChildCount(); i++) {
        Element *child = parent->getChild(i);
        if (child->getName() == elName) {
            laterSiblingElementsOfType.push_back(child);
        }
    }
    return laterSiblingElementsOfType;
}

std::vector<Element *> OpsinTools::getNextAdjacentSiblingsOfType(Element *currentElem, const std::wstring &elName) {
    std::vector<Element *> siblingElementsOfType;
    Element *parent = currentElem->getParent();
    if (parent == nullptr) {
        return siblingElementsOfType;
    }
    Element *nextSibling = getNextSibling(currentElem);
    while (nextSibling != nullptr && nextSibling->getName() == elName) {
        siblingElementsOfType.push_back(nextSibling);
        nextSibling = getNextSibling(nextSibling);
    }
    return siblingElementsOfType;
}

std::vector<Element *> OpsinTools::getNextSiblingsOfTypes(Element *currentElem, std::vector<std::wstring> &elNames) {
    std::vector<Element *> laterSiblingElementsOfTypes;
    currentElem = getNextSibling(currentElem);
    while (currentElem != nullptr) {
        std::wstring name = currentElem->getName();
        for (auto elName : elNames) {
            if (name == elName) {
                laterSiblingElementsOfTypes.push_back(currentElem);
                break;
            }
        }
        currentElem = getNextSibling(currentElem);
    }
    return laterSiblingElementsOfTypes;
}

std::vector<Element *> OpsinTools::getPreviousSiblingsOfType(Element *currentElem, const std::wstring &elName) {
    std::vector<Element *> earlierSiblingElementsOfType;
    Element *parent = currentElem->getParent();
    if (parent == nullptr) {
        return earlierSiblingElementsOfType;
    }
    int indexOfCurrentElem = parent->find(currentElem);
    for (int i = 0; i < indexOfCurrentElem; i++) {
        Element *child = parent->getChild(i);
        if (child->getName() == elName) {
            earlierSiblingElementsOfType.push_back(child);
        }
    }
    return earlierSiblingElementsOfType;
}

Element *
OpsinTools::getNextSiblingIgnoringCertainElements(Element *startingEl, std::vector<std::wstring> &elNamesToIgnore) {
    Element *parent = startingEl->getParent();
    if (parent == nullptr) {
        return nullptr;
    }
    int i = parent->find(startingEl);
    if (i + 1 >= parent->getChildCount()) {
        return nullptr;
    }
    Element *next = parent->getChild(i + 1);
    std::wstring elName = next->getName();
    for (auto namesToIgnore : elNamesToIgnore) {
        if (elName == namesToIgnore) {
            return getNextSiblingIgnoringCertainElements(next, elNamesToIgnore);
        }
    }
    return next;
}

Element *
OpsinTools::getPreviousSiblingIgnoringCertainElements(Element *startingEl, std::vector<std::wstring> &elNamesToIgnore) {
    Element *parent = startingEl->getParent();
    if (parent == nullptr) {
        return nullptr;
    }
    int i = parent->find(startingEl);
    if (i == 0) {
        return nullptr;
    }
    Element *previous = parent->getChild(i - 1);
    std::wstring elName = previous->getName();
    for (auto namesToIgnore : elNamesToIgnore) {
        if (elName == namesToIgnore) {
            return getPreviousSiblingIgnoringCertainElements(previous, elNamesToIgnore);
        }
    }
    return previous;
}

std::vector<Element *>
OpsinTools::getDescendantElementsWithTagName(Element *startingElement, const std::wstring &elementName) {
    std::vector<Element *> matchingElements;
    Deque < Element * > *stack = new ArrayDeque<Element *>();
    for (int i = startingElement->getChildCount() - 1; i >= 0; i--) {
        stack->add(startingElement->getChild(i));
    }
    while (stack->size() > 0) {
        Element *currentElement = stack->removeLast();
        if (currentElement->getName() == elementName) {
            matchingElements.push_back(currentElement);
        }
        for (int i = currentElement->getChildCount() - 1; i >= 0; i--) {
            stack->add(currentElement->getChild(i));
        }
    }
    return matchingElements;
}

std::vector<Element *>
OpsinTools::getDescendantElementsWithTagNames(Element *startingElement, std::vector<std::wstring> &elementNames) {
    std::vector<Element *> matchingElements;
    Deque < Element * > *stack = new ArrayDeque<Element *>();
    for (int i = startingElement->getChildCount() - 1; i >= 0; i--) {
        stack->add(startingElement->getChild(i));
    }
    while (stack->size() > 0) {
        Element *currentElement = stack->removeLast();
        std::wstring currentElName = currentElement->getName();
        for (auto targetTagName : elementNames) {
            if (currentElName == targetTagName) {
                matchingElements.push_back(currentElement);
                break;
            }
        }
        for (int i = currentElement->getChildCount() - 1; i >= 0; i--) {
            stack->add(currentElement->getChild(i));
        }
    }
    return matchingElements;
}

std::vector<Element *>
OpsinTools::getChildElementsWithTagNames(Element *startingElement, std::vector<std::wstring> &elementNames) {
    std::vector<Element *> matchingElements;
    for (int i = 0, l = startingElement->getChildCount(); i < l; i++) {
        Element *child = startingElement->getChild(i);
        std::wstring currentElName = child->getName();
        for (auto targetTagName : elementNames) {
            if (currentElName == targetTagName) {
                matchingElements.push_back(child);
                break;
            }
        }
    }
    return matchingElements;
}

std::vector<Element *>
OpsinTools::getDescendantElementsWithTagNameAndAttribute(Element *startingElement, const std::wstring &elementName,
                                                         const std::wstring &attributeName,
                                                         const std::wstring &attributeValue) {
    std::vector<Element *> matchingElements;
    Deque < Element * > *stack = new ArrayDeque<Element *>();
    for (int i = startingElement->getChildCount() - 1; i >= 0; i--) {
        stack->add(startingElement->getChild(i));
    }
    while (stack->size() > 0) {
        Element *currentElement = stack->removeLast();
        if (currentElement->getName() == elementName) {
            if (attributeValue == currentElement->getAttributeValue(attributeName)) {
                matchingElements.push_back(currentElement);
            }
        }
        for (int i = currentElement->getChildCount() - 1; i >= 0; i--) {
            stack->add(currentElement->getChild(i));
        }
    }
    return matchingElements;
}

std::vector<Element *>
OpsinTools::getChildElementsWithTagNameAndAttribute(Element *startingElement, const std::wstring &elementName,
                                                    const std::wstring &attributeName,
                                                    const std::wstring &attributeValue) {
    std::vector<Element *> matchingElements;
    for (int i = 0, l = startingElement->getChildCount(); i < l; i++) {
        Element *child = startingElement->getChild(i);
        if (child->getName() == elementName) {
            if (attributeValue == child->getAttributeValue(attributeName)) {
                matchingElements.push_back(child);
            }
        }
    }
    return matchingElements;
}

std::vector<int> OpsinTools::countNumberOfElementsAndNumberOfChildLessElements(Element *startingElement) {
    std::vector<int> counts(2);
    Deque < Element * > *stack = new ArrayDeque<Element *>();
    stack->add(startingElement);
    while (stack->size() > 0) {
        Element *currentElement = stack->removeLast();
        int childCount = currentElement->getChildCount();
        if (childCount == 0) {
            counts[1]++;
        } else {
            stack->addAll(currentElement->getChildElements());
            counts[0] += childCount;
        }
    }
    return counts;
}

std::vector<Element *> OpsinTools::getSiblingsUpToElementWithTagName(Element *startingEl, const std::wstring &elName) {
    std::vector<Element *> laterSiblings;
    Element *nextEl = getNextSibling(startingEl);
    while (nextEl != nullptr && nextEl->getName() != elName) {
        laterSiblings.push_back(nextEl);
        nextEl = getNextSibling(nextEl);
    }
    return laterSiblings;
}
