#pragma once

#include "Element.h"
#include <string>
#include <vector>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class Fragment; } } } } } }
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


						class TokenEl : public Element
						{

						private:
							std::wstring value;
							Fragment *frag;

						public:
							virtual ~TokenEl()
							{
								delete frag;
							}

							TokenEl(const std::wstring &name);

							TokenEl(const std::wstring &name, const std::wstring &value);

							virtual void addChild(Element *child) override;

							virtual Element *copy() override;

							/// <summary>
							/// Creates a copy with no parent
							/// The provided value is used instead of the Element to be copied's value </summary>
							/// <param name="value">
							/// @return </param>
							virtual TokenEl *copy(const std::wstring &value);

							virtual Element *getChild(int index) override;

							virtual int getChildCount() override;

							virtual std::vector<Element*> getChildElements() override;

							virtual std::vector<Element*> getChildElements(const std::wstring &name) override;

							virtual Element *getFirstChildElement(const std::wstring &name) override;

							virtual Fragment *getFrag() override;

							virtual std::wstring getValue() override;

							virtual int indexOf(Element *child) override;

							virtual void insertChild(Element *child, int index) override;

							virtual bool removeChild(Element *child) override;

							virtual Element *removeChild(int index) override;

							virtual void replaceChild(Element *oldChild, Element *newChild) override;

							virtual void setFrag(Fragment *frag) override;

							virtual void setValue(const std::wstring &text) override;

						};

					}
				}
			}
		}
	}
}
