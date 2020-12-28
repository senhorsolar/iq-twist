// Beware atrocious javascript

const COLS = 8;
const ROWS = 4;
//const BLOCK_SIZE = document.getElementById('gridd').offsetWidth / 9;
const BLOCK_SIZE = 40;

// Init Board
const canvas = document.getElementById('board');
const ctx = canvas.getContext('2d');

ctx.canvas.width = COLS * BLOCK_SIZE;
ctx.canvas.height = ROWS * BLOCK_SIZE;
ctx.scale(BLOCK_SIZE, BLOCK_SIZE)

// Same board, but for pegs
const pcanvas = document.getElementById('pboard');
const pctx = pcanvas.getContext('2d');

pctx.canvas.width = COLS * BLOCK_SIZE;
pctx.canvas.height = ROWS * BLOCK_SIZE;
pctx.scale(BLOCK_SIZE, BLOCK_SIZE)

// Init pegs
let pegs = document.getElementsByClassName("peg");
var x = 0;
for (var i = 0; i < pegs.length; i++) {
    let peg = pegs[i];
    let ctxPeg = peg.getContext("2d");
    ctxPeg.canvas.width = COLS * BLOCK_SIZE;
    ctxPeg.canvas.height = ROWS * BLOCK_SIZE;
    
    if (peg.id.includes("red") ) {
	ctxPeg.fillStyle = "red";
    }
    else if (peg.id.includes("blue")) {
	ctxPeg.fillStyle = "blue";
    }
    else if (peg.id.includes("green")) {
	ctxPeg.fillStyle = "green";
    }
    else if (peg.id.includes("yellow")) {
	ctxPeg.fillStyle = "yellow";
    }
    
    ctxPeg.beginPath();
    ctxPeg.arc(20, 10, 7, 0, 2 * Math.PI, false);
    ctxPeg.fill();
    ctxPeg.strokeStyle = 'black';
    ctxPeg.stroke();
}

var iqtwist = null;

var colorMapping = {
    'R': "red",
    'r': "red",
    'G': "green",
    'g': "green",
    'Y': "yellow",
    'y': "yellow",
    'B': "blue",
    'b': "blue"
};

let rowMapping = {
    "a": 0,
    "b": 1,
    "c": 2,
    "d": 3
};

onRuntimeInitialized: function placePegs() {
    
    console.log("placing pegs...");
    
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    pctx.clearRect(0, 0, pcanvas.width, pcanvas.height);
    
    if (iqtwist != null ) {
	iqtwist.delete();
	iqtwist = null;
    }
    
    const start = Date.now();
    
    var pegcmd = "";
    let pegcmds = document.getElementsByClassName("pegcmd");
    let pegSet = new Set();
    
    for (var i = 0; i < pegcmds.length; i++) {
	
	let cmd = pegcmds[i].value;
	
	if (cmd != "") {
	    console.assert(cmd.length == 2);
	    console.assert("abcd".includes(cmd[0].toLowerCase()));
	    console.assert("12345678".includes(cmd[1]));
	    
	    var color;
	    let pegId = pegcmds[i].id;
	    if (pegId.includes("red") ) {
		color = 'r';
	    }
	    else if (pegId.includes("blue")) {
		color = 'b';
	    }
	    else if (pegId.includes("green")) {
		color = 'g';
	    }
	    else if (pegId.includes("yellow")) {
		color = 'y';
	    }
	    
	    let row = rowMapping[cmd[0].toLowerCase()];
	    let col = parseInt(cmd[1]) - 1;
	    
	    pctx.fillStyle = colorMapping[color];
	    pctx.beginPath();
	    pctx.arc(col + 0.5, row + 0.5, 0.15, 0, 2 * Math.PI, false);
	    pctx.fill();
	    
	    pctx.strokeStyle = 'black';
	    pctx.lineWidth = 0.05;
	    pctx.stroke();
	    
	    var extraCmd = `-${color}${row}${col}`;
	    
	    console.assert(!pegSet.has(extraCmd), "Peg location already used")
	    pegSet.add(extraCmd);
	    
	    if (pegcmd == "") {
		pegcmd = extraCmd;
	    }
	    else {
		pegcmd += ` ${extraCmd}`;
	    }
	}
    }
    
    iqtwist = new Module.IqTwist(pegcmd);
    var nsolutions = iqtwist.search();
    
    document.getElementById("nsolutions").textContent = nsolutions;
    document.getElementById("solutionIdx").value = (nsolutions > 0) ? 1 : 0;
    
    console.log("Pegs placed");
    console.log(`-- nsolutions: ${nsolutions}, time: ${(Date.now() - start) / 1000}s`)
}

function prev() {
    if (iqtwist != null) {
	let solutionIdx = parseInt(document.getElementById("solutionIdx").value);

	if (solutionIdx > 1) {
	    document.getElementById("solutionIdx").value = solutionIdx - 1;
	    solve();
	}
    }
}

function next() {
    if (iqtwist != null) {
	let solutionIdx = parseInt(document.getElementById("solutionIdx").value);

	if (solutionIdx < iqtwist.solution_count()) {
	    document.getElementById("solutionIdx").value = solutionIdx + 1;
	    solve();
	}
    }
}

var added_buttons = false;

onRuntimeInitialized: function solve() {
    
    if (iqtwist == null) {
	placePegs()
    }

    if (added_buttons == false) {

	var solvingdiv = document.getElementById("solvingdiv");

	var nextButton = document.createElement("button");
	var prevButton = document.createElement("button");
	
	nextButton.onclick = next;
	nextButton.innerText = "Next";
	prevButton.onclick = prev;
	prevButton.innerText = "Prev";

	
	solvingdiv.appendChild(prevButton);
	solvingdiv.appendChild(nextButton);
		
	added_buttons = true;
	console.log("Placing buttons");
    }
    
    let solutionIdx = parseInt(document.getElementById("solutionIdx").value);
    
    console.assert(solutionIdx >= 1)
    console.assert(solutionIdx <= iqtwist.solution_count(), "select between " + 1 + " and " + iqtwist.solution_count())
    
    if ((iqtwist.solution_count() > 0) && (solutionIdx <= iqtwist.solution_count()) && (solutionIdx > 0)) {
	let solution = iqtwist.get_solution(solutionIdx - 1);
	
	console.log("Solution: " + solution);
	
	for (var i = 0; i < 4; i++) {
	    for (var j = 0; j < 8; j++) {
		ctx.fillStyle = colorMapping[solution.charAt(8*i + j)];
		ctx.fillRect(j, i, 1, 1);
		
		// borders
		ctx.fillStyle = 'black';
		
		// left border
		if (j > 0) {
		    if (solution.charAt(8*i + j) != solution.charAt(8*i + j - 1)) {
			ctx.fillRect(j, i, 0.05, 1);
		    }
		}
		// top border
		if (i > 0) {
		    if (solution.charAt(8*i + j) != solution.charAt(8*(i-1) + j)) {
			ctx.fillRect(j, i, 1, 0.05);
		    }
		}
	    }
	    
	}
    }
}
