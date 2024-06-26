#pragma once
#include <corecrt_math_defines.h>
#include <Windows.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>

#pragma region PopStructs
struct PopWString
{
    DWORD Padding;
    std::wstring WString;

    PopWString(std::wstring Str) { Padding = 0; WString = Str; }
    operator const wchar_t* () { return WString.c_str(); }
    operator std::wstring() { return WString; }
};

struct PopString
{
    DWORD Padding;
    std::string String;

    PopString(const char* Char = "") { Padding = 0; String = Char; }
    PopString(std::string Str) { Padding = 0; String = Str; }
    operator const char* () { return String.c_str(); }
    operator std::string() { return String; }
    void operator=(const char* Char) { String = Char; }
    void operator=(std::string Str) { String = Str; }
};

struct PopFlags
{
    int FlagsAdded;
    int FlagsRemoved;
    PopFlags(int Add = 0, int Rem = 0) { FlagsAdded = Add; FlagsRemoved = Rem; }
};

struct PopModalFlags
{
    int OverFlags;
    int UnderFlags;
    bool IsOver;
    PopModalFlags(int Over = 0, int Under = 0, bool IsO = false) { OverFlags = Over, UnderFlags = Under; IsOver = IsO; }
};

template <typename T>
struct PopList
{
    DWORD Padding;
    std::list<T> List;

    PopList() { Padding = 0; List = std::list<T>(); }
    PopList(std::list<T> Lst) { Padding = 0; List = Lst; }
    operator std::list<T>() { return List; }
};

template <typename T>
struct PopSet
{
    DWORD Padding;
    std::set<T> Set;

    PopSet() { Padding = 0; Set = std::set<T>(); }
    PopSet(std::set<T> NSet) { Padding = 0; Set = NSet; }
    operator std::set<T>() { return Set; }
};

template <typename T>
struct PopVector
{
    DWORD Padding;
    std::vector<T> Vector;

    PopVector() { Padding = 0; Vector = std::vector<T>(); }
    PopVector(std::vector<T> Vec) { Padding = 0; Vector = Vec; }
    operator std::vector<T>() { return Vector; }
};

template <typename K, typename V>
struct PopMap
{
    DWORD Padding;
    std::map<K, V> Map;

    PopMap() { Padding = 0; Map = std::map<K, V>; }
    PopMap(std::map<K, V> Mp) { Padding = 0; Map = Mp; }
    operator std::map<K, V>() { return Map; }
};

template <typename K, typename V>
struct PopPair
{
    DWORD Padding;
    std::pair<K, V> Pair;

    PopPair() { Padding = 0; Pair = std::pair<K, V>(); }
    PopPair(std::pair<K, V> Pr) { Padding = 0; Pair = Pr; }
    operator std::pair<K, V>() { return Pair; }
};
#pragma endregion

#pragma region Struct
#define IVector2 Vector2<int>
#define IVector3 Vector3<int>
#define FVector2 Vector2<float>
#define FVector3 Vector3<float>
#define DVector2 Vector2<double>
#define DVector3 Vector3<double>
#define DRect Rect<double>
#define FRect Rect<float>
#define IRect Rect<int>

template <typename T>
struct Vector3;

template <typename T>
struct Vector2
{
    T X;
    T Y;

    Vector2(T x = 0, T y = 0) : X(x), Y(y) {};
    IVector2 ToInt() { return IVector2(X, Y); };
    FVector2 ToFloat() { return FVector2(X, Y); };
    DVector2 ToDouble() { return DVector2(X, Y); };
    operator IVector2() { return IVector2(X, Y); }
    operator FVector2() { return FVector2(X, Y); }
    operator DVector2() { return DVector2(X, Y); }
    operator IVector3();
    operator FVector3();
    operator DVector3();
    Vector3<T> To3D();

    double Magnitude() { return sqrt(X * X + Y * Y); }
    Vector2<T> Normal() { return Vector2<T>(Y, -X); }
    DVector2 Unit() { double M = Magnitude(); return DVector2(X / M, Y / M); }
    DVector2 DirectionTo(DVector2 T) { return (T - *this).Unit(); }
    double DistanceFrom(DVector2 Other) { return (Other - *this).Magnitude(); }
    void SetMagnitude(double NewMagnitude)
    {
        double F = NewMagnitude / Magnitude();
        X *= F;
        Y *= F;
    }
    void Lerp(DVector2 Other, double Alpha)
    {
        double RT = 1 - Alpha;
        X = (X * RT) + (Other.X * Alpha);
        Y = (Y * RT) + (Other.Y * Alpha);
    }
    void Nudge(DVector2 Other, double Alpha)
    {
        auto Delta = (Other - *this).Unit();
        X += Delta.X * Alpha;
        Y += Delta.Y * Alpha;
    }

