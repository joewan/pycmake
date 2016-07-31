// Copyright 2001-2016 Crytek GmbH / Crytek Group. All rights reserved.

#ifndef _IRENDERPIPELINE_H_
#define _IRENDERPIPELINE_H_

#if _MSC_VER > 1000
	#pragma once
#endif // _MSC_VER > 1000

struct SEngineNodeState
{
	IRenderMesh* m_pMesh;
	Matrix34     m_Matrix;
	uint32       m_nObjFlags;
	IRenderNode* m_pRenderNode;
	IMaterial*   m_pMaterial;
	uint32       m_nMaterialLayers;
	int16        m_nTextureID;
};

struct IRenderState
{
	uint32         m_bWasDeleted : 1;
	CRenderObject* m_pInstance;
	IRenderNode*   m_pRenderNode;
	IRenderMesh*   m_pMesh;
	IMaterial*     m_pMaterial;
	uint32         m_nCurLod;
	int            m_nMeshChunk;
	uint32         m_nMaterialLayers;

	CRendElement* m_pRE;
	SShaderItem   m_ShaderItem;

	IRenderState()
	{
		m_bWasDeleted = false;

		m_pInstance       = NULL;
		m_pRenderNode     = NULL;
		m_pMesh           = NULL;
		m_pRE             = NULL;
		m_nMeshChunk      = -1;
		m_nCurLod         = 0;
		m_nMaterialLayers = -1;
	}
};

// Summary:
//   DX12 style render pipeline interface.
// Description:
struct IRenderPipeline
{
	// <interfuscator:shuffle>
	virtual ~IRenderPipeline(){}


	virtual IRenderState* GetRenderState(SEngineNodeState &InpState, uint32 nFlags)                                                       = 0;
	virtual void          AddRenderState(IRenderState* pRS, const SRenderingPassInfo &passInfo, int nAW = 1, int nList = EFSLIST_GENERAL) = 0;
	virtual void          RT_CreateGPUStates(IRenderState* pRootState)                                                                    = 0;
	virtual void          RT_ReleaseGPUStates(IRenderState* pRootState)                                                                   = 0;

	// Summary:
	//	 Flushes elements stored on the buffer to the screen.
	virtual void Flush() = 0;
	// </interfuscator:shuffle>
};



#endif // #ifndef _IRENDERPIPELINE_H_
