import { StandardMaterial, Color3 } from '@babylonjs/core';

const blueMat = () => {
  const blueMat = new StandardMaterial('blue');
  blueMat.diffuseColor = new Color3(0, 0, 1);

  return blueMat;
};

export { blueMat };
