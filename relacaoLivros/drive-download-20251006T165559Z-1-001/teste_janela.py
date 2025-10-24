import pandas as pd
import tkinter as tk
from tkinter import ttk, messagebox
import os

# --- Configuração de caminho ---
diretorio_script = os.path.dirname(os.path.abspath(__file__))
NOME_ARQUIVO_LIVROS = os.path.join(diretorio_script, 'Livros.csv')

# --- Funções do sistema de busca ---
def carregar_dados():
    """Carrega o arquivo CSV com pandas."""
    try:
        df = pd.read_csv(NOME_ARQUIVO_LIVROS, delimiter=';')
        return df
    except FileNotFoundError:
        messagebox.showerror("Erro", f"O arquivo '{NOME_ARQUIVO_LIVROS}' não foi encontrado.")
        return None

def buscar_livros():
    """Busca livros com base no termo inserido pelo usuário e exibe na interface."""
    termo_busca = entrada_busca.get().strip()
    if not termo_busca:
        messagebox.showwarning("Aviso", "Por favor, digite um termo de busca.")
        return

    df_livros = carregar_dados()
    if df_livros is None:
        return

    # Limpa a área de resultados antes de exibir novos
    area_resultados.delete('1.0', tk.END)

    # Filtra os dados (não diferencia maiúsculas/minúsculas)
    resultados = df_livros[
        df_livros['Titulo'].str.contains(termo_busca, case=False, na=False) |
        df_livros['Autor'].str.contains(termo_busca, case=False, na=False) |
        df_livros['Editora'].str.contains(termo_busca, case=False, na=False)
    ]

    # Exibe os resultados na área de texto
    if not resultados.empty:
        area_resultados.insert(tk.END, f"--- {len(resultados)} resultados encontrados ---\n\n")
        for _, livro in resultados.iterrows():
            area_resultados.insert(tk.END, f"Título: {livro['Titulo']}\n")
            area_resultados.insert(tk.END, f"Autor: {livro['Autor']}\n")
            area_resultados.insert(tk.END, f"Editora: {livro['Editora']}\n")
            area_resultados.insert(tk.END, f"ISBN: {livro['ISBN']}\n")
            area_resultados.insert(tk.END, f"Tradutor: {livro['Tradutor']}\n")
            area_resultados.insert(tk.END, "-" * 30 + "\n\n")
    else:
        area_resultados.insert(tk.END, "Nenhum livro encontrado com este termo.")

# --- Criação da janela ---
root = tk.Tk()
root.title("Sistema de Busca de Livros")
root.geometry("600x400")

# --- Widgets da interface ---
frame_busca = ttk.Frame(root, padding="10")
frame_busca.pack(fill="x")

ttk.Label(frame_busca, text="Termo de Busca:").pack(side="left", padx=5)

entrada_busca = ttk.Entry(frame_busca)
entrada_busca.pack(side="left", expand=True, fill="x", padx=5)

botao_buscar = ttk.Button(frame_busca, text="Buscar", command=buscar_livros)
botao_buscar.pack(side="left", padx=5)

# Área de texto para exibir os resultados
area_resultados = tk.Text(root, wrap="word", height=20, width=60)
area_resultados.pack(padx=10, pady=10, fill="both", expand=True)

# --- Execução do loop da janela ---
root.mainloop()

