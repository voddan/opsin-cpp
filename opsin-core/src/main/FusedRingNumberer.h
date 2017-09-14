#pragma once

#include "ChemEl.h"
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <boost/optional.hpp>
#include "stringbuilder.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class RingShape;
class Ring;
class Atom;
class Fragment;
class StructureBuildingException;
class Bond;


using org::apache::log4j::Logger;

/// <summary>
/// Numbers fusedRings
/// @author aa593
/// @author dl387
///
/// </summary>
class FusedRingNumberer {

private:
    static Logger *const LOG = Logger::getLogger(FusedRingNumberer::
    typeid);
private:
    class RingConnectivityTable {
    public:
        const std::vector<RingShape *> ringShapes = std::vector<RingShape *>();
        const std::vector<Ring *> neighbouringRings = std::vector<Ring *>();
        const std::vector<int> directionFromRingToNeighbouringRing = std::vector<int>();
        const std::vector<Ring *> usedRings = std::vector<Ring *>();

        virtual RingConnectivityTable *copy();
    };

    /// <summary>
    /// Wrapper for a ring of a fused ring system with the shape that ring is currently being treated as having
    /// @author dl387
    ///
    /// </summary>
private:
    class RingShape {
    private:
        Ring *const ring;
        const FusionRingShape shape;
    public:
        virtual ~RingShape() {
            delete ring;
        }

        RingShape(Ring *ring, FusionRingShape shape);

        virtual Ring *getRing();

        virtual FusionRingShape getShape();
    };

public:
    enum class FusionRingShape {
        enterFromLeftHouse, //5 membered ring
        enterFromTopLeftHouse, //5 membered ring
        enterFromTopRightHouse, //5 membered ring
        enterFromRightHouse, //5 membered ring
        enterFromLeftSevenMembered, //7 membered ring
        enterFromTopSevenMembered, //7 membered ring
        enterFromRightSevenMembered, //7 membered ring
        enterFromBottomRightSevenMembered, //7 membered ring
        enterFromBottomLeftSevenMembered, //7 membered ring
        standard
    };

private:
    class Chain {
    private:
        const int length;
        const int startingX;
        const int y;

    public:
        Chain(int length, int startingX, int y);

        virtual int getLength();

        virtual int getStartingX();

        virtual int getY();
    };

    /// <summary>
    /// Sorts by atomSequences by the IUPAC rules for determining the preferred labelling
    /// The most preferred will be sorted to the back (0th position)
    /// @author dl387
    ///
    /// </summary>
private:
    class SortAtomSequences : public Comparator<std::vector<Atom *>> {

    public:
        virtual int compare(std::vector<Atom *> &sequenceA, std::vector<Atom *> &sequenceB);
    };

private:
    static const std::unordered_map<ChemEl, int> heteroAtomValues;
private:
    class StaticConstructor {
    public:
        StaticConstructor();
    };

private:
    static FusedRingNumberer::StaticConstructor staticConstructor;

    /*
     * The meaning of the integers used is as follows:
     *        2
     *    3   ^  1
     *      \ | /
     * +-4 <-   -> 0
     *      / | \
     *   -3   v  -1
     *       -2
     *
     * They indicate the relative directions between rings
     * Possibly enums should be used...
     */

    /// <summary>
    /// Numbers the fused ring
    /// Works reliably for all common ring systems.
    /// Some complex fused ring systems involving multiple connections to rings with an odd number of edges may still be wrong </summary>
    /// <param name="fusedRing"> </param>
    /// <exception cref="StructureBuildingException"> </exception>
public:
    static void numberFusedRing(Fragment *fusedRing) throw(StructureBuildingException);

    /// <summary>
    /// Populates rings with their neighbouring fused rings and the bonds involved </summary>
    /// <param name="rings"> </param>
    static void setupAdjacentFusedRingProperties(std::vector<Ring *> &rings);

