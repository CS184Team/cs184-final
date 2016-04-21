#include "../Parser.h"
#include <vector>
#include "CGL/CGL.h"
#include <utility>
#include "../XYZRenderer.h"

using namespace std;

int main(int argc, char **argv) {
	if (argc < 2) {
		printf("Usage:\n\t%s <input.ply>\n", argv[0]);
		return 1;
	}
	char *iname = argv[1];
	Parser parser;
	vector<Vector3D> vertices;
	vector<pair<int, int> > lines;
	parser.parsePlyLines(iname, vertices, lines);
	function<void(void)> render_hook = [&vertices, &lines] {
		glPointSize(1.0f);
		glColor3f(1.0f,0.0f,0.0f);
		glBegin(GL_POINTS);
		for (auto vertex : vertices) {
			glVertex3d(vertex.x, vertex.y, vertex.z);
		}
		glEnd();
		glColor3f(1.0f,1.0f,1.0f);
		for (auto line : lines) {
			int i1 = line.first;
			int i2 = line.second;
			Vector3D v1 = vertices[i1];
			Vector3D v2 = vertices[i2];
			glBegin(GL_LINES);
			glVertex3d(v1.x, v1.y, v1.z);
			glVertex3d(v2.x, v2.y, v2.z);
			glEnd();
		}
	};
    XYZRenderer *xyzRenderer = new XYZRenderer();
    xyzRenderer->render_hook = &render_hook;
    xyzRenderer->setBounds(vertices);

	Viewer viewer = Viewer();
    viewer.set_renderer(xyzRenderer);
    viewer.init();
    viewer.start();
}