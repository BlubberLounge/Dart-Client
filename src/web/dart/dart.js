const d = document;
var initialized = false;

/*  */

d.addEventListener('DOMContentLoaded', function ()
{
    const req = new CRequest();

    let gameSettings = {
        points: 301
    };

    let player = {
        id: 1,
        name
    };

    req.get('/state', function(e)
    {
        console.log(e);
        initGame(e.game);
        initialized = true;
    });
});

function initGame(data)
{
    const settings = {
        points: data.points
    }

    for(const [i, player] of data.players.entries()) {
        const p = {
            id: player.code,
            name: player.name
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
                    ${gameSettings.points}
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
