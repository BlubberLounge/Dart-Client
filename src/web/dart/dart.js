const d = document;
var initialized = false;
const req = new CRequest();
var display = [];
var ws;
var lastUpdate;


d.addEventListener('DOMContentLoaded', function ()
{
    const btnList = d.querySelectorAll('#numpad button[data-bl-value]');
    [...btnList].map(e => {
        e.addEventListener('click', ev => {
            addToDisplay(e.getAttribute('data-bl-value'));
            updateDisplay();
        });
    });

    d.getElementById('enter').addEventListener('click', e => {


        let multiplier = 1;
        let ring = "";
        let field = 0;
        if(display.includes("D")) {
            const j = display.indexOf("D");
            field = display[1];
            ring = "D";
            multiplier = 2;
        } else if(display.includes("T")) {
            const j = display.indexOf("T");
            field = display[1];
            ring = "T";
            multiplier = 3;
        } else {
            ring = "S";
            field = display[0];
        }

        const t = {
            ring: ring,
            field: field,
            value: field * multiplier
        };

        req.postJson('/addThrow', t, () => {
            update()
            clearDisplay()
        });
    });

    d.getElementById('remove').addEventListener('click', e => {
        display.pop();
        updateDisplay();
    });

    addEventListener("visibilitychange", () => {
        if(!d.hidden && new Date () - lastUpdate > 5000)
            websocketRequest();
    }, false);

    req.get('/state', function(e)
    {
        initGame(e.game);
        initialized = true;
    });

    webS();
});

function webS()
{
	if(ws)
        return;

	let url = `ws://${window.location.hostname}/ws`;
	ws = new WebSocket(url);
	ws.binaryType = "arraybuffer";

	ws.onmessage = (e) => {
		var json = JSON.parse(e.data);
        console.log(json);
	};
	ws.onclose = (e) => {
		ws = null;
        // console.log("closed");
	}
	ws.onopen = (e)=>{
        // console.log("opened");
        websocketRequest();
	}
}

function websocketRequest(cmd)
{
    let request = null;
    var useWs = (ws && ws.readyState === WebSocket.OPEN);

    if (cmd) {
        cmd.d = true;
		cmd.time = Math.floor(Date.now() / 1000);

		request = JSON.stringify(cmd);
		if (request.length > 512) useWs = false; // do not send very long requests over websocket req.length > 1340
	}

    if (useWs) {
        ws.send( request ? request : '{"d":true}' );
        return;
    }

    lastUpdate = new Date();
}

function clearDisplay()
{
    display = [];
    updateDisplay();
}

function updateDisplay()
{
    d.getElementById('display').value = display.join("");
}

function addToDisplay(data)
{
    if(display.length >= 2)
        return;

    display.push(data);
}

function update()
{
    req.get('/state', function(e)
    {
        let game = e.game;
        let players = game.players;

        [...d.querySelectorAll(`.playercard.active`)].map(el => {
            el.classList.remove("active");
        });

        for(const [i, player] of players.entries())
        {
            let html = d.querySelector(`[data-user-id="${player.c}"]`);

            if(game.cp == player.c)
                html.classList.add('active');

            if(player.throws.length > 0)
                html.querySelector(`.playercard-total-points`).innerHTML = game.p - player.throws.reduce((s, i) => s + parseInt(i.v), 0);
        }
    });
}

function initGame(data)
{
    const settings = {
        points: data.p
    }

    for(const [i, player] of data.players.entries()) {
        const p = {
            id: player.c,
            name: player.c,
            points: player.p,
            isActive: player.c === data.cp
        }
        let pc = generatePlayercard(p, settings);
        d.getElementById(`p${i+1}`).innerHTML = pc;
    }

    if(data.players.length == 1)
    {
        d.getElementById("p2").remove();
    }

    if(data.players.length < 3)
    {
        d.getElementById("p34").remove();
    }

    if(data.players.length == 3)
    {
        d.getElementById("p4").remove();
    }
}

function generatePlayercard(player, gameSettings)
{
    return `
    <div class="playercard ${player.isActive ? 'active' : ''}" data-user-id="${player.id}">
        <div class="playercard-body">
            <div class="row" style="margin: 0;align-items: center;">
                <div class="col-a playercard-total-points total">
                    ${player.points}
                </div>
                <div class="col" style="font-size: .85em;padding-right:0;">
                    <div class="row" style="margin: 0;">
                        <div class="col">
                            ${player.name}
                        </div>
                    </div>
                    <div class="row" style="margin: 0;">
                        <div class="col">
                            AVG: <span class="avg">0</span>
                        </div>
                    </div>
                </div>
            </div>
            <div class="row g-1" style="text-align: center">
                <div class="col playercard-throw-count"> 1 </div>
                <div class="col playercard-throw-count"> 2 </div>
                <div class="col playercard-throw-count"> 3 </div>
                <div class="col"  style=" padding: .275rem 0 0 0;">
                    Total
                </div>
            </div>
            <div class="row g-1" style="text-align: center">
                <div class="col fw-bold playercard-throw-points">
                    <span class="throw-1 x">
                        X
                    </span>
                </div>
                <div class="col fw-bold playercard-throw-points">
                    <span class="throw-2 x">
                        X
                    </span>
                </div>
                <div class="col fw-bold playercard-throw-points">
                    <span class="throw-3 x">
                        X
                    </span>
                </div>
                <div class="col fw-bold turn-total">
                    0
                </div>
            </div>
        </div>
    </div>`;
}
