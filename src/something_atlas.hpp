#ifndef SOMETHING_ATLAS_HPP_
#define SOMETHING_ATLAS_HPP_

#include "./something_math.hpp"

struct Atlas {
    Texture texture;
    GL_Texture gl_texture;
    Dynamic_Array<AABB<float>> uvs;

    static Atlas from_config(const char *file_path, int margin);

    AABB<float> get_uv(Index<AABB<float>> uv_index) const;
};

#endif // SOMETHING_ATLAS_HPP_
