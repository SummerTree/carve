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

#include <carve/external/boost/unordered_set.hpp>
#include <carve/external/boost/unordered_map.hpp>
#include <functional>

namespace std {

  template <typename Key, typename T, typename Hash,
            typename Pred = std::equal_to<Key> >
  class unordered_map : public boost::unordered_map<Key, T, Hash, Pred> {
  };

  template <typename Value, typename Hash,
            typename Pred = std::equal_to<Value> >
  class unordered_set : public boost::unordered_set<Value, Hash, Pred> {
  };

}

#undef UNORDERED_COLLECTIONS_SUPPORT_RESIZE
