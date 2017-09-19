#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <boost/optional.hpp>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class Atom;
class Bond;
class AtomNeighbouringColoursComparator;
class AtomicNumberThenAtomicMassComparator;
class CipOrderingException;
class Fragment;


/// <summary>
/// Identifies stereocentres and determines the CIP order of connected atoms
/// @author dl387
///
/// </summary>
class StereoAnalyser {
    /// <summary>
    /// The atoms/bonds upon which this StereoAnalyser is operating </summary>
private:
    Collection<Atom *> *const atoms;
    Collection<Bond *> *const bonds;

    /// <summary>
    /// Maps each atom to its currently assigned colour. Eventually all atoms in non identical environments will have different colours. Higher is higher priority </summary>
    const std::unordered_map<Atom *, int> mappingToColour;

    /// <summary>
    /// Maps each atom to an array of the colours of its neighbours </summary>
    const std::unordered_map<Atom *, std::vector<int>> atomNeighbourColours;

    AtomNeighbouringColoursComparator *const atomNeighbouringColoursComparator = new AtomNeighbouringColoursComparator(
            this);
    static AtomicNumberThenAtomicMassComparator *const atomicNumberThenAtomicMassComparator = new AtomicNumberThenAtomicMassComparator();

    /// <summary>
    /// Holds information about a tetrahedral stereocentre
    /// @author dl387
    ///
    /// </summary>
public:
    class StereoCentre {
    private:
        StereoAnalyser *outerInstance;

        Atom *const stereoAtom;
        const bool trueStereoCentre;

        /// <summary>
        /// Creates a stereocentre object from a tetrahedral stereocentre atom </summary>
        /// <param name="stereoAtom"> </param>
    public:
        virtual ~StereoCentre() {
            delete stereoAtom;
            delete outerInstance;
        }

        StereoCentre(StereoAnalyser *outerInstance, Atom *stereoAtom, boost::optional<bool> isTrueStereoCentre);

        virtual Atom *getStereoAtom();

        /// <summary>
        /// Does this atom have 4 constitutionally different groups (or 3 and a lone pair)
        /// or is it only a stereo centre due to the presence of other centres in the molecule
        /// @return
        /// </summary>
        virtual bool isTrueStereoCentre();

        virtual std::vector<Atom *> getCipOrderedAtoms() throw(CipOrderingException);
    };

    /// <summary>
    ///*
    /// Holds information about a double bond that can possess E/Z stereochemistry
    /// @author dl387
    ///
    /// </summary>
public:
    class StereoBond {
    private:
        StereoAnalyser *outerInstance;

        Bond *const bond;
    public:
        virtual ~StereoBond() {
            delete bond;
            delete outerInstance;
        }

        StereoBond(StereoAnalyser *outerInstance, Bond *bond);

        virtual Bond *getBond();

        /// <summary>
        /// Returns the following atoms:
        /// Highest CIP atom on one side
        /// atom in bond
        /// other atom in bond
        /// Highest CIP atom on other side
        /// @return </summary>
        /// <exception cref="CipOrderingException">  </exception>
        virtual std::vector<Atom *> getOrderedStereoAtoms() throw(CipOrderingException);
    };

    /// <summary>
    /// Sorts atoms by their atomic number, low to high
    /// In the case of a tie sorts by atomic mass
    /// @author dl387
    ///
    /// </summary>
private:
    class AtomicNumberThenAtomicMassComparator : public Comparator<Atom *> {
    public:
        virtual int compare(Atom *a, Atom *b);
    };

public:
    virtual ~StereoAnalyser() {
        delete atoms;
        delete bonds;
        delete atomNeighbouringColoursComparator;
    }

private:
    static int compareAtomicNumberThenAtomicMass(Atom *a, Atom *b);

    /// <summary>
    /// Sorts based on the list of colours for neighbouring atoms
    /// e.g. [1,2] > [1,1]  [1,1,3] > [2,2,2]  [1,1,3] > [3]
    /// @author dl387
    ///
    /// </summary>
private:
    class AtomNeighbouringColoursComparator : public Comparator<Atom *> {
    private:
        StereoAnalyser *outerInstance;

    public:
        virtual ~AtomNeighbouringColoursComparator() {
            delete outerInstance;
        }

        AtomNeighbouringColoursComparator(StereoAnalyser *outerInstance);

        virtual int compare(Atom *a, Atom *b);
    };

    /// <summary>
    /// Employs a derivative of the InChI algorithm to label which atoms are equivalent.
    /// These labels can then be used by the findStereo(Atoms/Bonds) functions to find features that
    /// can possess stereoChemistry </summary>
    /// <param name="molecule"> </param>
public:
    StereoAnalyser(Fragment *molecule);

    /// <summary>
    /// Employs a derivative of the InChI algorithm to label which atoms are equivalent.
    /// These labels can then be used by the findStereo(Atoms/Bonds) functions to find features that
    /// can possess stereoChemistry
    /// NOTE: All bonds of every atom must be in the set of bonds, no atom may have a bond to an atom not in the list </summary>
    /// <param name="atoms"> </param>
    /// <param name="bonds"> </param>
    StereoAnalyser(Collection<Atom *> *atoms, Collection<Bond *> *bonds);

