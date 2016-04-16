//
// Created by Michael Snowden on 4/15/16.
//

#include "XYZRenderer.h"
#include <limits>

#define PI 3.14159265

namespace CGL {

    void XYZRenderer::init(void) {
        camera_angles = Vector3D(0.0, 0.0, 0.0);
        vfov = 35.f;
        nearClip = 0.1f;
        farClip = 1000.f;

        left_down = false;
        right_down = false;
        middle_down = false;
        mouse_rotate = false;
    }

    void XYZRenderer::render(void) {
        GLint view[4];
        glGetIntegerv(GL_VIEWPORT, view);
        resize((size_t) view[2], (size_t) view[3]);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        double cx, cy, cz;
        double up_x, up_y, up_z;
        up_x = up_y = up_z = 0.0;

        double v_x, v_y, v_z;
        v_x = view_focus.x;
        v_y = view_focus.y;
        v_z = view_focus.z;

        switch (up) {
            case Z_UP:
                up_z = 1.0;
                cx = v_x + view_distance * sin(camera_angles.x) * cos(camera_angles.y * .99);
                cy = v_y + view_distance * cos(camera_angles.x) * cos(camera_angles.y * .99);
                cz = v_z + view_distance * sin(camera_angles.y);
                break;
            default:
                return;
        }

        gluLookAt(cx, cy, cz, // camera location.
                  v_x, v_y, v_z, // point looking at.
                  up_x, up_y, up_z); // up direction.

        float *vertex = vertices;
        glPointSize(1.0f);  // default
        glDisable(GL_DEPTH_TEST);
        for (long i = 0; i < nVertices; ++i) {
            glBegin(GL_POINTS);
            glVertex3d(vertex[0], vertex[1], vertex[2]);
            glEnd();
            glBegin(GL_LINES);
            glVertex3d(vertex[0], vertex[1], vertex[2]);
            glVertex3d(vertex[0] + vertex[3], vertex[1] + vertex[4], vertex[2] + vertex[5]);
            glEnd();
            vertex += 6;
        }
        glFlush();
    }

    void XYZRenderer::resize(size_t w, size_t h) {
        screen_w = w;
        screen_h = h;

        // FIX the Text renderer.
        text_mgr.resize(w, h);

        // Enter perspective projection mode.
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(vfov, // Field of view in DEGREES.
                       (1.0 * screen_w) / screen_h, // Aspect Ratio.
                       nearClip, // distance near side of the view frustrum.
                       farClip);
    }

    std::string XYZRenderer::name(void) {
        return "XYZRenderer";
    }

    std::string XYZRenderer::info(void) {
        return "Renders .xyz files as point clouds";
    }

    inline float bound(float a, float low, float high) {
        return std::min(high, std::max(low, a));
    }

    void XYZRenderer::scroll_event(float offset_x, float offset_y) {
        if (offset_y > 0) {
            view_distance -= offset_y * (view_distance / 4);
        } else if (offset_y < 0) {
            view_distance -= offset_y * (view_distance / 4);
        }

        // Ensure appropiate bounds amounts.
        view_distance = bound((float) view_distance,
                              (float) min_view_distance,
                              (float) max_view_distance);

    }

    void XYZRenderer::mousePress(e_mouse_button b) {
        switch (b) {
            case LEFT:
                mouse_rotate = true;
                break;
            case RIGHT:
                mouse_rotate = true;
                break;
            case MIDDLE:
                break;
        }
    }

    void XYZRenderer::mouseRelease(e_mouse_button b) {
        switch (b) {
            case LEFT:
                mouse_rotate = false;
                break;
            case RIGHT:
                mouse_rotate = false;
                break;
            case MIDDLE:
                break;
        }
    }

    void XYZRenderer::mouseDrag(float x, float y) {
        // Rotate the camera when the left mouse button is dragged.
        float dx = (x - mouse_x);
        float dy = (y - mouse_y);

        if (mouse_rotate) {
            double &cx = camera_angles.x;
            double &cy = camera_angles.y;

            cx += dx * 2.0 * PI / screen_w;
            cy += dy * PI / screen_h;

            // Users can freely rotate the model's as much as they
            // want in the horizontal direction.
            cx = cx >= 0.0 ? std::min(cx, cx - 2.0 * PI) : (cx + 2.0 * PI);

            // Bound the vertical view angle.
            camera_angles.y = bound((float) camera_angles.y, (float) (-PI / 2.0), (float) (PI / 2.0));
        }
    }

