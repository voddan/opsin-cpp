#pragma once

#include <string>
#include <vector>
#include <cctype>
#include <boost/optional.hpp>

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



						class InchiPruner
						{
							/// <summary>
							/// Return a modified version of the given InChI where the:
							/// stereochemistry, fixed hydrogen and reconnected layers have been removed
							/// The S indicating standard InChI is also removed </summary>
							/// <param name="inchi"> </param>
							/// <returns> InChI just containing the c,h,q,p,i layers </returns>
						public:
							static std::wstring mainAndChargeLayers(const std::wstring &inchi);

							/// <summary>
							/// Return a modified version of the given InChI where the:
							/// fixed hydrogen and reconnected layers have been removed
							/// The S indicating standard InChI is also removed </summary>
							/// <param name="inchi"> </param>
							/// <returns> InChI just containing the c,h,q,p,b,t,m,s,i layers </returns>
							static std::wstring mainChargeAndStereochemistryLayers(const std::wstring &inchi);
						};

					}
				}
			}
		}
	}
}
