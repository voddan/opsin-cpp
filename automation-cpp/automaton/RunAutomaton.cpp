#include "RunAutomaton.h"
#include "Transition.h"
#include "SpecialOperations.h"
#include "Automaton.h"
#include "State.h"
#include "AutomatonMatcher.h"

namespace dk
{
	namespace brics
	{
		namespace automaton
		{

			void RunAutomaton::setAlphabet()
			{
				classmap = std::vector<int>(std::numeric_limits<wchar_t>::max() - std::numeric_limits<wchar_t>::min() + 1);
				int i = 0;
				for (int j = 0; j <= std::numeric_limits<wchar_t>::max() - std::numeric_limits<wchar_t>::min(); j++)
				{
					if (i + 1 < points.size() && j == points[i + 1])
					{
						i++;
					}
					classmap[j] = i;
				}
			}

			std::wstring RunAutomaton::toString()
			{
				StringBuilder *b = new StringBuilder();
				b->append(L"initial state: ")->append(initial)->append(L"\n");
				for (int i = 0; i < size; i++)
				{
					b->append(L"state ")->append(i);
					if (accept[i])
					{
						b->append(L" [accept]:\n");
					}
					else
					{
						b->append(L" [reject]:\n");
					}
					for (int j = 0; j < points.size(); j++)
					{
						int k = transitions[i * points.size() + j];
						if (k != -1)
						{
							wchar_t min = points[j];
							wchar_t max;
							if (j + 1 < points.size())
							{
								max = static_cast<wchar_t>(points[j + 1] - 1);
							}
							else
							{
								max = std::numeric_limits<wchar_t>::max();
							}
							b->append(L" ");
							Transition::appendCharString(min, b);
							if (min != max)
							{
								b->append(L"-");
								Transition::appendCharString(max, b);
							}
							b->append(L" -> ")->append(k)->append(L"\n");
						}
					}
				}
				return b->toString();
			}

			int RunAutomaton::getSize()
			{
				return size;
			}

			bool RunAutomaton::isAccept(int state)
			{
				return accept[state];
			}

			int RunAutomaton::getInitialState()
			{
				return initial;
			}

			std::vector<wchar_t> RunAutomaton::getCharIntervals()
			{
				return points.clone();
			}

			int RunAutomaton::getCharClass(wchar_t c)
			{
				return SpecialOperations::findIndex(c, points);
			}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @SuppressWarnings("unused") private RunAutomaton()
			RunAutomaton::RunAutomaton()
			{
			}

			RunAutomaton::RunAutomaton(Automaton *a) : RunAutomaton(a, true)
			{
			}

//			RunAutomaton *RunAutomaton::load(URL *url) throw(IOException, ClassCastException, ClassNotFoundException)
//			{
//				return load(url->openStream());
//			}

			RunAutomaton *RunAutomaton::load(std::istream stream) throw(IOException, ClassCastException, ClassNotFoundException)
			{
				ObjectInputStream *s = new ObjectInputStream(*stream);
				return static_cast<RunAutomaton*>(s->readObject());
			}

			void RunAutomaton::store(std::ostream stream) throw(IOException)
			{
				ObjectOutputStream *s = new ObjectOutputStream(*stream);
				s->writeObject(this);
				s->flush();
			}

			RunAutomaton::RunAutomaton(Automaton *a, bool tableize)
			{
				a->determinize();
				points = a->getStartPoints();
				Set<State*> *states = a->getStates();
				Automaton::setStateNumbers(states);
				initial = a->initial->number;
				size = states->size();
				accept = std::vector<bool>(size);
				transitions = std::vector<int>(size * points.size());
				for (int n = 0; n < size * points.size(); n++)
				{
					transitions[n] = -1;
				}
				for (auto s : states)
				{
					int n = s->number;
					accept[n] = s->accept;
					for (int c = 0; c < points.size(); c++)
					{
						State *q = s->step(points[c]);
						if (q != nullptr)
						{
							transitions[n * points.size() + c] = q->number;
						}
					}
				}
				if (tableize)
				{
					setAlphabet();
				}
			}

			int RunAutomaton::step(int state, wchar_t c)
			{
				if (classmap.empty())
				{
					return transitions[state * points.size() + getCharClass(c)];
				}
				else
				{
					return transitions[state * points.size() + classmap[c - std::numeric_limits<wchar_t>::min()]];
				}
			}

			bool RunAutomaton::run(const std::wstring &s)
			{
				int p = initial;
				int l = s.length();
				for (int i = 0; i < l; i++)
				{
					p = step(p, s[i]);
					if (p == -1)
					{
						return false;
					}
				}
				return accept[p];
			}

			int RunAutomaton::run(const std::wstring &s, int offset)
			{
				int p = initial;
				int l = s.length();
				int max = -1;
				for (int r = 0; offset <= l; offset++, r++)
				{
					if (accept[p])
					{
						max = r;
					}
					if (offset == l)
					{
						break;
					}
					p = step(p, s[offset]);
					if (p == -1)
					{
						break;
					}
				}
				return max;
			}

			AutomatonMatcher *RunAutomaton::newMatcher(std::string *s)
			{
				return new AutomatonMatcher(s, this);
			}

			AutomatonMatcher *RunAutomaton::newMatcher(std::string *s, int startOffset, int endOffset)
			{
				return new AutomatonMatcher(s->substr(startOffset, endOffset - startOffset), this);
			}
		}
	}
}
