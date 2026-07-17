## Chess Coordinate system

How do we talk about a chess board? We must know where the pieces are and what they are called. r, n, b, q, k, p are the black Rook, Knight, Bishop, Queen, and King, and R, N, B, Q, K, P and the white Rook, Knight, Bishop, Queen, and King, respectively. A row of a chess board is called a "rank" and a column is called a "file". Each square also has a location, and when talking about a square, its location is always used, such as "a white pawn is on a2". The letters are on the X axis of the board and the numbers are on Y axis of the board. Here is a visual description of the initial state of the board and of the idenitification system for each square:

|   |   |
|---|---|
|\|   \|   \|   \|   \|   \|   \|   \|   \|<br>\|---\|---\|---\|---\|---\|---\|---\|---\|<br>\|r\|n\|b\|q\|k\|b\|n\|r\|<br>\|p\|p\|p\|p\|p\|p\|p\|p\|<br>\|<br>\|<br>\|<br>\|<br>\|P\|P\|P\|P\|P\|P\|P\|P\|<br>\|R\|N\|B\|Q\|K\|B\|N\|R\||\|   \|   \|   \|   \|   \|   \|   \|   \|<br>\|---\|---\|---\|---\|---\|---\|---\|---\|<br>\|A8\|B8\|C8\|D8\|E8\|F8\|G8\|H8\|<br>\|A7\|B7\|C7\|D7\|E7\|F7\|G7\|H7\|<br>\|A6\|B6\|C6\|D6\|E6\|F6\|G6\|H6\|<br>\|A5\|B5\|C5\|D5\|E5\|F5\|G5\|H5\|<br>\|A4\|B4\|C4\|D4\|E4\|F4\|G4\|H4\|<br>\|A3\|B3\|C3\|D3\|E3\|F3\|G3\|H3\|<br>\|A2\|B2\|C2\|D2\|E2\|F2\|G2\|H2\|<br>\|A1\|B1\|C1\|D1\|E1\|F1\|G1\|H1\||

Note that the white side will **ALWAYS** be on the bottom and the black side will **ALWAYS** be on top.

## The 64-bit number

Here is an example of what a 64-bit number looks like:

|   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|

The blue digit is the 63rd bit (or Most Significant Bit, abbreviated MSB) and the green digit is the 0th bit (or the Least Significant Bit, abbreviated LSB). When used to represent chess state, this 64-bit number is called a bitboard.

Modern computers like the Opteron or IA64 can natively handle 64-bit numbers and manipulate them in one instruction. 32-bit computers must break up the bit operations in to 2 or more instructions causing a slowdown of performance. However, we will be ignoring such performance issues for clarity's sake.

The operations that can happen with a 64-bit number that concern us at this moment are the logical bitwise operators: **NOT, AND, OR, XOR**. The boolean logic looks much like this:

|   |   |   |   |   |   |   |
|---|---|---|---|---|---|---|
|A|B|**NOT A**|**NOT B**|**A AND B**|**A OR B**|**A XOR B**|
|0|0|1|1|0|0|0|
|0|1|1|0|0|1|1|
|1|0|0|1|0|1|1|
|1|1|0|0|1|1|0|

In practice, this is what an AND could look like:

|   |   |
|---|---|
||\|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|<br>\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|<br>\|1\|0\|1\|0\|1\|1\|1\|0\|1\|0\|0\|1\|1\|1\|0\|1\|0\|1\|1\|0\|1\|1\|0\|1\|0\|1\|0\|1\|0\|1\|1\|1\|0\|1\|0\|1\|0\|1\|1\|0\|0\|1\|0\|1\|1\|0\|1\|0\|1\|0\|1\|1\|0\|1\|1\|0\|0\|0\|1\|1\|1\|0\|1\|1\||
|**AND**|\|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|<br>\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|<br>\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|1\|1\|1\|1\|1\|1\|1\|1\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\||
|**=**|\|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|<br>\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|<br>\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|1\|0\|1\|0\|1\|1\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\||

There are also two more bit operations that aren't boolean logic, but instead actions that you can perform with a 64 bit number. The **LSHFT** and **RSHFT** operators are left shift and right shift operators respectively. In C, **LSHFT** would be **<<** and **RSHFT** would be **>>**. They mean that you take the 64 bit number and shift it left or right by a specified number of bit places. If bits fall off the edge, they are gone forever and if there aren't enough bits to produce a full 64 bit number, zero are added to always keep the bits at 64. Here's an example.

Suppose we have this 64-bit number:

|   |   |
|---|---|
|Number:|\|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|<br>\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|<br>\|1\|1\|1\|1\|1\|1\|1\|1\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|1\|1\|1\|1\|1\|1\|1\|1\||

If we shift it left (from LSB to MSB) by 4 bits, then this is what we end up with:

|   |   |
|---|---|
|Number:|\|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|<br>\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|<br>\|1\|1\|1\|1\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|1\|1\|1\|1\|1\|1\|1\|1\|0\|0\|0\|0\||