    Vector2<T> operator+(const Vector2<T> Other) { return Vector2<T>(X + Other.X, Y + Other.Y); }
    Vector2<T> operator-(const Vector2<T> Other) { return Vector2<T>(X - Other.X, Y - Other.Y); }
    Vector2<T> operator*(double F) { return Vector2<T>(X * F, Y * F); }
    Vector2<T> operator/(double F) { return Vector2<T>(X / F, Y / F); }
    Vector2<T> operator-() { return Vector2<T>(-X, -Y); }
    Vector2<T> operator%(double A)
    {
        double Rad = M_PI * A / 180 + atan2(Y, X);
        double Mag = sqrt(X * X + Y * Y);
        return Vector2(Mag * cos(Rad), Mag * sin(Rad));
    }

    bool operator==(Vector2<T> Other) { return (X == Other.X) && (Y == Other.Y); }
    void operator+=(Vector2<T> Other) { X += Other.X; Y += Other.Y; }
    void operator-=(Vector2<T> Other) { X -= Other.X; Y -= Other.Y; }
    void operator*=(double F) { X *= F; Y *= F; }
    void operator/=(double F) { X /= F; Y /= F; }
    void operator%=(double A)
    {
        double Rad = M_PI * A / 180 + atan2(Y, X);
        double Mag = sqrt(X * X + Y * Y);
        X = Mag * cos(Rad);
        Y = Mag * sin(Rad);
    }
};

template <typename T>
struct Vector3
{
    T X;
    T Y;
    T Z;

    Vector3(T x = 0, T y = 0, T z = 0) : X(x), Y(y), Z(z) {};

    double Magnitude() { return sqrt(X * X + Y * Y + Z * Z); }
    Vector3<T> Normal() { auto V2 = Project(); return Vector3<T>((float)V2.Y, (float)-V2.X); }
    DVector3 Unit() { float M = Magnitude(); return DVector3(X / M, Y / M, Z / M); }
    void SetMagnitude(double NewMagnitude)
    {
        double F = NewMagnitude / Magnitude();
        X *= F;
        Y *= F;
        Z *= F;
    }
    void Lerp(Vector3 Other, double Alpha)
    {
        double RT = 1 - Alpha;
        X = (X * RT) + (Other.X * Alpha);
        Y = (Y * RT) + (Other.Y * Alpha);
        Z = (Z * RT) + (Other.Z * Alpha);
    }
    void Nudge(Vector3 Other, double Alpha)
    {
        auto Delta = (Other - *this).Unit();
        X += Delta.X * Alpha;
        Y += Delta.Y * Alpha;
        Z += Delta.Z * Alpha;
    }

    Vector3<T> operator-(const Vector3<T> Other) { return DVector3(X - Other.X, Y - Other.Y, Z - Other.Z); }
    Vector3<T> operator+(const Vector3<T> Other) { return DVector3(X + Other.X, Y + Other.Y, Z + Other.Z); }
    Vector3<T> operator-(const Vector2<T> Other) { return DVector3((X - Other.X), (Y - Other.Y), Z); }
    Vector3<T> operator+(const Vector2<T> Other) { return DVector3((X + Other.X), (Y + Other.Y), Z); }
    Vector3<T> operator*(double F) { return Vector3<T>(X * F, Y * F, Z * F); }
    Vector3<T> operator/(double F) { return Vector3<T>(X / F, Y / F, Z / F); }
    Vector3<T> operator-() { return Vector3<T>(-X, -Y, -Z); }
    Vector3<T> operator%(float A)
    {
        double Rad = M_PI * A / 180 + atan2(Y, X);
        double Mag = sqrt(X * X + Y * Y);
        return Vector3<T>((float)(Mag * cos(Rad)), (float)(Mag * sin(Rad)), Z);
    }

    void operator+=(Vector3<T> Other) { X += Other.X; Y += Other.Y; Z += Other.Z; }
    void operator-=(Vector3<T> Other) { X -= Other.X; Y -= Other.Y; Z -= Other.Z; }
    void operator+=(Vector2<T> V2) { X += V2.X; Y += V2.Y; }
    void operator-=(Vector2<T> V2) { X -= V2.X; Y -= V2.Y; }
    void operator*=(double F) { X *= F; Y *= F; Z *= F; }
    void operator/=(double F) { X /= F; Y /= F; Z /= F; }
    bool operator==(Vector3<T> Other) { return (X == Other.X) && (Y == Other.Y) && (Z == Other.Z); }
    bool operator==(double N) { return (X == N) && (Y == N) && (Z == N); }
    void operator%=(double A)
    {
        double Rad = (M_PI * A / 180 + atan2(Y, X));
        double Mag = sqrt(X * X + Y * Y);
        X = (Mag * cos(Rad));
        Y = (Mag * sin(Rad));
    }

