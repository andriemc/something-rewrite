#include "./something_circle_renderer.hpp"

void Circle_Renderer::init()
{
    vbo_element_size[ATTRIB_CENTER] = sizeof(centers[0]);
    vbo_element_size[ATTRIB_RADIUS] = sizeof(radii[0]);
    vbo_element_size[ATTRIB_COLOR]  = sizeof(colors[0]);
    static_assert(COUNT_ATTRIBS == 3);

    vbo_datas[ATTRIB_CENTER] = centers;
    vbo_datas[ATTRIB_RADIUS] = radii;
    vbo_datas[ATTRIB_COLOR]  = colors;
    static_assert(COUNT_ATTRIBS == 3);

    attrib_size[ATTRIB_CENTER] = V2_COMPONENTS;
    attrib_size[ATTRIB_RADIUS] = 1;
    attrib_size[ATTRIB_COLOR]  = RGBA_COMPONENTS;
    static_assert(COUNT_ATTRIBS == 3);

    glGenVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);
    glGenBuffers(COUNT_ATTRIBS, vbo_ids);

    for (int attrib = 0; attrib < COUNT_ATTRIBS; ++attrib) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[attrib]);
        glBufferData(GL_ARRAY_BUFFER,
                     vbo_element_size[attrib] * CAPACITY,
                     vbo_datas[attrib],
                     GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(attrib);

        glVertexAttribPointer(
            attrib,    // index
            attrib_size[attrib],      // numComponents
            GL_FLOAT,           // type
            0,                  // normalized
            0,                  // stride
            0                   // offset
        );

        glVertexAttribDivisorARB(attrib, 1);
    }
}

void Circle_Renderer::sync_buffers()
{
    for (int attrib = 0; attrib < COUNT_ATTRIBS; ++attrib) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[attrib]);
        glBufferSubData(GL_ARRAY_BUFFER,
                        0,
                        vbo_element_size[attrib] * count,
                        vbo_datas[attrib]);
    }
}

void Circle_Renderer::fill_circle(V2<GLfloat> center, GLfloat radius, RGBA color)
{
    // NOTE: I'm not sure if we should ignore the call if the buffer is full or crash.
    // Crash can help to troubleshoot disappearing triangles problem in the future.
    assert(count < CAPACITY);
    centers[count] = center;
    radii[count]   = radius;
    colors[count]  = color;
    count += 1;
}

void Circle_Renderer::draw()
{
    glBindVertexArray(vao_id);

    sync_buffers();
    const size_t QUAD_COMPONENTS = 4;
    glDrawArraysInstancedARB(GL_TRIANGLE_STRIP,
                             0,
                             QUAD_COMPONENTS,
                             static_cast<GLsizei>(count));
}

void Circle_Renderer::clear()
{
    count = 0;
}