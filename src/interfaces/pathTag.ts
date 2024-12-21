/**
 * Represents a formatted <line> tag, which is exclusively a sub tag of a <path> tag
 */
export interface LineTag {
  endX: number;
  endY: number;
  endZ: number;
  startX: number;
  startY: number;
  startZ: number;
}

/**
 * Represents a formatted <arc> tag, which is exclusively a sub tag of a <path> tag
 */
export interface ArcTag {
  angle: number;
  centerX: number;
  centerY: number;
  centerZ: number;
  endX: number;
  endY: number;
  endZ: number;
  startX: number;
  startY: number;
  startZ: number;
  sweepDirection: "clockwise" | "counterclockwise";
}

/**
 * Represents a formatted <path> tag
 */
export interface PathTag {
  path: Array<{ line: LineTag } | { arc: ArcTag }>;
  startX: number;
  startY: number;
  startZ: number;
  width: number;
}
