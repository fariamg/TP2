import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import seaborn as sns

def generate_extended_analysis_graphs():
    """Gera gráficos comparativos entre versão base e estendida"""
    
    # Dados simulados dos resultados da análise
    data = {
        'Experiment': [
            'Package_Scaling_100', 'Package_Scaling_200', 'Package_Scaling_300', 'Package_Scaling_400', 'Package_Scaling_500',
            'Transport_Contention_baixa', 'Transport_Contention_media', 'Transport_Contention_alta', 'Transport_Contention_maxima',
            'Warehouse_Scaling_5', 'Warehouse_Scaling_10', 'Warehouse_Scaling_15', 'Warehouse_Scaling_20', 'Warehouse_Scaling_25',
            'Variable_Weight', 'Limited_Capacity', 'Multiple_Routes'
        ],
        'Base_Time': [150, 280, 450, 680, 950, 320, 480, 720, 1100, 180, 350, 520, 750, 1050, 420, 580, 380],
        'Extended_Time': [120, 210, 320, 480, 650, 250, 350, 520, 780, 160, 290, 420, 580, 800, 300, 400, 280],
        'Improvement_%': [20.0, 25.0, 28.9, 29.4, 31.6, 21.9, 27.1, 27.8, 29.1, 11.1, 17.1, 19.2, 22.7, 23.8, 28.6, 31.0, 26.3],
        'Route_Recalculations': [0, 0, 0, 0, 0, 15, 15, 15, 15, 0, 0, 0, 0, 0, 0, 0, 20],
        'Capacity_Overflows': [0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 0, 25, 0],
        'Weight_Overflows': [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 8, 0],
        'Alternative_Routes': [0, 0, 0, 0, 0, 8, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 35]
    }
    
    df = pd.DataFrame(data)
    
    # Configuração do estilo
    plt.style.use('seaborn-v0_8')
    sns.set_palette("husl")
    
    # Gráfico 1: Comparação de tempos por experimento
    plt.figure(figsize=(15, 8))
    x = np.arange(len(df))
    width = 0.35
    
    plt.bar(x - width/2, df['Base_Time'], width, label='Versão Base', alpha=0.8)
    plt.bar(x + width/2, df['Extended_Time'], width, label='Versão Estendida', alpha=0.8)
    
    plt.xlabel('Experimentos')
    plt.ylabel('Tempo de Execução (ms)')
    plt.title('Comparação de Performance: Versão Base vs Estendida')
    plt.xticks(x, df['Experiment'], rotation=45, ha='right')
    plt.legend()
    plt.tight_layout()
    plt.savefig('results/extended_performance_comparison.png', dpi=300, bbox_inches='tight')
    plt.close()
    
    # Gráfico 2: Melhorias percentuais
    plt.figure(figsize=(15, 6))
    colors = ['#FF6B6B', '#4ECDC4', '#45B7D1', '#96CEB4', '#FFEAA7']
    
    # Separar por tipo de experimento
    package_exp = df[df['Experiment'].str.contains('Package_Scaling')]
    transport_exp = df[df['Experiment'].str.contains('Transport_Contention')]
    warehouse_exp = df[df['Experiment'].str.contains('Warehouse_Scaling')]
    feature_exp = df[~df['Experiment'].str.contains('Scaling|Contention')]
    
    plt.bar(range(len(package_exp)), package_exp['Improvement_%'], 
           color=colors[0], alpha=0.8, label='Escalabilidade de Pacotes')
    plt.bar(range(len(package_exp), len(package_exp) + len(transport_exp)), 
           transport_exp['Improvement_%'], color=colors[1], alpha=0.8, label='Contenção de Transporte')
    plt.bar(range(len(package_exp) + len(transport_exp), 
                 len(package_exp) + len(transport_exp) + len(warehouse_exp)), 
           warehouse_exp['Improvement_%'], color=colors[2], alpha=0.8, label='Escalabilidade de Armazéns')
    plt.bar(range(len(package_exp) + len(transport_exp) + len(warehouse_exp), len(df)), 
           feature_exp['Improvement_%'], color=colors[3], alpha=0.8, label='Funcionalidades Especiais')
    
    plt.xlabel('Experimentos')
    plt.ylabel('Melhoria Percentual (%)')
    plt.title('Melhorias de Performance por Categoria de Experimento')
    plt.xticks(range(len(df)), df['Experiment'], rotation=45, ha='right')
    plt.legend()
    plt.grid(axis='y', alpha=0.3)
    plt.tight_layout()
    plt.savefig('results/extended_improvement_analysis.png', dpi=300, bbox_inches='tight')
    plt.close()
    
    # Gráfico 3: Análise de funcionalidades extras
    plt.figure(figsize=(12, 8))
    
    # Subplots para diferentes métricas
    fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=(15, 12))
    
    # Recalculações de rota
    route_data = df[df['Route_Recalculations'] > 0]
    ax1.bar(route_data['Experiment'], route_data['Route_Recalculations'], color='#FF6B6B', alpha=0.8)
    ax1.set_title('Recalculações de Rota (Roteamento Dinâmico)')
    ax1.set_ylabel('Número de Recalculações')
    ax1.tick_params(axis='x', rotation=45)
    
    # Overflows de capacidade
    capacity_data = df[df['Capacity_Overflows'] > 0]
    ax2.bar(capacity_data['Experiment'], capacity_data['Capacity_Overflows'], color='#4ECDC4', alpha=0.8)
    ax2.set_title('Overflows de Capacidade (Armazenamento Limitado)')
    ax2.set_ylabel('Número de Overflows')
    ax2.tick_params(axis='x', rotation=45)
    
    # Overflows de peso
    weight_data = df[df['Weight_Overflows'] > 0]
    ax3.bar(weight_data['Experiment'], weight_data['Weight_Overflows'], color='#45B7D1', alpha=0.8)
    ax3.set_title('Overflows de Peso (Peso Variável)')
    ax3.set_ylabel('Número de Overflows')
    ax3.tick_params(axis='x', rotation=45)
    
    # Rotas alternativas utilizadas
    alt_route_data = df[df['Alternative_Routes'] > 0]
    ax4.bar(alt_route_data['Experiment'], alt_route_data['Alternative_Routes'], color='#96CEB4', alpha=0.8)
    ax4.set_title('Rotas Alternativas Utilizadas (Múltiplas Rotas)')
    ax4.set_ylabel('Número de Rotas Alternativas')
    ax4.tick_params(axis='x', rotation=45)
    
    plt.tight_layout()
    plt.savefig('results/extended_features_analysis.png', dpi=300, bbox_inches='tight')
    plt.close()
    
    # Gráfico 4: Análise de escalabilidade comparativa
    plt.figure(figsize=(14, 10))
    
    # Escalabilidade de pacotes
    # filepath: /home/gabriel-faria/Documentos/UFMG/EDD/TP2/generate_extended_graphs.py
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import seaborn as sns

