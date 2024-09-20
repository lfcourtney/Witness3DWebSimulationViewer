# Witness3DWebSimulationViewer

## Overview

Witness 3D Web Simulation Viewer is a 3D web app that can show simulations running over time.

The simulations will come from [Witness](https://www.lanner.com/en-gb/technology/witness-simulation-software.html). Witness is a discrete event simulation modelling environment allowing domain experts to create simulation models of systems. Witness runs on Windows desktop. Witness has an option to create a 3D view from positional information in the 2D view. From the 3D visualisations in Witness, a detailed simulation trace output can be captured that describes all the 3D movements in the simulation over time.

The project is to use the simulation trace output to visualise these simulations running in 3D on the web. Because such visualisation capabilities come built into Witness, this project is a proof-of-concept demonstrating how feasible it is to shift these visualisation capabilities onto the browser and, through this, identify the costs and potential limitations.

By shifting the visualisation of simulations onto the browser, the intended benefit is to access opportunities for real-time collaboration by visualising the simulation from the same trace output file; the ability to visualise simulations will no longer be restricted to Windows desktops with an installation of Witness.

There are different approaches that can be taken to achieve these aims (eg, WebGL vs. WebGPU). The project will compare the different approaches and justify the approaches it takes.

**Demonstration of Witness Desktop 3D Visualisation Capabilities**
![Demonstration of Witness Desktop 3D Visualisation Capabilities](./WitnessImages/demonstration_of_witness_desktop_3d_visualisation_capabilities.gif 'Demonstration of Witness Desktop 3D Visualisation Capabilities')

## Technologies

Dawn, a WebGPU implementation, will be used to create a C++ codebase, using WebGPU to render the 3D shapes. This will be compiled into WebAssembly using emscripten, and the WebAssembly will be run through the browser.

## Important Links & Docs

- [3D Command Reference 21](./WitnessDocs/3D%20Command%20Reference%2021.pdf)
- [Part Queuing Positions Overview 01](./WitnessDocs/Part%20Queuing%20Positions%20Overview%2001.pdf)
- [Witness W3D Queue Implementation](./WitnessDocs/Witness%20W3D%20Queue%20Implementation.pdf)
- [WITNESS-Unity Proof of Concept 01](./WitnessDocs/WITNESS-Unity%20Proof%20of%20Concept%2001.pdf)

## Last Updated

This README was last updated on 20 September 2024.