    DVector3 DirectionTo(Vector3<T> Target, double Magnitude) { return (Target - *this).Unit() * Magnitude; }
    DVector3 DirectionTo(Vector3<T> Target) { return (Target - *this).Unit(); }

    IVector2 ToInt2D() { return Vector2<int>((int)X, (int)Y); }
    Vector2<T> To2D() { return Vector2<T>(X, Y); }
    Vector2<T> Project() { return Vector2<T>(X, Z + Y); }
    Vector2<T> Difference2D(Vector2<T> Other) { return Other - Project(); }

    operator IVector2() { return IVector2(X, Y); }
    operator FVector2() { return FVector2(X, Y); }
    operator DVector2() { return DVector2(X, Y); }
    operator IVector3() { return IVector3(X, Y, Z); }
    operator FVector3() { return FVector3(X, Y, Z); }
    operator DVector3() { return DVector3(X, Y, Z); }

    double DistanceFrom(Vector2<T> Other) { return (Other - *this).Magnitude(); }
    double DistanceFrom3D(Vector2<T> Other) { return (*this - Other).Magnitude(); }
    double DistanceFrom2D(Vector2<T> Other) { return this->Difference2D(Other).Magnitude(); }
};

template <typename T>
struct Rect
{
    T X;
    T Y;
    T W;
    T H;
    Rect(LPRECT Rect)
    {
        X = Rect->left;
        Y = Rect->top;
        W = Rect->right - X;
        H = Rect->bottom - Y;
    }
    Rect(T x = 0, T y = 0, T w = 0, T h = 0)
    {
        X = x;
        Y = y;
        W = w;
        H = h;
    }

    double GetArea() { return W * H; }
    Vector2<T> GetCenter() { return Vector2<T>(X + W / 2, Y + H / 2); }
    Vector2<T> Position() { return Vector2<T>(X, Y); }
    Vector2<T> Size() { return Vector2<T>(W, H); }

    Rect<T> GetIntersection(Rect<T> Other)
    {
        int MaxX = max(X, Other.X);
        int MaxY = max(Y, Other.Y);
        int MinW = min(X + W, Other.X + Other.W);
        int MinH = min(Y + H, Other.Y + Other.H);
        return Rect(
            MaxX,
            MaxY,
            (MinW - MaxX),
            (MinH - MaxY)
        );
    }
    Rect<T> SetPosition(Vector2<T> NewPos) { return Rect<T>(NewPos.X, NewPos.Y, W, H); }
    Rect<T> SetSize(Vector2<T> NewSize) { return Rect<T>(X, Y, NewSize.X, NewSize.Y); }
    Rect<T> SetCenter(Vector2<T> NewCenter) { return Rect<T>(NewCenter.X - W / 2, NewCenter.Y - H / 2); }
    Rect<T> Lerp(Rect<T> Other, float Alpha)
    {
        float RT = 1 - Alpha;
        return Rect<T>((X * RT) + (Other.X * Alpha),
            (Y * RT) + (Other.Y * Alpha),
            (W * RT) + (Other.W * Alpha),
            (H * RT) + (Other.H * Alpha)
        );
    }
    bool Intersects(Rect<T> Other)
    {
        int XDelta = X - Other.X;
        int YDelta = Y - Other.Y;
        if (XDelta < 0) { if (W < -XDelta) return false; }
        else if (Other.W < XDelta) return false;
        if (YDelta < 0) { if (H < -YDelta) return false; }
        return (Other.H >= YDelta);
    }
    bool Contains(Vector2<T> P)
    {
        auto Extents = Vector2(X + W, Y + H);
        return (P.X >= X && P.X <= Extents.X && P.Y >= Y && P.Y <= Extents.Y);
    }
    bool Contains(Vector3<T> V)
    {
        auto Extents = Vector2(X + W, Y + H);
        auto P = V.Project();
        return (P.X >= X && P.X <= Extents.X && P.Y >= Y && P.Y <= Extents.Y);
    }
    bool Contains(Rect<T> Other)
    {
        if (Other.X < X || Other.Y < Y) return false;
        if (Other.X + Other.W > X + W || Other.Y + Other.H > Y + H) return false;
        return true;
    }

