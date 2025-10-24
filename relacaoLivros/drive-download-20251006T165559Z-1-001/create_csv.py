# Importa as bibliotecas necessárias
import csv
from faker import Faker
import pandas as pd
import os

# -------------------------------------------------------------
# Definição dos nomes dos arquivos
# -------------------------------------------------------------
diretorio_script = os.path.dirname(os.path.abspath(__file__))

# -------------------------------------------------------------
# Definição dos nomes dos arquivos
# -------------------------------------------------------------
# Use os.path.join para construir os caminhos completos dos arquivos
NOME_ARQUIVO_LIVROS = os.path.join(diretorio_script, 'Livros.csv')
NOME_ARQUIVO_EDITORAS = os.path.join(diretorio_script, 'Editoras.csv')
NOME_ARQUIVO_TRADUTORES = os.path.join(diretorio_script, 'Tradutores.csv')

def create_csv_Livros():
    # Inicializa o gerador do Faker para dados em português do Brasil
    fake = Faker('pt_BR')
    registros = []

    # Define os cabeçalhos das colunas
    cabecalhos = ['Titulo', 'Autor', 'Editora', 'ISBN', 'CDD', 'Ilustracoes']

    # Lista de editoras fixas para o exemplo, garantindo editoras únicas no resultado
    editoras_exemplo = [
        'Companhia das Letras', 'Intrínseca', 'Editora Rocco', 'HarperCollins',
        'Zahar', 'Aleph', 'Panini', 'Editora 34', 'Penguin Brasil', 'Record'
        # Adicione mais editoras aqui, quantas desejar
    ]
    # Gera 1000 registros de dados fictícios para livros
    for i in range(1000):
        registro = {
            'Titulo': fake.catch_phrase(),  # Usa uma frase curta para o título
            'Autor': fake.name(),
            'Editora': fake.random_element(editoras_exemplo),  # Usa uma lista de editoras
            'ISBN': fake.isbn13(),  # Gera um ISBN válido (ISBN-13)
            'CDD': fake.numerify(text='###.##'),  # Gera um número no formato ###.## para CDD
            'Ilustracoes': fake.name(), # Usando fake.name() para ilustrador
        }
        registros.append(registro)

    # Abre o arquivo CSV em modo de escrita ('w') e especifica o encoding 'utf-8'
    with open(NOME_ARQUIVO_LIVROS, mode='w', newline='', encoding='utf-8') as arquivo_csv:
        # Crie um escritor de CSV, passando os cabeçalhos definidos explicitamente
        escritor = csv.DictWriter(arquivo_csv, fieldnames=cabecalhos, delimiter=';')
        escritor.writeheader()
        escritor.writerows(registros)
    print(f'Arquivo "{NOME_ARQUIVO_LIVROS}" gerado com sucesso!')

#-----------------------------------------------------
#   Função Para Criar Editoras.csv
#-----------------------------------------------------

def create_files_Editora(nome_arquivo_livros, nome_arquivo_editoras):
    fake = Faker('pt_BR')

    # 1. Lê o arquivo de livros, especificando o delimitador
    df_livros = pd.read_csv(nome_arquivo_livros, delimiter=';')
    editoras_unicas = df_livros['Editora'].unique()  # Pega os nomes únicos da coluna 'Editora'

    # 2. Gera dados falsos para cada editora única
    registros_editoras = []
    for editora in editoras_unicas:
        registro_editora = {
            'Nome_Editora': editora,
            'Endereco': fake.address(),
            'Telefone': fake.phone_number(),
            'Site': fake.url(),
            'Tradutor': fake.name(),
        }
        registros_editoras.append(registro_editora)

    # 3. Salva as informações de cada editora em um novo arquivo CSV
    cabecalhos_editoras = ['Nome_Editora', 'Endereco', 'Telefone', 'Site', 'Tradutor']
    with open(nome_arquivo_editoras, mode='w', newline='', encoding='utf-8') as arquivo_csv:
        escritor = csv.DictWriter(arquivo_csv, fieldnames=cabecalhos_editoras, delimiter=';')
        escritor.writeheader()
        escritor.writerows(registros_editoras)

    print(f"Arquivo '{nome_arquivo_editoras}' gerado com sucesso.")


#-----------------------------------------------------
#   Função Para Criar Editoras.csv
#-----------------------------------------------------
def create_arquivo_Tradutor(nome_arquivo_editoras, nome_arquivo_tradutor):
    fake = Faker('pt_BR')

    # 1. Lê o arquivo de livros, especificando o delimitador
    df_editoras = pd.read_csv(nome_arquivo_editoras, delimiter=';')
    tradutor_unicos = df_editoras['Tradutor'].unique()  # Pega os nomes únicos da coluna 'Tradutor'

    # 2. Gera dados falsos para cada editora única
    registros_tradutor = []
    for tradutor in tradutor_unicos:
        registro_tradutor = {
            'Nome_Tradutor': tradutor,
            'Endereco': fake.address(),
            'Telefone': fake.phone_number(),
        }
        registros_tradutor.append(registro_tradutor)

    # 3. Salva as informações de cada editora em um novo arquivo CSV
    cabecalhos_tradutor = ['Nome_Tradutor', 'Endereco', 'Telefone']
    with open(nome_arquivo_tradutor, mode='w', newline='', encoding='utf-8') as arquivo_csv:
        escritor = csv.DictWriter(arquivo_csv, fieldnames=cabecalhos_tradutor, delimiter=';')
        escritor.writeheader()
        escritor.writerows(registros_tradutor)

    print(f"Arquivo '{nome_arquivo_tradutor}' gerado com sucesso.")


# -------------------------------------------------------------
# Chamada das funções para gerar os arquivos
# -------------------------------------------------------------
if __name__ == "__main__":
    create_csv_Livros()
    create_files_Editora(NOME_ARQUIVO_LIVROS, NOME_ARQUIVO_EDITORAS)
    create_arquivo_Tradutor(NOME_ARQUIVO_EDITORAS, NOME_ARQUIVO_TRADUTORES)
