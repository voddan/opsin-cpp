#pragma once

#include "StructureBuildingException.h"
#include <string>
#include <stdexcept>

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


						/// <summary>
						/// Thrown if stereochemistry cannot be applied to a structure
						/// @author Daniel
						/// 
						/// </summary>
						class StereochemistryException : public StructureBuildingException
						{

						private:
							static constexpr long long serialVersionUID = 1LL;

						public:
							StereochemistryException();

							StereochemistryException(const std::wstring &message);

							StereochemistryException(const std::wstring &message, std::exception cause);

							StereochemistryException(std::exception cause);
						};
					}
				}
			}
		}
	}
}
