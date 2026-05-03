#pragma once
#include <cstdint>
#include <fstream>
#include <vector>


/**
* @brief TGA (TARGA) files consist of a linear 'stack' of data blocks.
* 1) The Fixed Header (18 bytes)
*    This is the instruction manual for the file. Every TGA file starts with these 18 bytes
*    The first byte is an ID Length. This sets the length of the 'metadata' block of data that
*    comes after the Header. If ID Length is 0, the Image ID field is skipped entirely.
* 2) Image ID (optional, variable length)
*    This stores some identifying text / metadata like "Created by XYZ"
* 3) Color Map Data (variable)
*    If the image uses a palette, the color definitions live here. A standard 24-bit or 32-bit 
*    'True Color' image doesn't use this, so this section is empty. A palette is like a an array
*    of colors where each index maps to a given color. On slower computers it is useful to 
*    store colors as indices of this array, rather than R,G,B values per pixel
* 4) Image Data (The Pixels)
*    This is the bulk of the file. It contains the actual color values for every pixel in the 
*    image. Depending on the header settings, these pixels are stored either raw or compressed 
*    using RLE (Run Length Encoding)
* 5) Footer (Optional, 26 bytes)
*    Modern TGA files often have a footer at the very end. This footer contains a "Signature"
*    (the word `TRUEVISION-XFILE`) which helps software confirm that the file is a valid TGA
*
* In our code we will only use the Header and Image Data portions.
*/


/**
 * @brief The TGA file header structure.
 * 
 * The #pragma pack(push,1) directive ensures the compiler does not add 
 * padding, as this must map exactly to the 18-byte file format on disk.
 */
#pragma pack(push,1)
struct TGAHeader {
    std::uint8_t  idlength = 0;          ///< Size of ID field that follows header
    std::uint8_t  colormaptype = 0;      ///< 0 = none, 1 = has color map
    std::uint8_t  datatypecode = 0;      ///< 2 = uncompressed RGB, 3 = uncompressed Grayscale, 10 = RLE RGB, 11 = RLE Grayscale
    std::uint16_t colormaporigin = 0;    ///< Index of first color map entry
    std::uint16_t colormaplength = 0;    ///< Count of color map entries
    std::uint8_t  colormapdepth = 0;     ///< Number of bits per pixel
    std::uint16_t x_origin = 0;          ///< X-coordinate of lower-left corner
    std::uint16_t y_origin = 0;          ///< Y-coordinate of lower-left corner
    std::uint16_t width = 0;             ///< Image width in pixels
    std::uint16_t height = 0;            ///< Image height in pixels
    std::uint8_t  bitsperpixel = 0;      ///< 8, 16, 24, or 32
    std::uint8_t  imagedescriptor = 0;   ///< 0-3: number of 'alpha' bits per pixel, 4-5: defines where "Pixel 0" is. 00 - Bottom Left, 10 - Top Left.
};
#pragma pack(pop)

/**
 * @brief Represents a single pixel color.
 * 
 * Defaults to BGRA format to match the standard TGA memory layout.
 */
struct TGAColor {
    std::uint8_t bgra[4] = {0,0,0,0};
    std::uint8_t bytespp = 4;

    /** @brief Access color channels by index (0=B, 1=G, 2=R, 3=A) */
    std::uint8_t& operator[](const int i) { return bgra[i]; }
    const std::uint8_t& operator[](const int i) const { return bgra[i]; }
};

/**
 * @brief Main class for loading, saving, and manipulating TGA images.
 */
struct TGAImage {
    enum Format { GRAYSCALE=1, RGB=3, RGBA=4 };

    TGAImage() = default;
    
    /**
     * @brief Initialize a blank image with a specific color.
     * @param w Width in pixels.
     * @param h Height in pixels.
     * @param bpp Bytes per pixel (use Format enum).
     * @param c Initial fill color.
     */
    TGAImage(const int w, const int h, const int bpp, TGAColor c = {});

    /** @brief Load image data from a .tga file. */
    bool  read_tga_file(const std::string filename);

    /**
     * @brief Save current data to a .tga file.
     * @param vflip Flip the image vertically on export (TGA is usually bottom-to-top).
     * @param rle Enable Run-Length Encoding compression.
     */
    bool write_tga_file(const std::string filename, const bool vflip=true, const bool rle=true) const;

    void flip_horizontally();
    void flip_vertically();

    /** @brief Get the color of the pixel at (x, y). */
    TGAColor get(const int x, const int y) const;

    /** @brief Set the color of the pixel at (x, y). */
    void set(const int x, const int y, const TGAColor &c);

    int width()  const;
    int height() const;

private:
    /** @brief Internal helper to decode RLE compressed streams. */
    bool   load_rle_data(std::ifstream &in);
    
    /** @brief Internal helper to encode RLE compressed streams. */
    bool unload_rle_data(std::ofstream &out) const;

    int w = 0, h = 0;
    std::uint8_t bpp = 0;
    std::vector<std::uint8_t> data = {};
};