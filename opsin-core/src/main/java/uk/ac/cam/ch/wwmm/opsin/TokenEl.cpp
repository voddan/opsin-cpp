#include "TokenEl.h"
#include "Fragment.h"
#include "Attribute.h"

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

						TokenEl::TokenEl(const std::wstring &name) : Element(name)
						{
							this->value = L"";
						}

						TokenEl::TokenEl(const std::wstring &name, const std::wstring &value) : Element(name)
						{
							this->value = value;
						}

						void TokenEl::addChild(Element *child)
						{
							throw UnsupportedOperationException(L"Tokens do not have children");
						}

						Element *TokenEl::copy()
						{
							TokenEl *copy = new TokenEl(this->name, this->value);
							for (int i = 0, len = this->attributes.size(); i < len; i++)
							{
								Attribute *atr = this->attributes[i];
								Attribute tempVar(atr);
								copy->addAttribute(&tempVar);
							}
							return copy;
						}

						TokenEl *TokenEl::copy(const std::wstring &value)
						{
							TokenEl *copy = new TokenEl(this->name, value);
							for (int i = 0, len = this->attributes.size(); i < len; i++)
							{
								Attribute *atr = this->attributes[i];
								Attribute tempVar(atr);
								copy->addAttribute(&tempVar);
							}
							return copy;
						}

						Element *TokenEl::getChild(int index)
						{
							throw UnsupportedOperationException(L"Tokens do not have children");
						}

						int TokenEl::getChildCount()
						{
							return 0;
						}

						std::vector<Element*> TokenEl::getChildElements()
						{
							return Collections::emptyList();
						}

						std::vector<Element*> TokenEl::getChildElements(const std::wstring &name)
						{
							return Collections::emptyList();
						}

						Element *TokenEl::getFirstChildElement(const std::wstring &name)
						{
							return nullptr;
						}

						Fragment *TokenEl::getFrag()
						{
							return frag;
						}

						std::wstring TokenEl::getValue()
						{
							return value;
						}

						int TokenEl::indexOf(Element *child)
						{
							return -1;
						}

						void TokenEl::insertChild(Element *child, int index)
						{
							throw UnsupportedOperationException(L"Tokens do not have children");
						}

						bool TokenEl::removeChild(Element *child)
						{
							throw UnsupportedOperationException(L"Tokens do not have children");
						}

						Element *TokenEl::removeChild(int index)
						{
							throw UnsupportedOperationException(L"Tokens do not have children");
						}

						void TokenEl::replaceChild(Element *oldChild, Element *newChild)
						{
							throw UnsupportedOperationException(L"Tokens do not have children");
						}

						void TokenEl::setFrag(Fragment *frag)
						{
							this->frag = frag;
						}

						void TokenEl::setValue(const std::wstring &text)
						{
							this->value = text;
						}
					}
				}
			}
		}
	}
}
