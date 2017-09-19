#pragma once

#include <vector>
#include <stdexcept>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
class Atom;


/// <summary>
/// Convenience class for iterating over a list of atoms that form a ring
/// Doing getNext when the index is the final atom in the list will return the first atom
/// Doing getPrevious when the index is the first atom in the list will return the final atom
/// @author dl387
///
/// </summary>
class CyclicAtomList {
private:
    int index = -1;
    const std::vector<Atom *> atomList;

    /// <summary>
    /// Construct a cyclicAtomList from an atomList
    /// Index defaults to -1 </summary>
    /// <param name="atomList"> </param>
public:
    CyclicAtomList(std::vector<Atom *> &atomList);

    /// <summary>
    /// Construct a cyclicAtomList from an atomList
    /// The second parameter sets the current index </summary>
    /// <param name="atomList"> </param>
    /// <param name="index"> </param>
    CyclicAtomList(std::vector<Atom *> &atomList, int index);

    /// <summary>
    /// Returns the number of elements in this list. If this list contains more
    /// than <tt>Integer.MAX_VALUE</tt> elements, returns
    /// <tt>Integer.MAX_VALUE</tt>.
    /// </summary>
    /// <returns> the number of elements in this list </returns>
    virtual int size();

    /// <summary>
    /// Returns the atom at the specified position in this list. </summary>
    /// <param name="index"> index of the element to return </param>
    /// <returns> Atom the atom at the specified position in this list </returns>
    /// <exception cref="IndexOutOfBoundsException"> - if the index is out of range (index < 0 || index >= size()) </exception>
    virtual Atom *get(int index) throw(std::out_of_range);

    /// <summary>
    /// Return the current index in the list
    /// @return
    /// </summary>
    virtual int getIndex();

    /// <summary>
    /// Set the current index </summary>
    /// <param name="index"> </param>
    virtual void setIndex(int index);

    /// <summary>
    /// Increments and returns the atom at the new index in the list (next atom)
    /// When the index is the final atom in the list will return the first atom
    /// @return
    /// </summary>
    virtual Atom *next();

    /// <summary>
    /// Decrements and returns the atom at the new index in the list (previous atom)
    /// when the index is the first atom in the list will return the final atom
    /// @return
    /// </summary>
    virtual Atom *previous();

    /// <summary>
    /// Returns the next atom in the list
    /// When the index is the final atom in the list will return the first atom
    /// Doesn't effect the list
    /// @return
    /// </summary>
    virtual Atom *peekNext();

    /// <summary>
    /// Returns the previous atom in the list
    /// when the index is the first atom in the list will return the final atom
    /// Doesn't effect the list
    /// @return
    /// </summary>
    virtual Atom *peekPrevious();

    /// <summary>
    /// Returns the atom corresponding to the current index
    /// Note that CycliAtomLists have a default index of -1
    /// @return
    /// </summary>
    virtual Atom *getCurrent();
};

