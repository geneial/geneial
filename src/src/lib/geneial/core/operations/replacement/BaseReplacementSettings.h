#pragma once

#include <geneial/namespaces.h>

geneial_private_namespace(geneial)
{
geneial_private_namespace(operation)
{
geneial_private_namespace(replacement)
{

geneial_export_namespace
{
class BaseReplacementSettings
{

public:
    /**
     * Every selection operation should allow the following modes:
     *     REPLACE_ALL_OFFSPRING -- Replace every offspring
     *     REPLACE_FIXED_NUMBER -- Replace a given number in the existing Population with offspring
     */
    typedef enum
    {
        REPLACE_ALL_OFFSPRING, REPLACE_FIXED_NUMBER
    } replace_offspring_mode;

    static const BaseReplacementSettings::replace_offspring_mode DEFAULT_REPLACEMENT_MODE = BaseReplacementSettings::REPLACE_ALL_OFFSPRING;
    static const unsigned int DEFAULT_AMOUNT_TO_REPLACE = 0;
    static const unsigned int DEFAULT_AMOUNT_ELTISIM = 1;

    BaseReplacementSettings():
        _mode(DEFAULT_REPLACEMENT_MODE),
        _amountToReplace(DEFAULT_AMOUNT_TO_REPLACE),
        _amountElitism(DEFAULT_AMOUNT_ELTISIM)
    {
    }


    explicit BaseReplacementSettings(const replace_offspring_mode mode, const unsigned int amountToReplace = 0, const unsigned int amountElitism =
            0) :
            _mode(mode), _amountToReplace(amountToReplace), _amountElitism(amountElitism)
    {
    }

    unsigned int getAmountToReplace() const
    {
        return _amountToReplace;
    }

    void setAmountToReplace(const unsigned int amountToReplace)
    {
        _amountToReplace = amountToReplace;
    }

    replace_offspring_mode getMode() const
    {
        return _mode;
    }

    void setMode(const replace_offspring_mode mode)
    {
        _mode = mode;
    }

    unsigned int getAmountElitism() const
    {
        return _amountElitism;
    }

    void setAmountElitism(const unsigned int amountElitism)
    {
        _amountElitism = amountElitism;
    }
private:
    replace_offspring_mode _mode;
    unsigned int _amountToReplace;
    unsigned int _amountElitism;

};

} /* geneial_export_namespace */
} /* private namespace replacement */
} /* private namespace operation */
} /* private namespace geneial */
