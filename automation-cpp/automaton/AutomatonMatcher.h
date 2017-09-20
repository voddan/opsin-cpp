#pragma once

#include <string>
#include <stdexcept>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace dk { namespace brics { namespace automaton { class RunAutomaton; }}}

/*
 * dk.brics.automaton - AutomatonMatcher
 *
 * Copyright (c) 2008-2017 John Gibson
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
            /// A tool that performs match operations on a given character sequence using
            /// a compiled automaton.
            ///
            /// @author John Gibson &lt;<a href="mailto:jgibson@mitre.org">jgibson@mitre.org</a>&gt; </summary>
            /// <seealso cref= RunAutomaton#newMatcher(java.lang.CharSequence) </seealso>
            /// <seealso cref= RunAutomaton#newMatcher(java.lang.CharSequence, int, int) </seealso>
            class AutomatonMatcher //: public MatchResult
            {

            public:
                virtual ~AutomatonMatcher() {
                    delete automaton;
                    delete chars;
                }

                AutomatonMatcher(std::string * const chars, RunAutomaton * const automaton);

            private:
                RunAutomaton * const automaton;
                std::string * const chars;

                int matchStart = -1;

                int matchEnd = -1;

                /// <summary>
                /// Find the next matching subsequence of the input.
                /// <br>
                /// This also updates the values for the {@code start}, {@code end}, and
                /// {@code group} methods.
                /// </summary>
                /// <returns> {@code true} if there is a matching subsequence. </returns>
            public:
                virtual bool find();

            private:
                void setMatch(int const matchStart, int const matchEnd) throw(std::invalid_argument);

                int getMatchStart();

                int getMatchEnd();

                std::string * getChars();

                /// <summary>
                /// Returns the offset after the last character matched.
                /// </summary>
                /// <returns> The offset after the last character matched. </returns>
                /// <exception cref="IllegalStateException"> if there has not been a match attempt or
                ///  if the last attempt yielded no results. </exception>
            public:
                virtual int end() throw(std::domain_error);

                /// <summary>
                /// Returns the offset after the last character matched of the specified
                /// capturing group.
                /// <br>
                /// Note that because the automaton does not support capturing groups the
                /// only valid group is 0 (the entire match).
                /// </summary>
                /// <param name="group"> the desired capturing group. </param>
                /// <returns> The offset after the last character matched of the specified
                ///  capturing group. </returns>
                /// <exception cref="IllegalStateException"> if there has not been a match attempt or
                ///  if the last attempt yielded no results. </exception>
                /// <exception cref="IndexOutOfBoundsException"> if the specified capturing group does
                ///  not exist in the underlying automaton. </exception>
                virtual int end(int const group) throw(std::out_of_range, std::domain_error);

                /// <summary>
                /// Returns the subsequence of the input found by the previous match.
                /// </summary>
                /// <returns> The subsequence of the input found by the previous match. </returns>
                /// <exception cref="IllegalStateException"> if there has not been a match attempt or
                ///  if the last attempt yielded no results. </exception>
                virtual std::wstring group() throw(std::domain_error);

                /// <summary>
                /// Returns the subsequence of the input found by the specified capturing
                /// group during the previous match operation.
                /// <br>
                /// Note that because the automaton does not support capturing groups the
                /// only valid group is 0 (the entire match).
                /// </summary>
                /// <param name="group"> the desired capturing group. </param>
                /// <returns> The subsequence of the input found by the specified capturing
                ///  group during the previous match operation the previous match. Or
                ///  {@code null} if the given group did match. </returns>
                /// <exception cref="IllegalStateException"> if there has not been a match attempt or
                ///  if the last attempt yielded no results. </exception>
                /// <exception cref="IndexOutOfBoundsException"> if the specified capturing group does
                ///  not exist in the underlying automaton. </exception>
                virtual std::wstring group(int const group) throw(std::out_of_range, std::domain_error);

                /// <summary>
                /// Returns the number of capturing groups in the underlying automaton.
                /// <br>
                /// Note that because the automaton does not support capturing groups this
                /// method will always return 0.
                /// </summary>
                /// <returns> The number of capturing groups in the underlying automaton. </returns>
                virtual int groupCount();

                /// <summary>
                /// Returns the offset of the first character matched.
                /// </summary>
                /// <returns> The offset of the first character matched. </returns>
                /// <exception cref="IllegalStateException"> if there has not been a match attempt or
                ///  if the last attempt yielded no results. </exception>
                virtual int start() throw(std::domain_error);

                /// <summary>
                /// Returns the offset of the first character matched of the specified
                /// capturing group.
                /// <br>
                /// Note that because the automaton does not support capturing groups the
                /// only valid group is 0 (the entire match).
                /// </summary>
                /// <param name="group"> the desired capturing group. </param>
                /// <returns> The offset of the first character matched of the specified
                ///  capturing group. </returns>
                /// <exception cref="IllegalStateException"> if there has not been a match attempt or
                ///  if the last attempt yielded no results. </exception>
                /// <exception cref="IndexOutOfBoundsException"> if the specified capturing group does
                ///  not exist in the underlying automaton. </exception>
                virtual int start(int group) throw(std::out_of_range, std::domain_error);

                /// <summary>
                /// Returns the current state of this {@code AutomatonMatcher} as a
                /// {@code MatchResult}.
                /// The result is unaffected by subsequent operations on this object.
                /// </summary>
                /// <returns> a {@code MatchResult} with the state of this
                ///  {@code AutomatonMatcher}. </returns>
                virtual AutomatonMatcher * toMatchResult();

                /// <summary>
                /// Helper method that requires the group argument to be 0. </summary>
            private:
                static void onlyZero(int const group) throw(std::out_of_range);

                /// <summary>
                /// Helper method to check that the last match attempt was valid. </summary>
                void matchGood() throw(std::domain_error);
            };

        }
    }
}