    /// <summary>
    /// Checks that all the rings are of sizes 3-8 or if larger than 8 are involved in 2 or fewer fused bonds </summary>
    /// <param name="rings">
    /// @return </param>
private:
    static bool checkRingApplicability(std::vector<Ring *> &rings);

    /// <summary>
    /// Returns possible enumerations of atoms. Currently Interior atoms are not considered.
    /// These enumerations will be compliant with rules FR5.1-FR5.3 of the fused ring nomenclature guidelines
    /// http://www.chem.qmul.ac.uk/iupac/fusedring/FR51.html </summary>
    /// <param name="rings"> </param>
    /// <param name="atomCountOfFusedRingSystem">
    /// @return </param>
    /// <exception cref="StructureBuildingException"> </exception>
    static std::vector<std::vector<Atom *>> determinePossiblePeripheryAtomOrders(std::vector<Ring *> &rings,
                                                                                 int atomCountOfFusedRingSystem) throw(StructureBuildingException);

    /// <summary>
    /// Finds the rings with the minimum number of fused bonds </summary>
    /// <param name="rings">
    /// @return </param>
    static std::vector<Ring *> findTerminalRings(std::vector<Ring *> &rings);

    /// <summary>
    /// Recursive function to create the connectivity table of the rings, for each connection includes both directions </summary>
    /// <param name="currentRing"> </param>
    /// <param name="previousRing"> </param>
    /// <param name="previousDir"> </param>
    /// <param name="previousBond"> </param>
    /// <param name="atom"> </param>
    /// <param name="ct"> </param>
    /// <param name="cts">
    /// @return </param>
    static std::vector<RingConnectivityTable *>
    buildRingConnectionTables(Ring *currentRing, Ring *previousRing, int previousDir, Bond *previousBond, Atom *atom,
                              RingConnectivityTable *ct, std::vector<RingConnectivityTable *> &cts);

    /// <summary>
    /// Returns the allowed shapes for the given ring.
    /// The starting bond is required to assured that elongated bonds do not unnecesarily correspond to fusions
    /// Currently only 5 membered rings are considered in multiple orientations but the same
    /// is probably required for 7+ member rings </summary>
    /// <param name="ring"> </param>
    /// <param name="startingBond">
    /// @return </param>
    static std::vector<FusionRingShape> getAllowedShapesForRing(Ring *ring, Bond *startingBond);

    /// <summary>
    /// Removes the ring shapes that for given distances have identical properties </summary>
    /// <param name="allowedRingShapes"> </param>
    /// <param name="distances"> </param>
    /// <param name="ringSize">  </param>
    static std::vector<FusionRingShape>
    removeDegenerateRingShapes(std::vector<FusionRingShape> &allowedRingShapes, std::vector<int> &distances,
                               int ringSize);

    /// <summary>
    /// Calculates the direction of the next ring according to the distance between fusion bonds and the previous direction </summary>
    /// <param name="ringShape"> </param>
    /// <param name="previousBond"> </param>
    /// <param name="currentBond"> </param>
    /// <param name="previousDir">
    /// @return </param>
    static int calculateRingDirection(RingShape *ringShape, Bond *previousBond, Bond *currentBond, int previousDir);

    /// <summary>
    /// Given two bonds on a ring returns the distance (in bonds) between them </summary>
    /// <param name="bond1"> </param>
    /// <param name="bond2"> </param>
    /// <param name="ring">
    /// @return </param>
    static int calculateDistanceBetweenBonds(Bond *bond1, Bond *bond2, Ring *ring);

    /// <summary>
    /// Uses the ring shape, the ring size and distance between the incoming and outgoing fused bond to determine
    /// the relative direction between the entry point on the ring and the exit point </summary>
    /// <param name="fusionRingShape"> </param>
    /// <param name="ringSize"> </param>
    /// <param name="dist">
    /// @return </param>
    static int getDirectionFromDist(FusionRingShape fusionRingShape, int ringSize, int dist);

