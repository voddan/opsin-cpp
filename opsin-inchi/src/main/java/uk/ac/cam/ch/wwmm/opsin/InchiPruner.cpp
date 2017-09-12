#include "InchiPruner.h"
#include "../../../../../../../../../../opsin-core/src/main/java/uk/ac/cam/ch/wwmm/opsin/StringTools.h"

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
						using StringTools = uk::ac::cam::ch::wwmm::opsin::StringTools;

						std::wstring InchiPruner::mainAndChargeLayers(const std::wstring &inchi)
						{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
							std::vector<std::wstring> inchiLayers = inchi.split(L"/");
							if (inchiLayers.size() < 2)
							{
								return inchi;
							}
							std::vector<std::wstring> retainedLayers;
							if (std::isalpha(inchiLayers[0][inchiLayers[0].length() - 1]))
							{ //remove the S indicating this to be a standard InChI
								inchiLayers[0] = inchiLayers[0].substr(0, inchiLayers[0].length() - 1);
							}
							retainedLayers.push_back(inchiLayers[0]); //version identifier
							retainedLayers.push_back(inchiLayers[1]); //molecular formula

							for (int i = 2; i < inchiLayers.size(); i++)
							{
								boost::optional<wchar_t> c = inchiLayers[i][0];
								if (c == L'c' || c == L'h' || c == L'q' || c == L'p' || c == L'i')
								{
									retainedLayers.push_back(inchiLayers[i]);
								}
								else if (c != L'b' && c != L't' && c != L'm' && c != L's')
								{ //ignore stereochemistry but continue as there may be an isotopic layer
									break;
								}
							}
							return StringTools::stringListToString(retainedLayers, L"/");
						}

						std::wstring InchiPruner::mainChargeAndStereochemistryLayers(const std::wstring &inchi)
						{
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'split' method:
							std::vector<std::wstring> inchiLayers = inchi.split(L"/");
							if (inchiLayers.size() < 2)
							{
								return inchi;
							}
							std::vector<std::wstring> retainedLayers;
							if (std::isalpha(inchiLayers[0][inchiLayers[0].length() - 1]))
							{ //remove the S indicating this to be a standard InChI
								inchiLayers[0] = inchiLayers[0].substr(0, inchiLayers[0].length() - 1);
							}
							retainedLayers.push_back(inchiLayers[0]); //version identifier
							retainedLayers.push_back(inchiLayers[1]); //molecular formula

							for (int i = 2; i < inchiLayers.size(); i++)
							{
								boost::optional<wchar_t> c = inchiLayers[i][0];
								if (c == L'c' || c == L'h' || c == L'q' || c == L'p' || c == L'b' || c == L't' || c == L'm' || c == L's' || c == L'i')
								{
									retainedLayers.push_back(inchiLayers[i]);
								}
								else
								{
									break;
								}
							}
							return StringTools::stringListToString(retainedLayers, L"/");
						}
					}
				}
			}
		}
	}
}
