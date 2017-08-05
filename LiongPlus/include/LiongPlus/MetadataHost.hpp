// File: MetadataHost.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include <any>
#include <map>
#include <string>
#include "Interface.hpp"

_L_NS_BEG

class MetadataHost
{
public:
	std::map<std::string, std::any> Metadata;
};

using IMetadataHost = I<MetadataHost>;

_L_NS_END
