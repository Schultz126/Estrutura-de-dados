/*
    Fórmulas para o código:
        - current bucket ID = [N / w];
        - δ = current bucket ID - 1;
        - w = ⌈1 / ε⌉;
        - Para remoção: count + δ ≤ current bucket ID
*/ 

class infoNumber { // Classe para a tabela de contagem
    constructor(number, count, delta) {
        this.number = number;
        this.count = count;
        this.delta = delta;
    }

    addCount() { // Incrementa a variável count. Ajuda na legibilidade do código
        this.count++;
    }
};

function linearCountTableSearch(number, array = []) {
    const numToFind = Number.parseInt(number); 
    for (let i = 0; i < array.length; i++) {
        if (numToFind === array[i].number) {
            return i; // Retorna o índice onde o número foi encontrado
        }
    }
    return -1; // Não encontrou
}

let countTable = []; // Vetor para armazenar as instâncias da classe infoNumber
let currentBucket = 1; // current_bucket_ID. Incrementado no pruning
let caixas = document.querySelectorAll('#stream .element'); // Vetor com as caixas em que os elementos estão "armazenados". Será usado para mudar a cor da borda, indicando que o elemento foi selecionado
let numeros = document.getElementsByClassName('number'); // Vetor com os <p> em que os elementos serão armazenados
let N = 0; // Variável global para controlar quantos quadrados foram preenchidos com números
let tabelaDeContagem = document.querySelector('div#contagem'); // container da contagem

document.querySelector('button#startButton').addEventListener('click', function() {
    if (N < 7) {
        for (let i = 0; i < numeros.length && i <= 7; i++) {
            setTimeout(() => { // Função setTimeOut() proporciona delays
                
                let numGerado = Math.floor((Math.random() * 7) + 1);
                numeros[i].textContent = numGerado; // Gerador de 1 a 7

                let index = linearCountTableSearch(numGerado, countTable);

                if (index === -1) {
                    // Item NÃO FOI ENCONTRADO
                    // Crie um novo e adicione ao array
                    let delta = (currentBucket - 1);
                    let newElement = new infoNumber(numGerado, 1, delta);
                    countTable.push(newElement);
                } else {
                    // Item FOI ENCONTRADO
                    // Use o 'index' para incrementar o contador
                    countTable[index].addCount();
                }
                
                N++; // Incrementa N em ambos os casos

                numeros[i].classList.add('highlightText'); // Adiciona e remove classes CSS para deixar a animação dinâmica
                caixas[i].classList.add('highlightBox');
                caixas[i].classList.remove('element');
                
                if (i > 0) { // Remove a partir do índice 1 para que o código não tente acessar índice -1
                    numeros[i - 1].classList.remove('highlightText');
                    caixas[i - 1].classList.remove('highlightBox');
                    caixas[i - 1].classList.add('element');
                }
                if (i === 7 || i === numeros.length - 1) {
                    document.querySelector('button#pruneButton').classList.remove('turnedOff');
                }
            }, i * 1000); // Espera por 1 segundo
        }
    } else {
        alert('Stream cheio. Realize o prune');
    }
})