    static void removeCTsWithDistortedRingShapes(std::vector<RingConnectivityTable *> &cts);

    /// <summary>
    /// Given a list of cts find the longest chain of rings in a line. This can be used to find a possible horizontal row
    /// The output is a map between the connection tables and the directions which give the longest chains
    /// Some cts may have no directions that give a chain of rings of this length
    /// </summary>
    /// <param name="cts">
    /// @return </param>
    static std::unordered_map<RingConnectivityTable *, std::vector<int>>
    findLongestChainDirections(std::vector<RingConnectivityTable *> &cts);

    /// <summary>
    /// Given a list of ringShapes finds the indice of the ringShape corresponding to the given ring
    /// returns -1 if this is not possible </summary>
    /// <param name="ringShapes"> </param>
    /// <param name="ring">
    /// @return </param>
    static int indexOfCorrespondingRingshape(std::vector<RingShape *> &ringShapes, Ring *ring);


    /// <summary>
    /// For each RingConnectivityTable and for each horizontal row direction creates a ringMap aligned along the given horizontal row direction </summary>
    /// <param name="horizonalRowDirectionsMap">
    /// @return </param>
    /// <exception cref="StructureBuildingException"> </exception>
    static std::vector<std::vector<std::vector<Ring *>>> createRingMapsAlignedAlongGivenhorizonalRowDirections(
            std::unordered_map<RingConnectivityTable *, std::vector<int>> &horizonalRowDirectionsMap) throw(StructureBuildingException);

    /// <summary>
    /// Applies FR5.2 B, C and D to determine the preferred orientation and returns lists of potential peripheral atom orderings </summary>
    /// <param name="ringMaps"> </param>
    /// <param name="atomCountOfFusedRingSystem">
    /// @return </param>
    static std::vector<std::vector<Atom *>>
    findPossiblePaths(std::vector<std::vector<std::vector<Ring *>>> &ringMaps, int atomCountOfFusedRingSystem);

    static std::vector<std::vector<Ring *>>
    generateRingMap(RingConnectivityTable *ct, std::vector<int> &directionFromRingToNeighbouringRing);

    /// <summary>
    /// Finds all the chains of maximum length for the current direction </summary>
    /// <param name="ringMap">
    /// @return </param>
    static std::vector<Chain *> findChainsOfMaximumLengthInHorizontalDir(std::vector<std::vector<Ring *>> &ringMap);

    /// <summary>
    /// Counts number of rings in each quadrant </summary>
    /// <param name="ringMap"> </param>
    /// <param name="midChainXcoord"> </param>
    /// <param name="yChain">
    /// @return </param>
    static std::vector<boost::optional<double>>
    countQuadrants(std::vector<std::vector<Ring *>> &ringMap, int midChainXcoord, int yChain);

    /// <summary>
    /// Applying rules FR5.2 B, C and D to the ring system.
    /// Return a list of possible upper right quadrants for each chain given. A chain may have multiple possible upper right quadrants (due to symmetry)
    /// or none if other chains can be shown to be preferable by application of the rules </summary>
    /// <param name="chainQs"> - array with number of ring in each quadrant for each chain. </param>
    static std::vector<std::vector<int>> rulesBCD(std::vector<std::vector<double>> &chainQs);

    /// <summary>
    /// Enumerates the peripheral atoms in a system in accordance with FR-5.3:
    /// First finds the uppermost right ring, takes the next neighbour in the clockwise direction, and so on until the starting atom is reached </summary>
    /// <param name="ringMap"> </param>
    /// <param name="inverseAtoms"> The direction in which the periphery atoms should be enumerated. Anticlockwise by default </param>
    /// <param name="atomCountOfFusedRingSystem">
    /// @return </param>
    static std::vector<Atom *>
    orderAtoms(std::vector<std::vector<Ring *>> &ringMap, bool inverseAtoms, int atomCountOfFusedRingSystem);