    LPRECT ToLPRECT()
    {
        LPRECT New = new tagRECT();
        New->left = X;
        New->top = Y;
        New->right = X + W;
        New->bottom = Y + H;
        return New;
    }

    Rect<T> operator+(Rect<T> O) {
        return Rect<T>(
            X + O.X,
            Y + O.Y,
            W + O.W,
            H + O.H
        );
    }
    Rect<T> operator-(Rect<T> O) {
        return Rect<T>(
            X - O.X,
            Y - O.Y,
            W - O.W,
            H - O.H
        );
    }
    Rect<T> operator*(float F) {
        auto Center = GetCenter();
        auto New = Rect<T>(X, Y, (int)(W * F), (int)(H * F));
        New.SetCenter(Center);
        return New;
    }
    Rect<T> operator/(float F) {
        auto Center = GetCenter();
        auto New = Rect<T>(X, Y, (int)(W / F), (int)(H / F));
        New.SetCenter(Center);
        return New;
    }
    Rect<T> operator-() { return Rect<T>(-X - W, -Y - H, W, H); }
    bool operator==(Rect<T> O) { return (X == O.X) && (Y == O.Y) && (W == O.W) && (H == O.H); }
    operator IRect() { return IRect(X, Y, W, H); }
    operator FRect() { return FRect(X, Y, W, H); }
    operator DRect() { return DRect(X, Y, W, H); }
    operator Vector2<T>() { return Vector2<T>(X, Y); }
};

struct Color
{
    int R;
    int G;
    int B;
    int A;
    Color(float r, float g, float b, float a) { R = (int)r; G = (int)g; B = (int)b; A = (int)a; }
    Color(float r, float g, float b, int a) { R = (int)r; G = (int)g; B = (int)b; A = a; }
    Color(int r = 0, int g = 0, int b = 0, int a = 255) { R = r; G = g; B = b; A = a; }

    static Color FromHSVA(int H, int S, int V, int Alpha)
    {
        // Thank you ChatGPT for this function :pray:
        Color rgb;

        // Ensure h, s, and v are in the valid range
        H %= 360;
        S = max(0, min(100, S));
        V = max(0, min(100, V));

        // Convert percentage values to [0, 1] range
        double hh = H / 60.0;
        double ss = S / 100.0;
        double vv = V / 100.0;

        int i = static_cast<int>(floor(hh));
        double f = hh - i;
        double p = vv * (1 - ss);
        double q = vv * (1 - (ss * f));
        double t = vv * (1 - (ss * (1 - f)));

        switch (i) {
        case 0: rgb = { static_cast<int>(vv * 255), static_cast<int>(t * 255), static_cast<int>(p * 255) }; break;
        case 1: rgb = { static_cast<int>(q * 255), static_cast<int>(vv * 255), static_cast<int>(p * 255) }; break;
        case 2: rgb = { static_cast<int>(p * 255), static_cast<int>(vv * 255), static_cast<int>(t * 255) }; break;
        case 3: rgb = { static_cast<int>(p * 255), static_cast<int>(q * 255), static_cast<int>(vv * 255) }; break;
        case 4: rgb = { static_cast<int>(t * 255), static_cast<int>(p * 255), static_cast<int>(vv * 255) }; break;
        default: rgb = { static_cast<int>(vv * 255), static_cast<int>(p * 255), static_cast<int>(q * 255) }; break;
        }

        rgb.A = Alpha;
        return rgb;
    }
    static Color FromHex(std::string Hex)
    {
        Color color = { 0, 0, 0, 255 };  // Default alpha value is 255 (fully opaque)

        // Remove '#' if it exists
        std::string hexValue = (Hex[0] == '#') ? Hex.substr(1) : Hex;

        // Check the length of the hex value
        if (hexValue.length() == 3) {
            // Convert short hex format to full hex format
            hexValue = { hexValue[0], hexValue[0], hexValue[1], hexValue[1], hexValue[2], hexValue[2] };
        }
        else if (hexValue.length() == 4) {
            // Convert short hex format with alpha to full hex format with alpha
            hexValue = { hexValue[0], hexValue[0], hexValue[1], hexValue[1], hexValue[2], hexValue[2], hexValue[3], hexValue[3] };
        }

        // Convert hex to int
        std::cout << hexValue;
        std::stringstream converter(hexValue);
        converter >> std::hex >> color.R >> color.G >> color.B;

        // Check if an alpha channel is present
        if (hexValue.length() == 8) {
            converter >> color.A;
        }

        return color;
    }

