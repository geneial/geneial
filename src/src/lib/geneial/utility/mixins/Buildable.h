#pragma once

#include <memory>

template<typename C>
class Buildable
{
public:
    using ptr = std::shared_ptr<C>;
    using const_ptr = std::shared_ptr<const C>;

    class Builder
    {
    public:
        Builder()
        {
        };

        virtual ptr create() = 0;
    };
};
