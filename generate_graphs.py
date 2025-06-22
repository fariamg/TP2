import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os

def setup_plot_style():
    """Configura o estilo dos gráficos"""
    plt.style.use('default')
    plt.rcParams['figure.figsize'] = (10, 6)
    plt.rcParams['font.size'] = 12
    plt.rcParams['axes.grid'] = True
    plt.rcParams['grid.alpha'] = 0.3

def read_csv_safe(filepath):
    """Lê arquivo CSV de forma segura, lidando com múltiplos cabeçalhos"""
    try:
        # Lê o arquivo linha por linha para encontrar o último cabeçalho
        with open(filepath, 'r') as f:
            lines = f.readlines()
        
        # Encontra a última linha que contém "Input_Size"
        header_line = 0
        for i, line in enumerate(lines):
            if 'Input_Size' in line:
                header_line = i
        
        # Lê dados a partir da última linha de cabeçalho
        df = pd.read_csv(filepath, skiprows=header_line)
        
        # Remove linhas com dados zerados ou inválidos
        df = df[(df['Execution_Time_MS'] > 0) & (df['Total_Packages'] > 0)]
        
        return df
    except Exception as e:
        print(f"Erro ao ler {filepath}: {e}")
        return pd.DataFrame()

def plot_warehouse_scaling():
    """Gera gráfico de escalabilidade por número de armazéns"""
    filepath = 'results/warehouse_scaling_results.csv'
    if not os.path.exists(filepath):
        print(f"Arquivo {filepath} não encontrado")
        return
    
    df = read_csv_safe(filepath)
    if df.empty:
        print("Dados de warehouse scaling não disponíveis")
        return
    
    plt.figure(figsize=(12, 8))
    
    # Subplot 1: Tempo de Execução
    plt.subplot(2, 2, 1)
    x = df['Input_Size']
    y = df['Execution_Time_MS']
    
    plt.plot(x, y, 'bo-', linewidth=2, markersize=8, label='Tempo Experimental')
    
    # Curva teórica O(n²log(n))
    if len(x) > 1:
        x_theory = np.linspace(min(x), max(x), 100)
        y_theory = x_theory * x_theory * np.log(x_theory) / 100
        plt.plot(x_theory, y_theory, 'r--', linewidth=2, label='O(n²log(n))')
    
    plt.xlabel('Número de Armazéns')
    plt.ylabel('Tempo de Execução (ms)')
    plt.title('Escalabilidade por Número de Armazéns')
    plt.legend()
    plt.grid(True, alpha=0.3)
    
    # Subplot 2: Taxa de Entrega
    plt.subplot(2, 2, 2)
    delivery_rate = (df['Delivered_Packages'] / df['Total_Packages']) * 100
    plt.bar(x, delivery_rate, color='green', alpha=0.7)
    plt.xlabel('Número de Armazéns')
    plt.ylabel('Taxa de Entrega (%)')
    plt.title('Taxa de Entrega por Armazéns')
    plt.ylim(0, 105)
    
    # Subplot 3: Rearmazenamentos
    plt.subplot(2, 2, 3)
    plt.plot(x, df['Rearrangements'], 'ro-', linewidth=2, markersize=6)
    plt.xlabel('Número de Armazéns')
    plt.ylabel('Número de Rearmazenamentos')
    plt.title('Rearmazenamentos vs Armazéns')
    
    # Subplot 4: Throughput
    plt.subplot(2, 2, 4)
    plt.plot(x, df['Throughput'], 'go-', linewidth=2, markersize=6)
    plt.xlabel('Número de Armazéns')
    plt.ylabel('Throughput (pacotes/s)')
    plt.title('Throughput vs Armazéns')
    
    plt.tight_layout()
    plt.savefig('results/warehouse_scaling_analysis.png', dpi=300, bbox_inches='tight')
    plt.show()
    print("Gráfico salvo: results/warehouse_scaling_analysis.png")

