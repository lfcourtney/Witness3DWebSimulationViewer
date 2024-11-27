import { StandardMaterial, Color3 } from "@babylonjs/core";

const blueMat = () => {
  const blueMat = new StandardMaterial("blue");
  blueMat.diffuseColor = Color3.FromHexString("#005779");

  return blueMat;
};

export { blueMat };
