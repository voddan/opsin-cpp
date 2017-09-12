#pragma once

#include "BondStereo.h"
#include <vector>
#include <stdexcept>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class Atom; } } } } } }
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class BondStereo; } } } } } }

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

						using BondStereoValue = uk::ac::cam::ch::wwmm::opsin::BondStereo::BondStereoValue;

						/// <summary>
						///A bond, between two atoms.
						/// 
						/// @author ptc24
						/// @author dl387
						/// 
						/// </summary>
						class Bond
						{
							/// <summary>
							/// The Atom the bond comes from </summary>
						private:
							Atom *const from;
							/// <summary>
							/// The Atom the bond goes to </summary>
							Atom *const to;
							/// <summary>
							/// The bond order </summary>
							int order = 0;

						public:
							enum class SMILES_BOND_DIRECTION
							{
								RSLASH,
								LSLASH
							};
							/// <summary>
							/// If this bond was built from SMILES can be set to either RSLASH or LSLASH. Subsequently read to add a bondStereoElement
							/// null by default
							/// </summary>
						private:
							SMILES_BOND_DIRECTION smilesBondDirection = nullptr;

							/// <summary>
							/// Holds the bondStereo object associated with this bond
							/// null by default
							/// </summary>
							BondStereo *bondStereo = nullptr;

							/// <summary>
							/// DO NOT CALL DIRECTLY EXCEPT FOR TESTING
							/// Creates a new Bond.
							/// </summary>
							/// <param name="from"> The Atom the bond comes from. </param>
							/// <param name="to"> The Atom the bond goes to. </param>
							/// <param name="order"> The bond order. </param>
						public:
							virtual ~Bond()
							{
								delete from;
								delete to;
								delete bondStereo;
							}

							Bond(Atom *from, Atom *to, int order);

							/// <summary>
							/// Gets from ID </summary>
							/// <returns> ID </returns>
							virtual int getFrom();

							/// <summary>
							/// Gets to ID </summary>
							/// <returns> ID </returns>
							virtual int getTo();

							/// <summary>
							///Gets order.
							/// @return
							/// </summary>
							virtual int getOrder();

							/// <summary>
							///Sets order. </summary>
							/// <param name="order"> </param>
							virtual void setOrder(int order);

							/// <summary>
							/// Gets from Atom </summary>
							/// <returns> Atom </returns>
							virtual Atom *getFromAtom();

							/// <summary>
							/// Gets to Atom </summary>
							/// <returns> Atom </returns>
							virtual Atom *getToAtom();

							/// <summary>
							///Adds to the bond order.
							/// </summary>
							/// <param name="o"> The value to be added to the bond order. </param>
							virtual void addOrder(int o);

							/// <summary>
							/// Returns either null or RSLASH or LSLASH
							/// @return
							/// </summary>
							virtual SMILES_BOND_DIRECTION getSmilesStereochemistry();

							virtual void setSmilesStereochemistry(SMILES_BOND_DIRECTION bondDirection);

							virtual BondStereo *getBondStereo();

							virtual void setBondStereo(BondStereo *bondStereo);

							virtual void setBondStereoElement(std::vector<Atom*> &atomRefs4, BondStereoValue cOrT);

							/// <summary>
							/// Returns the atom at the other end of the bond to given atom </summary>
							/// <param name="atom">
							/// @return </param>
							virtual Atom *getOtherAtom(Atom *atom);

							virtual int hashCode() override;

							virtual bool equals(void *obj) override;
						};

					}
				}
			}
		}
	}
}
