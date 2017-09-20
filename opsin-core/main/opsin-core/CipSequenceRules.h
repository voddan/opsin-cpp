#pragma once

#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <stdexcept>
#include <boost/optional.hpp>
#include <utils/Comparator.hpp>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class Atom;
class CipOrderingException;
class AtomWithHistory;
class AtomListCipComparator;
class ListOfAtomListsCipComparator;
class CipComparator;


/// <summary>
/// An implementation of rules 1-2 of the CIP rules i.e. constitutional differences then isotopes if there is a tie
/// Cases that require rules 3-5 to distinguish result in an exception
///
/// Phantom atoms are not added as I believe that the results of the program will still be the same even in their absence as everything beats a phantom and comparing phantoms to phantoms achieves nothing
/// (higher ligancy beats lower ligancy when comparisons are performed)
/// @author dl387
///
/// </summary>
class CipSequenceRules {
private:
    class CipOrderingRunTimeException : public std::exception {
    private:
        static constexpr long long serialVersionUID = 1LL;
    public:
        CipOrderingRunTimeException(const std::wstring &message);
    };

private:
    Atom *const chiralAtom;

public:
    virtual ~CipSequenceRules() {
        delete chiralAtom;
    }

    CipSequenceRules(Atom *chiralAtom);

    /// <summary>
    /// Returns the chiral atom's neighbours in CIP order from lowest priority to highest priority
    /// @return </summary>
    /// <exception cref="CipOrderingException">  </exception>
    virtual std::vector<Atom *> getNeighbouringAtomsInCipOrder() throw(CipOrderingException);

    /// <summary>
    /// Returns  the chiral atom's neighbours, with the exception of the given atom, in CIP order from lowest priority to highest priority </summary>
    /// <param name="neighbourToIgnore">
    /// @return </param>
    /// <exception cref="CipOrderingException">  </exception>
    virtual std::vector<Atom *>
    getNeighbouringAtomsInCipOrderIgnoringGivenNeighbour(Atom *neighbourToIgnore) throw(CipOrderingException);


    /// <summary>
    /// Holds information about what atoms to try next next and how those atoms were reached (to prevent immediate back tracking and to detect cycles)
    /// @author dl387
    ///
    /// </summary>
private:
    class CipState {
    public:
        CipState(std::vector<AtomWithHistory *> &nextAtoms1, std::vector<AtomWithHistory *> &nextAtoms2);

        const std::vector<AtomWithHistory *> nextAtoms1;
        const std::vector<AtomWithHistory *> nextAtoms2;
    };

    /// <summary>
    /// Holds an atom with associated visited atoms
    /// @author dl387
    ///
    /// </summary>
private:
    class AtomWithHistory {
    public:
        virtual ~AtomWithHistory() {
            delete atom;
        }

        AtomWithHistory(Atom *atom, std::vector<Atom *> &visitedAtoms, boost::optional<int> indexOfOriginalFromRoot);

        Atom *const atom;
        const std::vector<Atom *> visitedAtoms;
        const boost::optional<int> indexOfOriginalFromRoot;
    };

    /// <summary>
    /// Sorts atoms by their CIP order, low to high
    /// @author dl387
    ///
    /// </summary>
private:
    class SortByCipOrder : public Comparator<Atom> {
    private:
        CipSequenceRules *outerInstance;

        Atom *const chiralAtom;
        AtomListCipComparator *const atomListCipComparator = new AtomListCipComparator(this);
        ListOfAtomListsCipComparator *const listOfAtomListsCipComparator = new ListOfAtomListsCipComparator(this);
        CipComparator *const cipComparator = new CipComparator(this);
        int rule = 0;


    public:
        virtual ~SortByCipOrder() {
            delete chiralAtom;
            delete atomListCipComparator;
            delete listOfAtomListsCipComparator;
            delete cipComparator;
            delete outerInstance;
        }

        SortByCipOrder(CipSequenceRules *outerInstance, Atom *chiralAtom);

        virtual int compare(Atom *a, Atom *b) override;

