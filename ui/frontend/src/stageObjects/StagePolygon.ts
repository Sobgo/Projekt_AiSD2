import paper from 'paper';
import { StageBaseObject } from "./StageBaseObject";
import type { ShapeOptions, Subset } from "./types";

export class StagePolygon extends StageBaseObject {
    declare drawOptions: Subset<ShapeOptions>;
    points: paper.Point[];

    constructor(position: paper.PointLike, points: paper.PointLike[], options?: Subset<ShapeOptions>) {
        super(position, options);
        this.points = points.map(point => new paper.Point(point));
    }

    draw() {
        const path = new paper.Path({ segments: this.points, closed: true });
        path.bounds.topLeft.add(this.position);

        if (this.drawOptions?.stroke_color) path.strokeColor = new paper.Color(this.drawOptions.stroke_color);
        if (this.drawOptions?.fill_color) path.fillColor = new paper.Color(this.drawOptions.fill_color);
        if (this.drawOptions?.stroke_width) path.strokeWidth = this.drawOptions.stroke_width;

        return path;
    }
}
