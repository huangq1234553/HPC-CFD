#include "VTKStencil.h"
#include <cstdlib>
#include <iostream>
#include <fstream>


VTKStencil::VTKStencil(const Parameters &parameters) : FieldStencil<FlowField>(parameters),
                                                       prefix(_parameters.vtk.prefix) {}

void VTKStencil::apply(FlowField &flowField, int i, int j) {
    const int obstacle = flowField.getFlags().getValue(i, j);
    int _Nx = _parameters.parallel.localSize[0], _Ny = _parameters.parallel.localSize[1];
    if (i > 1 && j > 1 && i < _Nx + 3 && j < _Ny + 3) {
        if ((obstacle & OBSTACLE_SELF) == 0) {
            FLOAT pressure;
            FLOAT velocity[2];
            flowField.getPressureAndVelocity(pressure, velocity, i, j);
            pressureData = pressureData + "\n" + std::to_string(pressure);
            velocityData = velocityData + "\n" + std::to_string(velocity[0]) + " " + std::to_string(velocity[1]) + " 0";
        } else {
            pressureData = pressureData + "0\n";
            velocityData = velocityData + "0 0 0\n";
        }
    }
}


void VTKStencil::apply(FlowField &flowField, int i, int j, int k) {
    const int obstacle = flowField.getFlags().getValue(i, j, k);
    int _Nx = _parameters.parallel.localSize[0], _Ny = _parameters.parallel.localSize[1], _Nz = _parameters.parallel.localSize[2];
    if (i > 1 && j > 1 && k > 1 && i < _Nx + 3 && j < _Ny + 3 && k < _Nz + 3) {
        if ((obstacle & OBSTACLE_SELF) == 0) {
            FLOAT pressure;
            FLOAT velocity[3];
            flowField.getPressureAndVelocity(pressure, velocity, i, j, k);
            pressureData = pressureData + std::to_string(pressure) + "\n";
            velocityData = velocityData + std::to_string(velocity[0]) + " " + std::to_string(velocity[1]) + " " +
                           std::to_string(velocity[2]) + "\n";
        } else {
            pressureData = pressureData + "0\n";
            velocityData = velocityData + "0 0 0\n";
        }
    }
}

void VTKStencil::write(FlowField &flowField, int timeStep) {
    std::ofstream vtkFile;
    int _Nx = _parameters.parallel.localSize[0], _Ny = _parameters.parallel.localSize[1], _Nz = _parameters.parallel.localSize[2];
    int _cellTotal = _Nx * _Ny * _Nz;
    vtkFile.open("./VTK/" + prefix + "_" + std::to_string(_parameters.parallel.rank) + "_" + std::to_string(timeStep) +
                 ".vtk",
                 std::ios::app);
    vtkFile << "\n\nCELL_DATA " + std::to_string(_cellTotal) + "\nSCALARS pressure float 1\nLOOKUP_TABLE default\n";
    vtkFile << pressureData;
    vtkFile << "\n\nVECTORS velocity float\n";
    vtkFile << velocityData;
    vtkFile.close();

    pressureData = "";
    velocityData = "";
    std::cout << "Printing VTK for timestep " << timeStep << std::endl;
}