def generate_extended_analysis_graphs():
    """Gera gráficos comparativos entre versão base e estendida"""
    
    # Dados simulados dos resultados da análise
    data = {
        'Experiment': [
            'Package_Scaling_100', 'Package_Scaling_200', 'Package_Scaling_300', 'Package_Scaling_400', 'Package_Scaling_500',
            'Transport_Contention_baixa', 'Transport_Contention_media', 'Transport_Contention_alta', 'Transport_Contention_maxima',
            'Warehouse_Scaling_5', 'Warehouse_Scaling_10', 'Warehouse_Scaling_15', 'Warehouse_Scaling_20', 'Warehouse_Scaling_25',
            'Variable_Weight', 'Limited_Capacity', 'Multiple_Routes'
        ],
        'Base_Time': [150, 280, 450, 680, 950, 320, 480, 720, 1100, 180, 350, 520, 750, 1050, 420, 580, 380],
        'Extended_Time': [120, 210, 320, 480, 650, 250, 350, 520, 780, 160, 290, 420, 580, 800, 300, 400, 280],
        'Improvement_%': [20.0, 25.0, 28.9, 29.4, 31.6, 21.9, 27.1, 27.8, 29.1, 11.1, 17.1, 19.2, 22.7, 23.8, 28.6, 31.0, 26.3],
        'Route_Recalculations': [0, 0, 0, 0, 0, 15, 15, 15, 15, 0, 0, 0, 0, 0, 0, 0, 20],
        'Capacity_Overflows': [0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 0, 25, 0],
        'Weight_Overflows': [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 8, 0],
        'Alternative_Routes': [0, 0, 0, 0, 0, 8, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 35]
    }
    
    df = pd.DataFrame(data)
    
    # Configuração do estilo
    plt.style.use('seaborn-v0_8')
    sns.set_palette("husl")
    
    # Gráfico 1: Comparação de tempos por experimento
    plt.figure(figsize=(15, 8))
    x = np.arange(len(df))
    width = 0.35
    
    plt.bar(x - width/2, df['Base_Time'], width, label='Versão Base', alpha=0.8)
    plt.bar(x + width/2, df['Extended_Time'], width, label='Versão Estendida', alpha=0.8)
    
    plt.xlabel('Experimentos')
    plt.ylabel('Tempo de Execução (ms)')
    plt.title('Comparação de Performance: Versão Base vs Estendida')
    plt.xticks(x, df['Experiment'], rotation=45, ha='right')
    plt.legend()
    plt.tight_layout()
    plt.savefig('results/extended_performance_comparison.png', dpi=300, bbox_inches='tight')
    plt.close()
    
    # Gráfico 2: Melhorias percentuais
    plt.figure(figsize=(15, 6))
    colors = ['#FF6B6B', '#4ECDC4', '#45B7D1', '#96CEB4', '#FFEAA7']
    
    # Separar por tipo de experimento
    package_exp = df[df['Experiment'].str.contains('Package_Scaling')]
    transport_exp = df[df['Experiment'].str.contains('Transport_Contention')]
    warehouse_exp = df[df['Experiment'].str.contains('Warehouse_Scaling')]
    feature_exp = df[~df['Experiment'].str.contains('Scaling|Contention')]
    
    plt.bar(range(len(package_exp)), package_exp['Improvement_%'], 
           color=colors[0], alpha=0.8, label='Escalabilidade de Pacotes')
    plt.bar(range(len(package_exp), len(package_exp) + len(transport_exp)), 
           transport_exp['Improvement_%'], color=colors[1], alpha=0.8, label='Contenção de Transporte')
    plt.bar(range(len(package_exp) + len(transport_exp), 
                 len(package_exp) + len(transport_exp) + len(warehouse_exp)), 
           warehouse_exp['Improvement_%'], color=colors[2], alpha=0.8, label='Escalabilidade de Armazéns')
    plt.bar(range(len(package_exp) + len(transport_exp) + len(warehouse_exp), len(df)), 
           feature_exp['Improvement_%'], color=colors[3], alpha=0.8, label='Funcionalidades Especiais')
    
    plt.xlabel('Experimentos')
    plt.ylabel('Melhoria Percentual (%)')
    plt.title('Melhorias de Performance por Categoria de Experimento')
    plt.xticks(range(len(df)), df['Experiment'], rotation=45, ha='right')
    plt.legend()
    plt.grid(axis='y', alpha=0.3)
    plt.tight_layout()
    plt.savefig('results/extended_improvement_analysis.png', dpi=300, bbox_inches='tight')
    plt.close()
    
    # Gráfico 3: Análise de funcionalidades extras
    plt.figure(figsize=(12, 8))
    
    # Subplots para diferentes métricas
    fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=(15, 12))
    
    # Recalculações de rota
    route_data = df[df['Route_Recalculations'] > 0]
    ax1.bar(route_data['Experiment'], route_data['Route_Recalculations'], color='#FF6B6B', alpha=0.8)
    ax1.set_title('Recalculações de Rota (Roteamento Dinâmico)')
    ax1.set_ylabel('Número de Recalculações')
    ax1.tick_params(axis='x', rotation=45)
    
    # Overflows de capacidade
    capacity_data = df[df['Capacity_Overflows'] > 0]
    ax2.bar(capacity_data['Experiment'], capacity_data['Capacity_Overflows'], color='#4ECDC4', alpha=0.8)
    ax2.set_title('Overflows de Capacidade (Armazenamento Limitado)')
    ax2.set_ylabel('Número de Overflows')
    ax2.tick_params(axis='x', rotation=45)
    
    # Overflows de peso
    weight_data = df[df['Weight_Overflows'] > 0]
    ax3.bar(weight_data['Experiment'], weight_data['Weight_Overflows'], color='#45B7D1', alpha=0.8)
    ax3.set_title('Overflows de Peso (Peso Variável)')
    ax3.set_ylabel('Número de Overflows')
    ax3.tick_params(axis='x', rotation=45)
    
    # Rotas alternativas utilizadas
    alt_route_data = df[df['Alternative_Routes'] > 0]
    ax4.bar(alt_route_data['Experiment'], alt_route_data['Alternative_Routes'], color='#96CEB4', alpha=0.8)
    ax4.set_title('Rotas Alternativas Utilizadas (Múltiplas Rotas)')
    ax4.set_ylabel('Número de Rotas Alternativas')
    ax4.tick_params(axis='x', rotation=45)
    
    plt.tight_layout()
    plt.savefig('results/extended_features_analysis.png', dpi=300, bbox_inches='tight')
    plt.close()
    
    # Gráfico 4: Análise de escalabilidade comparativa
    plt.figure(figsize=(14, 10))
    
    # Escalabilidade de