    Color GetHSVA()
    {
        int H, S, V;
        // Convert RGB to percentage values
        double RR = R / 255.0;
        double GG = G / 255.0;
        double BB = B / 255.0;

        double MX = max(RR, max(GG, BB));
        double MN = min(RR, min(GG, BB));
        double Delta = MX - MN;

        // Calculate Hue
        if (Delta == 0) H = 0;
        else if (MX == RR) H = (int)(60 * fmod(((GG - BB) / Delta), 6));
        else if (MX == GG) H = (int)(60 * (((BB - RR) / Delta) + 2));
        else H = (int)(60 * (((RR - GG) / Delta) + 4));

        // Calculate Saturation
        if (MX == 0) S = 0;
        else S = (int)((Delta / MX) * 100);

        // Calculate Value
        V = (int)(MX * 100);
        return Color(H, S, V, A);
    }
    std::string GetHex()
    {
        std::stringstream Stream;
        Stream << std::uppercase << std::hex
            << std::setw(2) << std::setfill('0') << R
            << std::setw(2) << std::setfill('0') << G
            << std::setw(2) << std::setfill('0') << B
            << std::setw(2) << std::setfill('0') << A;

        return "#" + Stream.str();
    }

    int GetHue()
    {
        // Convert RGB to percentage values
        double RR = R / 255.0;
        double GG = G / 255.0;
        double BB = B / 255.0;

        double MX = max(RR, max(GG, BB));
        double MN = min(RR, min(GG, BB));
        double Delta = MX - MN;

        // Calculate Hue
        if (Delta == 0) return 0;
        else if (MX == RR) return (int)(60 * fmod(((GG - BB) / Delta), 6));
        else if (MX == GG) return (int)(60 * (((BB - RR) / Delta) + 2));
        else return (int)(60 * (((RR - GG) / Delta) + 4));
    }
    int GetSaturation()
    {
        // Convert RGB to percentage values
        double RR = R / 255.0;
        double GG = G / 255.0;
        double BB = B / 255.0;

        double MX = max(RR, max(GG, BB));
        double MN = min(RR, min(GG, BB));
        double Delta = MX - MN;

        // Calculate Saturation
        if (MX == 0) return 0;
        else return (int)((Delta / MX) * 100);
    }
    int GetValue()
    {
        // Convert RGB to percentage values
        double RR = R / 255.0;
        double GG = G / 255.0;
        double BB = B / 255.0;

        double MX = max(RR, max(GG, BB));
        double MN = min(RR, min(GG, BB));
        double Delta = MX - MN;

        // Calculate Value
        return (int)(MX * 100);
    }

    Color Lerp(Color Other, float Alpha)
    {
        float RT = 1 - Alpha;
        return Color((R * RT) + (Other.R * Alpha),
            (G * RT) + (Other.G * Alpha),
            (B * RT) + (Other.B * Alpha),
            (A * RT) + (Other.A * Alpha)
        );
    }
    Color Negate()
    {
        return Color
        (
            255 - R,
            255 - G,
            255 - B,
            A
        );
    }
    Color ShiftHue(int Amount)
    {
        Color HSV = GetHSVA();
        HSV.R += (Amount % 360);
        return FromHSVA(HSV.R, HSV.G, HSV.B, HSV.A);
    }
    Color Desaturate()
    {
        int Avg = (R + G + B) / 3;
        return Color(Avg, Avg, Avg, A);
    }

    Color operator+(Color O) { return Color(max(R + O.R, 255), max(G + O.G, 255), max(B + O.B, 255), max(A + O.A, 255)); }
    Color operator-(Color O) { return Color(min(R - O.R, 0), min(G - O.G, 0), min(B - O.B, 0), min(A - O.A, 0)); }
    bool operator==(Color O) { return (R == O.R) && (G == O.G) && (B == O.B) && (A == O.A); }

};

struct Pad
{
    int L;
    int T;
    int R;
    int B;
    Pad(float l, float t, float r, float b) { L = (int)l; T = (int)t; R = (int)r; B = (int)b; }
    Pad(int l, int t, int r, int b) { L = l; T = t; R = r; B = b; }
    Pad(int H, int V) { L = H; R = H; T = V; B = V; }
    Pad(int P) { L = P; T = P; R = P; B = P; }
    Pad() { L = 0; T = 0; R = 0; B = 0; }

    Pad Lerp(Pad Other, float Alpha = 0.5)
    {
        float RT = 1 - Alpha;
        return Pad((L * RT) + (Other.L * Alpha),
            (T * RT) + (Other.T * Alpha),
            (R * RT) + (Other.R * Alpha),
            (B * RT) + (Other.B * Alpha)
        );
    }

