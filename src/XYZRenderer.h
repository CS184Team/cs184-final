//
// Created by Michael Snowden on 4/15/16.
//

#ifndef COLLADAVIEWER_PLYRENDERER_H
#define COLLADAVIEWER_PLYRENDERER_H


#include <string>
#include <vector>

#include "CGL/CGL.h"

#include "scene.h"
#include "camera.h"
#include "light.h"

namespace CGL {

    class XYZRenderer : public Renderer {

    public:
        virtual void init(void);

        virtual void render(void);

        virtual void resize(size_t w, size_t h);

        virtual std::string name(void);

        virtual std::string info(void);

        virtual void scroll_event(float offset_x, float offset_y);

        void load(std::string fname);

    private:
        std::vector<Vector3D> vertices;
        std::vector<Vector3D> normals;
        Vector3D camera_angles;
        Vector3D view_focus;
        enum e_up {
            X_UP, Y_UP, Z_UP
        };
        e_up up;
        double view_distance;
        size_t screen_w;
        size_t screen_h;
        OSDText text_mgr;
        float vfov;
        float nearClip;
        float farClip;
        double min_view_distance;
        double max_view_distance;
        double canonical_view_distance;

        void mouseDrag(float x, float y);

        float mouse_x;
        float mouse_y;

        void cursor_event(float x, float y, unsigned char keys);

        void mouse_button_event(int button, int event);

        bool left_down;
        bool middle_down;
        bool right_down;

        void mouseMove(float x, float y);

        enum e_mouse_button {
            LEFT = MOUSE_BUTTON_LEFT,
            RIGHT = MOUSE_BUTTON_RIGHT,
            MIDDLE = MOUSE_BUTTON_MIDDLE
        };

        void mousePress(e_mouse_button b);

        void mouseRelease(e_mouse_button b);

        bool mouse_rotate;

        void updateMouseCoordinates(float x, float y); // updates stored mouse_x/y
        double k;
    };

} // namespace CGL


#endif //COLLADAVIEWER_PLYRENDERER_H
