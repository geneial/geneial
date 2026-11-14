#pragma once

#include <geneial/core/operations/selection/BaseSelectionOperation.h>
#include <geneial/core/operations/selection/FitnessProportionalSelectionSettings.h>
#include <geneial/utility/mixins/EnableMakeShared.h>

#include <memory>
#include <stdexcept>

geneial_private_namespace(geneial)
{
    geneial_private_namespace(operation)
    {
        geneial_private_namespace(selection)
        {
            using ::geneial::utility::EnableMakeShared;

            geneial_export_namespace
            {

                /**
                 * Select a number of parents based on a certain criteria.
                 */
                template <typename FITNESS_TYPE>
                class FitnessProportionalSelection
                    : public BaseSelectionOperation<FITNESS_TYPE>,
                      public virtual EnableMakeShared<FitnessProportionalSelection<FITNESS_TYPE>>
                {

                  protected:
                    FitnessProportionalSelection(const std::shared_ptr<FitnessProportionalSelectionSettings> &settings)
                        : BaseSelectionOperation<FITNESS_TYPE>(settings)
                    {
                    }

                  public:
                    virtual ~FitnessProportionalSelection() {}

                    virtual typename BaseSelectionOperation<FITNESS_TYPE>::selection_result_set
                    doSelect(const Population<FITNESS_TYPE> &population,
                             BaseManager<FITNESS_TYPE> &manager) const override;

                    const FitnessProportionalSelectionSettings &getSettings() const override
                    {
                        return dynamic_cast<const FitnessProportionalSelectionSettings &>(*this->_settings);
                    }

                    void setSettings(const std::shared_ptr<BaseSelectionSettings> &settings) override
                    {
                        if (!std::dynamic_pointer_cast<FitnessProportionalSelectionSettings>(settings))
                            throw std::invalid_argument(
                                "Fitness-proportional selection requires its specialized settings");
                        BaseSelectionOperation<FITNESS_TYPE>::setSettings(settings);
                    }

                    class Builder : public BaseSelectionOperation<FITNESS_TYPE>::Builder
                    {

                      public:
                        Builder()
                            : BaseSelectionOperation<FITNESS_TYPE>::Builder(
                                  std::make_shared<FitnessProportionalSelectionSettings>())
                        {
                        }

                        FitnessProportionalSelectionSettings &getSettings()
                        {
                            return *std::dynamic_pointer_cast<FitnessProportionalSelectionSettings>(this->_settings);
                        }

                        typename BaseSelectionOperation<FITNESS_TYPE>::ptr create() override
                        {
                            auto settings =
                                std::dynamic_pointer_cast<FitnessProportionalSelectionSettings>(this->_settings);
                            return FitnessProportionalSelection<FITNESS_TYPE>::makeShared(settings);
                        }
                    };
                };

            } /* geneial_export_namespace */
        } /* private namespace selection */
    } /* private namespace operation */
} /* private namespace geneial */

#include <geneial/core/operations/selection/FitnessProportionalSelection.hpp>