    static bool isEntirelyFusionAtoms(Ring *upperRightRing);

    /// <summary>
    /// Finds the neighbour ring, which is the clockwise of the given ring. </summary>
    /// <param name="ringMap"> </param>
    /// <param name="upperRightRing"> </param>
    /// <param name="visitedRings">
    /// @return </param>
    static Ring *findClockwiseRingFromUpperRightRing(std::vector<std::vector<Ring *>> &ringMap, Ring *upperRightRing,
                                                     std::vector<Ring *> &visitedRings);

    /// <summary>
    /// Finds the neighbour ring, which is the uppermost and on the left side from the given ring. Used to find previous bond for the uppermost right ring, from which we start to enumerate </summary>
    /// <param name="ringMap"> </param>
    /// <param name="upperRightRing">
    /// @return </param>
    static Ring *
    findUpperLeftNeighbourOfUpperRightRing(std::vector<std::vector<Ring *>> &ringMap, Ring *upperRightRing);

    /// <summary>
    /// Finds the position(i,j) of the ring in the map </summary>
    /// <param name="ringMap"> </param>
    /// <param name="ring">
    /// @return </param>
    static std::vector<int> findRingPosition(std::vector<std::vector<Ring *>> &ringMap, Ring *ring);

    /// <summary>
    /// Transform the map such that the candidate upper right quadrant actually is in the upper right corner </summary>
    /// <param name="ringMap"> </param>
    /// <param name="upperRightQuadrant">
    /// @return </param>
    static std::vector<std::vector<Ring *>>
    transformQuadrantToUpperRightOfRingMap(std::vector<std::vector<Ring *>> &ringMap, int upperRightQuadrant);

    /// <summary>
    /// Checks if array contains an object </summary>
    /// <param name="array"> </param>
    /// <param name="obj">
    /// @return </param>
    static bool arrayContains(std::vector<void *> &array_Renamed, void *obj);

    /// <summary>
    /// Returns a bond which is not a bond that is in two rings
    /// Preference is given to a bond that is at least a bond away from a fused bond to avoid problems with 5 member rings starting in bad orientations </summary>
    /// <param name="tRing">
    /// @return </param>
    static Bond *getStartingNonFusedBond(Ring *tRing);

    /// <summary>
    /// Given the direction of the bond from ring1 to ring2, returns the opposite direction: from ring2 to ring1 </summary>
    /// <param name="prevDir">
    /// @return </param>
public:
    static int getOppositeDirection(int prevDir);

    /// <summary>
    /// Finds the atom connected to the bond, takes into account the order of the bonds and atoms in the ring </summary>
    /// <param name="ring"> </param>
    /// <param name="curBond">
    /// @return </param>
private:
    static Atom *getAtomFromBond(Ring *ring, Bond *curBond);

    /// <summary>
    /// Finds the fusion bond between 2 rings </summary>
    /// <param name="r1"> </param>
    /// <param name="r2">
    /// @return </param>
    static Bond *findFusionBond(Ring *r1, Ring *r2);

    /// <summary>
    /// Counts delta x distance between previous and next rings </summary>
    /// <param name="val">
    /// @return </param>
    static float countDX(int val);

    /// <summary>
    /// Counts delta y distance (height) between previous and next rings </summary>
    /// <param name="val">
    /// @return </param>

    static int countDY(int val);

    /// <summary>
    /// Take into account the previous direction to convert the given relative direction into a direction that is absolute for the fused ring system </summary>
    /// <param name="fusionRingShape"> </param>
    /// <param name="ringSize"> </param>
    /// <param name="relativeDirection"> </param>
    /// <param name="previousDir">
    /// @return </param>
public:
    static int determineAbsoluteDirectionUsingPreviousDirection(FusionRingShape fusionRingShape, int ringSize,
                                                                int relativeDirection, int previousDir);

private:
    static void debugRingMap(std::vector<std::vector<Ring *>> &ringMap);
};

