#pragma once

#include <functional>

#include <geneial/namespaces.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(utility)
{

geneial_export_namespace
{

struct PairHasher { //https://stackoverflow.com/questions/20590656/error-for-hash-function-of-pair-of-ints
	public:
	  template <typename T, typename U>
	  std::size_t operator()(const std::pair<T, U> &x) const
	  {
	    return std::hash<T>()(x.first) ^ std::hash<U>()(x.second);
	  }
	};


} /* export namespace */
} /* namespace utility */
} /* namespace geneial */
