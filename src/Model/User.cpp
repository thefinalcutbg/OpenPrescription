#include "User.h"

static Doctor s_doctor;

const Doctor& User::doctor()
{
    return s_doctor;
}

void User::setCurrentDoctor(const Doctor& doctor)
{
    s_doctor = doctor;
}
