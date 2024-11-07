import { TranslateTag } from "./translateTag";
import { ScaleTag } from "./scaleTag";
import { RotateTag } from "./rotateTag";
/**
 * Represents a formatted <update> tag
 */
export interface UpdateTag {
  time: number;
  instanceName: string;
  visible?: boolean;
  translate?: TranslateTag;
  rotate?: RotateTag;
  scale?: ScaleTag;
}