    /// <summary>
    /// Adds "ghost" atoms in the same way as the CIP rules for handling double bonds
    /// e.g. C=C --> C(G)=C(G) where ghost is a carbon with no hydrogen bonded to it </summary>
    /// <returns> The ghost atoms created </returns>
private:
    std::vector<Atom *> addGhostAtoms();

    /// <summary>
    /// Removes the ghost atoms added by addGhostAtoms </summary>
    /// <param name="ghostAtoms">  </param>
    void removeGhostAtoms(std::vector<Atom *> &ghostAtoms);


    /// <summary>
    /// Takes a list of atoms sorted by atomic number/mass
    /// and populates the mappingToColour map </summary>
    /// <param name="atomList"> </param>
    /// <returns>  </returns>
    std::vector<std::vector<Atom *>> populateColoursByAtomicNumberAndMass(std::vector<Atom *> &atomList);

    /// <summary>
    /// Takes the lists of atoms pre-grouped by colour and sorts each by its neighbours colours
    /// The updatedGroupsByColour is populated with those for which this process caused a change
    /// and populates the mappingToColour map
    /// Returns whether mappingToColour was changed </summary>
    /// <param name="groupsByColour"> </param>
    /// <param name="updatedGroupsByColour"> </param>
    /// <returns> boolean Whether mappingToColour was changed </returns>
    bool populateColoursAndReportIfColoursWereChanged(std::vector<std::vector<Atom *>> &groupsByColour,
                                                      std::vector<std::vector<Atom *>> &updatedGroupsByColour);

    /// <summary>
    /// Produces a sorted (low to high) array of the colour of the atoms surrounding a given atom </summary>
    /// <param name="atom"> </param>
    /// <returns> int[] colourOfAdjacentAtoms </returns>
    std::vector<int> findColourOfNeighbours(Atom *atom);

    /// <summary>
    /// Retrieves a list of any tetrahedral stereoCentres
    /// Internally this is done by checking whether the "colour" of all neighbouring atoms of the tetrahedral atom are different </summary>
    /// <returns> List<StereoCentre> </returns>
public:
    virtual std::vector<StereoCentre *> findStereoCentres();

    /// <summary>
    /// Retrieves atoms that pass the isPossiblyStereogenic() criteria
    /// @return
    /// </summary>
private:
    std::vector<Atom *> getPotentialStereoCentres();

    /// <summary>
    /// Checks whether the atom has 3 or 4 neighbours all of which are constitutionally different </summary>
    /// <param name="potentialStereoAtom">
    /// @return </param>
    bool isTrueStereCentre(Atom *potentialStereoAtom);

    /// <summary>
    /// Finds a subset of the stereocentres associated with rule 2 from:
    /// DOI: 10.1021/ci00016a003 </summary>
    /// <param name="potentialStereoAtoms"> </param>
    /// <param name="trueStereoCentres">  </param>
    std::vector<Atom *>
    findParaStereoCentres(std::vector<Atom *> &potentialStereoAtoms, std::vector<Atom *> &trueStereoCentres);


    bool branchesHaveTrueStereocentre(Atom *branchAtom1, Atom *branchAtom2, Atom *potentialStereoAtom,
                                      std::vector<Atom *> &trueStereoCentres);

    /// <summary>
    /// Checks whether an atom could be a tetrahedral stereocentre by checking that it is both tetrahedral
    /// and does not have neighbours that are identical due to resonance/tautomerism </summary>
    /// <param name="atom">
    /// @return </param>
public:
    static bool isPossiblyStereogenic(Atom *atom);

    /// <summary>
    /// Roughly corresponds to the list of atoms in table 8 of the InChI manual
    /// Essentially does a crude check for whether an atom is known to be able to possess tetrahedral geometry
    /// and whether it is currently tetrahedral. Atoms that are tetrahedral but not typically considered chiral
    /// like tertiary amines are not recognised </summary>
    /// <param name="atom">
    /// @return </param>
    static bool isKnownPotentiallyStereogenic(Atom *atom);

private:
    static bool atomsContainABondBetweenThemselves(std::vector<Atom *> &atoms);

public:
    static bool isAchiralDueToResonanceOrTautomerism(Atom *atom);

private:
    static bool isOnlyBondedToHydrogensOtherThanGivenAtom(Atom *atom, Atom *attachedNonHydrogen);

    /// <summary>
    ///  Retrieves a list of any double bonds possessing the potential to have E/Z stereoChemistry
    ///  This is done internally by checking the two atoms attached to the ends of the double bond are different
    ///  As an exception nitrogen's lone pair is treated like a low priority group and so is allowed to only have 1 atom connected to it
    /// @return
    /// </summary>
public:
    virtual std::vector<StereoBond *> findStereoBonds();

    /// <summary>
    /// Returns a number describing the environment of an atom. Atoms with the same number are in identical environments
    /// Null if atom was not part of this environment analysis </summary>
    /// <param name="a">
    /// @return </param>
    virtual boost::optional<int> getAtomEnvironmentNumber(Atom *a);
};