Consequently, if we shift it right by 4 bits, then this is what we get:

|   |   |
|---|---|
|Number:|\|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|<br>\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|<br>\|0\|0\|0\|0\|1\|1\|1\|1\|1\|1\|1\|1\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|1\|1\|1\|1\||

Shifting might not appear to be of obvious use at this moment, but it will become very important when determining movements and attacks in parallel of certain pieces.

## **FirstBit** and **LastBit**

The function **FirstBit** gives the index of the first bit that is turned on from the LSB side. So in this example:

|   |   |
|---|---|
|Number:|\|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|<br>\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|<br>\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|1\|0\|0\|0\|1\|0\|0\|0\|0\|0\|1\|0\|0\|0\|0\|1\|0\|1\|0\|1\|1\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|1\|0\|0\|1\|0\|0\|0\|0\|0\|1\|0\|0\|0\|0\||

**FirstBit(Number)** is equal to **4** which is the number of the index (from zero) of the green digit in the Number from the LSB side.

The function **LastBit** gives the index of the first bit that is turned on from the MSB side. So, continuing the above example:

**LastBit(Number)** is equal to **45** which is the number of the index (from zero) of the blue digit in the Number from the LSB side.

## C code representation

Simple C89-ish code to define a bitboard can look like this if your compiler and architecture supports the **unsigned long long** extension at 8 bytes width:

typedef unsigned long long Bitboard;

C99 users could probably get away with:

typedef uint64_t Bitboard;

## Chess Board Mapping

So how do we decide how to map each chess board square to a bit in the 64-bit integer? This mapping could be arbitrary, but it behooves us to pick a mathematically advantageous mapping. In this particular mapping (or winding) we are going to say that position a1 is the least signficant bit (LSB), bit 0, of the 64 bit number and h8 is the most significant bit (MSB), bit 63. The squares will be assigned in a left to right, bottom to top ordering to each bit index in the 64 bit number from LSB to MSB.

|   |   |
|---|---|
|\|   \|   \|   \|   \|   \|   \|   \|   \|<br>\|---\|---\|---\|---\|---\|---\|---\|---\|<br>\|A8\|B8\|C8\|D8\|E8\|F8\|G8\|H8\|<br>\|A7\|B7\|C7\|D7\|E7\|F7\|G7\|H7\|<br>\|A6\|B6\|C6\|D6\|E6\|F6\|G6\|H6\|<br>\|A5\|B5\|C5\|D5\|E5\|F5\|G5\|H5\|<br>\|A4\|B4\|C4\|D4\|E4\|F4\|G4\|H4\|<br>\|A3\|B3\|C3\|D3\|E3\|F3\|G3\|H3\|<br>\|A2\|B2\|C2\|D2\|E2\|F2\|G2\|H2\|<br>\|A1\|B1\|C1\|D1\|E1\|F1\|G1\|H1\||\|   \|   \|   \|   \|   \|   \|   \|   \|<br>\|---\|---\|---\|---\|---\|---\|---\|---\|<br>\|56\|57\|58\|59\|60\|61\|62\|63\|<br>\|48\|49\|50\|51\|52\|53\|54\|55\|<br>\|40\|41\|42\|43\|44\|45\|46\|47\|<br>\|32\|33\|34\|35\|36\|37\|38\|39\|<br>\|24\|25\|26\|27\|28\|29\|30\|31\|<br>\|16\|17\|18\|19\|20\|21\|22\|23\|<br>\|8\|9\|10\|11\|12\|13\|14\|15\|<br>\|0\|1\|2\|3\|4\|5\|6\|7\||

Our first real example will be a bitboard which represents the WhitePawns. The left board is the initial physical location of the white pawns on a real board, and the right board is the bitboard equivalent.

|   |   |
|---|---|
|\|   \|   \|   \|   \|   \|   \|   \|   \|<br>\|---\|---\|---\|---\|---\|---\|---\|---\|<br>\|<br>\|<br>\|<br>\|<br>\|<br>\|<br>\|P\|P\|P\|P\|P\|P\|P\|P\|<br>\||\|   \|   \|   \|   \|   \|   \|   \|   \|<br>\|---\|---\|---\|---\|---\|---\|---\|---\|<br>\|0\|0\|0\|0\|0\|0\|0\|0\|<br>\|0\|0\|0\|0\|0\|0\|0\|0\|<br>\|0\|0\|0\|0\|0\|0\|0\|0\|<br>\|0\|0\|0\|0\|0\|0\|0\|0\|<br>\|0\|0\|0\|0\|0\|0\|0\|0\|<br>\|0\|0\|0\|0\|0\|0\|0\|0\|<br>\|1\|1\|1\|1\|1\|1\|1\|1\|<br>\|0\|0\|0\|0\|0\|0\|0\|0\||

The bitboard which represents the WhitePawns is this.

