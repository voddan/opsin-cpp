#include "NameToStructureException.h"

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

						NameToStructureException::NameToStructureException() : RuntimeException()
						{
						}

						NameToStructureException::NameToStructureException(const std::wstring &message) : RuntimeException(message)
						{
						}

						NameToStructureException::NameToStructureException(const std::wstring &message, std::exception cause) : RuntimeException(message, cause)
						{
						}

						NameToStructureException::NameToStructureException(std::exception cause) : RuntimeException(cause)
						{
						}
					}
				}
			}
		}
	}
}
