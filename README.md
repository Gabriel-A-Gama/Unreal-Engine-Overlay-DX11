# Lockdown Protocol - Estudo de Overlay UE5 DX11 + ImGui
## IMPORTANTE!!!!
Dentro do projeto tem uma pasta zipada com nome de SDK.ZIP é necessário extair essa pasta para a compilação do projeto
---
## Sobre o Projeto

Este projeto é um estudo educacional desenvolvido em **C++** com foco em entender como funcionam:

- Overlays em aplicações DirectX 11
- Renderização em tempo real
- Interfaces com ImGui
- Estruturas internas da Unreal Engine
- Leitura de atores e objetos do jogo
- Hooks gráficos utilizando Kiero

O projeto foi criado utilizando o jogo **Lockdown Protocol** apenas como ambiente de testes para fins de aprendizado e pesquisa.

---

## Objetivo Educacional

O principal objetivo deste repositório é estudar conceitos relacionados a:

- Funcionamento interno da Unreal Engine
- Pipeline de renderização
- Hooks em DirectX 11
- Criação de menus em overlay
- Interfaces em tempo real com ImGui
- Projeção de objetos na tela
- Manipulação e leitura de atores da engine
- Estruturas SDK geradas da Unreal Engine

Este projeto foi desenvolvido apenas para:

- Estudos
- Pesquisa
- Aprendizado técnico
- Engenharia reversa educacional
- Compreensão de renderização gráfica

---

## Tecnologias Utilizadas

### Linguagens e APIs

- C++
- WinAPI
- DirectX 11

### Bibliotecas

- **Kiero**
  - Utilizado para realizar hook no DirectX 11 e interceptar a renderização

- **ImGui**
  - Utilizado para criar o menu e interface em overlay

- **SDK Unreal Engine**
  - Utilizado para acessar atores, objetos e estruturas internas do jogo

---

# Sistema de Menu DX11 + ImGui

O projeto possui um menu em overlay desenvolvido utilizando:

- DirectX 11
- ImGui
- Hook de renderização via Kiero

O menu conta com:

- Sistema de abas
- Interface customizada
- Efeitos visuais e partículas
- Sistema de cores customizáveis
- Configuração em tempo real
- Sistema de atalhos via teclado
- Estrutura modular para estudos de UI

---

# Funcionalidades Presentes no Menu

## Visual Player

Sistema de visualização de jogadores contendo:

- Enable ESP
- ESP Line
- ESP Name
- ESP Impostor
- ESP Distance

Também possui:

- Customização de cores
- Alteração de cores do ESP em tempo real

---

## ESP World

Sistema de visualização de objetos e tarefas do mapa:

### ESP Itens

- ESP Dropped Item

### ESP Tasks

- ESP Gas Bottle
- ESP Battery
- ESP Vents
- ESP Container
- ESP Scanner
- ESP Delivery
- ESP Alimbox
- ESP ScrewDriver
- ESP Vents Filter

---

## Misc

Sistema misc contendo:

- Infinite Stamina
- Change Player Color

---

## Sistema de Atalhos

O menu possui suporte a atalhos via teclado:

- F2 → Respawn
- F3 → Detonate C4
- F4 → Spawn Pistol
- F5 → Spawn C4
- F9 → Spawn Pistol/Revolver
- F10 → C4 Actions
- HOME → Clear Saved Items
- Setas Direita/Esquerda → Alteração de cor

---

## Conceitos Técnicos Explorados

O projeto também explora conceitos como:

- Hooking em DX11
- Renderização em overlay
- Manipulação de renderização
- Estruturas internas da Unreal Engine
- Processamento de input
- World to Screen
- Runtime rendering
- Actor iteration
- Overlay drawing
- Particle rendering
- UI styling com ImGui

---

## Estrutura Geral

O projeto contém estudos relacionados a:

- Hooks gráficos
- Overlay em DirectX 11
- Menu ImGui
- Sistemas de renderização
- Utilitários de desenho
- Interação com Unreal Engine
- Gerenciamento de atores

---

## Aviso Importante

Este repositório foi criado **exclusivamente para fins educacionais e de pesquisa**.

O objetivo do projeto é apenas estudar:

- Funcionamento da Unreal Engine
- Overlays gráficos
- Hooks em DirectX 11
- Interfaces em tempo real
- Conceitos de engenharia reversa
- Estruturas internas de jogos

O autor não incentiva o uso indevido do código em ambientes online, competitivos ou multiplayer.

Utilize apenas em ambientes controlados e para aprendizado técnico.

---

## Observações

Alguns trechos do projeto demonstram:

- Inicialização de hooks
- Sistemas de overlay
- Renderização em tempo real
- Integração entre DX11 e ImGui
- Acesso a atores da Unreal Engine
- Estruturas internas do jogo

Este repositório deve ser tratado apenas como material de estudo técnico.
