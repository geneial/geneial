#pragma once

#include <geneial/core/population/management/BaseManager.h>
#include <geneial/namespaces.h>
#include <memory>
#include <stdexcept>
#include <unordered_set>
#include <vector>

geneial_private_namespace(geneial)
{
    geneial_private_namespace(algorithm)
    {
        geneial_private_namespace(stopping_criteria)
        {
            using ::geneial::population::management::BaseManager;
            using ::geneial::utility::Printable;
            geneial_export_namespace
            {

                template <typename FITNESS_TYPE>
                class BaseStoppingCriterion : public Printable,
                                              public std::enable_shared_from_this<BaseStoppingCriterion<FITNESS_TYPE>>
                {
                  public:
                    using ptr = std::shared_ptr<BaseStoppingCriterion>;
                    using const_ptr = std::shared_ptr<const BaseStoppingCriterion>;
                    virtual ~BaseStoppingCriterion() = default;
                    virtual void print(std::ostream &stream) const = 0;
                    virtual bool wasReached(BaseManager<FITNESS_TYPE> &manager) = 0;
                    virtual std::vector<ptr> getChildren() const
                    {
                        return {};
                    }

                    /** Clears every reachable state once, including shared children, without changing the graph. */
                    void reset()
                    {
                        std::unordered_set<const BaseStoppingCriterion *> seen{this};
                        auto pending = getChildren();
                        resetState();
                        while (!pending.empty())
                        {
                            auto child = std::move(pending.back());
                            pending.pop_back();
                            if (!child || !seen.insert(child.get()).second)
                                continue;
                            auto nested = child->getChildren();
                            child->resetState();
                            pending.insert(pending.end(), nested.begin(), nested.end());
                        }
                    }

                  protected:
                    virtual void resetState() {}
                    void validateChild(const ptr &child) const
                    {
                        if (!child)
                            throw std::invalid_argument("Criterion cannot be null");
                        std::unordered_set<const BaseStoppingCriterion *> seen;
                        std::vector<ptr> pending{child};
                        while (!pending.empty())
                        {
                            auto current = std::move(pending.back());
                            pending.pop_back();
                            if (!current)
                                continue;
                            if (current.get() == this)
                                throw std::invalid_argument("Stopping-criterion ownership cycles are not allowed");
                            if (!seen.insert(current.get()).second)
                                continue;
                            const auto nested = current->getChildren();
                            pending.insert(pending.end(), nested.begin(), nested.end());
                        }
                    }
                };
            }
        }
    }
}