|   |   |
|---|---|
|WhitePawns:|\|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|<br>\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|<br>\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|1\|1\|1\|1\|1\|1\|1\|1\|0\|0\|0\|0\|0\|0\|0\|0\||

These functions will be useful to us later when we want to iteratively process the results of a bitboard calculation.

At this point once more piece of information must be explained concerning the chess board square identification with respect to the winding. We are explicitly assigning a direct bit index to each position of the board. So:

A1 = 0  
B1 = 1  
C1 = 2  
D1 = 3  
E1 = 4  
F1 = 5  
G1 = 6  
H1 = 7  
A2 = 8  
B2 = 9  
.....  
G7 = 54  
H7 = 55  
A8 = 56  
B8 = 57  
C8 = 58  
D8 = 59  
E8 = 60  
F8 = 61  
G8 = 62  
H8 = 63  

And we are explicitly assigning a bit index to a position on the board:

0 = A1  
1 = B1  
.....  
62 = G8  
63 = H8  

This becomes important later when we start using the bit index and the chess board identification symbol interchangeably depending upon if we are talking about a position on the chess board or a index into a bitboard--especially in terms of **FirstBit** and **LastBit**

## Bitboard Visualization

Often in this document, you will see me formatting a bitboard like the above and doing logical bitwise operations on it with the results also being bitboards formatted to look like chess boards. In effect, if I had two boards, say **BOARD_A** and **BOARD_B**, then if I wanted to do this: **BOARD_A AND BOARD_B = BOARD_C**, then **BOARD_A's A1 AND BOARD_B's A1 = BOARD_C's A1** for each position from A1 to H8.

Here is an example of visualizing the bitboards as chess boards while doing bitwise operations on them.

A previous example:

|   |   |
|---|---|
||\|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|<br>\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|<br>\|1\|0\|1\|0\|1\|1\|1\|0\|1\|0\|0\|1\|1\|1\|0\|1\|0\|1\|1\|0\|1\|1\|0\|1\|0\|1\|0\|1\|0\|1\|1\|1\|0\|1\|0\|1\|0\|1\|1\|0\|0\|1\|0\|1\|1\|0\|1\|0\|1\|0\|1\|1\|0\|1\|1\|0\|0\|0\|1\|1\|1\|0\|1\|1\||
|**AND**|\|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|<br>\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|<br>\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|1\|1\|1\|1\|1\|1\|1\|1\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\||
|**=**|\|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|   \|<br>\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|---\|<br>\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|1\|0\|1\|0\|1\|1\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\|0\||

The previous example rendered as chess boards:

|   |   |   |   |   |
|---|---|---|---|---|
|\|   \|   \|   \|   \|   \|   \|   \|   \|<br>\|---\|---\|---\|---\|---\|---\|---\|---\|<br>\|0\|1\|1\|1\|0\|1\|0\|1\|<br>\|1\|0\|1\|1\|1\|0\|0\|1\|<br>\|1\|0\|1\|1\|0\|1\|1\|0\|<br>\|1\|1\|1\|0\|1\|0\|1\|0\|<br>\|0\|1\|1\|0\|1\|0\|1\|0\|<br>\|0\|1\|0\|1\|1\|0\|1\|0\|<br>\|0\|1\|1\|0\|1\|1\|0\|1\|<br>\|1\|1\|0\|1\|1\|1\|0\|0\||**AND**|\|   \|   \|   \|   \|   \|   \|   \|   \|<br>\|---\|---\|---\|---\|---\|---\|---\|---\|<br>\|0\|0\|0\|0\|0\|0\|0\|0\|<br>\|0\|0\|0\|0\|0\|0\|0\|0\|<br>\|0\|0\|0\|0\|0\|0\|0\|0\|<br>\|0\|0\|0\|0\|0\|0\|0\|0\|<br>\|1\|1\|1\|1\|1\|1\|1\|1\|<br>\|0\|0\|0\|0\|0\|0\|0\|0\|<br>\|0\|0\|0\|0\|0\|0\|0\|0\|<br>\|0\|0\|0\|0\|0\|0\|0\|0\||**=**|\|   \|   \|   \|   \|   \|   \|   \|   \|<br>\|---\|---\|---\|---\|---\|---\|---\|---\|<br>\|0\|0\|0\|0\|0\|0\|0\|0\|<br>\|0\|0\|0\|0\|0\|0\|0\|0\|<br>\|0\|0\|0\|0\|0\|0\|0\|0\|<br>\|0\|0\|0\|0\|0\|0\|0\|0\|<br>\|0\|1\|1\|0\|1\|0\|1\|0\|<br>\|0\|0\|0\|0\|0\|0\|0\|0\|<br>\|0\|0\|0\|0\|0\|0\|0\|0\|<br>\|0\|0\|0\|0\|0\|0\|0\|0\||

These two forms, the bitboard AND operation and the chess board AND operation, are equivalent.