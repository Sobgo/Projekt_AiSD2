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
            <p class="label" style="margin-top: 20px;">Opcje:</p>
            <button class="panel_button" id="reset_button" @click="computeAll">Oblicz</button>
            <button class="panel_button" id="draw_button" @click='download("svg")'>Pobierz jako SVG</button>
            <button class="panel_button" id="draw_button" @click='download("pdf")'>Pobierz jako PDF</button>
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
        let edges: [number, number][] = [];
        
        let i = 0;
        while (lines[i].split(" ").length > 1) {
            const [x, y] = lines[i].split(" ");
            vertices.push({ x: parseInt(x), y: parseInt(y) });
            i++;
        }

        let start: string =  "";

        i++
        if (i < lines.length) {
            start = lines[i].split(" ")[0];
        }
        i++;

        while (i < lines.length - 1) {
            const [v1, v2] = lines[i].split(" ");
            edges.push([parseInt(v1)-1, parseInt(v2)-1]);
            i++;
        }

        return [vertices, edges, start];
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

                // problem label
                stage.addObject(new StageLabel(
                    { x: 10, y: 10 },
                    "Problem Budowy Płotu:",
                    { font_size: 16, font_color: "white", align: "top", outline_color: "black"}
                ), enable_dragging);

                // parse textarea input
                const input = this.$refs.input as any;
               
                const [points, edges] = parse_input(input.value.split("\n"));
                const labels = points.map((_, i) => i.toString());
                const edgeLabels = edges.map(([_v1, _v2]) => "");

                // call fence logistics lib

                const response = await axios.get<ResType>(`http://localhost:8081/fence_logistics`, {
		            params: { input: input.value }
	            });

                // parse output TODO

                const out = response.data.stdout.split("\n");
                console.log(out);
               
                // display

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

                // remove clas from previous selected button
                const buttons = document.getElementsByClassName("mode_button");

                for (let i = 0; i < buttons.length; ++i) {
                    buttons[i].classList.remove("selected");
                }

                // add class to the selected button
                document.getElementById(id)?.classList.add("selected");
                stage.enableDragging(id === "on");
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
            input.value = "0 0\n0 -100\n-100 100\n100 100\n0 100\n\n2\n2 1\n1 3\n3 5\n5 3\n4 5\n";

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
