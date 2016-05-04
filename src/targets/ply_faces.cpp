#include "../Parser.h"
#include <vector>
#include "CGL/CGL.h"
#include <utility>
#include "../MyRenderer.h"

using namespace std;

int main(int argc, char **argv) {
	if (argc < 2) {
		printf("Usage:\n\t%s <input.ply>\n", argv[0]);
		return 1;
	}
	char *iname = argv[1];
	Parser parser;
	vector<Vector3D> vertices;
	vector<Triangle> faces;
	parser.parsePlyFaces(iname, vertices, faces);
	function<void(void)> render_hook = [&vertices, &faces] {
		glPointSize(1.0f);
		glColor3f(1.0f,0.0f,0.0f);
//		glBegin(GL_POINTS);
//		for (auto vertex : vertices) {
//			glVertex3d(vertex.x, vertex.y, vertex.z);
//		}
//		glEnd();
		glColor3f(1.0f,1.0f,1.0f);
		for (auto face : faces) {
			int i1 = face.v1;
			int i2 = face.v2;
			int i3 = face.v3;
			Vector3D v1 = vertices[i1];
			Vector3D v2 = vertices[i2];
			Vector3D v3 = vertices[i3];
			glBegin(GL_TRIANGLES);
			glVertex3d(v1.x, v1.y, v1.z);
			glVertex3d(v2.x, v2.y, v2.z);
			glVertex3d(v3.x, v3.y, v3.z);
			glEnd();
		}
	};
    MyRenderer *xyzRenderer = new MyRenderer();
    xyzRenderer->renderHook = render_hook;
    xyzRenderer->setBounds(vertices);
	Viewer viewer = Viewer();
    viewer.set_renderer(xyzRenderer);
    viewer.init();
    viewer.start();
}