    void XYZRenderer::mouseMove(float x, float y) {
        // Highlight the mesh element the mouse is hovering over.
    }

    void XYZRenderer::mouse_button_event(int button, int event) {
        switch (event) {
            case MOUSE_BUTTON_RELEASE: {
                switch (button) {
                    case MOUSE_BUTTON_LEFT:
                        mouseRelease(LEFT);
                        left_down = false;
                        break;
                    case MOUSE_BUTTON_RIGHT:
                        mouseRelease(RIGHT);
                        right_down = false;
                        break;
                    case MOUSE_BUTTON_MIDDLE:
                        mouseRelease(MIDDLE);
                        middle_down = false;
                        break;
                }
                break;
            }
            case MOUSE_BUTTON_PRESS: {
                switch (button) {
                    case MOUSE_BUTTON_LEFT:
                        left_down = true;
                        mousePress(LEFT);
                        break;
                    case MOUSE_BUTTON_RIGHT:
                        right_down = true;
                        mousePress(RIGHT);
                        break;
                    case MOUSE_BUTTON_MIDDLE:
                        middle_down = true;
                        mousePress(MIDDLE);
                        break;
                }
                break;
            }
        }
    }

    void XYZRenderer::cursor_event(float x, float y, unsigned char keys) {

        // Mouse dragged if any moust button is held down.
        // if(keys != 0)
        if (left_down || middle_down || right_down) {
            mouseDrag(x, y);
        } else // Mouse moved otherwise.
        {
            mouseMove(x, y);
        }

        // Always update the correct current mouse coordinates.
        updateMouseCoordinates(x, y);

        return;
    }

    void XYZRenderer::updateMouseCoordinates(float x, float y) {
        mouse_x = x;
        mouse_y = y;
    }

    void XYZRenderer::load(std::string fname) {
        FILE *file = fopen(fname.c_str(), "r");
        if (!file) return;

        char line[256];
        nVertices = -1;
        fgets(line, sizeof(line), file);
        sscanf(line, "%li", &nVertices);
        if (nVertices == -1) return;

        free(vertices);
        vertices = (GLfloat *) malloc(sizeof(GLfloat) * nVertices * 6);

        GLfloat *vertex = vertices;
        while (fgets(line, sizeof(line), file)) {
            sscanf(line, "%f %f %f %f %f %f", vertex, vertex + 1, vertex + 2, vertex + 3, vertex + 4, vertex + 5);
            vertex += 6;
        }
        fclose(file);

        double maxVal = std::numeric_limits<double>::max();
        Vector3D low(maxVal, maxVal, maxVal);
        Vector3D high(-maxVal, -maxVal, -maxVal);
        Vector3D centroid(0.0, 0.0, 0.0);
        vertex = vertices;
        for (int i = 0; i < nVertices; ++i) {
            GLfloat x = vertex[0];
            GLfloat y = vertex[1];
            GLfloat z = vertex[2];
            if (x < low.x) {
                low.x = x;
            }
            if (y < low.y) {
                low.y = y;
            }
            if (z < low.z) {
                low.z = z;
            }
            if (x > high.x) {
                high.x = x;
            }
            if (y > high.y) {
                high.y = y;
            }
            if (z > high.z) {
                high.z = z;
            }
            centroid.x += x;
            centroid.y += y;
            centroid.z += z;
            vertex += 6;
        }
        centroid /= (double) nVertices;

        // Determine how far away the camera should be.
        // Minimum distance guaranteed to not clip into the model in C - V.
        canonical_view_distance = (high - low).norm() * 1.01; // norm is magnitude.
        min_view_distance = canonical_view_distance / 10.0;
        view_distance = canonical_view_distance * 2.;
        max_view_distance = canonical_view_distance * 20.;

        camera_angles = Vector3D(0., 0., 0.);
        view_focus = centroid;
        up = Z_UP;
    }

//    void XYZRender::
}