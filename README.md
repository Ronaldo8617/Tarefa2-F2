# Ohmímetro - BitDogLab (Sistemas Embarcados)

## 📌 Sumário  
- [📹 Demonstração](#-demonstração)  
- [🎯 Objetivo](#-objetivo)  
- [🛠️ Funcionalidades Obrigatórias](#️-funcionalidades-obrigatórias)  
- [✨ Funcionalidades Adicionais](#-funcionalidades-adicionais)  
- [📦 Componentes Utilizados](#-componentes-utilizados)  
- [⚙️ Compilação e Gravação](#️-compilação-e-gravação)  
- [📂 Estrutura do Código](#-estrutura-do-código)  
- [👨‍💻 Autor](#-autor)  

## 📹 Demonstração  
[clique aqui para acessar o vídeo](https://youtu.be/_i7lHrQA3NY)

## 🎯 Objetivo  
Desenvolver um sistema embarcado capaz de medir o valor de um resistor desconhecido utilizando a placa BitDogLab com o microcontrolador RP2040. O valor medido é exibido em um display OLED, junto com as bandas de cores equivalentes, baseando-se nos padrões de resistores comerciais. O projeto ainda permite resetar o sistema através do botão BOOTSEL.  

## 🛠️ Funcionalidades Obrigatórias  
✅ Leitura analógica via ADC para medir a tensão de divisor resistivo.  
✅ Cálculo da resistência desconhecida (Rx) baseado na leitura ADC.  
✅ Arredondamento para o valor comercial mais próximo de resistor.  
✅ Exibição no display OLED: valor do ADC, valor da resistência e bandas de cor.  

## ✨ Funcionalidades Adicionais  
🔹 Sistema de reset via botão BOOTSEL com interrupção GPIO.  
🔹 Arredondamento inteligente conforme tabelas reais de resistores comerciais.  
🔹 Exibição individual de cada banda de cor separadamente no display OLED.  

## 📦 Componentes Utilizados  
- Microcontrolador: RP2040 (BitDogLab)  
- Display: OLED SSD1306 (128x64, I2C)  
- Resistor conhecido: 10kΩ (R_KNOWN)  
- Botão BOOTSEL: Usado para reset do sistema via USB  
- Cabos de conexão e jumpers  

## ⚙️ Compilação e Gravação  
Pelo ambiente do VScode compile e execute na placa de desnvovimento BitDogLab

## 📂 Estrutura do Código
plaintext
Copiar
Editar
projeto-medidor-resistores/  
├── lib/  
│   ├── font.h               # Fonte de caracteres do display OLED  
│   ├── ssd1306.c, h         # Biblioteca de controle para o display SSD1306  
├── CMakeLists.txt           # Arquivo de configuração da build  
├── projeto-medidor-resistores.c  # Código principal (main)  
├── README.md                # Documento de descrição do projeto  

## 👨‍💻 Autor
Nome: Ronaldo César Santos Rocha
GitHub: Ronaldo8617




