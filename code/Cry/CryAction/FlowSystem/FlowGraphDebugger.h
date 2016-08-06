// Copyright 2001-2016 Crytek GmbH / Crytek Group. All rights reserved.

// -------------------------------------------------------------------------
//  File name:   FlowGraphDebugger.h
//  Version:     v1.00
//  Created:     13/09/2011 by Sascha Hoba.
//  Description:
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#if defined(INCLUDE_FLOWGRAPHDEBUGGER_EXTENSION)

#ifndef __FLOWGRAPHDEBUGGER_H__
	#define __FLOWGRAPHDEBUGGER_H__

	#include "CryListenerSet.h"
	#include <IFlowGraphDebugger.h>
	#include <CryExtension/Impl/ClassWeaver.h>


class CFlowGraphDebugger:public IFlowGraphDebugger
{
	CRYINTERFACE_SIMPLE(IFlowGraphDebugger)
	CRYGENERATE_SINGLETONCLASS(CFlowGraphDebugger, IFlowGraphDebuggerExtensionName, 0x2D6810E10CBD467A, 0xB5646539C3778285)

	public:

	// IFlowGraphDebugger
	virtual bool               AddBreakpoint(IFlowGraphPtr pFlowgraph, const SFlowAddress &addr) override;
	virtual bool               RemoveBreakpoint(IFlowGraphPtr pFlowgraph, const SFlowAddress &addr) override;
	virtual bool               RemoveAllBreakpointsForNode(IFlowGraphPtr pFlowGraph, TFlowNodeId nodeID) override;
	virtual bool               RemoveAllBreakpointsForGraph(IFlowGraphPtr pFlowgraph) override;
	virtual bool               HasBreakpoint(IFlowGraphPtr pFlowGraph, const SFlowAddress &addr) const override;
	virtual bool               HasBreakpoint(IFlowGraphPtr pFlowGraph, TFlowNodeId nodeID) const override;
	virtual bool               HasBreakpoint(IFlowGraphPtr pFlowGraph) const override;
	virtual bool               EnableBreakpoint(IFlowGraphPtr pFlowgraph, const SFlowAddress &addr, bool enable) override;
	virtual bool               EnableTracepoint(IFlowGraphPtr pFlowgraph, const SFlowAddress &addr, bool enable) override;
	virtual bool               IsBreakpointEnabled(IFlowGraphPtr pFlowgraph, const SFlowAddress &addr) const override;
	virtual bool               IsTracepoint(IFlowGraphPtr pFlowgraph, const SFlowAddress &addr) const override;
	virtual const SBreakPoint& GetBreakpoint() const override {return m_BreakPoint; }
	virtual void               InvalidateBreakpoint() override;
	virtual bool               BreakpointHit() const override;
	virtual bool               RegisterListener(IFlowGraphDebugListener* pListener, const char* name) override;
	virtual void               UnregisterListener(IFlowGraphDebugListener* pListener) override;
	virtual bool               PerformActivation(IFlowGraphPtr pFlowgraph, const SFlowAddress &addr, const TFlowInputData &value) override;
	virtual bool               PerformActivation(IFlowGraphPtr pFlowgraph, int edgeIndex, const SFlowAddress &fromAddr, const SFlowAddress &toAddr, const TFlowInputData &value) override;
	virtual bool               RePerformActivation() override;
	virtual void               ClearBreakpoints() override;
	virtual bool               GetBreakpoints(DynArray<SBreakPoint> &breakpointsDynArray) override;
	virtual IFlowGraphPtr      GetRootGraph(IFlowGraphPtr pFlowGraph) const override;
	virtual void               EnableStepMode(bool bEnableStepMode) override {m_bStepModeEnabled = bEnableStepMode; }
	virtual bool               IsStepModeEnabled() override                  {return m_bStepModeEnabled; }
	virtual bool               AddIgnoredFlowgraph(IFlowGraphPtr pFlowgraph) override;
	virtual bool               RemoveIgnoredFlowgraph(IFlowGraphPtr pFlowgraph) override;
	virtual bool               IsFlowgraphIgnored(IFlowGraphPtr pFlowgraph) override;
	virtual bool               IgnoreFlowgraphType(IFlowGraph::EFlowGraphType type, bool bIgnore) override;
	virtual bool               IsFlowgraphTypeIgnored(IFlowGraph::EFlowGraphType type) override;
	// ~IFlowgraphDebugger

	private:

	bool CheckForValidIDs(TFlowNodeId nodeID, TFlowPortId portID) const;
	bool CheckForDelayedActivations();
	bool TryActivatePort(SBreakPoint &breakpoint, IFlowEdgeIterator::Edge &edge, bool &bPerform, int edgeIndex);

	void FillDynArray(DynArray<SBreakPoint> &breakpointsDynArray, TFlowPortIDS portIDS, IFlowGraphPtr pFlowgraph, TFlowNodeId nodeID, bool bIsOutput);

	const SBreakPointPortInfo* GetBreakpointInfo(IFlowGraphPtr pFlowgraph, const SFlowAddress &addr) const;
	SBreakPointPortInfo*       GetBreakpointInfo(IFlowGraphPtr pFlowgraph, const SFlowAddress &addr);

	TDebugInfo  m_DebugInfo;
	bool        m_BreakpointHit;
	bool        m_RePerformActivation;
	bool        m_bStepModeEnabled;
	SBreakPoint m_BreakPoint;

	typedef std::deque<SBreakPoint> TDelayedActivations;
	TDelayedActivations m_DelayedActivations;

	typedef std::vector<IFlowGraphPtr> TIgnoredFlowgraphs;
	TIgnoredFlowgraphs m_IgnoredFlowgraphs;

	typedef std::vector<IFlowGraph::EFlowGraphType> TIgnoredFlowgraphTypes;
	TIgnoredFlowgraphTypes m_IgnoredFlowgraphTypes;

	CListenerSet<IFlowGraphDebugListener*> m_Listeners;
};

#endif
#endif