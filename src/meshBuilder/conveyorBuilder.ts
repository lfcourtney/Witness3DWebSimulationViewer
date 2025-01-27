import {
  Vector3,
  Mesh,
  Texture,
  StandardMaterial,
  Path3D,
} from "@babylonjs/core";
import { PathTag, LineTag, ArcTag } from "../interfaces/pathTag";
import { blackMat } from "../utilities/colors";
import { line2D } from "../utilities/line2D";
import { ConveyorInformation } from "../interfaces/conveyorInformation";

/**
 * Represents the means by which a conveyor or path mesh is created from information contained within the <path> tag of the given <create> tag.
 */
export class ConveyorBuilder {
  /**
   * The name of the geometry model used to render the given conveyor or path as
   * specified in the models corresponding <create> tag
   */
  private readonly _geometryName: string;

  /**
   * <path> tag from a <create> tag used to create the conveyor or path
   */
  private readonly _pathTag: PathTag;

  /**
   * The information representing the shape of the rendered conveyor or path
   */
  private _conveyorInformation: ConveyorInformation;

  /**
   * Creates an instance of ConveyorBuilder class
   * @param pathTag <path> tag from a <create> tag used to create the conveyor or path
   * @param geometryName The name of the geometry model used to render the given conveyor or path as specified
   * in the models corresponding <create> tag
   */
  constructor(pathTag: PathTag, geometryName: string) {
    this._pathTag = pathTag;
    this._geometryName = geometryName;
    this._conveyorInformation = {
      conveyorPoints: [],
      conveyorTangents: [],
      conveyorBinormals: [],
    };
  }

  /**
   * Get the information representing the shape of the rendered conveyor or path
   */
  public get conveyorInformation() {
    return this._conveyorInformation;
  }

  /**
   * Use information from <path> tag to create a Mesh extrusion that draws shape of conveyor or path.
   * @return Mesh that represents conveyor or path. Otherwise, undefined if there was a problem creating such mesh
   */
  buildConveyor(): Mesh | undefined {
    // Array of Vector3 points that, collectively, represent the shape of the conveyor or path
    const conveyorPoints: Vector3[] = [];

    this._pathTag.path.forEach((lineOrArc) => {
      if (this.isLineTag(lineOrArc)) {
        const getLineTag = lineOrArc.line;

        const linePoints: Vector3[] = [
          // Start of <line>
          new Vector3(getLineTag.startX, getLineTag.startY, getLineTag.startZ),
          // End of <line>
          new Vector3(getLineTag.endX, getLineTag.endY, getLineTag.endZ),
        ];

        // Add points to overall conveyor array.
        conveyorPoints.push(...linePoints);

        return;
      }

      if (this.isArcTag(lineOrArc)) {
        //TODO: Deal with <arc> tags

        // const getArcTag = lineOrArc.arc;

        // const createArc = Curve3.CreateQuadraticBezier(
        //   new Vector3(getArcTag.startX, getArcTag.startY, getArcTag.startZ),
        //   new Vector3(getArcTag.centreX, getArcTag.centreY, getArcTag.centreZ),
        //   new Vector3(getArcTag.endX, getArcTag.endY, getArcTag.endZ),
        //   25,
        // );

        // conveyorPoints.push(...createArc.getPoints());

        return;
      }
    });

    this._conveyorInformation.conveyorPoints = conveyorPoints;

    /**
     * Make necessary adjustments to array of Vectors before they are used to render the shape of the path or conveyor
     */
    const modifiedConveyorPoints =
      this.modifyConveyorPointsRemoveTwistedLines();

    const conveyorPath = line2D("line", {
      path: modifiedConveyorPoints,
      width: 0.5,
      standardUV: false,
    });

    const conveyorPath3D = new Path3D(modifiedConveyorPoints);

    this._conveyorInformation.conveyorBinormals = conveyorPath3D.getBinormals();
    this._conveyorInformation.conveyorTangents = conveyorPath3D.getTangents();

    const conveyorMesh = this.formatLine(conveyorPath);

    if (!conveyorMesh) return undefined;

    return conveyorMesh;
  }

