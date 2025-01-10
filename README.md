# Witness3DWebSimulationViewer

## Overview

Witness 3D Web Simulation Viewer is a 3D web app that can show simulations running over time.

The simulations will come from [Witness](https://www.lanner.com/en-gb/technology/witness-simulation-software.html). Witness is a discrete event simulation modelling environment allowing domain experts to create simulation models of systems. Witness runs on Windows desktop. Witness has an option to create a 3D view from positional information in the 2D view. From the 3D visualisations in Witness, a detailed simulation trace output can be captured that describes all the 3D movements in the simulation over time.

The project is to use the simulation trace output to visualise these simulations running in 3D on the web. Because such visualisation capabilities come built into Witness, this project is a proof-of-concept demonstrating how feasible it is to shift these visualisation capabilities onto the browser and, through this, identify the costs and potential limitations.

By shifting the visualisation of simulations onto the browser, the intended benefit is to access opportunities for real-time collaboration by visualising the simulation from the same trace output file; the ability to visualise simulations will no longer be restricted to Windows desktops with an installation of Witness.

There are different approaches that can be taken to achieve these aims (eg, WebGL vs. WebGPU). The project will compare the different approaches and justify the approaches it takes.

**Demonstration of Witness Desktop 3D Visualisation Capabilities**
![Demonstration of Witness Desktop 3D Visualisation Capabilities](./WitnessImages/demonstration_of_witness_desktop_3d_visualisation_capabilities.gif "Demonstration of Witness Desktop 3D Visualisation Capabilities")

## Technologies

Babylon.js, Vite and TypeScript will be used to create the web application. The official documentation for this approach can be found [here](https://doc.babylonjs.com/guidedLearning/usingVite).

Babylon.js, which is an open-source web rendering engine, will utilise the WebGPU engine.

# Getting Started

## Prerequisites

- Clone the GitHub repo, and run `npm i` to install Babylon.js, Vite and the other npm dependencies.
- Run `npm run dev` to begin a development server running the code. The server will begin on port `4000`.
- To stop the server, go back to the terminal and press Ctrl+C.

## Installing

- Run `npm run build`. This will bundle the application, outputting this bundle to the `dist` folder within the root directory, creating this directory if it does not already exist.
- Run `npm run preview` to see the exported code bundle running in a local development server.
- Now the contents of the `dist` folder can be uploaded to an external server, deploying the application.

## Husky pre-commit hook

`.husky\pre-commit` contains the contents of a pre-commit Git hook as per the [Husky](https://typicode.github.io/husky/) standard. This script will thus run each time you run `git commit`. Along with linting staged files to enforce code style, the script will run a Node.js script to automatically update the patch version number as held within `package.json`. Such updated `package.json` file will automatically be added to the outgoing commit.

The `--no-verify` (shortened to -n) Git commit flag can bypass the pre-commit hook. Therefore, run `git commit -n` to make a commit without updating the version number. **Important**: this should be used sparingly, only for commits involving very trivial changes.

## Debug

Witness 3D Web Simulation Viewer is developed and debugged in Visual Studio Code. It is recommended to use Visual Studio Code for debugging due to its built-in debugging capabilities.

To do open a debugging environment in Visual Studio Code, open a `JavaScript Debug Terminal` and run `npm run dev` to begin the development server as usual. From here, the application will stop at any breakpoints set in Visual Studio Code.

Alternatively, many web browsers offer debugging capabilities that can be used for debugging Witness 3D Web Simulation Viewer.

# Running the tests

Witness 3D Web Simulation Viewer uses the Vitest testing library, created by the same team as the Vite build tool and comes with Vite integration, to create unit tests.

Test configuration is located in the `vitest.config.ts` file, which is also where the Vite configuration is located.

## Run Tests

- Run `npm test`. This will initiate a single run of the unit tests.

## Code Coverage

A successful test run will produce code coverage. The directory for code coverage is `coverage`, which is ignored by Git.

## Debug Tests

A run of Vitest unit tests can be debugged in VS Code. To do this, open a `JavaScript Debug Terminal` and run `npm run test`. For more information, go to the Vitest [official documentation](https://vitest.dev/guide/debugging).

# Important Links & Docs

- [3D Command Reference 21](./WitnessDocs/3D%20Command%20Reference%2021.pdf)
- [Part Queuing Positions Overview 01](./WitnessDocs/Part%20Queuing%20Positions%20Overview%2001.pdf)
- [The WebGPU Sourcebook](./WitnessDocs/The%20WebGPU%20Sourcebook.pdf)
- [Witness W3D Queue Implementation](./WitnessDocs/Witness%20W3D%20Queue%20Implementation.pdf)
- [WITNESS-Unity Proof of Concept 01](./WitnessDocs/WITNESS-Unity%20Proof%20of%20Concept%2001.pdf)

# Last Updated

This README was last updated on 10 January 2024.
