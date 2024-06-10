
let canvas;
let context;
let points = [];
let hull = [];

class Point {
	constructor(x, y) {
		this.x = x;
		this.y = y;
	}
}

const addPoint = (event) => {
	var point = new Point(event.offsetX, event.offsetY);
	points.push(point);

	refresh();
}

const refresh = () => {
	// debug
	hull = points.map((point, index) => index);
	
	// update textarea
	var text = "";
	for (var i = 0; i < points.length; i++) {
		text += points[i].x + " " + points[i].y + "\n";
	}
	document.getElementById("points").value = text;

	// draw points and convex hull
	clearCanvas();
	
	drawPoints();
	drawConvexHull();
}

const drawPoints = () => {
	for (var i = 0; i < points.length; i++) {

		if (hull.includes(i)) {
			drawPoint(points[i], "red");
		} else {
			drawPoint(points[i]);
		}
	}
}

const drawPoint = (point, color) => {
	context.fillStyle = color ? color : "white";

	context.beginPath();
	context.arc(point.x, point.y, 3, 0, 2 * Math.PI);
	context.fill();
}

const drawConvexHull = () => {
	for (var i = 0; i < hull.length; i++) {
		drawLine(points[hull[i]], points[hull[(i + 1) % hull.length]], "red");
	}
}

const drawLine = (point1, point2, color) => {
	context.strokeStyle = color ? color : "white";

	context.beginPath();
	context.moveTo(point1.x, point1.y);
	context.lineTo(point2.x, point2.y);
	context.stroke();
}


const clearCanvas = () => {
	context.clearRect(0, 0, canvas.width, canvas.height);
}

const init = () => {
	canvas = document.getElementById("canvas");
	context = canvas.getContext("2d");
	canvas.addEventListener("click", addPoint);
}

const generate = () => {
	// pass points to fence logistics cli
}