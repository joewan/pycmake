// Copyright 2001-2016 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

// dependencies: uint32

//! Should be simple to integrate into every project (SRF Processing, CryRenderer) so dependencies are kept minimal
//! radius is limited
//! use like this:
//! dil.SetDilationRadius(dwR);
//! for(uint32 dwY=0;dwY<m_dwWidth;++dwY)
//! for(uint32 dwX=0;dwY<m_dwHeight;++dwX)
//!   { uint32 dwLX,dwLY; if(dil.GetBestSampleBorder(dwX,dwY,dwLX,dwLY) {} }
class CBitmapDilation
{
	public:

	//! Image is created with all bits set invalid.
	CBitmapDilation( const uint32 dwWidth, const uint32 dwHeight ) : m_dwWidth(dwWidth), m_dwHeight(dwHeight)
	{
		assert(dwWidth);
		assert(dwHeight);

		SetDilationRadius(9);                          //!< Set m_dw.

		m_Mask.resize((dwWidth * dwHeight + 7) / 8,0); //!< Allocate at least dwWidth*dwHeight bits and clear to invalid.

	}

	void SetValid( const uint32 dwX, const uint32 dwY )
	{
		assert(dwX < m_dwWidth);
		assert(dwY < m_dwHeight);

		uint32 dwGlobalBitNo  = dwX + dwY * m_dwWidth;
		uint32 dwGlobalByteNo = dwGlobalBitNo >> 3;
		uint32 dwLocalBitNo   = dwGlobalBitNo & 0x7;

		m_Mask[dwGlobalByteNo] |= 1 << dwLocalBitNo;
	}

	//! fast
	//! \param dwDist - 0..9, 0 means no dilation
	void SetDilationRadius( const uint32 dwDist )
	{
		switch (dwDist)
		{
		case 0: m_dwNoEnd = 1 + 0;
			return;
		case 1: m_dwNoEnd = 1 + 4;
			return;
		case 2: m_dwNoEnd = 1 + 12;
			return;
		case 3: m_dwNoEnd = 1 + 36;
			return;
		case 4: m_dwNoEnd = 1 + 60;
			return;
		case 5: m_dwNoEnd = 1 + 88;
			return;
		case 6: m_dwNoEnd = 1 + 120;
			return;
		case 7: m_dwNoEnd = 1 + 176;
			return;
		case 8: m_dwNoEnd = 1 + 212;
			return;
		case 9: m_dwNoEnd = 1 + 292;
			return;

		default: assert(0);   // more needs to be done through multiple calls
		}
	}

	//! Call SetDilationRadius() before to set the radius.
	//! \param dwInX - 0..m_dwWidth-1
	//! \param dwInX - 0..m_dwHeight-1
	//! \param dwOutX output parameter, set to 0 if no valid sample was found
	//! \param dwOutY output parameter, set to 0 if no valid sample was found
	//! \return true if a valid border is found, false otherwise.
	bool GetBestSampleBorder( const uint32 dwInX, const uint32 dwInY, uint32 &dwOutX, uint32 &dwOutY ) const
	{
		uint32 dwSampleNo = 0;
		int    iLocalX,iLocalY;

		for (uint32 dwI = 0; dwI < m_dwNoEnd; ++dwI)
		{
			bool bOk = GetSpiralPoint(dwSampleNo++,iLocalX,iLocalY);
			assert(bOk);

			if (IsValidBorder(iLocalX + dwInX,iLocalY + dwInY))
			{
				dwOutX = (uint32)(iLocalX + dwInX);
				dwOutY = (uint32)(iLocalY + dwInY);
				return true;
			}
		}

		dwOutX = 0;
		dwOutY = 0;                  // nothing found
		return false;
	}

	//! Sample count for the set radius.
	uint32 GetSampleCountForSetRadius() const
	{
		return m_dwNoEnd;
	}

	private:

	uint32             m_dwWidth;  //!< >0
	uint32             m_dwHeight; //!< >0
	std::vector<uint8> m_Mask;     //!< bits define the valid areas on the image (1=valid, 0=invalid)
	uint32             m_dwNoEnd;

	//! Border is wrapped.
	inline bool IsValidTiled( const int iX, const int iY ) const
	{
		const uint32 dwX = (uint32)iX;
		const uint32 dwY = (uint32)iY;

		return _IsValid(dwX % m_dwWidth,dwY % m_dwHeight);
	}

	//! Border is treated invalid.
	inline bool IsValidBorder( const int iX, const int iY ) const
	{
		const uint32 dwX = (uint32)iX;
		const uint32 dwY = (uint32)iY;

		if (dwX >= m_dwWidth)
			return false;

		if (dwY >= m_dwHeight)
			return false;

		return _IsValid(dwX,dwY);
	}

