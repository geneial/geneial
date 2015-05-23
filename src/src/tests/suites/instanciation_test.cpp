#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE algorithm/criteria

#include <boost/test/unit_test.hpp>

#include <geneial/algorithm/SteadyStateAlgorithm.h>

#include <geneial/core/operations/mutation/NonUniformMutationOperation.h>
#include <geneial/core/operations/mutation/SmoothPeakMutationOperation.h>
#include <geneial/core/operations/mutation/UniformMutationOperation.h>


#ifndef NDEBUG

/**
 * @brief Initialization is used to ensure all the geneial code compiles,
 * regardless of whether it is being used in some demo program or not.
 */

//Algorithm

template class geneial::algorithm::SteadyStateAlgorithm<int>;
template class geneial::algorithm::SteadyStateAlgorithm<float>;
template class geneial::algorithm::SteadyStateAlgorithm<double>;


//Mutation
template class geneial::operation::mutation::NonUniformMutationOperation<int,int>;
template class geneial::operation::mutation::NonUniformMutationOperation<float,int>;
template class geneial::operation::mutation::NonUniformMutationOperation<double,int>;

template class geneial::operation::mutation::NonUniformMutationOperation<int,float>;
template class geneial::operation::mutation::NonUniformMutationOperation<float,float>;
template class geneial::operation::mutation::NonUniformMutationOperation<double,float>;

template class geneial::operation::mutation::NonUniformMutationOperation<int,double>;
template class geneial::operation::mutation::NonUniformMutationOperation<float,double>;
template class geneial::operation::mutation::NonUniformMutationOperation<double,double>;


template class geneial::operation::mutation::SmoothPeakMutationOperation<int,int>;
template class geneial::operation::mutation::SmoothPeakMutationOperation<float,int>;
template class geneial::operation::mutation::SmoothPeakMutationOperation<double,int>;

template class geneial::operation::mutation::SmoothPeakMutationOperation<int,float>;
template class geneial::operation::mutation::SmoothPeakMutationOperation<float,float>;
template class geneial::operation::mutation::SmoothPeakMutationOperation<double,float>;

template class geneial::operation::mutation::SmoothPeakMutationOperation<int,double>;
template class geneial::operation::mutation::SmoothPeakMutationOperation<float,double>;
template class geneial::operation::mutation::SmoothPeakMutationOperation<double,double>;


template class geneial::operation::mutation::UniformMutationOperation<int,int>;
template class geneial::operation::mutation::UniformMutationOperation<float,int>;
template class geneial::operation::mutation::UniformMutationOperation<double,int>;

template class geneial::operation::mutation::UniformMutationOperation<int,float>;
template class geneial::operation::mutation::UniformMutationOperation<float,float>;
template class geneial::operation::mutation::UniformMutationOperation<double,float>;

template class geneial::operation::mutation::UniformMutationOperation<int,double>;
template class geneial::operation::mutation::UniformMutationOperation<float,double>;
template class geneial::operation::mutation::UniformMutationOperation<double,double>;


#endif




BOOST_AUTO_TEST_SUITE( __INSTANCIATION_TEST_ALIBI );
BOOST_AUTO_TEST_CASE( Instanciation )
{
    BOOST_CHECK(1==1); //when this test compiles it already has reached it's purpose
}
BOOST_AUTO_TEST_SUITE_END()