def plot_package_scaling():
    """Gera gráfico de escalabilidade por número de pacotes"""
    filepath = 'results/package_scaling_results.csv'
    if not os.path.exists(filepath):
        print(f"Arquivo {filepath} não encontrado")
        return
    
    df = read_csv_safe(filepath)
    if df.empty:
        print("Dados de package scaling não disponíveis")
        return
    
    plt.figure(figsize=(12, 8))
    
    # Subplot 1: Tempo de Execução
    plt.subplot(2, 2, 1)
    x = df['Input_Size']
    y = df['Execution_Time_MS']
    
    plt.plot(x, y, 'bo-', linewidth=2, markersize=8, label='Tempo Experimental')
    
    # Curva teórica O(n log(n))
    if len(x) > 1:
        x_theory = np.linspace(min(x), max(x), 100)
        y_theory = x_theory * np.log(x_theory) / 10
        plt.plot(x_theory, y_theory, 'r--', linewidth=2, label='O(n log(n))')
    
    plt.xlabel('Número de Pacotes')
    plt.ylabel('Tempo de Execução (ms)')
    plt.title('Escalabilidade por Número de Pacotes')
    plt.legend()
    plt.grid(True, alpha=0.3)
    
    # Subplot 2: Throughput
    plt.subplot(2, 2, 2)
    plt.plot(x, df['Throughput'], 'go-', linewidth=2, markersize=6)
    plt.xlabel('Número de Pacotes')
    plt.ylabel('Throughput (pacotes/s)')
    plt.title('Throughput vs Pacotes')
    
    # Subplot 3: Rearmazenamentos vs Pacotes
    plt.subplot(2, 2, 3)
    plt.plot(x, df['Rearrangements'], 'ro-', linewidth=2, markersize=6)
    plt.xlabel('Número de Pacotes')
    plt.ylabel('Número de Rearmazenamentos')
    plt.title('Rearmazenamentos vs Pacotes')
    
    # Subplot 4: Eficiência (Pacotes/ms)
    plt.subplot(2, 2, 4)
    efficiency = df['Total_Packages'] / df['Execution_Time_MS']
    plt.plot(x, efficiency, 'mo-', linewidth=2, markersize=6)
    plt.xlabel('Número de Pacotes')
    plt.ylabel('Eficiência (pacotes/ms)')
    plt.title('Eficiência vs Pacotes')
    
    plt.tight_layout()
    plt.savefig('results/package_scaling_analysis.png', dpi=300, bbox_inches='tight')
    plt.show()
    print("Gráfico salvo: results/package_scaling_analysis.png")

def plot_transport_contention():
    """Gera gráfico de análise de contenção por transporte"""
    filepath = 'results/transport_contention_results.csv'
    if not os.path.exists(filepath):
        print(f"Arquivo {filepath} não encontrado")
        return
    
    df = read_csv_safe(filepath)
    if df.empty:
        print("Dados de transport contention não disponíveis")
        return
    
    plt.figure(figsize=(12, 6))
    
    # Subplot 1: Rearmazenamentos vs Tempo
    plt.subplot(1, 2, 1)
    x = df['Rearrangements']
    y = df['Execution_Time_MS']
    
    plt.scatter(x, y, s=100, alpha=0.7, c=range(len(x)), cmap='viridis')
    plt.xlabel('Número de Rearmazenamentos')
    plt.ylabel('Tempo de Execução (ms)')
    plt.title('Contenção: Rearmazenamentos vs Tempo')
    
    # Adiciona linha de tendência
    if len(x) > 1:
        z = np.polyfit(x, y, 1)
        p = np.poly1d(z)
        plt.plot(x, p(x), "r--", alpha=0.8)
    
    # Subplot 2: Configurações de Contenção
    plt.subplot(1, 2, 2)
    
    # Cria gráfico de barras para diferentes configurações
    configs = range(len(df))
    plt.bar(configs, df['Rearrangements'], alpha=0.7, color='red', label='Rearmazenamentos')
    plt.bar(configs, df['Transport_Events']/10, alpha=0.7, color='blue', label='Eventos Transp./10')
    
    plt.xlabel('Configuração de Teste')
    plt.ylabel('Quantidade')
    plt.title('Comparação de Métricas por Configuração')
    plt.legend()
    plt.xticks(configs, [f'Config {i+1}' for i in configs])
    
    plt.tight_layout()
    plt.savefig('results/transport_contention_analysis.png', dpi=300, bbox_inches='tight')
    plt.show()
    print("Gráfico salvo: results/transport_contention_analysis.png")

