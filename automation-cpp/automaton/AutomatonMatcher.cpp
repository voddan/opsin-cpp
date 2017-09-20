#include "AutomatonMatcher.h"
#include "RunAutomaton.h"

namespace dk
{
	namespace brics
	{
		namespace automaton
		{

			AutomatonMatcher::AutomatonMatcher(CharSequence *const chars, RunAutomaton *const automaton) : automaton(automaton), chars(chars)
			{
			}

			bool AutomatonMatcher::find()
			{
				int begin;
				switch (getMatchStart())
				{
					case -2:
					return false;
					case -1:
					begin = 0;
						break;
					default:
					begin = getMatchEnd();
						// This occurs when a previous find() call matched the empty string. This can happen when the pattern is a* for example.
						if (begin == getMatchStart())
						{
							begin += 1;
							if (begin > getChars()->length())
							{
								setMatch(-2, -2);
								return false;
							}
						}
				}

				int match_start;
				int match_end;
				if (automaton->isAccept(automaton->getInitialState()))
				{
					match_start = begin;
					match_end = begin;
				}
				else
				{
					match_start = -1;
					match_end = -1;
				}
				int l = getChars()->length();
				while (begin < l)
				{
					int p = automaton->getInitialState();
					for (int i = begin; i < l; i++)
					{
						constexpr int new_state = automaton->step(p, getChars()->charAt(i));
						if (new_state == -1)
						{
							break;
						}
						else if (automaton->isAccept(new_state))
						{
							// found a match from begin to (i+1)
							match_start = begin;
							match_end = (i + 1);
						}
						p = new_state;
					}
					if (match_start != -1)
					{
						setMatch(match_start, match_end);
						return true;
					}
					begin += 1;
				}
				if (match_start != -1)
				{
					setMatch(match_start, match_end);
					return true;
				}
				else
				{
					setMatch(-2, -2);
					return false;
				}
			}

			void AutomatonMatcher::setMatch(int const matchStart, int const matchEnd) throw(std::invalid_argument)
			{
				if (matchStart > matchEnd)
				{
					throw std::invalid_argument(std::wstring(L"Start must be less than or equal to end: ") + matchStart + std::wstring(L", ") + std::to_wstring(matchEnd));
				}
				this->matchStart = matchStart;
				this->matchEnd = matchEnd;
			}

			int AutomatonMatcher::getMatchStart()
			{
				return matchStart;
			}

			int AutomatonMatcher::getMatchEnd()
			{
				return matchEnd;
			}

			CharSequence *AutomatonMatcher::getChars()
			{
				return chars;
			}

			int AutomatonMatcher::end() throw(IllegalStateException)
			{
				matchGood();
				return matchEnd;
			}

			int AutomatonMatcher::end(int const group) throw(std::out_of_range, IllegalStateException)
			{
				onlyZero(group);
				return end();
			}

			std::wstring AutomatonMatcher::group() throw(IllegalStateException)
			{
				matchGood();
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
				return chars->substr(matchStart, matchEnd - matchStart)->toString();
			}

			std::wstring AutomatonMatcher::group(int const group) throw(std::out_of_range, IllegalStateException)
			{
				onlyZero(group);
				return group();
			}

			int AutomatonMatcher::groupCount()
			{
				return 0;
			}

			int AutomatonMatcher::start() throw(IllegalStateException)
			{
				matchGood();
				return matchStart;
			}

			int AutomatonMatcher::start(int group) throw(std::out_of_range, IllegalStateException)
			{
				onlyZero(group);
				return start();
			}

			MatchResult *AutomatonMatcher::toMatchResult()
			{
				AutomatonMatcher * const match = new AutomatonMatcher(chars, automaton);
				match->matchStart = this->matchStart;
				match->matchEnd = this->matchEnd;
				return match;
			}

			void AutomatonMatcher::onlyZero(int const group) throw(std::out_of_range)
			{
				if (group != 0)
				{
					throw std::out_of_range("The only group supported is 0.");
				}
			}

			void AutomatonMatcher::matchGood() throw(IllegalStateException)
			{
				if ((matchStart < 0) || (matchEnd < 0))
				{
					throw IllegalStateException(L"There was no available match.");
				}
			}
		}
	}
}
