<template>
    <div class="center">
        <div id="left_panel">
            <div class="controlPanel">
                <p class="label">Przesuwanie (tylko wizualnie):</p>
                <div class="mode_selector">
                    <button class ="panel_button mode_button" id="off"  @click='toggleAndCompute("off")'>OFF</button>
                    <button class="panel_button mode_button" id="on" @click='toggleAndCompute("on")'>ON</button>
                </div>
            </div>
            <p class="label" style="margin-top: 20px;">Widoczność:</p>
            <div class="mode_selector">
                <button class="panel_button option_button selected" id="hullbtn" @click='toggleAndCompute("hullbtn")'>płot</button>
                <button class="panel_button option_button" id="inbtn" @click='toggleAndCompute("inbtn")'>wejście</button>
            </div>
            <div class="mode_selector">
                <button class="panel_button option_button selected" id="pathbtn" @click='toggleAndCompute("pathbtn")'>ścieżki</button>
                <button class="panel_button option_button selected" id="weightbtn" @click='toggleAndCompute("weightbtn")'>wagi</button>
            </div>
            <p class="label" style="margin-top: 20px;">Opcje:</p>
            <button class="panel_button" id="reset_button" @click="computeAll">Oblicz</button>
            <button class="panel_button" id="draw_button" @click='download("svg")'>Pobierz jako SVG</button>
            <!-- <button class="panel_button" id="draw_button" @click='download("pdf")'>Pobierz jako PDF</button> -->
        </div>
        <div id="canvas_wrapper">
            <Stage ref="stage"/>
        </div>
        <div id="right_panel">
            <p class="label" style="margin-top: 10px;">Input:</p>
            <textarea ref="input" id="input_area"></textarea>
        </div> 
    </div>
</template>

<script lang="ts">
    import { defineComponent } from "vue";
    import paper from "paper";
    import axios from "axios";

    import Stage from "./components/Stage.vue";
    import { StageLabel } from "./stageObjects/StageLabel";
    import { StageGraph } from "./stageObjects/StageGraph";

    type ResType = {
        execCode: number | null;
        stdout: string;
        stderr: string;
    };

    const parse_input = (lines: string[]) => {
        let vertices: { x: number, y: number }[] = [];
        let edges: [number, number, boolean, string][] = [];
        let edgeLabels: string[] = [];
        
        let i = 0;
        while (lines[i].split(" ").length > 1) {
            const [x, y] = lines[i].split(" ");
            vertices.push({ x: parseFloat(x), y: parseFloat(y) });
            i++;
        }

        let start: string =  "";

        if (i < lines.length) {
            start = lines[i].split(" ")[0];
        }
        i++;

        while (i < lines.length - 1) {
            const [v1, v2] = lines[i].split(" ");
            edges.push([parseInt(v1)-1, parseInt(v2)-1, true, "red"]);
            
            // distance from v1 to v2
            const dist = Math.sqrt(
                Math.pow(vertices[parseInt(v1)-1].x - vertices[parseInt(v2)-1].x, 2) +
                Math.pow(vertices[parseInt(v1)-1].y - vertices[parseInt(v2)-1].y, 2)
            ).toFixed(2);

            edgeLabels.push(dist);
            i++;
        }

        return [vertices, edges, edgeLabels];
    };

    export default defineComponent({
        components: { Stage },

        data: () => ({ 
            dragging: false,
            graph: null
        }),

        methods: {
            async computeAll() {
                paper.view.center = new paper.Point(250, 250);
                paper.view.zoom = 1;

                const stage = this.$refs.stage as any;
                stage.clearObjects();

                const enable_dragging = document.getElementById("on")?.classList.contains("selected");

                // parse textarea input
                const input = this.$refs.input as any;
            
                const [points, inputEdges, inputLabels] = parse_input(input.value.split("\n"));
                const labels = points.map((_, i) => (i+1).toString());

                // call fence logistics lib

                const response = await axios.get<ResType>(`http://localhost:8081/fence_logistics`, {
		            params: { input: input.value }
	            });

                if (response.data.execCode !== 0) {
                    console.log("Error while executing the code");
                    console.log(response.data.stderr);
                    return;
                }

                console.log(response.data.stdout);

                const out = response.data.stdout.split("\n").filter((line) => line.length > 0);
                const convexHullLen = parseFloat(out[0]);
                const convexHullPoints = out[1].split(" ").map((x) => parseInt(x));
                const convexHullEdges = convexHullPoints.map(
                    (_, i) => [convexHullPoints[i]-1, convexHullPoints[(i+1)%convexHullPoints.length]-1, false, "white"]
                );
                const convexHullLabels = convexHullPoints.map((_) => '');

                // display
                stage.addObject(new StageLabel(
                    { x: 10, y: 10 },
                    "Problem Budowy Płotu:",
                    { font_size: 16, font_color: "white", align: "top", outline_color: "black"}
                ), enable_dragging);

                stage.addObject(new StageLabel(
                    { x: 30, y: 30 },
                    "Długość płotu: " + convexHullLen.toString(),
                    { font_size: 16, font_color: "white", align: "top", outline_color: "black"}
                ), enable_dragging);

                stage.addObject(new StageLabel(
                    { x: 30, y: 50 },
                    "Punkty w płocie: " + out[1],
                    { font_size: 16, font_color: "white", align: "top", outline_color: "black"}
                ), enable_dragging);

                out.shift();
                out.shift();

                const paths: [number, number, boolean, string][] = [];
                const pathLabels: string[] = [];

                for (let i = 0; i < out.length; i++) {
                    const pointsPath = out[i].split(" ");
                   
                    pointsPath.shift();
                    pointsPath.shift();

                    for (let j = 0; j < pointsPath.length - 1; j++) {
                        paths.push([parseInt(pointsPath[j])-1, parseInt(pointsPath[j+1])-1, true, "green"]);
                        pathLabels.push(Math.sqrt(
                            Math.pow(points[parseInt(pointsPath[j])-1].x - points[parseInt(pointsPath[j+1])-1].x, 2) +
                            Math.pow(points[parseInt(pointsPath[j])-1].y - points[parseInt(pointsPath[j+1])-1].y, 2)

                        ).toFixed(2));
                    }
                }

                const showHull = document.getElementById("hullbtn")?.classList.contains("selected");
                const showIn = document.getElementById("inbtn")?.classList.contains("selected");
                const showPath = document.getElementById("pathbtn")?.classList.contains("selected");
                const showWeight = document.getElementById("weightbtn")?.classList.contains("selected");

                const edges = [showPath ? paths : [], showIn ? inputEdges : [], showHull ? convexHullEdges : []].flat();
                
                const edgeLabels = showWeight ?
                    [showPath ? pathLabels : [], showIn ? inputLabels : [], showHull ? convexHullLabels : []].flat() :
                edges.map((_) => "");

                this.graph = new StageGraph(
                    { x: 230, y: 230 },
                    {
                        vertices: points,
                        vertexValues: labels,
                        edges: edges,
                        edgeValues: edgeLabels,
                    },
                    {
                        dragable: enable_dragging,
                    }
                );

                stage.addObject(this.graph, false);

                this.update();
            },

            update() {
                const stage = this.$refs.stage as any;
                stage.updateCanvas();
            },

            download(type: "svg" | "pdf") {
                const stage = this.$refs.stage as any;
                stage.download(type);
            },

            toggle(id: string) {
                const stage = this.$refs.stage as any;

                if (id === "on" || id ==="off") {
                    // remove clas from previous selected button
                    const buttons = document.getElementsByClassName("mode_button");

                    for (let i = 0; i < buttons.length; ++i) {
                        buttons[i].classList.remove("selected");
                    }

                    // add class to the selected button
                    document.getElementById(id)?.classList.add("selected");
                    stage.enableDragging(id === "on");
                } else {
                    const button = document.getElementById(id) as any;
                    button.classList.toggle("selected");
                }
            },

            toggleAndCompute(id: string) {
                this.toggle(id);
                this.computeAll();
            }
        },

        updated() {
            this.update();
        },

        mounted() {
            const input = this.$refs.input as any;
            input.value = "0 0\n0 -100\n-100 100\n100 100\n0 100\n2\n2 1\n1 3\n3 5\n5 3\n4 5\n";

            this.toggle("off");
            this.computeAll();
        }
    });
