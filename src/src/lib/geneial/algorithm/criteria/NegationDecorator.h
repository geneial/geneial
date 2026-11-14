#pragma once

#include <geneial/algorithm/criteria/BaseStoppingCriterion.h>
#include <geneial/core/population/Population.h>
#include <stdexcept>

geneial_private_namespace(geneial)
{
    geneial_private_namespace(algorithm)
    {
        geneial_private_namespace(stopping_criteria)
        {
            using ::geneial::population::management::BaseManager;

            geneial_export_namespace
            {

                /**
                 * Decorator that negates a criterion
                 */
                template <typename FITNESS_TYPE> class NegationDecorator : public BaseStoppingCriterion<FITNESS_TYPE>
                {
                  public:
                    typedef typename BaseStoppingCriterion<FITNESS_TYPE>::ptr criterion;

                    NegationDecorator(typename BaseStoppingCriterion<FITNESS_TYPE>::ptr criterion)
                        : _criterion(criterion)
                    {
                        this->validateChild(criterion);
                    }

                    ~NegationDecorator() override = default;

                    bool wasReached(BaseManager<FITNESS_TYPE> &manager) override
                    {
                        return !(_criterion->wasReached(manager));
                    }

                    void print(std::ostream &os) const override
                    {
                        os << "Negated (" << *_criterion << ")";
                    }

                    const criterion &getCriterion() const
                    {
                        return _criterion;
                    }

                    std::vector<criterion> getChildren() const override
                    {
                        return {_criterion};
                    }

                    void setCriterion(const criterion &criterion)
                    {
                        this->validateChild(criterion);
                        _criterion = criterion;
                    }

                  private:
                    criterion _criterion;
                };

            } /* geneial_export_namespace */
        } /* private namespace stopping_criteria */
    } /* private namespace algorithm */
} /* private namespace geneial */
