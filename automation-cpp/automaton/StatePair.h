#pragma once

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
			/// Pair of states.
			/// @author Anders M&oslash;ller &lt;<a href="mailto:amoeller@cs.au.dk">amoeller@cs.au.dk</a>&gt;
			/// </summary>
			class StatePair
			{
			public:
				State *s;
				State *s1;
				State *s2;

				virtual ~StatePair()
				{
					delete s;
					delete s1;
					delete s2;
				}

				StatePair(State *s, State *s1, State *s2);

				/// <summary>
				/// Constructs a new state pair. </summary>
				/// <param name="s1"> first state </param>
				/// <param name="s2"> second state </param>
				StatePair(State *s1, State *s2);

				/// <summary>
				/// Returns first component of this pair. </summary>
				/// <returns> first state </returns>
				virtual State *getFirstState();

				/// <summary>
				/// Returns second component of this pair. </summary>
				/// <returns> second state </returns>
				virtual State *getSecondState();

				/// <summary>
				/// Checks for equality. </summary>
				/// <param name="obj"> object to compare with </param>
				/// <returns> true if <tt>obj</tt> represents the same pair of states as this pair </returns>
				virtual bool equals(void *obj) override;

				/// <summary>
				/// Returns hash code. </summary>
				/// <returns> hash code </returns>
				virtual int hashCode() override;
			};

		}
	}
}
