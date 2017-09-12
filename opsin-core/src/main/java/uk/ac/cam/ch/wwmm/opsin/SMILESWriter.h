#pragma once

#include "ChemEl.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/optional.hpp>
#include "stringbuilder.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class Bond; } } } } } }
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class Atom; } } } } } }
namespace uk { namespace ac { namespace cam { namespace ch { namespace wwmm { namespace opsin { class Fragment; } } } } } }

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
						/// Writes an isomeric SMILES serialisation of an OPSIN fragment
						/// @author dl387
						/// 
						/// </summary>
						class SMILESWriter
						{

							/// <summary>
							///The organic atoms and their allowed implicit valences in SMILES </summary>
						private:
							static const std::unordered_map<ChemEl, std::vector<Integer>> organicAtomsToStandardValencies;

							/// <summary>
							///Closures 1-9, %10-99, 0 </summary>
							static const std::vector<std::wstring> closureSymbols;


							/// <summary>
							///The available ring closure symbols, ordered from start to end in the preferred order for use. </summary>
							Deque<std::wstring> *const availableClosureSymbols = new ArrayDeque<std::wstring>(closureSymbols);

							/// <summary>
							///Maps between bonds and the ring closure to use when the atom that ends the bond is encountered. </summary>
							const std::unordered_map<Bond*, std::wstring> bondToClosureSymbolMap = std::unordered_map<Bond*, std::wstring>();

							/// <summary>
							///Maps between bonds and the atom that this bond will go to in the SMILES. Populated in the order the bonds are to be made </summary>
							const std::unordered_map<Bond*, Atom*> bondToNextAtomMap = new LinkedHashMap<Bond*, Atom*>();

							/// <summary>
							///The structure to be converted to SMILES </summary>
							Fragment *const structure;

							/// <summary>
							///Holds the SMILES string which is under construction </summary>
							StringBuilder *const smilesBuilder = new StringBuilder();

							/// <summary>
							///Should extended SMILES be output </summary>
							const bool outputExtendedSmiles;

							/// <summary>
							///The order atoms were traversed when creating the SMILES </summary>
							std::vector<Atom*> smilesOutputOrder;

												private:
													class StaticConstructor
													{
													public:
														StaticConstructor();
													};

												private:
													static SMILESWriter::StaticConstructor staticConstructor;


							/// <summary>
							/// Creates a SMILES writer for the given fragment </summary>
							/// <param name="structure"> </param>
							/// <param name="outputExtendedSmiles">  </param>
						public:
							virtual ~SMILESWriter()
							{
								delete availableClosureSymbols;
								delete structure;
								delete smilesBuilder;
							}

						private:
							SMILESWriter(Fragment *structure, bool outputExtendedSmiles);

							/// <summary>
							/// Generates SMILES for the given fragment
							/// The following assumptions are currently made:
							/// 	The fragment contains no bonds to atoms outside the fragment
							/// 	Hydrogens are all explicit
							/// 	Spare valency has been converted to double bonds </summary>
							/// <returns> SMILES String </returns>
						public:
							static std::wstring generateSmiles(Fragment *structure);

							/// <summary>
							/// Generates extended SMILES for the given fragment
							/// The following assumptions are currently made:
							/// 	The fragment contains no bonds to atoms outside the fragment
							/// 	Hydrogens are all explicit
							/// 	Spare valency has been converted to double bonds </summary>
							/// <returns> Extended SMILES String </returns>
							static std::wstring generateExtendedSmiles(Fragment *structure);

							virtual std::wstring writeSmiles();

						private:
							void writeExtendedSmilesLayer();

							std::wstring escapeExtendedSmilesLabel(const std::wstring &str);

							/// <summary>
							/// Walks through the fragment populating the Atom.VISITED property indicating how many bonds
							/// an atom is from the start of the fragment walk. A new walk will be started for each disconnected component of the fragment
							/// </summary>
							void assignSmilesOrder();

						private:
							class TraversalState
							{
							private:
								Atom *const atom;
								Bond *const bondTaken;
								const int depth;

							public:
								virtual ~TraversalState()
								{
									delete atom;
									delete bondTaken;
								}

							private:
								TraversalState(Atom *atom, Bond *bondTaken, int depth);
							};

							/// <summary>
							/// Iterative function for populating the Atom.VISITED property 
							/// Also populates the bondToNextAtom Map </summary>
							/// <param name="startingAtom">
							/// @return </param>
						private:
							void traverseMolecule(Atom *startingAtom);

							bool isSmilesImplicitProton(Atom *atom);

							/// <summary>
							/// Goes through the bonds with BondStereo in the order the are to be created in the SMILES
							/// The bondStereo is used to set whether the bonds to non-implicit hydrogens that are adjacent to this bond
							/// should be be represented by / or \ in the SMILES. If this method has already set the slash on some bonds
							/// e.g. in a conjugated system this must be taken into account when setting the next slashes so as to not
							/// create a contradictory double bond stereochemistry definition.
							/// </summary>
							void assignDoubleBondStereochemistrySlashes();


							static TraversalState *const startBranch = new TraversalState(nullptr, nullptr, -1);
							static TraversalState *const endBranch = new TraversalState(nullptr, nullptr, -1);

							/// <summary>
							/// Generates the SMILES starting from the currentAtom, iteratively exploring
							/// in the same order as <seealso cref="SMILESWriter#traverseMolecule(Atom)"/> </summary>
							/// <param name="startingAtom"> </param>
							void traverseSmiles(Atom *startingAtom);

							/// <summary>
							/// Returns the SMILES describing the given atom.
							/// Where possible square brackets are not included to give more readable SMILES </summary>
							/// <param name="atom"> </param>
							/// <param name="depth"> </param>
							/// <param name="bondtaken">
							/// @return </param>
							std::wstring atomToSmiles(Atom *atom, int depth, Bond *bondtaken);

							int calculateNumberOfBondedExplicitHydrogen(Atom *atom);

							bool determineWhetherAtomNeedsSquareBrackets(Atom *atom, int hydrogenCount);

							std::wstring atomParityToSmiles(Atom *currentAtom, int depth, Bond *bondtaken);

							/// <summary>
							/// Generates the SMILES description of the bond
							/// In the case of cis/trans stereochemistry this relies on the <seealso cref="SMILESWriter#assignDoubleBondStereochemistrySlashes"/>
							/// having been run to setup the smilesBondDirection attribute </summary>
							/// <param name="bond">
							/// @return </param>
							std::wstring bondToSmiles(Bond *bond);
						};

					}
				}
			}
		}
	}
}
