#include <Windows.h>
#include <iostream>
#include <vector>

namespace VBCompression
{
	struct Segment
	{
	public:
		BYTE Color;
		BYTE CSize;
		UINT RepeatCount;

		Segment(BYTE C, UINT RC)
		{
			Color = C;
			RepeatCount = RC;
		}

		static Segment DecompressFrom(BYTE* In)
		{
			signed char CHeader = *In;
			BYTE Color = CHeader << 1;
			bool DoesRepeat = CHeader & 0b10000000;
			Segment self = Segment(Color, 0);

			self.CSize = 1;
			In++;

			// std::cout << "Repeats: " << DoesRepeat << "\n";
			if (!DoesRepeat)
				self.RepeatCount = 1;
			else
			{
				BYTE SizeHeader = *In;
				BYTE SizeByteCost = SizeHeader >> 6;
				UINT RepCount = 0;

				// std::cout << "SBC: " << (int)SizeByteCost << " | " << (int)SizeHeader << "\n";
				if (SizeByteCost == 0b00)// Costs 1 byte
				{
					// std::cout << "1 BC\n";
					RepCount = SizeHeader;

					In++;
					self.CSize++;
				}
				else if (SizeByteCost == 0b01)// Costs 2 bytes
				{
					// std::cout << "2 BC: " << *(UINT*)In << "\n";
					BYTE FirstByte = *In & 0b00111111; // Mask out the highest 2 bit
					BYTE LowerByte = *(In + 1);
					RepCount = ((UINT)FirstByte << 8) | (UINT)LowerByte;

					In += 2;
					self.CSize += 2;
				}
				else if (SizeByteCost == 0b10)// Costs 3 bytes
				{
					// std::cout << "3 BC: " << *(UINT*)In << "\n";
					BYTE FirstByte = *In & 0b01111111; // Mask out the highest bit
					WORD LowerBytes = *(WORD*)(In + 1);
					RepCount = ((UINT)FirstByte << 16) | (UINT)LowerBytes;

					In += 3;
					self.CSize += 3;
				}
				else
				{
					// std::cout << "4 BC\n";
					RepCount = (*(UINT*)In) & 0b00111111111111111111111111111111;
					
					In += 4;
					self.CSize += 4;
				}

				RepCount++;
				self.RepeatCount = RepCount;
			}

			return self;
		}

		BYTE* CompressTo(BYTE* Destination)
		{
			// A repeated segment takes up 1 byte for color & 1 byte for rep count minimum
			BYTE* DestBackup = Destination;
			BYTE CompressedColor = Color >> 1;
			if (RepeatCount > 1)
			{
				CompressedColor |= 0b10000000;
				*Destination = CompressedColor;
				Destination++;

				RepeatCount--;
				if (RepeatCount < 0b01000000)
				{
					// 1 Byte is enough to store the count
					*Destination = (BYTE)(RepeatCount);
					Destination++;
				}
				else if (RepeatCount < 0b0100000000000000)
				{
					// 2 Bytes are enough to store the count
					// Flip the first 2 bits to 01, indicating that 1 more byte is to be read to form the full count
					*Destination = ((BYTE)(RepeatCount >> 8)) | 0b01000000;// Set size header
					Destination++;
					*(BYTE*)Destination = (BYTE)(RepeatCount);
					Destination++;
				}
				else if (RepeatCount < 0b010000000000000000000000)
				{
					// 3 Bytes are enough to store the count
					*Destination = ((BYTE)(RepeatCount >> 16)) | 0b10000000;// Set size header
					Destination++;
					*(WORD*)Destination = (WORD)(RepeatCount);
					Destination += 2;
				}
				else if (RepeatCount < 0b01000000000000000000000000000000)
				{
					// 4 Bytes are enough to store the count
					*(UINT*)Destination = (RepeatCount) | 0b01000000000000000000000000000000;
					Destination += 4;// Add 4 bytes (sizeof UINT)
				}
				RepeatCount++;

				auto Decompressed = DecompressFrom(DestBackup);
				if (Decompressed == *this)
				{
					//std::cout << "-----------------------Consistency-----------------------\n";
					//std::cout << "Property | Original Segment | Decompressed Segment \n";
					//std::cout << "Color | " << (int)Color << " | " << (int)Decompressed.Color << "\n";
					//std::cout << "RC | " << RepeatCount << " | " << Decompressed.RepeatCount << "\n";
				}
				else
				{
					std::cout << "--------------------------------------------Inconsistency--------------------------------------------\n";
					std::cout << "Property | Original Segment | Decompressed Segment \n";
					std::cout << "Color | " << (int)Color << " | " << (int)Decompressed.Color << "\n";
					std::cout << "RC | " << RepeatCount << " | " << Decompressed.RepeatCount << "\n";
				}
				return Destination;
			}
			else
			{
				// Do not repeat, only write the bytes
				*Destination = CompressedColor;
				Destination++;
				return Destination;
			}
		}
		
		void DecompressTo(BYTE* Out)
		{
			for (int i = 0; i < RepeatCount; i++)
			{
				*Out = Color;
				Out++;
			}
		}

		bool operator==(Segment Other)
		{
			return (abs(Color - Other.Color) < 2) && RepeatCount == Other.RepeatCount;
		}
	};

	static std::vector<Segment> Segments = std::vector<Segment>();
	BYTE* Compress(size_t InSize, BYTE* In, BYTE* Out)
	{
		Segments.clear();
		
		// Quantize Segments
		BYTE* InputEnd = In + InSize;
		BYTE LastColor = *In;
		int RepeatCount = 1;
		In++;

		while (In < InputEnd)
		{
			BYTE Color = *In;
			if (Color == LastColor)
				RepeatCount++;
			else
			{
				Segments.push_back(Segment(LastColor, RepeatCount));
				LastColor = Color;
				RepeatCount = 1;
			}
			In++;
		}
		Segments.push_back(Segment(LastColor, RepeatCount));// Add the last segment
		
		// Compressing Segments
		for (auto Seg : Segments)
		{
			Out = Seg.CompressTo(Out);
		}
		return Out;
	}

	void Decompress(size_t InSize, BYTE* In, BYTE* Out)
	{
		BYTE* InputEnd = In + InSize;
		while (In < InputEnd)
		{
			auto S = Segment::DecompressFrom(In);
			In += S.CSize;
			for (int i = 0; i < S.RepeatCount; i++)
			{
				*Out = S.Color;
				Out++;
			}
		}
	}
}