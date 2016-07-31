// Copyright 2001-2016 Crytek GmbH / Crytek Group. All rights reserved.

#ifndef NULL_INPUT_H
#define NULL_INPUT_H

#pragma once
#include <IInput.h>

class CNullInput:public IInput
{
	public:
	virtual void AddEventListener( IInputEventListener* pListener )    {}
	virtual void RemoveEventListener( IInputEventListener* pListener ) {}

	virtual void AddConsoleEventListener( IInputEventListener* pListener )    {}
	virtual void RemoveConsoleEventListener( IInputEventListener* pListener ) {}

	virtual bool AddTouchEventListener( ITouchEventListener* pListener, const char* name ) {return false; }
	virtual void RemoveTouchEventListener( ITouchEventListener* pListener )                {}

	virtual void                 SetExclusiveListener( IInputEventListener* pListener ) {}
	virtual IInputEventListener* GetExclusiveListener()                                 { return NULL; }

	virtual bool AddInputDevice(IInputDevice* pDevice) { return false; }

	virtual void EnableEventPosting ( bool bEnable )                                 {}
	virtual bool IsEventPostingEnabled() const                                       { return false; }
	virtual void PostInputEvent( const SInputEvent &event, bool bForce = false )     {}
	virtual void PostTouchEvent( const STouchEvent &event, bool bForce = false )     {}
	virtual void PostUnicodeEvent( const SUnicodeEvent &event, bool bForce = false ) {}

	virtual void ForceFeedbackEvent( const SFFOutputEvent &event ) {}
	virtual void ForceFeedbackSetDeviceIndex( int index )          {}

	virtual bool Init()              { return true; }
	virtual void PostInit()          {}
	virtual void Update(bool bFocus) {}
	virtual void ShutDown()          {}

	virtual void SetExclusiveMode(EInputDeviceType deviceType, bool exclusive,void* hwnd = 0) {}

	virtual bool InputState(const TKeyName &key, EInputState state) { return false; }

	virtual const char* GetKeyName(const SInputEvent &event) const { return NULL; }

	virtual const char* GetKeyName(EKeyId keyId) const { return NULL; }

	virtual char GetInputCharAscii(const SInputEvent &event) { return 0; }

	virtual SInputSymbol* LookupSymbol( EInputDeviceType deviceType, int deviceIndex, EKeyId keyId ) { return NULL; }

	virtual const SInputSymbol* GetSymbolByName(const char* name) const { return NULL; }

	virtual const char* GetOSKeyName(const SInputEvent &event) { return NULL; }

	virtual void ClearKeyState() {}

	virtual void ClearAnalogKeyState() {}

	virtual void RetriggerKeyState() {}

	virtual bool Retriggering() { return false; }

	virtual bool HasInputDeviceOfType( EInputDeviceType type ) { return false; }

	virtual int GetModifiers() const { return 0; }

	virtual void EnableDevice( EInputDeviceType deviceType, bool enable) {}

	virtual void SetDeadZone(float fThreshold) {}

	virtual void RestoreDefaultDeadZone() {}

	virtual IInputDevice* GetDevice(uint16 id, EInputDeviceType deviceType) { return NULL; }

	virtual uint32 GetPlatformFlags() const { return 0; }

	virtual IKinectInput*       GetKinectInput()       {return NULL; }
	virtual INaturalPointInput* GetNaturalPointInput() {return NULL; }
	// Input blocking functionality
	virtual bool SetBlockingInput(const SInputBlockData &inputBlockData)       { return false; }
	virtual bool RemoveBlockingInput(const SInputBlockData &inputBlockData)    { return false; }
	virtual bool HasBlockingInput(const SInputBlockData &inputBlockData) const { return false; }
	virtual int  GetNumBlockingInputs() const                                  { return 0; }
	virtual void ClearBlockingInputs()                                         {}
	virtual bool ShouldBlockInputEventPosting(const EKeyId           keyId,
	                                          const EInputDeviceType deviceType,
	                                          const uint8            deviceIndex) const { return false; }

	virtual void ProcessKey(uint32 key, bool pressed, WCHAR unicode, bool repeat) {}
	virtual bool GrabInput(bool bGrab)                                            { return false; }
};

#endif
