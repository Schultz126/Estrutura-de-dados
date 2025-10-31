let caixas = document.querySelectorAll('#stream .element'); // Vetor com as caixas em que os elementos estão "armazenados". Será usado para mudar a cor da borda, indicando que o elemento foi selecionado
let numeros = document.getElementsByClassName('number'); // Vetor com os <p> em que os elementos serão armazenados
let N = 0; // Variável global para controlar quantos quadrados foram preenchidos com números

document.querySelector('button#startButton').addEventListener('click', function() {
    for (let i = 0; i < numeros.length && i <= 7; i++) {
        setTimeout(() => { // Função setTimeOut() proporciona delays sem que o navegador trave. A função delay() trava o navegador
            numeros[i].textContent = Math.floor((Math.random() * 7) + 1);
            N++;
            numeros[i].classList.add('highlightText');
            caixas[i].classList.add('highlightBox');
            caixas[i].classList.remove('element');
            // Remoção do destaque do elemento anterior
            if (i > 0) {
                numeros[i - 1].classList.remove('highlightText');
                caixas[i - 1].classList.remove('highlightBox');
                caixas[i - 1].classList.add('element');
            }
            if (i === 7 || i === numeros.length - 1) {
                document.querySelector('button#pruneButton').classList.remove('turnedOff');
            }
        }, i * 1000);
    }
})