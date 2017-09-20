#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <list>
#include <algorithm>
#include <limits>
#include <stdexcept>
#include <boost/optional.hpp>
#include <set>
#include "stringbuilder.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace dk { namespace brics { namespace automaton { class State; }}}
namespace dk { namespace brics { namespace automaton { class Transition; }}}
namespace dk { namespace brics { namespace automaton { class StatePair; }}}

/*
 * dk.brics.automaton
 * 
 * Copyright (c) 2001-2017 Anders Moeller
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

namespace dk {
    namespace brics {
        namespace automaton {


            /// <summary>
            /// Finite-state automaton with regular expression operations.
            /// <para>
            /// Class invariants:
            /// <ul>
            /// <li> An automaton is either represented explicitly (with <seealso cref="State"/> and <seealso cref="Transition"/> objects)
            ///      or with a singleton string (see <seealso cref="#getSingleton()"/> and <seealso cref="#expandSingleton()"/>) in case the automaton is known to accept exactly one string.
            ///      (Implicitly, all states and transitions of an automaton are reachable from its initial state.)
            /// <li> Automata are always reduced (see <seealso cref="#reduce()"/>)
            ///      and have no transitions to dead states (see <seealso cref="#removeDeadTransitions()"/>).
            /// <li> If an automaton is nondeterministic, then <seealso cref="#isDeterministic()"/> returns false (but
            ///      the converse is not required).
            /// <li> Automata provided as input to operations are generally assumed to be disjoint.
            /// </ul>
            /// </para>
            /// <para>
            /// If the states or transitions are manipulated manually, the <seealso cref="#restoreInvariant()"/>
            /// and <seealso cref="#setDeterministic(boolean)"/> methods should be used afterwards to restore
            /// representation invariants that are assumed by the built-in automata operations.
            ///
            /// @author Anders M&oslash;ller &lt;<a href="mailto:amoeller@cs.au.dk">amoeller@cs.au.dk</a>&gt;
            /// </para>
            /// </summary>
            class Automaton // : public Serializable, public Cloneable
            {

            public:
                static constexpr long long serialVersionUID = 10001;

                /// <summary>
                /// Minimize using Huffman's O(n<sup>2</sup>) algorithm.
                /// This is the standard text-book algorithm. </summary>
                /// <seealso cref= #setMinimization(int) </seealso>
                static constexpr int MINIMIZE_HUFFMAN = 0;

                /// <summary>
                /// Minimize using Brzozowski's O(2<sup>n</sup>) algorithm.
                /// This algorithm uses the reverse-determinize-reverse-determinize trick, which has a bad
                /// worst-case behavior but often works very well in practice
                /// (even better than Hopcroft's!). </summary>
                /// <seealso cref= #setMinimization(int) </seealso>
                static constexpr int MINIMIZE_BRZOZOWSKI = 1;

                /// <summary>
                /// Minimize using Hopcroft's O(n log n) algorithm.
                /// This is regarded as one of the most generally efficient algorithms that exist. </summary>
                /// <seealso cref= #setMinimization(int) </seealso>
                static constexpr int MINIMIZE_HOPCROFT = 2;

                /// <summary>
                /// Minimize using Valmari's O(n + m log m) algorithm. </summary>
                /// <seealso cref= #setMinimization(int) </seealso>
                static constexpr int MINIMIZE_VALMARI = 3;

                /// <summary>
                /// Selects minimization algorithm (default: <code>MINIMIZE_HOPCROFT</code>). </summary>
                static int minimization;

                /// <summary>
                /// Initial state of this automaton. </summary>
                State * initial;

                /// <summary>
                /// If true, then this automaton is definitely deterministic
                /// (i.e., there are no choices for any run, but a run may crash).
                /// </summary>
                bool deterministic = false;

                /// <summary>
                /// Extra data associated with this automaton. </summary>
                void * info;

                /// <summary>
                /// Hash code. Recomputed by <seealso cref="#minimize()"/>. </summary>
                int hash_code = 0;

                /// <summary>
                /// Singleton string. Null if not applicable. </summary>
                std::wstring singleton;

                /// <summary>
                /// Minimize always flag. </summary>
                static bool minimize_always;

                /// <summary>
                /// Selects whether operations may modify the input automata (default: <code>false</code>). </summary>
                static bool allow_mutation;

                /// <summary>
                /// Caches the <code>isDebug</code> state. </summary>
                static boost::optional<bool> is_debug;

                /// <summary>
                /// Constructs a new automaton that accepts the empty language.
                /// Using this constructor, automata can be constructed manually from
                /// <seealso cref="State"/> and <seealso cref="Transition"/> objects. </summary>
                /// <seealso cref= #setInitialState(State) </seealso>
                /// <seealso cref= State </seealso>
                /// <seealso cref= Transition </seealso>
                virtual ~Automaton() {
                    delete initial;
                    delete info;
                }

                Automaton();

                virtual bool isDebug();

                /// <summary>
                /// Selects minimization algorithm (default: <code>MINIMIZE_HOPCROFT</code>). </summary>
                /// <param name="algorithm"> minimization algorithm </param>
                static void setMinimization(int algorithm);

                /// <summary>
                /// Sets or resets minimize always flag.
                /// If this flag is set, then <seealso cref="#minimize()"/> will automatically
                /// be invoked after all operations that otherwise may produce non-minimal automata.
                /// By default, the flag is not set. </summary>
                /// <param name="flag"> if true, the flag is set </param>
                static void setMinimizeAlways(bool flag);

                /// <summary>
                /// Sets or resets allow mutate flag.
                /// If this flag is set, then all automata operations may modify automata given as input;
                /// otherwise, operations will always leave input automata languages unmodified.
                /// By default, the flag is not set. </summary>
                /// <param name="flag"> if true, the flag is set </param>
                /// <returns> previous value of the flag </returns>
                static bool setAllowMutate(bool flag);

                /// <summary>
                /// Returns the state of the allow mutate flag.
                /// If this flag is set, then all automata operations may modify automata given as input;
                /// otherwise, operations will always leave input automata languages unmodified.
                /// By default, the flag is not set. </summary>
                /// <returns> current value of the flag </returns>
                static bool getAllowMutate();

                virtual void checkMinimizeAlways();

                virtual bool isSingleton();

                /// <summary>
                /// Returns the singleton string for this automaton.
                /// An automaton that accepts exactly one string <i>may</i> be represented
                /// in singleton mode. In that case, this method may be used to obtain the string. </summary>
                /// <returns> string, null if this automaton is not in singleton mode. </returns>
                virtual std::wstring getSingleton();

                /// <summary>
                /// Sets initial state. </summary>
                /// <param name="s"> state </param>
                virtual void setInitialState(State * s);

                /// <summary>
                /// Gets initial state. </summary>
                /// <returns> state </returns>
                virtual State * getInitialState();

                /// <summary>
                /// Returns deterministic flag for this automaton. </summary>
                /// <returns> true if the automaton is definitely deterministic, false if the automaton
                ///         may be nondeterministic </returns>
                virtual bool isDeterministic();

                /// <summary>
                /// Sets deterministic flag for this automaton.
                /// This method should (only) be used if automata are constructed manually. </summary>
                /// <param name="deterministic"> true if the automaton is definitely deterministic, false if the automaton
                ///                      may be nondeterministic </param>
                virtual void setDeterministic(bool deterministic);

                /// <summary>
                /// Associates extra information with this automaton. </summary>
                /// <param name="info"> extra information </param>
                virtual void setInfo(void * info);

                /// <summary>
                /// Returns extra information associated with this automaton. </summary>
                /// <returns> extra information </returns>
                /// <seealso cref= #setInfo(Object) </seealso>
                virtual void * getInfo();

                /// <summary>
                /// Returns the set of states that are reachable from the initial state. </summary>
                /// <returns> set of <seealso cref="State"/> objects </returns>
                virtual std::set<State *> * getStates();

                /// <summary>
                /// Returns the set of reachable accept states. </summary>
                /// <returns> set of <seealso cref="State"/> objects </returns>
                virtual std::set<State *> * getAcceptStates();

                /// <summary>
                /// Assigns consecutive numbers to the given states.
                /// </summary>
                static void setStateNumbers(std::set<State *> * states);

                /// <summary>
                /// Adds transitions to explicit crash state to ensure that transition function is total.
                /// </summary>
                virtual void totalize();

                /// <summary>
                /// Restores representation invariant.
                /// This method must be invoked before any built-in automata operation is performed
                /// if automaton states or transitions are manipulated manually. </summary>
                /// <seealso cref= #setDeterministic(boolean) </seealso>
                virtual void restoreInvariant();

                /// <summary>
                /// Reduces this automaton.
                /// An automaton is "reduced" by combining overlapping and adjacent edge intervals with same destination.
                /// </summary>
                virtual void reduce();

                /// <summary>
                /// Returns sorted array of all interval start points.
                /// </summary>
                virtual std::vector<wchar_t> getStartPoints();

                /// <summary>
                /// Returns the set of live states. A state is "live" if an accept state is reachable from it. </summary>
                /// <returns> set of <seealso cref="State"/> objects </returns>
                virtual std::set<State *> * getLiveStates();

            private:
                std::set<State *> * getLiveStates(std::set<State *> * states);

                /// <summary>
                /// Removes transitions to dead states and calls <seealso cref="#reduce()"/> and <seealso cref="#clearHashCode()"/>.
                /// (A state is "dead" if no accept state is reachable from it.)
                /// </summary>
            public:
                virtual void removeDeadTransitions();

                /// <summary>
                /// Returns a sorted array of transitions for each state (and sets state numbers).
                /// </summary>
                static std::vector<std::vector<Transition *>> getSortedTransitions(std::set<State *> * states);

                /// <summary>
                /// Expands singleton representation to normal representation.
                /// Does nothing if not in singleton representation.
                /// </summary>
                virtual void expandSingleton();

                /// <summary>
                /// Returns the number of states in this automaton.
                /// </summary>
                virtual int getNumberOfStates();

                /// <summary>
                /// Returns the number of transitions in this automaton. This number is counted
                /// as the total number of edges, where one edge may be a character interval.
                /// </summary>
                virtual int getNumberOfTransitions();

                /// <summary>
                /// Returns true if the language of this automaton is equal to the language
                /// of the given automaton. Implemented using <code>hashCode</code> and
                /// <code>subsetOf</code>.
                /// </summary>
                virtual bool equals(void * obj);

                /// <summary>
                /// Returns hash code for this automaton. The hash code is based on the
                /// number of states and transitions in the minimized automaton.
                /// Invoking this method may involve minimizing the automaton.
                /// </summary>
                virtual int hashCode();

                /// <summary>
                /// Recomputes the hash code.
                /// The automaton must be minimal when this operation is performed.
                /// </summary>
                virtual void recomputeHashCode();

                /// <summary>
                /// Must be invoked when the stored hash code may no longer be valid.
                /// </summary>
                virtual void clearHashCode();

                /// <summary>
                /// Returns a string representation of this automaton.
                /// </summary>
                virtual std::wstring toString();

                /// <summary>
                /// Returns <a href="http://www.research.att.com/sw/tools/graphviz/" target="_top">Graphviz Dot</a>
                /// representation of this automaton.
                /// </summary>
                virtual std::wstring toDot();

                /// <summary>
                /// Returns a clone of this automaton, expands if singleton.
                /// </summary>
                virtual Automaton * cloneExpanded();

                /// <summary>
                /// Returns a clone of this automaton unless <code>allow_mutation</code> is set, expands if singleton.
                /// </summary>
                virtual Automaton * cloneExpandedIfRequired();

                /// <summary>
                /// Returns a clone of this automaton.
                /// </summary>
                virtual Automaton * clone();

                /// <summary>
                /// Returns a clone of this automaton, or this automaton itself if <code>allow_mutation</code> flag is set.
                /// </summary>
                virtual Automaton * cloneIfRequired();

                /// <summary>
                /// Retrieves a serialized <code>Automaton</code> located by a URL. </summary>
                /// <param name="url"> URL of serialized automaton </param>
                /// <exception cref="IOException"> if input/output related exception occurs </exception>
                /// <exception cref="ClassCastException"> if the data is not a serialized <code>Automaton</code> </exception>
                /// <exception cref="ClassNotFoundException"> if the class of the serialized object cannot be found </exception>
//				static Automaton *load(URL *url) throw(std::ios_base::failure, std::bad_cast);

                /// <summary>
                /// Retrieves a serialized <code>Automaton</code> from a stream. </summary>
                /// <param name="stream"> input stream with serialized automaton </param>
                /// <exception cref="IOException"> if input/output related exception occurs </exception>
                /// <exception cref="ClassCastException"> if the data is not a serialized <code>Automaton</code> </exception>
                /// <exception cref="ClassNotFoundException"> if the class of the serialized object cannot be found </exception>
                static Automaton * load(std::istream stream) throw(std::ios_base::failure, std::bad_cast);

                /// <summary>
                /// Writes this <code>Automaton</code> to the given stream. </summary>
                /// <param name="stream"> output stream for serialized automaton </param>
                /// <exception cref="IOException"> if input/output related exception occurs </exception>
                virtual void store(std::ostream stream) throw(std::ios_base::failure);

                /// <summary>
                /// See <seealso cref="BasicAutomata#makeEmpty()"/>.
                /// </summary>
                static Automaton * makeEmpty();

                /// <summary>
                /// See <seealso cref="BasicAutomata#makeEmptyString()"/>.
                /// </summary>
                static Automaton * makeEmptyString();

                /// <summary>
                /// See <seealso cref="BasicAutomata#makeAnyString()"/>.
                /// </summary>
                static Automaton * makeAnyString();

                /// <summary>
                /// See <seealso cref="BasicAutomata#makeAnyChar()"/>.
                /// </summary>
                static Automaton * makeAnyChar();

                /// <summary>
                /// See <seealso cref="BasicAutomata#makeChar(char)"/>.
                /// </summary>
                static Automaton * makeChar(wchar_t c);

                /// <summary>
                /// See <seealso cref="BasicAutomata#makeCharRange(char, char)"/>.
                /// </summary>
                static Automaton * makeCharRange(wchar_t min, wchar_t max);

                /// <summary>
                /// See <seealso cref="BasicAutomata#makeCharSet(String)"/>.
                /// </summary>
                static Automaton * makeCharSet(const std::wstring & set);

                /// <summary>
                /// See <seealso cref="BasicAutomata#makeInterval(int, int, int)"/>.
                /// </summary>
                static Automaton * makeInterval(int min, int max, int digits) throw(std::invalid_argument);

                /// <summary>
                /// See <seealso cref="BasicAutomata#makeString(String)"/>.
                /// </summary>
                static Automaton * makeString(const std::wstring & s);

                /// <summary>
                /// See <seealso cref="BasicAutomata#makeStringUnion(CharSequence...)"/>.
                /// </summary>
                static Automaton * makeStringUnion(std::vector<std::string> & strings);

                /// <summary>
                /// See <seealso cref="BasicAutomata#makeMaxInteger(String)"/>.
                /// </summary>
                static Automaton * makeMaxInteger(const std::wstring & n);

                /// <summary>
                /// See <seealso cref="BasicAutomata#makeMinInteger(String)"/>.
                /// </summary>
                static Automaton * makeMinInteger(const std::wstring & n);

                /// <summary>
                /// See <seealso cref="BasicAutomata#makeTotalDigits(int)"/>.
                /// </summary>
                static Automaton * makeTotalDigits(int i);

                /// <summary>
                /// See <seealso cref="BasicAutomata#makeFractionDigits(int)"/>.
                /// </summary>
                static Automaton * makeFractionDigits(int i);

                /// <summary>
                /// See <seealso cref="BasicAutomata#makeIntegerValue(String)"/>.
                /// </summary>
                static Automaton * makeIntegerValue(const std::wstring & value);

                /// <summary>
                /// See <seealso cref="BasicAutomata#makeDecimalValue(String)"/>.
                /// </summary>
                static Automaton * makeDecimalValue(const std::wstring & value);

                /// <summary>
                /// See <seealso cref="BasicAutomata#makeStringMatcher(String)"/>.
                /// </summary>
                static Automaton * makeStringMatcher(const std::wstring & s);

                /// <summary>
                /// See <seealso cref="BasicOperations#concatenate(Automaton, Automaton)"/>.
                /// </summary>
                virtual Automaton * concatenate(Automaton * a);

                /// <summary>
                /// See <seealso cref="BasicOperations#concatenate(List)"/>.
                /// </summary>
                static Automaton * concatenate(std::vector<Automaton *> & l);

                /// <summary>
                /// See <seealso cref="BasicOperations#optional(Automaton)"/>.
                /// </summary>
                virtual Automaton * optional();

                /// <summary>
                /// See <seealso cref="BasicOperations#repeat(Automaton)"/>.
                /// </summary>
                virtual Automaton * repeat();

                /// <summary>
                /// See <seealso cref="BasicOperations#repeat(Automaton, int)"/>.
                /// </summary>
                virtual Automaton * repeat(int min);

                /// <summary>
                /// See <seealso cref="BasicOperations#repeat(Automaton, int, int)"/>.
                /// </summary>
                virtual Automaton * repeat(int min, int max);

                /// <summary>
                /// See <seealso cref="BasicOperations#complement(Automaton)"/>.
                /// </summary>
                virtual Automaton * complement();

                /// <summary>
                /// See <seealso cref="BasicOperations#minus(Automaton, Automaton)"/>.
                /// </summary>
                virtual Automaton * minus(Automaton * a);

                /// <summary>
                /// See <seealso cref="BasicOperations#intersection(Automaton, Automaton)"/>.
                /// </summary>
                virtual Automaton * intersection(Automaton * a);

                /// <summary>
                /// See <seealso cref="BasicOperations#subsetOf(Automaton, Automaton)"/>.
                /// </summary>
                virtual bool subsetOf(Automaton * a);

                /// <summary>
                /// See <seealso cref="BasicOperations#union(Automaton, Automaton)"/>.
                /// </summary>
                virtual Automaton * union_Renamed(Automaton * a);

                /// <summary>
                /// See <seealso cref="BasicOperations#union(Collection)"/>.
                /// </summary>
                static Automaton * union_Renamed(std::vector<Automaton *> * l);

                /// <summary>
                /// See <seealso cref="BasicOperations#determinize(Automaton)"/>.
                /// </summary>
                virtual void determinize();

                /// <summary>
                /// See <seealso cref="BasicOperations#addEpsilons(Automaton, Collection)"/>.
                /// </summary>
                virtual void addEpsilons(std::vector<StatePair *> * pairs);

                /// <summary>
                /// See <seealso cref="BasicOperations#isEmptyString(Automaton)"/>.
                /// </summary>
                virtual bool isEmptyString();

                /// <summary>
                /// See <seealso cref="BasicOperations#isEmpty(Automaton)"/>.
                /// </summary>
                virtual bool isEmpty();

                /// <summary>
                /// See <seealso cref="BasicOperations#isTotal(Automaton)"/>.
                /// </summary>
                virtual bool isTotal();

                /// <summary>
                /// See <seealso cref="BasicOperations#getShortestExample(Automaton, boolean)"/>.
                /// </summary>
                virtual std::wstring getShortestExample(bool accepted);

                /// <summary>
                /// See <seealso cref="BasicOperations#run(Automaton, String)"/>.
                /// </summary>
                virtual bool run(const std::wstring & s);

                /// <summary>
                /// See <seealso cref="MinimizationOperations#minimize(Automaton)"/>.
                /// </summary>
                virtual void minimize();

                /// <summary>
                /// See <seealso cref="MinimizationOperations#minimize(Automaton)"/>.
                /// Returns the automaton being given as argument.
                /// </summary>
                static Automaton * minimize(Automaton * a);

                /// <summary>
                /// See <seealso cref="SpecialOperations#overlap(Automaton, Automaton)"/>.
                /// </summary>
                virtual Automaton * overlap(Automaton * a);

                /// <summary>
                /// See <seealso cref="SpecialOperations#singleChars(Automaton)"/>.
                /// </summary>
                virtual Automaton * singleChars();

                /// <summary>
                /// See <seealso cref="SpecialOperations#trim(Automaton, String, char)"/>.
                /// </summary>
                virtual Automaton * trim(const std::wstring & set, wchar_t c);

                /// <summary>
                /// See <seealso cref="SpecialOperations#compress(Automaton, String, char)"/>.
                /// </summary>
                virtual Automaton * compress(const std::wstring & set, wchar_t c);

                /// <summary>
                /// See <seealso cref="SpecialOperations#subst(Automaton, Map)"/>.
                /// </summary>
                virtual Automaton * subst(std::unordered_map<wchar_t, std::set<wchar_t> *> & map);

                /// <summary>
                /// See <seealso cref="SpecialOperations#subst(Automaton, char, String)"/>.
                /// </summary>
                virtual Automaton * subst(wchar_t c, const std::wstring & s);

                /// <summary>
                /// See <seealso cref="SpecialOperations#homomorph(Automaton, char[], char[])"/>.
                /// </summary>
                virtual Automaton * homomorph(std::vector<wchar_t> & source, std::vector<wchar_t> & dest);

                /// <summary>
                /// See <seealso cref="SpecialOperations#projectChars(Automaton, Set)"/>.
                /// </summary>
                virtual Automaton * projectChars(std::set<wchar_t> * chars);

                /// <summary>
                /// See <seealso cref="SpecialOperations#isFinite(Automaton)"/>.
                /// </summary>
                virtual bool isFinite();

                /// <summary>
                /// See <seealso cref="SpecialOperations#getStrings(Automaton, int)"/>.
                /// </summary>
                virtual std::set<std::wstring> * getStrings(int length);

                /// <summary>
                /// See <seealso cref="SpecialOperations#getFiniteStrings(Automaton)"/>.
                /// </summary>
                virtual std::set<std::wstring> * getFiniteStrings();

                /// <summary>
                /// See <seealso cref="SpecialOperations#getFiniteStrings(Automaton, int)"/>.
                /// </summary>
                virtual std::set<std::wstring> * getFiniteStrings(int limit);

                /// <summary>
                /// See <seealso cref="SpecialOperations#getCommonPrefix(Automaton)"/>.
                /// </summary>
                virtual std::wstring getCommonPrefix();

                /// <summary>
                /// See <seealso cref="SpecialOperations#prefixClose(Automaton)"/>.
                /// </summary>
                virtual void prefixClose();

                /// <summary>
                /// See <seealso cref="SpecialOperations#hexCases(Automaton)"/>.
                /// </summary>
                static Automaton * hexCases(Automaton * a);

                /// <summary>
                /// See <seealso cref="SpecialOperations#replaceWhitespace(Automaton)"/>.
                /// </summary>
                static Automaton * replaceWhitespace(Automaton * a);

                /// <summary>
                /// See <seealso cref="ShuffleOperations#shuffleSubsetOf(Collection, Automaton, Character, Character)"/>.
                /// </summary>
                static std::wstring
                shuffleSubsetOf(std::vector<Automaton *> * ca, Automaton * a, boost::optional<wchar_t> suspend_shuffle,
                                boost::optional<wchar_t> resume_shuffle);

                /// <summary>
                /// See <seealso cref="ShuffleOperations#shuffle(Automaton, Automaton)"/>.
                /// </summary>
                virtual Automaton * shuffle(Automaton * a);
            };

        }
    }
}
