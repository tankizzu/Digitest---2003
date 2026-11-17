/* Draws the touch controls (../touch.c) over the finished frame, just before the swap: one quad per shape, the
   shape itself (a ring, a disc, the PlayStation symbols) cut out in the fragment shader */
#include "gl.h"
#include "../touch.h"

static const char *TOUCH_VS =
    "#version 330 core\n"
    "layout(location = 0) in vec2 pos;\n"
    "layout(location = 1) in vec4 shape;\n" /* centre x, y, radius, kind */
    "layout(location = 2) in vec2 alpha_value;\n"
    "uniform vec2 screen;\n"
    "out vec2 p;\n"
    "flat out int kind;\n"
    "out float a;\n"
    "out float value;\n"
    "void main() {\n"
    "    p = (pos - shape.xy) / shape.z;\n"
    "    kind = int(shape.w + 0.5);\n"
    "    a = alpha_value.x;\n"
    "    value = alpha_value.y;\n"
    "    vec2 ndc = pos / screen * 2.0 - 1.0;\n"
    "    gl_Position = vec4(ndc.x, -ndc.y, 0.0, 1.0);\n"
    "}\n";

static const char *TOUCH_FS =
    "#version 330 core\n"
    "in vec2 p;\n"
    "flat in int kind;\n"
    "in float a;\n"
    "in float value;\n"
    "out vec4 frag;\n"
    "float seg(vec2 q, vec2 s, vec2 e) { vec2 d = e - s; float t = clamp(dot(q - s, d) / dot(d, d), 0.0, 1.0); return length(q - s - d * t); }\n"
    "void main() {\n"
    "    float d = length(p), aa = 0.04;\n"
    "    float disc = 1.0 - smoothstep(1.0 - aa, 1.0, d);\n"
    "    if (kind == 0) { frag = vec4(1.0, 1.0, 1.0, a * (1.0 - smoothstep(0.05, 0.05 + aa, abs(d - 0.92)))); return; }\n"
    "    if (kind == 1) { frag = vec4(1.0, 1.0, 1.0, a * disc); return; }\n"
    "    if (kind == 10) {\n" /* a ring that fills clockwise from the top, over a dim one */
    "        float turn = fract(atan(p.x, -p.y) / 6.2831853);\n"
    "        float band = 1.0 - smoothstep(0.1, 0.1 + aa, abs(d - 0.8));\n"
    "        float lit = turn <= value ? 1.0 : 0.25;\n"
    "        frag = vec4(1.0, 0.82, 0.48, a * band * lit + 0.35 * a * disc * (1.0 - band));\n"
    "        return;\n"
    "    }\n"
    "    if (kind == 13) { discard; }\n" /* the arrow itself: the sprite pass below */
    "    if (kind == 14) {\n" /* the speed: x1, x2, x3 */
    "        float sp = floor(value + 0.5), g = min(seg(p, vec2(-0.95, -0.3), vec2(-0.4, 0.35)), seg(p, vec2(-0.95, 0.35), vec2(-0.4, -0.3)));\n"
    "        if (sp == 1.0) { g = min(g, min(seg(p, vec2(0.4, -0.65), vec2(0.4, 0.65)), seg(p, vec2(0.12, -0.38), vec2(0.4, -0.65)))); }\n"
    "        if (sp == 2.0) { g = min(g, min(min(seg(p, vec2(0.05, -0.4), vec2(0.25, -0.65)), seg(p, vec2(0.25, -0.65), vec2(0.6, -0.65))),\n"
    "                              min(min(seg(p, vec2(0.6, -0.65), vec2(0.75, -0.35)), seg(p, vec2(0.75, -0.35), vec2(0.05, 0.65))), seg(p, vec2(0.05, 0.65), vec2(0.75, 0.65))))); }\n"
    "        if (sp == 3.0) { g = min(g, min(min(seg(p, vec2(0.05, -0.65), vec2(0.72, -0.65)), seg(p, vec2(0.72, -0.65), vec2(0.35, -0.05))),\n"
    "                              min(min(seg(p, vec2(0.35, -0.05), vec2(0.75, 0.25)), seg(p, vec2(0.75, 0.25), vec2(0.5, 0.65))), seg(p, vec2(0.5, 0.65), vec2(0.05, 0.6))))); }\n"
    "        float ink = 1.0 - smoothstep(0.1, 0.1 + aa, g), rim = (1.0 - smoothstep(0.2, 0.2 + aa, g)) - ink;\n"
    "        frag = vec4(mix(vec3(0.12, 0.05, 0.0), vec3(1.0, 0.6, 0.15), ink), a * max(ink, rim * 0.8));\n"
    "        return;\n"
    "    }\n"
    "    float g = 1e3; vec3 c = vec3(1.0);\n"
    "    if (kind == 2) { g = min(seg(p, vec2(-0.38, -0.38), vec2(0.38, 0.38)), seg(p, vec2(-0.38, 0.38), vec2(0.38, -0.38))); c = vec3(0.55, 0.7, 1.0); }\n"
    "    if (kind == 3) { g = abs(d - 0.4); c = vec3(1.0, 0.45, 0.45); }\n"
    "    if (kind == 4) { g = min(min(seg(p, vec2(0.0, -0.45), vec2(0.42, 0.3)), seg(p, vec2(0.42, 0.3), vec2(-0.42, 0.3))), seg(p, vec2(-0.42, 0.3), vec2(0.0, -0.45))); c = vec3(0.45, 1.0, 0.65); }\n"
    "    if (kind == 5) { vec2 q = abs(p) - vec2(0.36); g = abs(max(q.x, q.y)); c = vec3(1.0, 0.6, 0.85); }\n"
    "    if (kind == 6) { g = min(min(seg(p, vec2(-0.25, -0.35), vec2(0.35, 0.0)), seg(p, vec2(0.35, 0.0), vec2(-0.25, 0.35))), seg(p, vec2(-0.25, 0.35), vec2(-0.25, -0.35))); }\n"
    "    if (kind == 7) { g = min(seg(p, vec2(-0.35, -0.15), vec2(0.35, -0.15)), seg(p, vec2(-0.35, 0.15), vec2(0.35, 0.15))); }\n"
    "    if (kind == 8) { g = min(seg(p, vec2(-0.2, -0.4), vec2(-0.2, 0.35)), seg(p, vec2(-0.2, 0.35), vec2(0.3, 0.35))); }\n"
    "    if (kind == 9) { g = min(min(seg(p, vec2(-0.25, -0.4), vec2(-0.25, 0.4)), seg(p, vec2(-0.25, -0.4), vec2(0.2, -0.4))),\n"
    "                     min(seg(p, vec2(0.2, -0.4), vec2(0.2, 0.0)), min(seg(p, vec2(0.2, 0.0), vec2(-0.25, 0.0)), seg(p, vec2(-0.05, 0.0), vec2(0.3, 0.4))))); }\n"
    "    if (kind == 11) { g = min(min(seg(p, vec2(-0.38, -0.28), vec2(0.38, -0.28)), seg(p, vec2(-0.38, 0.0), vec2(0.38, 0.0))), seg(p, vec2(-0.38, 0.28), vec2(0.38, 0.28))); }\n"
    "    if (kind == 12) { g = min(seg(p, vec2(0.12, -0.38), vec2(-0.22, 0.0)), seg(p, vec2(-0.22, 0.0), vec2(0.12, 0.38))); }\n"
    "    float glyph = 1.0 - smoothstep(0.07, 0.07 + aa, g);\n"
    "    vec4 bg = vec4(0.05, 0.08, 0.1, 0.55 * a * disc);\n"
    "    frag = mix(bg, vec4(c, min(1.0, a * 2.2)), glyph * disc);\n"
    "}\n";

