#pragma once

#include <string>
#include <vector>
#include "stringbuilder.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class Attribute;
class Fragment;


class Element {

protected:
    std::wstring name;
    Element *parent = nullptr;
    const std::vector<Attribute *> attributes = std::vector<Attribute *>();

public:
    virtual ~Element() {
        delete parent;
    }

    Element(const std::wstring &name);

    virtual void addAttribute(Attribute *attribute);

    virtual void addAttribute(const std::wstring &atrName, const std::wstring &atrValue);

    /// <summary>
    /// Adds a child element </summary>
    /// <param name="child"> </param>
    virtual void addChild(Element *child) = 0;

    /// <summary>
    /// Creates a deep copy with no parent
    /// </summary>
    virtual Element *copy() = 0;

    virtual void detach();

    virtual Attribute *getAttribute(int index);

    /// <summary>
    /// Returns the attribute with the given name
    /// or null if the attribute doesn't exist </summary>
    /// <param name="name">
    /// @return </param>
    virtual Attribute *getAttribute(const std::wstring &name);

    virtual int getAttributeCount();

    /// <summary>
    /// Returns the value of the attribute with the given name
    /// or null if the attribute doesn't exist </summary>
    /// <param name="name">
    /// @return </param>
    virtual std::wstring getAttributeValue(const std::wstring &name);

    /// <summary>
    /// Returns the child at the given index in the children list </summary>
    /// <param name="index">
    /// @return </param>
    virtual Element *getChild(int index) = 0;

    /// <summary>
    /// Returns the number of children
    /// @return
    /// </summary>
    virtual int getChildCount() = 0;

    /// <summary>
    /// Returns a copy of the child elements
    ///
    /// @return
    /// </summary>
    virtual std::vector<Element *> getChildElements() = 0;

    /// <summary>
    /// Gets child elements with this name (in iteration order) </summary>
    /// <param name="name">
    /// @return </param>
    virtual std::vector<Element *> getChildElements(const std::wstring &name) = 0;

    /// <summary>
    /// Returns the first child element with the specified name
    /// </summary>
    /// <param name="name">
    /// @return </param>
    virtual Element *getFirstChildElement(const std::wstring &name) = 0;

    /// <summary>
    /// Returns the fragment associated with this element (only applicable to tokens)
    /// @return
    /// </summary>
    virtual Fragment *getFrag();

    virtual std::wstring getName();

    virtual Element *getParent();

    virtual std::wstring getValue() = 0;

    /// <summary>
    /// Returns the index of the given child in the children list (or -1 if it isn't a child) </summary>
    /// <param name="child">
    /// @return </param>
    virtual int indexOf(Element *child) = 0;

    /// <summary>
    /// Inserts the element at the given index in the children list </summary>
    /// <param name="child"> </param>
    /// <param name="index"> </param>
    virtual void insertChild(Element *child, int index) = 0;

    virtual bool removeAttribute(Attribute *attribute);

    /// <summary>
    /// Removes the given child element </summary>
    /// <param name="child">
    /// @return </param>
    virtual bool removeChild(Element *child) = 0;

    /// <summary>
    /// Removes the element at the given index in the children list </summary>
    /// <param name="index">
    /// @return </param>
    virtual Element *removeChild(int index) = 0;

    /// <summary>
    /// Replaces a child element with another element </summary>
    /// <param name="oldChild"> </param>
    /// <param name="newChild"> </param>
    virtual void replaceChild(Element *oldChild, Element *newChild) = 0;

    /// <summary>
    /// Sets the fragment associated with this element (only applicable to tokens!) </summary>
    /// <param name="frag"> </param>
    virtual void setFrag(Fragment *frag);

    virtual void setName(const std::wstring &name);

    virtual void setParent(Element *newParentEl);

    virtual void setValue(const std::wstring &text) = 0;

    virtual std::wstring toString() override;

    virtual std::wstring toXML();

private:
    StringBuilder *toXML(int indent);

};

