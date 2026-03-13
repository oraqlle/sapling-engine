# Contributing

## Header Structure

* Headers (`*.h`) should only contain interfaces unless they are templates
* Source files (`*.cxx`) contain implementation
* The engine is broken up into subsystems (refer to [architecture.md](architecture.md)),
  each with one or more modules.

```cxx
// <[subsystem]/[module]/[header_name].h> -*- C++ -*-

//  Sapling 3D Game Engine
//  Copyright (C) 2026  Tyler Swann, Georgia Kannelis
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License v2.1 as published by the Free Software Foundation.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
//  USA

#ifndef SAPLING_ENGINE_<HEADER_NAME>_H
#define SAPLING_ENGINE_<HEADER_NAME>_H

#include <> // internal module headers

#include <> // other subsystem and module from engine

#include <> // third party headers

#include <> // standard headers

namespace sap::<subsystem>::<module> {

// ... header content ...

} // namespace sap::<subsystem>::<module>

#endif // SAPLING_ENGINE_<HEADER_NAME>_H
```

## Source Files

```cxx
// <[subsystem]/[module]/[source_name].cxx> -*- C++ -*-

//  Sapling 3D Game Engine
//  Copyright (C) 2026  Tyler Swann, Georgia Kannelis
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License v2.1 as published by the Free Software Foundation.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
//  USA

#include <> // corresponding header
#include <> // internal module headers

#include <> // other subsystem and module from engine

#include <> // third party headers

#include <> // standard headers

namespace sap::<subsystem>::<module> {

// ... source implementations ...

} // namespace sap::<subsystem>::<module>
```
