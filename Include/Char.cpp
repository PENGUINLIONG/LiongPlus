// File: Char.cpp
// Author: Rendong Liang (Liong)
#include "Char.hpp"

namespace LiongPlus
{
	/// <summary>
	/// Converts a specified numeric Unicode character to a double-precision floating-point number.
	/// </summary>
	/// <param name="c">The Unicode character to convert.</param>
	/// <returns>The numeric value of c if that character represents a number; otherwise, -1.0.</returns>
	/// <remarks>
	/// CJK numeric chars are supported.
	/// U+215F will not return a value because I suppose "one devide by an unknown value will not equals to a certain value".
	/// There is a list of part of available charactors at http://stackoverflow.com/questions/2866738/whats-the-deal-with-char-getnumericvalue
	/// </remarks>
	double Char::GetNumericValue(_L_Char c)
	{
		switch (c & 0xFF00)
		{
			case 0x0000:
				if (c >= 0x0030 && c <= 0x0039)
					return c - 0x0030;
				switch (c)
				{
					case 0x00B2:
						return 2.0;
					case 0x00B3:
						return 3.0;
					case 0x00B9:
						return 1.0;
					case 0x00BC:
						return 0.25;
					case 0x00BD:
						return 0.5;
					case 0x00BE:
						return 0.75;
					default:
						break;
				}
				break;
			case 0x0600:
				if (c >= 0x0660 && c <= 0x0669)
					return c - 0x0660;
				if (c >= 0x06F0 && c <= 0x06F9)
					return c - 0x06F0;
				break;
			case 0x0700:
				if (c >= 0x07C0 && c <= 0x07C9)
					return c - 0x07C0;
				break;
			case 0x0900:
				if (c >= 0x0966 && c <= 0x096F)
					return c - 0x0966;
				if (c >= 0x09E6 && c <= 0x09EF)
					return c - 0x09E6;
				if (c >= 0x09F4 && c <= 0x09F7)
					return c - 0x09F4;
				if (c == 0x09F9)
					return 16.0;
				break;
			case 0x0A00:
				if (c >= 0x0A66 && c <= 0x0A6F)
					return c - 0x0A66;
				if (c >= 0x0AE6 && c <= 0x0AEF)
					return c - 0x0AE6;
				break;
			case 0x0B00:
				if (c >= 0x0B66 && c <= 0x0B6F)
					return c - 0x0B66;
				if (c >= 0x0BE6 && c <= 0x0BF0)
					return c - 0x0BE6;
				if (c == 0x0bF1)
					return 100.0;
				if (c == 0x0bF2)
					return 1000.0;
				break;
			case 0x0C00:
				if (c >= 0x0C66 && c <= 0x0C6F)
					return c - 0x0C66;
				if (c >= 0x0CE6 && c <= 0x0CF0)
					return c - 0x0CE6;
				break;
			case 0x0D00:
				if (c >= 0x0D66 && c <= 0x0D6F)
					return c - 0x0D66;
				break;
			case 0x0E00:
				if (c >= 0x0E50 && c <= 0x0E59)
					return c - 0x0E50;
				if (c >= 0x0ED0 && c <= 0x0ED9)
					return c - 0x0ED0;
				break;
			case 0x0F00:
				if (c >= 0x0F20 && c <= 0x0F29)
					return c - 0x0F20;
				if (c >= 0x0F2A && c <= 0x0F32)
					return (double)(c - 0x0F2A) + 0.5;
				if (c == 0x0F33)
					return -0.5;
				break;
			case 0x1000:
				if (c >= 0x1040 && c <= 0x1049)
					return c - 0x1040;
				break;
			case 0x1300:
				if (c >= 0x1369 && c <= 0x1371)
					return c - 0x1369;
				if (c >= 0x1372 && c <= 0x137B)
					return (double)(c - 0x1372) * 10.0;
				if (c == 0x137C)
					return 10000.0;
				break;
			case 0x1600:
				switch (c)
				{
					case 0x16EE:
						return 17.0;
					case 0x16EF:
						return 18.0;
					case 0x16F0:
						return 19.0;
					default:
						break;
				}
				break;
			case 0x1700:
				if (c >= 0x17E0 && c <= 0x17E9)
					return c - 0x17E0;
				if (c >= 0x17F0 && c <= 0x17F9)
					return c - 0x17F0;
				break;
			case 0x1800:
				if (c >= 0x1810 && c <= 0x1819)
					return c - 0x1810;
				break;
			case 0x1900:
				if (c >= 0x1946 && c <= 0x194F)
					return c - 0x1946;
				break;
			case 0x1B00:
				if (c >= 0x1B50 && c <= 0x1B59)
					return c - 0x1B50;
				break;
			case 0x2000:
				if (c == 0x2070 || (c >= 0x2074 && c <= 0x2079))
					return c - 0x2070;
				if (c >= 0x2080 && c <= 0x2080)
					return c - 0x2080;
				break;
			case 0x2100:
				switch (c & 0x00F0)
				{
					case 0x0050:
						switch (c)
						{
							case 0x2153:
								return 1.0 / 3.0;
							case 0x2154:
								return 2.0 / 3.0;
							case 0x2155:
								return 0.2;
							case 0x2156:
								return 0.4;
							case 0x2157:
								return 0.6;
							case 0x2158:
								return 0.8;
							case 0x2159:
								return 1.0 / 6.0;
							case 0x215A:
								return 5.0 / 6.0;
							case 0x215B:
								return 0.125;
							case 0x215C:
								return 0.375;
							case 0x215D:
								return 0.625;
							case 0x215E:
								return 0.875;
						}
						break;
					case 0x0060:
					case 0x0070:
						switch (c & 0x000F)
						{
							case 0x000C:
								return 50.0;
							case 0x000D:
								return 100.0;
							case 0x000E:
								return 500.0;
							case 0x000F:
								return 1000.0;
							default:
								return c < 2170 ? c - 0x2160 : c - 0x2170;
						}
						break;
					case 0x0080:
						switch (c)
						{
							case 0x2180:
								return 1000.0;
							case 0x2181:
								return 5000.0;
							case 0x2182:
								return 10000.0;
							default:
								break;
						}
					default:
						break;
				}
				break;
			case 0x2400:
				if (c >= 0x2460 && c <= 0x2473)
					return c - 0x2460;
				if (c >= 0x2474 && c <= 0x2487)
					return c - 0x2474;
				if (c >= 0x2488 && c <= 0x249B)
					return c - 0x2488;
				if (c == 0x24EA)
					return 0.0;
				if (c >= 0x24EB && c <= 0x24F4)
					return (double)(c - 0x24EB) + 10.0;
				if (c >= 0x24F5 && c <= 0x24FE)
					return c - 0x24F5;
				if (c == 0x24FF)
					return 0;
				break;
			case 0x2700:
				if (c >= 0x2776 && c <= 0x277F)
					return c - 0x2776;
				if (c >= 0x2780 && c <= 0x2789)
					return c - 0x2780;
				if (c >= 0x278A && c <= 0x2793)
					return c - 0x278A;
				break;
			case 0x2C00:
				if (c == 0x2CFD)
					return 0.5;
				break;
			case 0x3000:
				if (c == 0x3007)
					return 0.0;
				if (c >= 0x3021 && c <= 0x3029)
					return c - 0x3021;
				if (c >= 0x3038 && c <= 0x303A)
					return (double)(c - 0x3021) * 10.0;
				break;
			case 0x3100:
				if (c >= 0x3192 && c <= 0x3195)
					return c - 0x3192;
				break;
			case 0x3200:
				if (c >= 0x3220 && c <= 0x3229)
					return c - 0x3220;
				if (c >= 0x3251 && c <= 0x325F)
					return (double)(c - 0x3251) + 21.0;
				if (c >= 0x32B1 && c <= 0x32BF)
					return (double)(c - 0x32B1) + 36.0;
				break;
			case 0x9600:
			case 0x4E00: // CJK Support.
				switch (c)
				{
					//case 0x3007: // Checked before.
					case 0x96F6:
					case 0xF9B2:
						return 0.0;
					case 0x4E00:
					case 0x58F1:
					case 0x5E7A:
					case 0x5F0C:
					case 0xC77C:
						return 1.0;
					case 0x4E8C:
					case 0x3483:
					case 0x5169:
					case 0x5F0D:
					case 0x5F10:
					case 0x8CAE:
					case 0x8CB3:
					case 0x8D30:
					case 0xF978:
					case 0x4E24:
					case 0x4FE9:
					case 0x5006:
					case 0xC774:
						return 2.0;
					case 0x4E09:
					case 0x4EE8:
					case 0x53C1:
					case 0x53C2:
					case 0x53C3:
					case 0x53C4:
					case 0x5F0E:
					case 0xF96B:
					case 0xC0BC:
						return 3.0;
					case 0x56DB:
					case 0x4E96:
					case 0x8086:
					case 0xC0AC:
						return 4.0;
					case 0x4E94:
					case 0x4F0D:
					case 0x3405:
					case 0xC624:
						return 5.0;
					case 0x516D:
					case 0x9646:
					case 0x9678:
					case 0xF9D1:
					case 0xF9D3:
					case 0xC721:
						return 6.0;
					case 0x4E03:
					case 0x3B4D:
					case 0x67D2:
					case 0xCE60:
						return 7.0;
					case 0x516B:
					case 0x2F0B:
					case 0x634C:
					case 0xD314:
						return 8.0;
					case 0x4E5D:
					case 0x7396:
					case 0xAD6C:
						return 9.0;
					case 0x5341:
					case 0x2F17:
					case 0x62FE:
					case 0x4EC0:
					case 0xF973:
					case 0xF9FD:
					case 0xC2ED:
						return 10.0;
					case 0x767E:
					case 0x4F70:
						return 100.0;
					case 0x5343:
					case 0x4EDF:
						return 1000.0;
					case 0x4E07:
					case 0x842C:
					case 0x3E18:
						return 10000.0;
					case 0x4EBF:
					case 0x5104:
						return 100000000.0;
					case 0x5146:
						return 1000000000000.0;
					default: break;
				}
			case 0xFF00:
				if (c >= 0xFF10 && c <= 0xFF19)
					return c - 0xFF10;
				break;
			default:
				break;
		}
		return -1.0;
	}