static GLuint s_prog, s_vao, s_vbo;
static GLint s_screen;

/* The balloons' arrow (touch_arrow): a textured quad, its texels as the game's, turned to point right */
static const char *ARROW_VS =
    "#version 330 core\n"
    "layout(location = 0) in vec2 pos;\n"
    "layout(location = 1) in vec2 uv_in;\n"
    "uniform vec2 screen;\n"
    "out vec2 uv;\n"
    "void main() {\n"
    "    uv = uv_in;\n"
    "    vec2 ndc = pos / screen * 2.0 - 1.0;\n"
    "    gl_Position = vec4(ndc.x, -ndc.y, 0.0, 1.0);\n"
    "}\n";
static const char *ARROW_FS =
    "#version 330 core\n"
    "in vec2 uv;\n"
    "uniform sampler2D tex;\n"
    "uniform vec4 tint;\n" /* the shadow pass: black; the arrow: white, its alpha */
    "out vec4 frag;\n"
    "void main() {\n"
    "    vec4 c = texture(tex, uv);\n"
    "    frag = vec4(c.rgb * tint.rgb, c.a * tint.a);\n"
    "}\n";
static GLuint s_arrow_prog, s_arrow_tex;
static GLint s_arrow_screen, s_arrow_tint;

#define ARROW_MAX 24 /* touch.c's */