  /**
   * Format line mesh produced by 'line2D' utility function so that the mesh has the correct height and
   * texture to be the shape of a conveyor or path.
   * @param lineMesh Line mesh produced by 'line2D' utility function.
   * @returns Formatted mesh. Otherwise null if formatting was unsuccessful.
   */
  private formatLine(lineMesh: Mesh): Mesh | undefined {
    const thickLineMesh = this.increaseConveyorPathHeight(10, lineMesh);

    if (!thickLineMesh) return undefined;

    const thickLineMaterial = new StandardMaterial("thickLineMaterial");
    const thickLineMaterialTexture = new Texture(
      "https://raw.githubusercontent.com/lfcourtney/Witness3DWebSimulationViewerModels/main/Textures/" +
        this._geometryName +
        ".png",
      undefined,
      undefined,
      undefined,
      undefined,
      // onLoad
      () => {
        thickLineMaterial.diffuseTexture = thickLineMaterialTexture;
        thickLineMesh.material = thickLineMaterial;
      },
      // onError
      (errorMessage) => {
        console.error(errorMessage);
        // Use standard black material as an alternative if the texture failed to load
        thickLineMesh.material = blackMat();
      },
    );

    return thickLineMesh;
  }

  /**
   * Take line of fixed width that represents conveyor or path shape and duplicate this mesh by height value
   * so as to increase height thickness of the line. This is because the line, by itself, cannot have any height, only width.
   * @param heightValue The height value to apply to the line of fixed width.
   * @param conveyorPath The original Mesh that is the line of a fixed width representing conveyor or path shape.
   * @returns The new line of fixed width mesh once its height has been increased. Otherwise, null if there was a problem creating this Mesh.
   */
  private increaseConveyorPathHeight(
    heightValue: number,
    conveyorPath: Mesh,
  ): Mesh | null {
    const conveyorPathMeshes: Mesh[] = [conveyorPath];

    for (let i = 1; i <= heightValue; i++) {
      const clonedConveyorPath = conveyorPath.clone("line" + i);
      clonedConveyorPath.position.y = 0.01 * i;

      conveyorPathMeshes.push(clonedConveyorPath);
    }

    return Mesh.MergeMeshes(
      conveyorPathMeshes,
      true,
      false,
      undefined,
      false,
      true,
    );
  }

  /**
   * Checks if a path object from PathTag object contains information of a <line> tag
   * @param isLine path object from PathTag object
   * @returns True if object contains a <line> tag. False otherwise.
   */
  private isLineTag(isLine: object): isLine is { line: LineTag } {
    return isLine["line"] !== undefined;
  }

  /**
   * Checks if a path object from PathTag object contains information of a <arc> tag
   * @param isArc path object from PathTag object
   * @returns True if object contains a <arc> tag. False otherwise.
   */
  private isArcTag(isArc: object): isArc is { arc: ArcTag } {
    return isArc["arc"] !== undefined;
  }

  /**
   * Modify conveyor points so as to remove twisted lines that can exist for lines on X and Z axes.
   * @returns A modified version of the '_conveyorPoints' field so that such function can be passed into the Line2D utility function
   * to draw a line on the X and Z axes without any unnecessary twisting of lines when the line increases on the Y axis.
   * @see {@link https://forum.babylonjs.com/t/line-in-xoz-plane-utility-function-rendering-twisted-lines/56056|Line in XoZ Plane utility function rendering twisted lines}
   * for the Babylon.js forum post that describes the issue this method is designed to solve in addition to the solution that this method
   * is designed to implement.
   */
  private modifyConveyorPointsRemoveTwistedLines(): Vector3[] {
    // Create shallow copy of an array
    const newConveyorPoints: Vector3[] = [
      ...this._conveyorInformation.conveyorPoints,
    ];

    for (let i = 2; i < newConveyorPoints.length; i++) {
      const secondPriorConveyorPoint = newConveyorPoints[i - 2];
      const priorConveyorPoint = newConveyorPoints[i - 1];
      const currentConveyorPoint = newConveyorPoints[i];
      const diffZPosition = currentConveyorPoint.z - priorConveyorPoint.z;

      // True if 'z' position has not changed for the previous 2 Vector3's and
      // yet has changed for the current Vector3 and the one immediately prior to it. False otherwise.
      const hasZPositionChanged: boolean =
        secondPriorConveyorPoint.z === priorConveyorPoint.z &&
        currentConveyorPoint.z !== priorConveyorPoint.z;

      if (
        hasZPositionChanged &&
        /**
         * Only consider a change in 'z' value position if the 'x' value position has not changed between this Vector3 and the prior one.
         * Indeed, the given Line2D utility function is for creating lines on the X and Z axes, so such behaviour, by itself,
         * can cause problems for the function unless this method is used.
         */
        currentConveyorPoint.x === priorConveyorPoint.x
      ) {
        newConveyorPoints[i] = priorConveyorPoint.add(
          diffZPosition < 0 ? new Vector3(0, 0, -1) : new Vector3(0, 0, 1),
        );
      }
    }
    return newConveyorPoints;
  }
}
