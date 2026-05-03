#include <iostream>
#include <cstring>
#include "tgaimage.h"

/**
 * @brief Construct a new TGAImage and initialize with a fill color.
 */
TGAImage::TGAImage(const int w, const int h, const int bpp, TGAColor c) 
    : w(w), h(h), bpp(bpp), data(w * h * bpp, 0) {
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            set(i, j, c);
        }
    }
}

/**
 * @brief Reads a TGA file from disk. Supports Raw and RLE compressed formats.
 */
bool TGAImage::read_tga_file(const std::string filename) {
    std::ifstream in;
    in.open(filename, std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "can't open file " << filename << "\n";
        return false;
    }

    TGAHeader header;
    in.read(reinterpret_cast<char *>(&header), sizeof(header));
    if (!in.good()) {
        std::cerr << "an error occurred while reading the header\n";
        return false;
    }

    w   = header.width;
    h   = header.height;
    bpp = header.bitsperpixel >> 3; // Convert bits to bytes
    
    if (w <= 0 || h <= 0 || (bpp != GRAYSCALE && bpp != RGB && bpp != RGBA)) {
        std::cerr << "bad bpp (or width/height) value\n";
        return false;
    }

    size_t nbytes = (size_t)bpp * w * h;
    data = std::vector<std::uint8_t>(nbytes, 0);

    // Datatype 2: Uncompressed True-color, 3: Uncompressed Black & White
    if (3 == header.datatypecode || 2 == header.datatypecode) {
        in.read(reinterpret_cast<char *>(data.data()), nbytes);
        if (!in.good()) {
            std::cerr << "an error occurred while reading the data\n";
            return false;
        }
    } 
    // Datatype 10: RLE True-color, 11: RLE Black & White
    else if (10 == header.datatypecode || 11 == header.datatypecode) {
        if (!load_rle_data(in)) {
            std::cerr << "an error occurred while reading the data\n";
            return false;
        }
    } else {
        std::cerr << "unknown file format " << (int)header.datatypecode << "\n";
        return false;
    }

    // Check imagedescriptor for flip bits (bit 5 is vertical, bit 4 is horizontal)
    if (!(header.imagedescriptor & 0x20)) flip_vertically();
    if (header.imagedescriptor & 0x10) flip_horizontally();

    std::cerr << w << "x" << h << "/" << bpp * 8 << "\n";
    return true;
}

/**
 * @brief Decodes Run-Length Encoded (RLE) data from the input stream.
 */
bool TGAImage::load_rle_data(std::ifstream &in) {
    size_t pixelcount = (size_t)w * h;
    size_t currentpixel = 0;
    size_t currentbyte  = 0;
    TGAColor colorbuffer;

    do {
        std::uint8_t chunkheader = in.get();
        if (!in.good()) return false;

        if (chunkheader < 128) { 
            // Raw chunk: next (chunkheader + 1) pixels are read directly
            chunkheader++;
            for (int i = 0; i < chunkheader; i++) {
                in.read(reinterpret_cast<char *>(colorbuffer.bgra), bpp);
                if (!in.good()) return false;
                for (int t = 0; t < bpp; t++)
                    data[currentbyte++] = colorbuffer.bgra[t];
                currentpixel++;
            }
        } else {
            // RLE chunk: next (chunkheader - 127) pixels have the same color
            chunkheader -= 127;
            in.read(reinterpret_cast<char *>(colorbuffer.bgra), bpp);
            if (!in.good()) return false;
            for (int i = 0; i < chunkheader; i++) {
                for (int t = 0; t < bpp; t++)
                    data[currentbyte++] = colorbuffer.bgra[t];
                currentpixel++;
            }
        }
    } while (currentpixel < pixelcount);
    return true;
}

/**
 * @brief Writes the current image data to a TGA file.
 * Includes a standard TGA 2.0 footer for compatibility.
 */