static void draw_arrow(const TouchShape *b, int w, int h) {
    const uint32_t *px;
    int aw, ah;
    float k, qw, qh, x0, y0, v[4 * 6];

    if (!touch_arrow(&px, &aw, &ah) || aw == 0) {
        return;
    }
    if (s_arrow_prog == 0) {
        if ((s_arrow_prog = gl_program(ARROW_VS, ARROW_FS)) == 0) {
            return;
        }
        s_arrow_screen = GL(glGetUniformLocation)(s_arrow_prog, "screen");
        s_arrow_tint = GL(glGetUniformLocation)(s_arrow_prog, "tint");
        GL(glGenTextures)(1, &s_arrow_tex);
    }
    GL(glActiveTexture)(GL_TEXTURE0);
    GL(glBindTexture)(GL_TEXTURE_2D, s_arrow_tex);
    GL(glPixelStorei)(GL_UNPACK_ALIGNMENT, 4);
    GL(glTexImage2D)(GL_TEXTURE_2D, 0, GL_RGBA8, ARROW_MAX, ARROW_MAX, 0, GL_RGBA, GL_UNSIGNED_BYTE, px);
    GL(glTexParameteri)(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    GL(glTexParameteri)(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    GL(glTexParameteri)(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    GL(glTexParameteri)(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    /* turned a quarter to the left: the image's down (where the arrow points) is the screen's right */
    k = b->r * 1.9f / (float)SDL_max(aw, ah); /* its long side, once turned, 1.9 radii across */
    qw = (float)ah * k, qh = (float)aw * k;
    x0 = b->x - b->r * 1.05f, y0 = b->y - qh * 0.5f;
    {
        float us = (float)aw / ARROW_MAX, vs = (float)ah / ARROW_MAX;
        /* corners: screen (x, y) and texture (u, v) with u = 1 - screen y, v = screen x */
        float c[6][4] = {{x0, y0, us, 0}, {x0 + qw, y0, us, vs}, {x0 + qw, y0 + qh, 0, vs},
                         {x0, y0, us, 0}, {x0 + qw, y0 + qh, 0, vs}, {x0, y0 + qh, 0, 0}};
        SDL_memcpy(v, c, sizeof c);
    }
    GL(glUseProgram)(s_arrow_prog);
    GL(glUniform2f)(s_arrow_screen, (float)w, (float)h);
    GL(glUniform1i)(GL(glGetUniformLocation)(s_arrow_prog, "tex"), 0);
    GL(glBindVertexArray)(s_vao);
    GL(glBindBuffer)(GL_ARRAY_BUFFER, s_vbo);
    GL(glDisableVertexAttribArray)(2);
    GL(glEnableVertexAttribArray)(0);
    GL(glVertexAttribPointer)(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    GL(glEnableVertexAttribArray)(1);
    GL(glVertexAttribPointer)(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    for (int pass = 0; pass < 2; pass++) { /* a shadow a little down and right, then the arrow */
        float d = pass == 0 ? k * 0.8f : 0.0f;
        float shifted[4 * 6];
        for (int i = 0; i < 6; i++) {
            shifted[i * 4] = v[i * 4] + d, shifted[i * 4 + 1] = v[i * 4 + 1] + d;
            shifted[i * 4 + 2] = v[i * 4 + 2], shifted[i * 4 + 3] = v[i * 4 + 3];
        }
        GL(glBufferData)(GL_ARRAY_BUFFER, sizeof shifted, shifted, GL_STREAM_DRAW);
        GL(glUniform4f)(s_arrow_tint, pass == 0 ? 0.0f : 1.0f, pass == 0 ? 0.0f : 1.0f, pass == 0 ? 0.0f : 1.0f,
                        pass == 0 ? b->alpha * 0.55f : b->alpha);
        GL(glDrawArrays)(GL_TRIANGLES, 0, 6);
    }
}

void gl_touch_draw(int w, int h) {
    TouchShape shapes[16];
    float v[16 * 6 * 8];
    int n = touch_shapes(shapes, 16), k = 0;
    GLint prog, vao, vbo, view[4];

    if (n == 0) {
        return;
    }
    if (s_prog == 0) {
        if ((s_prog = gl_program(TOUCH_VS, TOUCH_FS)) == 0) {
            return;
        }
        s_screen = GL(glGetUniformLocation)(s_prog, "screen");
        GL(glGenVertexArrays)(1, &s_vao);
        GL(glGenBuffers)(1, &s_vbo);
    }
    for (int i = 0; i < n; i++) {
        const TouchShape *s = &shapes[i];
        static const float CORNER[6][2] = {{-1, -1}, {1, -1}, {1, 1}, {-1, -1}, {1, 1}, {-1, 1}};

        for (int c = 0; c < 6; c++) {
            float *o = &v[k++ * 8];
            o[0] = s->x + CORNER[c][0] * s->r, o[1] = s->y + CORNER[c][1] * s->r;
            o[2] = s->x, o[3] = s->y, o[4] = s->r, o[5] = (float)s->kind, o[6] = s->alpha, o[7] = s->value;
        }
    }
    GL(glGetIntegerv)(GL_CURRENT_PROGRAM, &prog);
    GL(glGetIntegerv)(GL_VERTEX_ARRAY_BINDING, &vao);
    GL(glGetIntegerv)(GL_ARRAY_BUFFER_BINDING, &vbo);
    GL(glGetIntegerv)(GL_VIEWPORT, view);
    GL(glBindFramebuffer)(GL_FRAMEBUFFER, 0);
    GL(glViewport)(0, 0, w, h);
    GL(glDisable)(GL_SCISSOR_TEST);
    GL(glDisable)(GL_DEPTH_TEST);
    GL(glDisable)(GL_STENCIL_TEST);
    GL(glEnable)(GL_BLEND);
    GL(glBlendEquation)(GL_FUNC_ADD);
    GL(glBlendFuncSeparate)(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    GL(glUseProgram)(s_prog);
    GL(glUniform2f)(s_screen, (float)w, (float)h);
    GL(glBindVertexArray)(s_vao);
    GL(glBindBuffer)(GL_ARRAY_BUFFER, s_vbo);
    GL(glBufferData)(GL_ARRAY_BUFFER, (GLsizeiptr)(k * 8 * sizeof(float)), v, GL_STREAM_DRAW);
    GL(glEnableVertexAttribArray)(0);
    GL(glVertexAttribPointer)(0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    GL(glEnableVertexAttribArray)(1);
    GL(glVertexAttribPointer)(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(2 * sizeof(float)));
    GL(glEnableVertexAttribArray)(2);
    GL(glVertexAttribPointer)(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    GL(glDrawArrays)(GL_TRIANGLES, 0, k);
    for (int i = 0; i < n; i++) {
        if (shapes[i].kind == TOUCH_AUTO) {
            draw_arrow(&shapes[i], w, h);
        }
    }
    GL(glDisable)(GL_BLEND);
    GL(glUseProgram)((GLuint)prog);
    GL(glBindVertexArray)((GLuint)vao);
    GL(glBindBuffer)(GL_ARRAY_BUFFER, (GLuint)vbo);
    GL(glViewport)(view[0], view[1], view[2], view[3]);
}
