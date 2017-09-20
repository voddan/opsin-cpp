#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <list>
#include <algorithm>
#include <limits>
#include <set>
#include "stringhelper.h"
#include "stringbuilder.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace dk { namespace brics { namespace automaton { class Automaton; } } }
namespace dk { namespace brics { namespace automaton { class State; } } }
namespace dk { namespace brics { namespace automaton { class StatePair; } } }

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

namespace dk
{
	namespace brics
	{
		namespace automaton
		{


			/// <summary>
			/// Basic automata operations.
			/// </summary>
			class BasicOperations final
			{

			private:
				BasicOperations();

				/// <summary>
				/// Returns an automaton that accepts the concatenation of the languages of 
				/// the given automata. 
				/// <para>
				/// Complexity: linear in number of states. 
				/// </para>
				/// </summary>
			public:
				static Automaton *concatenate(Automaton *a1, Automaton *a2);

				/// <summary>
				/// Returns an automaton that accepts the concatenation of the languages of
				/// the given automata.
				/// <para>
				/// Complexity: linear in total number of states.
				/// </para>
				/// </summary>
				static Automaton *concatenate(std::vector<Automaton*> &l);

				/// <summary>
				/// Returns an automaton that accepts the union of the empty string and the
				/// language of the given automaton.
				/// <para>
				/// Complexity: linear in number of states.
				/// </para>
				/// </summary>
				static Automaton *optional(Automaton *a);

				/// <summary>
				/// Returns an automaton that accepts the Kleene star (zero or more
				/// concatenated repetitions) of the language of the given automaton.
				/// Never modifies the input automaton language.
				/// <para>
				/// Complexity: linear in number of states.
				/// </para>
				/// </summary>
				static Automaton *repeat(Automaton *a);

				/// <summary>
				/// Returns an automaton that accepts <code>min</code> or more
				/// concatenated repetitions of the language of the given automaton.
				/// <para>
				/// Complexity: linear in number of states and in <code>min</code>.
				/// </para>
				/// </summary>
				static Automaton *repeat(Automaton *a, int min);

				/// <summary>
				/// Returns an automaton that accepts between <code>min</code> and
				/// <code>max</code> (including both) concatenated repetitions of the
				/// language of the given automaton.
				/// <para>
				/// Complexity: linear in number of states and in <code>min</code> and
				/// <code>max</code>.
				/// </para>
				/// </summary>
				static Automaton *repeat(Automaton *a, int min, int max);

				/// <summary>
				/// Returns a (deterministic) automaton that accepts the complement of the
				/// language of the given automaton.
				/// <para>
				/// Complexity: linear in number of states (if already deterministic).
				/// </para>
				/// </summary>
				static Automaton *complement(Automaton *a);

				/// <summary>
				/// Returns a (deterministic) automaton that accepts the intersection of
				/// the language of <code>a1</code> and the complement of the language of 
				/// <code>a2</code>. As a side-effect, the automata may be determinized, if not
				/// already deterministic.
				/// <para>
				/// Complexity: quadratic in number of states (if already deterministic).
				/// </para>
				/// </summary>
				static Automaton *minus(Automaton *a1, Automaton *a2);

				/// <summary>
				/// Returns an automaton that accepts the intersection of
				/// the languages of the given automata. 
				/// Never modifies the input automata languages.
				/// <para>
				/// Complexity: quadratic in number of states.
				/// </para>
				/// </summary>
				static Automaton *intersection(Automaton *a1, Automaton *a2);

				/// <summary>
				/// Returns true if the language of <code>a1</code> is a subset of the
				/// language of <code>a2</code>. 
				/// As a side-effect, <code>a2</code> is determinized if not already marked as
				/// deterministic.
				/// <para>
				/// Complexity: quadratic in number of states.
				/// </para>
				/// </summary>
				static bool subsetOf(Automaton *a1, Automaton *a2);

				/// <summary>
				/// Returns an automaton that accepts the union of the languages of the given automata.
				/// <para>
				/// Complexity: linear in number of states.
				/// </para>
				/// </summary>
				static Automaton *union_Renamed(Automaton *a1, Automaton *a2);

				/// <summary>
				/// Returns an automaton that accepts the union of the languages of the given automata.
				/// <para>
				/// Complexity: linear in number of states.
				/// </para>
				/// </summary>
				static Automaton *union_Renamed(std::vector<Automaton*> *l);

				/// <summary>
				/// Determinizes the given automaton.
				/// <para>
				/// Complexity: exponential in number of states.
				/// </para>
				/// </summary>
				static void determinize(Automaton *a);

				/// <summary>
				/// Determinizes the given automaton using the given set of initial states. 
				/// </summary>
				static void determinize(Automaton *a, std::set<State*> *initialset);

				/// <summary>
				/// Adds epsilon transitions to the given automaton.
				/// This method adds extra character interval transitions that are equivalent to the given
				/// set of epsilon transitions. </summary>
				/// <param name="pairs"> collection of <seealso cref="StatePair"/> objects representing pairs of source/destination states 
				///        where epsilon transitions should be added </param>
				static void addEpsilons(Automaton *a, std::vector<StatePair*> *pairs);

				/// <summary>
				/// Returns true if the given automaton accepts the empty string and nothing else.
				/// </summary>
				static bool isEmptyString(Automaton *a);

				/// <summary>
				/// Returns true if the given automaton accepts no strings.
				/// </summary>
				static bool isEmpty(Automaton *a);

				/// <summary>
				/// Returns true if the given automaton accepts all strings.
				/// </summary>
				static bool isTotal(Automaton *a);

				/// <summary>
				/// Returns a shortest accepted/rejected string. 
				/// If more than one shortest string is found, the lexicographically first of the shortest strings is returned. </summary>
				/// <param name="accepted"> if true, look for accepted strings; otherwise, look for rejected strings </param>
				/// <returns> the string, null if none found </returns>
				static std::wstring getShortestExample(Automaton *a, bool accepted);

				static std::wstring getShortestExample(State *s, bool accepted);

				/// <summary>
				/// Returns true if the given string is accepted by the automaton. 
				/// <para>
				/// Complexity: linear in the length of the string.
				/// </para>
				/// <para>
				/// <b>Note:</b> for full performance, use the <seealso cref="RunAutomaton"/> class.
				/// </para>
				/// </summary>
				static bool run(Automaton *a, const std::wstring &s);
			};

		}
	}
}
