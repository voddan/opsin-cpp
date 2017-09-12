#pragma once

#include "Bond.h"
#include "ChemEl.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <boost/optional.hpp>
#include "stringhelper.h"
#include "stringbuilder.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class Atom; } } } } } }
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class IDManager; } } } } } }
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class Fragment; } } } } } }
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class StructureBuildingException; } } } } } }
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class AtomParity; } } } } } }
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class Element; } } } } } }
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class Bond; } } } } } }

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

						using namespace uk::ac::cam::ch::wwmm::opsin;
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//						import static uk.ac.cam.ch.wwmm.opsin.XmlDeclarations.*;


						using SMILES_BOND_DIRECTION = uk::ac::cam::ch::wwmm::opsin::Bond::SMILES_BOND_DIRECTION;

						/// <summary>
						/// A builder for fragments specified as SMILES. A slightly custom SMILES dialect is used.
						/// It includes all common features of SMILES and a few useful extensions:
						/// | is used within a square bracketed element to directly set valency e.g. [P|5]. This is the same as using the lambda convention
						/// sb/te are allowed (aromatic antimony/tellurium):
						/// H? e.g. [SeH?] is used to indicate that the atom should use the default valency. It is equivalent to not using square brackets for organic atoms
						/// 
						/// Allowed:
						/// Organic elements B,C,N,O,P,S,F,Cl,Br,I (square brackets not required)
						/// Aromatic elements c,n,o,p,s (square brackets not required) si,as,se,sb,te (square brackets required) Note that the inclusion of si/sb/te are an unofficial extension
						/// =, # for bond orders
						/// . for disconnection
						/// (, ) for branching
						/// [, ] for placing inorganic elements within and specifying charge. Allowed: [Al3+] or [Al+++]
						/// 012345679 - ring closures
						/// %10 %99 - more ring closures (%100 is ring closure %10 and 0 as in normal SMILES)
						/// / and \ to set double bond stereochemistry to cis/trans
						/// @ and @@ to set tetrahedral stereochemistry as in SMILES.
						/// Hx where x is a digit is used to sort of set the hydrogen. In actuality the valency of the atom is derived and a valency hint added to the atom
						/// This valency hint is the minimum valency that atom may be in. H? as an extension gives you the lowest acceptable valency.
						/// |3 |5 etc. can be used to set the valency of an atom e.g.  [Se|2]
						/// 
						/// Also, an = or # at the start of the string indicates that the group attaches to its parent group via a double or triple bond.
						/// 
						/// A -,=,# on the end indicates that in the absence of locants, other groups attach to
						/// *it* via the atom at the end of the string, not at the start of the string with -,=,# meaning single,double or triple bond
						/// This behaviour is overridden for certain suffixes to give different meanings to the atom the -,=,# is referring to
						/// 
						/// @author ptc24
						/// @author dl387
						/// 
						/// </summary>
						class SMILESFragmentBuilder
						{

							/// <summary>
							///A "struct" to hold information on the parsing stack
							/// 
							/// @author ptc24
							/// 
							/// </summary>
						private:
							class StackFrame
							{
								/// <summary>
								///The Atom currently under consideration. </summary>
							public:
								Atom *atom;

								/// <summary>
								///The order of the bond about to be formed. </summary>
								int bondOrder = 0;

								/// <summary>
								///Whether the bond is a \ or / bond for use in determining cis/trans. </summary>
								SMILES_BOND_DIRECTION slash = nullptr;

								/// <summary>
								///The index of a dummy atom in the atom's stereochemistry atomrefs4 </summary>
								boost::optional<int> indexOfDummyAtom = boost::none;

								/// <summary>
								///Creates a stack frame with given parameters.
								/// </summary>
								/// <param name="a"> An atom or null </param>
								/// <param name="bondOrderVal"> The value for bondOrder. </param>
								virtual ~StackFrame()
								{
									delete atom;
								}

								StackFrame(Atom *a, int bondOrderVal);

								/// <summary>
								///Creates a copy of an existing StackFrame.
								/// </summary>
								/// <param name="sf"> The stackframe to copy. </param>
								StackFrame(const StackFrame &sf);
							};

							/// <summary>
							///Ring opening dummy atom, used as a placeholder in stereochemistry atomrefs4 </summary>
						private:
							static Atom *const ringOpeningDummyAtom = new Atom(ChemEl::R);

							/// <summary>
							///Organic Atoms. </summary>
							static Set<std::wstring> *const organicAtoms;
							/// <summary>
							///Aromatic Atoms. </summary>
							static Set<std::wstring> *const aromaticAtoms;

												private:
													class StaticConstructor
													{
													public:
														StaticConstructor();
													};

												private:
													static SMILESFragmentBuilder::StaticConstructor staticConstructor;


							IDManager *const idManager;

						public:
							virtual ~SMILESFragmentBuilder()
							{
								delete idManager;
							}

							SMILESFragmentBuilder(IDManager *idManager);

						private:
							class ParserInstance
							{
												private:
													SMILESFragmentBuilder *outerInstance;

								Deque<StackFrame*> *const stack = new ArrayDeque<StackFrame*>();
								const std::unordered_map<std::wstring, StackFrame*> ringClosures = std::unordered_map<std::wstring, StackFrame*>();

								const std::wstring smiles;
								const int endOfSmiles;
								Fragment *const fragment;

								int i = 0;

							public:
								virtual ~ParserInstance()
								{
									delete stack;
									delete fragment;
									delete outerInstance;
								}

								ParserInstance(SMILESFragmentBuilder *outerInstance, const std::wstring &smiles, Fragment *fragment);

								virtual void parseSmiles() throw(StructureBuildingException);

								/// <summary>
								/// An organic atom e.g. 'C', 'Cl', 'c' etc. </summary>
								/// <param name="ch"> </param>
								/// <exception cref="StructureBuildingException"> </exception>
							private:
								void processOrganicAtom(wchar_t ch) throw(StructureBuildingException);

								/// <summary>
								/// square brackets- contain non-organic atoms or where required to set properties such as charge/chirality etc.
								/// e.g. [Na+] </summary>
								/// <exception cref="StructureBuildingException"> </exception>
								void processBracketedAtom() throw(StructureBuildingException);

								/// <summary>
								/// Adds an atomParity element to the given atom using the information at the current index </summary>
								/// <param name="atom"> </param>
								/// <param name="previousAtom"> </param>
								void processTetrahedralStereochemistry(Atom *atom, Atom *previousAtom);

								/// <summary>
								/// Process ring openings and closings e.g. the two 1s in c1ccccc1 </summary>
								/// <param name="ch"> </param>
								/// <exception cref="StructureBuildingException"> </exception>
								void processRingOpeningOrClosure(wchar_t ch) throw(StructureBuildingException);

								void processRingOpening(const std::wstring &closure) throw(StructureBuildingException);

								void processRingClosure(const std::wstring &closure) throw(StructureBuildingException);

								/// <summary>
								/// Adds an atom at the first non-null position in the atomParity's atomRefs4 </summary>
								/// <param name="atomParity"> </param>
								/// <param name="atom"> </param>
								/// <returns> Returns the index of the atom in the atomParity's atomRefs4 </returns>
								/// <exception cref="StructureBuildingException"> </exception>
								int addAtomToAtomParity(AtomParity *atomParity, Atom *atom) throw(StructureBuildingException);

								/// <summary>
								/// For non-empty SMILES will return the atom at the top of the stack i.e. the one that will be bonded to next if the SMILES continued
								/// (only valid during execution of and after <seealso cref="ParserInstance#parseSmiles()"/> has been called)
								/// @return
								/// </summary>
							public:
								virtual Atom *getInscopeAtom();
							};

							/// <summary>
							/// Build a Fragment based on a SMILES string.
							/// The type/subType of the Fragment are the empty String
							/// The fragment has no locants
							/// </summary>
							/// <param name="smiles"> The SMILES string to build from. </param>
							/// <returns> The built fragment. </returns>
							/// <exception cref="StructureBuildingException"> </exception>
						public:
							virtual Fragment *build(const std::wstring &smiles) throw(StructureBuildingException);

							/// <summary>
							/// Build a Fragment based on a SMILES string. </summary>
							/// <param name="smiles"> The SMILES string to build from. </param>
							/// <param name="type"> The type of the fragment retrieved when calling <seealso cref="Fragment#getType()"/> </param>
							/// <param name="labelMapping"> A string indicating which locants to assign to each atom. Can be a slash delimited list, "numeric", "fusedRing" or "none"/""
							/// @return </param>
							/// <exception cref="StructureBuildingException"> </exception>
							virtual Fragment *build(const std::wstring &smiles, const std::wstring &type, const std::wstring &labelMapping) throw(StructureBuildingException);

							/// <summary>
							/// Build a Fragment based on a SMILES string. </summary>
							/// <param name="smiles"> The SMILES string to build from. </param>
							/// <param name="tokenEl"> The corresponding tokenEl </param>
							/// <param name="labelMapping"> A string indicating which locants to assign to each atom. Can be a slash delimited list, "numeric", "fusedRing" or "none"/"" </param>
							/// <returns> Fragment The built fragment. </returns>
							/// <exception cref="StructureBuildingException"> </exception>
							virtual Fragment *build(const std::wstring &smiles, Element *tokenEl, const std::wstring &labelMapping) throw(StructureBuildingException);

						private:
							Fragment *build(const std::wstring &smiles, Fragment *fragment, const std::wstring &labelMapping) throw(StructureBuildingException);

							void processLabelling(const std::wstring &labelMapping, std::vector<Atom*> &atomList) throw(StructureBuildingException);

							void verifyAndTakeIntoAccountLonePairsInAtomParities(std::vector<Atom*> &atomList) throw(StructureBuildingException);

							void addBondStereoElements(Fragment *currentFrag) throw(StructureBuildingException);

							/// <summary>
							/// Utilises the atom's hydrogen count as set by the SMILES as well as incoming valency to determine the atom's valency
							/// If the atom is charged whether protons have been added or removed will also need to be determined </summary>
							/// <param name="atom"> </param>
							/// <exception cref="StructureBuildingException">  </exception>
							void setupAtomValency(Atom *atom) throw(StructureBuildingException);


							/// <summary>
							/// Create a new Atom of the given element belonging to the given fragment </summary>
							/// <param name="elementSymbol"> </param>
							/// <param name="frag"> </param>
							/// <returns> Atom </returns>
							Atom *createAtom(const std::wstring &elementSymbol, Fragment *frag);

							/// <summary>
							/// Create a new Atom of the given element belonging to the given fragment </summary>
							/// <param name="chemEl"> </param>
							/// <param name="frag"> </param>
							/// <returns> Atom </returns>
							Atom *createAtom(ChemEl chemEl, Fragment *frag);

							/// <summary>
							/// Create a new bond between two atoms.
							/// The bond is associated with these atoms. </summary>
							/// <param name="fromAtom"> </param>
							/// <param name="toAtom"> </param>
							/// <param name="bondOrder"> </param>
							/// <returns> Bond </returns>
							Bond *createBond(Atom *fromAtom, Atom *toAtom, int bondOrder);

							bool is_A_to_Z(wchar_t ch);

							bool is_a_to_z(wchar_t ch);

							bool is_0_to_9(wchar_t ch);

						};

					}
				}
			}
		}
	}
}
