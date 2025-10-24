import pandas as pd
import os

# --- Configuração de caminho ---
diretorio_script = os.path.dirname(os.path.abspath(__file__))
NOME_ARQUIVO_LIVROS = os.path.join(diretorio_script, 'Livros.csv')

def sistema_busca_livros():
    try:
        # 1. Carregar o arquivo CSV com pandas
        df_livros = pd.read_csv(NOME_ARQUIVO_LIVROS, delimiter=';')

        print("--- Sistema de Busca de Livros ---")
        print("Digite 'sair' para encerrar o programa.")

        while True:
            # 2. Receber a entrada do usuário
            termo_busca = input("\nO que você deseja buscar? (Título, Autor, Editora): ").strip()
            
            if termo_busca.lower() == 'sair':
                break
            
            if not termo_busca:
                print("Por favor, digite um termo de busca.")
                continue
            
            # 3. Filtrar os dados com base no termo de busca
            # A busca não diferencia maiúsculas de minúsculas
            resultados = df_livros[
                df_livros['Titulo'].str.contains(termo_busca, case=False, na=False) |
                df_livros['Autor'].str.contains(termo_busca, case=False, na=False) |
                df_livros['Editora'].str.contains(termo_busca, case=False, na=False)
            ]

            # 4. Exibir os resultados
            if not resultados.empty:
                print(f"\n--- {len(resultados)} resultados encontrados ---")
                # Exibe o DataFrame de resultados de forma amigável
                for index, livro in resultados.iterrows():
                    print("-" * 30)
                    print(f"Título: {livro['Titulo']}")
                    print(f"Autor: {livro['Autor']}")
                    print(f"Editora: {livro['Editora']}")
                    print(f"ISBN: {livro['ISBN']}")
                    print("-" * 30)
            else:
                print("Nenhum livro encontrado com este termo.")

    except FileNotFoundError:
        print(f"Erro: O arquivo '{NOME_ARQUIVO_LIVROS}' não foi encontrado.")
    except Exception as e:
        print(f"Ocorreu um erro: {e}")

# --- Execução da função ---
if __name__ == "__main__":
    sistema_busca_livros()
