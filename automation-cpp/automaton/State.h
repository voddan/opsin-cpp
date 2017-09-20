#pragma once

#include <string>
#include <vector>
#include <set>
#include "stringbuilder.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace dk { namespace brics { namespace automaton { class Transition; } } }

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
			/// <tt>Automaton</tt> state. 
			/// @author Anders M&oslash;ller &lt;<a href="mailto:amoeller@cs.au.dk">amoeller@cs.au.dk</a>&gt;
			/// </summary>
			class State //: public Serializable, public Comparable<State*>
			{

			public:
				static constexpr long long serialVersionUID = 30001;

				bool accept = false;
				std::set<Transition*> *transitions;

				int number = 0;

				int id = 0;
				static int next_id;

				/// <summary>
				/// Constructs a new state. Initially, the new state is a reject state. 
				/// </summary>
				virtual ~State()
				{
					delete transitions;
				}

				State();

				/// <summary>
				/// Resets transition set. 
				/// </summary>
				void resetTransitions();

				/// <summary>
				/// Returns the set of outgoing transitions. 
				/// Subsequent changes are reflected in the automaton. </summary>
				/// <returns> transition set </returns>
				virtual std::set<Transition*> *getTransitions();

				/// <summary>
				/// Adds an outgoing transition. </summary>
				/// <param name="t"> transition </param>
				virtual void addTransition(Transition *t);

				/// <summary>
				/// Sets acceptance for this state. </summary>
				/// <param name="accept"> if true, this state is an accept state </param>
				virtual void setAccept(bool accept);

				/// <summary>
				/// Returns acceptance status. </summary>
				/// <returns> true is this is an accept state </returns>
				virtual bool isAccept();

				/// <summary>
				/// Performs lookup in transitions, assuming determinism. </summary>
				/// <param name="c"> character to look up </param>
				/// <returns> destination state, null if no matching outgoing transition </returns>
				/// <seealso cref= #step(char, Collection) </seealso>
				virtual State *step(wchar_t c);

				/// <summary>
				/// Performs lookup in transitions, allowing nondeterminism. </summary>
				/// <param name="c"> character to look up </param>
				/// <param name="dest"> collection where destination states are stored </param>
				/// <seealso cref= #step(char) </seealso>
				virtual void step(wchar_t c, std::vector<State*> *dest);

				virtual void addEpsilon(State *to);

				/// <summary>
				/// Returns transitions sorted by (min, reverse max, to) or (to, min, reverse max) </summary>
				virtual std::vector<Transition*> getSortedTransitionArray(bool to_first);

				/// <summary>
				/// Returns sorted list of outgoing transitions. </summary>
				/// <param name="to_first"> if true, order by (to, min, reverse max); otherwise (min, reverse max, to) </param>
				/// <returns> transition list </returns>
				virtual std::vector<Transition*> getSortedTransitions(bool to_first);

				/// <summary>
				/// Returns string describing this state. Normally invoked via 
				/// <seealso cref="Automaton#toString()"/>. 
				/// </summary>
				virtual std::wstring toString();

				/// <summary>
				/// Compares this object with the specified object for order.
				/// States are ordered by the time of construction.
				/// </summary>
				virtual int compareTo(State *s);

				/// <summary>
				/// See <seealso cref="java.lang.Object#equals(java.lang.Object)"/>.
				/// </summary>
				virtual bool equals(void *obj);

				/// <summary>
				/// See <seealso cref="java.lang.Object#hashCode()"/>.
				/// </summary>
				virtual int hashCode();
			};

		}
	}
}
