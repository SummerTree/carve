// Begin License:
// Copyright (C) 2006-2008 Tobias Sargeant (tobias.sargeant@gmail.com).
// All rights reserved.
//
// This file is part of the Carve CSG Library (http://carve-csg.com/)
//
// This file may be used under the terms of the GNU General Public
// License version 2.0 as published by the Free Software Foundation
// and appearing in the file LICENSE.GPL2 included in the packaging of
// this file.
//
// This file is provided "AS IS" with NO WARRANTY OF ANY KIND,
// INCLUDING THE WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE.
// End:


#if defined(HAVE_CONFIG_H)
#  include <carve_config.h>
#endif

#include <carve/interpolator.hpp>

#include <carve/csg.hpp>

#include "geom_draw.hpp"
#include "geometry.hpp"
#include "scene.hpp"

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include <fstream>
#include <string>
#include <utility>
#include <set>

#include <time.h>

#if defined(__GNUC__)
#define __stdcall
#endif

#if defined(__APPLE__)
  typedef GLvoid (*GLUTessCallback)(...);
#else
  typedef void (__stdcall *GLUTessCallback)();
#endif

#include "rgb.hpp"
#include "geometry.hpp"

static inline void glVertex(const carve::geom3d::Vector &v) {
  glVertex3f(g_scale * (v.x + g_translation.x),
             g_scale * (v.y + g_translation.y),
             g_scale * (v.z + g_translation.z));
}

static inline void glColor(const RGBA &c) {
  glColor4f(c.r, c.g, c.b, c.a);
}

carve::interpolate::FaceVertexAttr<RGBA> fv_colours;

void drawColourPolyhedron(carve::poly::Polyhedron *poly, float r, float g, float b, float a, bool offset) {
  if (offset) {
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(0.5, 0.5);
  }

  RGBA cdefault = RGBA(r, g, b);
  glColor(cdefault);

  std::vector<RGBA> vc;

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glBegin(GL_TRIANGLES);
  for (size_t i = 0, l = poly->faces.size(); i != l; ++i) {
    carve::poly::Face &f = poly->faces[i];
    if (f.vertices.size() == 3) {
      glNormal3dv(f.plane_eqn.N.v);
      glColor(fv_colours.getAttribute(&f, 0, cdefault));
      glVertex(f.vertices[0]->v);
      glColor(fv_colours.getAttribute(&f, 1, cdefault));
      glVertex(f.vertices[1]->v);
      glColor(fv_colours.getAttribute(&f, 2, cdefault));
      glVertex(f.vertices[2]->v);
    }
  }
  glEnd();
  if (offset) {
    glDisable(GL_POLYGON_OFFSET_FILL);
  }

  for (size_t i = 0, l = poly->faces.size(); i != l; ++i) {
    carve::poly::Face &f = poly->faces[i];
    if (f.vertices.size() != 3) {
      vc.resize(f.vertices.size());
      for (size_t j = 0; j < f.vertices.size(); ++j) {
        vc[j] = fv_colours.getAttribute(&f, j, cdefault);
      }
      drawColourFace(&poly->faces[i], vc, offset);
    }
  }
}

