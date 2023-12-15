const d = document;
const maxPlayer = 4;

d.addEventListener('DOMContentLoaded', function ()
{
    const _IEC7064 = new IEC7064();
    const req = new CRequest();

    // application container
    var app = d.getElementById('app');

    const page_index = d.getElementById('page-index');
    const page_newGame = d.getElementById('page-createGame');
    var currentPage = page_index;

    d.getElementById('btn-newGame').addEventListener('click', e => {
        showPage(page_newGame);
    });

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
        if(playerCode === "" || playerCode === null || playerName === "" || playerName === null)
            return;


        if(!_IEC7064.verify(playerCode)){
            displayError('Code is not valid');
            // console.log('Code is not valid: '+ playerCode);
            // console.log(_IEC7064.checksum(playerCode));
            return;
        }

        if(!addPlayer(playerCode, playerName))
            return;

    });

    [d.getElementById('newPlayerCode'), d.getElementById('newPlayerName')].map(el => {
        el.addEventListener("keypress", e => {
            if(e.key !== "Enter")
                return;

            e.preventDefault();
            btnAdd.click();
        });
    });

    function addPlayer(code, name)
    {
        let success = true;
        let newPlayer = {
            code: code,
            name: name
        };

        const playerList = d.getElementById('playerlist');
        const playerCount = playerList.childElementCount;
        for(const item of playerList.children)
        {
            console.log(newPlayer.code +' - '+ item.querySelector('input').value);
            if((newPlayer.code+':'+newPlayer.name) == item.querySelector('input').value) {
                displayError('Player is already in the list');
                return false;
            }
        }

        req.post('/addPlayer', newPlayer, function(e)
        {
            console.log(e);
            if(e.message != "ok")
                success = false;
        });

        if(!success) {
            displayError('Server error');
            return false;
        }

        const nextPlayerNumber = playerCount + 1;
        const newItem = d.createElement('li');
        newItem.innerHTML = newPlayer.name;
        const newHidden = d.createElement('input');
        newHidden.type = 'hidden';
        newHidden.id = 'p' + nextPlayerNumber;
        newHidden.name = 'p' + nextPlayerNumber;
        newHidden.value = newPlayer.code + ':' + newPlayer.name;
        newItem.appendChild(newHidden);
        playerList.append(newItem);

        // clear inputs
        d.getElementById('newPlayerCode').value = null;
        d.getElementById('newPlayerName').value = null;
        d.getElementById('errorMsg').innerHTML = null;

        return true;
    }

    function showPage(page)
    {
        if(page === undefined)
            return;

        toggleClass(currentPage, 'hidden');
        toggleClass(page, 'hidden');
    }

    function displayError(msg)
    {
        d.getElementById('errorMsg').innerHTML = msg;
    }

}, false);

function toggleClass(e, c)
{
    e.classList.contains(c) ? e.classList.remove(c) : e.classList.add(c);
}
