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


#pragma once

#include <carve/carve.hpp>

#include <carve/vector.hpp>
#include <carve/tag.hpp>

#include <vector>
#include <list>

namespace carve {
  namespace poly {



    struct Polyhedron;
    class Face;



    class Edge : public tagable {
    public:
      const Vertex *v1, *v2;
      const Polyhedron *owner;

      Edge(const Vertex *_v1, const Vertex *_v2, const Polyhedron *_owner);

      ~Edge();
    };



    struct hash_edge_ptr {
      size_t operator()(const Edge * const &e) const {
        return (size_t)e;
      }
    };



  }
}