carve::poly::Polyhedron *colourCube(const carve::math::Matrix &transform = carve::math::Matrix::IDENT()) {
  std::vector<carve::poly::Vertex> v;
  v.push_back(carve::poly::Vertex(transform * carve::geom::VECTOR(+1.0, +1.0, +1.0)));
  v.push_back(carve::poly::Vertex(transform * carve::geom::VECTOR(-1.0, +1.0, +1.0)));
  v.push_back(carve::poly::Vertex(transform * carve::geom::VECTOR(-1.0, -1.0, +1.0)));
  v.push_back(carve::poly::Vertex(transform * carve::geom::VECTOR(+1.0, -1.0, +1.0)));
  v.push_back(carve::poly::Vertex(transform * carve::geom::VECTOR(+1.0, +1.0, -1.0)));
  v.push_back(carve::poly::Vertex(transform * carve::geom::VECTOR(-1.0, +1.0, -1.0)));
  v.push_back(carve::poly::Vertex(transform * carve::geom::VECTOR(-1.0, -1.0, -1.0)));
  v.push_back(carve::poly::Vertex(transform * carve::geom::VECTOR(+1.0, -1.0, -1.0)));

  std::vector<carve::poly::Face> faces;
  faces.reserve(6);

  faces.push_back(carve::poly::Face(&v[0], &v[1], &v[2], &v[3]));
  fv_colours.setAttribute(&faces[0], 0, RGBA(0,0,1));
  fv_colours.setAttribute(&faces[0], 1, RGBA(0,0,0));
  fv_colours.setAttribute(&faces[0], 2, RGBA(0,1,1));
  fv_colours.setAttribute(&faces[0], 3, RGBA(1,0,1));

  faces.push_back(carve::poly::Face(&v[7], &v[6], &v[5], &v[4]));
  fv_colours.setAttribute(&faces[1], 0, RGBA(0,1,0));
  fv_colours.setAttribute(&faces[1], 1, RGBA(0,1,1));
  fv_colours.setAttribute(&faces[1], 2, RGBA(0,0,0));
  fv_colours.setAttribute(&faces[1], 3, RGBA(1,1,0));

  faces.push_back(carve::poly::Face(&v[0], &v[4], &v[5], &v[1]));
  fv_colours.setAttribute(&faces[2], 0, RGBA(0,1,1));
  fv_colours.setAttribute(&faces[2], 1, RGBA(0,1,0));
  fv_colours.setAttribute(&faces[2], 2, RGBA(0,0,1));
  fv_colours.setAttribute(&faces[2], 3, RGBA(1,1,1));

  faces.push_back(carve::poly::Face(&v[1], &v[5], &v[6], &v[2]));
  fv_colours.setAttribute(&faces[3], 0, RGBA(1,0,0));
  fv_colours.setAttribute(&faces[3], 1, RGBA(1,0,1));
  fv_colours.setAttribute(&faces[3], 2, RGBA(1,1,0));
  fv_colours.setAttribute(&faces[3], 3, RGBA(0,0,0));

  faces.push_back(carve::poly::Face(&v[2], &v[6], &v[7], &v[3]));
  fv_colours.setAttribute(&faces[4], 0, RGBA(1,0,1));
  fv_colours.setAttribute(&faces[4], 1, RGBA(1,0,0));
  fv_colours.setAttribute(&faces[4], 2, RGBA(1,1,1));
  fv_colours.setAttribute(&faces[4], 3, RGBA(0,0,1));

  faces.push_back(carve::poly::Face(&v[3], &v[7], &v[4], &v[0]));
  fv_colours.setAttribute(&faces[5], 0, RGBA(1,1,0));
  fv_colours.setAttribute(&faces[5], 1, RGBA(1,1,1));
  fv_colours.setAttribute(&faces[5], 2, RGBA(1,0,0));
  fv_colours.setAttribute(&faces[5], 3, RGBA(0,1,0));


  return new carve::poly::Polyhedron(faces);
}

struct TestScene : public Scene {
  GLuint draw_list_base;
  std::vector<bool> draw_flags;

  virtual bool key(unsigned char k, int x, int y) {
    char *t;
    static const char *l = "1234567890!@#$%^&*()";
    t = strchr(l, k);
    if (t != NULL) {
      int layer = t - l;
      if (layer < draw_flags.size()) {
        draw_flags[layer] = !draw_flags[layer];
      }
    }
    return true;
  }

  virtual GLvoid draw() {
    for (int i = 0; i < draw_flags.size(); ++i) {
      if (draw_flags[i]) glCallList(draw_list_base + i);
    }
  }

  TestScene(int argc, char **argv, int n_dlist) : Scene(argc, argv) {
    draw_list_base = glGenLists(n_dlist);

    draw_flags.resize(n_dlist, false);
  }

  virtual ~TestScene() {
    glDeleteLists(draw_list_base, draw_flags.size());
  }
};

int main(int argc, char **argv) {
  installDebugHooks();

  TestScene *scene = new TestScene(argc, argv, 4);

  g_scale = 10.0;

  glNewList(scene->draw_list_base, GL_COMPILE);
  carve::poly::Polyhedron *a = colourCube(carve::math::Matrix::ROT(.4, .2, .3, .4));
  carve::poly::Polyhedron *b = makeTorus(20, 20, .9, .5);
  carve::csg::CSG csg;
  fv_colours.installHooks(csg);
  carve::poly::Polyhedron *c = csg.compute(a, b, carve::csg::CSG::A_MINUS_B);
  glEndList();

  glNewList(scene->draw_list_base + 1, GL_COMPILE);
  drawColourPolyhedron(a, .6, .6, .6, 1.0, false);
  glEndList();
  glNewList(scene->draw_list_base + 2, GL_COMPILE);
  drawColourPolyhedron(b, .6, .6, .6, 1.0, false);
  glEndList();
  glNewList(scene->draw_list_base + 3, GL_COMPILE);
  drawColourPolyhedron(c, .6, .6, .6, 1.0, false);
  glEndList();

  scene->draw_flags[0] = false;
  scene->draw_flags[1] = false;
  scene->draw_flags[2] = false;
  scene->draw_flags[3] = true;

  scene->run();

  delete scene;

  return 0;
}
