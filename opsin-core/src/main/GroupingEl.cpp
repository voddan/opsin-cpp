#include "GroupingEl.h"
#include "Attribute.h"


GroupingEl::GroupingEl(const std::wstring &name) : Element(name) {
}

void GroupingEl::addChild(Element *child) {
    child->setParent(this);
    children.push_back(child);
}

Element *GroupingEl::copy() {
    GroupingEl *copy = new GroupingEl(this->name);
    for (auto childEl : this->children) {
        Element *newChild = childEl->copy();
        newChild->setParent(copy);
        copy->addChild(newChild);
    }
    for (int i = 0, len = this->attributes.size(); i < len; i++) {
        Attribute *atr = this->attributes[i];
        Attribute tempVar(atr);
        copy->addAttribute(&tempVar);
    }
    return copy;
}

Element *GroupingEl::getChild(int index) {
    return children[index];
}

int GroupingEl::getChildCount() {
    return children.size();
}

std::vector<Element *> GroupingEl::getChildElements() {
    return std::vector<Element *>(children);
}

std::vector<Element *> GroupingEl::getChildElements(const std::wstring &name) {
    std::vector<Element *> elements(1);
    for (auto element : children) {
        if (element->name.equals(name)) {
            elements.push_back(element);
        }
    }
    return elements;
}

Element *GroupingEl::getFirstChildElement(const std::wstring &name) {
    for (auto child : children) {
        if (child->getName().equals(name)) {
            return child;
        }
    }
    return nullptr;
}

std::wstring GroupingEl::getValue() {
    int childCount = getChildCount();
    if (childCount == 0) {
        return L"";
    }
    StringBuilder *result = new StringBuilder();
    for (int i = 0; i < childCount; i++) {
        result->append(children[i]->getValue());
    }
    return result->toString();
}

int GroupingEl::indexOf(Element *child) {
    return children.find(child);
}

void GroupingEl::insertChild(Element *child, int index) {
    child->setParent(this);
    children.push_back(index, child);
}

bool GroupingEl::removeChild(Element *child) {
    child->setParent(nullptr);
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
    return children.remove(child);
}

Element *GroupingEl::removeChild(int index) {
    Element *removed = children.erase(children.begin() + index);
    removed->setParent(nullptr);
    return removed;
}

void GroupingEl::replaceChild(Element *oldChild, Element *newChild) {
    int index = indexOf(oldChild);
    if (index == -1) {
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("oldChild is not a child of this element.");
        throw std::exception();
    }
    removeChild(index);
    insertChild(newChild, index);
}

void GroupingEl::setValue(const std::wstring &text) {
    throw UnsupportedOperationException(L"Token groups do not have a value");
}
