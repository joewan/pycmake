// Copyright 2001-2016 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include "FlowBaseNode.h"
#include <ISystem.h>
#include <IEntityProxy.h>
#include <IDynamicResponseSystem.h>

class CFlowNode_SendDynamicResponseSignal:public CFlowBaseNode<eNCT_Instanced>
{
	public:

	explicit CFlowNode_SendDynamicResponseSignal(SActivationInfo* pActInfo)
	{}

//////////////////////////////////////////////////////////////////////////
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo)
	{
		return new CFlowNode_SendDynamicResponseSignal(pActInfo);
	}

//////////////////////////////////////////////////////////////////////////
	enum EInputs
	{
		eIn_Send,
		eIn_SignalName,
		eIn_StringContextVariableName,
		eIn_StringContextVariableValue,
		eIn_IntContextVariableName,
		eIn_IntContextVariableValue,
	};

	enum EOutputs
	{
		eOut_SignalSent,
		//TODO: eOut_Done
	};

	virtual void GetConfiguration(SFlowNodeConfig &config)
	{
		static const SInputPortConfig inputs[] =
		{
			InputPortConfig_Void("Send", _HELP("send dynamic response signal name")),
			InputPortConfig<string>("SignalName", _HELP("dynamic response signal name"), "Name"),
			InputPortConfig<string>("StringContextVariableName", _HELP("the name of a string variable that is send along with this signal and can therefore be used in the responses")),
			InputPortConfig<string>("StringContextVariableValue", _HELP("a string value to send along with this signal")),
			InputPortConfig<string>("IntContextVariableName", _HELP("the name of a numeric variable that is send along with this signal and can therefore be used in the responses")),
			InputPortConfig<int>("IntContextVariableValue", _HELP("a numeric value to send along with this signal")),
			{ 0 }
		};

		static const SOutputPortConfig outputs[] =
		{
			OutputPortConfig<string>("Sent", _HELP("Will be triggered when the signal has been sent")),
			{ 0 }
		};

		config.pInputPorts  = inputs;
		config.pOutputPorts = outputs;
		config.sDescription = _HELP("This node sends a signal to the Dynamic Response System.");
		config.nFlags      |= EFLN_TARGET_ENTITY;
		config.SetCategory(EFLN_APPROVED);
	}

//////////////////////////////////////////////////////////////////////////
	virtual void Serialize(SActivationInfo* pActInfo, TSerialize ser)
	{}

//////////////////////////////////////////////////////////////////////////
	virtual void ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
	{
		switch (event)
		{
		case eFE_Initialize:
		{
			break;
		}
		case eFE_Activate:
		{
			if (IsPortActive(pActInfo, eIn_Send))
			{
				if (pActInfo->pEntity)
				{
					const string &                       signalName       = GetPortString(pActInfo, eIn_SignalName);
					IEntityDynamicResponseProxyPtr const pIEntityDrsProxy = crycomponent_cast<IEntityDynamicResponseProxyPtr>(pActInfo->pEntity->CreateProxy(ENTITY_PROXY_DYNAMICRESPONSE));

					SET_DRS_USER_SCOPED("SendDrsSignal FlowGraph");

					const string &stringVariableName = GetPortString(pActInfo, eIn_StringContextVariableName);
					const string &intVariableName    = GetPortString(pActInfo, eIn_IntContextVariableName);

					DRS::IVariableCollectionSharedPtr pContextVariableCollection = nullptr;
					if (!stringVariableName.empty())
					{
						pContextVariableCollection = gEnv->pDynamicResponseSystem->CreateContextCollection();
						const string &stringVariableValue = GetPortString(pActInfo, eIn_StringContextVariableValue);
						pContextVariableCollection->SetVariableValue(stringVariableName, CHashedString(stringVariableValue));
					}
					if (!intVariableName.empty())
					{
						if (!pContextVariableCollection)
						{
							pContextVariableCollection = gEnv->pDynamicResponseSystem->CreateContextCollection();
						}
						const int intVariableValue = GetPortInt(pActInfo, eIn_IntContextVariableValue);
						pContextVariableCollection->SetVariableValue(intVariableName.c_str(), intVariableValue);
					}
					pIEntityDrsProxy->QueueSignal(signalName, pContextVariableCollection);
					ActivateOutput(pActInfo, eOut_SignalSent, true);
				}
				else
				{
					CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_WARNING, "CAction:DRS_SendSignal: Cannot send a signal without an specified EntityID.");
				}
			}
			break;
		}
		}
	}

