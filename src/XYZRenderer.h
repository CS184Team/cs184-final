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
        virtual void init(void) override;

        virtual void render(void) override;

        virtual void resize(size_t w, size_t h) override;

        virtual std::string name(void) override;

        virtual std::string info(void) override;

        void load(std::string fname);

    private:
        GLfloat * vertices;
        long nVertices;
    };

} // namespace CGL


#endif //COLLADAVIEWER_PLYRENDERER_H