def plot_comparative_analysis():
    """Gera gráfico comparativo entre todos os experimentos"""
    
    # Lê todos os dados disponíveis
    warehouse_df = read_csv_safe('results/warehouse_scaling_results.csv')
    package_df = read_csv_safe('results/package_scaling_results.csv')
    transport_df = read_csv_safe('results/transport_contention_results.csv')
    
    plt.figure(figsize=(15, 10))
    
    # Subplot 1: Comparação de Tempos Normalizados
    plt.subplot(2, 3, 1)
    if not warehouse_df.empty:
        x1 = warehouse_df['Input_Size']
        y1 = warehouse_df['Execution_Time_MS'] / warehouse_df['Total_Packages']
        plt.plot(x1, y1, 'bo-', label='Por Armazém', markersize=6)
    
    if not package_df.empty:
        x2 = package_df['Input_Size']
        y2 = package_df['Execution_Time_MS'] / package_df['Total_Packages']
        plt.plot(x2, y2, 'ro-', label='Por Pacote', markersize=6)
    
    plt.xlabel('Tamanho da Entrada')
    plt.ylabel('Tempo por Pacote (ms)')
    plt.title('Tempo Normalizado por Pacote')
    plt.legend()
    plt.grid(True, alpha=0.3)
    
    # Subplot 2: Comparação de Eficiência
    plt.subplot(2, 3, 2)
    if not warehouse_df.empty:
        eff1 = warehouse_df['Throughput']
        plt.bar(range(len(eff1)), eff1, alpha=0.7, label='Warehouse Scaling')
    
    if not package_df.empty:
        eff2 = package_df['Throughput']
        plt.bar(range(len(eff2)), eff2, alpha=0.7, label='Package Scaling')
    
    plt.ylabel('Throughput (pacotes/s)')
    plt.title('Comparação de Throughput')
    plt.legend()
    
    # Subplot 3: Taxa de Entrega Geral
    plt.subplot(2, 3, 3)
    datasets = []
    labels = []
    
    if not warehouse_df.empty:
        rate1 = (warehouse_df['Delivered_Packages'] / warehouse_df['Total_Packages'] * 100).mean()
        datasets.append(rate1)
        labels.append('Warehouse')
    
    if not package_df.empty:
        rate2 = (package_df['Delivered_Packages'] / package_df['Total_Packages'] * 100).mean()
        datasets.append(rate2)
        labels.append('Package')
    
    if not transport_df.empty:
        rate3 = (transport_df['Delivered_Packages'] / transport_df['Total_Packages'] * 100).mean()
        datasets.append(rate3)
        labels.append('Transport')
    
    if datasets:
        plt.bar(labels, datasets, color=['blue', 'red', 'green'][:len(datasets)])
        plt.ylabel('Taxa de Entrega Média (%)')
        plt.title('Taxa de Entrega por Experimento')
        plt.ylim(0, 105)
    
    # Subplot 4: Rearmazenamentos Totais
    plt.subplot(2, 3, 4)
    rearr_data = []
    rearr_labels = []
    
    if not warehouse_df.empty:
        rearr_data.append(warehouse_df['Rearrangements'].sum())
        rearr_labels.append('Warehouse')
    
    if not package_df.empty:
        rearr_data.append(package_df['Rearrangements'].sum())
        rearr_labels.append('Package')
    
    if not transport_df.empty:
        rearr_data.append(transport_df['Rearrangements'].sum())
        rearr_labels.append('Transport')
    
    if rearr_data:
        plt.bar(rearr_labels, rearr_data, color=['blue', 'red', 'green'][:len(rearr_data)])
        plt.ylabel('Total de Rearmazenamentos')
        plt.title('Rearmazenamentos por Experimento')
    
    # Subplot 5: Complexidade Observada (log-log plot)
    plt.subplot(2, 3, 5)
    if not package_df.empty and len(package_df) > 1:
        x = package_df['Input_Size']
        y = package_df['Execution_Time_MS']
        plt.loglog(x, y, 'bo-', label='Dados Experimentais')
        
        # Linha teórica n*log(n)
        x_theory = np.linspace(min(x), max(x), 100)
        y_theory = x_theory * np.log(x_theory) / 100
        plt.loglog(x_theory, y_theory, 'r--', label='O(n log n)')
        
        plt.xlabel('Número de Pacotes (log)')
        plt.ylabel('Tempo (ms) (log)')
        plt.title('Análise de Complexidade')
        plt.legend()
        plt.grid(True, alpha=0.3)
    
    # Subplot 6: Resumo de Performance
    plt.subplot(2, 3, 6)
    
    summary_data = []
    summary_labels = []
    
    if not warehouse_df.empty:
        avg_time_warehouse = warehouse_df['Execution_Time_MS'].mean()
        summary_data.append(avg_time_warehouse)
        summary_labels.append('Warehouse\nScaling')
    
    if not package_df.empty:
        avg_time_package = package_df['Execution_Time_MS'].mean()
        summary_data.append(avg_time_package)
        summary_labels.append('Package\nScaling')
    
    if not transport_df.empty:
        avg_time_transport = transport_df['Execution_Time_MS'].mean()
        summary_data.append(avg_time_transport)
        summary_labels.append('Transport\nContention')
    
    if summary_data:
        bars = plt.bar(summary_labels, summary_data, color=['skyblue', 'lightcoral', 'lightgreen'][:len(summary_data)])
        plt.ylabel('Tempo Médio (ms)')
        plt.title('Resumo de Performance')
        
        # Adiciona valores nas barras
        for bar, value in zip(bars, summary_data):
            plt.text(bar.get_x() + bar.get_width()/2, bar.get_height() + 0.1,
                    f'{value:.1f}', ha='center', va='bottom')
    
    plt.tight_layout()
    plt.savefig('results/comparative_analysis.png', dpi=300, bbox_inches='tight')
    plt.show()
    print("Gráfico salvo: results/comparative_analysis.png")

