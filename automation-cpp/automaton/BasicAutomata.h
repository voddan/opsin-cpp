#pragma once

#include <string>
#include <vector>
#include <limits>
#include <stdexcept>
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
			/// Construction of basic automata.
			/// </summary>
			class BasicAutomata final
			{

			private:
				BasicAutomata();

				/// <summary>
				/// Returns a new (deterministic) automaton with the empty language. 
				/// </summary>
			public:
				static Automaton *makeEmpty();

				/// <summary>
				/// Returns a new (deterministic) automaton that accepts only the empty string. 
				/// </summary>
				static Automaton *makeEmptyString();

				/// <summary>
				/// Returns a new (deterministic) automaton that accepts all strings. 
				/// </summary>
				static Automaton *makeAnyString();

				/// <summary>
				/// Returns a new (deterministic) automaton that accepts any single character. 
				/// </summary>
				static Automaton *makeAnyChar();

				/// <summary>
				/// Returns a new (deterministic) automaton that accepts a single character of the given value. 
				/// </summary>
				static Automaton *makeChar(wchar_t c);

				/// <summary>
				/// Returns a new (deterministic) automaton that accepts a single char 
				/// whose value is in the given interval (including both end points). 
				/// </summary>
				static Automaton *makeCharRange(wchar_t min, wchar_t max);

				/// <summary>
				/// Returns a new (deterministic) automaton that accepts a single character in the given set. 
				/// </summary>
				static Automaton *makeCharSet(const std::wstring &set);

				/// <summary>
				/// Constructs sub-automaton corresponding to decimal numbers of 
				/// length x.substring(n).length().
				/// </summary>
			private:
				static State *anyOfRightLength(const std::wstring &x, int n);

				/// <summary>
				/// Constructs sub-automaton corresponding to decimal numbers of value 
				/// at least x.substring(n) and length x.substring(n).length().
				/// </summary>
				static State *atLeast(const std::wstring &x, int n, std::vector<State*> *initials, bool zeros);

				/// <summary>
				/// Constructs sub-automaton corresponding to decimal numbers of value 
				/// at most x.substring(n) and length x.substring(n).length().
				/// </summary>
				static State *atMost(const std::wstring &x, int n);

				/// <summary>
				/// Constructs sub-automaton corresponding to decimal numbers of value 
				/// between x.substring(n) and y.substring(n) and of
				/// length x.substring(n).length() (which must be equal to y.substring(n).length()).
				/// </summary>
				static State *between(const std::wstring &x, const std::wstring &y, int n, std::vector<State*> *initials, bool zeros);

				/// <summary>
				/// Returns a new automaton that accepts strings representing 
				/// decimal non-negative integers in the given interval. </summary>
				/// <param name="min"> minimal value of interval </param>
				/// <param name="max"> maximal value of inverval (both end points are included in the interval) </param>
				/// <param name="digits"> if &gt;0, use fixed number of digits (strings must be prefixed
				///               by 0's to obtain the right length) -
				///               otherwise, the number of digits is not fixed </param>
				/// <exception cref="IllegalArgumentException"> if min&gt;max or if numbers in the interval cannot be expressed
				///                                     with the given fixed number of digits </exception>
			public:
				static Automaton *makeInterval(int min, int max, int digits) throw(std::invalid_argument);

				/// <summary>
				/// Returns a new (deterministic) automaton that accepts the single given string.
				/// </summary>
				static Automaton *makeString(const std::wstring &s);

				/// <summary>
				/// Returns a new (deterministic and minimal) automaton that accepts the union of the
				/// given set of strings. The input character sequences are internally sorted in-place,
				/// so the input array is modified. </summary>
				/// <seealso cref= StringUnionOperations </seealso>
				static Automaton *makeStringUnion(std::vector< std::string> &strings);

				/// <summary>
				/// Constructs automaton that accept strings representing nonnegative integers
				/// that are not larger than the given value. </summary>
				/// <param name="n"> string representation of maximum value </param>
				static Automaton *makeMaxInteger(const std::wstring &n);

			private:
				static void maxInteger(const std::wstring &n, int i, StringBuilder *b);

				/// <summary>
				/// Constructs automaton that accept strings representing nonnegative integers
				/// that are not less that the given value. </summary>
				/// <param name="n"> string representation of minimum value </param>
			public:
				static Automaton *makeMinInteger(const std::wstring &n);

			private:
				static void minInteger(const std::wstring &n, int i, StringBuilder *b);

				/// <summary>
				/// Constructs automaton that accept strings representing decimal numbers
				/// that can be written with at most the given number of digits.
				/// Surrounding whitespace is permitted. </summary>
				/// <param name="i"> max number of necessary digits </param>
			public:
				static Automaton *makeTotalDigits(int i);

				/// <summary>
				/// Constructs automaton that accept strings representing decimal numbers
				/// that can be written with at most the given number of digits in the fraction part.
				/// Surrounding whitespace is permitted. </summary>
				/// <param name="i"> max number of necessary fraction digits </param>
				static Automaton *makeFractionDigits(int i);

				/// <summary>
				/// Constructs automaton that accept strings representing the given integer.
				/// Surrounding whitespace is permitted. </summary>
				/// <param name="value"> string representation of integer </param>
				static Automaton *makeIntegerValue(const std::wstring &value);

				/// <summary>
				/// Constructs automaton that accept strings representing the given decimal number.
				/// Surrounding whitespace is permitted. </summary>
				/// <param name="value"> string representation of decimal number </param>
				static Automaton *makeDecimalValue(const std::wstring &value);

				/// <summary>
				/// Constructs deterministic automaton that matches strings that contain the given substring.
				/// </summary>
				static Automaton *makeStringMatcher(const std::wstring &s);
			};

		}
	}
}
