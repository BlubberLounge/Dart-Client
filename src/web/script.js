document.addEventListener('DOMContentLoaded', function ()
{
    // application container
    var app = document.getElementById('app');

    var page_index = document.getElementById('page-index');
    var page_newGame = document.getElementById('page-newGame');
    var currentPage = page_index;

    document.getElementById('btn-newGame').addEventListener('click', e => {
        showPage(page_newGame);
    });

    function showPage(page)
    {
        if(page === undefined)
            return;

        toggleClass(currentPage, 'hidden');
        toggleClass(page, 'hidden');
    }


}, false);

function toggleClass(e, c)
{
    e.classList.contains(c) ? e.classList.remove(c) : e.classList.add(c);
}
