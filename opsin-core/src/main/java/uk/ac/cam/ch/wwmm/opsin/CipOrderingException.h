#pragma once

#include "StereochemistryException.h"
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
						///Thrown if the ordering of ligands can now be determined by OPSIN's implementation of the CIP rules.
						/// This could be due to a limitation of the implementation or ligands actually being indistinguishable
						/// 
						/// @author dl387
						/// 
						/// </summary>
						class CipOrderingException : public StereochemistryException
						{

						private:
							static constexpr long long serialVersionUID = 1LL;

						public:
							CipOrderingException();

							CipOrderingException(const std::wstring &message);

							CipOrderingException(const std::wstring &message, std::exception cause);

							CipOrderingException(std::exception cause);

						};

					}
				}
			}
		}
	}
}
