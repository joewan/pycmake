// Copyright 2001-2016 Crytek GmbH / Crytek Group. All rights reserved.

#ifndef GAME_ISTREAMPERSIST_H
#define GAME_ISTREAMPERSIST_H
#if _MSC_VER > 1000
	#pragma once
#endif

struct IScriptObject;
class CStream;

enum DirtyFlags
{
	DIRTY_NAME   = 0x1,
	DIRTY_POS    = 0x2,
	DIRTY_ANGLES = 0x4,
};

//! Store the stream status per connection(per serverslot).
/*struct StreamStatus
   {
   StreamStatus()
   {
    nUserFlags=0;
    nLastUpdate=0;
    nUpdateNumber=0;
   }
   unsigned int nUserFlags;
   unsigned int nLastUpdate;
   unsigned int nUpdateNumber;
   };*/


//! This interface must be implemented by all objects that must be serialized through the network or file.
//! The main purpose of the serialization is reproduce the game remotely or saving and restoring.
//! This mean that the object must not save everything but only what really need to be restored correctly.
struct IStreamPersist
{
	// <interfuscator:shuffle>
	//! Serialize the object to a bitstream(network).
	//! \param stm Stream class that will store the bitstream.
	//! \return true on success, false on failure.
	virtual bool Write(CStream &) = 0;

	//! Reads the object from a stream(network).
	//! \param stm Stream class that store the bitstream.
	//! \return true on success, false on failure.
	virtual bool Read(CStream &) = 0;

	//! Checks if the object must be synchronized since the last serialization.
	//! \return true must be serialized, false the object didn't change.
	virtual bool IsDirty() = 0;

	//! Serializes the object to a bitstream(file persistence).
	//! \param stm Stream class that will store the bitstream.
	//! \return true on success, false on failure.
	virtual bool Save(CStream &stm) = 0;

	//! Reads the object from a stream(file persistence).
	//! \param stm The stream class that store the bitstream.
	//! \param pStream Script wrapper for the stream(optional).
	//! \return true on success, false on failure.
	virtual bool Load(CStream &stm,IScriptObject* pStream = NULL) = 0;
	// </interfuscator:shuffle>
};

#endif // GAME_ISTREAMPERSIST_H
