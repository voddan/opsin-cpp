#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <set>
#include <algorithm>
#include <limits>
#include <boost/optional.hpp>
#include "stringbuilder.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace dk { namespace brics { namespace automaton { class Automaton; } } }
namespace dk { namespace brics { namespace automaton { class State; } } }

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
			/// Special automata operations.
			/// </summary>
			class SpecialOperations final
			{

			private:
				SpecialOperations();

				/// <summary>
				/// Reverses the language of the given (non-singleton) automaton while returning
				/// the set of new initial states.
				/// </summary>
			public:
				static std::set<State*> *reverse(Automaton *a);

				/// <summary>
				/// Returns an automaton that accepts the overlap of strings that in more than one way can be split into
				/// a left part being accepted by <code>a1</code> and a right part being accepted by
				/// <code>a2</code>.
				/// </summary>
				static Automaton *overlap(Automaton *a1, Automaton *a2);

			private:
				static void acceptToAccept(Automaton *a);

				/// <summary>
				/// Returns an automaton that accepts the single chars that occur 
				/// in strings that are accepted by the given automaton. 
				/// Never modifies the input automaton.
				/// </summary>
			public:
				static Automaton *singleChars(Automaton *a);

				/// <summary>
				/// Returns an automaton that accepts the trimmed language of the given
				/// automaton. The resulting automaton is constructed as follows: 1) Whenever
				/// a <code>c</code> character is allowed in the original automaton, one or
				/// more <code>set</code> characters are allowed in the new automaton. 2)
				/// The automaton is prefixed and postfixed with any number of
				/// <code>set</code> characters. </summary>
				/// <param name="set"> set of characters to be trimmed </param>
				/// <param name="c"> canonical trim character (assumed to be in <code>set</code>) </param>
				static Automaton *trim(Automaton *a, const std::wstring &set, wchar_t c);

			private:
				static void addSetTransitions(State *s, const std::wstring &set, State *p);

				/// <summary>
				/// Returns an automaton that accepts the compressed language of the given
				/// automaton. Whenever a <code>c</code> character is allowed in the
				/// original automaton, one or more <code>set</code> characters are allowed
				/// in the new automaton. </summary>
				/// <param name="set"> set of characters to be compressed </param>
				/// <param name="c"> canonical compress character (assumed to be in <code>set</code>) </param>
			public:
				static Automaton *compress(Automaton *a, const std::wstring &set, wchar_t c);

				/// <summary>
				/// Returns an automaton where all transition labels have been substituted.
				/// <para>
				/// Each transition labeled <code>c</code> is changed to a set of
				/// transitions, one for each character in <code>map(c)</code>. If
				/// <code>map(c)</code> is null, then the transition is unchanged.
				/// </para>
				/// </summary>
				/// <param name="map"> map from characters to sets of characters (where characters 
				///            are <code>Character</code> objects) </param>
				static Automaton *subst(Automaton *a, std::unordered_map<wchar_t, std::set<wchar_t>*> &map);

				/// <summary>
				/// Finds the largest entry whose value is less than or equal to c, 
				/// or 0 if there is no such entry. 
				/// </summary>
				static int findIndex(wchar_t c, std::vector<wchar_t> &points);

				/// <summary>
				/// Returns an automaton where all transitions of the given char are replaced by a string. </summary>
				/// <param name="c"> char </param>
				/// <param name="s"> string </param>
				/// <returns> new automaton </returns>
				static Automaton *subst(Automaton *a, wchar_t c, const std::wstring &s);

				/// <summary>
				/// Returns an automaton accepting the homomorphic image of the given automaton
				/// using the given function.
				/// <para>
				/// This method maps each transition label to a new value.
				/// <code>source</code> and <code>dest</code> are assumed to be arrays of
				/// same length, and <code>source</code> must be sorted in increasing order
				/// and contain no duplicates. <code>source</code> defines the starting
				/// points of char intervals, and the corresponding entries in
				/// <code>dest</code> define the starting points of corresponding new
				/// intervals.
				/// </para>
				/// </summary>
				static Automaton *homomorph(Automaton *a, std::vector<wchar_t> &source, std::vector<wchar_t> &dest);

				/// <summary>
				/// Returns an automaton with projected alphabet. The new automaton accepts
				/// all strings that are projections of strings accepted by the given automaton
				/// onto the given characters (represented by <code>Character</code>). If
				/// <code>null</code> is in the set, it abbreviates the intervals
				/// u0000-uDFFF and uF900-uFFFF (i.e., the non-private code points). It is
				/// assumed that all other characters from <code>chars</code> are in the
				/// interval uE000-uF8FF.
				/// </summary>
				static Automaton *projectChars(Automaton *a, std::set<wchar_t> *chars);

				/// <summary>
				/// Returns true if the language of this automaton is finite.
				/// </summary>
				static bool isFinite(Automaton *a);

				/// <summary>
				/// Checks whether there is a loop containing s. (This is sufficient since 
				/// there are never transitions to dead states.) 
				/// </summary>
			private:
				static bool isFinite(State *s, std::unordered_set<State*> &path, std::unordered_set<State*> &visited);

				/// <summary>
				/// Returns the set of accepted strings of the given length.
				/// </summary>
			public:
				static std::set<std::wstring> *getStrings(Automaton *a, int length);

			private:
				static void getStrings(State *s, std::set<std::wstring> *strings, StringBuilder *path, int length);

				/// <summary>
				/// Returns the set of accepted strings, assuming this automaton has a finite
				/// language. If the language is not finite, null is returned.
				/// </summary>
			public:
				static std::set<std::wstring> *getFiniteStrings(Automaton *a);

				/// <summary>
				/// Returns the set of accepted strings, assuming that at most <code>limit</code>
				/// strings are accepted. If more than <code>limit</code> strings are
				/// accepted, null is returned. If <code>limit</code>&lt;0, then this
				/// methods works like <seealso cref="#getFiniteStrings(Automaton)"/>.
				/// </summary>
				static std::set<std::wstring> *getFiniteStrings(Automaton *a, int limit);

				/// <summary>
				/// Returns the strings that can be produced from the given state, or false if more than 
				/// <code>limit</code> strings are found. <code>limit</code>&lt;0 means "infinite". 
				/// 
				/// </summary>
			private:
				static bool getFiniteStrings(State *s, std::unordered_set<State*> &pathstates, std::unordered_set<std::wstring> &strings, StringBuilder *path, int limit);

				/// <summary>
				/// Returns the longest string that is a prefix of all accepted strings and
				/// visits each state at most once. </summary>
				/// <returns> common prefix </returns>
			public:
				static std::wstring getCommonPrefix(Automaton *a);

				/// <summary>
				/// Prefix closes the given automaton.
				/// </summary>
				static void prefixClose(Automaton *a);

				/// <summary>
				/// Constructs automaton that accepts the same strings as the given automaton
				/// but ignores upper/lower case of A-F. </summary>
				/// <param name="a"> automaton </param>
				/// <returns> automaton </returns>
				static Automaton *hexCases(Automaton *a);

				/// <summary>
				/// Constructs automaton that accepts 0x20, 0x9, 0xa, and 0xd in place of each 0x20 transition
				/// in the given automaton. </summary>
				/// <param name="a"> automaton </param>
				/// <returns> automaton </returns>
				static Automaton *replaceWhitespace(Automaton *a);
			};

		}
	}
}
