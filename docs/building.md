# Building

Instructions on how to build, develop and install Sapling Engine.

> [!NOTES]
> Managing external dependencies
>
> For the fetching of external dependencies for developing the engine, it is preferred to
> use packages or binaries already built for the target development platform when
> available instead of requiring people to build from source.
>
> This reduces the cognative overhead for new contributors or setting up a development
> env. Anything not provided by a package manager will need to be built with the project
> and for this we will use CMake's FetchContent module.
>
> If versioning becomes an issue we'll move to a config fully managed by FetchContent.
> We'll provide instructions for building each dependency (except the Vulkan SDK) from
> source should it be needed.
>
> The only outlier is the Vulkan SDK which will require manual intervention on all
> platforms but this cannot be avoided without massive maintenence effort to ensure all
> SDK modules are compatible and available. We'll provide clear instrucitons for it in
> the building from source guide.
>
> The ultimate goal of the project is have the engine distributed as a binary anyway so
> this should not affect engine users, only engine maintainers.

## Perquisites

* lib1
* lib1

### Linux

#### Linux Specific Libraries

ABC

ABC

### macOS

ABC

### Windows

ABC

### Helper Scripts

ABC

## Build the Engine

```sh
cmake -B build
```

## Install Engine

```sh
# install
```

