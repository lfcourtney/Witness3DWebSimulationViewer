import { Vector3, Mesh } from "@babylonjs/core";
import { PathTag, LineTag, ArcTag } from "../interfaces/pathTag";
import { blackMat } from "../utilities/colors";
import { line2D } from "../utilities/line2D";

/**
 * Represents the means by which a conveyor or path mesh is created from information contained within the <path> tag of the given <create> tag.
 */
export class ConveyorBuilder {
  /**
   * <path> tag from a <create> tag used to create the conveyor or path
   */
  private readonly _pathTag: PathTag;

  /**
   * The array of Vector3 points representing the conveyor or path mesh shape
   */
  private _conveyorPoints: Vector3[] = [];

  /**
   * Creates an instance of ConveyorBuilder class
   * @param pathTag <path> tag from a <create> tag used to create the conveyor or path
   */
  constructor(pathTag: PathTag) {
    this._pathTag = pathTag;
  }

  /**
   * Get the array of Vector3 points representing the conveyor or path mesh shape
   */
  public get conveyorPoints() {
    return this._conveyorPoints;
  }

  /**
   * Use information from <path> tag to create a Mesh extrusion that draws shape of conveyor or path.
   * @return Mesh that represents conveyor or path. Otherwise, undefined if there was a problem creating such mesh
   */
  buildConveyor(): Mesh | undefined {
    const conveyorPoints: Vector3[] = [];

    this._pathTag.path.forEach((lineOrArc) => {
      if (this.isLineTag(lineOrArc)) {
        const getLineTag = lineOrArc.line;

        conveyorPoints.push(
          // Start of <line>
          new Vector3(getLineTag.startX, getLineTag.startY, getLineTag.startZ),
          // End of <line>
          new Vector3(getLineTag.endX, getLineTag.endY, getLineTag.endZ),
        );

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

    this._conveyorPoints = conveyorPoints;

    const conveyorPath = line2D("line", { path: conveyorPoints, width: 0.5 });

    const conveyorMesh = this.increaseConveyorPathHeight(10, conveyorPath);

    if (!conveyorMesh) return undefined;

    conveyorMesh.material = blackMat();

    return conveyorMesh;
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
}