        /// <summary>
        /// Compares the neighbours of the atoms specified in nextAtom1/2 in cipstate.
        /// Returns the result of the comparison between these neighbours
        /// If the comparison returned 0 adds new cipstates to the queue </summary>
        /// <param name="cipState"> </param>
        /// <param name="queue">
        /// @return </param>
    private:
        int compareAtNextLevel(CipState *cipState, std::list<CipState *> &queue);

        int compareNeighboursByCipPriorityRules(std::vector<std::vector<AtomWithHistory *>> &neighbours1,
                                                std::vector<std::vector<AtomWithHistory *>> &neighbours2);

        std::vector<std::vector<AtomWithHistory *>> getNextLevelNeighbours(std::vector<AtomWithHistory *> &nextAtoms);

        /// <summary>
        /// If given say [H,C,C] this becomes [H] [C,C]
        /// If given say [H,C,C] [H,C,C] this becomes [H,H] [C,C,C,C]
        /// If given say [H,C,C] [H,C,F] this becomes [H],[C,C][H][C][F]
        /// as [H,C,F] is higher priority than [H,C,C] so all its atoms must be evaluated first
        /// The input lists of neighbours are assumed to have been presorted. </summary>
        /// <param name="neighbourLists"> </param>
        std::vector<std::vector<AtomWithHistory *>>
        formListsWithSamePriority(std::vector<std::vector<AtomWithHistory *>> &neighbourLists);


        /// <summary>
        /// Sorts atoms by their atomic number, low to high
        /// @author dl387
        ///
        /// </summary>
    private:
        class CipComparator : public Comparator<AtomWithHistory> {
        private:
            CipSequenceRules::SortByCipOrder *outerInstance;

        public:
            virtual ~CipComparator() {
                delete outerInstance;
            }

            CipComparator(CipSequenceRules::SortByCipOrder *outerInstance);

            virtual int compare(AtomWithHistory *a, AtomWithHistory *b)  override;
        };

        /// <summary>
        /// Sorts atomLists by CIP rules, low to high
        /// @author dl387
        ///
        /// </summary>
    private:
        class AtomListCipComparator : public Comparator<std::vector<AtomWithHistory *>> {
        private:
            CipSequenceRules::SortByCipOrder *outerInstance;

        public:
            virtual ~AtomListCipComparator() {
                delete outerInstance;
            }

            AtomListCipComparator(CipSequenceRules::SortByCipOrder *outerInstance);

            virtual int compare(std::vector<AtomWithHistory *> *a, std::vector<AtomWithHistory *> *b) override;
        };

        /// <summary>
        /// Sorts lists of atomLists by CIP rules, low to high
        /// @author dl387
        ///
        /// </summary>
    private:
        class ListOfAtomListsCipComparator : public Comparator<std::vector<std::vector<AtomWithHistory *>>> {
        private:
            CipSequenceRules::SortByCipOrder *outerInstance;

        public:
            virtual ~ListOfAtomListsCipComparator() {
                delete outerInstance;
            }

            ListOfAtomListsCipComparator(CipSequenceRules::SortByCipOrder *outerInstance);

            virtual int
            compare(std::vector<std::vector<AtomWithHistory *>> &a, std::vector<std::vector<AtomWithHistory *>> &b);
        };

        /// <summary>
        /// Gets the neighbouring atoms bar the previous atom in CIP order
        /// If the neighbouring atom has already been visited it is replaced with a ghost atom
        /// Multiple bonds including those to previous atoms yield ghost atoms unless the bond goes to the chiral atom e.g. in a sulfoxide </summary>
        /// <param name="atoms">
        /// @return </param>
    private:
        std::vector<AtomWithHistory *> getNextAtomsWithAppropriateGhostAtoms(AtomWithHistory *atomWithHistory);

        /// <summary>
        /// Greater than 0 means a is preferred over b (vice versa for less than 1) </summary>
        /// <param name="a"> </param>
        /// <param name="b">
        /// @return </param>
        int compareByCipRules(AtomWithHistory *a, AtomWithHistory *b);
    };

};

