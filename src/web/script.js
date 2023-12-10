var d = document;
var maxPlayer = 4;

d.addEventListener('DOMContentLoaded', function ()
{
    const _IEC7064 = new IEC7064();

    // application container
    var app = d.getElementById('app');

    const page_index = d.getElementById('page-index');
    const page_newGame = d.getElementById('page-createGame');
    var currentPage = page_index;

    d.getElementById('btn-newGame').addEventListener('click', e => {
        showPage(page_newGame);
    });

    function showPage(page)
    {
        if(page === undefined)
            return;

        toggleClass(currentPage, 'hidden');
        toggleClass(page, 'hidden');
    }

    const btnAdd = d.getElementById('btn-addPlayer');
    btnAdd.addEventListener('click', e =>
    {
        e.preventDefault();
        const playerList = d.getElementById('playerlist');
        const playerCount = playerList.childElementCount;
        if(playerCount >= maxPlayer)
            return;

        const playerCode = d.getElementById('newPlayerCode').value;
        const playerName = d.getElementById('newPlayerName').value;
        if(playerCode === "" || playerCode === null || playerName === "" || playerName === null)
            return;


        if(!_IEC7064.verify(playerCode)){
            console.log('Code is not valid: '+ playerCode);
            console.log(_IEC7064.checksum(playerCode));
            return;
        }

        const nextPlayerNumber = playerCount + 1;
        const newItem = d.createElement('li');
        newItem.innerHTML = playerName;
        const newHidden = d.createElement('input');
        newHidden.type = 'hidden';
        newHidden.id = 'p' + nextPlayerNumber;
        newHidden.name = 'p' + nextPlayerNumber;
        newHidden.value = playerCode + ':' + playerName;
        newItem.appendChild(newHidden);

        playerList.append(newItem);

        // clear inputs
        playerCode.value = null;
        playerName.value = null;
    });

    [d.getElementById('newPlayerCode'), d.getElementById('newPlayerName')].map(el => {
        el.addEventListener("keypress", e => {
            if(e.key !== "Enter")
                return;

            e.preventDefault();
            btnAdd.click();
        });
    });

}, false);

function toggleClass(e, c)
{
    e.classList.contains(c) ? e.classList.remove(c) : e.classList.add(c);
}
