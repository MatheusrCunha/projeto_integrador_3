# Projeto Integrador 3
Alunos: Matheus Rodrigues Cunha e Ueslei Marian 

Engenharia Eletrônica 2024/2
## Introdução

No mundo moderno, onde a rotina das pessoas está cada vez mais acelerada e incerta, garantir uma alimentação regular e balanceada para os gatos de estimação se torna um grande desafio. 
Muitos proprietários de gatos enfrentam dificuldades em manter horários consistentes de alimentação devido a compromissos profissionais, viagens ou rotina diária. 

## Objetivos

- Nossa ideia de projeto é desenvolver mecanicamente e eletronicamente um produto cuja finalidade é alimentar gatos automaticamente.
* Na parte eletrônica, vamos desenvolver um produto que seja possível programar até 24 horários de alimentação, permitindo ajustar a quantidade de porções para cada refeição.
+ Essa configuração poderá ser feita diretamente pelo produto ou por um aplicativo nosso que desenvolvemos, onde ele se comunicará por wifi com o produto.
- O microcontrolador será o módulo ESP32, que possui WIFI integrado. 

## Desenvolvimento


# Módulos Escolhidos

|    Módulos       |  Quantidade   |
| -------------    | ------------- |    
| Display LCD 16x2 |       1       |
|     Esp32        |       1       |
|  Servo Motor     |       1       |
| Fonte de 5V/2A   |       1       |
|Botão touch ESP32 |       1       |


# Especificação das tensões:


|    Módulos       |     Tensão    |
| -------------    | ------------- |      
| Display LCD 16x2 |      5V       |
|     ESP32        |   5V ou 3,3V  |
|  Servo Motor     |       5V      |
| Fonte de tensão   |     5V/2A    |



## Referências

A2ROBOTICS. **Display LCD 16x2**. [S.d]. Disponível em:
https://www.a2robotics.com.br/display-lcd-16x2-azul-com-i2c-acoplado?utm_source=Site&utm_medium. Acesso em: 16 mar. 2024.

MAKERHERO. **Servo motor 6221mg**. [S.d]. Disponível em:
https://www.makerhero.com/produto/servo-jx-pdi-6221mg-alto-torque-20kg/. Acesso em: 16 mar. 2024.

FRANZININHO. **Exemplos ESPIDF**. [S.d]. Disponível em:
https://docs.franzininho.com.br/docs/franzininho-wifi/exemplos-espidf/primeiros-passos. Acesso em: 21 mar. 2024.