# Questions

## What's `stdint.h`?

A C header file which contains the exact-width integer structures (used to define the Windows data types).

## What's the point of using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` in a program?

To know exactly how many bits, and what each value represents. E.g. `uint8_t` is an unsigned 8-bit (1-byte) integer.

## How many bytes is a `BYTE`, a `DWORD`, a `LONG`, and a `WORD`, respectively?

So: `BYTE` is 1 byte long, `DWORD` is 4 bytes long, `LONG` is 4 bytes long, and `WORD` is 2 bytes long.

## What (in ASCII, decimal, or hexadecimal) must the first two bytes of any BMP file be? Leading bytes used to identify file formats (with high probability) are generally called "magic numbers."

Since the first two bytes represent the file type in: ASCII this is `B M`, decimal this is `66 77`, and in hexadecimal this is `42 4D`.

## What's the difference between `bfSize` and `biSize`?

The difference is that `bfSize` is the size of the entire bitmap file in bytes, whereas `biSize` is the number of bytes required for the structure `BITMAPINFOHEADER`.

## What does it mean if `biHeight` is negative?

The pixels are given top-down, hence the origin is in the upper-left corner.

## What field in `BITMAPINFOHEADER` specifies the BMP's color depth (i.e., bits per pixel)?

The `biBitCount` field specifies the colour depth by a 2-byte integer.

## Why might `fopen` return `NULL` in lines 24 and 32 of `copy.c`?

If an invalid file name is given (or some other system error occurs, such as no space in memory can be found which is large enough) and so the pointers are given no memory addresses.

## Why is the third argument to `fread` always `1` in our code?

Since you have already given the size of `BITMAPFILEHEADER` and `BITMAPINFOHEADER` you only want to read these from the file once.

## What value does line 63 of `copy.c` assign to `padding` if `bi.biWidth` is `3`?

So since `sizeof(RGBTRIPLE)` will return `3` (bytes), if `bi.biWidth` is `3` then: `(4 - [(3 * 3) % 4]) % 4 = (4 - [9 % 4]) % 4 = (4 - 1) % 4 = 3 % 4 = 3`. Hence `padding` is assigned a value of `3`.

## What does `fseek` do?

The function `fseek` moves the file pointer (backwards or forwards) by a given amount, such that you can skip over or move back to earlier bites in a file.

## What is `SEEK_CUR`?

The parameter `SEEK_CUR`, of the function `fseek`, gives the current location of the file pointer.

## Whodunit?

Professor Plum with the candlestick in the library.