//////////////////////////////////////////////////////////////////////////
	virtual void GetMemoryUsage(ICrySizer* s) const
	{
		s->Add(*this);
	}

	private:
//////////////////////////////////////////////////////////////////////////
};

class CFlowNode_CancelDynamicResponseSignal:public CFlowBaseNode<eNCT_Instanced>
{
	public:
	explicit CFlowNode_CancelDynamicResponseSignal(SActivationInfo* pActInfo)
	{}

	//////////////////////////////////////////////////////////////////////////
	virtual IFlowNodePtr Clone(SActivationInfo* pActInfo)
	{
		return new CFlowNode_CancelDynamicResponseSignal(pActInfo);
	}

	//////////////////////////////////////////////////////////////////////////
	enum EInputs
	{
		eIn_Cancel,
		eIn_SignalName,
	};

	enum EOutputs
	{
		eOut_CancelRequestSent,
	};

	virtual void GetConfiguration(SFlowNodeConfig &config)
	{
		static const SInputPortConfig inputs[] =
		{
			InputPortConfig_Void("Cancel", _HELP("cancel the processing of the dynamic response signal")),
			InputPortConfig<string>("SignalName", _HELP("the name of the dynamic response signal name to cancel"), "Name"),
			{ 0 }
		};

		static const SOutputPortConfig outputs[] =
		{
			OutputPortConfig<string>("Canceled", _HELP("Will be triggered when the cancel-request for the signal has been sent")),
			{ 0 }
		};

		config.pInputPorts  = inputs;
		config.pOutputPorts = outputs;
		config.sDescription = _HELP("This node sends a signal to the Dynamic Response System.");
		config.nFlags      |= EFLN_TARGET_ENTITY;
		config.SetCategory(EFLN_APPROVED);
	}

	//////////////////////////////////////////////////////////////////////////
	virtual void Serialize(SActivationInfo* pActInfo, TSerialize ser)
	{}

	//////////////////////////////////////////////////////////////////////////
	virtual void ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
	{
		switch (event)
		{
		case eFE_Initialize:
		{
			break;
		}
		case eFE_Activate:
		{
			if (IsPortActive(pActInfo, eIn_Cancel))
			{
				const string &signalName = GetPortString(pActInfo, eIn_SignalName);
				if (pActInfo->pEntity)
				{
					IEntityDynamicResponseProxyPtr const pIEntityDrsProxy = crycomponent_cast<IEntityDynamicResponseProxyPtr>(pActInfo->pEntity->CreateProxy(ENTITY_PROXY_DYNAMICRESPONSE));
					gEnv->pDynamicResponseSystem->CancelSignalProcessing(signalName, pIEntityDrsProxy->GetResponseActor());
				}
				else
				{
					gEnv->pDynamicResponseSystem->CancelSignalProcessing(signalName);
				}
				ActivateOutput(pActInfo, eOut_CancelRequestSent, true);
			}
			break;
		}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	virtual void GetMemoryUsage(ICrySizer* s) const
	{
		s->Add(*this);
	}

	private:
	//////////////////////////////////////////////////////////////////////////
};

#if !defined(IS_EAAS)
REGISTER_FLOW_NODE("DynamicResponse:SendSignal", CFlowNode_SendDynamicResponseSignal);
REGISTER_FLOW_NODE("DynamicResponse:CancelSignalProcessing", CFlowNode_CancelDynamicResponseSignal);
#endif