bool TGAImage::write_tga_file(const std::string filename, const bool vflip, const bool rle) const {
    constexpr std::uint8_t developer_area_ref[4] = {0, 0, 0, 0};
    constexpr std::uint8_t extension_area_ref[4] = {0, 0, 0, 0};
    constexpr std::uint8_t footer[18] = {'T','R','U','E','V','I','S','I','O','N','-','X','F','I','L','E','.','\0'};
    
    std::ofstream out;
    out.open(filename, std::ios::binary);
    if (!out.is_open()) {
        std::cerr << "can't open file " << filename << "\n";
        return false;
    }

    TGAHeader header = {};
    header.bitsperpixel = bpp << 3;
    header.width  = w;
    header.height = h;
    header.datatypecode = (bpp == GRAYSCALE ? (rle ? 11 : 3) : (rle ? 10 : 2));
    header.imagedescriptor = vflip ? 0x00 : 0x20; 

    out.write(reinterpret_cast<const char *>(&header), sizeof(header));
    if (!out.good()) goto err;

    if (!rle) {
        out.write(reinterpret_cast<const char *>(data.data()), (size_t)w * h * bpp);
    } else {
        if (!unload_rle_data(out)) goto err;
    }

    out.write(reinterpret_cast<const char *>(developer_area_ref), sizeof(developer_area_ref));
    out.write(reinterpret_cast<const char *>(extension_area_ref), sizeof(extension_area_ref));
    out.write(reinterpret_cast<const char *>(footer), sizeof(footer));
    
    if (!out.good()) goto err;
    return true;

err:
    std::cerr << "can't dump the tga file\n";
    return false;
}

/**
 * @brief Encodes image data into RLE chunks for disk storage.
 */
bool TGAImage::unload_rle_data(std::ofstream &out) const {
    const std::uint8_t max_chunk_length = 128;
    size_t npixels = (size_t)w * h;
    size_t curpix = 0;

    while (curpix < npixels) {
        size_t chunkstart = curpix * bpp;
        size_t curbyte = curpix * bpp;
        std::uint8_t run_length = 1;
        bool raw = true;

        // Search for repeating pixels
        while (curpix + run_length < npixels && run_length < max_chunk_length) {
            bool succ_eq = true;
            for (int t = 0; succ_eq && t < bpp; t++)
                succ_eq = (data[curbyte + t] == data[curbyte + t + bpp]);
            
            curbyte += bpp;
            if (1 == run_length) raw = !succ_eq;
            if (raw && succ_eq) { run_length--; break; }
            if (!raw && !succ_eq) break;
            run_length++;
        }
        curpix += run_length;
        out.put(raw ? run_length - 1 : run_length + 127);
        out.write(reinterpret_cast<const char *>(data.data() + chunkstart), (raw ? run_length * bpp : bpp));
        if (!out.good()) return false;
    }
    return true;
}

TGAColor TGAImage::get(const int x, const int y) const {
    if (data.empty() || x < 0 || y < 0 || x >= w || y >= h) return {};
    TGAColor ret = {0, 0, 0, 0, bpp};
    const std::uint8_t *p = data.data() + (x + y * w) * bpp;
    for (int i = bpp; i--; ret.bgra[i] = p[i]);
    return ret;
}

void TGAImage::set(int x, int y, const TGAColor &c) {
    if (data.empty() || x < 0 || y < 0 || x >= w || y >= h) return;
    memcpy(data.data() + (x + y * w) * bpp, c.bgra, bpp);
}

void TGAImage::flip_horizontally() {
    for (int i = 0; i < w / 2; i++) {
        for (int j = 0; j < h; j++) {
            for (int b = 0; b < bpp; b++) {
                std::swap(data[(i + j * w) * bpp + b], data[(w - 1 - i + j * w) * bpp + b]);
            }
        }
    }
}

void TGAImage::flip_vertically() {
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h / 2; j++) {
            for (int b = 0; b < bpp; b++) {
                std::swap(data[(i + j * w) * bpp + b], data[(i + (h - 1 - j) * w) * bpp + b]);
            }
        }
    }
}

int TGAImage::width() const { return w; }
int TGAImage::height() const { return h; }