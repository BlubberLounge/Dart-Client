const d = document;
var initialized = false;
const req = new CRequest();
var display = [];

/*  */

d.addEventListener('DOMContentLoaded', function ()
{

    let gameSettings = {
        points: 301
    };

    let player = {
        id: 1,
        name
    };

    const btnList = d.querySelectorAll('#numpad button');
    [...btnList].map(e => {
        e.addEventListener('click', ev => {
            addToDisplay(e.getAttribute('data-bl-value'));
            updateDisplay();
        });
    });

    req.get('/state', function(e)
    {
        initGame(e.game);
        initialized = true;
    });

    setInterval(update, 5000);

});

function updateDisplay()
{
    console.log(display);
    d.getElementById('display').value = display.join("");
}

function addToDisplay(data)
{
    if(display.length >= 4)
        return;

    display.push(data);
}

function update()
{
    req.get('/state', function(e)
    {
        console.log("update");

        let game = e.game;
        let players = game.players;

        for(const [i, player] of players.entries())
        {
            let html = d.querySelector(`[data-user-id="${player.code}"]`);
            if(player.throws.length > 0) {
                console.log(player.throws.reduce((a, b) => a + parseInt(b.value), 0));
                html.querySelector(`.playercard-total-points`).innerHTML = game.points - player.throws.reduce((s, i) => s + parseInt(i.value), 0);
            }
        }
    });
}

function initGame(data)
{
    const settings = {
        points: data.points
    }

    for(const [i, player] of data.players.entries()) {
        const p = {
            id: player.code,
            name: player.name,
            points: player.points
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
    <div class="playercard" data-user-id="${player.id}">
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
