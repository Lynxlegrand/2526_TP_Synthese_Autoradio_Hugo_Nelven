# 🎓 TP de Synthèse — Autoradio

## 👥 Équipe

| Nom | Prénom | Groupe |
|:--|:--|:--:|
| THÉBAULT | [Nelven](https://github.com/NelvTheb) | ESE TP1 |
| CORDI | [Hugo](https://github.com/Lynxlegrand) | ESE TP1 |

🏫 **ENSEA — 3A ESE**  
👨‍🏫 **Encadrant :** [L.Fiack](https://github.com/lfiack)  
📅 **Séances de TP :** 
- séance 1 : 13 novembre 2025 
- séance 2 : 21 novembre 2025
- séance 3 :
- séance 4 :

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

## 🧪 Séance 1 — Démarrage

### 🎯 Objectif de la séance
> Initialiser le projet STM32 sur la carte **NUCLEO-L476RG** et mettre en place l’environnement de base nécessaire au développement de l’autoradio :
> - Vérifier le fonctionnement de la LED et de l’USART2  
> - Activer **FreeRTOS** et créer une tâche principale  
> - Intégrer un **shell interactif** accessible via le port série  
> - Préparer la communication SPI pour le GPIO Expander

---

### ⚙️ Mise en place du projet

1. **Création du projet :**
   - Cible : `NUCLEO-L476RG`
   - Langage : `C`
   - IDE : `STM32CubeIDE`
   - Configuration : périphériques par défaut, **sans activer la BSP**

2. **Test LED (LD2) :**
   - LED connectée à la broche **PA5**
   - Fonction de test :
     ```c
     HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
     HAL_Delay(200);
     ```
   - ✅ Résultat : LED clignotante confirmant le bon fonctionnement du GPIO.

3. **Test USART2 (via ST-Link VCP) :**
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

4. **Activation de FreeRTOS :**
   - Middleware activé : **CMSIS v1**
   - Création d’une tâche principale `StartDefaultTask` :
     ```c
     void StartDefaultTask(void *argument)
     {
         for (;;) {
             HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
             osDelay(500);
         }
     }
     ```
   - ✅ Résultat : LED contrôlée par une tâche FreeRTOS (test multitâche réussi).

5. **Mise en place du Shell :**
   - Shell basé sur le projet de référence : [rtos_td_shell](https://github.com/lfiack/rtos_td_shell)
   - Intégration du shell dans une tâche dédiée, avec interruption UART.
   - ✅ Résultat : commandes interactives fonctionnelles via terminal série.

6. **Driver**
   - GPIO_Expancder -> MCP23S17
   - SPI3 utilisé car SCK/MISO/MOSI/CS sur PC10/PC11/PB5/PB7

---
