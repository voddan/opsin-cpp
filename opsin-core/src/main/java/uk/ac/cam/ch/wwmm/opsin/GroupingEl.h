#pragma once

#include "Element.h"
#include <string>
#include <vector>
#include <stdexcept>
#include "stringbuilder.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class Element; } } } } } }

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


						class GroupingEl : public Element
						{

						private:
							const std::vector<Element*> children = std::vector<Element*>();

						public:
							GroupingEl(const std::wstring &name);

							virtual void addChild(Element *child) override;

							virtual Element *copy() override;

							virtual Element *getChild(int index) override;

							virtual int getChildCount() override;

							virtual std::vector<Element*> getChildElements() override;

							virtual std::vector<Element*> getChildElements(const std::wstring &name) override;

							virtual Element *getFirstChildElement(const std::wstring &name) override;

							virtual std::wstring getValue() override;

							virtual int indexOf(Element *child) override;

							virtual void insertChild(Element *child, int index) override;

							virtual bool removeChild(Element *child) override;

							virtual Element *removeChild(int index) override;

							virtual void replaceChild(Element *oldChild, Element *newChild) override;

							virtual void setValue(const std::wstring &text) override;

						};

					}
				}
			}
		}
	}
}
