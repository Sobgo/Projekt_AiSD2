import paper from 'paper';

export class StageBaseObject {
    declare drawOptions: Record<string, any>;
    protected _position: paper.Point;

    set position(position: paper.PointLike) {
        this._position = new paper.Point(position);
    }

    get position(): paper.Point {
        return this._position;
    }

    constructor(position: paper.PointLike, options?: Record<string, any>) {
        this._position = new paper.Point(position);
        this.drawOptions = options ?? {};
    }

    draw() {
        return new paper.Item();
    }

    update(delta: any) {
        return;
    }
}
