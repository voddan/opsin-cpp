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

						Attribute::Attribute(const std::wstring &name, const std::wstring &value) : name(name)
						{
							this->value = value;
						}

						Attribute::Attribute(const Attribute &attribute) : name(attribute->getName())
						{
							this->value = attribute->getValue();
						}

						std::wstring Attribute::getValue()
						{
							return value;
						}

						std::wstring Attribute::getName()
						{
							return name;
						}

						void Attribute::setValue(const std::wstring &value)
						{
							this->value = value;
						}

						std::wstring Attribute::toXML()
						{
							return getName() + std::wstring(L"=\"") + escapeText(value) + std::wstring(L"\"");
						}

						std::wstring Attribute::toString()
						{
							return name + std::wstring(L"\t") + value;
						}

						std::wstring Attribute::escapeText(const std::wstring &s)
						{
							StringBuilder *result = new StringBuilder();
							for (int i = 0, l = s.length(); i < l; i++)
							{
								wchar_t c = s[i];
								switch (c)
								{
								case L'\t':
									result->append(L"&#x09;");
									break;
								case L'\n':
									result->append(L"&#x0A;");
									break;
								case L'\r':
									result->append(L"&#x0D;");
									break;
								case L'"':
									result->append(L"&quot;");
									break;
								case L'&':
									result->append(L"&amp;");
									break;
								case L'<':
									result->append(L"&lt;");
									break;
								case L'>':
									result->append(L"&gt;");
									break;
								default:
									result->append(c);
								}
							}
							return result->toString();
						}
					}
				}
			}
		}
	}
}
