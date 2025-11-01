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

//Função de busca sequencial para encontrar os números dentro do vetor
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
let caixas; // Vetor com as caixas em que os elementos estão "armazenados". Será usado para mudar a cor da borda, indicando que o elemento foi selecionado
let numeros; // Vetor com os <p> em que os elementos serão armazenados
let N = 0; // Variável global para controlar quantos quadrados foram preenchidos com números
let tabelaDeContagem = document.querySelector('div#contagem'); // container da contagem 
let buckets = 0; // Quantidade de buckets
let pop_up = document.querySelector('button#closeModal'); // Modal com o relatório do pruning

// Listener do botão de iniciar stream
document.querySelector('button#startButton').addEventListener('click', function() {
    if(this.classList.contains('turnedOff')) {
        return;
    }
    let delimitadorRandomico = Number.parseInt(buckets * 0.9);
    if (N < buckets) {
        for (let i = 0; i < numeros.length; i++) {
            setTimeout(() => { // Função setTimeOut() proporciona delays
                let numGerado = Math.floor((Math.random() * delimitadorRandomico) + 1);
                numeros[i].textContent = numGerado; // Gerador de 1 a 7

                let index = linearCountTableSearch(numGerado, countTable);

                if (index === -1) {
                    // Item NÃO FOI ENCONTRADO
                    // Crie um novo e adicione ao array
                    let delta = (currentBucket - 1);
                    let newElement = new infoNumber(numGerado, 1, delta);
                    countTable.push(newElement); 
                    updateVisualCountTable(numGerado); 
               } else {
                    // Item FOI ENCONTRADO
                    // Use o 'index' para incrementar o contador
                    countTable[index].addCount();  
                    updateVisualCountTable(numGerado);
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
                if (i === numeros.length - 1) {
                    document.querySelector('button#pruneButton').classList.remove('turnedOff');
                }
            }, i * 1500); // Espera por 1 segundo
        }
    } else {
        alert('Stream cheio. Realize o prune');
    }
})

// Função auxiliar para a criação de um novo elemento
function updateVisualCountTable(highlightNumber = null) {
     // 1. Limpa o contêiner
     tabelaDeContagem.innerHTML = '';

     // 2. Redesenha tudo a partir da fonte de dados (countTable)
     countTable.forEach(element => {
         let newParagraph = document.createElement("p");
         newParagraph.textContent = `(${element.number}, ${element.count}, ${element.delta})`;
         newParagraph.classList.add('auxClass'); // Você pode manter sua classe

        // Adiciona a classe de destaque se o número corresponder
        if (highlightNumber !== null && element.number === highlightNumber) {
            newParagraph.classList.add('highlightSmallText');
        }

         tabelaDeContagem.appendChild(newParagraph);
    });
}

document.querySelector('button#pruneButton').addEventListener('click', function() {
    if(this.classList.contains('turnedOff')) { // Não permite que o botão seja precionado caso ele esteja desativado
        return;
    }
    currentBucket++; // Incrementa o currentBucket 
    document.querySelector('span#bucketCount').textContent = currentBucket;

    // Filtra a countTable
    // A fórmula para remoção é: (element.count + element.delta <= currentBucket)
    // Então, nós mantemos tudo o que for: (element.count + element.delta > currentBucket)
    
    countTable = countTable.filter(element => {
        return (element.count + element.delta) > currentBucket;
    });

    updateVisualCountTable(); // Atualiza a tabela visual para refletir a remoção
    
    N = 0; // Reseta N para que o startButton possa preencher o stream novamente

    // Limpa as caixas do stream visual
    for (let i = 0; i < numeros.length; i++) {
        numeros[i].textContent = '';
        numeros[i].classList.remove('highlightText');
        caixas[i].classList.remove('highlightBox');
        caixas[i].classList.add('element');
    }   
    if(document.querySelector('input#relatorio').checked) {
        document.querySelector('div#modal').classList.remove('hidden');
        document.querySelector('div#modal').classList.add('display');
        document.body.classList.add('grayBackground');
        document.html.classList.add('grayBackground');
    }
    this.classList.add('turnedOff');
});

document.querySelector('input#generateBoxes').addEventListener('click', function(event) {
    event.preventDefault(); // Previne que o botão input resete a página

    if(this.classList.contains('turnedOff')) {
        return;
    }
    let epsilon = Number.parseFloat(document.querySelector('input#epsilonInput').value);
    buckets = Math.ceil(1 / epsilon);

    if (!buckets || buckets <= 0 || buckets === Infinity) { // Validação de erros, caso epsilon for 0, NaN ou inválido
        alert("Por favor, insira um valor válido para ε (ex: 0.1, 0.25)");
        buckets = 0; // Reseta os buckets
        return;
    }
    document.querySelector('p#tamanhoDoBucket').textContent = `Tamanho do bucket = ${buckets}`
    document.querySelector('button#startButton').classList.remove('turnedOff'); // Habilita o botão para iniciar a stream somente depois de validar o valor de epsilon
    for (let i = 0; i < buckets; i++) {
        generateBoxOnStream();
    }
    
    // Atualiza as variáveis globais DEPOIS de criar as caixas
     caixas = document.querySelectorAll('#stream .element');
     numeros = document.getElementsByClassName('number');

     this.classList.add('turnedOff');
});

// Função auxiliar para o listener gerador das caixa
function generateBoxOnStream() {
    let newBox = document.createElement('div');
    let paragraphInsideNewElement = document.createElement('p');

    // Atribuição das classes de cada novo elemento
    newBox.classList.add('element');
    paragraphInsideNewElement.classList.add('number');

    // Coloca o parágrafo dentro da caixa e em seguida coloca tudo dentro da stream
    newBox.appendChild(paragraphInsideNewElement);
    document.querySelector('div#stream').appendChild(newBox);
}   

// Botão de fechar do modal com o relatório do prune
document.querySelector('button#closeModal').addEventListener('click', function() {
    document.querySelector('div#modal').classList.remove('display');
    document.querySelector('div#modal').classList.add('hidden');
    document.body.classList.remove('grayBackground');
});