#include "NameValidator.h"


bool NameValidator::validateInput(const std::string& text)
{
    if (!not_empty.validateInput(text))
    {
        _errorMsg = &not_empty.getErrorMessage();
        return false;
    }

    if (!cyrillic_only.validateInput(text))
    {
        _errorMsg = &mustBeInCyrillic;
        return false;
    }
    
    /*
    _errorMsg = &invalidName;
    
    auto& firstCharacter = text[0];

        if (firstCharacter == ' ' || firstCharacter == '-')
        {
            return false;
        }

    */

    return true;;
};