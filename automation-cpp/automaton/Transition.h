#pragma once

#include <string>
#include <stdexcept>
#include "stringbuilder.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
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
			/// <tt>Automaton</tt> transition. 
			/// <para>
			/// A transition, which belongs to a source state, consists of a Unicode character interval
			/// and a destination state.
			/// @author Anders M&oslash;ller &lt;<a href="mailto:amoeller@cs.au.dk">amoeller@cs.au.dk</a>&gt;
			/// </para>
			/// </summary>
			class Transition : public Serializable, public Cloneable
			{

			public:
				static constexpr long long serialVersionUID = 40001;

				/* 
				 * CLASS INVARIANT: min<=max
				 */

				wchar_t min = L'\0';
				wchar_t max = L'\0';

				State *to;

				/// <summary>
				/// Constructs a new singleton interval transition. </summary>
				/// <param name="c"> transition character </param>
				/// <param name="to"> destination state </param>
				virtual ~Transition()
				{
					delete to;
				}

				Transition(wchar_t c, State *to);

				/// <summary>
				/// Constructs a new transition. 
				/// Both end points are included in the interval. </summary>
				/// <param name="min"> transition interval minimum </param>
				/// <param name="max"> transition interval maximum </param>
				/// <param name="to"> destination state </param>
				Transition(wchar_t min, wchar_t max, State *to);

				/// <summary>
				/// Returns minimum of this transition interval. </summary>
				virtual wchar_t getMin();

				/// <summary>
				/// Returns maximum of this transition interval. </summary>
				virtual wchar_t getMax();

				/// <summary>
				/// Returns destination of this transition. </summary>
				virtual State *getDest();

				/// <summary>
				/// Checks for equality. </summary>
				/// <param name="obj"> object to compare with </param>
				/// <returns> true if <tt>obj</tt> is a transition with same 
				///         character interval and destination state as this transition. </returns>
				virtual bool equals(void *obj) override;

				/// <summary>
				/// Returns hash code.
				/// The hash code is based on the character interval (not the destination state). </summary>
				/// <returns> hash code </returns>
				virtual int hashCode() override;

				/// <summary>
				/// Clones this transition. </summary>
				/// <returns> clone with same character interval and destination state </returns>
				virtual Transition *clone() override;

				static void appendCharString(wchar_t c, StringBuilder *b);

				/// <summary>
				/// Returns a string describing this state. Normally invoked via 
				/// <seealso cref="Automaton#toString()"/>. 
				/// </summary>
				virtual std::wstring toString() override;

				virtual void appendDot(StringBuilder *b);
			};

		}
	}
}
