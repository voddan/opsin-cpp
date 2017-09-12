#pragma once

#include "WordRule.h"
#include "OpsinWarning.h"
#include <string>
#include <unordered_map>
#include <vector>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class FragmentManager; } } } } } }
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class Element; } } } } } }
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class Fragment; } } } } } }
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class NameToStructureConfig; } } } } } }
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class OpsinWarning; } } } } } }

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


						using OpsinWarningType = uk::ac::cam::ch::wwmm::opsin::OpsinWarning::OpsinWarningType;

						/// <summary>
						/// Used to pass the current configuration and FragmentManager around
						/// The currentWordRule can be mutated to keep track of what the parent wordRule is at the given time
						/// 
						/// @author dl387
						/// 
						/// </summary>
						class BuildState
						{

						public:
							FragmentManager *const fragManager;
							const std::unordered_map<Element*, std::vector<Fragment*>> xmlSuffixMap;
							NameToStructureConfig *const n2sConfig;
						private:
							const std::vector<OpsinWarning*> warnings = std::vector<OpsinWarning*>();

						public:
							WordRule currentWordRule = nullptr;

							virtual ~BuildState()
							{
								delete fragManager;
								delete n2sConfig;
							}

							BuildState(NameToStructureConfig *n2sConfig);

							virtual std::vector<OpsinWarning*> getWarnings();

							virtual void addWarning(OpsinWarningType type, const std::wstring &message);

							virtual void addIsAmbiguous(const std::wstring &message);
						};

					}
				}
			}
		}
	}
}
