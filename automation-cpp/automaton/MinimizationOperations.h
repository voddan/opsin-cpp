#pragma once

#include <vector>
#include <list>
#include <set>
#include <limits>
#include <boost/optional.hpp>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace dk { namespace brics { namespace automaton { class Automaton; } } }
namespace dk { namespace brics { namespace automaton { class Transition; } } }
namespace dk { namespace brics { namespace automaton { class IntPair; } } }
namespace dk { namespace brics { namespace automaton { class State; } } }
namespace dk { namespace brics { namespace automaton { class StateListNode; } } }

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
			/// Operations for minimizing automata.
			/// </summary>
			class MinimizationOperations final
			{

			private:
				MinimizationOperations();

				/// <summary>
				/// Minimizes (and determinizes if not already deterministic) the given automaton. </summary>
				/// <seealso cref= Automaton#setMinimization(int) </seealso>
			public:
				static void minimize(Automaton *a);

			private:
				static bool statesAgree(std::vector<std::vector<Transition*>> &transitions, std::vector<std::vector<bool>> &mark, int n1, int n2);

				static void addTriggers(std::vector<std::vector<Transition*>> &transitions, std::vector<std::vector<std::unordered_set<IntPair*>>> &triggers, int n1, int n2);

				static void markPair(std::vector<std::vector<bool>> &mark, std::vector<std::vector<std::unordered_set<IntPair*>>> &triggers, int n1, int n2);

				template<typename T>
				static void initialize(std::vector<T> &list, int size);

				/// <summary>
				/// Minimizes the given automaton using Huffman's algorithm. 
				/// </summary>
			public:
				static void minimizeHuffman(Automaton *a);

				/// <summary>
				/// Minimizes the given automaton using Brzozowski's algorithm. 
				/// </summary>
				static void minimizeBrzozowski(Automaton *a);

				/// <summary>
				/// Minimizes the given automaton using Hopcroft's algorithm. 
				/// </summary>
				static void minimizeHopcroft(Automaton *a);

				/// <summary>
				/// Minimizes the given automaton using Valmari's algorithm.
				/// </summary>
				static void minimizeValmari(Automaton *automaton);

			private:
				static void makeAdjacent(std::vector<int> &A, std::vector<int> &F, std::vector<int> &K, int nn, int mm);

				static void splitTransitions(Set<State*> *states);

			public:
				class IntPair
				{

				public:
					int n1 = 0, n2 = 0;

					IntPair(int n1, int n2);
				};

			public:
				class StateList
				{

				public:
					int size = 0;

					StateListNode *first, *last;

					virtual ~StateList()
					{
						delete first;
						delete last;
					}

					virtual StateListNode *add(State *q);
				};

			public:
				class StateListNode
				{

				public:
					State *q;

					StateListNode *next, *prev;

					StateList *sl;

					virtual ~StateListNode()
					{
						delete q;
						delete next;
						delete prev;
						delete sl;
					}

					StateListNode(State *q, StateList *sl);

					virtual void remove();
				};

			public:
				class Partition
				{

				public:
					std::vector<int> markedElementCount; // number of marked elements in set
					std::vector<int> touchedSets; // sets with marked elements
					int touchedSetCount = 0; // number of sets with marked elements

					int setCount = 0; // number of sets
					std::vector<boost::optional<int>> elements; // elements, i.e s = { elements[first[s]], elements[first[s] + 1], ..., elements[past[s]-1] }
					std::vector<int> locations; // location of element i in elements
					std::vector<int> setNo; // the set number element i belongs to
					std::vector<int> first; // "first": start index of set
					std::vector<int> past; // "past": end index of set

					Partition(int size);

					virtual void mark(int e);

					virtual void split();
				};

			public:
				class LabelComparator : public Comparator<int>
				{

				private:
					std::vector<IntPair*> labels;

				public:
					LabelComparator(std::vector<IntPair*> &labels);

					virtual int compare(boost::optional<int> i, boost::optional<int> j);
				};
			};

		}
	}
}