</script>

<style scoped>
    .center {
        width: 100vw;
        height: 100vh;
        background-color: #181818;
        display: flex;
        justify-content: center;
        align-items: center;
    }

    .controlPanel {
        display: flex;
        flex-direction: column;
        align-items: center;
        margin-top: 10px;
    }

    .mode_selector {
        width: 200px;
        display: flex;
        flex-direction: row;
        align-items: center;
        justify-content: space-around;
    }

    .selected {
        background-color: #bbbbbb!important;
        color: black !important;
    }

    .mode_button {
        width: 90px !important;
    }

    .option_button {
        width: 90px !important;
    }

    .label {
        color: white;
        margin-top: 0px;
        margin-bottom: 0px;
    }

    #left_panel {
        display: flex;
        flex-direction: column;
        align-items: center;
        height: 500px;
        width: 250px;
        background-color: #202020;
        border-right: none;
        border-radius: 4px 0 0 4px;
        border: 1px solid #bbbbbb;
    }

    #right_panel {
        display: flex;
        flex-direction: column;
        align-items: center;
        height: 500px;
        width: 250px;
        background-color: #202020;
        border-left: none;
        border-radius: 0 4px 4px 0;
        border: 1px solid #bbbbbb;
    }

    #input_area {
        resize: none;
        width: 230px;
        height: 450px;
        margin-top: 10px;
        background-color: #303030;
        border: 1px solid #bbbbbb;
        border-radius: 0;
        color: white;
    }

    #canvas_wrapper {
        border-radius: 0 4px 4px 0;
        border: 1px solid #bbbbbb;
        border-left: none;
        width: 500px;
        height: 500px;
    }

    .panel_button {
        border: 1px solid #bbbbbb;
        margin-top: 10px;
        width: 200px;
        height: 50px;
        color:#bbbbbb;
        font-size: 20px;
        background-color: #181818;
        border-radius: 4px;
        display: flex;
        justify-content: center;
        align-items: center;
    }

    .panel_button:hover {
        background-color: #303030;
        cursor: pointer;
    }

    .panel_button:active {
        transform: translateY(1px);
    }
</style>