def main():
    """Função principal que gera todos os gráficos"""
    print("=== GERADOR DE GRÁFICOS - ANÁLISE EXPERIMENTAL ===")
    print()
    
    # Verifica se o diretório results existe
    if not os.path.exists('results'):
        print("Diretório 'results' não encontrado!")
        print("Execute primeiro 'make analysis' para gerar os dados.")
        return
    
    # Configura estilo dos gráficos
    setup_plot_style()
    
    print("Gerando gráficos...")
    
    # Gera todos os gráficos
    print("1. Análise de escalabilidade por armazéns...")
    plot_warehouse_scaling()
    
    print("2. Análise de escalabilidade por pacotes...")
    plot_package_scaling()
    
    print("3. Análise de contenção por transporte...")
    plot_transport_contention()
    
    print("4. Análise comparativa geral...")
    plot_comparative_analysis()
    
    print()
    print("=== GRÁFICOS GERADOS COM SUCESSO ===")
    print("Arquivos salvos em:")
    print("- results/warehouse_scaling_analysis.png")
    print("- results/package_scaling_analysis.png") 
    print("- results/transport_contention_analysis.png")
    print("- results/comparative_analysis.png")
    print()
    print("Abra os arquivos PNG para visualizar os resultados!")

if __name__ == "__main__":
    main()