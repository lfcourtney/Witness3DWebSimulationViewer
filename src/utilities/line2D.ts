/**
 * @file Manages in TypeScript format a utility function from the Babylon.js documentation to draw a line with a set width on the X and Z axes
 */

import { Vector3, Mesh, VertexData } from "@babylonjs/core";

/**
 * Babylon.js utility function for creating a line Mesh with a set width on the X and Z axes.
 * @param name Name of the line Mesh
 * @param options Options for the line Mesh
 * @returns Line Mesh with a set width on the X and Z axes
 * @see {@link https://doc.babylonjs.com/toolsAndResources/utilities/Line2D|Draw a Line With a Set Width} for source.
 * Specifically, look to 'Line in XoZ Plane Formed from Central Path' header.
 */
export const line2D = (
  name: string,
  options: {
    width?: number;
    path: Vector3[];
    closed?: boolean;
    standardUV?: boolean;
  },
): Mesh => {
  // Arrays for vertex positions and indices
  const positions: number[] = [];
  const indices: number[] = [];
  const normals: number[] = [];

  const width = options.width ? options.width / 2 : 0.5;
  const path = options.path;
  const closed = options.closed || false;
  const standardUV =
    options.standardUV === undefined ? true : options.standardUV;

  let angle = 0;

  const nbPoints = path.length;
  const line = Vector3.Zero();
  const nextLine = Vector3.Zero();

  const innerData: Vector3[] = [];
  const outerData: Vector3[] = [];

  path[1].subtractToRef(path[0], line);

  if (nbPoints > 2 && closed) {
    path[2].subtractToRef(path[1], nextLine);
    for (let p = 0; p < nbPoints; p++) {
      angle =
        Math.PI -
        Math.acos(
          Vector3.Dot(line, nextLine) / (line.length() * nextLine.length()),
        );
      const direction = Vector3.Cross(line, nextLine).normalize().y;
      const lineNormal = new Vector3(-line.z, 0, line.x).normalize();
      line.normalize();
      innerData[(p + 1) % nbPoints] = path[(p + 1) % nbPoints]
        .subtract(lineNormal.scale(width))
        .subtract(line.scale((direction * width) / Math.tan(angle / 2)));
      outerData[(p + 1) % nbPoints] = path[(p + 1) % nbPoints]
        .add(lineNormal.scale(width))
        .add(line.scale((direction * width) / Math.tan(angle / 2)));
      line.copyFrom(nextLine);
      path[(p + 3) % nbPoints].subtractToRef(
        path[(p + 2) % nbPoints],
        nextLine,
      );
    }
  } else {
    let lineNormal = new Vector3(-line.z, 0, line.x).normalize();
    line.normalize();
    innerData[0] = path[0].subtract(lineNormal.scale(width));
    outerData[0] = path[0].add(lineNormal.scale(width));

    for (let p = 0; p < nbPoints - 2; p++) {
      path[p + 2].subtractToRef(path[p + 1], nextLine);
      angle =
        Math.PI -
        Math.acos(
          Vector3.Dot(line, nextLine) / (line.length() * nextLine.length()),
        );
      const direction = Vector3.Cross(line, nextLine).normalize().y;
      lineNormal = new Vector3(-line.z, 0, line.x).normalize();
      line.normalize();
      innerData[p + 1] = path[p + 1]
        .subtract(lineNormal.scale(width))
        .subtract(line.scale((direction * width) / Math.tan(angle / 2)));
      outerData[p + 1] = path[p + 1]
        .add(lineNormal.scale(width))
        .add(line.scale((direction * width) / Math.tan(angle / 2)));
      line.copyFrom(nextLine);
    }
    if (nbPoints > 2) {
      path[nbPoints - 1].subtractToRef(path[nbPoints - 2], line);
      lineNormal = new Vector3(-line.z, 0, line.x).normalize();
      line.normalize();
      innerData[nbPoints - 1] = path[nbPoints - 1].subtract(
        lineNormal.scale(width),
      );
      outerData[nbPoints - 1] = path[nbPoints - 1].add(lineNormal.scale(width));
    } else {
      innerData[1] = path[1].subtract(lineNormal.scale(width));
      outerData[1] = path[1].add(lineNormal.scale(width));
    }
  }

  let maxX = Number.MIN_VALUE;
  let minX = Number.MAX_VALUE;
  let maxZ = Number.MIN_VALUE;
  let minZ = Number.MAX_VALUE;

  for (let p = 0; p < nbPoints; p++) {
    positions.push(innerData[p].x, innerData[p].y, innerData[p].z);
    maxX = Math.max(innerData[p].x, maxX);
    minX = Math.min(innerData[p].x, minX);
    maxZ = Math.max(innerData[p].z, maxZ);
    minZ = Math.min(innerData[p].z, minZ);
  }

  for (let p = 0; p < nbPoints; p++) {
    positions.push(outerData[p].x, outerData[p].y, outerData[p].z);
    maxX = Math.max(innerData[p].x, maxX);
    minX = Math.min(innerData[p].x, minX);
    maxZ = Math.max(innerData[p].z, maxZ);
    minZ = Math.min(innerData[p].z, minZ);
  }

  for (let i = 0; i < nbPoints - 1; i++) {
    indices.push(i, i + 1, nbPoints + i + 1);
    indices.push(i, nbPoints + i + 1, nbPoints + i);
  }

  if (nbPoints > 2 && closed) {
    indices.push(nbPoints - 1, 0, nbPoints);
    indices.push(nbPoints - 1, nbPoints, 2 * nbPoints - 1);
  }

  const uvs: number[] = [];

  if (standardUV) {
    for (let p = 0; p < positions.length; p += 3) {
      uvs.push(
        (positions[p] - minX) / (maxX - minX),
        (positions[p + 2] - minZ) / (maxZ - minZ),
      );
    }
  } else {
    // Alternative UV mapping logic
    // Implement here if needed
  }

  VertexData.ComputeNormals(positions, indices, normals);
  VertexData._ComputeSides(Mesh.DOUBLESIDE, positions, indices, normals, uvs);

  // Create a custom mesh
  const customMesh = new Mesh(name);

  // Create a vertexData object
  const vertexData = new VertexData();

  // Assign positions and indices to vertexData
  vertexData.positions = positions;
  vertexData.indices = indices;
  vertexData.normals = normals;
  vertexData.uvs = uvs;

  // Apply vertexData to custom mesh
  vertexData.applyToMesh(customMesh);

  return customMesh;
};
