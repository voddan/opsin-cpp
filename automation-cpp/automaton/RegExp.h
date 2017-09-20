#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include "stringbuilder.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace dk { namespace brics { namespace automaton { class Automaton; } } }
namespace dk { namespace brics { namespace automaton { class AutomatonProvider; } } }

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
			/// Regular Expression extension to <code>Automaton</code>.
			/// <para>
			/// Regular expressions are built from the following abstract syntax:
			/// <table border=0>
			/// <tr><td><i>regexp</i></td><td>::=</td><td><i>unionexp</i></td><td></td><td></td></tr>
			/// <tr><td></td><td>|</td><td></td><td></td><td></td></tr>
			/// 
			/// <tr><td><i>unionexp</i></td><td>::=</td><td><i>interexp</i>&nbsp;<tt><b>|</b></tt>&nbsp;<i>unionexp</i></td><td>(union)</td><td></td></tr>
			/// <tr><td></td><td>|</td><td><i>interexp</i></td><td></td><td></td></tr>
			/// 
			/// <tr><td><i>interexp</i></td><td>::=</td><td><i>concatexp</i>&nbsp;<tt><b>&amp;</b></tt>&nbsp;<i>interexp</i></td><td>(intersection)</td><td><small>[OPTIONAL]</small></td></tr>
			/// <tr><td></td><td>|</td><td><i>concatexp</i></td><td></td><td></td></tr>
			/// 
			/// <tr><td><i>concatexp</i></td><td>::=</td><td><i>repeatexp</i>&nbsp;<i>concatexp</i></td><td>(concatenation)</td><td></td></tr>
			/// <tr><td></td><td>|</td><td><i>repeatexp</i></td><td></td><td></td></tr>
			/// 
			/// <tr><td><i>repeatexp</i></td><td>::=</td><td><i>repeatexp</i>&nbsp;<tt><b>?</b></tt></td><td>(zero or one occurrence)</td><td></td></tr>
			/// <tr><td></td><td>|</td><td><i>repeatexp</i>&nbsp;<tt><b>*</b></tt></td><td>(zero or more occurrences)</td><td></td></tr>
			/// <tr><td></td><td>|</td><td><i>repeatexp</i>&nbsp;<tt><b>+</b></tt></td><td>(one or more occurrences)</td><td></td></tr>
			/// <tr><td></td><td>|</td><td><i>repeatexp</i>&nbsp;<tt><b>{</b><i>n</i><b>}</b></tt></td><td>(<tt><i>n</i></tt> occurrences)</td><td></td></tr>
			/// <tr><td></td><td>|</td><td><i>repeatexp</i>&nbsp;<tt><b>{</b><i>n</i><b>,}</b></tt></td><td>(<tt><i>n</i></tt> or more occurrences)</td><td></td></tr>
			/// <tr><td></td><td>|</td><td><i>repeatexp</i>&nbsp;<tt><b>{</b><i>n</i><b>,</b><i>m</i><b>}</b></tt></td><td>(<tt><i>n</i></tt> to <tt><i>m</i></tt> occurrences, including both)</td><td></td></tr>
			/// <tr><td></td><td>|</td><td><i>complexp</i></td><td></td><td></td></tr>
			/// 
			/// <tr><td><i>complexp</i></td><td>::=</td><td><tt><b>~</b></tt>&nbsp;<i>complexp</i></td><td>(complement)</td><td><small>[OPTIONAL]</small></td></tr>
			/// <tr><td></td><td>|</td><td><i>charclassexp</i></td><td></td><td></td></tr>
			/// 
			/// <tr><td><i>charclassexp</i></td><td>::=</td><td><tt><b>[</b></tt>&nbsp;<i>charclasses</i>&nbsp;<tt><b>]</b></tt></td><td>(character class)</td><td></td></tr>
			/// <tr><td></td><td>|</td><td><tt><b>[^</b></tt>&nbsp;<i>charclasses</i>&nbsp;<tt><b>]</b></tt></td><td>(negated character class)</td><td></td></tr>
			/// <tr><td></td><td>|</td><td><i>simpleexp</i></td><td></td><td></td></tr>
			/// 
			/// <tr><td><i>charclasses</i></td><td>::=</td><td><i>charclass</i>&nbsp;<i>charclasses</i></td><td></td><td></td></tr>
			/// <tr><td></td><td>|</td><td><i>charclass</i></td><td></td><td></td></tr>
			/// 
			/// <tr><td><i>charclass</i></td><td>::=</td><td><i>charexp</i>&nbsp;<tt><b>-</b></tt>&nbsp;<i>charexp</i></td><td>(character range, including end-points)</td><td></td></tr>
			/// <tr><td></td><td>|</td><td><i>charexp</i></td><td></td><td></td></tr>
			/// 
			/// <tr><td><i>simpleexp</i></td><td>::=</td><td><i>charexp</i></td><td></td><td></td></tr>
			/// <tr><td></td><td>|</td><td><tt><b>.</b></tt></td><td>(any single character)</td><td></td></tr>
			/// <tr><td></td><td>|</td><td><tt><b>#</b></tt></td><td>(the empty language)</td><td><small>[OPTIONAL]</small></td></tr>
			/// <tr><td></td><td>|</td><td><tt><b>@</b></tt></td><td>(any string)</td><td><small>[OPTIONAL]</small></td></tr>
			/// <tr><td></td><td>|</td><td><tt><b>"</b></tt>&nbsp;&lt;Unicode string without double-quotes&gt;&nbsp;<tt><b>"</b></tt></td><td>(a string)</td><td></td></tr>
			/// <tr><td></td><td>|</td><td><tt><b>(</b></tt>&nbsp;<tt><b>)</b></tt></td><td>(the empty string)</td><td></td></tr>
			/// <tr><td></td><td>|</td><td><tt><b>(</b></tt>&nbsp;<i>unionexp</i>&nbsp;<tt><b>)</b></tt></td><td>(precedence override)</td><td></td></tr>
			/// <tr><td></td><td>|</td><td><tt><b>&lt;</b></tt>&nbsp;&lt;identifier&gt;&nbsp;<tt><b>&gt;</b></tt></td><td>(named automaton)</td><td><small>[OPTIONAL]</small></td></tr>
			/// <tr><td></td><td>|</td><td><tt><b>&lt;</b><i>n</i>-<i>m</i><b>&gt;</b></tt></td><td>(numerical interval)</td><td><small>[OPTIONAL]</small></td></tr>
			/// 
			/// <tr><td><i>charexp</i></td><td>::=</td><td>&lt;Unicode character&gt;</td><td>(a single non-reserved character)</td><td></td></tr>
			/// <tr><td></td><td>|</td><td><tt><b>\</b></tt>&nbsp;&lt;Unicode character&gt;&nbsp;</td><td>(a single character)</td><td></td></tr>
			/// </table>
			/// </para>
			/// <para>
			/// The productions marked <small>[OPTIONAL]</small> are only allowed
			/// if specified by the syntax flags passed to the <code>RegExp</code>
			/// constructor.  The reserved characters used in the (enabled) syntax
			/// must be escaped with backslash (<tt><b>\</b></tt>) or double-quotes
			/// (<tt><b>"..."</b></tt>). (In contrast to other regexp syntaxes,
			/// this is required also in character classes.)  Be aware that
			/// dash (<tt><b>-</b></tt>) has a special meaning in <i>charclass</i> expressions.
			/// An identifier is a string not containing right angle bracket
			/// (<tt><b>&gt;</b></tt>) or dash (<tt><b>-</b></tt>).  Numerical intervals are
			/// specified by non-negative decimal integers and include both end
			/// points, and if <tt><i>n</i></tt> and <tt><i>m</i></tt> have the
			/// same number of digits, then the conforming strings must have that
			/// length (i.e. prefixed by 0's).
			/// @author Anders M&oslash;ller &lt;<a href="mailto:amoeller@cs.au.dk">amoeller@cs.au.dk</a>&gt; 
			/// 
			/// </para>
			/// </summary>
			class RegExp
			{

			public:
				enum class Kind
				{
					REGEXP_UNION,
					REGEXP_CONCATENATION,
					REGEXP_INTERSECTION,
					REGEXP_OPTIONAL,
					REGEXP_REPEAT,
					REGEXP_REPEAT_MIN,
					REGEXP_REPEAT_MINMAX,
					REGEXP_COMPLEMENT,
					REGEXP_CHAR,
					REGEXP_CHAR_RANGE,
					REGEXP_ANYCHAR,
					REGEXP_EMPTY,
					REGEXP_STRING,
					REGEXP_ANYSTRING,
					REGEXP_AUTOMATON,
					REGEXP_INTERVAL
				};

				/// <summary>
				/// Syntax flag, enables intersection (<tt>&amp;</tt>). 
				/// </summary>
			public:
				static constexpr int INTERSECTION = 0x0001;

				/// <summary>
				/// Syntax flag, enables complement (<tt>~</tt>). 
				/// </summary>
				static constexpr int COMPLEMENT = 0x0002;

				/// <summary>
				/// Syntax flag, enables empty language (<tt>#</tt>). 
				/// </summary>
				static constexpr int EMPTY = 0x0004;

				/// <summary>
				/// Syntax flag, enables anystring (<tt>@</tt>). 
				/// </summary>
				static constexpr int ANYSTRING = 0x0008;

				/// <summary>
				/// Syntax flag, enables named automata (<tt>&lt;</tt>identifier<tt>&gt;</tt>). 
				/// </summary>
				static constexpr int AUTOMATON = 0x0010;

				/// <summary>
				/// Syntax flag, enables numerical intervals (<tt>&lt;<i>n</i>-<i>m</i>&gt;</tt>). 
				/// </summary>
				static constexpr int INTERVAL = 0x0020;

				/// <summary>
				/// Syntax flag, enables all optional regexp syntax. 
				/// </summary>
				static constexpr int ALL = 0xffff;

				/// <summary>
				/// Syntax flag, enables no optional regexp syntax. 
				/// </summary>
				static constexpr int NONE = 0x0000;

			private:
				static bool allow_mutation;

			public:
				Kind kind = static_cast<Kind>(0);
				RegExp *exp1, *exp2;
				std::wstring s;
				wchar_t c = L'\0';
				int min = 0, max = 0, digits = 0;
				wchar_t from = L'\0', to = L'\0';

				std::wstring b;
				int flags = 0;
				int pos = 0;

				virtual ~RegExp()
				{
					delete exp1;
					delete exp2;
				}

				RegExp();

				/// <summary>
				/// Constructs new <code>RegExp</code> from a string. 
				/// Same as <code>RegExp(s, ALL)</code>. </summary>
				/// <param name="s"> regexp string </param>
				/// <exception cref="IllegalArgumentException"> if an error occured while parsing the regular expression </exception>
				RegExp(const std::wstring &s) throw(std::invalid_argument);

				/// <summary>
				/// Constructs new <code>RegExp</code> from a string. </summary>
				/// <param name="s"> regexp string </param>
				/// <param name="syntax_flags"> boolean 'or' of optional syntax constructs to be enabled </param>
				/// <exception cref="IllegalArgumentException"> if an error occured while parsing the regular expression </exception>
				RegExp(const std::wstring &s, int syntax_flags) throw(std::invalid_argument);

				/// <summary>
				/// Constructs new <code>Automaton</code> from this <code>RegExp</code>. 
				/// Same as <code>toAutomaton(null)</code> (empty automaton map).
				/// </summary>
				virtual Automaton *toAutomaton();

				/// <summary>
				/// Constructs new <code>Automaton</code> from this <code>RegExp</code>. 
				/// Same as <code>toAutomaton(null,minimize)</code> (empty automaton map).
				/// </summary>
				virtual Automaton *toAutomaton(bool minimize);

				/// <summary>
				/// Constructs new <code>Automaton</code> from this <code>RegExp</code>. 
				/// The constructed automaton is minimal and deterministic and has no 
				/// transitions to dead states. </summary>
				/// <param name="automaton_provider"> provider of automata for named identifiers </param>
				/// <exception cref="IllegalArgumentException"> if this regular expression uses
				///   a named identifier that is not available from the automaton provider </exception>
				virtual Automaton *toAutomaton(AutomatonProvider *automaton_provider) throw(std::invalid_argument);

				/// <summary>
				/// Constructs new <code>Automaton</code> from this <code>RegExp</code>. 
				/// The constructed automaton has no transitions to dead states. </summary>
				/// <param name="automaton_provider"> provider of automata for named identifiers </param>
				/// <param name="minimize"> if set, the automaton is minimized and determinized </param>
				/// <exception cref="IllegalArgumentException"> if this regular expression uses
				///   a named identifier that is not available from the automaton provider </exception>
				virtual Automaton *toAutomaton(AutomatonProvider *automaton_provider, bool minimize) throw(std::invalid_argument);

				/// <summary>
				/// Constructs new <code>Automaton</code> from this <code>RegExp</code>. 
				/// The constructed automaton is minimal and deterministic and has no 
				/// transitions to dead states. </summary>
				/// <param name="automata"> a map from automaton identifiers to automata 
				///   (of type <code>Automaton</code>). </param>
				/// <exception cref="IllegalArgumentException"> if this regular expression uses
				///   a named identifier that does not occur in the automaton map </exception>
				virtual Automaton *toAutomaton(std::unordered_map<std::wstring, Automaton*> &automata) throw(std::invalid_argument);

				/// <summary>
				/// Constructs new <code>Automaton</code> from this <code>RegExp</code>. 
				/// The constructed automaton has no transitions to dead states. </summary>
				/// <param name="automata"> a map from automaton identifiers to automata 
				///   (of type <code>Automaton</code>). </param>
				/// <param name="minimize"> if set, the automaton is minimized and determinized </param>
				/// <exception cref="IllegalArgumentException"> if this regular expression uses
				///   a named identifier that does not occur in the automaton map </exception>
				virtual Automaton *toAutomaton(std::unordered_map<std::wstring, Automaton*> &automata, bool minimize) throw(std::invalid_argument);

				/// <summary>
				/// Sets or resets allow mutate flag.
				/// If this flag is set, then automata construction uses mutable automata,
				/// which is slightly faster but not thread safe. 
				/// By default, the flag is not set. </summary>
				/// <param name="flag"> if true, the flag is set </param>
				/// <returns> previous value of the flag </returns>
				virtual bool setAllowMutate(bool flag);

			private:
				Automaton *toAutomatonAllowMutate(std::unordered_map<std::wstring, Automaton*> &automata, AutomatonProvider *automaton_provider, bool minimize) throw(std::invalid_argument);

				Automaton *toAutomaton(std::unordered_map<std::wstring, Automaton*> &automata, AutomatonProvider *automaton_provider, bool minimize) throw(std::invalid_argument);

				void findLeaves(RegExp *exp, Kind kind, std::vector<Automaton*> &list, std::unordered_map<std::wstring, Automaton*> &automata, AutomatonProvider *automaton_provider, bool minimize);

				/// <summary>
				/// Constructs string from parsed regular expression. 
				/// </summary>
			public:
				virtual std::wstring toString() override;

				virtual StringBuilder *toStringBuilder(StringBuilder *b);

			private:
				void appendChar(wchar_t c, StringBuilder *b);

				/// <summary>
				/// Returns set of automaton identifiers that occur in this regular expression. 
				/// </summary>
			public:
				virtual Set<std::wstring> *getIdentifiers();

				virtual void getIdentifiers(Set<std::wstring> *set);

				static RegExp *makeUnion(RegExp *exp1, RegExp *exp2);

				static RegExp *makeConcatenation(RegExp *exp1, RegExp *exp2);

			private:
				static RegExp *makeString(RegExp *exp1, RegExp *exp2);

				static RegExp *makeIntersection(RegExp *exp1, RegExp *exp2);

				static RegExp *makeOptional(RegExp *exp);

				static RegExp *makeRepeat(RegExp *exp);

				static RegExp *makeRepeat(RegExp *exp, int min);

				static RegExp *makeRepeat(RegExp *exp, int min, int max);

				static RegExp *makeComplement(RegExp *exp);

				static RegExp *makeChar(wchar_t c);

				static RegExp *makeCharRange(wchar_t from, wchar_t to);

				static RegExp *makeAnyChar();

				static RegExp *makeEmpty();

				static RegExp *makeString(const std::wstring &s);

				static RegExp *makeAnyString();

				static RegExp *makeAutomaton(const std::wstring &s);

				static RegExp *makeInterval(int min, int max, int digits);

				bool peek(const std::wstring &s);

				bool match(wchar_t c);

				bool more();

				wchar_t next() throw(std::invalid_argument);

				bool check(int flag);

			public:
				RegExp *parseUnionExp() throw(std::invalid_argument);

				RegExp *parseInterExp() throw(std::invalid_argument);

				RegExp *parseConcatExp() throw(std::invalid_argument);

				RegExp *parseRepeatExp() throw(std::invalid_argument);

				RegExp *parseComplExp() throw(std::invalid_argument);

				RegExp *parseCharClassExp() throw(std::invalid_argument);

				RegExp *parseCharClasses() throw(std::invalid_argument);

				RegExp *parseCharClass() throw(std::invalid_argument);

				RegExp *parseSimpleExp() throw(std::invalid_argument);

				wchar_t parseCharExp() throw(std::invalid_argument);
			};

		}
	}
}
