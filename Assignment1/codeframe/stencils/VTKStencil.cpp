#include "VTKStencil.h"
#include <cstdlib>
#include <iostream>
#include <fstream>


VTKStencil::VTKStencil(const Parameters &parameters) : FieldStencil<FlowField>(parameters),
                                                       _prefix(_parameters.vtk.prefix) {}

// Stores all output in strings in the VTKStencil object. Prints all information with headers at once after iterating
// over the flowField
void VTKStencil::apply(FlowField &flowField, int i, int j) {

    const int obstacle = flowField.getFlags().getValue(i, j);
    int Nx = _parameters.parallel.localSize[0], Ny = _parameters.parallel.localSize[1];
    //    Identifies if current cell is a fluid cell, outputs 0 otherwise
    if (i > 1 && j > 1 && i < Nx + 3 && j < Ny + 3) {
        if ((obstacle & OBSTACLE_SELF) == 0) {
            FLOAT pressure;
            FLOAT velocity[2];
            flowField.getPressureAndVelocity(pressure, velocity, i, j);
            _pressureData = _pressureData + "\n" + std::to_string(pressure);
            _velocityData = _velocityData + "\n" + std::to_string(velocity[0]) + " " + std::to_string(velocity[1]) + " 0";
        } else {
            _pressureData = _pressureData + "0\n";
            _velocityData = _velocityData + "0 0 0\n";
        }
    }
}


void VTKStencil::apply(FlowField &flowField, int i, int j, int k) {
    const int obstacle = flowField.getFlags().getValue(i, j, k);
    int Nx = _parameters.parallel.localSize[0], Ny = _parameters.parallel.localSize[1], Nz = _parameters.parallel.localSize[2];
    //    Identifies if current cell is a fluid cell, outputs 0 otherwise
    if (i > 1 && j > 1 && k > 1 && i < Nx + 3 && j < Ny + 3 && k < Nz + 3) {
        if ((obstacle & OBSTACLE_SELF) == 0) {
            FLOAT pressure;
            FLOAT velocity[3];
            flowField.getPressureAndVelocity(pressure, velocity, i, j, k);
            _pressureData = _pressureData + std::to_string(pressure) + "\n";
            _velocityData = _velocityData + std::to_string(velocity[0]) + " " + std::to_string(velocity[1]) + " " +
                           std::to_string(velocity[2]) + "\n";
        } else {
            _pressureData = _pressureData + "0\n";
            _velocityData = _velocityData + "0 0 0\n";
        }
    }
}

void VTKStencil::write(FlowField &flowField, int timeStep) {
    std::ofstream vtkFile;
    int Nx = _parameters.parallel.localSize[0], Ny = _parameters.parallel.localSize[1], Nz = _parameters.parallel.localSize[2];
    int numProcesses = _parameters.parallel.numProcessors[0] * _parameters.parallel.numProcessors[1] *
                       _parameters.parallel.numProcessors[2];
    int _cellTotal = Nx * Ny * Nz;
    if (numProcesses > 1) {
        vtkFile.open(
                "./VTK/" + _prefix + "_" + std::to_string(_parameters.parallel.rank) + "_" + std::to_string(timeStep) +
                ".vtk",
                std::ios::app);
    } else {
        vtkFile.open("./VTK/" + _prefix + "_" + std::to_string(timeStep) +
                     ".vtk",
                     std::ios::app);
    }

    vtkFile << "\n\nCELL_DATA " + std::to_string(_cellTotal) + "\nSCALARS pressure float 1\nLOOKUP_TABLE default\n";
    vtkFile << _pressureData;
    vtkFile << "\n\nVECTORS velocity float\n";
    vtkFile << _velocityData;
    vtkFile.close();

    _pressureData = "";
    _velocityData = "";
    std::cout << "Printing VTK for timestep " << timeStep << std::endl;
}