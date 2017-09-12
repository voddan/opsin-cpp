#include "StereochemistryException.h"

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

						StereochemistryException::StereochemistryException() : StructureBuildingException()
						{
						}

						StereochemistryException::StereochemistryException(const std::wstring &message) : StructureBuildingException(message)
						{
						}

						StereochemistryException::StereochemistryException(const std::wstring &message, std::exception cause) : StructureBuildingException(message, cause)
						{
						}

						StereochemistryException::StereochemistryException(std::exception cause) : StructureBuildingException(cause)
						{
						}
					}
				}
			}
		}
	}
}