    Pad operator+(Pad O) { return Pad(L + O.L, T + O.T, R + O.R, B + O.B); }
    Pad operator-(Pad O) { return Pad(L - O.L, T - O.T, R - O.R, B - O.B); }
    Pad operator*(float F) { return Pad(L * F, T * F, R * F, B * F); }
    Pad operator/(float F) { return Pad(L / F, T / F, R / F, B / F); }
    bool operator==(Pad O) { return (L == O.L) && (T == O.T) && (R == O.R) && (B == O.B); }
};

struct Ratio
{
    int Numerator;
    int Denominator;
};

class Buffer
{
    PopVector<PopString> Data;
    int DataBitSize;
    int ReadBitPos;
    int WriteBitPos;

    virtual ~Buffer();
    Buffer()
    {
        *(DWORD*)this = 0x67530C;
        Data = PopVector<PopString>();
        DataBitSize = 0;
        ReadBitPos = 0;
        WriteBitPos = 0;
    }
    PopWString** __thiscall UTF8ToWideString(_Out_ PopWString&& WStr)
    {
        CONST DWORD Function = 0x5D60F0;
        __asm
        {
            mov edi, WStr
            mov ecx, this
            call Function
            ret 0x4
        }
    }
    void SeekFront() { ReadBitPos = 0; }
    void __thiscall Clear()
    {
        CONST DWORD Function = 0x5D61F0;
        __asm
        {
            mov eax, ecx
            call Function
            ret
        }
    }
    void __thiscall WriteByte(BYTE NewByte)
    {
        CONST DWORD Function = 0x5D6240;
        __asm
        {
            push NewByte
            push this
            call Function
            ret 0x4
        }
    }
};

#pragma region Post Def Functions
template<typename T>
Vector3<T> Vector2<T>::To3D() { return Vector3(X, Y, 0); }
template <typename T>
Vector2<T>::operator IVector3() { return IVector3(X, Y); }
template <typename T>
Vector2<T>::operator FVector3() { return FVector3(X, Y); }
template <typename T>
Vector2<T>::operator DVector3() { return DVector3(X, Y); }

#pragma endregion
#pragma endregion

#pragma region Other Structures
struct SmoothArray
{
    int Item;
    float Weight;
    float LastPicked;
    float SecondLastPicked;
};
#pragma endregion


class LawnApp;

template <typename T>
struct DataArrayItem;

template <typename T>
struct DataArray;

// DD Stuff
class IDirectDrawSurface;
class _DDSURFACEDESC
{
    unsigned long Size;
    unsigned long Flags;
    unsigned long Height;
    unsigned long Width;
    BYTE ImNotAddingAllThat[92];
};

// Game Objects
class Lawn;
class Plant;
class Zombie;
class Pickup;
class GridItem;
class LawnMower;
class Projectile;
class SeedPacket;
class PottedPlant;
class GameObject
{
public:
    LawnApp* App;
    Lawn* MyLawn;
    IRect Bounds;
    bool Visible;
    int Lane;
    int RenderOrder;

    IVector2 GetCell();
};


// Screens
class ChallengeScreen;
class LoadingScreen;
class CreditScreen;
class SeedChooser;
class AwardScreen;
class MainMenu;


// Systems
class SoundSystem;
class EffectSystem;


// Animation
class ReanimatorTrackInstance;
class ReanimatorDefinition;
class ReanimationHolder;
class Reanimation;


// Other
class ReanimatorCache;
class ProfileManager;
class ParticleSystem;
class CursorPreview;
class ToolTipWidget;
class MessageWidget;
class CursorObject;
class TypingCheck;
class LevelStats;
class GameButton;
class PoolEffect;
class ZenGarden;
class CrazyDave;
class Challenge;
class UserData;
class CutScene;
class SeedBank;
class PopDRM;
class Music;


// Enums
enum GameMode;
enum GameScene;
enum GameResult;

enum ReanimationType;
enum ReanimLoopType;
enum FilterEffect;

enum GridSquareType;
enum BackgroundType;
enum DebugTextMode;
enum TutorialState;
enum PlantRowType;
enum BoardResult;
enum AdviceType;

enum AttachmentID { ATTACHMENTID_NULL = 0 };
enum ParticleSystemID { PARTICLESYSTEMID_NULL = 0 };
enum ReanimationID { REANIMATIONID_NULL = 0 };
enum ZombieID { ZOMBIEID_NULL = 0 };

