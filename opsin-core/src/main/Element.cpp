#include "Element.h"
#include "Attribute.h"
#include "Fragment.h"
#include "OpsinTools.h"


Element::Element(const std::wstring &name) {
    this->name = name;
}

void Element::addAttribute(Attribute *attribute) {
    attributes.push_back(attribute);
}

void Element::addAttribute(const std::wstring &atrName, const std::wstring &atrValue) {
    Attribute tempVar(atrName, atrValue);
    attributes.push_back(&tempVar);
}

void Element::detach() {
    if (parent != nullptr) {
        parent->removeChild(this);
    }
}

Attribute *Element::getAttribute(int index) {
    return attributes[index];
}

Attribute *Element::getAttribute(const std::wstring &name) {
    for (int i = 0, len = attributes.size(); i < len; i++) {
        Attribute *a = attributes[i];
        if (a->getName() == name) {
            return a;
        }
    }
    return nullptr;
}

int Element::getAttributeCount() {
    return attributes.size();
}

std::wstring Element::getAttributeValue(const std::wstring &name) {
    Attribute *attribute = getAttribute(name);
    if (attribute != nullptr) {
        return attribute->getValue();
    }
    return L"";
}

Fragment *Element::getFrag() {
    throw UnsupportedOperationException(L"Only tokens can have associated fragments");
}

std::wstring Element::getName() {
    return name;
}

Element *Element::getParent() {
    return this->parent;
}

bool Element::removeAttribute(Attribute *attribute) {
//JAVA TO C++ CONVERTER TODO TASK: The Java Collection 'remove(Object)' method is not converted:
    return attributes.remove(attribute);
}

void Element::setFrag(Fragment *frag) {
    throw UnsupportedOperationException(L"Only tokens can have associated fragments");
}

void Element::setName(const std::wstring &name) {
    this->name = name;
}

void Element::setParent(Element *newParentEl) {
    this->parent = newParentEl;
}

std::wstring Element::toString() {
    return toXML();
}

std::wstring Element::toXML() {
    return toXML(0)->toString();
}

StringBuilder *Element::toXML(int indent) {
    StringBuilder *result = new StringBuilder();
    for (int i = 0; i < indent; i++) {
        result->append(L"  ");
    }
    result->append(L'<');
    result->append(name);
    for (auto atr : attributes) {
        result->append(L' ');
        result->append(atr->toXML());
    }
    result->append(L'>');
    if (getChildCount() > 0) {
        for (auto child : getChildElements()) {
            result->append(OpsinTools::NEWLINE);
            result->append(child->toXML(indent + 1));
        }
        result->append(OpsinTools::NEWLINE);
        for (int i = 0; i < indent; i++) {
            result->append(L"  ");
        }
    } else {
        result->append(getValue());
    }
    result->append(L"</");
    result->append(name);
    result->append(L'>');

    return result;
}