	bool Char::IsControl(_L_Char c)
	{
		throw NotImplementedException();
	}

	bool Char::IsDigit(_L_Char c)
	{
		throw NotImplementedException();
	}

	bool Char::IsHighSurrogate(_L_Char c)
	{
		throw NotImplementedException();
	}

	bool Char::IsLetter(_L_Char c)
	{
		throw NotImplementedException();
	}

	bool Char::IsLetterOrDigit(_L_Char c)
	{
		throw NotImplementedException();
	}

	bool Char::IsLower(_L_Char c)
	{
		throw NotImplementedException();
	}

	bool Char::IsLowSurrogate(_L_Char c)
	{
		throw NotImplementedException();
	}

	bool Char::IsNumber(_L_Char c)
	{
		throw NotImplementedException();
	}

	bool Char::IsPunctuation(_L_Char c)
	{
		throw NotImplementedException();
	}

	bool Char::IsSeparator(_L_Char c)
	{
		throw NotImplementedException();
	}

	bool Char::IsSurrogate(_L_Char c)
	{
		throw NotImplementedException();
	}

	bool Char::IsSurrogatePair(_L_Char c)
	{
		throw NotImplementedException();
	}

	bool Char::IsSymbol(_L_Char c)
	{
		throw NotImplementedException();
	}

	bool Char::IsUpper(_L_Char c)
	{
		throw NotImplementedException();
	}

	bool Char::IsWhiteSpace(_L_Char c)
	{
		throw NotImplementedException();
	}

	_L_Char Char::ToLowerInvariant(_L_Char c)
	{
		throw NotImplementedException();
	}

	_L_Char Char::ToUpperInvariant(_L_Char c)
	{
		throw NotImplementedException();
	}
}
