import paper from 'paper';
import type { ShapeOptions, Subset } from "./types";
import { StageBaseObject } from "./StageBaseObject";

export class StageEllipse extends StageBaseObject {
    declare drawOptions: Subset<ShapeOptions>;
    width: number;
    height: number;

    constructor(position: paper.PointLike, width: number, height: number, options?: Subset<ShapeOptions>) {
        super(position, options);
        this.width = width;
        this.height = height;
    }

    draw() {
        const path = new paper.Path.Ellipse({
            point: this.position, 
            size: new paper.Size(this.width, this.height)
        });

        if (this.drawOptions?.stroke_color) path.strokeColor = new paper.Color(this.drawOptions.stroke_color);
        if (this.drawOptions?.fill_color) path.fillColor = new paper.Color(this.drawOptions.fill_color);
        if (this.drawOptions?.stroke_width) path.strokeWidth = this.drawOptions.stroke_width;

        return path;
    }
}
