import paper from 'paper';
import type { ShapeOptions, Subset } from "./types";
import { StageBaseObject } from "./StageBaseObject";

export class StageRect extends StageBaseObject {
    declare drawOptions: Subset<ShapeOptions>;
    width: number;
    height: number;

    constructor(position: paper.PointLike, width: number, height: number, options?: Subset<ShapeOptions>) {
        super(position, options);
        this.width = width;
        this.height = height;
    }

    draw() {
        const value = new paper.Path.Rectangle({
            point: this.position,
            size: new paper.Size(this.width, this.height)
        });

        if (this.drawOptions?.stroke_color) value.strokeColor = new paper.Color(this.drawOptions.stroke_color);
        if (this.drawOptions?.fill_color) value.fillColor = new paper.Color(this.drawOptions.fill_color);
        if (this.drawOptions?.stroke_width) value.strokeWidth = this.drawOptions.stroke_width;

        return value;
    }
}
