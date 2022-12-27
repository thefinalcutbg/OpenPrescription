#pragma once

#include "Model/Patient.h"

namespace DbPatient
{
    long long insert(const Patient& patient);
    bool update(const Patient& patient);
    Patient get(std::string patientID, int type);
    Patient get(long long rowid);
};

