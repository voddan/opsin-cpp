#include "DatatypesAutomatonProvider.h"
#include "Automaton.h"
#include "Datatypes.h"

namespace dk
{
	namespace brics
	{
		namespace automaton
		{

			DatatypesAutomatonProvider::DatatypesAutomatonProvider()
			{
				enable_unicodeblocks = enable_unicodecategories = enable_xml = true;
			}

			DatatypesAutomatonProvider::DatatypesAutomatonProvider(bool enable_unicodeblocks, bool enable_unicodecategories, bool enable_xml)
			{
				this->enable_unicodeblocks = enable_unicodeblocks;
				this->enable_unicodecategories = enable_unicodecategories;
				this->enable_xml = enable_xml;
			}

			Automaton *DatatypesAutomatonProvider::getAutomaton(const std::wstring &name)
			{
				if ((enable_unicodeblocks && Datatypes::isUnicodeBlockName(name)) || (enable_unicodecategories && Datatypes::isUnicodeCategoryName(name)) || (enable_xml && Datatypes::isXMLName(name)))
				{
						return Datatypes::get(name);
				}
				return nullptr;
			}
		}
	}
}
