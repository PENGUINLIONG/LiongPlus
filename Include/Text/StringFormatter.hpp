// File: StringFormatter.hpp
// Author: Rendong Liang (Liong)

#ifndef _L_StringFormatter
#define _L_StringFormatter
#include "../Fundamental.hpp"
#include "../Char.hpp"
#include "../Collections/List.hpp"
#include "../Exception.hpp"
#include "../String.hpp"
#include "StringBuilder.hpp"

namespace LiongPlus
{
	namespace Text
	{
		/* [Abolished]
		class IFormatProvider
			: public Interface
		{
		public:
			
		};

		class RTFormatter
			: public Object
		{
		private:
			struct Token
			{
			public:
				Token()
				{
					Buffer::Memset(this, 0, sizeof(Token));
				}
				Token(const Token& instance)
					: IsString(instance.IsString)
				{
					if (IsString)
						Str = instance.Str;
					else
						InsSite = instance.InsSite;
				}
				Token(Token&& instance)
					: IsString(true)
					, InsSite{ String(), 0, 0 }
					, Str{ 0, nullptr }
				{
					Swap(IsString, instance.IsString);
					if (sizeof(InsSite) > sizeof(Str))
						Swap(InsSite, instance.InsSite);
					else
						Swap(Str, instance.Str);
				}
				Token(_L_Char* c_str, long length)
					: Token()
				{
					IsString = true;
					Str.Length = length;
					Str.Field = c_str;
				}
				Token(long index)
					: Token()
				{
					IsString = false;
					InsSite.Index = index;
				}
				~Token()
				{
				}

				Token& operator=(const Token& instance)
				{
					IsString = instance.IsString;
					if (IsString)
						Str = instance.Str;
					else
						InsSite = instance.InsSite;
					return *this;
				}
				Token& operator=(Token&& instance)
				{
					Swap(IsString, instance.IsString);
					if (sizeof(InsSite) > sizeof(Str))
						Swap(InsSite, instance.InsSite);
					else
						Swap(Str, instance.Str);
					return *this;
				}
				bool operator==(Token& value)
				{
					return false;
				}

				bool IsString;
				union
				{
					struct
					{
						long Length;
						_L_Char* Field;
					} Str;
					struct
					{
						String Args;
						long MinLength;
					} InsSite;
				};
			};
		public:
			RTFormatter(String& format)
				: _Length(format.GetLength())
				, _Field(reinterpret_cast<_L_Char*>(format.AbandonBuffer()))
			{
				Analyse();
			}
			RTFormatter(_L_Char* format)
				: _Length(Buffer::Wcslen(format))
				, _Field(new _L_Char[_Length])
			{
				Buffer::Wcscpy(_Field, format, _Length);
				Analyse();
			}
			~RTFormatter()
			{
				delete _Field;
			}

			void Analyse()
			{
				auto beg = _Field;
				auto pos = beg;
				auto const end = beg + _Length - 1; // '\0'

				while (true)
				{
					while (pos < end)
					{
						auto c = *(pos++);

						if (pos < end && c == _LT('{'))
						{
							if (*pos == _LT('{')) // Skip {{.
							{

								++pos;
							}
							else
							{
								*(pos - 1) = Char::EndOfString;
								if (pos - beg - 1 > 0)
									_Tokens.Add(Token(beg, pos - beg - 1)); // Pure string section.
								break;
							}
						}
						else if (pos < end && c == _LT('}'))
						{
							if (*pos == _LT('}')) // Skip }}.
								++pos;
							else
								throw ArgumentException("$format");
						}
					}
					if (pos == end)
					{
						_Tokens.Add(Token(beg, pos - beg));
						return;
					}
					long index = 0;
					while (Char::IsDigit(*pos)) // Calculate the index.
					{
						index = index * 10 + *pos - _LT('0');
						pos++;
					}
					Token token(index);
					if (pos < end && *pos == _LT(',')) // Calculate the least length the string presents.
					{
						long minLength = 0;
						while (pos < end && Char::IsDigit(*(++pos)))
						{
							minLength *= 10;
							minLength += *pos - _LT('0');
						}
						token.InsSite.MinLength = minLength;
					}
					if (pos < end && *pos == _LT(':')) // Get the args to format object.
					{
						auto argsBeg = pos + 1;

						while (pos < end && Char::IsLetterOrDigit(*(++pos)));

						_L_Char* c_str = new _L_Char[pos - argsBeg + 1];
						Buffer::Wcscpy(c_str, argsBeg, pos - argsBeg);
						c_str[pos - argsBeg] = Char::EndOfString;
						token.InsSite.Args = String(c_str, pos - argsBeg + 1);
					}
					if (pos < end && *pos == _LT('}')) // Insertion site should be finished now.
					{
						_Tokens.Add(token);
						if (++pos != end)
							beg = pos;
						else
							return;
					}
					else
						throw ArgumentException("$format");
				}
			}

			template<typename ... TArgs>
			String ToString(TArgs ... args)
			{
				assert(sizeof...(args) == _Tokens.GetCount());
				StringBuilder sb;
				long index = 0;
				long i[] = { 0, (SubstituteArg(sb, args, index++), 0) ... };
			}

			template<typename T>
			Token SubstituteArg(StringBuilder& builder, T arg, long index)
			{
				for (auto& token : _Tokens)
				{
					if (token.IsString)
						builder.Append(token.Str.Field, token.Str.Length);
					else
						builder.Append(token.)
				}
				builder.Append();
			}

		private:
			List<Token> _Tokens;
			long _Length;
			_L_Char* _Field;
		};
		*/

		/*
		* To use CTFormatter like this:
		* CTFormatter<String, int, _L_Char> formatter;
		* formatter.ToString(_LT("Hi! I'm agent"), 12450, _LT('!'));
		*/
		
		/// <summary>
		/// A template-based string formatter. CT for Compile time as the insertion 'sites' are decided in compile time.
		/// </summary>
		/// <typeparam name="TArgs">The type list of insertion values.</typeparam>
		/// <note>No runtime analytic time-cost for this formatter! This is suitable for formatting with a few and non-repeated params. If not so, please use [LiongPlus::Text::RTFormatter] instead.</note>
		template<typename ... TArgs>
		class CTFormatter
			: public Object
		{
		public:
			String ToString(TArgs&& ... args)
			{
				StringBuilder builder;
				int callerArr[] = { 0, ((void)builder.Append(Forward<TArgs>(args)), 0) ... }; // Only the expressions using param pack in initializer list can be executed(compiled). As an optimization, this array will be omitted by compiler generally. The second 0 provides values of type [long] that satisfies the type of $callerArr. For more detail about the second 0, check out comma operator.

				return builder.ToString();
			}

			/* [C++17 Version]
			String ToString(TArgs&& ... args)
			{
				StringBuilder builder;
				(ImportParam(builder, builder.Append(args)), ...); // Fold expression

				return builder.ToString();
			}
			*/
		};
	}
}
#endif