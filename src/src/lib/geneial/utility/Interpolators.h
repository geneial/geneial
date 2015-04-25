#ifndef __GENEIAL_INTERPOLATORS_H_
#define __GENEIAL_INTERPOLATORS_H_

namespace geneial {
namespace utility {

//Library for some common smoothing operations


class Interpolators{

private:
	Interpolators(){};
	virtual ~Interpolators(){};

public:

	template<typename VALUE_TYPE>
	static inline VALUE_TYPE linearInterpolate(VALUE_TYPE y0, VALUE_TYPE x0, VALUE_TYPE y1, VALUE_TYPE x1, VALUE_TYPE x);

    template<typename VALUE_TYPE>
    static inline VALUE_TYPE linearInterpolate(VALUE_TYPE y0, VALUE_TYPE y1, VALUE_TYPE t);



	template<typename VALUE_TYPE>
	static inline VALUE_TYPE cosineInterpolate(VALUE_TYPE y0, VALUE_TYPE x0, VALUE_TYPE y1, VALUE_TYPE x1, VALUE_TYPE x);

	template<typename VALUE_TYPE>
	static inline VALUE_TYPE cosineInterpolate(VALUE_TYPE y0, VALUE_TYPE y1, VALUE_TYPE t);

};

} /* namespace utility */
} /* namespace geneial */


#include <geneial/utility/Interpolators.hpp>


#endif /* __GENEIAL_INTERPOLATORS_H_ */
