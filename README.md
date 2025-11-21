# 🎓 TP de Synthèse — Autoradio

## 👥 Équipe

| Nom | Prénom | Groupe |
|:--|:--|:--:|
| THÉBAULT | [Nelven](https://github.com/NelvTheb) | ESE TP1 |
| CORDI | [Hugo](https://github.com/Lynxlegrand) | ESE TP1 |

🏫 **ENSEA — 3A ESE**  
👨‍🏫 **Encadrant :** [L.Fiack](https://github.com/lfiack)  

---

## 🎯 Objectifs du TP

> **But :** Concevoir et analyser un système d’autoradio analogique simplifié comprenant :
>
> - La réception et la démodulation du signal audio  
> - L’amplification du signal de sortie  
> - La gestion de la commande de volume  
>
> Ce TP vise à mettre en œuvre les compétences acquises en électronique analogique et en traitement du signal.
---

## 1. Démarrage

1. Créez un projet pour la carte NUCLEO_L476RG. Initialisez les périphériques avec leur mode par défaut, mais n’activez pas la BSP.
   - Cible : `NUCLEO-L476RG`
   - Langage : `C`
   - IDE : `STM32CubeIDE`
   - Configuration : périphériques par défaut, **sans activer la BSP**

2. Testez la LED LD2.
   - LED connectée à la broche **PA5**
   - Fonction de test :
     ```c
     HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
     HAL_Delay(200);
     ```
   - ✅ Résultat : LED clignotante confirmant le bon fonctionnement du GPIO.

3. Testez l’USART2 connecté à la STLink interne.
   - Broches : `PA2 (TX)` / `PA3 (RX)`
   - Configuration : 115200 bauds
   - Ajout de la redirection pour le printf
     ```c
        int __io_putchar(int chr)
        {
        HAL_UART_Transmit(&huart2, (uint8_t*)&chr,1,HAL_MAX_DELAY);
        return chr;
        }
     ```
   - Envoi d’un message simple :
     ```c
     printf("==== Autoradio Hugo Nelven ====\r\n");
     ```
   - ✅ Résultat : message reçu correctement dans le terminal série.

4. Débrouillez-vous pour que la fonction printf fonctionne.
   - Middleware activé : **CMSIS v1**
   - Création d’une tâche principale `StartDefaultTask` :
     ```c
     void task_led(void * unused)
      {
         for (;;)
         {
            HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
            vTaskDelay(250);
         }
      }
     ```
   - ✅ Résultat : LED contrôlée par une tâche FreeRTOS (test multitâche réussi).

5. Activez FreeRTOS en mode CMSIS V1.
   - Shell basé sur le projet de référence : [rtos_td_shell](https://github.com/lfiack/rtos_td_shell)
   - Intégration du shell dans une tâche dédiée, avec interruption UART et sémaphores et librairie.
   - ✅ Résultat : commandes interactives fonctionnelles via terminal série.

6. Faites fonctionner le shell :
   - (a) Dans une tâche,
     
   GPIO_Expander -> MCP23S17

   SPI3 utilisé car SCK/MISO/MOSI/CS sur PC10/PC11/PB5/PB7

   Commencé à coder le driver du GPIO_Expander

   - (b) En mode interruption,

   - (c) Avec un driver sous forme de structure.

Remarque : Vous pouvez vous aider des codes disponibles sur ce projet github :  
https://github.com/lfiack/rtos_td_shell

---

## 2. Le GPIO Expander et le VU-Metre

### 2.1 Configuration

1. Quelle est la référence du GPIO Expander ? Vous aurez besoin de sa datasheet, téléchargez-la.
2. Sur le STM32, quel SPI est utilisé ?
3. Quels sont les paramètres à configurer dans STM32CubeIDE ?
4. Configurez-les.

### 2.2 Tests

1. Faites clignoter une ou plusieurs LED.
2. Pour toutes les tester, vous pouvez faire un chenillard (par exemple).

### 2.3 Driver

1. Écrivez un driver pour piloter les LED. Utilisez une structure.
2. Écrivez une fonction shell permettant d’allumer ou d’éteindre n’importe quelle LED.

---

## 3. Le CODEC Audio SGTL5000

### 3.1 Configuration préalables

Le CODEC a besoin de deux protocoles de communication :  
— L’I2C pour la configuration,  
— L’I2S pour le transfert des échantillons audio.  

Les configurations suivantes sont à faire sur le logiciel STM32CubeIDE dans la partie graphique CubeMX. Le protocole I2S est géré par le périphérique SAI (Serial Audio Interface).

1. Quelles pins sont utilisées pour l’I2C ? À quel I2C cela correspond dans le STM32 ?
2. Activez l’I2C correspondant, laissez la configuration par défaut.
3. Configurez le SAI2 :
   - SAI A : Master with Master Clock Out,
   - Cochez I2S/PCM protocol,
   - SAI B : Synchronous Slave,
   - Cochez I2S/PCM protocol.

4. Si nécessaire, déplacez les signaux sur les bonnes broches. Vous pouvez déplacer une broche avec un [Ctrl+Clic Gauche]. Les signaux du SAI doivent être connectés au broches suivantes :
   - PB12 : SAI2_FS_A
   - PB13 : SAI2_SCK_A
   - PB14 : SAI2_MCLK_A
   - PB15 : SAI2_SD_A
   - PC12 : SAI2_SD_B

5. Dans l’onglet Clock Configuration, configurez PLLSAI1 pour obtenir la fréquence To SAI2 à 12.235294 MHz.
6. Configurez les blocs SAI A et SAI B de la manière suivante :
7. Activez les interruptions.
8. Configurez le DMA pour le SAI A et le SAI B. Activez le mode circulaire.
9. Avant de passer à la suite, il est nécessaire d’activer l’horloge MCLK pour que le CODEC fonctionne. Pour cela, dans la fonction main(), après les initialisations, ajoutez la ligne suivante :
```c
__HAL_SAI_ENABLE(&hsai_BlockA2);
