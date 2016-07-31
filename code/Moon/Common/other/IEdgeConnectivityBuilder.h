// Copyright 2001-2016 Crytek GmbH / Crytek Group. All rights reserved.

// -------------------------------------------------------------------------
//  File name:   IEdgeVolumeBuilder.h
//  Version:     v0.60
//  Created:     08/27/2002 by Martin Mittring
//  Compilers:   Visual Studio.NET
//  Description:
// -------------------------------------------------------------------------
//  History:     interface extracted from Serg
//
////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef __IEDGE_CONNECTIVITYBUILDER_H
	#define __IEDGE_CONNECTIVITYBUILDER_H


class IStencilShadowConnectivity;
class CStencilShadowConnectivity;

//
//
class IStencilShadowConnectivity
{
	public:
	typedef unsigned short vindex;        //!< Vertex index (0..0xffff).

	// <interfuscator:shuffle>
	//! Don't forget to call Release for freeing the memory resources.
	virtual void Release( void ) = 0;

	//! Keeps the interface small and the access fast.
	//! \return Pointer to the internal memory representation (only used within module 3DEngine).
	const virtual CStencilShadowConnectivity* GetInternalRepresentation( void ) const = 0;

	//! For debugging and profiling.
	//! \param outVertexCount.
	//! \param outTriangleCount.
	virtual void GetStats( DWORD &outVertexCount, DWORD &outTriangleCount ) = 0;

	//! Memorize the vertex buffer in this connectivity object.
	//! This is needed if a static object doesn't need this info.
	virtual void SetVertices(const Vec3* pVertices, unsigned numVertices) = 0;

	//! Serializes this object to the given memory block.
	//! If it's NULL, only returns the number of bytes required.
	//! If it's not NULL, returns the number of bytes written.
	//! Returning 0 means error, insufficient space.
	virtual unsigned Serialize (bool bSave, void* pStream, unsigned nSize, IMiniLog* pWarningLog = NULL) = 0;

	//! Calculates the size of this object.
	virtual void GetMemoryUsage(ICrySizer* pSizer) = 0;

	//! If this returns true, then the meshing functions don't need external input of mesh information.
	//! This is for static objects only.
	virtual bool IsStandalone() const {return false; }

	//! \return Number of orphaned (open) edges.
	virtual unsigned numOrphanEdges() const = 0;
	// </interfuscator:shuffle>

	#if CRY_PLATFORM_WINDOWS
	//! \param szFilename Filename with path (or relative) and extension.
	//! \return true on success, false otherwise.
	virtual bool DebugConnectivityInfo( const char* szFilename ) = 0;
	#endif
};

// (don't copy the interface pointer and don't forget to call Release)
class IEdgeConnectivityBuilder
{
	public:
	typedef IStencilShadowConnectivity::vindex vindex;
	// <interfuscator:shuffle>
	virtual ~IEdgeConnectivityBuilder() {}

	//! Return to the state right after construction.
	virtual void Reinit( void ) = 0;

	virtual void SetWeldTolerance (float fTolerance) {}

	//! Reserves space for the given number of triangles that are to be added.
	virtual void ReserveForTriangles( unsigned innNumTriangles, unsigned innNumVertices ) = 0;

	//! Adds a single triangle to the mesh.
	//! The triangle is defined by three vertices, in counter-clockwise order.
	//! These vertex indices will be used later when accessing the array of
	//! deformed character/model vertices to determine the shadow volume boundary.
	//! \param nV0 Vertex index one, 0..0xffff.
	//! \param nV1 Vertex index two, 0..0xffff.
	//! \param nV2 Vertex index three, 0..0xffff.
	virtual void AddTriangle( vindex nV0, vindex nV1, vindex nV2 ) = 0;

