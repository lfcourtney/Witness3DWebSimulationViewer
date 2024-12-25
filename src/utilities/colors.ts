/**
 * @file Manages and stores the materials used by Witness 3D Web Simulation Viewer
 */

import { StandardMaterial, Color3 } from "@babylonjs/core";

const blueMat = () => {
  const blueMat = new StandardMaterial("blue");
  blueMat.diffuseColor = Color3.FromHexString("#005779");

  return blueMat;
};

const blackMat = () => {
  const blackMat = new StandardMaterial("black");
  blackMat.diffuseColor = new Color3(0, 0, 0);
  blackMat.specularColor = new Color3(0, 0, 0);

  return blackMat;
};

export { blueMat, blackMat };
