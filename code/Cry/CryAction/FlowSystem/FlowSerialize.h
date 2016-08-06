// Copyright 2001-2016 Crytek GmbH / Crytek Group. All rights reserved.


#ifndef __FLOWSERIALIZE_H__
#define __FLOWSERIALIZE_H__

#pragma once

#include "IFlowSystem.h"

bool   SetFromString( TFlowInputData &value, const char* str );
string ConvertToString( const TFlowInputData &value );
bool   SetAttr( XmlNodeRef node, const char* attr, const TFlowInputData &value );

#endif