	//! slower than AddTriangle but with the auto weld feature (if there are vertices with the same position your result is smaller and therefore faster).
	//! \param nV0 Vertex index one, 0..0xffff.
	//! \param nV1 Vertex index two, 0..0xffff.
	//! \param nV2 Vertex index three, 0..0xffff.
	//! \param vV0 Original vertex one position.
	//! \param vV1 Original vertex two position.
	//! \param vV2 Original vertex three position.
	virtual void AddTriangleWelded( vindex nV0, vindex nV1, vindex nV2, const Vec3 &vV0, const Vec3 &vV1, const Vec3 &vV2 ) = 0;

	//! Constructs/compiles the optimum representation of the connectivity to be used in run-time.
	//! \note Use Release method to dispose of the connectivity object.
	virtual IStencilShadowConnectivity* ConstructConnectivity( void ) = 0;

	//! \return Number of single (with no pair faces found) or orphaned edges.
	virtual unsigned numOrphanedEdges( void ) const = 0;

	//! Returns the list of faces for orphaned edges into the given buffer.
	//! For each orphaned edge, one face will be returned; some faces may be duplicated.
	virtual void getOrphanedEdgeFaces (unsigned* pBuffer) = 0;

	virtual void GetMemoryUsage(class ICrySizer* pSizer) {}
	// </interfuscator:shuffle>
};

// *****************************************************************

//! Used for runtime edge calculations.
class IEdgeDetector
{
	public:
	typedef unsigned short vindex;

	// <interfuscator:shuffle>
	virtual ~IEdgeDetector(){}
	// vertex index

	//! For deformable objects.
	//! \param pConnectivity Must not be 0 - don't use if there is nothing to do.
	//! \param invLightPos
	//! \param pDeformedVertices Must not be 0 - don't use if there is nothing to do.
	virtual void BuildSilhuetteFromPos( const IStencilShadowConnectivity* pConnectivity, const Vec3 &invLightPos, const Vec3* pDeformedVertices ) = 0;

	//! For undeformed objects.
	//! \param outiNumTriangles Must not be 0 - don't use if there is nothing to do.
	//! \return Pointer to the cleared (set to zero) bitfield where each bit represents the orientation of one triangle.
	virtual unsigned* getOrientationBitfield( int iniNumTriangles ) = 0;

	//! \param pConnectivity Must not be 0 - don't use if there is nothing to do.
	//! \param inpVertices Must not be 0 - don't use if there is nothing to do.
	virtual void BuildSilhuetteFromBitfield( const IStencilShadowConnectivity* pConnectivity, const Vec3* inpVertices ) = 0;

	//! Pointer to the triplets defining shadow faces.
	virtual const vindex* getShadowFaceIndices( unsigned &outCount ) const = 0;

	//! O(1), returns the pointer to an array of unsigned short pairs of vertex numbers.
	virtual const vindex* getShadowEdgeArray( unsigned &outiNumEdges ) const = 0;

	virtual unsigned numShadowVolumeVertices( void ) const = 0;

	//! Number of indices required to define the shadow volume.
	//! Use this to determine the size of index buffer passed to meshShadowVolume.
	//! \note This is always a dividend of 3.
	virtual unsigned numShadowVolumeIndices( void ) const = 0;

	//! Make up the shadow volume.
	//! Constructs the shadow volume mesh, and puts the mesh definition into the
	//! vertex buffer (vertices that define the mesh) and index buffer (triple
	//! integers defining the triangular faces, counterclockwise order).
	//! \param vLight
	//! \param fFactor
	//! \param outpVertexBuf Size of the vertex buffer must be at least numVertices().
	//! \param outpIndexBuf Size of the index buffer must be at least numIndices().
	virtual void meshShadowVolume (Vec3 vLight, float fFactor, Vec3* outpVertexBuf, unsigned short* outpIndexBuf) = 0;

	virtual void GetMemoryUsage(class ICrySizer* pSizer) {}
	// </interfuscator:shuffle>
};





#endif // __IEDGE_CONNECTIVITYBUILDER_H
