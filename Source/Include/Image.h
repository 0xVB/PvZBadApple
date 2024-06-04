#pragma once
#include "Definitions.h"
struct BColor
{
	BYTE B;
	BYTE G;
	BYTE R;
	BYTE A;

	BColor(byte a, byte r, byte g, byte b)
	{
		A = a;
		R = r;
		G = g;
		B = b;
	}

	BColor(BColor Original, byte a)
	{
		A = a;
		R = Original.R;
		G = Original.G;
		B = Original.B;
	}

	BColor(BColor C0, BColor C1, float a)
	{
		float b = 1 - a;
		A = b * C0.A + C1.A * a;
		R = b * C0.R + C1.R * a;
		G = b * C0.G + C1.G * a;
		B = b * C0.B + C1.B * a;
	}

	BYTE Brightness()
	{
		return (R + G + B) / 3;
	}
};

class Sexy::Image
{
public:
	bool Drawn;
	PopString FilePath;
	IVector2 Size;
	int RowCount;
	int ColCount;
	AnimInfo* AnimInfo;

	virtual ~Image();// 00
	virtual bool PolyFill3D(IVector2* Vertices, int VertexCount, IRect* ClipRect, Color* Color, int DrawMode, int Tx, int Ty, bool Convex);// 04
	virtual void FillRect(IRect* Rect, Color* Color, int DrawMode);// 08
	virtual void DrawRect(IRect* Rect, Color* Color, int DrawMode);// 0C
	virtual void ClearRect(IRect* Rect);// 10
	virtual void DrawLine(double Sx, double Sy, double Ex, double Ey, Color* Color, int DrawMode);// 14
	virtual void DrawLineAA(double Sx, double Sy, double Ex, double Ey, Color* Color, int DrawMode);// 18
	virtual void FillScanLines(Span* Spans, int SpanCount, Color* Color, int DrawMode);// 1C
	virtual void FillScanLinesWithCoverage(Span* Spans, int SpanCount, Color* Color, int DrawMode, char* Coverage, int Cx, int Cy, int Cw, int Ch);// 20
	virtual void DrawImage(Image* Other, int X, int Y, IRect* Source, Color* Color, int DrawMode);// 24
};

class Sexy::MemoryImage : public Sexy::Image
{
public:
	BColor* Bits;
	int BitsChangedCount;
	void* D3DData;
	unsigned long D3DFlags;
	unsigned long* ColorTable;
	unsigned char* ColorIndices;
	bool ForcedMode;
	bool HasTrans;
	bool HasAlpha;
	bool IsVolatile;
	bool PurgeBits;
	bool WantPal;
	unsigned long* NativeAlphaData;
	unsigned char* RLAlphaData;
	unsigned char* RLAdditiveData;
	bool BitsChanged;
	SexyAppBase* App;

	BColor GetPixel(int X, int Y)
	{
		X = min(X, Size.X);
		Y = min(Y, Size.Y);
		int Offset = Size.Y * X + Y;
		return Bits[Offset];
	}
	BColor GetPixel(int I)
	{
		return Bits[I];
	}
	void SetPixel(int X, int Y, BColor Col)
	{
		X = min(X, Size.X);
		Y = min(Y, Size.Y);
		int Offset = Size.Y * X + Y;
		Bits[Offset] = Col;
	}
	void SetPixel(int I, BColor Col)
	{
		Bits[I] = Col;
	}
};

class Sexy::DDImage : public Sexy::MemoryImage
{
private:
	static const DWORD DESTRUCTOR = 0x56B980;

public:
	DDInterface* DDInterface;
	IDirectDrawSurface* Surface;
	bool SurfaceSet;
	bool NoLock;
	bool VideoMemory;
	bool FirstPixelTrans;
	bool WantDDInterface;
	bool DrawToBits;
	int LockCount;
	_DDSURFACEDESC LockedSurfaceDesc;
	virtual ~DDImage();
};