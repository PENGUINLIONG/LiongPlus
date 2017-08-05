// File: Path.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include "AnsiCharUtils.hpp"
#include "_"

_L_NS_BEG

namespace Path
{
	template<typename TStr>
	TStr GetExtension(const TStr& path)
	{
		auto rPos = AnsiCharUtils::SeekForChar(path.rbegin(), path.rend(), '.');
		TStr rv(path.rbegin(), rPos);
		std::reverse(rv.begin(), rv.end());
		return rv;
	}

	template<typename TStr>
	TStr GetDirectory(const TStr& path)
	{
		auto rPos = AnsiCharUtils::SeekForChar(path.rbegin(), path.rend(), '/', '\\');
		TStr rv(rPos + 1, path.rend());
		std::reverse(rv.begin(), rv.end());
		return rv;
	}

	template<typename TStr>
	TStr GetFileName(const TStr& path)
	{
		auto rPos = AnsiCharUtils::SeekForChar(path.rbegin(), path.rend(), '/', '\\');
		TStr rv(path.rbegin(), rPos);
		std::reverse(rv.begin(), rv.end());
		return rv;
	}

	template<typename TStr>
	TStr GetFileNameNoExt(const TStr& path)
	{
		auto rSlashPos = AnsiCharUtils::SeekForChar(path.rbegin(), path.rend(), '/', '\\');
		auto rDotPos = AnsiCharUtils::SeekForChar(path.rbegin(), rSlashPos, '.');
		TStr rv(rDotPos + 1, rSlashPos);
		std::reverse(rv.begin(), rv.end());
		return rv;
	}

	/*
	 * Note:
	 *   ONLY token '.' and '..' with nothing other than '/' or '\'
	 *     attached will be recognized as relative directory.
	 */
	template<typename TStr>
	TStr ToAbsolutePath(const TStr baseDir, const TStr relativePath)
	{
		auto rv = baseDir;
		bool firstDotFound = true;
		auto tokenBeg = relativePath.begin();

		for (;;)
		{
			const auto tokenEnd = AnsiCharUtils::SeekForChar(tokenBeg, relativePath.end(), '/', '\\');
			if (tokenBeg == tokenEnd) break;
			TStr token(tokenBeg, tokenEnd);
			if (1 == token.size() && '.' == token[0])
				;
			else if (2 == token.size() && '.' == token[0] && '.' == token[1])
				rv = GetDirectory(rv);
			else
			{
				rv.reserve(rv.size() + 1 + token.size());
				rv.append("/");
				rv.append(token);
			}
			tokenBeg = tokenEnd + 1;
		}

		return rv;
	}
}

_L_NS_END
