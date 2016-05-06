//
// Created by Michael Snowden on 4/15/16.
//

#include "MyRenderer.h"
#include <limits>
#include "Parser.h"

#define PI 3.14159265

namespace CGL {

    void MyRenderer::init(void) {
        camera_angles = Vector3D(0.0, 0.0, 0.0);
        vfov = 35.f;
        nearClip = 0.001f;
        farClip = 10000.0f;

        left_down = false;
        right_down = false;
        middle_down = false;
        mouse_rotate = false;
        if (initHook != NULL) {
            initHook();
        }
    }

    void MyRenderer::render(void) {
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

        glPointSize(1.0f);  // default
        glDisable(GL_DEPTH_TEST);

        if (renderHook != NULL) {
            renderHook();
            glFlush();
            return;
        }

        for (int i = 0; i < vertices.size(); ++i) {
            // if (i % 10 == 0) {
                const Vector3D &v = vertices[i];
                const Vector3D &n = normals[i];
                double x1 = v.x;
                double y1 = v.y;
                double z1 = v.z;
                double x2 = x1 + n.x * k;
                double y2 = y1 + n.y * k;
                double z2 = z1 + n.z * k;
                glBegin(GL_POINTS);
                glColor3f(1.0f,0.0f,0.0f);
                glVertex3d(x1, y1, z1);
                glColor3f(0.0f,1.0f,0.0f);
                glVertex3d(x2, y2, z2);
                glEnd();
                glColor3f(0.0f,0.0f,1.0f);
                glBegin(GL_LINES);
                glVertex3d(x1, y1, z1);
                glVertex3d(x2, y2, z2);
                glEnd();
            // }
        }
        glFlush();
    }

    void MyRenderer::resize(size_t w, size_t h) {
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

    std::string MyRenderer::name(void) {
        return "MyRenderer";
    }

    std::string MyRenderer::info(void) {
        return "";
    }

    inline float bound(float a, float low, float high) {
        return std::min(high, std::max(low, a));
    }

    void MyRenderer::scroll_event(float offset_x, float offset_y) {
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

    void MyRenderer::mousePress(e_mouse_button b) {
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

    void MyRenderer::mouseRelease(e_mouse_button b) {
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

    void MyRenderer::mouseDrag(float x, float y) {
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

    void MyRenderer::mouseMove(float x, float y) {
        // Highlight the mesh element the mouse is hovering over.
    }

    void MyRenderer::mouse_button_event(int button, int event) {
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

    void MyRenderer::cursor_event(float x, float y, unsigned char keys) {

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

    void MyRenderer::updateMouseCoordinates(float x, float y) {
        mouse_x = x;
        mouse_y = y;
    }

    void MyRenderer::load(std::string fname) {
        Parser parser;
        vertices.clear();
        normals.clear();
        if (!parser.parseTxt(fname.c_str(), vertices, normals)) {
            printf("Something went wrong reading %s. Exiting.\n", fname.c_str());
            return;
        }

        setBounds(vertices);
    }

    void MyRenderer::setBounds(const vector<Vector3D> &vertices) {
        double maxVal = std::numeric_limits<double>::max();
        Vector3D low(maxVal, maxVal, maxVal);
        Vector3D high(-maxVal, -maxVal, -maxVal);
        Vector3D centroid(0.0, 0.0, 0.0);
        for (auto vertex : vertices) {
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
        }
        centroid /= (double) vertices.size();
        setBounds(low, centroid, high);

    }

    void MyRenderer::setBounds(const Vector3D &low, const Vector3D &mid, const Vector3D &high) {// Determine how far away the camera should be.
        // Minimum distance guaranteed to not clip into the model in C - V.
        canonical_view_distance = (high - low).norm() * 1.01; // norm is magnitude.
        min_view_distance = canonical_view_distance / 10.0;
        view_distance = canonical_view_distance * 2.;
        max_view_distance = canonical_view_distance * 20.;
        k = canonical_view_distance / 10.0;
        camera_angles = Vector3D(0., 0., 0.);
        view_focus = mid;
        up = Z_UP;
    }

    void MyRenderer::key_event(char key) {
        if (keyEventHook != NULL) {
            keyEventHook(key);
            return;
        }
        switch (key) {
            case ' ':
                std::cout << "Pausing/Unpausing" << std::endl;
                break;
        }
    }

//    void XYZRender::
}