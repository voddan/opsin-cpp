#pragma once

#include <string>
#include <vector>
#include <limits>
#include "stringbuilder.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace dk { namespace brics { namespace automaton { class Automaton; }}}
namespace dk { namespace brics { namespace automaton { class AutomatonMatcher; }}}

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
            /// Finite-state automaton with fast run operation.
            /// @author Anders M&oslash;ller &lt;<a href="mailto:amoeller@cs.au.dk">amoeller@cs.au.dk</a>&gt;
            /// </summary>
            class RunAutomaton //: public Serializable
            {

            public:
                static constexpr long long serialVersionUID = 20001;

                int size = 0;
                std::vector<bool> accept;
                int initial = 0;
                std::vector<int> transitions; // delta(state,c) = transitions[state*points.length + getCharClass(c)]
                std::vector<wchar_t> points; // char interval start points
                std::vector<int> classmap; // map from char number to class class

                /// <summary>
                /// Sets alphabet table for optimal run performance.
                /// </summary>
                virtual void setAlphabet();

                /// <summary>
                /// Returns a string representation of this automaton.
                /// </summary>
                virtual std::wstring toString();

                /// <summary>
                /// Returns number of states in automaton.
                /// </summary>
                virtual int getSize();

                /// <summary>
                /// Returns acceptance status for given state.
                /// </summary>
                virtual bool isAccept(int state);

                /// <summary>
                /// Returns initial state.
                /// </summary>
                virtual int getInitialState();

                /// <summary>
                /// Returns array of character class interval start points. The array should
                /// not be modified by the caller.
                /// </summary>
                virtual std::vector<wchar_t> getCharIntervals();

                /// <summary>
                /// Gets character class of given char.
                /// </summary>
                virtual int getCharClass(wchar_t c);

            private:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @SuppressWarnings("unused") private RunAutomaton()
                RunAutomaton();

                /// <summary>
                /// Constructs a new <code>RunAutomaton</code> from a deterministic
                /// <code>Automaton</code>. Same as <code>RunAutomaton(a, true)</code>. </summary>
                /// <param name="a"> an automaton </param>
            public:
                RunAutomaton(Automaton * a);

                /// <summary>
                /// Retrieves a serialized <code>RunAutomaton</code> located by a URL. </summary>
                /// <param name="url"> URL of serialized automaton </param>
                /// <exception cref="IOException"> if input/output related exception occurs </exception>
                /// <exception cref="ClassCastException"> if the data is not a serialized <code>RunAutomaton</code> </exception>
                /// <exception cref="ClassNotFoundException"> if the class of the serialized object cannot be found </exception>
//				static RunAutomaton *load(URL *url) throw(std::ios_base::failure, std::bad_cast);

                /// <summary>
                /// Retrieves a serialized <code>RunAutomaton</code> from a stream. </summary>
                /// <param name="stream"> input stream with serialized automaton </param>
                /// <exception cref="IOException"> if input/output related exception occurs </exception>
                /// <exception cref="ClassCastException"> if the data is not a serialized <code>RunAutomaton</code> </exception>
                /// <exception cref="ClassNotFoundException"> if the class of the serialized object cannot be found </exception>
                static RunAutomaton * load(std::istream stream) throw(std::ios_base::failure, std::bad_cast);

                /// <summary>
                /// Writes this <code>RunAutomaton</code> to the given stream. </summary>
                /// <param name="stream"> output stream for serialized automaton </param>
                /// <exception cref="IOException"> if input/output related exception occurs </exception>
                virtual void store(std::ostream stream) throw(std::ios_base::failure);

                /// <summary>
                /// Constructs a new <code>RunAutomaton</code> from a deterministic
                /// <code>Automaton</code>. If the given automaton is not deterministic,
                /// it is determinized first. </summary>
                /// <param name="a"> an automaton </param>
                /// <param name="tableize"> if true, a transition table is created which makes the <code>run</code>
                ///                 method faster in return of a higher memory usage </param>
                RunAutomaton(Automaton * a, bool tableize);

                /// <summary>
                /// Returns the state obtained by reading the given char from the given
                /// state. Returns -1 if not obtaining any such state. (If the original
                /// <code>Automaton</code> had no dead states, -1 is returned here if and
                /// only if a dead state is entered in an equivalent automaton with a total
                /// transition function.)
                /// </summary>
                virtual int step(int state, wchar_t c);

                /// <summary>
                /// Returns true if the given string is accepted by this automaton.
                /// </summary>
                virtual bool run(const std::wstring & s);

                /// <summary>
                /// Returns the length of the longest accepted run of the given string
                /// starting at the given offset. </summary>
                /// <param name="s"> the string </param>
                /// <param name="offset"> offset into <code>s</code> where the run starts </param>
                /// <returns> length of the longest accepted run, -1 if no run is accepted </returns>
                virtual int run(const std::wstring & s, int offset);

                /// <summary>
                /// Creates a new automaton matcher for the given input. </summary>
                /// <param name="s"> the CharSequence to search </param>
                /// <returns> A new automaton matcher for the given input </returns>
                virtual AutomatonMatcher * newMatcher(std::string * s);

                /// <summary>
                /// Creates a new automaton matcher for the given input. </summary>
                /// <param name="s"> the CharSequence to search </param>
                /// <param name="startOffset"> the starting offset of the given character sequence </param>
                /// <param name="endOffset"> the ending offset of the given character sequence </param>
                /// <returns> A new automaton matcher for the given input </returns>
                virtual AutomatonMatcher * newMatcher(std::string * s, int startOffset, int endOffset);
            };

        }
    }
}
