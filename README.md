# Witness3DWebSimulationViewer
Witness 3D Web Simulation Viewer is a 3D web visualisation solution that can show simulations running over time on the web.

The simulations will come from Witness. Witness is a discrete event simulation modelling environment allowing domain experts to create simulation models of systems. Witness runs on Windows desktop. Witness has an option to create a 3D view from positional information in the 2D view. Here, a detailed simulation trace output can be captured that describes all the 3D movements in the simulation over time.

The project is to use the simulation trace output to visualise these simulations running in 3D on the web, with the intention being to explore opportunities for real-time collaboration by rendering from the 3D simulation view from the same trace output file. Additionally, the intention is to explore non desktop bound views of the simulation.

Dawn, a WebGPU implementation, will be used to create a C++ codebase, using WebGPU to render the 3D shapes. This will be compiled into WebAssembly using emscripten, and the WebAssembly will be run through the browser.