enum ZombieType;
enum ZombiePhase;
enum ZombieHeight;
enum HelmType;
enum ShieldType;
enum PlantID;
enum SeedType;
enum PlantState;
enum PlantOnBungeeState;
enum MagnetItemType;
enum PickupType;
enum PickupMotion;
enum GardenType;
enum DrawVariation;
enum PottedPlantAge;
enum PottedPlantNeed;
enum GridItemType;
enum GridItemState;
enum VaseType;
enum ProjectileMotion;
enum ProjectileType;
enum ParticleEffect
{
    PARTICLE_NONE = 0xFFFFFFFF,
    PARTICLE_MELONSPLASH = 0x0,
    PARTICLE_WINTERMELON = 0x1,
    PARTICLE_FUMECLOUD = 0x2,
    PARTICLE_POPCORNSPLASH = 0x3,
    PARTICLE_POWIE = 0x4,
    PARTICLE_JACKEXPLODE = 0x5,
    PARTICLE_ZOMBIE_HEAD = 0x6,
    PARTICLE_ZOMBIE_ARM = 0x7,
    PARTICLE_TRAFFIC_CONE = 0x8,
    PARTICLE_BUCKET = 0x9,
    PARTICLE_FOOTBALL_HELM = 0xA,
    PARTICLE_FLAG = 0xB,
    PARTICLE_SCREENDOOR = 0xC,
    PARTICLE_ZOMBIE_NEWSPAPER = 0xD,
    PARTICLE_ZOMBIE_HEADLIGHT = 0xE,
    PARTICLE_POW = 0xF,
    PARTICLE_ZOMBIE_POGO = 0x10,
    PARTICLE_ZOMBIE_NEWSPAPER_HEAD = 0x11,
    PARTICLE_ZOMBIE_BALLOON_HEAD = 0x12,
    PARTICLE_LANE_DUST = 0x13,
    PARTICLE_GRAVE_STONE_RISE = 0x14,
    PARTICLE_SOIL = 0x15,
    PARTICLE_WATER = 0x16,
    PARTICLE_ZOMBIE_RISE = 0x17,
    PARTICLE_STONE = 0x18,
    PARTICLE_SOIL_SPLASH = 0x19,
    PARTICLE_POOL_SPLASH = 0x1A,
    PARTICLE_ICE_SPARKLE = 0x1B,
    PARTICLE_SEED_PACKET = 0x1C,
    PARTICLE_TALL_NUT_BLOCK = 0x1D,
    PARTICLE_DOOM = 0x1E,
    PARTICLE_DIGGER_RISE = 0x1F,
    PARTICLE_DIGGER_TUNNEL = 0x20,
    PARTICLE_DANCER_RISE = 0x21,
    PARTICLE_POOL_SPARKLY = 0x22,
    PARTICLE_WALLNUT_EAT_SMALL = 0x23,
    PARTICLE_WALLNUT_EAT_LARGE = 0x24,
    PARTICLE_PEA_SPLAT = 0x25,
    PARTICLE_SPIKE_SPLAT = 0x26,
    PARTICLE_BUTTER_SPLAT = 0x27,
    PARTICLE_PUFF_SPLAT = 0x28,
    PARTICLE_STAR_SPLAT = 0x29,
    PARTICLE_ICE_TRAP = 0x2A,
    PARTICLE_SNOWPEA_SPLAT = 0x2B,
    PARTICLE_SNOWPEA_PUFF = 0x2C,
    PARTICLE_SNOWPEA_TRAIL = 0x2D,
    PARTICLE_LANTERN_SHINE = 0x2E,
    PARTICLE_SEED_PACKET_PICKUP = 0x2F,
    PARTICLE_POTATO_MINE = 0x30,
    PARTICLE_POTATO_MINE_RISE = 0x31,
    PARTICLE_PUFFSHROOM_TRAIL = 0x32,
    PARTICLE_PUFFSHROOM_MUZZLE = 0x33,
    PARTICLE_SEED_PACKET_FLASH = 0x34,
    PARTICLE_WHACK_A_ZOMBIE_RISE = 0x35,
    PARTICLE_ZOMBIE_LADDER = 0x36,
    PARTICLE_UMBRELLA_REFLECT = 0x37,
    PARTICLE_SEED_PACKET_PICK = 0x38,
    PARTICLE_ICE_TRAP_ZOMBIE = 0x39,
    PARTICLE_ICE_TRAP_RELEASE = 0x3A,
    PARTICLE_ZAMBONI_SMOKE = 0x3B,
    PARTICLE_GLOOMCLOUD = 0x3C,
    PARTICLE_ZOMBIE_POGO_HEAD = 0x3D,
    PARTICLE_ZAMBONI_TIRE = 0x3E,
    PARTICLE_ZAMBONI_EXPLOSION = 0x3F,
    PARTICLE_ZAMBONI_EXPLOSION2 = 0x40,
    PARTICLE_CATAPULT_EXPLOSION = 0x41,
    PARTICLE_MOWER_CLOUD = 0x42,
    PARTICLE_BOSS_ICE_BALL = 0x43,
    PARTICLE_BLASTMARK = 0x44,
    PARTICLE_COIN_PICKUP_ARROW = 0x45,
    PARTICLE_PRESENT_PICKUP = 0x46,
    PARTICLE_IMITATER_MORPH = 0x47,
    PARTICLE_MOWERED_ZOMBIE_HEAD = 0x48,
    PARTICLE_MOWERED_ZOMBIE_ARM = 0x49,
    PARTICLE_ZOMBIE_HEAD_POOL = 0x4A,
    PARTICLE_ZOMBIE_BOSS_FIREBALL = 0x4B,
    PARTICLE_FIREBALL_DEATH = 0x4C,
    PARTICLE_ICEBALL_DEATH = 0x4D,
    PARTICLE_ICEBALL_TRAIL = 0x4E,
    PARTICLE_FIREBALL_TRAIL = 0x4F,
    PARTICLE_BOSS_EXPLOSION = 0x50,
    PARTICLE_SCREEN_FLASH = 0x51,
    PARTICLE_TROPHY_SPARKLE = 0x52,
    PARTICLE_PORTAL_CIRCLE = 0x53,
    PARTICLE_PORTAL_SQUARE = 0x54,
    PARTICLE_POTTED_PLANT_GLOW = 0x55,
    PARTICLE_POTTED_WATER_PLANT_GLOW = 0x56,
    PARTICLE_POTTED_ZEN_GLOW = 0x57,
    PARTICLE_MIND_CONTROL = 0x58,
    PARTICLE_VASE_SHATTER = 0x59,
    PARTICLE_VASE_SHATTER_LEAF = 0x5A,
    PARTICLE_VASE_SHATTER_ZOMBIE = 0x5B,
    PARTICLE_AWARD_PICKUP_ARROW = 0x5C,
    PARTICLE_ZOMBIE_SEAWEED = 0x5D,
    PARTICLE_ZOMBIE_MUSTACHE = 0x5E,
    PARTICLE_ZOMBIE_FUTURE_GLASSES = 0x5F,
    PARTICLE_PINATA = 0x60,
    PARTICLE_DUST_SQUASH = 0x61,
    PARTICLE_DUST_FOOT = 0x62,
    PARTICLE_DAISY = 0x63,
    PARTICLE_CREDIT_STROBE = 0x64,
    PARTICLE_CREDITS_RAYSWIPE = 0x65,
    PARTICLE_CREDITS_ZOMBIEHEADWIPE = 0x66,
    PARTICLE_STARBURST = 0x67,
    PARTICLE_CREDITS_FOG = 0x68,
    NUM_PARTICLES = 0x69,
};


