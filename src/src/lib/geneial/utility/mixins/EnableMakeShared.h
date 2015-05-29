#pragma once

/**
 * @brief This class can be used widget style like.
 * Will allow to call make shared on protected ctors.
 */
template<typename CLIENT>
class EnableMakeShared
{
protected:
    template<typename ...Arg> std::shared_ptr<CLIENT> static makeShared(Arg&&...arg)
    {
        struct LocalEnableMakeShared: public CLIENT
        {
            LocalEnableMakeShared(Arg&&...arg) :
                    CLIENT(std::forward<Arg>(arg)...)
            {
            }
        };
        return std::make_shared<LocalEnableMakeShared>(std::forward<Arg>(arg)...);
    }
};
