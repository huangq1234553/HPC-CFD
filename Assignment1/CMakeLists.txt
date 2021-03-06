cmake_minimum_required(VERSION 3.12)
project(codeframe)

set(CMAKE_CXX_STANDARD 14)
list(APPEND CMAKE_MODULE_PATH "cmake-build-debug")
include_directories(parallelManagers)
include_directories(solvers)
include_directories(stencils)
include_directories(${PETSC_INCLUDES})
link_libraries(${PETSC_LIBRARIES})
#include_directories(/home/keefe/petsc-3.3-p7/arch-linux2-c-debug/include)
#include_directories(/home/keefe/petsc-3.3-p7/include)
include_directories(/usr/lib/x86_64-linux-gnu/openmpi/include)

add_executable(codeframe
        3rdparty/tinyxml2/tinyxml2.cpp
        3rdparty/tinyxml2/tinyxml2.h
        parallelManagers/PetscParallelConfiguration.cpp
        parallelManagers/PetscParallelConfiguration.h
        solvers/PetscSolver.cpp
        solvers/PetscSolver.h
        solvers/SORSolver.cpp
        solvers/SORSolver.h
        stencils/BFInputStencils.cpp
        stencils/BFInputStencils.h
        stencils/BFStepInitStencil.cpp
        stencils/BFStepInitStencil.h
        stencils/FGHStencil.cpp
        stencils/FGHStencil.h
        stencils/InitTaylorGreenFlowFieldStencil.h
        stencils/MaxUStencil.cpp
        stencils/MaxUStencil.h
        stencils/MovingWallStencils.cpp
        stencils/MovingWallStencils.h
        stencils/NeumannBoundaryStencils.cpp
        stencils/NeumannBoundaryStencils.h
        stencils/ObstacleStencil.cpp
        stencils/ObstacleStencil.h
        stencils/PeriodicBoundaryStencils.cpp
        stencils/PeriodicBoundaryStencils.h
        stencils/RHSStencil.cpp
        stencils/RHSStencil.h
        stencils/StencilFunctions.h
        stencils/VelocityStencil.cpp
        stencils/VelocityStencil.h
        stencils/VTKStencil.cpp
        stencils/VTKStencil.h
        Configuration.cpp
        Configuration.h
        DataStructures.cpp
        DataStructures.h
        Definitions.h
        FlowField.cpp
        FlowField.h
        GlobalBoundaryFactory.cpp
        GlobalBoundaryFactory.h
        Iterators.h
        LinearSolver.cpp
        LinearSolver.h
        main.cpp
        Meshsize.cpp
        Meshsize.h
        MeshsizeFactory.h
        Parameters.h
        SimpleTimer.cpp
        SimpleTimer.h
        Simulation.h
        Stencil.h)
