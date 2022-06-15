#pragma once
#include <cstdint>
#include <fstream>
#include <vector>

#pragma pack(push,1)//将原来的对齐方式设置为压栈，并自定义对齐字节数
struct TGAHeader {
    std::uint8_t  idlength{};
    std::uint8_t  colormaptype{};
    std::uint8_t  datatypecode{};
    std::uint16_t colormaporigin{};
    std::uint16_t colormaplength{};
    std::uint8_t  colormapdepth{};
    std::uint16_t x_origin{};
    std::uint16_t y_origin{};
    std::uint16_t width{};
    std::uint16_t height{};
    std::uint8_t  bitsperpixel{};
    std::uint8_t  imagedescriptor{};
};
#pragma pack(pop)//取消自定义对齐方式

/*
**首先确定数据成员unint8_t bgra[4]用于存放RGBA值，再设置构造函数
**添加数组赋值构造函数，故需添加数据成员bytespp
**对于unint8_t bgra[4]，添加下标获取函数
*/
struct TGAColor {
    std::uint8_t bgra[4] = {0,0,0,0};  //BRGA
    std::uint8_t bytespp {0};  //用于确定数组赋值构造函数的元素个数

    /*
    **三种构造函数：默认构造函数、直接赋值构造函数、通过数组赋值构造函数
    */
    TGAColor() = default;
    TGAColor(const std::uint8_t R, const std::uint8_t G, const std::uint8_t B, const std::uint8_t A=255) : bgra{B,G,R,A}, bytespp(4) { }
    TGAColor(const std::uint8_t *p, const std::uint8_t bpp) : bytespp(bpp) {
        for (int i=bpp; i--; bgra[i] = p[i]);
    }

    std::uint8_t& operator[](const int i) { return bgra[i]; }
};

struct TGAImage {
    enum Format { GRAYSCALE=1, RGB=3, RGBA=4 };//不同的颜色设置对应不同的向量维度，用于设置数据成员bpp

    /*
    **两种构造函数：默认构造函数、直接赋值构造函数
    */
    TGAImage() = default;
    TGAImage(const int w, const int h, const int bpp);

    bool read_tga_file(const std::string filename);
    bool write_tga_file(const std::string filename, const bool vflip=true, const bool rle=true) const;

    void flip_horizontally();
    void flip_vertically();

    TGAColor get(const int x, const int y) const;
    void set(const int x, const int y, const TGAColor &c);

    /*
    **数据成员的获取函数
    */
    int width()  const;
    int height() const;

private:
    bool load_rle_data(std::ifstream &in);
    bool unload_rle_data(std::ofstream &out) const;

    int w   = 0;  //画框的宽度
    int h   = 0;  //画框的高度
    int bpp = 0;  //bytes per pixel, 每像素所占字节数
    std::vector<std::uint8_t> data = {};  //元素为uint8_t类型的vector
};