	inline bool _IsValid( const uint32 dwX, const uint32 dwY ) const
	{
		assert(dwX < m_dwWidth);
		assert(dwY < m_dwHeight);

		uint32 dwGlobalBitNo  = dwX + dwY * m_dwWidth;
		uint32 dwGlobalByteNo = dwGlobalBitNo >> 3;
		uint32 dwLocalBitNo   = dwGlobalBitNo & 0x7;

		return (m_Mask[dwGlobalByteNo] & (1 << dwLocalBitNo)) != 0;
	}


	public:
	//! \return false = end reached, true otherwise
	inline bool GetSpiralPoint( const uint32 dwNo, int &iX, int &iY ) const
	{
		// x,y pairs in a circle around 0,0 with radius 9 sorted by distance (~500 bytes)
		// data was generated by code
		static const char SpiraleArray[] =
		{
			0,0, 0,1, 1,0, 0,-1, -1,0, 1,1, 1,-1, -1,-1, -1,1, 0,2,
			2,0, 0,-2, -2,0, -1,2, 1,2, 2,1, 2,-1, 1,-2, -1,-2, -2,-1,
			-2,1, 2,-2, -2,-2, -2,2, 2,2, 0,3, 3,0, 0,-3, -1,-3, -3,-1,
			-3,0, -3,1, -1,3, 1,3, 3,1, 3,-1, 1,-3, 3,-2, 2,-3, -2,-3,
			-3,-2, -3,2, -2,3, 2,3, 3,2, 0,4, 4,0, 3,-3, 1,-4, 0,-4,
			-1,-4, -3,-3, -4,-1, -4,0, -4,1, -3,3, -1,4, 1,4, 3,3, 4,1,
			4,-1, 4,2, 4,-2, 2,-4, -2,-4, -4,-2, -4,2, -2,4, 2,4, 3,4,
			4,3, 4,-3, 3,-4, -3,-4, -4,-3, -4,3, -3,4, 0,5, 5,0, 1,-5,
			0,-5, -1,-5, -5,-1, -5,0, -5,1, -1,5, 1,5, 5,1, 5,-1, 2,-5,
			-2,-5, -5,-2, -5,2, -2,5, 2,5, 5,2, 5,-2, -4,-4, -4,4, 4,4,
			4,-4, -3,-5, -5,-3, -5,3, -3,5, 3,5, 5,3, 5,-3, 3,-5, 0,6,
			6,1, 6,0, 6,-1, 1,-6, 0,-6, -1,-6, -6,-1, -6,0, -6,1, -1,6,
			1,6, 2,6, 4,5, 5,4, 6,2, 6,-2, 5,-4, 4,-5, 2,-6, -2,-6,
			-4,-5, -5,-4, -6,-2, -6,2, -5,4, -4,5, -2,6, 3,-6, -3,-6, -6,-3,
			-6,3, -3,6, 3,6, 6,3, 6,-3, -5,5, 5,5, 5,-5, -5,-5, 0,7,
			7,1, 7,0, 7,-1, 6,-4, 4,-6, 1,-7, 0,-7, -1,-7, -4,-6, -6,-4,
			-7,-1, -7,0, -7,1, -6,4, -4,6, -1,7, 1,7, 4,6, 6,4, -7,2,
			-2,7, 2,7, 7,2, 7,-2, 2,-7, -2,-7, -7,-2, -7,-3, -7,3, -3,7,
			3,7, 7,3, 7,-3, 3,-7, -3,-7, 5,6, 6,5, 6,-5, 5,-6, -5,-6,
			-6,-5, -6,5, -5,6, -4,7, 4,7, 7,4, 7,-4, 4,-7, -4,-7, -7,-4,
			-7,4, 0,8, 8,1, 8,0, 8,-1, 1,-8, 0,-8, -1,-8, -8,-1, -8,0,
			-8,1, -1,8, 1,8, -2,-8, -8,-2, -8,2, -2,8, 2,8, 8,2, 8,-2,
			2,-8, -6,6, 6,6, 6,-6, -6,-6, 5,7, 7,5, 8,3, 8,-3, 7,-5,
			5,-7, 3,-8, -3,-8, -5,-7, -7,-5, -8,-3, -8,3, -7,5, -5,7, -3,8,
			3,8, -8,4, -4,8, 4,8, 8,4, 8,-4, 4,-8, -4,-8, -8,-4, 0,9,
			9,1, 9,0, 9,-1, 7,-6, 6,-7, 1,-9, 0,-9, -1,-9, -6,-7, -7,-6,
			-9,-1, -9,0, -9,1, -7,6, -6,7, -1,9, 1,9, 6,7, 7,6, 2,-9,
			-2,-9, -9,-2, -9,2, -2,9, 2,9, 9,2, 9,-2, -5,-8, -8,-5, -8,5,
			-5,8, 5,8, 8,5, 8,-5, 5,-8, -9,-3, -9,3, -3,9, 3,9, 9,3,
			9,-3, 3,-9, -3,-9
		};

		if (dwNo >= sizeof(SpiraleArray) / 2)
			return false;

		uint32 dwEl = dwNo * 2;

		iX = (int)SpiraleArray[dwEl];
		iY = (int)SpiraleArray[dwEl + 1];
		return true;
	}
};
