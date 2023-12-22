const d = document;
const maxPlayer = 4;
var initialized = false;
var display = [];
var ws;
var lastUpdate;
var currentPage;
const req = new CRequest();

d.addEventListener('DOMContentLoaded', function ()
{
    const _IEC7064 = new IEC7064();

    // application container
    var app = d.getElementById('app');

    const page_index = d.getElementById('page-index');
    const page_game = d.getElementById('page-game');
    currentPage = page_index;

    const btnAdd = d.getElementById('btn-addPlayer');
    btnAdd.addEventListener('click', e =>
    {
        e.preventDefault();
        const playerList = d.getElementById('playerlist');
        const playerCount = playerList.childElementCount;
        if(playerCount >= maxPlayer) {
            displayError('Max number of player reached');
            return;
        }

        var playerCode = d.getElementById('newPlayerCode').value;
        var playerName = d.getElementById('newPlayerName').value;
        var playerColor = d.querySelector('input[name="color"]:checked').value;
        if(playerCode === "" || playerCode === null || playerName === "" || playerName === null || playerColor === null)
            return;


        if(!_IEC7064.verify(playerCode)){
            displayError('Code is not valid');
            // console.log('Code is not valid: '+ playerCode);
            // console.log(_IEC7064.checksum(playerCode));
            return;
        }

        for(const item of playerList.children)
        {
            if((playerCode+':'+playerName) == item.querySelector('input').value) {
                displayError('Player is already in the list');
                return false;
            }
        }

        // send server request
        websocketRequest({
            ap: {
                c: playerCode,
                n: playerName,
                clr: playerColor
            }
        });

        // let success = true;
        // fallback json api endpoint
        // req.post('/addPlayer', newPlayer, function(e)
        // {
        //     console.log(e);
        //     if(e.message != "ok")
        //         success = false;
        // });

        // if(!success) {
        //     displayError('Server error');
        //     return;
        // }

        const nextPlayerNumber = playerCount + 1;

        const newItem = d.createElement('li');
        newItem.innerHTML = playerName;

        const colorBox = d.createElement('div');
        colorBox.classList.add(playerColor, "colorBox");
        newItem.appendChild(colorBox);

        const newHidden = d.createElement('input');
        newHidden.type = 'hidden';
        newHidden.id = 'hp' + nextPlayerNumber;
        newHidden.name = 'p' + nextPlayerNumber;
        newHidden.value = playerCode + ':' + playerName;
        newItem.appendChild(newHidden);
        playerList.append(newItem);

        // clear inputs
        d.getElementById('newPlayerCode').value = null;
        d.getElementById('newPlayerName').value = null;
        displayError(null);
    });

    [d.getElementById('newPlayerCode'), d.getElementById('newPlayerName')].map(el => {
        el.addEventListener("keypress", e => {
            if(e.key !== "Enter")
                return;

            e.preventDefault();
            btnAdd.click();
        });
    });

    d.getElementById('btn-startGame').addEventListener('click', e => {
        websocketRequest({
            gi: true,
            p: d.getElementById('points').value
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

        websocketRequest({
            th: {
                ring: ring,
                field: field,
                value: field * multiplier
            }
        });
        clearDisplay();
    });

    d.getElementById('remove').addEventListener('click', e => {
        display.pop();
        updateDisplay();
    });

    d.getElementById('abort').addEventListener('click', e => {
        websocketRequest({
            sc: "abort"
        });

        window.location.reload();
    });

    d.addEventListener("visibilitychange", () => {
        if(!d.hidden && new Date () - lastUpdate > 5000)
            websocketRequest();
    }, false);

    webS();
}, false);

function showPage(page)
{
    if(page === undefined)
        return;

    if(page == d.getElementById('page-game') && !initialized)
        return;

    if(page == d.getElementById('page-index') && initialized)
        return;

    let nav = d.querySelector("nav");
    let navActive = nav.querySelector(".active");
    navActive.remove();
    let el = d.createElement("div");
    el.classList.add("active", "text-center");
    el.innerHTML = page.id.replace("page-", "");
    nav.querySelector(`[data-page=${page.id.replace("page-", "")}]`).appendChild(el);

    toggleClass(currentPage, 'hidden');
    toggleClass(page, 'hidden');

    currentPage = page;
}

function displayError(msg)
{
    d.getElementById('errorMsg').innerHTML = msg;
}

function toggleClass(e, c)
{
    e.classList.contains(c) ? e.classList.remove(c) : e.classList.add(c);
}

function webS()
{
	if(ws)
        return;

	let url = `ws://${window.location.hostname}/ws`;
	ws = new WebSocket(url);
	ws.binaryType = "arraybuffer";

	ws.onmessage = (e) => {
		var json = JSON.parse(e.data);

        if(!initialized && json.game.s == "started")
            initGame(json.game);

        if(json.game.s == "running") {
            if(!initialized)
                initialized = true;
            showPage(d.getElementById("page-game"));
            update(json.game);
        }
	};
	ws.onclose = (e) => {
		ws = null;
        // console.log("closed");
	}
	ws.onopen = (e)=>{
        // console.log("opened");
        // websocketRequest();
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

function update(game)
{
    let players = game.players;

    [...d.querySelectorAll(`.playercard.active`)].map(el => {
        el.classList.remove("active");
    });

    for(const [i, player] of players.entries())
    {
        let html = d.querySelector(`[data-user-id="${player.c}"]`);

        if(game.cp == player.c)
            html.classList.add('active');

        html.querySelector(`.playercard-total-points`).innerHTML = player.p;

        for(let j = 0; j <= 2; j++)
            html.querySelector(`.throw-${(j+1)}`).innerHTML = player.throws[j] != undefined ? player.throws[j].v : 'X';
    }
}

function initGame(data)
{
    initialized = true;
    showPage(d.getElementById("page-game"));

    for(const [i, player] of data.players.entries()) {
        const p = {
            id: player.c,
            name: player.n,
            points: player.p,
            isActive: player.c === data.cp
        }
        let pc = generatePlayercard(p);
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

    const btnList = d.querySelectorAll('#numpad button[data-bl-value]');
    [...btnList].map(e => {
        e.addEventListener('click', ev => {
            addToDisplay(e.getAttribute('data-bl-value'));
            updateDisplay();
        });
    });
}

function generatePlayercard(player)
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