// Sexy
namespace Sexy
{
    struct Span;
    struct Edge;
    struct GraphicsState;
    class Graphics;

    class SexyAppBase;
    class SexyApp;

    // UI Elements
    class UIContainer;
    class UIElement;
    class UIRoot;

    class UIDialog;
    class UIButton;
    class UIDialogButton;

    class UISafeDeleteInfo;
    class ButtonListener;
    class DialogListener;

    // Image
    class Image;
    class MemoryImage;
    class SharedImage;
    class DDImage;
    class SharedImageRef;
    class AnimInfo;
    struct TriVertex;

    // Font
    class Font;
    class FontData;
    class ImageFont;
    class SysFont;
    class FontLayer;
    struct ActiveFontLayer;

    // Interfaces
    class InternetManager;
    class ResourceManager;
    class MusicInterface;
    class SoundManager;
    class DDInterface;
    class BetaSupport;

    // Other
    class PreModalInfo;

#pragma region Structs
    struct SexyMatrix3
    {
        float M00;
        float M01;
        float M02;
        float M10;
        float M11;
        float M12;
        float M20;
        float M21;
        float M22;
    };

    struct SexyTransform2D : SexyMatrix3
    {

    };
#pragma endregion
}

// Definitions
struct PlantDefinition
{
    SeedType SeedType;
    Sexy::Image** PlantImage;
    ReanimationType ReanimType;
    int PacketIndex;
    int SunCost;
    int Cooldown;
    bool Offensive;
    int ActionRate;
    const char* PlantName;
};