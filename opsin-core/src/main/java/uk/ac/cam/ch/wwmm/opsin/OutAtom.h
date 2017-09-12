#pragma once

#include <string>
#include <boost/optional.hpp>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class Atom; } } } } } }

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
						/// Struct for an OutAtom. As expected holds a reference to an atom.
						/// However if setExplicitly is not true then it is not true then it is not absolutely definitely this amount that is referred to
						/// e.g. propyl is stored as prop-1-yl with this set to false while prop-2-yl has it set to true
						/// 
						/// Also holds the order of the bond that will be created when it is used (valency) e.g.  Eg. chloro 1, oxo 2
						/// 
						/// Optionally a locant may be specified for what the outAtom should connect to if it is convenient to store such information. This is used in ester formation and epoxy fomation
						/// @author dl387
						/// 
						/// </summary>
						class OutAtom
						{

						private:
							Atom *atom;
							int valency = 0;
							bool setExplicitly = false;
							std::wstring locant;

						public:
							virtual ~OutAtom()
							{
								delete atom;
							}

							OutAtom(Atom *atom, int valency, boost::optional<bool> setExplicitly);

							OutAtom(Atom *atom, int valency, boost::optional<bool> setExplicitly, const std::wstring &locant);
							virtual Atom *getAtom();

							virtual void setAtom(Atom *atom);

							virtual int getValency();

							virtual void setValency(int valency);

							virtual bool isSetExplicitly();

							virtual void setSetExplicitly(bool setExplicitly);

							virtual std::wstring getLocant();

							virtual void setLocant(const std::wstring &locant);
						};

					}
				}
			}
		}
	}
}
