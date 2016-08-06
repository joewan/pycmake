// Copyright 2001-2016 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <yasli/Enum.h>
#include "NameGeneration.h"

///////////////////////////////////////////////////////////////////////
// Implement serialization for a seperately defined enum
#define SERIALIZATION_ENUM_BEGIN(Type, label) YASLI_ENUM_BEGIN(Type, label)
#define SERIALIZATION_ENUM_BEGIN_NESTED(Class, Enum, label) YASLI_ENUM_BEGIN_NESTED(Class, Enum, label)
#define SERIALIZATION_ENUM_BEGIN_NESTED2(Class1, Class2, Enum, label) YASLI_ENUM_BEGIN_NESTED2(Class1, Class2, Enum, label)
#define SERIALIZATION_ENUM(value, name, label) YASLI_ENUM(value, name, label)
#define SERIALIZATION_ENUM_END() YASLI_ENUM_END()

///////////////////////////////////////////////////////////////////////
// Define enum and serialization together.
// Enum has scoped names (enum class), and optional size.
// Each enum value has its serialization "name" equal to its code id,
// and its "label" automatically generated from the name,
// with automatic capitalization and spaces. Ex:
//		fixed      -> "Fixed"
//		blackHole  -> "Black Hole"
//		heat_death -> "Heat Death"

// Declare an enum and registration class, for use in header files. Ex:
//		SERIALIZATION_ENUM_DECLARE(EType,, one = 1, two, duece = two, three)  // assigned values are allowed
//		SERIALIZATION_ENUM_DECLARE(ESmallType, :uint8, ...)  // size is optional
// To force separate "name" and "label", e.g. for compatibility with serialized files:
//		SERIALIZATION_ENUM_DECLARE(EType,,
//			Mercury, Venus,
//			_Gaia, Earth = _Gaia,    // creates label Earth, name Gaia
//			Mars))

///////////////////////////////////////////////////////////////////////
// Implementation
namespace Serialization
{
using yasli::EnumDescription;
using yasli::getEnumDescription;

template<class Enum>
bool SerializeEnum(yasli::Archive &ar, Enum &value, const char* name, const char* label)
{
	int  iValue = check_cast<int>(value);
	bool ret    = getEnumDescription<Enum>().serialize(ar, iValue, name, label);
	if (ret && ar.isInput())
		value = check_cast<Enum>(iValue);
	return ret;
}

// Helper object for tracking enum names and values
struct EnumInit
{
	EnumInit(EnumDescription &desc, char* names)
	{
		context().Init(desc, names);
	}
	EnumInit()
	{
		AddElem(context().nextValue);
	}
	EnumInit(EnumInit const &in)
	{
		AddElem(in.value);
	}
	EnumInit(int v)
	{
		AddElem(v);
	}

	private:

	int    value;
	cstr   name;
	string label;

	void AddElem(int val)
	{
		value = val;
		name  = context().ParseNextEnum();
		if (*name == '_')
		{
			// Alias name, register without label
			name++;
			context().enumDesc->add(value, name, nullptr);
		}
		else
		{
			string labelStr = NameToLabel(name);
			if (labelStr == name)
			{
				context().enumDesc->add(value, name, name);
			}
			else
			{
				label = labelStr;
				context().enumDesc->add(value, name, label);
			}
		}
		context().nextValue = value + 1;
	}

	struct Context
	{
		EnumDescription* enumDesc;
		char*            enumStr;
		int              nextValue;

		void Init(EnumDescription &desc, char* names)
		{
			enumDesc  = &desc;
			enumStr   = names;
			nextValue = 0;
		}

		cstr ParseNextEnum()
		{
			char* s = enumStr;
			while (isspace(*s)) s++;
			if (!*s)
				return 0;

			cstr se = s;
			while (isalnum(*s) || *s == '_') s++;

			if (*s == ',')
			{
				* s++ = 0;
			}
			else if (*s)
			{
				* s++ = 0;
				while (*s && *s != ',') s++;
				if (*s) s++;
			}
			enumStr = s;
			return se;
		}
	};

	static Context& context()
	{
		static Context s;
		return s;
	}
};
}


///////////////////////////////////////////////////////////////////////
// Declare enums and serialization together, with scoped names

#define SERIALIZATION_ENUM_DECLARE(Type, sizespec, ...) \
	enum class Type                  sizespec { __VA_ARGS__ }; \
	struct SerializationRegister_ ## Type { \
		bool Register() { \
			static char enum_str[] = #__VA_ARGS__; \
			static Serialization::EnumInit init(yasli::EnumDescriptionImpl<Type>::the(), enum_str), __VA_ARGS__; \
			return true; \
		} \
		SerializationRegister_ ## Type() { static bool b = Register(); } \
	}; \
	inline bool Serialize(yasli::Archive & ar, Type & value, const char* name, const char* label) { \
		return Serialization::SerializeEnum(ar, value, name, label); \
	} \

// Implement the enum registration. Place in a single compilation file
#define SERIALIZATION_ENUM_IMPLEMENT(Type) \
	SerializationRegister_ ## Type doSerializationRegister_ ## Type; \

// Define and implement an enum and registration within a class, or globally in a file
#define SERIALIZATION_ENUM_DEFINE(Type, sizespec, ...) \
	SERIALIZATION_ENUM_DECLARE(Type, sizespec, __VA_ARGS__) \
	SERIALIZATION_ENUM_IMPLEMENT(Type) \

// Define and implement an enum and registration statically in a file
#define SERIALIZATION_ENUM_DEFINE_STATIC(Type, sizespec, ...) \
	SERIALIZATION_ENUM_DECLARE(Type, sizespec, __VA_ARGS__) \
	static SERIALIZATION_ENUM_IMPLEMENT(Type) \

// Legacy
#define SERIALIZATION_DECLARE_ENUM(Type, ...) \
	SERIALIZATION_ENUM_DEFINE(Type,, __VA_ARGS__) \

